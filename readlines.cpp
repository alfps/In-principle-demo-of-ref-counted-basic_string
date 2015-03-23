#include <cppx/core/type_builders.hpp>
#include <stud/string.hpp>
#include <iostream>
using cppx::Ref_to;
using namespace std;

auto main() -> int
{
    stud::string line;
    while( getline( cin, line ) )
    {
        cout << line << endl;
    }
}
