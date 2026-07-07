#include "oracle/DepQBFSolver.hpp"

#include <sys/wait.h>
#include <unistd.h>
#include "util/QDIMACSFormatter.hpp"
#include <fstream>
#include <fcntl.h>

int run_depqbf(const char *qdimacs_file)
{
  pid_t pid = fork();

  if (pid == 0)
  {
    int dev_null = open("/dev/null", O_WRONLY);
    if (dev_null < 0) {
      perror("Failed to open /dev/null");
      exit(1);
    }

    dup2(dev_null, STDOUT_FILENO);
    dup2(dev_null, STDERR_FILENO);

    close(dev_null);
    execlp("depqbf", "depqbf", "--max-secs=300",
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
  if (base_result != OracleResult::UNKNOWN)
    return base_result;

  // return OracleResult::UNKNOWN;

  std::ofstream out("temp.txt", std::ios::out | std::ios::trunc);

  if (!out) {
    throw std::runtime_error("Could not create temp.txt");
  }

  out << QDIMACSFormatter::format(*(qbf.materialize()));
  out.flush();
  out.close();

  int result = run_depqbf("temp.txt");
  
  std::remove("temp.txt");
  
  if (result == 10)
    return OracleResult::TRUE_RESULT;
  else if (result == 20)
    return OracleResult::FALSE_RESULT;
  else
  {
    printf("Solver returned %d\n", result);
    return OracleResult::UNKNOWN; 
  }
}
