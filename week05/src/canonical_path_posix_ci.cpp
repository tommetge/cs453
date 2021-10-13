#include "canonical_path_posix_ci.hpp"

using namespace cs453;

// POSIX-specific canonical form of a path (case-insensitive)
std::string CanonicalPathPOSIXCI::canonicalForm()
{
    return path;
}
