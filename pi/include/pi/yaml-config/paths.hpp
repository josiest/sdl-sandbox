#pragma once
#include <string_view>
#include <filesystem>

inline namespace pi {
const std::filesystem::path& root_path();
std::filesystem::path resource_dir();
std::filesystem::path resource_path(std::string_view filename);
}
