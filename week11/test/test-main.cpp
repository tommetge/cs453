#define CATCH_CONFIG_MAIN
#include "include/catch.hpp"

#include "pw.hpp"

using namespace std;

TEST_CASE( "Alphanumeric (lowercase)", "[alpha]" )
{
	std::string test("a");
	for (long i='a'; i<='z'; i++) {
		test = string(1, (char)i);
		REQUIRE(combinations(test) == 26);
		REQUIRE(bitSize(test) == 4);
	}

    test = "az";
    REQUIRE(combinations(test) == 676);
	REQUIRE(bitSize(test) == 9);
    test = "za";
    REQUIRE(combinations(test) == 676);
	REQUIRE(bitSize(test) == 9);
}

TEST_CASE( "Alphanumeric (uppercase)", "[ALPHA]" )
{
    std::string test("A");
	for (long i='A'; i<='Z'; i++) {
		test = string(1, (char)i);
		REQUIRE(combinations(test) == 26);
		REQUIRE(bitSize(test) == 4);
	}

    test = "AZ";
    REQUIRE(combinations(test) == 676);
	REQUIRE(bitSize(test) == 9);
    test = "ZA";
    REQUIRE(combinations(test) == 676);
	REQUIRE(bitSize(test) == 9);
}

TEST_CASE( "Symbols", "[symbols]" )
{
    std::string test("!");
    for (long i=0; i<symRangesSize; i++) {
    	for (long j=symRanges[i][0]; j<=symRanges[i][1]; j++) {
    		test = string(1, (char)j);
    		REQUIRE(combinations(test) == 32);
		    REQUIRE(bitSize(test) == 5);
    	}
    }
    test = "!~";
    REQUIRE(combinations(test) == 1024);
    REQUIRE(bitSize(test) == 10);
    test = "~!";
    REQUIRE(combinations(test) == 1024);
    REQUIRE(bitSize(test) == 10);
}

TEST_CASE( "Numbers", "[numbers]" )
{
    std::string test("0");
    for (long i='0'; i<10; i++) {
    	test = string(1, (char)i);
	    REQUIRE(combinations(test) == 10);
	}
    test = "1234";
    REQUIRE(combinations(test) == 10000);
    REQUIRE(bitSize(test) == 13);
}

TEST_CASE( "Combinations", "[combined]" )
{
    std::string test("aA");
    REQUIRE(combinations(test) == 2704);
    REQUIRE(bitSize(test) == 11);
    test = string(("Aa"));
    REQUIRE(combinations(test) == 2704);
    REQUIRE(bitSize(test) == 11);

    test = string(("a0"));
    REQUIRE(combinations(test) == 1296);
    REQUIRE(bitSize(test) == 10);

    test = string(("aB0"));
    REQUIRE(combinations(test) == 238328);
    REQUIRE(bitSize(test) == 17);

    test = string(("aZ4%"));
    REQUIRE(combinations(test) == 78074896);
    REQUIRE(bitSize(test) == 26);
}
