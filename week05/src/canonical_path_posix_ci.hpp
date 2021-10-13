#ifndef CANONICAL_PATH_POSIX_CI_HPP
#define CANONICAL_PATH_POSIX_CI_HPP

#include "canonical_path_posix.hpp"

namespace cs453
{

// Case-insensitive canonicalization of a POSIX path
class CanonicalPathPOSIXCI : public CanonicalPathPOSIX
{
public:
    using CanonicalPathPOSIX::CanonicalPathPOSIX;

    std::string canonicalForm();
};

} // namespace cs453

#endif // CANONICAL_PATH_POSIX_CI_HPP
