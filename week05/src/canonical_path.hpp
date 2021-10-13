#ifndef CANONICAL_PATH_HPP
#define CANONICAL_PATH_HPP

#include <string>

namespace cs453
{

/* The following offers a non-normalized default implementation.
 * It is expected that sub-classes will override the following:
 *   - canonicalForm()
 */

class CanonicalPath
{
protected:
    std::string path;

public:
    explicit CanonicalPath(std::string &p) : path(p)
    {}

    // Corresponds to C().
    // Returns the canonical form of the path. Must be overridden
    // by sub-classes. The default implementation returns the raw
    // path.
    virtual std::string canonicalForm()
    {
        return path;
    }

    // Corresponds to H().
    // Indicates if the two paths are the same in their canonical
    // form - if they are homographs.
    bool compare(CanonicalPath &other)
    {
        std::string left = canonicalForm();
        std::string right = other.canonicalForm();
        return left.compare(right) == 0;
    }

}; // class CanonicalPath

} // namespace cs453

#endif // CANONICAL_PATH_HPP
