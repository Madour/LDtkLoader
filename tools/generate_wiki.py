# Created by Modar Nasser on 08/06/2024

from dataclasses import dataclass
from os import listdir, makedirs
from os import path
from typing import Any, Optional, Union

import yaml

###############################################################################
# YAML parsing                                                                #
###############################################################################

class YamlObj:
    def __init__(self, *args, **kwargs):
        for k, v in kwargs.items():
            setattr(self, k, yaml2obj(v))
    def __iter__(self):
        for k, v in self.__dict__.items():
            yield k, v


def yaml2obj(val) -> YamlObj:
    if isinstance(val, dict):
        return YamlObj(**val)
    elif isinstance(val, list):
        return [yaml2obj(e) for e in val]
    elif isinstance(val, str):
        return val.strip()
    else:
        return val


###############################################################################
# YAML deserialization                                                        #
###############################################################################

@dataclass
class DocumentedType:
    name: str
    desc: str
    page: 'Page'


@dataclass
class Member:
    decl: str
    desc: str
    cls: 'Class'


@dataclass
class Field(Member):
    pass


@dataclass
class Func(Member):
    pass


@dataclass
class Class(DocumentedType):
    ctor: list[Func]
    fields: dict[str, Field]
    methods: dict[str, list[Func]]
    parent: Optional['Class']

    @property
    def methods_count(self):
        return len(self.methods) + (len(self.parent.methods) if self.parent else 0)


@dataclass
class Struct(Class):
    pass


@dataclass
class Enum(DocumentedType):
    values: dict[int, str]


@dataclass
class Type(DocumentedType):
    decl: str


@dataclass
class Page:
    title: str
    filename: str
    classes: list[Class]
    structs: list[Struct]
    enums: list[Enum]
    types: list[Type]

    @property
    def documented_types_count(self):
        return len(self.classes) + len(self.structs) + len(self.enums) + len(self.types)


def deserialize_yaml_obj(data: YamlObj) -> Page:
    page = Page(
        title=data.title,
        filename=data.filename,
        classes=[],
        structs=[],
        enums=[],
        types=[]
    )

    def _deserialize_class_or_struct(obj: YamlObj, typename: type) -> Union[Class, Struct]:
        c = typename(
            name=obj.name,
            desc=obj.desc,
            page=page,
            ctor=[],
            fields={},
            methods={},
            parent=None
        )

        if hasattr(obj, 'parent'):
            c.parent = obj.parent

        if hasattr(obj, 'ctor'):
            for ctor in obj.ctor:
                c.ctor.append(Func(decl=ctor.decl, desc=ctor.desc, cls=c))

        if hasattr(obj, 'methods'):
            for name, funcs in obj.methods:
                c.methods[name] = []
                for func in funcs:
                    c.methods[name].append(Func(decl=func.decl, desc=func.desc, cls=c))

        if hasattr(obj, 'fields'):
            for name, field in obj.fields:
                c.fields[name] = Field(decl=field.decl, desc=field.desc, cls=c)

        return c

    def _deserialize_enum(obj: YamlObj) -> Enum:
        return Enum(
            name=obj.name,
            desc=obj.desc,
            page=page,
            values={val: name for name, val in obj.values}
        )

    def _deserialize_type(obj: YamlObj) -> Type:
        return Type(
            name=obj.name,
            desc=obj.desc,
            page=page,
            decl=obj.decl
        )

    if hasattr(data, 'classes'):
        for cls in data.classes:
            page.classes.append(_deserialize_class_or_struct(cls, Class))

    if hasattr(data, 'structs'):
        for struct in data.structs:
            page.structs.append(_deserialize_class_or_struct(struct, Struct))

    if hasattr(data, 'enums'):
        for enum in data.enums:
            page.enums.append(_deserialize_enum(enum))

    if hasattr(data, 'types'):
        for type_ in data.types:
            page.types.append(_deserialize_type(type_))

    return page


###############################################################################
# Markdown linking                                                            #
###############################################################################

def markdown_anchor(name: str, page: str = ''):
    sanitized_name = name.translate({ord(c): None for c in '[]:<>`\\'}).replace(' ', '-')
    return f'{page}#{sanitized_name}'


def mardown_link(text, anchor):
    if anchor[0] == '#':
        return f'[{text}]({anchor})'
    else:
        return f'[[{text}|{anchor}]]'

@dataclass
class Ref:
    obj: DocumentedType
    page: str

    @property
    def _prefix(self):
        prefix = ''
        if isinstance(self.obj, Struct):
            prefix = 'struct : ldtk::'
        elif isinstance(self.obj, Class):
            prefix = 'class : ldtk::'
        return prefix

    @property
    def local_anchor(self) -> str:
        return markdown_anchor(self._prefix + self.obj.name)

    @property
    def external_anchor(self) -> str:
        return markdown_anchor(self._prefix + self.obj.name, self.page.title)

    def link_from(self, referer: DocumentedType, replace: tuple[str, str] = ('', '')) -> str:
        text = self.obj.name.replace(replace[0], replace[1])
        if referer.page == self.page:
            return mardown_link(text, self.local_anchor)
        else:
            return mardown_link(text, self.external_anchor)


###############################################################################
# Markdown generation utilities                                               #
###############################################################################

class GenCtx:
    def __init__(self, pages: list[Page], refs: list[Ref]):
        self.pages = pages
        self.refs = refs
        self.page: Optional[Page] = None
        self.current_type: Optional[DocumentedType] = None

        classes_table: dict[str, Class] = {}
        for p in pages:
            for c in p.classes + p.structs:
                classes_table[c.name] = c

        for p in pages:
            for c in p.classes + p.structs:
                if c.parent:
                    c.parent = classes_table[c.parent]

    def link_refs(self, desc: str) -> str:
        for ref in self.refs:
            if ref.obj.name != self.current_type.name:
                for prefix, suffix  in [('', ' '), (' ', ' '), (' ', '.'), (' ', ','), (' ', 's')]:
                    fixed_name = prefix + ref.obj.name + suffix
                    fixed_link = prefix + ref.link_from(self.current_type) + suffix
                    desc = desc.replace(fixed_name, fixed_link)

                    if '<T>' in ref.obj.name:
                        name = ref.obj.name.replace('<T>', '')
                        fixed_name = prefix + name + suffix
                        fixed_link = prefix + ref.link_from(self.current_type, replace=('<T>', '')) + suffix
                        desc = desc.replace(fixed_name, fixed_link)

        return desc

    def link_ref(self, desc: str, referer: DocumentedType) -> str:
        for ref in self.refs:
            if ref.obj.name != referer.name:
                desc = desc.replace(ref.obj.name, ref.link_from(referer))

        return desc


class MarkdownGenerator:
    def __init__(self, ctx: GenCtx):
        self.ctx = ctx
        self.lines = []

    def _generate_goto_button(self, anchor: str):
        self.lines += [
            f'<p align=right><a href={anchor}>&#x1F51D;</a></p>'
            , f''
        ]

    def _generate_member(self, member: Member, inherited: bool = False):
        decl = member.decl
        desc = member.desc

        if inherited:
            decl = member.decl.replace(member.cls.name, self.ctx.current_type.name)
            desc = member.desc.replace(member.cls.name, self.ctx.current_type.name)

        decl = decl.split('\n')
        decls = []
        i = 0
        while i < len(decl):
            d = decl[i]
            if d.count('template') > 0:
                decls.append(d + '\n' + decl[i + 1])
                i += 2
            else:
                decls.append(d)
                i += 1

        for decl in decls:
            self.lines += [
                f'```c++'
                , f'{decl.strip()}'
                , f'```'
            ]

            if decl.strip()[-1] == ';':
                print(f'bad decl in {self.ctx.current_type.name}: {decl}')

        if desc:
            desc = self.ctx.link_refs(desc)
            # desc = desc.replace('\n', '\n\n')
            self.lines += [
                f'{desc}'
                , ''
            ]

            if desc[-1] not in ['.', '`']:
                print(f'bad desc in {self.ctx.current_type.name}: {desc}')

    def _generate_method(self, name: str, funcs: list[Func], inherited: bool = False):
        sanitanized_name = name.replace('<', '\<').replace('>', '\>')
        self.lines += [
            f'### {sanitanized_name}'
            , f''
        ]

        if inherited:
            self.lines += [
                f'> Inherited from {self.ctx.link_ref(self.ctx.current_type.parent.name, self.ctx.current_type)}'
                , f''
            ]

        for func in funcs:
            self._generate_member(func, inherited)
            if func is not funcs[-1]:
                self.lines += [
                    '---'
                    , ''
                ]

    def _generate_field(self, name: str, field: Field):
        self.lines += [
            f'### {name}'
            , f''
        ]

        self._generate_member(field)

    def _generate_index(self, members: list[str], indent_size = 0, anchor_prefix = ''):
        indent = ' ' * indent_size
        for name in members:
            anchor = markdown_anchor(anchor_prefix + name)
            self.lines += [f'{indent}- {mardown_link(name, anchor)}']
        self.lines += [f'']

    def _generate_class_or_struct(self, cls: Class):
        self.ctx.current_type = cls

        typename = 'Class'
        if isinstance(cls, Struct):
            typename = 'Struct'

        self.lines += [
            f'# {typename} : `ldtk::{cls.name}`'
            , f''
        ]

        if cls.desc:
            self.lines += [
                self.ctx.link_refs(cls.desc),
                ''
            ]

        if [len(cls.ctor) > 0, len(cls.fields) > 0, cls.methods_count > 0].count(True) > 1:
            if len(cls.ctor) > 0:
                self.lines += [
                    f'* {mardown_link("Constructor", markdown_anchor("Constructor"))}'
                ]

            if len(cls.fields) > 0:
                self.lines += [
                    f'* {mardown_link("Fields", markdown_anchor("Fields"))}'
                ]

            if len(cls.methods) > 0:
                self.lines += [
                    f'* {mardown_link("Methods", markdown_anchor("Methods"))}'
                ]

            self.lines += ['']

        if cls.ctor:
            self.lines += [
                f'## Constructor'
                , f''
            ]
            for ctor in cls.ctor:
                self.lines += [
                    f'```c++'
                    , ctor.decl
                    , f'```'
                    , ctor.desc
                    , f''
                ]

        if cls.fields:
            self.lines += [
                f'## Fields'
                , f''
            ]

            if len(cls.fields) > 3:
                self._generate_index(['`' + name + '`' for name in cls.fields.keys()])

            gen_goto_button_after_entry = len(cls.fields) > 4

            for name, field in cls.fields.items():
                self._generate_field(name, field)
                if gen_goto_button_after_entry:
                    self._generate_goto_button(markdown_anchor(f'{typename.lower()}--ldtk' + cls.name))

            if not gen_goto_button_after_entry:
                if cls.page.documented_types_count > 1:
                    self._generate_goto_button(markdown_anchor(''))
                else:
                    self._generate_goto_button(markdown_anchor(f'{typename.lower()}--ldtk' + cls.name))

        if cls.methods:
            self.lines += [
                f'## Methods'
                , f''
            ]

            if cls.methods_count > 3:
                methods_list = ['`' + name + '`' for name in cls.methods.keys()]
                if cls.parent:
                    methods_list += ['`' + name + '`' for name in cls.parent.methods.keys()]
                self._generate_index(methods_list)

            gen_goto_button_after_entry = cls.methods_count > 4

            for name, funcs in cls.methods.items():
                self._generate_method(name, funcs)
                if gen_goto_button_after_entry:
                    self._generate_goto_button(markdown_anchor(f'{typename.lower()}--ldtk' + cls.name))

            if cls.parent:
                for name, funcs in cls.parent.methods.items():
                    self._generate_method(name, funcs, inherited=True)
                    if gen_goto_button_after_entry:
                        self._generate_goto_button(markdown_anchor(f'{typename.lower()}--ldtk' + cls.name))

            if not gen_goto_button_after_entry:
                if cls.page.documented_types_count > 1:
                    self._generate_goto_button(markdown_anchor(''))
                else:
                    self._generate_goto_button(markdown_anchor(f'{typename.lower()}--ldtk' + cls.name))

        self.ctx.current_type = None

    def _generate_enum(self, enum: Enum):
        self.ctx.current_type = enum

        self.lines += [
            f'# Enum : `ldtk::{enum.name}`'
            , f''
        ]

        if enum.desc:
            self.lines += [
                self.ctx.link_refs(enum.desc),
                ''
            ]

        for val, val_name in enum.values.items():
            self.lines += [
                f'{val}. `{enum.name}::{val_name}`',
            ]

        self.lines += ['']

        self._generate_goto_button(markdown_anchor(''))

    def _generate_type(self, type_: Type):
        self.ctx.current_type = type_

        self.lines += [
            f'# Type : `ldtk::{type_.name}`'
            , f''
        ]

        self.lines += [
            '```c++',
            type_.decl,
            '```',
        ]

        if type_.desc:
            self.lines += [
                self.ctx.link_refs(type_.desc),
            ]

        self.lines += ['']

        self._generate_goto_button(markdown_anchor(''))

    def generate_page(self, page: Page):
        self.ctx.page = page

        self.lines = [
            f'### In file `{page.filename}`'
            , f''
        ]

        if len(page.classes) + len(page.structs) + len(page.enums) + len(page.types) > 1:
            if page.classes:
                self.lines += [
                    f'* Classes',
                ]
                self._generate_index(['`ldtk::' + cls.name + '`' for cls in page.classes], 4, 'class : ')

            if page.structs:
                self.lines += [
                    f'* Structs',
                ]
                self._generate_index(['`ldtk::' + struct.name + '`' for struct in page.structs], 4, 'struct : ')

            if page.enums:
                self.lines += [
                    f'* Enums',
                ]
                self._generate_index(['`ldtk::' + enum.name + '`' for enum in page.enums], 4, 'enum : ')

            if page.types:
                self.lines += [
                    f'* Types',
                ]
                self._generate_index(['`ldtk::' + type_.name + '`' for type_ in page.types], 4, 'type : ')


        for cls in page.classes:
            self._generate_class_or_struct(cls)

        for struct in page.structs:
            self._generate_class_or_struct(struct)

        for enum in page.enums:
            self._generate_enum(enum)

        for type_ in page.types:
            self._generate_type(type_)

        self.ctx.page = None

    @property
    def generated_string(self) -> str:
        return '\n'.join(self.lines)


if __name__ == '__main__':
    DOC_DIR = 'doc'
    GEN_DIR = 'wiki'

    refs : list[Ref] = []
    pages : list[Page] = []

    directory = DOC_DIR

    for filename in listdir(directory):
        if not path.isfile(path.join(directory, filename)):
            continue

        if not filename.endswith('.yml'):
            continue

        file_path = path.join(directory, filename)

        with open(file_path) as file:
            yaml_obj = yaml2obj(yaml.safe_load(file))

        page = deserialize_yaml_obj(yaml_obj)

        print(f'Processed {file_path}')

        # fill the references dict
        for cls in page.classes:
            refs.append(Ref(cls, page))

        for struct in page.structs:
            refs.append(Ref(struct, page))

        for enum in page.enums:
            refs.append(Ref(enum, page))

        for type_ in page.types:
            refs.append(Ref(type_, page))

        pages.append(page)

    ctx = GenCtx(pages, refs)

    print(f'{len(refs)} refs found: {", ".join(sorted([ref.obj.name for ref in refs]))}')

    makedirs(GEN_DIR, exist_ok=True)
    for page in pages:
        with open(path.join(GEN_DIR, page.title + '.md'), 'w+') as md_file:
            generator = MarkdownGenerator(ctx)
            generator.generate_page(page)
            md_file.write(generator.generated_string)
