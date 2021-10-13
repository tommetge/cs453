#include <iostream>
#include <string>

#include "canonical_path_posix.hpp"

using namespace std;
using namespace cs453;

int main()
{
    string filename1, filename2;

    cout << "Specify the first filename: ";
    getline(cin, filename1);
    cout << "Specify the second filename: ";
    getline(cin, filename2);

    // Update with the specific sub-class desired
    CanonicalPath path1(filename1);
    CanonicalPath path2(filename2);

    if (path1.compare(path2)) {
        cout << "The paths are homographs" << endl;
        return 0;
    }

    cout << "The paths are NOT homographs" << endl;
    return 1;
}