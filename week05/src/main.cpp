#include <iostream>
#include <string>

using namespace std;

string canonicalPath(string path) {
    return path;
}

bool comparePaths(string path1, string path2) {
    return canonicalPath(path1).compare(canonicalPath(path2)) == 0;
}

int main() {
    string filename1, filename2;

    cout << "Specify the first filename: ";
    getline(cin, filename1);
    cout << "Specify the second filename: ";
    getline(cin, filename2);

    if (comparePaths(filename1, filename2)) {
        cout << "The paths are homographs" << endl;
        return 0;
    }

    cout << "The paths are NOT homographs" << endl;
    return 1;
}