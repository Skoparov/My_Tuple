#ifndef TESTS_H
#define TESTS_H

#include <iostream>

#include "../my_tuple.h"
#include "../details/type_present.h"

using namespace basic;

namespace test
{
    void tuple_assert( bool val, std::string error )
    {
        if( !val ){
            throw std::runtime_error( error );
        }
    }

    void test_args_size()
    {
        static_assert( details::args_size< int, double, int >() == 3, "args_size fail");
    }

    void test_compare_with_types()
    {
        static_assert( details::compare_with_types< int, int >( 1, 1 ), "compare with types fail" );
        static_assert( !details::compare_with_types< int, int >( 1, 2 ), "compare with types fail" );
        static_assert( !details::compare_with_types< int, double >( 1, 1.0 ), "compare with types fail" );
    }

    void test_first_of_type()
    {
        constexpr my_tuple< int, float, double, int, float, double > v;

        static_assert( first_of_type< float >( v ) == 1, "first_of_type fail" );
        static_assert( first_of_type< float, 2 >( v ) == 4, "first_of_type fail" );
        static_assert( first_of_type< short >( v ) == 6, "first_of_type fail" );
    }

    void test_for_each_of_type()
    {
        my_tuple< std::string, double, std::string > v{ std::string("qwerty"), 3.3, std::string("qwerty") };

        auto f = [](std::string& s){ s += "q"; };
        for_each_of_type< std::string >( v, f );

        tuple_assert( get< 0 >( v ) == std::string("qwertyq"), "for_each_of_type fail\n" );
        tuple_assert( get< 2 >( v ) == std::string("qwertyq"), "for_each_of_type fail\n" );
    }

    void test_tuple_class()
    {
        my_tuple< int, float, double, std::string > v1;
        constexpr my_tuple< int, float, double > v2{ 1, 2.2f, 3.3 };
        constexpr my_tuple< int, float, double > v3 = make_my_tuple( 1, 2.2f, 3.3 );

        static_assert( v2 == v3, "tuple creation fail" );
    }

    void test_get()
    {
        constexpr my_tuple< int, float, double > v1{ 1, 2.2f, 3.3 };
        static_assert( get<0>(v1) == 1, "get fail" );
        static_assert( get< 1 >(v1) == 2.2f, "get fail" );
        static_assert( get< 2 >(v1) == 3.3, "get fail" );
    }

    void test_compare()
    {
        constexpr my_tuple< int, float, double > v1{ 1, 2.2f, 3.3 };
        constexpr my_tuple< int, float, double > v2{ 1, 2.2f, 4.3 };
        constexpr auto copy_v1 = v1;

        static_assert( copy_v1 == v1, "compare_fail" );
        static_assert( !( copy_v1 != v1 ), "compare_fail" );
        static_assert( v2 != v1, "compare_fail" );
        static_assert( !( v2 == v1 ), "compare_fail" );

    }

    void test_copy()
    {
        my_tuple< int, float, double > v { 1, 2.2f, 3.3 };
        my_tuple< int, float, double > v1;
        my_tuple< int, float, double > v2{ 2, 0, 0 };

        copy_tuple(v, v1);
        tuple_assert( v == v1 , "copy fail\n" );

        copy_tuple< 0, 0, 3 >(v, v1);
        tuple_assert( v== v1 , "copy fail\n" );

        copy_tuple< 1, 1, 2 >(v, v2);
        tuple_assert( get< 1 >(v) == get< 1 >(v2) &&
                      get< 2 >(v) == get< 2 >(v2) &&
                      get< 0 >(v) != get< 0 >(v2),
                      "copy fail\n" );
    }

    void test_concat()
    {
        constexpr my_tuple< int, float, double > v { 1, 2.2f, 3.3 };
        constexpr my_tuple< int, float, double > v1{ 3, 4.4f, 5.5 };

        auto v3 = concat_tuples( v, v1 );
        static_assert( std::is_same< my_tuple< int, float, double, int, float, double >, decltype( v3 ) >::value,
                       "concat fail" );

        tuple_assert( get< 0 >( v3 ) == get< 0 >( v ), "concat fail" );
        tuple_assert( get< 1 >( v3 ) == get< 1 >( v ), "concat fail" );
        tuple_assert( get< 2 >( v3 ) == get< 2 >( v ), "concat fail" );
        tuple_assert( get< 3 >( v3 ) == get< 0 >( v1 ), "concat fail" );
        tuple_assert( get< 4 >( v3 ) == get< 1 >( v1 ), "concat fail" );
        tuple_assert( get< 5 >( v3 ) == get< 2 >( v1 ), "concat fail" );
    }

    void test_type_present()
    {
        static_assert( type_present< int, float, double, int >::value, " type_present fail" );
        static_assert( !type_present< short, float, double, int >::value, " type_present fail" );

        constexpr my_tuple< int, float, double > v;
        static_assert( type_present_t< int >( v ), " type_present fail" );
        static_assert( !type_present_t< short >( v ), " type_present fail" );
    }

    void test_find()
    {
        constexpr my_tuple< int, float, int, double, int > test1 = make_my_tuple( 0, 1.1f, 2, 2.2, 2 );
        static_assert( tuple_find< int >( 2, test1 ) == 2, "find fail" );
        static_assert( tuple_find< int, 3 >( 2, test1 ) == 4, "find fail" );
        static_assert( tuple_find< int >( 5, test1 ) == 5, "find fail" );
        static_assert( tuple_find< short >( 2, test1 ) == 5, "find fail" );
    }

    void run_all_tests()
    {
        try
        {
            test_args_size();
            test_compare_with_types();
            test_first_of_type();
            test_for_each_of_type();
            test_tuple_class();
            test_get();
            test_compare();
            test_copy();
            test_concat();
            test_find();
        }
        catch( const std::exception& e )
        {
            std::cout<<e.what()<<std::endl;
            return;
        }

        std::cout<<"Runtime tests ok\n";
    }
}

#endif
