#include <Helpers/ArgvGenerator.h>

ArgvGenerator::ArgvGenerator(
    std::initializer_list<std::string_view> arguments) {
  for (const auto &arg : arguments) {
    auto elem = argv.emplace_back(arg);
    argvChar.push_back(elem.data());
  }

  argvChar.push_back(nullptr);
}

char **ArgvGenerator::GetArgvs() { return argvChar.data(); }
size_t ArgvGenerator::GetArgc() { return argv.size() - 1; }
