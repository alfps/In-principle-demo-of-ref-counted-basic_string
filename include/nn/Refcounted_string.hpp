#pragma once
#include <nn/compare.hpp>

#include <cppx/basic_types/Index.hpp>       // cppx::Index
#include <cppx/basic_types/Size.hpp>        // cppx::Size
#include <cppx/core/type_builders.hpp>      // cppx::Ptr_to, cppx::Ref_to, cppx::Rvalue_ref_to

#include <algorithm>        // std::copy, std::swap, std::min
#include <assert.h>         // assert
#include <memory>           // std::allocator, std::shared_ptr
#include <memory.h>         // memcmp, wmemcmp
#include <string>           // std::char_traits;
#include <utility>          // std::move;
#include <vector>           // std::vector

namespace nn {      // no nonsense
    using cppx::Index;
    using cppx::Ptr_to;
    using cppx::Ref_to;
    using cppx::Rvalue_ref_to;
    using cppx::Size;

    using std::allocator;
    using std::char_traits;
    using std::copy;
    using std::min;
    using std::move;
    using std::shared_ptr;

    namespace impl {
        using std::vector;

        enum Repeat_char {};
        enum Copy_data_from {};

        template< class Char_t, class Allocator >
        class String_buffer_
        {
        private:
            using This = String_buffer_;
            String_buffer_( Rvalue_ref_to<This> ) = delete;     // Not movable
            String_buffer_( Ref_to<const This> ) = delete;      // Not directly copyable

        public:
            vector< Char_t, Allocator > data_;
            bool                        has_external_ref_;

            auto is_shareable() const -> bool   { return not has_external_ref_; }
            auto size() const -> Size           { return data_.size() - 1; }
            auto capacity() const -> Size       { return data_.capacity() - 1; }

            void ensure_capacity( const Size n )
            {
                data_.reserve( n + 1 );
            }

            void resize( const Size new_size, const Char_t fill_char = 0 )
            {
                if( new_size > size() )
                {
                    data_[size()] = fill_char;
                }
                data_.resize( new_size + 1, fill_char );
                data_[new_size] = 0;
            }

            String_buffer_( Ref_to<const Allocator> allocator )
                : data_( 1, 0, allocator )
                , has_external_ref_( false )
            {}

            String_buffer_(
                Repeat_char, const Char_t ch, const Size n, Ref_to<const Allocator> allocator
                )
                : data_( n + 1, ch, allocator )
                , has_external_ref_( false )
            { data_[n] = 0; }

            template< class Input_iterator >
            String_buffer_(
                const Input_iterator begin, const Input_iterator end, Ref_to<const Allocator> allocator
                )
                : data_( begin, end, allocator )
                , has_external_ref_( false )
            { data_.push_back( 0 ); }

            String_buffer_( Copy_data_from, Ref_to<const String_buffer_> other )
                : data_( other.data_.begin(), other.data_.end(), other.data_.get_allocator() )
                , has_external_ref_( false )
            {}

            String_buffer_(
                Copy_data_from,
                Ref_to<const String_buffer_>    other,
                Ref_to<const Allocator>         allocator
                )
                : data_( other.data_.begin(), other.data_.end(), allocator )
                , has_external_ref_( false )
            {}
        };
    }  // namespace impl

    template< class Char_t, class Traits = char_traits<Char_t>, class Allocator = allocator<Char_t> >
    class Refcounted_string_
    {
    private:
        using This = Refcounted_string_;
        using String_buffer = impl::String_buffer_<Char_t, Allocator>;

        mutable shared_ptr<String_buffer> p_buffer_;

        void ensure_unique_buffer() const
        {
            if( not p_buffer_.unique() )
            {
                p_buffer_.reset( new String_buffer( impl::Copy_data_from(), *p_buffer_ ) );
            }
        }

        void note_buffer_ref() const
        {
            p_buffer_->has_external_ref_ = true;
        }

        Refcounted_string_(
            impl::Repeat_char, const Char_t ch, const Size n, Ref_to<const Allocator> a
            )
            : p_buffer_( new String_buffer( impl::Repeat_char(), ch, n, a ) )
        {}

    public:
        using Size  = cppx::Size;
        using Index = cppx::Index;

        using It    = typename std::vector< Char_t, Allocator >::iterator;
        using C_it  = typename std::vector< Char_t, Allocator >::const_iterator;

        auto c_str() const
            -> Ptr_to<const Char_t>
        { ensure_unique_buffer();  note_buffer_ref();  return &p_buffer_->data_[0]; }

        auto shortlived_c_str() const
            -> Ptr_to<const Char_t>
        { return &p_buffer_->data_[0]; }

        auto size() const noexcept
            -> Size
        { return p_buffer_->size(); }

        auto capacity() const noexcept
            -> Size
        { return p_buffer_->capacity(); }

        void resize( const Size new_size, const Char_t fill_char = 0 )
        {
            ensure_unique_buffer();
            p_buffer_->resize( new_size, fill_char );
        }

        void ensure_capacity( Size n )
        {
            p_buffer_->ensure_capacity( n );
        }

        void shrink_to_fit()
        {
            // TODO:
            p_buffer_.reset( new String_buffer( impl::Copy_data_from(), *p_buffer_ ) );
        }

        auto operator[]( const Index i )
            -> Ref_to<Char_t>
        { ensure_unique_buffer();  note_buffer_ref();  return p_buffer_->data_[i]; }

        auto operator[]( const Index i ) const
            -> Ref_to<const Char_t>
        { ensure_unique_buffer();  note_buffer_ref();  return p_buffer_->data_[i]; }

        auto begin()
            -> It
        { ensure_unique_buffer();  note_buffer_ref();  return p_buffer_->data_.begin(); }

        auto begin() const
            -> C_it
        { ensure_unique_buffer();  note_buffer_ref();  return p_buffer_->data_.begin(); }

        auto end()
            -> It
        { ensure_unique_buffer();  note_buffer_ref();  return p_buffer_->data_.end() - 1; }

        auto end() const
            -> C_it
        { ensure_unique_buffer();  note_buffer_ref();  return p_buffer_->data_.end() - 1; }

        friend
        void swap( Ref_to<This> a, Ref_to<This> b )
            noexcept
        {
            using std::swap;
            swap( a.p_buffer_, b.p_buffer_ );
        }

        auto operator=( Ref_to<const This> other )
            -> Ref_to<This>
        {
            This copy = other;
            swap( *this, copy );
            return *this;
        }

        auto operator=( Rvalue_ref_to<This> other )
            -> Ref_to<This>
        {
            This the_others_essence = move( other );
            swap( *this, the_others_essence );
            return *this;
        }

        void operator+=( Ref_to<const This> other )
        {
            const Index n_self = size();
            resize( n_self + other.size() );
            assert( p_buffer_.unique() );
            Ref_to<String_buffer> impl = *p_buffer_;
            copy( other.begin(), other.end(), begin() + n_self );
        }

        void operator+=( const Char_t ch )
        {
            resize( size() + 1, ch );
        }

        Refcounted_string_( Ref_to<const Allocator> allocator = Allocator() )
            : p_buffer_( new String_buffer( allocator ) )
        {}

        template< class Input_iterator >
        Refcounted_string_(
            const Input_iterator begin, const Input_iterator end, Ref_to<const Allocator> a = Allocator()
            )
            : p_buffer_( new String_buffer( begin, end, a ) )
        {}

        Refcounted_string_( const Ptr_to<const Char_t> s )
            : Refcounted_string_( s, s + Traits::length( s ) )
        {}

        // Ordinary copy constructor.
        Refcounted_string_( Ref_to<const This> other )
            : p_buffer_( other.p_buffer_ )
        {
            if( not p_buffer_->is_shareable() )
            {
                ensure_unique_buffer();
            }
        }

        Refcounted_string_(
            Ref_to<const This>          other,
            Ref_to<const Allocator>     a
            )
            : p_buffer_( new String_buffer( impl::Copy_data_from(), *other.p_buffer_, a ) )
        {}

        Refcounted_string_( Rvalue_ref_to<This> other ) noexcept
            : p_buffer_( move( other.p_buffer_ ) )
        { other.p_buffer_.reset(); }

        static
        auto repeat( const Char_t ch, const Size n, Ref_to<const Allocator> a )
            -> This
        { return This( impl::Repeat_char(), ch, n, a ); }
    };

    template< class Char_t >
    auto compare(
        Refcounted_string_<Char_t> const& a,
        Refcounted_string_<Char_t> const& b
        )
        -> int
    { return compare( a.shortlived_c_str(), a.size(), b.shortlived_c_str(), b.size() ); }

    using Refcounted_string     = Refcounted_string_<char>;
    using Refcounted_wstring    = Refcounted_string_<wchar_t>;
}  // namespace nn
