#include "include/catch.hpp"

/* Two major filesystems to consider:
 *   1. HFS (legacy)
 *   2. APFS (current)
 * Both are POSIX compliant filesystems. This test suite
 * focuses on what makes these two filesystems unique and
 * defers POSIX-style testing to the POSIX tests found in
 * test-posix.cpp.
 *
 * What makes HFS / APFS unique? The native path separator
 * is ':' (not '/'). It is the only illegal character in
 * file and directory names. For HFS, the ':' character
 * is treated as short-hand for defining a fork or
 * extended attribute. For testing canonicalization, this
 * matters because extended attributes / resource forks
 * can modify a file at a path that is different than what
 * was provided. The following examples resolve to the
 * same file:
 *
 *   /Users/tom/restricted_file.txt
 *   /Users/tom/restricted_file.txt:forkname
 *   /Users/tom/restricted_file.txt:../other.txt
 *   /Users/tom/restricted_file.txt:/../../../blah.md
 *
 * This applies only to HFS. APFS does not use the ':' to
 * identify extended attributes (it has a physically
 * separate API for this). While there are some quirks we
 * could exploint on APFS, they are specific to the APFS
 * APIs, not the common POSIX path APIs, including kernel
 * calls, that nearly all applications use. Because of
 * this, we only test HFS here and consider APFS to be
 * covered by the POSIX tests.
 */

TEST_CASE( "Example test case", "[example]" )
{
    REQUIRE( true == true );
}

TEST_CASE("resource forks are resolved correctly", "[HFS]")
{

}