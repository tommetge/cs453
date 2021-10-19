#include <iostream>
#include <string>

#include "query_generator.hpp"

using namespace std;
using namespace cs453;

int main()
{
    string username, password;

    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    // Generate the query string and print it
    cout << generateQuery(username, password) << endl;

    return 0;
}