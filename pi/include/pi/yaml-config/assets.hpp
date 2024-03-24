#pragma once
#include "pi/yaml-config/meta.hpp"

#include <string>
#include <string_view>
#include <expected>

#include <filesystem>
#include <format>

#include <yaml-cpp/yaml.h>

inline namespace pi {

const std::filesystem::path resolve_path_to_root(std::string_view path);
std::expected<YAML::Node, std::string> load_yaml_resource(std::string_view filename);

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