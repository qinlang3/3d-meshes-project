#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include <iostream>

void per_corner_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double corner_threshold,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(F.rows()*3,3);
  std::vector<std::vector<int> > VF;
  vertex_triangle_adjacency(F, V.rows(), VF);
  Eigen::RowVector3d a, b, c, n_f, n_sum, n;
  double pi = 2 * acos(0.0);
  double threshold = cos((corner_threshold/180) * pi);

  for (int i = 0; i < F.rows(); i++) {
	n_f = (triangle_area_normal(V.row(F(i, 0)), V.row(F(i, 1)), V.row(F(i, 2)))).normalized();
	
	// first corner
	n_sum = Eigen::RowVector3d(0, 0, 0);
	for (int j = 0; j < VF[F(i, 0)].size(); j++) {
	  a = V.row(F(VF[F(i, 0)][j], 0));
	  b = V.row(F(VF[F(i, 0)][j], 1));
	  c = V.row(F(VF[F(i, 0)][j], 2));
	  n = triangle_area_normal(a, b, c); 
	  if(n_f.dot(n.normalized()) > threshold) n_sum += n;
	}
	N.row(i*3) = n_sum.normalized();

	// second corner
	n_sum = Eigen::RowVector3d(0, 0, 0);
	for (int j = 0; j < VF[F(i, 1)].size(); j++) {
	  a = V.row(F(VF[F(i, 1)][j], 0));
	  b = V.row(F(VF[F(i, 1)][j], 1));
	  c = V.row(F(VF[F(i, 1)][j], 2));
	  n = triangle_area_normal(a, b, c);
	  if (n_f.dot(n.normalized()) > threshold) n_sum += n;
	}
	N.row(i*3+1) = n_sum.normalized();

	// thrid corner
	n_sum = Eigen::RowVector3d(0, 0, 0);
	for (int j = 0; j < VF[F(i, 2)].size(); j++) {
	  a = V.row(F(VF[F(i, 2)][j], 0));
	  b = V.row(F(VF[F(i, 2)][j], 1));
	  c = V.row(F(VF[F(i, 2)][j], 2));
	  n = triangle_area_normal(a, b, c);
	  if (n_f.dot(n.normalized()) > threshold) n_sum += n;
	}
	N.row(i*3+2) = n_sum.normalized();
  }
}
