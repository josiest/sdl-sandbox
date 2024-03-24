#include "pi/yaml-config/paths.hpp"
#include <SDL2/SDL_filesystem.h>
#include <filesystem>
namespace fs = std::filesystem;

const std::filesystem::path& pi::root_path()
{
    static const fs::path root = fs::canonical(fs::path{ SDL_GetBasePath() }/"..");
    return root;
}

std::filesystem::path pi::resource_dir()
{
    return root_path()/"resources";
}

std::filesystem::path pi::resource_path(std::string_view filename)
{
    return resource_dir()/filename;
}
