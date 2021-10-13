#ifndef CANONICAL_PATH_MACOS_HPP
#define CANONICAL_PATH_MACOS_HPP

#include "canonical_path_posix.hpp"

namespace cs453
{

// MacOS-specific canonicalization of a path. Note that this is a
// sub-class of CanonicalPathPOSIX: it will build on the POSIX
// canonicalization.
class CanonicalPathMacOS : public CanonicalPathPOSIX
{
public:
    using CanonicalPathPOSIX::CanonicalPathPOSIX;

    std::string canonicalForm();
};

} // namespace cs453

#endif // CANONICAL_PATH_MACOS_HPP
