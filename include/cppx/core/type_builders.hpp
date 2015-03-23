#pragma once
#include <array>                            // std::array
#include <cppx/basic_types/Size.hpp>        // cppx::Size

namespace cppx {
    template< class T >         using Type          = T;

    template< class T >         using Ptr_to        = T*;
    template< class T >         using Ref_to        = T&;
    template< class T >         using Rvalue_ref_to = T&&;
    template< class T >         using Universal_ref_to  = T&&;      // A bit subtle.
    template< Size n, class T > using Raw_array_of  = T[n];     // A bit dangerous.
    template< Size n, class T > using Array_of      = std::array< T, n >;
}  // namespace cppx
