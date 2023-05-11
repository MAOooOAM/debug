#include <iostream>
#include <string_view>


namespace test {

std::ostream* test_stream_ptr = &std::cout;
void set_test_stream(std::ostream& os) { test_stream_ptr = &os; }

}  // namespace test

using test::set_test_stream;


class TestBase {
 public:
  void test() {
    *test::test_stream_ptr << test_header() << std::endl;
    test_impl();
    *test::test_stream_ptr << std::endl;
  }

 private:
  virtual std::string_view test_header() const = 0;
  virtual void test_impl() const = 0;
};

#define DECLARE_TEST(NAME)                                        \
  class NAME : public TestBase {                                  \
    virtual std::string_view test_header() const override {       \
      return "--------------------" #NAME "--------------------"; \
    }                                                             \
    virtual void test_impl() const override;                      \
  };                                                              \
  void NAME::test_impl() const
