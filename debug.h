#ifndef debug


#include <array>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


#ifndef DEBUG_STREAM
#define DEBUG_STREAM std::cerr
#endif  // DEBUG_STREAM


namespace mao {

namespace traits {

template <typename T, typename... Ts>
struct is_one_of : std::disjunction<std::is_same<T, Ts>...> {};
template <typename T, typename... Ts>
inline constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;

template <typename T>
struct is_string_like : is_one_of<T, std::string, std::string_view, const char*, char*, char[]> {};
template <typename T> inline constexpr bool is_string_like_v = is_string_like<T>::value;

template <typename T, template <typename...> class Template> struct is_spec_of : std::false_type {};
template <template <typename...> class Template, typename... Ts>
struct is_spec_of<Template<Ts...>, Template> : std::true_type {};
template <typename T, template <typename...> class Template>
inline constexpr bool is_spec_of_v = is_spec_of<T, Template>::value;

template <typename T, template <typename...> class... Templates>
struct is_spec_one_of : std::disjunction<is_spec_of<T, Templates>...> {};
template <typename T, template <typename...> class... Templates>
inline constexpr bool is_spec_one_of_v = is_spec_one_of<T, Templates...>::value;

template <typename T> struct is_vector_like : is_spec_one_of<T, std::vector, std::deque> {};
template <typename T> inline constexpr bool is_vector_like_v = is_vector_like<T>::value;

template <typename T>
struct is_line_like : is_spec_one_of<T, std::vector, std::deque, std::list> {};
template <typename T> inline constexpr bool is_line_like_v = is_line_like<T>::value;

template <typename T>
struct is_stl_container_adapter : is_spec_one_of<T, std::queue, std::priority_queue> {};
template <typename T>
inline constexpr bool is_stl_container_adapter_v = is_stl_container_adapter<T>::value;

template <typename T>
struct is_set_like
    : is_spec_one_of<T, std::set, std::multiset, std::unordered_set, std::unordered_multiset> {};
template <typename T> inline constexpr bool is_set_like_v = is_set_like<T>::value;

template <typename T>
struct is_map_like
    : is_spec_one_of<T, std::map, std::multimap, std::unordered_map, std::unordered_multimap> {};
template <typename T> inline constexpr bool is_map_like_v = is_map_like<T>::value;

}  // namespace traits


template <typename T, typename = void> struct PrinterWrapper {
  static void print(const T& x) { DEBUG_STREAM << x; }
};

template <typename T> void print_impl(const T& x) { PrinterWrapper<T>::print(x); }

void print_impl(const bool& x) { DEBUG_STREAM << (x ? "True" : "False"); }

void print_impl(const char& x) {
  if (x == '\0') {
    DEBUG_STREAM << "\'\\0\'";
  } else if (x == '\t') {
    DEBUG_STREAM << "\'\\t\'";
  } else if (x == '\n') {
    DEBUG_STREAM << "\'\\n\'";
  } else if (x == '\r') {
    DEBUG_STREAM << "\'\\r\'";
  } else if (x == '\'') {
    DEBUG_STREAM << "\'\\\'\'";
  } else if (x == '\"') {
    DEBUG_STREAM << "\'\\\"\'";
  } else if (x == '\\') {
    DEBUG_STREAM << "\'\\\\\'";
  } else {
    DEBUG_STREAM << '\'' << x << '\'';
  }
}

void print_impl(const signed char& x) { print_impl(static_cast<short>(x)); }

void print_impl(const unsigned char& x) { print_impl(static_cast<unsigned short>(x)); }

void print_impl(const std::vector<bool>& x) {
  for (int a : x) {
    DEBUG_STREAM << a;
  }
}

template <typename T, size_t N> void print_impl(const std::array<T, N>& x) {
  static_assert(N > 0);
  DEBUG_STREAM << '[';
  print_impl(x[0]);
  for (size_t i = 1; i < N; ++i) {
    DEBUG_STREAM << ',';
    print_impl(x[1]);
  }
  DEBUG_STREAM << ']';
}

template <typename T> struct PrinterWrapper<T, std::enable_if_t<traits::is_string_like_v<T>>> {
  static void print(const T& x) { DEBUG_STREAM << '\"' << x << '\"'; }
};

template <typename T> struct PrinterWrapper<T, std::enable_if_t<traits::is_line_like_v<T>>> {
  static void print(const T& x) {
    DEBUG_STREAM << '[';
    if (typename T::const_iterator it = x.cbegin(); it != x.cend()) {
      print_impl(*it);
      while (++it != x.cend()) {
        DEBUG_STREAM << ',';
        print_impl(*it);
      }
    }
    DEBUG_STREAM << ']';
  }
};

template <typename T>
struct PrinterWrapper<T, std::enable_if_t<traits::is_stl_container_adapter_v<T>>> {
  static void print(const T& x) {
    struct Accessor : T {
      using T::c;
    };
    print_impl(x.*&Accessor::c);
  }
};

template <typename T> struct PrinterWrapper<T, std::enable_if_t<traits::is_set_like_v<T>>> {
  static void print(const T& x) {
    DEBUG_STREAM << '{';
    if (typename T::const_iterator it = x.cbegin(); it != x.cend()) {
      print_impl(*it);
      while (++it != x.cend()) {
        DEBUG_STREAM << ',';
        print_impl(*it);
      }
    }
    DEBUG_STREAM << '}';
  }
};

template <typename T> struct PrinterWrapper<T, std::enable_if_t<traits::is_map_like_v<T>>> {
  static void print(const T& x) {
    DEBUG_STREAM << '{';
    if (typename T::const_iterator it = x.cbegin(); it != x.cend()) {
      print_impl(it->first);
      DEBUG_STREAM << ':';
      print_impl(it->second);
      while (++it != x.cend()) {
        DEBUG_STREAM << ',';
        print_impl(it->first);
        DEBUG_STREAM << ':';
        print_impl(it->second);
      }
    }
    DEBUG_STREAM << '}';
  }
};

template <typename T1, typename T2> struct PrinterWrapper<std::pair<T1, T2>, void> {
  static void print(const std::pair<T1, T2>& x) {
    DEBUG_STREAM << '<';
    print_impl(x.first);
    DEBUG_STREAM << ',';
    print_impl(x.second);
    DEBUG_STREAM << '>';
  }
};

template <typename T1, typename... Ts> struct PrinterWrapper<std::tuple<T1, Ts...>, void> {
  static void print(const std::tuple<T1, Ts...>& x) {
    DEBUG_STREAM << '<';
    print_impl(std::get<0>(x));
    print_tail(x, std::make_index_sequence<sizeof...(Ts)>{});
    DEBUG_STREAM << '>';
  }
  template <size_t... Is>
  static void print_tail(const std::tuple<T1, Ts...>& x, std::index_sequence<Is...>) {
    (print_tail_impl(std::get<Is + 1>(x)), ...);
  }
  template <typename U> static void print_tail_impl(const U& x) {
    DEBUG_STREAM << ',';
    print_impl(x);
  }
};


template <typename T> void debug_single(const T& x) {
  DEBUG_STREAM << ' ';
  print_impl(x);
}

inline void debug_impl() { DEBUG_STREAM << std::endl; }

template <typename T, typename... Ts> void debug_impl(const T& head, const Ts&... tail) {
  debug_single(head);
  debug_impl(tail...);
}

}  // namespace mao


#define debug(...)                          \
  do {                                      \
    DEBUG_STREAM << "[" #__VA_ARGS__ "] :"; \
    mao::debug_impl(__VA_ARGS__);           \
  } while (false)


#endif  // debug
