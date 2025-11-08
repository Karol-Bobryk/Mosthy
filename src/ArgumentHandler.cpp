#include "ArgumentHandler.h"

ArgumentHandler::ArgumentHandler() {}

ArgumentHandler::ArgumentHandler(int argc, char *argv[]) {
  for (int i = 0; i < argc; ++i) {
    values.push_back(argv[i]);
  }
}

ArgumentHandler::~ArgumentHandler() {}
