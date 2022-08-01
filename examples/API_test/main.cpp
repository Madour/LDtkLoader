// Created by Modar Nasser on 28/01/2021.

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <streambuf>
#include <vector>

#include "LDtkLoader/Project.hpp"

template <std::size_t Size>
struct custom_stream_buffer : std::streambuf {
    explicit custom_stream_buffer(const std::string& filename) {
        m_handle.open(filename);
    }
    auto underflow() -> int_type override {
        if (m_handle.eof()) {
            return traits_type::eof();
        }

        // emulate loading bytes from a virtual filesystem
        m_handle.read(m_buffer, Size);
        std::streamsize read_size = m_handle.gcount();

        setg(m_buffer, m_buffer, m_buffer + read_size);
        return *gptr();
    }
private:
    std::ifstream m_handle;
    char m_buffer[Size] = {0};
};

auto createCustomStreamBuffer(const std::string& filepath) -> std::unique_ptr<std::streambuf> {
    return std::unique_ptr<std::streambuf>(new custom_stream_buffer<2048>(filepath));
}

auto getFileBytes(const std::string& filepath) -> std::vector<std::uint8_t> {
    std::basic_ifstream<std::uint8_t> file(filepath, std::ios::in | std::ios::binary);

    return std::vector<std::uint8_t>(
        std::istreambuf_iterator<std::uint8_t>(file),
        std::istreambuf_iterator<std::uint8_t>()
    );
}

int main() {
    try {
        // load from file
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromFile("all_features.ldtk");
        }

        // load from multiple files
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromFile("all_features_external.ldtk");
        }

        // load from file using custom stream buffer
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromFile("all_features.ldtk", createCustomStreamBuffer);
        }

        // load from multiple files using custom stream buffer
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromFile("all_features_external.ldtk", createCustomStreamBuffer);
        }

        // load from memory (vector)
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromMemory(getFileBytes("all_features.ldtk"));
        }

        // load from memory (pointer and size)
        {
            auto file_bytes = getFileBytes("all_features.ldtk");
            ldtk::Project ldtk_project;
            ldtk_project.loadFromMemory(file_bytes.data(), static_cast<unsigned int>(file_bytes.size()));
        }
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // loads everything without errors
    std::cout << "Loaded API test file successfully !" << std::endl;

    return 0;
}
