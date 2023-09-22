#pragma once
#include <expected>
#include <format>
#include <string>

#include <string_view>
#include <filesystem>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL_filesystem.h>

inline namespace pi {

inline const std::filesystem::path& root_path()
{
    namespace fs = std::filesystem;
    static const fs::path root =
        fs::canonical(fs::path{ SDL_GetBasePath() }/"..");
    return root;
}

inline std::filesystem::path resource_dir()
{
    return root_path()/"resources";
}

inline std::filesystem::path resource_path(std::string_view filename)
{
    return resource_dir()/filename;
}

inline std::expected<YAML::Node, std::string>
load_config(std::string_view filename)
{
    namespace fs = std::filesystem;
    using namespace std::string_view_literals;

    const auto config_path = resource_path(filename);
    std::error_code ec;
    if (not fs::exists(config_path, ec)) {
        return std::unexpected(std::format(
                    "no file named \"{}\"", config_path.string()));
    }
    if (not fs::is_regular_file(config_path, ec)) {
        return std::unexpected(std::format(
                    "\"{}\" is not a file", config_path.string()));
    }
    const auto permissions = fs::status(config_path, ec).permissions();
    const auto readable = fs::perms::owner_read
                        | fs::perms::group_read
                        | fs::perms::others_read;

    if (readable != (permissions & readable)) {
        return std::unexpected(std::format(
                    "\"{}\" does not have correct read permissions",
                    config_path.string()));
    }
    return YAML::LoadFile(config_path.string());
}
}
