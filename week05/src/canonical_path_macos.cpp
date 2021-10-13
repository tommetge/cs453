#include "canonical_path_macos.hpp"

using namespace cs453;

// POSIX-specific canonical form of a path (case-sensitive)
std::string CanonicalPathMacOS::canonicalForm()
{
    return path;
}
