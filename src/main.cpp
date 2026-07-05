#include <iostream>
#include <filesystem>

#include "explorer/StrategyExplorer.hpp"

#include "evaluator/CountEvaluator.hpp"

#include "util/QDIMACSParser.hpp"
#include "util/QBFFormatter.hpp"

int
main(
    int argc,
    char* argv[]
    ) {

  if (argc <= 1) {

    std::cerr
      << "Please provide the file containing QBF definition"
      << std::endl;

    return 1;
  }

  QDIMACSParser parser;
  std::string iFileName(argv[1]);
  std::filesystem::path iFilePath(iFileName);
  iFilePath.replace_extension("");

  auto qBFName = iFilePath.string();

  auto formula =
    parser.parse(
        iFileName
        );

  std::cout
    << QBFFormatter::format(
        *formula
        )
    << std::endl;

  StrategyExplorer explorer(qBFName, formula);

  explorer.explore();

  std::cout
    << "Exploration finished."
    << std::endl;

  CountEvaluator evaluator(
        explorer.state(),
        *formula
      );

  SolverResult result =
    evaluator.evaluate();

  std::cout
    << "Count result: "
    << result
    << std::endl;
  return 0;
}
