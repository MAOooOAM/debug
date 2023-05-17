#include <bitset>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>


namespace debug {

std::ostream* debug_stream_ptr = &std::cerr;
void set_debug_stream(std::ostream& os) { debug_stream_ptr = &os; }

}  // namespace debug

using debug::set_debug_stream;


template <typename T, typename... Ts>
struct is_one_of : std::disjunction<std::is_same<T, Ts>...> {};
template <typename T, typename... Ts> constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;

template <typename T, template <typename...> class Template>
struct is_specialization_of : std::false_type {};
template <template <typename...> class Template, typename... Ts>
struct is_specialization_of<Template<Ts...>, Template> : std::true_type {};
template <typename T, template <typename...> class Template>
constexpr bool is_specialization_of_v = is_specialization_of<T, Template>::value;

#define DECLARE_IS_SPEC_1(NAME)                                              \
  template <typename T> struct is_##NAME : is_specialization_of<T, NAME> {}; \
  template <typename T> constexpr bool is_##NAME##_v = is_##NAME<T>::value;
#define DECLARE_IS_SPEC_2(FIELD, NAME)                                              \
  template <typename T> struct is_##NAME : is_specialization_of<T, FIELD::NAME> {}; \
  template <typename T> constexpr bool is_##NAME##_v = is_##NAME<T>::value;
#define GET3(ARG1, ARG2, ARG3, ...) ARG3
#define DECLARE_IS_SPEC_IMPL(...) \
  GET3(__VA_ARGS__, DECLARE_IS_SPEC_2, DECLARE_IS_SPEC_1, )(__VA_ARGS__)
#define DECLARE_IS_SPEC(...) DECLARE_IS_SPEC_IMPL(__VA_ARGS__)

DECLARE_IS_SPEC(std, pair)
DECLARE_IS_SPEC(std, tuple)
DECLARE_IS_SPEC(std, queue)
DECLARE_IS_SPEC(std, priority_queue)

template <typename T> using begin_type = decltype(std::begin(std::declval<T>()));
template <typename, typename = void> struct has_begin_helper : std::false_type {};
template <typename T> struct has_begin_helper<T, std::void_t<begin_type<T>>> : std::true_type {};
template <typename T> using has_begin = has_begin_helper<T>;
template <typename T> constexpr bool has_begin_v = has_begin<T>::value;

template <typename T>
struct is_rai : std::is_same<typename std::iterator_traits<T>::iterator_category,
                             std::random_access_iterator_tag> {};
template <typename T> constexpr bool is_rai_v = is_rai<T>::value;


constexpr std::string_view c_escape(const char& c) {
  if (c == '\0') {
    return "\\0";
  } else if (c == '\a') {
    return "\\a";
  } else if (c == '\b') {
    return "\\b";
  } else if (c == '\t') {
    return "\\t";
  } else if (c == '\n') {
    return "\\n";
  } else if (c == '\v') {
    return "\\v";
  } else if (c == '\f') {
    return "\\f";
  } else if (c == '\r') {
    return "\\r";
  } else if (c == '\e') {
    return "\\e";
  } else if (c == '\'') {
    return "\\\'";
  } else if (c == '\"') {
    return "\\\"";
  } else if (c == '\\') {
    return "\\\\;
  } else {
    return std::string_view(&c, 1);
  }
}


template <typename T> void print_impl(std::ostream&, T&& x);
template <typename Tuple, std::size_t... Is>
void print_tuple_impl(std::ostream& os, Tuple&& t, std::index_sequence<Is...>);

template <typename T> void print_impl(std::ostream& os, T&& x) {
  using U = std::decay_t<T>;
  if constexpr (is_one_of_v<U, bool>) {
    os << (x ? "True" : "False");
  } else if constexpr (is_one_of_v<U, signed char>) {
    os << static_cast<short>(x);
  } else if constexpr (is_one_of_v<U, unsigned char>) {
    os << static_cast<unsigned short>(x);
  } else if constexpr (is_one_of_v<U, char>) {
    os << '\'' << c_escape(x) << '\'';
  } else if constexpr (is_one_of_v<U, char*, const char*>) {
    os << '\"';
    for (const char* ptr = x; *ptr; ptr++) os << c_escape(*ptr);
    os << '\"';
  } else if constexpr (is_one_of_v<U, std::string, std::string_view>) {
    os << '\"';
    for (const char& c : x) os << c_escape(c);
    os << '\"';
  } else if constexpr (is_one_of_v<U, std::vector<bool>>) {
    for (auto&& b : x) os << b;
  } else if constexpr (is_pair_v<U>) {
    print_impl(os << '<', std::forward<T>(x).first);
    print_impl(os << ',', std::forward<T>(x).second);
    os << '>';
  } else if constexpr (is_tuple_v<U>) {
    print_tuple_impl(os, std::forward<T>(x), std::make_index_sequence<std::tuple_size_v<U>>{});
  } else if constexpr (has_begin_v<T>) {
    using Iterator = begin_type<T>;
    constexpr bool flag = is_rai_v<Iterator>;
    Iterator it = std::begin(x), ed = std::end(x);
    os << (flag ? '[' : '{');
    if (it != ed) {
      print_impl(os, *it++);
      while (it != ed) print_impl(os << ',', *it++);
    }
    os << (flag ? ']' : '}');
  } else if constexpr (is_queue_v<U>) {
    U a(x);
    std::vector<typename U::value_type> b;
    for (; !a.empty(); a.pop()) b.push_back(a.front());
    print_impl(os, b);
  } else if constexpr (is_priority_queue_v<U>) {
    U a(x);
    std::vector<typename U::value_type> b;
    for (; !a.empty(); a.pop()) b.push_back(a.top());
    print_impl(os, b);
  } else {
    os << x;
  }
}
template <typename Tuple, std::size_t... Is>
void print_tuple_impl(std::ostream& os, Tuple&& t, std::index_sequence<Is...>) {
  (print_impl(os << (Is ? ',' : '<'), std::get<Is>(std::forward<Tuple>(t))), ...);
  os << '>';
}


template <typename... Ts> void debug_impl(Ts&&... args) {
  (..., print_impl(*debug::debug_stream_ptr << ' ', std::forward<Ts>(args)));
  *debug::debug_stream_ptr << std::endl;
}

#define debug(...) *debug::debug_stream_ptr << "[" #__VA_ARGS__ "] :", debug_impl(__VA_ARGS__)
