#include "canonical_path_macos.hpp"

#include <sstream>
#include <string>
#include <vector>

using namespace cs453;
using namespace std;

const char FORK_DELIMITER = ':';

// POSIX-specific canonical form of a path (case-sensitive)
std::string CanonicalPathMacOS::canonicalForm()
{
    // Root is already normalized
    if (path == "/" || path.size() == 0) {
        return path;
    }

    // Tokenize the path by the FORK_DELIMITER (:). On HFS for
    // macOS, resource forks are accessed via POSIX APIS using
    // FORK_DELIMITER, making it possible for homographic paths
    // that differ by anything after the FORK_DELIMITER.

    vector<string> pathComponents;
    stringstream pathStream(path);
    string standardPath;

    if (getline(pathStream, standardPath, FORK_DELIMITER)) {
        path = standardPath;
        return CanonicalPathPOSIX::canonicalForm();
    }

    return CanonicalPathPOSIX::canonicalForm();
}
