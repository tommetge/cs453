#include "include/catch.hpp"
#include "canonical_path_macos.hpp"
#include "canonical_path_macos_ci.hpp"

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

using namespace std;
using namespace cs453;

bool compareMacOS(string path1, string path2) {
    CanonicalPathMacOS c1(path1);
    CanonicalPathMacOS c2(path2);
    return c1.compare(c2);
}

bool compareMacOSCI(string path1, string path2) {
    CanonicalPathMacOSCI c1(path1);
    CanonicalPathMacOSCI c2(path2);
    return c1.compare(c2);
}

TEST_CASE("Resource forks are resolved correctly", "[HFS]")
{
    SECTION("NON-Homographs") {
        REQUIRE_FALSE(compareMacOS("file1", "file2:../file1"));
        REQUIRE_FALSE(compareMacOS("file1", "./file2:../file1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/some/path/to/:file1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/some/path/:to/file1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/some/:path/to/file1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/:some/path/to/file1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/some/path/:to/:file1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/some/:path/:to/:file1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/:some/:path/:to/:file1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/some/path/to/file2:../file1"));
    }

    SECTION("Homographs") {
        REQUIRE(compareMacOS("/some/path/to/file1", "/some/path/to/file1:forkname"));
        REQUIRE(compareMacOS("/some/path/to/file1", "/some/path/to/file1:../file2"));
        REQUIRE(compareMacOS("/some/path/to/file1", "/some/path/to/file1:./file2"));
        REQUIRE(compareMacOS("file1", "file1:forkname"));
        REQUIRE(compareMacOS("file1", "file1:../file2"));
        REQUIRE(compareMacOS("file1", "./file1:../file2"));
    }
}

TEST_CASE("Resource forks are resolved correctly (CI)", "[HFS-CI]")
{
    SECTION("NON-Homographs") {
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/SOME/PATH/TO/:FILE1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/SOME/PATH/:TO/FILE1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/SOME/:PATH/TO/FILE1"));
        REQUIRE_FALSE(compareMacOS("/some/path/to/file1", "/:SOME/PATH/TO/FILE1"));
    }

    SECTION("Homographs") {
        REQUIRE(compareMacOSCI("/some/path/to/file1", "/SOME/PATH/TO/FILE1:forkname"));
        REQUIRE(compareMacOSCI("/some/path/to/file1", "/SOME/PATH/TO/FILE1:FORKMAME"));
        REQUIRE(compareMacOSCI("/some/path/to/file1", "/Some/path/to/file1:../file2"));
        REQUIRE(compareMacOSCI("/some/path/to/file1", "/some/Path/to/file1:./file2"));
        REQUIRE(compareMacOSCI("file1", "FILE1:FORKNAME"));
        REQUIRE(compareMacOSCI("file1", "FILE1:../FILE2"));
        REQUIRE(compareMacOSCI("file1", "./FILE1:../FILE2"));
    }
}
