#ifndef CANONICAL_PATH_MACOS_CI_HPP
#define CANONICAL_PATH_MACOS_CI_HPP

#include "canonical_path_macos.hpp"

namespace cs453
{

// MacOS-specific canonicalization of a path. This is the
// case-insensitive version.
class CanonicalPathMacOSCI : public CanonicalPathMacOS
{
public:
    using CanonicalPathMacOS::CanonicalPathMacOS;

    std::string canonicalForm();
};

} // namespace cs453

#endif // CANONICAL_PATH_MACOS_CI_HPP
