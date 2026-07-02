#include "oracle/SemanticOracle.hpp"
#include "oracle/CNFTautChecker.hpp"
#include "oracle/DepQBFSolver.hpp"
#include "util/QDIMACSFormatter.hpp"
#include "simplify/Canonicalizer.hpp"

#include <algorithm>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <stdexcept>

int run_qsole(const char *f1, const char *f2, int num_vars)
{
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
        exit(1);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1)
    return -1;

    if (!WIFEXITED(status))
        return -1;

    return WEXITSTATUS(status);
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
    QBFFormula f1 = Canonicalizer::canonicalize_formula(left.materialize());
    QBFFormula f2 = Canonicalizer::canonicalize_formula(right.materialize());

    std::ofstream out1("left.qdimacs");
    std::ofstream out2("right.qdimacs");

    out1 << QDIMACSFormatter::format(f1);
    out2 << QDIMACSFormatter::format(f2);

    out1.close();
    out2.close();

    if (f1.num_variables == f2.num_variables) return false;

int num_vars = f1.num_variables;

    int result = run_qsole("left.qdimacs", "right.qdimacs", num_vars);

    if (result == 10) return true;
    if (result == 20) return false;

    throw std::runtime_error("qsole failed: " + std::to_string(result));
}
