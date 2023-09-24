#pragma once
#include "pi/config/meta.hpp"

#include <string>
#include <string_view>
#include <expected>

#include <filesystem>
#include <format>

#include <SDL2/SDL_filesystem.h>
#include <yaml-cpp/yaml.h>

inline namespace pi {

inline const std::filesystem::path resolve_path_to_root(std::string_view path)
{
    namespace fs = std::filesystem;
    static const fs::path root = fs::canonical(fs::path{ SDL_GetBasePath() }/"..");
    return root/path;
}

inline std::expected<YAML::Node, std::string>
load_yaml_resource(std::string_view filename)
{
    namespace fs = std::filesystem;
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

template<reflectable Asset>
Asset load_asset(std::string_view asset_path)
{
    Asset loaded_asset;
    const auto yaml = load_yaml_resource(asset_path);
    if (yaml) {
        if (config::decode(*yaml, loaded_asset)) { return loaded_asset; }
    }
    else {
        std::printf("%s\n", yaml.error().c_str());
    }
    // TODO: use reflection to get asset name
    std::printf("unable to load asset, using defaults\n");
    return loaded_asset;
}
}