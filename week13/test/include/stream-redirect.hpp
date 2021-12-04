#include <iostream>
#include <string>
#include <sstream>

/* ===== TEST HELPERS ===== */

/* 
 * The following will temporarily redirect cout/cin
 * to the specified streambufs, allowing some degree
 * of unit testing for programs using cout/cin.
 */

struct cout_redirect {
  cout_redirect(std::streambuf *new_buffer)
    : old(std::cout.rdbuf(new_buffer))
  {}

  ~cout_redirect() {
    std::cout.rdbuf(old);
  }

private:
  std::streambuf *old;
};

struct cin_redirect {
  cin_redirect(std::streambuf *new_buffer)
    : old(std::cin.rdbuf(new_buffer))
  {}

  ~cin_redirect() {
    std::cin.rdbuf(old);
  }

private:
  std::streambuf *old;
};

/* Some unit test helpers */

// Checks equality. If a != b, it will print a failure
// and the input to the screen.
#define ASSERT_EQUAL(a, b) \
  if (a != b) { \
    std::ostringstream os; \
    os << "FAILED: " << a << " != " << b << std::endl; \
    printf("%s", os.str().c_str()); \
    return;\
  }

// Checks truthiness. If a != b, it will print a failure
// and the input to the screen.
#define ASSERT_TRUE(a) \
  if (!a) { \
    std::ostringstream os; \
    os << "FAILED: " << a << " is NOT true" << std::endl; \
    printf("%s", os.str().c_str()); \
    return;\
  }

/* ===== UNIT TESTS ===== */
