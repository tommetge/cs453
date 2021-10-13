#ifndef CANONICAL_PATH_POSIX_CI_HPP
#define CANONICAL_PATH_POSIX_CI_HPP

#include "canonical_path.hpp"

namespace cs453
{

// Case-insensitive canonicalization of a POSIX path
class CanonicalPathPOSIXCI : public CanonicalPath
{
public:
    using CanonicalPath::CanonicalPath;

    std::string canonicalForm();
};

} // namespace cs453

#endif // CANONICAL_PATH_POSIX_CI_HPP
