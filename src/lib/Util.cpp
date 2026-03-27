#include "Util.h"
#include <iterator>
#include <string>
#include <vector>

std::vector<std::string> Util::Explode(const std::string &text,
                                       char delimiter) {
  using Words = std::vector<std::string>;

  Words res;
  std::string::const_iterator itStart = text.begin();
  std::string::const_iterator itEnd = text.begin();

  while (true) {
    if (itEnd == text.end() || *itEnd == delimiter) {
      if (itEnd != itStart)
        res.emplace_back(itStart, itEnd);

      if (itEnd == text.end())
        break;

      itEnd = std::next(itEnd);
      itStart = itEnd;

      continue;
    }

    itEnd = std::next(itEnd);
  }

  return res;
}
