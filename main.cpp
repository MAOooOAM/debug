#include <bits/stdc++.h>

#include "debug.h"
#include "test.h"


template <typename T> void SingleTest(T x) {
  T& y = x;
  const T& z = x;
  auto lambda = [x]() -> T { return x; };
  debug(x, y, z, lambda());
}


DECLARE_TEST(TestBasicType) {
  SingleTest(1);
  SingleTest('x');
  SingleTest(static_cast<signed char>('x'));
  SingleTest(false);
  SingleTest(1ll << 60);
  SingleTest(2.0);
}


int main() {
  set_debug_stream(std::cout);
  TestBasicType().test();
  return 0;
}
