#include "query_generator.hpp"

#include <stdio.h>

using namespace std;

static const char* SQLAuthStatement = "SELECT COUNT(*) FROM users WHERE username = '%s' AND password = '%s';";

string cs453::generateQuery(string &username, string &password) {
    char buff[1024];
    snprintf(buff, 1024, SQLAuthStatement, username.c_str(), password.c_str());

    return string(buff);
}
