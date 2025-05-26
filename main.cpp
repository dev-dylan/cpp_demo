
#ifndef MAIN_CPP
#define MAIN_CPP

#include "src/gzip.hpp"
#include "src/map.hpp"
#include <gtest/gtest.h>
#include <iostream>
#endif // HOMEBOT_CPP

#define ENABLE_CTEST false
#if ENABLE_CTEST
int main(int argc, char **argv) {
  map_p::testFuncInput();
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#else
int main() {
  // Expect two strings not to be equal.
  map_p::testFuncInput();
  return 0;
}
#endif
