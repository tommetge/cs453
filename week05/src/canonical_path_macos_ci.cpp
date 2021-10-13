#include "canonical_path_macos_ci.hpp"

using namespace cs453;

// POSIX-specific canonical form of a path (case-insensitive)
std::string CanonicalPathMacOSCI::canonicalForm()
{
    return path;
}
