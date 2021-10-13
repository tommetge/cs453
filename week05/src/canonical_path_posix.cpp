#include "canonical_path_posix.hpp"

#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace cs453;
using namespace std;

const char PATH_DELIMITER = '/';

// POSIX-specific canonical form of a path (case-sensitive)
string CanonicalPathPOSIX::canonicalForm()
{
    // Root is already normalized
    if (path == "/" || path.size() == 0) {
        return path;
    }

    // Tokenize the path by the PATH_DELIMITER (/).
    // This is an easy way for us to start working with the path
    // in a hierarchical fashion.
    vector<string> pathComponents;
    stringstream pathStream(path);
    string component;

    while(getline(pathStream, component, PATH_DELIMITER)) {
        // If we see '..', remove the most recent path component
        // from our list and move onto the next component.
        if (component == "..") {
            if (pathComponents.size() > 0) {
                pathComponents.pop_back();
            }
            continue;
        }

        // Skip empty path components and '.'
        if (component.length() > 0 && component != ".") {
            pathComponents.push_back(component);
        }
    }

    // If there are no path components after the above, it's
    // either a root ('/') or relative root ('').
    if (pathComponents.size() == 0) {
        if (path[0] == PATH_DELIMITER) {
            return "/";
        }

        return "";
    }

    // Join our path components with PATH_DELIMITER ('/') to create
    // our canonical path.
    string canonicalPath = accumulate(
        next(pathComponents.begin()),
             pathComponents.end(),
             pathComponents[0],
             [](string a, string b)
        {
            return a + PATH_DELIMITER + b;
        }
    );

    // We inadvertently removed the leading '/', if one was present.
    // Add it back if needed.
    if (path[0] == PATH_DELIMITER && pathComponents[0] != "/") {
        ostringstream os;
        os << "/" << canonicalPath;
        canonicalPath = os.str();
    }

    return canonicalPath;
}
