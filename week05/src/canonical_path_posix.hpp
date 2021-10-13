#ifndef CANONICAL_PATH_POSIX_HPP
#define CANONICAL_PATH_POSIX_HPP

#include "canonical_path.hpp"

namespace cs453
{

// Case-sensitive canonicalization of a POSIX path
class CanonicalPathPOSIX : public CanonicalPath
{
public:
    using CanonicalPath::CanonicalPath;

    std::string canonicalForm();
};

} // namespace cs453

#endif // CANONICAL_PATH_POSIX_HPP
