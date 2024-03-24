#pragma once
#include "pi/system-graph/system_graph.hpp"
#include "pi/sdl-systems/init_params.hpp"

inline namespace pi {
class init_system {
public:
#pragma region Rule of Five
    init_system(const init_system&) = delete;
    init_system& operator=(const init_system&) = delete;

    init_system(init_system && tmp);
    init_system& operator=(init_system && tmp);

    ~init_system();
#pragma endregion
    static init_system* load(system_graph& systems);
    static init_system* load(system_graph& systems, const YAML::Node& root);
    static init_system* load(system_graph& systems, const pi::init_params& params);
private:
    init_system() = default;
    bool should_quit = true;
};
}
