#include <bits/stdc++.h>

#include "debug.h"
#include "test.h"


template <typename T> void SingleTest(T x) {
  T& y = x;
  const T& z = x;
  auto w = [x]() -> T { return x; };
  debug(x, y, z);
  debug(typeid(T).name(), w());
}


DECLARE_TEST(TestBasicType) {
  SingleTest(1);
  SingleTest('x');
  SingleTest(static_cast<signed char>('x'));
  SingleTest(false);
  SingleTest(1ll << 60);
  SingleTest(2.0);
  SingleTest(static_cast<long double>(3.1415926));
  SingleTest(reinterpret_cast<void*>(0x1bf52));
}

DECLARE_TEST(TestSTLInt) {
  std::valarray a{1, 2, 3, 1, 3};
  std::vector<std::pair<int, int>> b{{1, 2}, {3, 4}, {5, 6}, {1, 4}};
  SingleTest(a);
  SingleTest(std::array{1, 2, 3, 1, 3});
  SingleTest(std::vector(std::begin(a), std::end(a)));
  SingleTest(std::deque(std::begin(a), std::end(a)));
  SingleTest(std::priority_queue(std::begin(a), std::end(a)));
  SingleTest(std::set(std::begin(a), std::end(a)));
  SingleTest(std::unordered_set(std::begin(a), std::end(a)));
  SingleTest(std::list(std::begin(a), std::end(a)));
  SingleTest(std::map(std::begin(b), std::end(b)));
  SingleTest(std::unordered_map(std::begin(b), std::end(b)));
  SingleTest(std::multiset(std::begin(a), std::end(a)));
  SingleTest(std::multimap(std::begin(b), std::end(b)));
}

DECLARE_TEST(TestTuple) {
  std::tuple t = std::make_tuple("true", true, 'T', 1);
  std::tuple f = std::make_tuple("false", false, 'F', 0);
  SingleTest(std::make_tuple(std::make_pair(t, f), std::pair(f, t), t, f));
}

DECLARE_TEST(TestComplex) {
  std::map<int, int> fib = {{0, 1}, {1, 1}, {2, 2}, {3, 3}, {4, 5}, {5, 8}};
  std::map<int, int> two = {{0, 1}, {1, 2}, {2, 4}};
  SingleTest(std::vector{std::map{
      std::pair{1, std::tuple{std::vector{fib, two}, std::set{fib, two},
                              std::string_view{"a long string"}, std::vector{&fib, &two}}}}});
}

DECLARE_TEST(TestSTLClass) {
  std::vector<std::complex<float>> a{{1, 2}, {3, 4}, {5, 6}};
  std::valarray b = std::valarray{a[0], a[1], a[2]}.apply(std::conj);
  SingleTest(a);
  SingleTest(b);
  SingleTest(std::vector<bool>{0, 1, 1, 0, 1});
  SingleTest(std::bitset<5>{"01101"});
}

struct Point2D {
  double x = 0, y = 0;
  Point2D(double x_, double y_) : x(x_), y(y_) {}
};
struct Point3D {
  double x = 0, y = 0, z = 0;
  Point3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};
std::ostream& operator<<(std::ostream& os, Point2D p) {
  return os << '(' << p.x << ',' << p.y << ')';
}
std::ostream& operator<<(std::ostream& os, Point3D p) {
  return os << '(' << p.x << ',' << p.y << ',' << p.z << ')';
}
DECLARE_TEST(TestNativeClass) {
  SingleTest(Point2D{3, 4});
  SingleTest(Point3D{3, 4, 5});
}

int main() {
  set_debug_stream(std::cout);
  TestBasicType().test();
  TestSTLInt().test();
  TestTuple().test();
  TestComplex().test();
  TestSTLClass().test();
  TestNativeClass().test();
  return 0;
}
