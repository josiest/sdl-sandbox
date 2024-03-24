#pragma once

#include <cstdio>
#include <cstdint>

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/node/convert.h>
#include <entt/meta/meta.hpp>

#include "pi/reflection/reflect.hpp"
#include "pi/yaml-config/errors.hpp"

inline namespace pi {
namespace config {

template<typename T>
concept yaml_decodable =
    std::default_initializable<T> and
requires(T& val, const YAML::Node& node) {
    { YAML::convert<T>::decode(node, val) } -> std::same_as<bool>;
};


YAML::Node encode_unsigned_integer(const entt::meta_any & number);
YAML::Node encode_integer(const entt::meta_any & obj);
YAML::Node encode_class(const entt::meta_any & obj);

template<yaml_decodable ValueType>
inline bool decode_yaml(const YAML::Node & node, entt::meta_any & obj)
{
    ValueType val;
    if (YAML::convert<ValueType>::decode(node, val)) {
        obj = val;
        return true;
    }
    return false;
}

bool decode_float(const YAML::Node& number, entt::meta_any& obj);
bool decode_unsigned_integer(const YAML::Node & number, entt::meta_any & obj);
bool decode_integer(const YAML::Node & node, entt::meta_any & obj);
bool decode_with_function(const YAML::Node & node, const entt::meta_func & decode_fn, entt::meta_any & obj);
bool decode_class(const YAML::Node & node, entt::meta_any & obj);
bool decode_scalar(const YAML::Node & node, entt::meta_any & obj);
bool decode_map(const YAML::Node & node, entt::meta_any & obj);

}
}
template<>
struct YAML::convert<entt::meta_any> {
    inline static YAML::Node encode(const entt::meta_any& obj)
    {
        const auto type = obj.type();
        if (type.is_integral()) {
            return pi::config::encode_integer(obj);
        }
        // TODO: encode floating point
        if (type.is_class()) {
            return pi::config::encode_class(obj);
        }
        // TODO: add more info to message
        std::printf("failed to encode type\n");
        return YAML::Node{};
    }

    inline static bool decode(const YAML::Node& node, entt::meta_any& obj)
    {
        namespace msg = YAML::ErrorMsg;
        if (not node) {
            std::printf("%s\n", msg::invalid_node);
            return false;
        }
        if (node.IsScalar()) {
            return pi::config::decode_scalar(node, obj);
        }
        if (node.IsMap()) {
            return pi::config::decode_map(node, obj);
        }
        // TODO: decode other yaml-types
        return false;
    }
};

inline namespace pi {
namespace config {

template<reflectable T>
bool decode(const YAML::Node & node, T & val)
{
    using as_any = YAML::convert<entt::meta_any>;
    pi::reflect<T>();

    entt::meta_any any_val{ std::in_place_type<T &>, val };
    const auto type_name = any_val.type().info().name();
    if (as_any::decode(node, any_val)) {
        val = any_val.cast<T>();
        return true;
    }
    return false;
}
}
}
