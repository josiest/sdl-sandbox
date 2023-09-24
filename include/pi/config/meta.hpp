#pragma once

#include <cstdio>
#include <cstdint>

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/node/convert.h>
#include <entt/meta/meta.hpp>

#include "pi/meta/reflect.hpp"
#include "pi/config/errors.hpp"

template<>
struct YAML::convert<entt::meta_any> {
    static YAML::Node encode(const entt::meta_any& obj);
    static bool decode(const YAML::Node& node, entt::meta_any& obj);
};

inline namespace pi {
namespace config {

template<typename T>
concept yaml_decodable =
    std::default_initializable<T> and
requires(T& val, const YAML::Node& node) {
    { YAML::convert<T>::decode(node, val) } -> std::same_as<bool>;
};


inline YAML::Node encode_unsigned_integer(const entt::meta_any & number)
{
    const auto type = number.type();
    if (type.size_of() == sizeof(std::uint8_t)) {
        return YAML::Node{ number.cast<std::uint8_t>() };
    }
    else if (type.size_of() == sizeof(std::uint16_t)) {
        return YAML::Node{ number.cast<std::uint16_t>() };
    }
    else if (type.size_of() == sizeof(std::uint32_t)) {
        return YAML::Node{ number.cast<std::uint32_t>() };
    }
    else if (type.size_of() == sizeof(std::uint64_t)) {
        return YAML::Node{ number.cast<std::uint64_t>() };
    }
    // TODO: add more info to message
    std::printf("tried to encode unsigned int with unknown size\n");
    return YAML::Node{};
}

inline YAML::Node encode_integer(const entt::meta_any & obj)
{
    // TODO: encode signed int
    if (not obj.type().is_signed()) {
        return pi::config::encode_unsigned_integer(obj);
    }
    std::printf("failed to encode integer\n");
    return YAML::Node{};
}

inline YAML::Node encode_class(const entt::meta_any & obj)
{
    using namespace entt::literals;
    if (auto encoded = obj.invoke("yaml-encode"_hs, obj)) {
        if (auto * node = encoded.try_cast<YAML::Node>()) {
            return *node;
        }
        else {
            // TODO: add more info to message
            printf("yaml-encode did not return node\n");
            return YAML::Node{};
        }
    }
    // TODO: encode non-specialized types
    std::printf("failed to encode class\n");
    return YAML::Node{};
}

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

inline bool decode_float(const YAML::Node& number, entt::meta_any& obj)
{
    const auto type = obj.type();
    if (type.size_of() == sizeof(float))
    {
        return decode_yaml<float>(number, obj);
    }
    if (type.size_of() == sizeof(double))
    {
        return decode_yaml<double>(number, obj);
    }
    if (type.size_of() == sizeof(long double))
    {
        return decode_yaml<long double>(number, obj);
    }
    // TODO: include more info in this message
    std::printf("couldn't decode floating point number of unknown size\n");
    return false;
}

inline bool decode_unsigned_integer(const YAML::Node & number,
                                    entt::meta_any & obj)
{
    const auto type = obj.type();
    if (type.size_of() == sizeof(std::uint8_t)) {
        return decode_yaml<std::uint8_t>(number, obj);
    }
    else if (type.size_of() == sizeof(std::uint16_t)) {
        return decode_yaml<std::uint16_t>(number, obj);
    }
    else if (type.size_of() == sizeof(std::uint32_t)) {
        return decode_yaml<std::uint32_t>(number, obj);
    }
    else if (type.size_of() == sizeof(std::uint64_t)) {
        return decode_yaml<std::uint64_t>(number, obj);
    }
    // TODO: include more info in this message
    std::printf("couldn't decode unsigned integer of unknown size\n");
    return false;
}

inline bool decode_integer(const YAML::Node & node, entt::meta_any & obj)
{
    // TODO: decode signed integer
    if (not obj.type().is_signed()) {
        return decode_unsigned_integer(node, obj);
    }
    std::printf("decoding signed integer currently not suported\n");
    return false;
}

inline bool decode_with_function(const YAML::Node & node,
                                 const entt::meta_func & decode_fn,
                                 entt::meta_any & obj)
{
    if (not decode_fn.is_static()) {
        // TODO: add more to message
        std::printf("decode function isn't static\n");
        return false;
    }
    if (decode_fn.arity() != 2) {
        // TODO: add more to message
        std::printf("wrong number of arguments to yaml decode\n");
        return false;
    }
    const auto node_arg = decode_fn.arg(0);
    if (node_arg != entt::resolve<const YAML::Node &>()) {
        // TODO: add more to message
        std::printf("first argument of yaml-decode is not node!\n");
        return false;
    }
    const auto obj_arg = decode_fn.arg(1);
    if (obj_arg != obj.type()) {
        // TODO: add more to message
        std::printf("second argument of yaml-decode is not correct!\n");
        return false;
    }
    const auto ret_type = decode_fn.ret();
    if (ret_type != entt::resolve<bool>()) {
        // TODO: add more to message
        std::printf("return type of yaml-decode is not bool!\n");
        return false;
    }
    std::array args{ entt::meta_any{ node }, obj.as_ref() };
    auto decoded = decode_fn.invoke(obj, args.data(), args.size());
    if (const bool succeeded = decoded.cast<bool>()) {
        return succeeded;
    }
    else {
        // TODO: add more info to message
        std::printf("Failed to invoke yaml-decode\n");
        return false;
    }
}

inline bool decode_class(const YAML::Node & node, entt::meta_any & obj)
{
    using namespace entt::literals;
    if (auto decode_fn = obj.type().func("yaml-decode"_hs)) {
        return decode_with_function(node, decode_fn, obj);
    }
    if (obj.type() == entt::resolve<std::string>())
    {
        return decode_yaml<std::string>(node, obj);
    }
    // TODO: decode non-specialized types
    std::printf("failed to decode class\n");
    return false;
}

inline bool decode_scalar(const YAML::Node & node, entt::meta_any & obj)
{
    const auto type = obj.type();
    if (type.is_integral()) {
        return decode_integer(node, obj);
    }
    if (type.is_arithmetic())
    {
        return decode_float(node, obj);
    }
    // TODO: decode other types
    if (type.is_class()) {
        return decode_class(node, obj);
    }
    // TODO: add more info to message
    std::printf("failed to decode scalar value\n");
    return false;
}

inline bool decode_map(const YAML::Node & node, entt::meta_any & obj)
{
    if (not node or not node.IsMap()) { return false; }
    bool success = true;
    // TODO: forward declare decode functions
    using as_any = YAML::convert<entt::meta_any>;
    for (const auto & elem : node) {
        const std::string prop_name = elem.first.Scalar();
        const auto prop_id = entt::hashed_string{ prop_name.c_str() };
        auto prop = obj.get(prop_id);
        // TODO: add base case for max recursion depth
        success = success and as_any::decode(elem.second, prop);
        obj.set(prop_id, prop);
    }
    if (success) { return true; }

    // TODO: add more info to message
    std::printf("encountered errors when decoding map value\n");
    return false;
}

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

YAML::Node YAML::convert<entt::meta_any>
               ::encode(const entt::meta_any& obj)
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

bool YAML::convert<entt::meta_any>
         ::decode(const YAML::Node& node, entt::meta_any& obj)
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
