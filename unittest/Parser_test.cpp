#include "Parser.h"

TEST( ctor , str ) 
{
    auto arr = Parser::Parse("ls -al");
    ASSERT_EQ( arr[0].originStr , "ls -al" );
    ASSERT_FALSE( arr[0].isSyntaxError );

    auto arr2 = Parser::Parse("ls -al aaa bbb ccc | grep test | hi");
    ASSERT_EQ( arr2[0].originStr , "ls -al aaa bbb ccc" );
    ASSERT_EQ( arr2[1].originStr , "grep test" );
    ASSERT_EQ( arr2[2].originStr , "hi" );
    ASSERT_FALSE( arr2[0].isSyntaxError );
    ASSERT_FALSE( arr2[1].isSyntaxError );
    ASSERT_FALSE( arr2[2].isSyntaxError );
}
