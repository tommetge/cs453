#include "include/catch.hpp"

#include "query_generator.hpp"

using namespace cs453;
using namespace std;

bool inline testGenerateQuery(const char *username, const char *password, const char *expected, QueryState state = QueryStateNoMitigation) {
    string user = string(username);
    string pass = string(password);
    string query = generateQuery(user, pass, state);
    string expect = string(expected);
    if (query != expect) {
        printf("\nFAILED:\n\t%s\n!=\n\t%s\n", query.c_str(), expected);
    }
    return query == expect;
}

TEST_CASE( "Valid test cases", "[valid]" )
{
    REQUIRE( testGenerateQuery("tom", "pass", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'pass';"));

    REQUIRE( testGenerateQuery("tom2@example1.com", "pass1234!", "SELECT COUNT(*) FROM users WHERE username = 'tom2@example1.com' AND password = 'pass1234!';"));

    REQUIRE( testGenerateQuery("sam@samluc.com", "null", "SELECT COUNT(*) FROM users WHERE username = 'sam@samluc.com' AND password = 'null';"));
}

TEST_CASE( "Valid test cases with weak mitigation", "[valid-weak]" )
{
    REQUIRE( testGenerateQuery("tom", "pass", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'pass';", QueryStateWeakMitigation));
    REQUIRE( testGenerateQuery("tom2@example1.com", "pass1234!", "SELECT COUNT(*) FROM users WHERE username = 'tom2@example1.com' AND password = 'pass1234!';", QueryStateWeakMitigation));
}

TEST_CASE( "Valid test cases with strong mitigation", "[valid-strong]" )
{
    REQUIRE( testGenerateQuery("tom", "pass", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'pass';", QueryStateStrongMitigation));
    REQUIRE( testGenerateQuery("tom2@example1.com", "pass1234!", "SELECT COUNT(*) FROM users WHERE username = 'tom2@example1.com' AND password = 'pass1234!';", QueryStateStrongMitigation));
}