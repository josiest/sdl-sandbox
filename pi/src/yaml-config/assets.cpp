#include "pi/yaml-config/assets.hpp"
#include <filesystem>
#include <SDL2/SDL_filesystem.h>

namespace fs = std::filesystem;

const std::filesystem::path pi::resolve_path_to_root(std::string_view path)
{
    static const fs::path root = fs::canonical(fs::path{ SDL_GetBasePath() }/"..");
    return root/path;
}

std::expected<YAML::Node, std::string> pi::load_yaml_resource(std::string_view filename)
{
    using namespace std::string_view_literals;

    const auto config_path = resolve_path_to_root(filename);
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
