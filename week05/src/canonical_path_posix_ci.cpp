#include "canonical_path_posix_ci.hpp"

#include <algorithm>
#include <string>

using namespace cs453;

std::string toLowerStr(std::string str)
{
    std::string lowered;
    lowered.resize(str.size());
    std::transform(str.begin(), str.end(), lowered.begin(), ::tolower);
    return lowered;
}

// POSIX-specific canonical form of a path (case-insensitive)
std::string CanonicalPathPOSIXCI::canonicalForm()
{
    // Lowercase the path and compare with the case-sensitive version
    path = toLowerStr(path);
    return CanonicalPathPOSIX::canonicalForm();
}
