#pragma once
// http://stackoverflow.com/questions/12520192/is-stdstring-refcounted-in-gcc-c11
// http://stackoverflow.com/questions/12199710/legality-of-cow-stdstring-implementation-in-c11

#include <cppx/basic_types/Size.hpp>        // cppx::Size
#include <cppx/core/type_builders.hpp>      // cppx::Ref_to, cppx::Rvalue_ref_to
#include <nn/Refcounted_string.hpp>         // nn::Refcounted_string_

#include <initializer_list>     // std::initializer_list
#include <iosfwd>               // std::basic_istream, std::basic_ostream
#include <iterator>             // std::reverse_iterator
#include <limits>               // std::numeric_limits
#include <memory>               // std::allocator, std::allocator_traits
#include <string>               // std::char_traits
#include <stddef.h>             // size_t

namespace nn {      // no nonsense
    using cppx::Ptr_to;
    using cppx::Ref_to;
    using cppx::Rvalue_ref_to;
    using cppx::Size;

    using std::allocator;
    using std::allocator_traits;
    using std::basic_istream;
    using std::basic_ostream;
    using std::char_traits;
    using std::initializer_list;
    using std::numeric_limits;

    template<class charT, class traits = char_traits<charT>, class Allocator = allocator<charT> >
    class basic_string {
    private:
        using This = basic_string;
        using Impl = nn::Refcounted_string_< charT, traits, Allocator >;

        Impl  impl_;

    public:
        // types:
        using traits_type       = traits;
        using value_type        = typename traits::char_type;
        using allocator_type    = Allocator;
        using size_type         = typename allocator_traits<Allocator>::size_type;
        using difference_type   = typename allocator_traits<Allocator>::difference_type;
        using reference         = value_type&;
        using const_reference   = const value_type&;
        using pointer           = typename allocator_traits<Allocator>::pointer;
        using const_pointer     = typename allocator_traits<Allocator>::const_pointer;
        using iterator          = typename Impl::It; // See 23.2
        using const_iterator    = typename Impl::C_it; // See 23.2
        using reverse_iterator          = std::reverse_iterator<iterator>;
        using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
        static const size_type npos = -1;

        // 21.4.2, construct/copy/destroy:

        /*   1 */   explicit basic_string( Ref_to<const Allocator> a = Allocator())
                        : impl_( a ) {}

        /*   2 */   basic_string( Ref_to<const This> other )
                        : impl_( other.impl_ ) {}

        /*   3 */   basic_string( Rvalue_ref_to<This> other ) noexcept
                        : impl_( std::move( other.impl_ ) ) {}

        /*   4 */   basic_string( Ref_to<const This> other, size_type pos, size_type n = npos, Ref_to<const Allocator> a = Allocator() )
                    : impl_()
                    {
                        const size_type other_size = other.impl_.size();
                        if( pos > other_size ) { throw std::out_of_range( "nn::basic_string::<init>: pos > other.size()" ); }
                        const size_type rlen = std::min( n, other_size - pos );
                        impl_ = nn::Refcounted_string( &other.impl_[pos], &other.impl_[pos + rlen], a );
                    }

        /*   5 */   basic_string( Ptr_to<const charT> s, size_type n, Ref_to<const Allocator> a = Allocator() )
                    : impl_( s, s + n, a ) {}

        /*   6 */   basic_string( Ptr_to<const charT> s, Ref_to<const Allocator> a = Allocator() )
                    : impl_( s, s + traits::length( s ), a ) {}

        /*   7 */   basic_string( size_type n, charT c, Ref_to<const Allocator> a = Allocator() )
                    : impl_( nn::Refcounted_string_<charT>::repeat( c, n, a ) ) {}

        /*   8 */   template<class InputIterator>
                    basic_string( InputIterator begin, InputIterator end, Ref_to<const Allocator> a = Allocator() )
                    : impl_( begin, end, a ) {}

        /*   9 */   basic_string( initializer_list<charT> data, Ref_to<const Allocator> a = Allocator() )
                    : impl_( data.begin(), data.end(), a ) {}

        /*  10 */   basic_string( Ref_to<const This> other, Ref_to<const Allocator> a )
                    : impl_( other.impl_, a ) {}

        /*  11 */   basic_string( Rvalue_ref_to<This> other, Ref_to<const Allocator> a )
                    : impl_( move( other.impl_ ), a ) {}

        /*  12 */   ~basic_string()
                    {}

        /*  13 */   auto operator=( Ref_to<const This> other )
                        -> Ref_to<This>
                    { impl_ = other.impl_;  return *this; }

        /*  14 */   auto operator=( Rvalue_ref_to<This> other ) noexcept
                        -> Ref_to<This>
                    { impl_ = move( other.impl_ ); return *this; }

        /*  15 */   auto operator=( Ptr_to<const charT> s )
                        -> Ref_to<This>
                    { impl_ = nn::Refcounted_string( s, s + traits::length( s ) );  return *this; }

        /*  16 */   auto operator=( charT c )
                        -> Ref_to<This>
                    { impl_ = nn::Refcounted_string( &c, &c + 1 );  return *this; }

        /*  17 */   auto operator=( initializer_list<charT> data ) -> Ref_to<This>
                    { impl_ = nn::Refcounted_string( data.begin(), data.end() );  return *this; }

        // 21.4.3, iterators:

        /*  18 */   auto begin() noexcept
                        -> iterator
                    { return impl_.begin(); }

        /*  19 */   auto begin() const noexcept
                        -> const_iterator
                    { return impl_.begin(); }

        /*  20 */   auto end() noexcept
                        -> iterator
                    { return impl_.end(); }

        /*  21 */   auto end() const noexcept
                        -> const_iterator
                    { return impl_.end(); }

        /*  22 */   auto rbegin() noexcept
                        -> reverse_iterator
                    { return reverse_iterator( impl_.end() ); }

        /*  23 */   auto rbegin() const noexcept
                        -> const_reverse_iterator
                    { return const_reverse_iterator( impl_.end() ); }

        /*  24 */   auto rend() noexcept
                        -> reverse_iterator
                    { return reverse_iterator( impl_.begin() ); }

        /*  25 */   auto rend() const noexcept
                        -> const_reverse_iterator
                    { return const_reverse_iterator( impl_.begin() ); }

        /*  26 */   auto cbegin() const noexcept
                        -> const_iterator
                    { Ref_to<const This> c_self = *this; return c_self.begin(); }

        /*  27 */   auto cend() const noexcept
                        -> const_iterator
                    { Ref_to<const This> c_self = *this; return c_self.end(); }

        /*  28 */   auto crbegin() const noexcept
                        -> const_reverse_iterator
                    { Ref_to<const This> c_self = *this; return c_self.rbegin(); }

        /*  29 */   auto crend() const noexcept
                        -> const_reverse_iterator
                    { Ref_to<const This> c_self = *this; return c_self.rend(); }

        // 21.4.4, capacity:

        /*  30 */   auto size() const noexcept
                        -> size_type
                    { return impl_.size(); }

        /*  31 */   auto length() const noexcept
                        -> size_type
                    { return impl_.size(); }

        /*  32 */   auto max_size() const noexcept
                        -> size_type
                    { return numeric_limits<typename Impl::Size>::max(); }

        /*  33 */   void resize( size_type n, charT c )
                    { impl_.resize( n, c ); }

        /*  34 */   void resize( size_type n )
                    { impl_.resize( n ); }

        /*  35 */   auto capacity() const noexcept
                        -> size_type
                    { return impl_.capacity(); }

        /*  36 */   void reserve( size_type n = 0 )
                    { impl_.ensure_capacity( n ); }

        /*  37 */   void shrink_to_fit()
                    { impl_.shrink_to_fit(); }

        /*  38 */   void clear() noexcept
                    { impl_ = Impl(); }

        /*  39 */   auto empty() const noexcept
                        -> bool
                    { return impl_.size() == 0; }

        // 21.4.5, element access:

        /*  40 */   auto operator[]( size_type i ) const
                        -> const_reference
                    { return impl_[i]; }

        /*  41 */   auto operator[]( size_type i )
                        -> reference
                    { return impl_[i]; }

        /*  42 */   auto at( size_type i ) const
                        -> const_reference
                    {
                        if( i >= impl_.size() ){ throw std::out_of_range( "nn::basic_string::at" ); }
                        return operator[]( i );
                    }

        /*  43 */   auto at( size_type i )
                        -> reference
                    {
                        if( i >= size_t( impl_.size() ) ){ throw std::out_of_range( "nn::basic_string::at" ); }
                        return operator[]( i );
                    }

        /*  44 */   auto front() const
                        -> Ref_to<const charT>
                    { return *impl_.begin(); }

        /*  45 */   auto front()
                        -> Ref_to<charT>
                    { return *impl_.begin(); }

        /*  46 */   auto back() const
                        -> Ref_to<const charT>
                    { return *(impl_.end() - 1); }

        /*  47 */   auto back()
                        -> Ref_to<charT>
                    { return *(impl_.end() - 1); }

        // 21.4.6, modifiers:

        /*  48 */   auto operator+=( Ref_to<const basic_string> str )
                        -> Ref_to<basic_string>
                    { impl_ += str.impl_;  return *this; }

        /*  49 */   auto operator+=( Ptr_to<const charT> s )
                        -> Ref_to<basic_string>
                    { impl_ += Impl( s );  return *this; }

        /*  50 */   auto operator+=( const charT c )
                        -> Ref_to< basic_string >
                    { impl_ += c;  return *this; }

        /*  51 */   basic_string& operator+=(initializer_list<charT>);
        /*  52 */   basic_string& append(const basic_string& str);
        /*  53 */   basic_string& append(const basic_string& str, size_type pos, size_type n);
        /*  54 */   basic_string& append(const charT* s, size_type n);
        /*  55 */   basic_string& append(const charT* s);
        /*  56 */   basic_string& append(size_type n, charT c);
        /*  57 */   template<class InputIterator>
                    basic_string& append(InputIterator first, InputIterator last);
        /*  58 */   basic_string& append(initializer_list<charT>);
        /*  59 */   void push_back(charT c);
        /*  60 */   basic_string& assign(const basic_string& str);
        /*  61 */   basic_string& assign(basic_string&& str) noexcept;
        /*  62 */   basic_string& assign(const basic_string& str, size_type pos, size_type n);
        /*  63 */   basic_string& assign(const charT* s, size_type n);
        /*  64 */   basic_string& assign(const charT* s);
        /*  65 */   basic_string& assign(size_type n, charT c);
        /*  66 */   template<class InputIterator>
                    basic_string& assign(InputIterator first, InputIterator last);
        /*  67 */   basic_string& assign(initializer_list<charT>);
        /*  68 */   basic_string& insert(size_type pos1, const basic_string& str);
        /*  69 */   basic_string& insert(size_type pos1, const basic_string& str,
        /*  70 */   size_type pos2, size_type n);
        /*  71 */   basic_string& insert(size_type pos, const charT* s, size_type n);
        /*  72 */   basic_string& insert(size_type pos, const charT* s);
        /*  73 */   basic_string& insert(size_type pos, size_type n, charT c);
        /*  74 */   iterator insert(const_iterator p, charT c);
        /*  75 */   iterator insert(const_iterator p, size_type n, charT c);
        /*  76 */   template<class InputIterator>
                    iterator insert(const_iterator p, InputIterator first, InputIterator last);
        /*  77 */   iterator insert(const_iterator p, initializer_list<charT>);
        /*  78 */   basic_string& erase(size_type pos = 0, size_type n = npos);
        /*  79 */   iterator erase(const_iterator p);
        /*  80 */   iterator erase(const_iterator first, const_iterator last);
        /*  81 */   void pop_back();
        /*  82 */   basic_string& replace(size_type pos1, size_type n1, const basic_string& str);
        /*  83 */   basic_string& replace(size_type pos1, size_type n1, const basic_string& str,
        /*  84 */   size_type pos2, size_type n2);
        /*  85 */   basic_string& replace(size_type pos, size_type n1, const charT* s, size_type n2);
        /*  86 */   basic_string& replace(size_type pos, size_type n1, const charT* s);
        /*  87 */   basic_string& replace(size_type pos, size_type n1, size_type n2, charT c);
        /*  88 */   basic_string& replace(const_iterator i1, const_iterator i2, const basic_string& str);
        /*  89 */   basic_string& replace(const_iterator i1, const_iterator i2, const charT* s, size_type n);
        /*  90 */   basic_string& replace(const_iterator i1, const_iterator i2, const charT* s);
        /*  91 */   basic_string& replace(const_iterator i1, const_iterator i2, size_type n, charT c);
        /*  92 */   template<class InputIterator>
                    basic_string& replace(const_iterator i1, const_iterator i2, InputIterator j1, InputIterator j2);
        /*  93 */   basic_string& replace(const_iterator, const_iterator, initializer_list<charT>);
        /*  94 */   size_type copy(charT* s, size_type n, size_type pos = 0) const;

        /*  95 */   void swap( basic_string& other )
                    { swap( impl_, other.impl_ ); }

        // 21.4.7, string operations:

        /*  96 */   auto c_str() const noexcept
                        -> Ptr_to<const charT>
                    { return impl_.c_str(); }

        auto __shortlived_c_str() const noexcept
            -> Ptr_to<const charT>
        { return impl_.shortlived_c_str(); }

        /*  97 */   auto data() const noexcept
                        -> Ptr_to<const charT>
                    { return impl_.c_str(); }

        /*  98 */   allocator_type get_allocator() const noexcept;
        /*  99 */   size_type find (const basic_string& str, size_type pos = 0) const noexcept;
        /* 100 */   size_type find (const charT* s, size_type pos, size_type n) const;
        /* 101 */   size_type find (const charT* s, size_type pos = 0) const;
        /* 102 */   size_type find (charT c, size_type pos = 0) const noexcept;
        /* 103 */   size_type rfind(const basic_string& str, size_type pos = npos) const noexcept;
        /* 104 */   size_type rfind(const charT* s, size_type pos, size_type n) const;
        /* 105 */   size_type rfind(const charT* s, size_type pos = npos) const;
        /* 106 */   size_type rfind(charT c, size_type pos = npos) const noexcept;
        /* 107 */   size_type find_first_of(const basic_string& str, size_type pos = 0) const noexcept;
        /* 108 */   size_type find_first_of(const charT* s, size_type pos, size_type n) const;
        /* 109 */   size_type find_first_of(const charT* s, size_type pos = 0) const;
        /* 110 */   size_type find_first_of(charT c, size_type pos = 0) const noexcept;
        /* 111 */   size_type find_last_of (const basic_string& str, size_type pos = npos) const noexcept;
        /* 112 */   size_type find_last_of (const charT* s, size_type pos, size_type n) const;
        /* 113 */   size_type find_last_of (const charT* s, size_type pos = npos) const;
        /* 114 */   size_type find_last_of (charT c, size_type pos = npos) const noexcept;
        /* 115 */   size_type find_first_not_of(const basic_string& str, size_type pos = 0) const noexcept;
        /* 116 */   size_type find_first_not_of(const charT* s, size_type pos, size_type n) const;
        /* 117 */   size_type find_first_not_of(const charT* s, size_type pos = 0) const;
        /* 118 */   size_type find_first_not_of(charT c, size_type pos = 0) const noexcept;
        /* 119 */   size_type find_last_not_of (const basic_string& str, size_type pos = npos) const noexcept;
        /* 120 */   size_type find_last_not_of (const charT* s, size_type pos, size_type n) const;
        /* 121 */   size_type find_last_not_of (const charT* s, size_type pos = npos) const;
        /* 122 */   size_type find_last_not_of (charT c, size_type pos = npos) const noexcept;
        /* 123 */   basic_string substr(size_type pos = 0, size_type n = npos) const;

        /* 124 */   auto compare( Ref_to<const basic_string> other ) const noexcept
                        -> int
                    { using nn::compare;  return compare( impl_, other.impl_ ); }
                    
        /* 125 */   int compare(size_type pos1, size_type n1, const basic_string& str) const;
        /* 126 */   int compare(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2) const;
        /* 127 */   int compare(const charT* s) const;
        /* 128 */   int compare(size_type pos1, size_type n1, const charT* s) const;
        /* 129 */   int compare(size_type pos1, size_type n1, const charT* s, size_type n2) const;
    };

    template< class CharType, class Traits, class Allocator >
    auto operator<<(
        Ref_to<basic_ostream<CharType>>                         stream,
        Ref_to<const basic_string<CharType, Traits, Allocator>> str
        ) -> Ref_to<basic_ostream<CharType>>
    {
        typename basic_ostream<CharType, Traits>::sentry k( stream );
        if( k )
        {
            // TODO: padding
            stream.write( str.__shortlived_c_str(), str.size() );
        }
        return stream;
    }

    template< class CharType, class Traits, class Allocator >
    auto getline (
        Ref_to<basic_istream<CharType, Traits>>             stream,
        Ref_to<basic_string<CharType, Traits, Allocator>>   str,
        const CharType                                      delimiter
        )
        -> Ref_to<basic_istream<CharType, Traits>>
    {
        typename basic_istream<CharType, Traits>::sentry k( stream, true );
        if( k )
        {
            str.clear();
            const Size max_chars = str.max_size() - str.size();
            char ch;
            Size n_read = 0;
            while( n_read < max_chars && Traits::not_eof( ch = stream.get() ) && ch != delimiter )
            {
                str += ch;
            }

            if( n_read == max_chars )
            {
                stream.setstate( std::ios_base::failbit );
            }
        }
        return stream;
    }

    template< class CharType, class Traits, class Allocator >
    auto getline(
        Ref_to<std::basic_istream<CharType, Traits>>        stream,
        Ref_to<basic_string<CharType, Traits, Allocator>>   str
        )
        -> Ref_to<basic_istream<CharType, Traits>>
    { return getline( stream, str, stream.widen( '\n' ) ); }

    //template<class CharType, class Traits, class Allocator>
    //    basic_istream<CharType, Traits>& getline (
    //        basic_istream<CharType, Traits>&& _IStream,
    //        basic_string<CharType, Traits, Allocator>& _Str
    //    ); 
    // template<class CharType, class Traits, class Allocator>
    //    basic_istream<CharType, Traits>& getline (
    //        basic_istream<CharType, Traits>&& _IStream,
    //        basic_string<CharType, Traits, Allocator>& _Str,
    //        CharType _Delimiter
    //    );

    template< class CharType, class Traits, class Allocator >
    auto operator!=(
        Ref_to<const basic_string<CharType, Traits, Allocator>> a,
        Ref_to<const basic_string<CharType, Traits, Allocator>> b
        )
        -> bool
    { return a.compare( b ) != 0; }

    template< class CharType, class Traits, class Allocator >
    auto operator<(
        Ref_to<const basic_string<CharType, Traits, Allocator>> a,
        Ref_to<const basic_string<CharType, Traits, Allocator>> b
        )
        -> bool
    { return a.compare( b ) < 0; }

    template< class CharType, class Traits, class Allocator >
    auto operator<=(
        Ref_to<const basic_string<CharType, Traits, Allocator>> a,
        Ref_to<const basic_string<CharType, Traits, Allocator>> b
        )
        -> bool
    { return a.compare( b ) <= 0; }

    template< class CharType, class Traits, class Allocator >
    auto operator==(
        Ref_to<const basic_string<CharType, Traits, Allocator>> a,
        Ref_to<const basic_string<CharType, Traits, Allocator>> b
        )
        -> bool
    { return a.compare( b ) == 0; }

    template< class CharType, class Traits, class Allocator >
    auto operator>=(
        Ref_to<const basic_string<CharType, Traits, Allocator>> a,
        Ref_to<const basic_string<CharType, Traits, Allocator>> b
        )
        -> bool
    { return a.compare( b ) >= 0; }

    template< class CharType, class Traits, class Allocator >
    auto operator>(
        Ref_to<const basic_string<CharType, Traits, Allocator>> a,
        Ref_to<const basic_string<CharType, Traits, Allocator>> b
        )
        -> bool
    { return a.compare( a, b ) > 0; }

    using string = basic_string<char>;
    using wstring = basic_string<wchar_t>;
}  // namespace nn
