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

TEST_CASE( "Comment attack test cases", "[comment]" )
{
    REQUIRE( testGenerateQuery("Root'; -- ", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'Root'; -- ' AND password = 'nothing' OR 'x' = 'x';") );
}

TEST_CASE( "Comment attack with weak mitigation", "[comment-weak]")
{
    REQUIRE( testGenerateQuery("Root'; -- ", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'Root; -- ' AND password = 'nothing OR x = x';", QueryStateWeakMitigation) );
    REQUIRE_FALSE( testGenerateQuery("Root'; -- ", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'Root'; -- ' AND password = 'nothing' OR 'x' = 'x';", QueryStateWeakMitigation) );
}

TEST_CASE ( "Comment attack with strong mitigation", "[comment-strong]")
{
    REQUIRE( testGenerateQuery("Root'; -- ", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'Root' AND password = 'nothing';", QueryStateStrongMitigation) );
    REQUIRE_FALSE( testGenerateQuery("Root'; -- ", "nothing' OR 'x' = 'x", "SELECT COUNT(*) FROM users WHERE username = 'Root'; -- ' AND password = 'nothing' OR 'x' = 'x';", QueryStateStrongMitigation) );
}