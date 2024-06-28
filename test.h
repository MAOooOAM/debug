#define DEBUG_STREAM std::cout
#include "debug.h"


class TestBase {
 public:
  virtual ~TestBase() {}
  void test() {
    DEBUG_STREAM << test_header() << std::endl;
    test_impl();
    DEBUG_STREAM << std::endl;
  }

 private:
  virtual std::string_view test_header() const = 0;
  virtual void test_impl() const = 0;
};

#define DEFINE_TEST(NAME)                                         \
  class NAME : public TestBase {                                  \
   public:                                                        \
    virtual ~NAME() override {}                                   \
    virtual std::string_view test_header() const override {       \
      return "--------------------" #NAME "--------------------"; \
    }                                                             \
    virtual void test_impl() const override;                      \
  };                                                              \
  void NAME::test_impl() const
