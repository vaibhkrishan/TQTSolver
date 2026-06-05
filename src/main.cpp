#include <iostream>

#include "explorer/StrategyExplorer.hpp"

#include "util/QDIMACSParser.hpp"
#include "util/QBFFormatter.hpp"

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    std::cerr << "Please provide the file containing QBF definition" << std::endl;
    exit(1);
  }

  QDIMACSParser parser;
  auto formula =
    parser.parse(std::string(argv[1]));
  std::cout
    << QBFFormatter::format(*formula)
    << std::endl;
  ResidualQBF root;

  root.formula = formula;

  root.trail = nullptr;

  StrategyExplorer explorer;

  std::cout << "Result: " << explorer.recurse(root) << std::endl;
  return 0;
}
