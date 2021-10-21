#include "query_generator.hpp"
#include <regex>

#include <stdio.h>

using namespace std;

static const char* SQLAuthStatement = "SELECT COUNT(*) FROM users WHERE username = '%s' AND password = '%s';";

string inline weakMitigation(string &input) {
    // notAllowed characters = "\'"
    regex pattern("'");
    string sanatized = regex_replace(input, pattern, "");
    return sanatized;
}

string inline strongMitigation(string &input) {
    // allowedCharacters = "[a-zA-Z0-9@\#\!\$\*\?\\.]*"
    regex pattern("[a-zA-Z0-9@\\#\\!\\$\\*\\?\\.]*");
    smatch match;
    if (regex_search(input, match, pattern))
    {
        return match.str();
    }
    return "";
}

string cs453::generateQuery(string &username, string &password, QueryState state) {
    string sanatizedPassword = password;
    string sanatizedUsername = username; 
    
    switch (state)
    {
    case QueryStateWeakMitigation:
        sanatizedUsername = weakMitigation(username);
        sanatizedPassword = weakMitigation(password);
        break;
    case QueryStateStrongMitigation:
        sanatizedUsername = strongMitigation(username);
        sanatizedPassword = strongMitigation(password);
        break;
    default:
        break;
    }
    
    char buff[1024];
    snprintf(buff, 1024, SQLAuthStatement, sanatizedUsername.c_str(), sanatizedPassword.c_str());

    return string(buff);
}
