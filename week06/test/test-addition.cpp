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

TEST_CASE( "Addition attack test cases", "[addition]" )
{
    REQUIRE( testGenerateQuery("tom", "nothing'; INSERT INTO passwordList (name, passwd) VALUES 'Bob', '1234", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = 'nothing'; INSERT INTO passwordList (name, passwd) VALUES 'Bob', '1234';") );

    REQUIRE( testGenerateQuery("sam", "nothing'; INSERT INTO passwordList (name, passwd) VALUES 'Bob', '1234", "SELECT COUNT(*) FROM users WHERE username = 'sam' AND password = 'nothing'; INSERT INTO passwordList (name, passwd) VALUES 'sam', 'samluvu';") );
}

TEST_CASE( "Addition attack with weak mitigation", "[addition-weak]")
{
    REQUIRE( testGenerateQuery("tom", " nothing'; INSERT INTO passwordList (name, passwd) VALUES 'Bob', '1234", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = ' nothing; INSERT INTO passwordList (name, passwd) VALUES Bob, 1234';", QueryStateWeakMitigation) );
    REQUIRE_FALSE( testGenerateQuery("tom", " nothing'; INSERT INTO passwordList (name, passwd) VALUES 'Bob', '1234", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = ' nothing'; INSERT INTO passwordList (name, passwd) VALUES 'Bob', '1234';", QueryStateWeakMitigation) );
}

TEST_CASE( "Addition attack with strong mitigation", "[addition-strong]")
{
    REQUIRE( testGenerateQuery("tom", " nothing'; INSERT INTO passwordList (name, passwd) VALUES 'Bob', '1234", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = '';", QueryStateStrongMitigation) );
    REQUIRE_FALSE( testGenerateQuery("tom", " nothing'; INSERT INTO passwordList (name, passwd) VALUES 'Bob', '1234", "SELECT COUNT(*) FROM users WHERE username = 'tom' AND password = ' nothing'; INSERT INTO passwordList (name, passwd) VALUES 'Bob', '1234';", QueryStateStrongMitigation) );
}