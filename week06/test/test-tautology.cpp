#include "include/catch.hpp"

#include "query_generator.hpp"

using namespace cs453;
using namespace std;

bool inline testGenerateQuery(const char *username, const char *password, const char *expected) {
    string user = string(username);
    string pass = string(password);
    string query = generateQuery(user, pass);
    string expect = string(expected);
    if (query != expect) {
        printf("FAILED: %s != %s", query.c_str(), expected);
    }
    return query == expect;
}

TEST_CASE( "Tautology attack test cases", "[tautology]" )
{
    REQUIRE( testGenerateQuery("tom", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing' OR 'x' = 'x';") );
}
