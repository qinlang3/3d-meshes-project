#include "write_obj.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_obj(
  const std::string & filename,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & UV,
  const Eigen::MatrixXi & UF,
  const Eigen::MatrixXd & NV,
  const Eigen::MatrixXi & NF)
{
  assert((F.size() == 0 || F.cols() == 3 || F.cols() == 4) && "F must have 3 or 4 columns");
  std::ofstream output_file(filename);
  if (!output_file) return false;
  for (int i = 0; i < V.rows();i++) {
    if (!(output_file << "v" << " " << V.row(i) << std::endl)) return false;
  }
  for (int i = 0; i < UV.rows();i++) {
    if (!(output_file << "vt" << " " << UV.row(i) << std::endl)) return false;
  }
  for (int i = 0; i < NV.rows();i++) {
    if (!(output_file << "vn" << " " << NV.row(i) << std::endl)) return false;
  }
  for (int i = 0; i < F.rows();i++) {
    if (!(output_file << "f")) return false;
    for (int j = 0; j < F.cols();j++) {
      if (!(output_file << " " << F(i,j) << "/" << UF(i, j) << "/" << NF(i, j))) return false;
    }
    if (!(output_file << std::endl)) return false;
  }
  return true;
}
