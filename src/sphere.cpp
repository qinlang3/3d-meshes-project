#include "sphere.h"
#include <iostream>

void sphere(
  const int num_faces_u,
  const int num_faces_v,
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
  int v = num_faces_u * (num_faces_v+1);
  int f = num_faces_u * num_faces_v;
  double pi = 2 * acos(0.0);
  double long_delta = (2 * pi) / num_faces_u;
  double lat_delta = pi / num_faces_v;
  V.resize(v, 3);
  F.resize(f, 4);
  UV.resize((num_faces_u + 1) * (num_faces_v + 1), 2);
  UF.resize(f, 4);
  NV.resize(v, 3);
  NF.resize(f, 4);
  int count_v = 0;
  int count_f = 0;
  for (int i = 0; i < num_faces_u; i++) {
	for (int j = 0; j < num_faces_v; j++) {
	  V(count_v, 0) = cos(i * long_delta) * sin(j * lat_delta);
	  V(count_v, 1) = sin(i * long_delta) * sin(j * lat_delta);
	  V(count_v, 2) = cos(j * lat_delta);
	  NV(count_v, 0) = V(count_v, 0);
	  NV(count_v, 1) = V(count_v, 1);
	  NV(count_v, 2) = V(count_v, 2);
	  UV(count_v, 0) = i/(double)num_faces_u;
	  UV(count_v, 1) = 1-j/(double)num_faces_v;

	  F(count_f, 0) = count_v+1;
	  F(count_f, 1) = count_v+1+1;
	  F(count_f, 2) = i != num_faces_u-1 ? count_v+1+1+num_faces_v+1 : j+1+1;
	  F(count_f, 3) = i != num_faces_u-1 ? count_v+1+num_faces_v+1 : j+1;

	  UF(count_f, 0) = F(count_f, 0);
	  UF(count_f, 1) = F(count_f, 1);
	  UF(count_f, 2) = count_v+1+1+num_faces_v+1;
	  UF(count_f, 3) = count_v+1+num_faces_v+1;

	  NF(count_f, 0) = F(count_f, 0);
	  NF(count_f, 1) = F(count_f, 1);
	  NF(count_f, 2) = F(count_f, 2);
	  NF(count_f, 3) = F(count_f, 3);
	  count_v++;
	  count_f++;
	}
	// The last vertex in latitudinal direction
	V(count_v, 0) = V(0, 0);   // 0
	V(count_v, 1) = V(0, 1);   // 0
	V(count_v, 2) = -V(0, 2);  // -1
	NV(count_v, 0) = V(count_v, 0);
	NV(count_v, 1) = V(count_v, 1);
	NV(count_v, 2) = V(count_v, 2);
	UV(count_v, 0) = i / (double)num_faces_u;
	UV(count_v, 1) = 0;
	count_v++;
  }

  // The extra UV maps.
  for (int j = 0; j < num_faces_v; j++) {
	UV(count_v, 0) = 1;
	UV(count_v, 1) = 1 - j / (double)num_faces_v;
	count_v++;
  }
  UV(count_v, 0) = 1;
  UV(count_v, 1) = 0;

}
