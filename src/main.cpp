#include <iostream>
#include <filesystem>
#include <chrono>

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

auto exploration_start =
    std::chrono::steady_clock::now();

  StrategyExplorer explorer(qBFName, formula);

  explorer.explore();

auto exploration_end =
    std::chrono::steady_clock::now();

double exploration_time =
    std::chrono::duration<double>(
        exploration_end - exploration_start).count();

  std::cout
    << "Exploration finished."
    << std::endl;

std::cout
    << "Exploration time: "
    << exploration_time
    << " s"
    << std::endl;

auto counting_start =
    std::chrono::steady_clock::now();

  CountEvaluator evaluator(
        explorer.state(),
        *formula
      );

  SolverResult result =
    evaluator.evaluate();

auto counting_end =
    std::chrono::steady_clock::now();

double counting_time =
    std::chrono::duration<double>(
        counting_end - counting_start).count();

  std::cout
    << "Count result: "
    << result
    << std::endl;

std::cout
    << "Counting time: "
    << counting_time
    << " s"
    << std::endl;
  return 0;
}
