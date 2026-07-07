#include "oracle/SemanticOracle.hpp"
#include "oracle/CNFTautChecker.hpp"
#include "oracle/DepQBFSolver.hpp"
#include "util/QDIMACSFormatter.hpp"

#include <algorithm>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <stdexcept>

#include <cmath>
#include <sys/stat.h>
#include <signal.h>
#include <chrono>
#include <thread>
#include <iostream>

int run_qsole(const char *f1, const char *f2, int num_vars)
{
  struct stat st1{}, st2{};
  if (stat(f1, &st1) != 0 || stat(f2, &st2) != 0)
    return -1;

  size_t max_size = std::max(st1.st_size, st2.st_size);

  int timeout = (max_size >= 512 * 1024) ? 300 :
    90 + static_cast<int>((1.0 - std::pow(1.0 - (double)max_size / (512 * 1024), 2)) * 210);

  pid_t pid = fork();

  if (pid == 0)
  {
    char vbuf[32];
    snprintf(vbuf, sizeof(vbuf), "%d", num_vars);

    execlp("qsole",
        "qsole",
        f1,
        f2,
        vbuf,
        "--check",
        "skeq",
        (char*)NULL);

    perror("execlp qsole failed");
    _exit(1);
  }

  if (pid < 0)
    return -1;

  auto start = std::chrono::steady_clock::now();

  while (true)
  {
    int status;
    pid_t r = waitpid(pid, &status, WNOHANG);

if (r == pid)
{
    if (!WIFEXITED(status))
        return -1;

    return WEXITSTATUS(status);
}

    if (r == -1)
      return -1;

    auto elapsed =
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::steady_clock::now() - start).count();

    if (elapsed >= timeout)
    {
      std::cerr << "[qsole] timeout after "
        << timeout
        << " seconds\n";

      kill(pid, SIGKILL);
      waitpid(pid, nullptr, 0);

      return -2;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

SemanticOracle::SemanticOracle() {
  taut_checker = new CNFTautChecker;
  fqbf_solver = new DepQBFSolver;
}

bool SemanticOracle::is_taut(
    const ResidualQBF& qbf
    ) {
  return taut_checker->is_taut(qbf);
}

OracleResult SemanticOracle::is_fqbf(
    const ResidualQBF& qbf
    ) {
  return fqbf_solver->is_fqbf(qbf);
}


bool SemanticOracle::equivalent(
    const ResidualQBF& left,
    const ResidualQBF& right)
{

  auto f1 = left.materialize();
  auto f2 = right.materialize();

  if (f1->num_variables != f2->num_variables) return false;

  std::ofstream out1("left.qdimacs");
  std::ofstream out2("right.qdimacs");

  out1 << QDIMACSFormatter::format(*f1);
  out2 << QDIMACSFormatter::format(*f2);

  out1.close();
  out2.close();



  int result = run_qsole("left.qdimacs", "right.qdimacs", f1->num_variables);

std::remove("left.qdimacs");
std::remove("right.qdimacs");


    if (result == 10) return true;
    if (result == 20) return false;

  if (result == -2)
  {
    std::cerr << "[SemanticOracle] qsole timed out, treating as NOT equivalent\n";
    return false;
  }

  throw std::runtime_error("qsole failed: " + std::to_string(result));
}
