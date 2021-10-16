#include "canonical_path_macos_ci.hpp"
#include "canonical_path_posix_ci.hpp"

#include <sstream>
#include <string>
#include <vector>

using namespace cs453;
using namespace std;

const char FORK_DELIMITER = ':';

// POSIX-specific canonical form of a path (case-insensitive)
std::string CanonicalPathMacOSCI::canonicalForm()
{
    std::string preprocessed = CanonicalPathMacOS::canonicalForm();
    // Use CanonicalPathPOSIXCI to further refine.
    CanonicalPathPOSIXCI posixCI(preprocessed);

    return posixCI.canonicalForm();
}
