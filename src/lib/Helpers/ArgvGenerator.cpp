#include <Helpers/ArgvGenerator.h>
#include <cstdlib>
#include <cstring>

ArgvGenerator::ArgvGenerator(
    std::initializer_list<std::string_view> arguments) {

  argvChar.reserve(arguments.size() + 1);

  for (const auto &elem : arguments)
    argvChar.push_back(strdup(elem.data()));

  argvChar.push_back(nullptr);
}

ArgvGenerator::~ArgvGenerator() {
  // Yes, this is ugly although it is needed to simulate the real argv
  for (auto *e : argvChar)
    if (e != nullptr)
      // NOLINTBEGIN
      free(e);
  // NOLINTEND
}

char **ArgvGenerator::GetArgvs() { return argvChar.data(); }

size_t ArgvGenerator::GetArgc() { return argvChar.size() - 1; }
