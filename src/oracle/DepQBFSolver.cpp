#include "oracle/DepQBFSolver.hpp"

#include <sys/wait.h>
#include <unistd.h>
#include "util/QDIMACSFormatter.hpp"
#include <fstream>

int run_depqbf(const char *qdimacs_file)
{
  pid_t pid = fork();

  if (pid == 0)
  {
    execlp("depqbf", "depqbf", "--max-secs=1200",
        qdimacs_file,
        (char *)NULL);

    perror("execl");
    exit(1);
  }

  int status;
  waitpid(pid, &status, 0);

  if (!WIFEXITED(status))
    return -1;

  return WEXITSTATUS(status);
}

OracleResult DepQBFSolver::is_fqbf(const ResidualQBF& qbf) {
  auto base_result = FQBFSolver::is_fqbf(qbf);
  return base_result;

  /*
  std::ofstream out("temp.txt", std::ios::out | std::ios::trunc);

  if (!out) {
    throw std::runtime_error("Could not create temp.txt");
  }

  out << QDIMACSFormatter::format(formula);
  out.flush();
  out.close();

  int result = run_depqbf("temp.txt");
  if (result == 10)
    return false;
  else if (result == 20)
    return true;
  else
  {
    printf("Solver returned %d\n", result);
    return false; 
  }*/
}
