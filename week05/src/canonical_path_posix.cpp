#include "canonical_path_posix.hpp"

using namespace cs453;

// POSIX-specific canonical form of a path (case-sensitive)
std::string CanonicalPathPOSIX::canonicalForm()
{
    return path;
}
