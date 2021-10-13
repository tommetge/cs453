#include "include/catch.hpp"

/* A POSIX compliant filesystem must:
 *   - Allow hierarchical paths (files are members of parent folders)
 *   - Allow hierarchical navigation via '.', '..', '...'
 *   - Support UNIX permissions
 * ... and more that is less relevant here. In this test suite, we are
 * primarily concerned with hierarchical navigation using the standard
 * POSIX path separator ('/') and short-hand for current working dir
 * ('.') and parent dir ('..').
 *
 * We include both case-sensitive and case-insensitive versions as there
 * are POSIX compliant filesystems available in both categories.
 */