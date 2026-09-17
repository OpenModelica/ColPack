#include "ColPackHeaders.h"

int main()
{
  // Distance-1 coloring of the path graph 0 - 1 - 2 needs 2 colors.
  unsigned int row0[] = {1, 1};
  unsigned int row1[] = {2, 0, 2};
  unsigned int row2[] = {1, 1};
  unsigned int *adolc[] = {row0, row1, row2};

  ColPack::GraphColoringInterface g(SRC_MEM_ADOLC, adolc, 3);
  g.Coloring("NATURAL", "DISTANCE_ONE");
  return g.GetVertexColorCount() == 2 ? 0 : 1;
}
