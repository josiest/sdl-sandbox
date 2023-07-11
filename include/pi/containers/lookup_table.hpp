#pragma once
#include <concepts>
#include <algorithm>

#include <cstddef>
#include <utility>
#include <array>
#include <initializer_list>

inline namespace pi {

/** A simple map with a static size that allows for constexpr initialization */
template<typename Key, typename Value, std::size_t N>
struct lookup_table {
    // Container Types
    using value_type = std::pair<Key, Value>;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    // ReversibleContainer Types
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // AssociativeContainer Types (Partially Satisfied)
    using key_type = Key;
    using mapped_type = Value;
    // using key_compare = ;
    // using value_compare = ;

    constexpr lookup_table(std::initializer_list<value_type> args)
    {
        std::ranges::copy_n(args.begin(), std::min(args.size(), N),
                            mappings.begin());
    }

    constexpr auto begin() { return mappings.begin(); }
    constexpr auto rbegin() { return mappings.rbegin(); }
    constexpr auto begin() const { return mappings.begin(); }
    constexpr auto rbegin() const { return mappings.rbegin(); }
    constexpr auto cbegin() const { return mappings.begin(); }
    constexpr auto crbegin() const { return mappings.crbegin(); }
    constexpr auto end() { return mappings.end(); }
    constexpr auto rend() { return mappings.rend(); }
    constexpr auto end() const { return mappings.end(); }
    constexpr auto rend() const { return mappings.rend(); }
    constexpr auto cend() const { return mappings.end(); }
    constexpr auto crend() const { return mappings.crend(); }

    constexpr std::size_t size() const { return N; }
    constexpr std::size_t max_size() const { return N; }
    constexpr bool empty() const { return size() == 0; }

    void swap(lookup_table<Key, Value, N>& other) {
        mappings.swap(other.mappings);
    }

    template<std::equality_comparable_with<Key> AltKey>
    constexpr auto find(const AltKey& key)
    {
        return std::ranges::find(mappings, key, &value_type::first);
    }
    template<std::equality_comparable_with<Value> AltValue>
    requires (not std::equality_comparable_with<AltValue, Key>)
    constexpr auto find(const AltValue& value)
    {
        return std::ranges::find(mappings, value, &value_type::second);
    }
    template<std::equality_comparable_with<Key> AltKey>
    constexpr auto find_by_key(const Key& key)
    {
        return std::ranges::find(mappings, key, &value_type::first);
    }
    template<std::equality_comparable_with<Value> AltValue>
    constexpr auto find_by_value(const AltValue& value)
    {
        return std::ranges::find(mappings, value, &value_type::second);
    }

    template<std::equality_comparable_with<Key> AltKey>
    constexpr auto find(const AltKey& key) const
    {
        return std::ranges::find(mappings, key, &value_type::first);
    }
    template<std::equality_comparable_with<Value> AltValue>
    requires (not std::equality_comparable_with<AltValue, Key>)
    constexpr auto find(const AltValue& value) const
    {
        return std::ranges::find(mappings, value, &value_type::second);
    }
    template<std::equality_comparable_with<Key> AltKey>
    constexpr auto find_by_key(const Key& key) const
    {
        return std::ranges::find(mappings, key, &value_type::first);
    }
    template<std::equality_comparable_with<Value> AltValue>
    constexpr auto find_by_value(const AltValue& value) const
    {
        return std::ranges::find(mappings, value, &value_type::second);
    }

    std::array<value_type, N> mappings;
};

template<typename Key, typename Value, std::size_t N>
auto operator<=>(const lookup_table<Key, Value, N>& a,
                 const lookup_table<Key, Value, N>& b)
{
    return a.mappings <=> b.mappings;
}
}

template<typename Key, typename Value, std::size_t N>
struct std::tuple_size<pi::lookup_table<Key, Value, N>>
    : public std::integral_constant<std::size_t, N>
{
};

template<std::size_t Index, typename Key, typename Value, std::size_t N>
requires (Index < N)
struct std::tuple_element<Index, pi::lookup_table<Key, Value, N>> {
    using type = std::pair<Key, Value>;
};
