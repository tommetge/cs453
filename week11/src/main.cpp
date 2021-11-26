#include <iostream>
#include <string>

#include "pw.hpp"

using namespace std;

int main(int argc, char **argv) {
	string input;
	cout << "Please enter the password: ";
	cin >> input;

	cout << "\tThere are " << combinations(input) << " combinations" << endl;
	cout << "\tThat is equivalent to a key of " << bitSize(input) << " bits" << endl;

	return 0;
}
