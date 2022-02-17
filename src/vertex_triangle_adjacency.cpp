#include "vertex_triangle_adjacency.h"

void vertex_triangle_adjacency(
  const Eigen::MatrixXi & F,
  const int num_vertices,
  std::vector<std::vector<int> > & VF)
{
  VF.resize(num_vertices);
  for (int i = 0; i < F.rows(); i++) {
	VF[F(i, 0)].push_back(i);
	VF[F(i, 1)].push_back(i);
	VF[F(i, 2)].push_back(i);
  }
}

