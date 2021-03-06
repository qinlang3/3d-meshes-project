#include "per_vertex_normals.h"
#include "triangle_area_normal.h"

void per_vertex_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(V.rows(),3);
  Eigen::RowVector3d n;
  for (int i = 0;i < V.rows(); i++) {
    n = Eigen::RowVector3d(0, 0, 0);
    for (int j = 0; j < F.rows(); j++) {
      if ((F(j, 0) == i)||(F(j, 1) == i)||(F(j, 2) == i)) {
        n += triangle_area_normal(V.row(F(j, 0)), V.row(F(j, 1)), V.row(F(j, 2)));
      }
    }
    N.row(i) = n.normalized();
  }
}
