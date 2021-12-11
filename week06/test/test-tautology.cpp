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

TEST_CASE( "Tautology attack test cases", "[tautology]" )
{
    REQUIRE( testGenerateQuery("tom", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing' OR 'x' = 'x';") );

    REQUIRE( testGenerateQuery("sam", "nothing' OR 'god' = 'god", "SELECT COUNT(*) FROM users WHERE username = 'sam' AND password = 'nothing' OR 'god' = 'god"';") );
}

TEST_CASE( "Tautology attack with weak mitigation", "[tautology-weak]")
{
    REQUIRE( testGenerateQuery("tom", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing OR x = x';", QueryStateWeakMitigation) );
    REQUIRE_FALSE( testGenerateQuery("tom", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing' OR 'x' = 'x';", QueryStateWeakMitigation) );
}

TEST_CASE( "Tautology attack with strong mitigation", "[tautology-strong]")
{
    REQUIRE( testGenerateQuery("tom", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing';", QueryStateStrongMitigation) );
    REQUIRE_FALSE( testGenerateQuery("tom", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing' OR 'x' = 'x';", QueryStateStrongMitigation) );
}
