#pragma once
#include <cppx/basic_types/Size.hpp>
#include <cppx/core/type_builders.hpp>

#include <algorithm>    // std::min
#include <memory.h>     // memcmp, wmemcmp
#include <string>       // std::char_traits

namespace nn {
    using cppx::Ptr_to;
    using cppx::Size;
    using std::char_traits;
    using std::min;

    // The Microsoft docs indicate that their char_traits::compare stops at 0-values.
    // Hence this possibly redundant compare implementation, using traits as fallback.

    template< class Char_t >
    auto compare(
        const Ptr_to<const Char_t> a, const Size size_a,
        const Ptr_to<const Char_t> b, const Size size_b
        )
        -> int
    {
        if( const int r = char_traits<Char_t>::compare( a, b,  min( size_a, size_b ) ) )
        {
            return r;
        }
        else
        {
            return (size_a < size_b? -1 : size_a == size_b? 0 : +1);
        }
    }

    inline
    auto compare(
        const Ptr_to<const char> a, const Size size_a,
        const Ptr_to<const char> b, const Size size_b
        )
        -> int
    {
        if( const int r = ::memcmp( a, b,  min( size_a, size_b ) ) )
        {
            return r;
        }
        else
        {
            return (size_a < size_b? -1 : size_a == size_b? 0 : +1);
        }
    }

    inline
    auto compare(
        const Ptr_to<const wchar_t> a, const Size size_a,
        const Ptr_to<const wchar_t> b, const Size size_b
        )
        -> int
    {
        if( const int r = ::wmemcmp( a, b,  min( size_a, size_b ) ) )
        {
            return r;
        }
        else
        {
            return (size_a < size_b? -1 : size_a == size_b? 0 : +1);
        }
    }
}  // namespace nn
