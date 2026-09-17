// Star colors the sparsity pattern of a tridiagonal 5x5 Hessian and checks
// the resulting seed matrix.
#include "ColPackHeaders.h"

using namespace ColPack;

int main()
{
  const int n = 5;
  // ADOL-C format: row[0] is the number of nonzeros, followed by the columns.
  unsigned int **pattern = new unsigned int *[n];
  for (int i = 0; i < n; i++)
  {
    pattern[i] = new unsigned int[4];
    unsigned int nnz = 0;
    for (int j = i - 1; j <= i + 1; j++)
      if (j >= 0 && j < n)
        pattern[i][++nnz] = j;
    pattern[i][0] = nnz;
  }

  GraphColoringInterface g(SRC_MEM_ADOLC, pattern, n);
  g.Coloring("SMALLEST_LAST", "STAR");

  int rows = 0;
  int cols = 0;
  g.GetSeedMatrix(&rows, &cols);
  std::cout << "Seed matrix: " << rows << "x" << cols << std::endl;

  for (int i = 0; i < n; i++)
    delete[] pattern[i];
  delete[] pattern;

  // A star coloring of a tridiagonal matrix needs at least 3 colors, the
  // greedy heuristic may use more.
  return (rows == n && cols >= 3 && cols <= n) ? 0 : 1;
}
