#include <cppx/core/type_builders.hpp>
#include <nn/string.hpp>

#include <algorithm>        // std::sort
#include <iostream>
#include <string>           // std::string
#include <string.h>         // strcpy, strlen
#include <time.h>
#include <utility>          // std::move
#include <vector>           // std::vector
using cppx::Ptr_to;
using cppx::Ref_to;
using namespace std;

struct S
{
    S( Ref_to<S const> ) = delete;

    Ptr_to<char const> p;
    operator Ptr_to<char const> () const { return p; }
    ~S(){ delete[] p; }
    S( Ptr_to<char const> s ): p( strcpy( new char[strlen(s) + 1], s ) ) {}
    S( S&& other ): p( other.p ) { other.p = nullptr; }
};

template< class String >
void test( char const label[], Ref_to<const vector<S>> original_lines )
{
    const clock_t start_time = clock();
    for( int i = 1; i <= 200; ++i )
    {
        vector<String> lines( original_lines.begin(), original_lines.end() );
        sort( lines.begin(), lines.end() );
    }
    const clock_t end_time = clock();
    clog << label << ": " << double(end_time - start_time)/CLOCKS_PER_SEC << endl;

    vector<String> lines( original_lines.begin(), original_lines.end() );
    sort( lines.begin(), lines.end() );
    for( Ref_to<const String> line : lines )
    {
        cout << line << endl;
    }
}

auto main() -> int
{
    vector<S> original_lines;
    for( nn::string line;  getline( cin, line ); )
    {
        original_lines.emplace_back( line.c_str() );
    }

    test<nn::string>( "nn::string", original_lines );
    test<std::string>( "std::string", original_lines );
}
