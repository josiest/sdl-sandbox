# Managing Subsystems with EnTT
Even wtih toy gamedev tools having a subsystem manager is extroardinarily
useful. EnTT's component mechancis can be used pretty effectively to get
subsystems by their type.

```cpp
struct system_manager {
    system_manager() : subsystems{}, id{ subsystems.create() } {}

    entt::registry subsystems;
    const entt::entity id;
};
system_manager& get_systems() { static system_manager sys; return sys; }
struct window_system {
    window_system(...) : { window = SDL_CreateWindow(...); }
    SDL_Window* window;
};
int main()
{
    // initialize sdl
    ...

    auto& sys = get_systems();
    sys.subsystems.emplace<window_system>(sys.id, ...);
}
```
To add more infrastructure to this, subsystems should be able to register
dependencies as meta information with the system manager. Then the system
manager can iterate over subsystems in the order their dependencies allow.

# Registering Dependencies

## static dependencies function (and concept)
When registering subsystems dependencies, you shouldn't need to create an
instance of either the dependent class, or its dependencies. So obviously there
should be a static function to register its dependencies.

The system manager will have some template "register" functions. There should
be a loosely constrained version of the function that accepts a derived
subsystem class but assumes no dependencies, and a more constrained version
that only accepts derived classes that have a static "dependencies"
function.

In true STL form, the "dependencies" function should take and return an output
iterator of "meta type" objects. Individual implementation will likely always
take a similar form as in this example:

```cpp
class mechanics_subsystem : pi::subsystem {
public:
    template<std::output_iterator<entt::meta_type> MetaOutput>
    static MetaOutput dependencies(MetaOutput into_dependencies)
    {
        return ranges::copy(std::array{ entt::resolve<...>(), ... },
                            into_dependencies).out;
    }

    ...
};
```

## dependency graph representation
The dependency graph will take the form of an edge list. More specifically,
it will map meta types to a pair of meta type sets. The first set are the
dependencies (or parents) of that type, while the second set are the dependents
(or children) of that type.

```cpp
namespace dependencies {
using set = std::unordered_set<entt::meta_type>;
using node = std::pair<set, set>;
using graph = std::unordered_map<entt::meta_type, node>;
}
```

## registering subsystems
Registering a subsystem will dynamically update its dependencies to reflect
the new dependent subsystem
```
for each dependency of subsystem:
    find or insert dependency in registered subsystems
    add subsystem to dependency's children
```

# Initializing/Destroying Subsystems

# Polymorphic type handling

## Using an ABC/Interface


## Using concepts + traits
