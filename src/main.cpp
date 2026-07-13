#include <iostream>
#include <filesystem>
#include <chrono>
#include <iomanip>

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
        std::cerr << "Usage: " << argv[0]
                  << " <input.qdimacs> [--no_qsole]" << std::endl;
        return 1;
    }

    bool useQsole = true;
    std::string inputFile;

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);

        if (arg == "--no_qsole") {
            useQsole = false;
        } else {
            inputFile = arg;
        }
    }

    if (inputFile.empty()) {
        std::cerr << "No input file specified." << std::endl;
        return 1;
    }

    QDIMACSParser parser;
    std::filesystem::path iFilePath(inputFile);
    iFilePath.replace_extension("");

    auto qBFName = iFilePath.string();

    auto formula = parser.parse(inputFile);

  std::cout
    << QBFFormatter::format(
        *formula
        )
    << std::endl;
	
std::cout << std::fixed << std::setprecision(4);

auto exploration_start =
    std::chrono::steady_clock::now();

 StrategyExplorer explorer(qBFName, formula, useQsole);

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
