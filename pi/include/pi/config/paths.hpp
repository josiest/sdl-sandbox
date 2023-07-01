#pragma once
#include <string_view>
#include <filesystem>
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
}