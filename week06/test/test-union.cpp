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

TEST_CASE( "Union query attack test cases", "[union]" )
{
    REQUIRE( testGenerateQuery("tom", "nothing' UNION SELECT authenticate FROM passwordList", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing' UNION SELECT authenticate FROM passwordList';") );
}

TEST_CASE( "Union query attack with weak mitigation", "[union-weak]" )
{
    REQUIRE( testGenerateQuery("tom", "nothing' UNION SELECT authenticate FROM passwordList", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing UNION SELECT authenticate FROM passwordList';", QueryStateWeakMitigation) );
    REQUIRE_FALSE( testGenerateQuery("tom", "nothing' UNION SELECT authenticate FROM passwordList", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing' UNION SELECT authenticate FROM passwordList';", QueryStateWeakMitigation) );
}

TEST_CASE( "Union query attack with strong mitigation", "[union-strong]" )
{
    REQUIRE( testGenerateQuery("tom", "nothing' UNION SELECT authenticate FROM passwordList", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing';", QueryStateStrongMitigation) );
    REQUIRE_FALSE( testGenerateQuery("tom", "nothing' UNION SELECT authenticate FROM passwordList", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing' UNION SELECT authenticate FROM passwordList';", QueryStateStrongMitigation) );
}