/* Week 05 Lab: Team 4's Solution */

/*
 * The following is an aggregated version of our solution to
 * week 5's lab. We originally structured this across multiple
 * files - they are all included here. To build and run, use a
 * modern C++ compiler (g++ or clang++):
 *
 * clang++ -std=c++11 combined.cpp
 *
 * Note: the tests are disabled. To run, use the Github project
 * available here:
 *
 * https://github.com/tommetge/cs453
 *
 */

#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


/* ========== CanonicalPath ========== */

/* This is the base class for all implementations */

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



/* ========== CanonicalPathPOSIX ========== */

/* The POSIX-specific implementation (targeting ext4) */

// Case-sensitive canonicalization of a POSIX path
class CanonicalPathPOSIX : public CanonicalPath
{
public:
    using CanonicalPath::CanonicalPath;

    std::string canonicalForm();
    std::string canonicalFormAlt();
};


const char PATH_DELIMITER = '/';

// POSIX-specific canonical form of a path (case-sensitive)
string CanonicalPathPOSIX::canonicalForm()
{
    // Root is already normalized
    if (path == "/" || path.size() == 0) {
        return path;
    }

    // Tokenize the path by the PATH_DELIMITER (/).
    // This is an easy way for us to start working with the path
    // in a hierarchical fashion.
    vector<string> pathComponents;
    stringstream pathStream(path);
    string component;

    while(getline(pathStream, component, PATH_DELIMITER)) {
        // If we see '..', remove the most recent path component
        // from our list and move onto the next component.
        if (component == "..") {
            if (pathComponents.size() > 0) {
                pathComponents.pop_back();
            }
            continue;
        }

        // Skip empty path components and '.'
        if (component.length() > 0 && component != ".") {
            pathComponents.push_back(component);
        }
    }

    // If there are no path components after the above, it's
    // either a root ('/') or relative root ('').
    if (pathComponents.size() == 0) {
        if (path[0] == PATH_DELIMITER) {
            return "/";
        }

        return "";
    }

    // Join our path components with PATH_DELIMITER ('/') to create
    // our canonical path.
    string canonicalPath = accumulate(
        next(pathComponents.begin()),
             pathComponents.end(),
             pathComponents[0],
             [](string a, string b)
        {
            return a + PATH_DELIMITER + b;
        }
    );

    // We inadvertently removed the leading '/', if one was present.
    // Add it back if needed.
    if (path[0] == PATH_DELIMITER && pathComponents[0] != "/") {
        ostringstream os;
        os << "/" << canonicalPath;
        canonicalPath = os.str();
    }

    return canonicalPath;
}

// An alternative approach we also identified for canonicalization
std::string CanonicalPathPOSIX::canonicalFormAlt()
{
    vector<string> vctr('/');
    string part = "";

    for (int i = 0; i < path.length(); i++) {
        part += path[i];
        if (part == "../" || (part == ".." && i == path.length() - 1))
        {
            if (vctr.back() != "/")
                vctr.pop_back();
            part = "";
        } 
        else if (part == "./")
        {
            part = "";
        } 
        else if (path[i] == '/')
        {
            if (path[i-1] == '/')
            {
                part.erase(part.length() - 1);
            } else {
                vctr.push_back(part);
                part = "";
            }
        }
    }
    if (part != ".")
        vctr.push_back(part);
  
    string newPath = "";
    for (vector<string>::iterator it = vctr.begin(); it < vctr.end(); it++) 
    {
        newPath += *it;
    }

    return newPath;
}


/* ========== CanonicalPathPOSIXCI ========== */

/* The case-sensitive version of CanonicalPathPOSIX */

// Case-insensitive canonicalization of a POSIX path
class CanonicalPathPOSIXCI : public CanonicalPathPOSIX
{
public:
    using CanonicalPathPOSIX::CanonicalPathPOSIX;

    std::string canonicalForm();
};


std::string toLowerStr(std::string str)
{
    std::string lowered;
    lowered.resize(str.size());
    std::transform(str.begin(), str.end(), lowered.begin(), ::tolower);
    return lowered;
}

// POSIX-specific canonical form of a path (case-insensitive)
std::string CanonicalPathPOSIXCI::canonicalForm()
{
    // Lowercase the path and compare with the case-sensitive version
    path = toLowerStr(path);
    return CanonicalPathPOSIX::canonicalForm();
}

/* ========== CanonicalPathMacOS ========== */

/* The macOS-specific implementation (targeting HFS) */

// MacOS-specific canonicalization of a path. Note that this is a
// sub-class of CanonicalPathPOSIX: it will build on the POSIX
// canonicalization.
class CanonicalPathMacOS : public CanonicalPathPOSIX
{
public:
    using CanonicalPathPOSIX::CanonicalPathPOSIX;

    std::string canonicalForm();
};


const char FORK_DELIMITER = ':';

// macOS-specific (HFS) canonical form of a path (case-sensitive)
std::string CanonicalPathMacOS::canonicalForm()
{
    // Root is already normalized
    if (path == "/" || path.size() == 0) {
        return path;
    }

    // Tokenize the path by the FORK_DELIMITER (:). On HFS for
    // macOS, resource forks are accessed via POSIX APIS using
    // FORK_DELIMITER, making it possible for homographic paths
    // that differ by anything after the FORK_DELIMITER.

    vector<string> pathComponents;
    stringstream pathStream(path);
    string standardPath;

    if (getline(pathStream, standardPath, FORK_DELIMITER)) {
        path = standardPath;
        return CanonicalPathPOSIX::canonicalForm();
    }

    return CanonicalPathPOSIX::canonicalForm();
}


/* ========== CanonicalPathMacOSCI ========== */

/* The case-sensistive version of CanonicalPathMacOS */

// MacOS-specific canonicalization of a path. This is the
// case-insensitive version.
class CanonicalPathMacOSCI : public CanonicalPathMacOS
{
public:
    using CanonicalPathMacOS::CanonicalPathMacOS;

    std::string canonicalForm();
};


// POSIX-specific canonical form of a path (case-insensitive)
std::string CanonicalPathMacOSCI::canonicalForm()
{
    std::string preprocessed = CanonicalPathMacOS::canonicalForm();
    // Use CanonicalPathPOSIXCI to further refine.
    CanonicalPathPOSIXCI posixCI(preprocessed);

    return posixCI.canonicalForm();
}


int main()
{
    string filename1, filename2;

    cout << "Specify the first filename: ";
    getline(cin, filename1);
    cout << "Specify the second filename: ";
    getline(cin, filename2);

    // Update with the specific sub-class desired
    CanonicalPathPOSIX path1(filename1);
    CanonicalPathPOSIX path2(filename2);

    if (path1.compare(path2)) {
        cout << "The paths are homographs" << endl;
        return 0;
    }

    cout << "The paths are NOT homographs" << endl;
    return 1;
}


/* ========== TEST CASES ========== */

/* 
 * Note: the following require the catch2 test library to run
 * and are disabled by default. To enable and run, Please use
 * the github project linked at the top. Note: you must have
 * the catch2 unit test header (a single file) available to
 * compile. It can be downloaded here:
 *
 * https://raw.githubusercontent.com/catchorg/Catch2/v2.x/single_include/catch2/catch.hpp
 */


/*

#include "../test/include/catch.hpp"
#define CATCH_CONFIG_MAIN

*/

/* ========== POSIX Tests ========== */

/* A POSIX compliant filesystem must:
 *   - Allow hierarchical paths (files are members of parent folders)
 *   - Allow hierarchical navigation via '.', '..'
 *   - Support UNIX permissions
 * ... and more that is less relevant here. In this test suite, we are
 * primarily concerned with hierarchical navigation using the standard
 * POSIX path separator ('/') and short-hand for current working dir
 * ('.') and parent dir ('..').
 *
 * We include both case-sensitive and case-insensitive versions as there
 * are POSIX compliant filesystems available in both categories.
 */

/*

bool compare(string path1, string path2) {
    CanonicalPathPOSIX c1(path1);
    CanonicalPathPOSIX c2(path2);
    return c1.compare(c2);
}

bool compareCI(string path1, string path2) {
    CanonicalPathPOSIXCI c1(path1);
    CanonicalPathPOSIXCI c2(path2);
    return c1.compare(c2);
}

TEST_CASE("Hierarchical navigation is resolved correctly", "[POSIX]")
{
    SECTION("NON-Homographs") {
        REQUIRE_FALSE(compare("/some/path/to/file1", "/some/path/to/file2"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "/some/path/to/file2/../file2"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "/some/path/to/file1/../file2"));
        REQUIRE_FALSE(compare("/some/path/to/file1.txt", "/some/path/to/file1.txt/../file2"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "/some/path/to/./file2"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "/some/path/to/../to/file2"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "/../../../path/to/file2/../file2"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "/some/../some/path/to/file2/../file2"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "file1"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "to/file1"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "path/to/file1"));
        REQUIRE_FALSE(compare("/some/path/to/file1.txt", "path/to/file1.txt"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "some/path/to/file1"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "./some/path/to/file1"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "/some/path/to/../file1"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "/some/path/to/file1/file1"));
        REQUIRE_FALSE(compare("/some/path/to/file1", "/some/path/to/file1/../../.."));
    }

    SECTION("Homographs") {
        REQUIRE(compare("/some/path/to/file1", "/some/path/to/file1"));
        REQUIRE(compare("/some/path/to/file1.txt", "/some/path/to/file1.txt"));
        REQUIRE(compare("/some/path/to/file1", "/some/path/to/file1/../file1"));
        REQUIRE(compare("/some/path/to/file1.txt", "/some/path/to/file1/../file1.txt"));
        REQUIRE(compare("/some/path/to/file1", "/some/path/to/file2/../file1"));
        REQUIRE(compare("/some/path/to/file1", "/some/path/to/./file1"));
        REQUIRE(compare("/some/path/to/file1.txt", "/some/path/to/./file1.txt"));
        REQUIRE(compare("/some/path/to/file1", "/some/path/to/../to/file1"));
        REQUIRE(compare("some/path/to/file1", "../../../some/path/to/file1"));
        REQUIRE(compare("/some/path/to/file1", "/../../../some/path/to/file1"));
        REQUIRE(compare("/some/path/to/file1", "/../../../some/path/to/file1/../file1"));
        REQUIRE(compare("/some/path/to/file1", "/some/../some/path/to/file2/../file1"));
        REQUIRE(compare("/some/path/to/file1", "//////some//////path/////to/////////file1"));
        REQUIRE(compare("/", "/some/path/to/file1/../../../../../../../.."));
        REQUIRE(compare("/", "/some/path/to/file1/../../../.."));
        REQUIRE(compare("/", "/////////////////"));
        REQUIRE(compare("", "."));
        REQUIRE(compare("", ".."));
        REQUIRE(compare("", "../../.."));
    }
}

TEST_CASE("Hierarchical navigation is resolved correctly (CI)", "[POSIX-CI]")
{
    SECTION("NON-Homographs") {
        REQUIRE_FALSE(compareCI("/some/path/to/file1", "/SOME/../PATH/TO/FILE1"));
    }

    SECTION("Homographs") {
        REQUIRE(compareCI("/some/path/to/file1", "/SOME/PATH/TO/FILE1"));
        REQUIRE(compareCI("/some/path/to/file1", "/SOME/path/TO/FILE1"));
        REQUIRE(compareCI("/some/path/to/file1", "/SoMe/PaTh/To/FiLe1"));
        REQUIRE(compareCI("/some/path/to/file1", "/SOME/./PATH/TO/FILE1"));
        REQUIRE(compareCI("file1", "FILE1"));
        REQUIRE(compareCI("file1", "./FILE1"));
        REQUIRE(compareCI("./file1", "./FILE1"));
    }
}

*/

/* ========== macOS Tests ========== */

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

/*

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

*/
