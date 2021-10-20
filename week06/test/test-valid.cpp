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

TEST_CASE( "Valid test cases", "[valid]" )
{
    REQUIRE( testGenerateQuery("tom", "pass", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'pass';"));
    REQUIRE( testGenerateQuery("tom2@example1.com", "pass1234!", "SELECT COUNT(*) FROM users WHERE username = 'tom2@example1.com' AND password = 'pass1234!';"));
}
