#ifndef CANONICAL_PATH_MACOS_CI_HPP
#define CANONICAL_PATH_MACOS_CI_HPP

#include "canonical_path_posix_ci.hpp"

namespace cs453
{

// MacOS-specific canonicalization of a path. Note that this is a
// sub-class of CanonicalPathPOSIX: it will build on the POSIX
// canonicalization. This is the case-insensitive version.
class CanonicalPathMacOSCI : public CanonicalPathPOSIXCI
{
public:
    using CanonicalPathPOSIXCI::CanonicalPathPOSIXCI;

    std::string canonicalForm();
};

} // namespace cs453

#endif // CANONICAL_PATH_MACOS_CI_HPP
