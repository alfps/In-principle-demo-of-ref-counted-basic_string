#include <stud/string.hpp>

#include <nasa/Refcounted_string.hpp>

#include <cppx/core/type_builders.hpp>
#include <iostream>
#include <list>
using cppx::Ref_to;
using namespace std;

void test_construct_copy()
{
    std::allocator<char>    alloc;
    const std::list<char>   list = { '3', '1', '4', '1', '5', '9', '2', '6', '5', '3', '5', '8', '9', '7', '9' };

    // 1
    stud::string s1a;
    stud::string s1b( alloc );
    cout << "  1: [" << s1a << "]" << endl;

    // 2
    stud::string s2( s1a );
    cout << "  2: [" << s2 << "]" << endl;

    // 3
    static auto const foo = []() -> stud::string { return stud::string( "foo" ); };
    stud::string s3( foo() );
    cout << "  3: [" << s3 << "]" << endl;

    // 4
    stud::string s4a( s3, 0 );
    stud::string s4b( s3, 0, 0 );
    stud::string s4c( s3, 0, 0, alloc );
    cout << "  4: [" << s4a << "]" << endl;

    // 5
    stud::string s5a( "Blackened", 5 );
    stud::string s5b( "Blackened", 5, alloc );
    cout << "  5: [" << s5a << "]" << endl;

    // 6
    stud::string s6a( "Whitened" );
    stud::string s6b = "Whitened";
    stud::string s6c( "Whitened", alloc );
    cout << "  6: [" << s6a << "]" << endl;

    // 7
    stud::string s7a( 40, '-' );
    stud::string s7b( 40, '-', alloc );
    cout << "  7: [" << s7a << "]" << endl;

    // 8
    stud::string s8a( list.begin(), list.end() );
    stud::string s8b( list.begin(), list.end(), alloc );
    cout << "  8: [" << s8a << "]" << endl;

    // 9
    stud::string s9a( { '3', '1', '4', '1', '5', '9', '2', '6', '5', '3', '5', '8', '9', '7', '9' } );
    stud::string s9b( { '3', '1', '4', '1', '5', '9', '2', '6', '5', '3', '5', '8', '9', '7', '9' }, alloc );
    cout << "  9: [" << s9a << "]" << endl;

    // 10
    stud::string s10a( s9a );
    stud::string s10b( s9a, alloc );
    cout << " 10: [" << s10a << "]" << endl;

    // 11
    stud::string s11( foo(), alloc );
    cout << " 11: [" << s11 << "]" << endl;

    // 12 is destructor.
    cout << " 12: destructor not tested." << endl;

    // 13
    stud::string s13;
    s13 = s8a;
    cout << " 13: [" << s13 << "]" << endl;

    // 14
    stud::string s14;
    s14 = foo();
    cout << " 14: [" << s14 << "]" << endl;

    // 15
    stud::string s15;
    s15 = "Literal";
    cout << " 15: [" << s15 << "]" << endl;

    // 16
    stud::string s16;
    s16 = 'A';
    cout << " 16: [" << s16 << "]" << endl;

    // 17
    stud::string s17;
    s17 = { '3', '1', '4', '1', '5', '9', '2', '6', '5', '3', '5', '8', '9', '7', '9' };
    cout << " 17: [" << s17 << "]" << endl;
}

void test_iterators()
{
    std::allocator<char>    alloc;
    const std::list<char>   list = { '3', '1', '4', '1', '5', '9', '2', '6', '5', '3', '5', '8', '9', '7', '9' };

    // 18 and 20
    stud::string s = "Complete";
    cout << " 18 & 20: [";
    for( char ch : s ) { cout << ch << ' '; }
    cout << "]" << endl;

    // 19 and 21
    const stud::string cs = "Complete";
    cout << " 19 & 21: [";
    for( char ch : cs ) { cout << ch << ' '; }
    cout << "]" << endl;

    // 22 and 24
    stud::string rs = "Reversed";
    cout << " 22 & 24: [";
    for( auto it = rs.rbegin(); it != rs.rend(); ++it ) { cout << *it << ' '; }
    cout << "]" << endl;

    // 23 and 25
    const stud::string crs = "Reversed";
    cout << " 23 & 25: [";
    for( auto it = crs.rbegin(); it != crs.rend(); ++it ) { cout << *it << ' '; }
    cout << "]" << endl;

    // 26 and 27
    cout << " 26 & 27: [";
    for( auto it = s.cbegin(); it != s.cend(); ++it ) { cout << *it << ' '; }
    cout << "]" << endl;

    // 28 and 29
    cout << " 28 & 29: [";
    for( auto it = rs.crbegin(); it != rs.crend(); ++it ) { cout << *it << ' '; }
    cout << "]" << endl;
}

void test_capacity()
{
    // 30
    const stud::string s = "Blah!";
    cout << " 30: [" << s << "] has size " << s.size() << "." << endl;

    // 31
    cout << " 31: [" << s << "] has length " << s.length() << "." << endl;

    // 32
    cout << " 32: [" << s << "] has max size " << s.max_size() << "." << endl;

    // 33
    stud::string varying_s = "AAAAA";
    varying_s.resize( 8, 'B' );
    cout << " 33: [" << varying_s << "] size is " << varying_s.size() << "." << endl;

    // 34
    varying_s = "zzzzz";
    varying_s.resize( 8 );
    cout << " 34: [" << varying_s << "] size is " << varying_s.size() << "." << endl;

    // 35
    cout << " 35: [" << varying_s << "] capacity is " << varying_s.capacity() << "." << endl;

    // 36
    stud::string c = "Capacious";
    c.reserve( 123 );
    cout << " 36: [" << c << "] size is " << c.size() << ", capacity is " << c.capacity() << "." << endl;

    // 37
    c.shrink_to_fit();
    cout << " 37: [" << c << "] size is " << c.size() << ", capacity is " << c.capacity() << "." << endl;

    // 38
    c.clear();
    cout << " 38: [" << c << "] size is " << c.size() << ", capacity is " << c.capacity() << "." << endl;

    // 39
    cout << " 39: [" << c << "] result of `empty()` is " << c.empty() << "." << endl;
}

void test_element_access()
{
    // 40
    const stud::string x = "ABCXYZ";
    cout << " 40: [" << x << "] [3] is " << x[3] << "." << endl;

    // 41
    stud::string v = "Fact";
    v[2] = 'r';
    cout << " 41: [" << v << "]" << endl;

    // 42
    cout << " 42: [" << x << "] at(3) is " << x[3] << "." << endl;

    // 43
    v.at(2) = 's';
    cout << " 43: [" << v << "]" << endl;

    // 44
    cout << " 44: [" << x << "].front() is " << x.front() << "." << endl;

    // 45
    v.front() = 'L';
    cout << " 45: [" << v << "].front() is " << v.front() << "." << endl;

    // 46
    cout << " 46: [" << x << "].back() is " << x.back() << "." << endl;

    // 47
    v.back() = 's';
    cout << " 47: [" << v << "].back() is " << v.back() << "." << endl;
}

void test_modifiers()
{
    // 48
    stud::string a = "Con";
    a += stud::string( "catenated" );
    cout << " 48: [" << a << "]" << endl;

    // 49
    stud::string b = "Con";
    b += "catenated";
    cout << " 49: [" << b << "]" << endl;
}

void test_operations()
{
    // 96
    const stud::string z = "C-string";
    cout << " 96: [" << z << "].c_str() is [" << z.c_str() << "]." << endl;

    // 97
    cout << " 97: [" << z << "].data() is [" << z.data() << "]." << endl;
}

auto main() -> int
{
    cout << "Construct & copy:" << endl;
    test_construct_copy();

    cout << "\n";
    cout << "Iterators:" << endl;
    test_iterators();

    cout << "\n";
    cout << "Capacity:" << endl;
    test_capacity();

    cout << "\n";
    cout << "Element access:" << endl;
    test_element_access();

    cout << "\n";
    cout << "Modifiers:" << endl;
    test_modifiers();

    cout << "\n";
    cout << "Operations:" << endl;
    test_operations();
}
