#include "../gtest/include/gtest/gtest.h"

#include "Parser.h"

TEST( ctor , str ) 
{
    auto arr1 = Parser::Parse("ls -al");
    ASSERT_EQ( arr1[0], Command( "ls", {"-al"}, "", "", CmdRes_Ok) );

    auto arr2 = Parser::Parse("calc -a -b <in >out -c");
    ASSERT_EQ( arr2[0], Command( "calc", {"-a","-b","-c"}, "out", "in", CmdRes_Ok) );

    auto arr3 = Parser::Parse("calc -a -f <in>out -c");
    ASSERT_EQ( arr3[0], Command( "calc", {"-a","-f","-c"}, "out", "in", CmdRes_Ok) );

    auto arr4 = Parser::Parse("calc -a -b -c < in -asd -efg > out -d -e -f");
    ASSERT_EQ( arr4[0], Command( "calc", {"-a","-b","-c","-asd","-efg","-d","-e","-f"}, "out", "in", CmdRes_Ok) );

    auto arr5 = Parser::Parse("ls -al >a >b >c >d");
    ASSERT_EQ( arr5[0], Command( "ls", {"-al"}, "", "", CmdRes_DupOutRe) );

    auto arr6 = Parser::Parse("ls -al >a <b <c >d");
    ASSERT_EQ( arr6[0], Command( "ls", {"-al"}, "", "", CmdRes_DupInRe) );
}
