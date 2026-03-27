#ifndef UTIL
#define UTIL
#include <string>
#include <vector>
namespace Util {
std::vector<std::string> Explode(const std::string &text, char delimiter);

template <typename First, typename... Tvals>
bool IsIn(First &&value, Tvals &&...t) {
  return ((std::forward<First>(value) == std::forward<Tvals>(t)) || ...);
}
} // namespace Util
#endif
