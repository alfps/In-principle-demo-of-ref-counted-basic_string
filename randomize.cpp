#include <cppx/core/type_builders.hpp>
#include <nn/string.hpp>

#include <algorithm>        // std::random_shuffle
#include <iostream>
#include <utility>          // std::move
#include <vector>           // std::vector
using cppx::Ref_to;
using namespace std;

auto main() -> int
{
    vector<nn::string> lines;
    for( nn::string line;  getline( cin, line ); )
    {
        lines.push_back( move( line ) );
    }
    random_shuffle( lines.begin(), lines.end() );
    for( const auto s : lines )
    {
        cout << s << " " << s << " " << s << " " << s << endl;
    }
}
