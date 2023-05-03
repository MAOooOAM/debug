#include <bits/stdc++.h>

// https://en.cppreference.com/w/cpp/types/remove_cvref
#if __cplusplus <= 201703L
namespace std {
template <class T> struct remove_cvref {
  typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};
template <class T> using remove_cvref_t = typename remove_cvref<T>::type;
}  // namespace std
#endif  // __cplusplus

template <typename T>
struct is_random_access_iterator : std::is_same<typename std::iterator_traits<T>::iterator_category,
                                                std::random_access_iterator_tag> {};
template <typename T>
constexpr bool is_random_access_iterator_v = is_random_access_iterator<T>::value;
template <typename T> struct is_string_like : std::false_type {};
template <typename... Ts> struct is_string_like<std::basic_string<Ts...>> : std::true_type {};
template <std::size_t N> struct is_string_like<char[N]> : std::true_type {};
template <> struct is_string_like<char[]> : std::true_type {};
template <> struct is_string_like<char*> : std::true_type {};
template <> struct is_string_like<const char*> : std::true_type {};
template <typename T> constexpr bool is_string_like_v = is_string_like<T>::value;

template <typename Pair, typename = typename std::remove_cvref_t<Pair>::first_type>
std::ostream& operator<<(std::ostream& os, Pair&& p) {
  return os << '<' << std::forward<Pair>(p).first << ',' << std::forward<Pair>(p).second << '>';
}

template <typename Container, typename Iterator = decltype(std::begin(std::declval<Container>())),
          typename = std::enable_if_t<!is_string_like_v<std::remove_cvref_t<Container>>>,
          bool flag = is_random_access_iterator_v<Iterator>>
std::ostream& operator<<(std::ostream& os, Container&& a) {
  constexpr char L = flag ? '[' : '{';
  constexpr char R = flag ? ']' : '}';
  Iterator it = std::begin(a), ed = std::end(a);
  if (it == ed) return os << L << R;
  for (os << L << *it++; it != ed; it++) os << ',' << *it;
  return os << R;
}

template <typename T> std::ostream& operator<<(std::ostream& os, std::queue<T> a) {
  std::vector<T> b;
  for (; !a.empty(); a.pop()) b.push_back(a.front());
  return os << b;
}

template <typename T> std::ostream& operator<<(std::ostream& os, std::priority_queue<T> a) {
  std::vector<T> b;
  for (; !a.empty(); a.pop()) b.push_back(a.top());
  return os << b;
}

template <typename Tuple, std::size_t... Is>
void print_tuple_impl(std::ostream& os, Tuple&& t, std::index_sequence<Is...>) {
  ((os << (Is == 0 ? '<' : ',') << std::get<Is>(std::forward<Tuple>(t))), ...);
  os << '>';
}

template <typename... Ts> std::ostream& operator<<(std::ostream& os, std::tuple<Ts...>&& t) {
  print_tuple_impl(os, std::forward<std::tuple<Ts...>>(t), std::index_sequence_for<Ts...>{});
  return os;
}

template <typename T> void print_impl(std::ostream& os, T&& object) {
  if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>) {
    os << (object ? "True" : "False");
  } else if constexpr (is_string_like_v<std::remove_cvref_t<T>>) {
    os << '"' << object << '"';
  } else {
    os << object;
  }
}

template <typename... Ts> void debug_impl(Ts&&... args) {
  (..., print_impl(std::cerr << ' ', std::forward<Ts>(args)));
  std::cerr << std::endl;
}

#define debug(arg...)            \
  do {                           \
    std::cerr << "[" #arg "] :"; \
    debug_impl(arg);             \
  } while (false)
