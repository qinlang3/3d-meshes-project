#include "catmull_clark.h"
#include <unordered_map>
#include <utility>
#include <functional>
#include <iostream>

void catmull_clark(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const int num_iters,
  Eigen::MatrixXd & SV,
  Eigen::MatrixXi & SF)
{
  Eigen::MatrixXd SSV = Eigen::MatrixXd::Zero(V.rows()+F.rows()*3, 3);
  Eigen::MatrixXi SSF = Eigen::MatrixXi::Zero(F.rows() * 4, 4);

  Eigen::RowVector3d face_p, edge_p_a, edge_p_b, edge_p_c, edge_p_d;
  Eigen::MatrixXi MAP_FP = Eigen::MatrixXi::Zero(F.rows(), 5);
  Eigen::MatrixXi MAP_AP = Eigen::MatrixXi::Zero(F.rows(), 9);

  if (num_iters == 0) {
	SV = V;
	SF = F;
  }else if (num_iters > 0) {
	int count = 0;
	
	// Adding face points.
	for (int i = 0; i < F.rows(); i++) {
	  face_p = (V.row(F(i, 0)) + V.row(F(i, 1)) + V.row(F(i, 2)) + V.row(F(i, 3))) / (double)4;
	  SSV.row(count) = face_p;
	  MAP_FP(i, 0) = count; // stores index into SSV
	  MAP_FP(i, 1) = F(i, 0);  // stores index into V
	  MAP_FP(i, 2) = F(i, 1);  // stores index into V
	  MAP_FP(i, 3) = F(i, 2);  // stores index into V
	  MAP_FP(i, 4) = F(i, 3);  // stores index into V
	  count++;
	}
	

	// Adding edge points.
	// For each edge of the face, find the adjacent face and compute correspoindg edge point, remove duplicates.
	for (int i = 0; i < F.rows(); i++) {
	  int duplicate = 0; // Indicate whether or not the edge has been visited.
	  int p_idx = 0; // If been visited, record the index into SSV of that edge point.
	  // compute edge point a
	  for (int j = 0; j < F.rows(); j++) { // Iterate through all the faces to find adjacent face of the edge.
		int found = 0;
		for (int k = 0; k < 4; k++) {
		  if (F(j, k) == MAP_FP(i, 2) && F(j, ((k != 3) ? (k + 1) : 0)) == MAP_FP(i, 1)) {
			if (j < i) {
			  duplicate = 1;
			  if (MAP_AP(j, 0) == F(j, k)) p_idx = MAP_AP(j, 1);
			  if (MAP_AP(j, 2) == F(j, k)) p_idx = MAP_AP(j, 3);
			  if (MAP_AP(j, 4) == F(j, k)) p_idx = MAP_AP(j, 5);
			  if (MAP_AP(j, 6) == F(j, k)) p_idx = MAP_AP(j, 7);
			}else {
			  edge_p_a = ((SSV.row(MAP_FP(i, 0))+SSV.row(MAP_FP(j, 0)))/(double)2 + (V.row(MAP_FP(i, 1))+V.row(MAP_FP(i, 2)))/(double)2)/(double)2;
			}	
			found = 1;
			break;
		  }
		}
		if (found) break;
	  }
	  if (duplicate) { // edge point has been computed before, avoid duplicates
		MAP_AP(i, 0) = F(i, 0); // stores index into V
		MAP_AP(i, 1) = p_idx; // stores index into SSV
	  }else { // add edge point to SSV
		SSV.row(count) = edge_p_a;
		MAP_AP(i, 0) = F(i, 0); // stores index into V
		MAP_AP(i, 1) = count; // stores index into SSV
		count++;
	  }

	  duplicate = 0; 
	  p_idx = 0;
	  // compute edge point b
	  for (int j = 0; j < F.rows(); j++) {
		int found = 0;
		for (int k = 0; k < 4; k++) {
		  if (F(j, k) == MAP_FP(i, 3) && F(j, ((k != 3) ? (k + 1) : 0)) == MAP_FP(i, 2)) {
			if (j < i) {
			  duplicate = 1;
			  if (MAP_AP(j, 0) == F(j, k)) p_idx = MAP_AP(j, 1);
			  if (MAP_AP(j, 2) == F(j, k)) p_idx = MAP_AP(j, 3);
			  if (MAP_AP(j, 4) == F(j, k)) p_idx = MAP_AP(j, 5);
			  if (MAP_AP(j, 6) == F(j, k)) p_idx = MAP_AP(j, 7);
			}else {
			  edge_p_b = ((SSV.row(MAP_FP(i, 0))+SSV.row(MAP_FP(j, 0)))/(double)2 + (V.row(MAP_FP(i, 2))+V.row(MAP_FP(i, 3)))/(double)2)/(double)2;
			}
			found = 1;
			break;
		  }
		}
		if (found) break;
	  }
	  if (duplicate) { // edge point has been computed before, avoid duplicates
		MAP_AP(i, 2) = F(i, 1); // stores index into V
		MAP_AP(i, 3) = p_idx; // stores index into SSV
	  }
	  else { // add edge point to SSV
		SSV.row(count) = edge_p_b;
		MAP_AP(i, 2) = F(i, 1); // stores index into V
		MAP_AP(i, 3) = count; // stores index into SSV
		count++;
	  }

	  duplicate = 0;
	  p_idx = 0;
	  // compute edge point c
	  for (int j = 0; j < F.rows(); j++) {
		int found = 0;
		for (int k = 0; k < 4; k++) {
		  if (F(j, k) == MAP_FP(i, 4) && F(j, ((k != 3) ? (k + 1) : 0)) == MAP_FP(i, 3)) {
			if (j < i) {
			  duplicate = 1;
			  if (MAP_AP(j, 0) == F(j, k)) p_idx = MAP_AP(j, 1);
			  if (MAP_AP(j, 2) == F(j, k)) p_idx = MAP_AP(j, 3);
			  if (MAP_AP(j, 4) == F(j, k)) p_idx = MAP_AP(j, 5);
			  if (MAP_AP(j, 6) == F(j, k)) p_idx = MAP_AP(j, 7);
			}else {
			  edge_p_c = ((SSV.row(MAP_FP(i, 0))+SSV.row(MAP_FP(j, 0)))/(double)2 + (V.row(MAP_FP(i, 3))+V.row(MAP_FP(i, 4)))/(double)2)/(double)2;
			}
			found = 1;
			break;
		  }
		}
		if (found) break;
	  }
	  if (duplicate) { // edge point has been computed before, avoid duplicates
		MAP_AP(i, 4) = F(i, 2); // stores index into V
		MAP_AP(i, 5) = p_idx; // stores index into SSV
	  }
	  else { // add edge point to SSV
		SSV.row(count) = edge_p_c;
		MAP_AP(i, 4) = F(i, 2); // stores index into V
		MAP_AP(i, 5) = count; // stores index into SSV
		count++;
	  }

	  duplicate = 0;
	  p_idx = 0;
	  // compute edge point d
	  for (int j = 0; j < F.rows(); j++) {
		int found = 0;
		for (int k = 0; k < 4; k++) {
		  if (F(j, k) == MAP_FP(i, 1) && F(j, ((k != 3) ? (k + 1) : 0)) == MAP_FP(i, 4)) {
			if (j < i) {
			  duplicate = 1;
			  if (MAP_AP(j, 0) == F(j, k)) p_idx = MAP_AP(j, 1);
			  if (MAP_AP(j, 2) == F(j, k)) p_idx = MAP_AP(j, 3);
			  if (MAP_AP(j, 4) == F(j, k)) p_idx = MAP_AP(j, 5);
			  if (MAP_AP(j, 6) == F(j, k)) p_idx = MAP_AP(j, 7);
			}else {
			  edge_p_d = ((SSV.row(MAP_FP(i, 0))+SSV.row(MAP_FP(j, 0)))/(double)2 + (V.row(MAP_FP(i, 4))+V.row(MAP_FP(i, 1)))/(double)2)/(double)2;
			}
			found = 1;
			break;
		  }
		}
		if (found) break;
	  }
	  if (duplicate) { // edge point has been computed before, avoid duplicates
		MAP_AP(i, 6) = F(i, 3); // stores index into V
		MAP_AP(i, 7) = p_idx; // stores index into SSV
	  }
	  else { // add edge point to SSV
		SSV.row(count) = edge_p_d;
		MAP_AP(i, 6) = F(i, 3); // stores index into V
		MAP_AP(i, 7) = count; // stores index into SSV
		count++;
	  }
	  MAP_AP(i, 8) = MAP_FP(i, 0);  // add the face point index
	}
	

	MAP_FP = Eigen::MatrixXi::Zero(F.rows(), 9);
	MAP_FP = MAP_AP;
	// Adding original points and modify them.
	Eigen::RowVector3d avg_F, avg_R, new_p;
	for (int i = 0; i < V.rows(); i++) {
	  int num = 0;
	  avg_F = Eigen::RowVector3d(0, 0, 0);
	  avg_R = Eigen::RowVector3d(0, 0, 0);
	  for (int j = 0; j < F.rows(); j++) {
		if (MAP_FP(j, 0) == i) {
		  avg_F += SSV.row(MAP_FP(j, 8));
		  avg_R += (V.row(MAP_FP(j, 0))+ V.row(MAP_FP(j, 2)))/(double) 2;
		  num++;
		  MAP_AP(j, 0) = count;
		}
		if (MAP_FP(j, 2) == i) {
		  avg_F += SSV.row(MAP_FP(j, 8));
		  avg_R += (V.row(MAP_FP(j, 2))+ V.row(MAP_FP(j, 4)))/(double) 2;
		  num++;
		  MAP_AP(j, 2) = count;
		}
		if (MAP_FP(j, 4) == i) {
		  avg_F += SSV.row(MAP_FP(j, 8));
		  avg_R += (V.row(MAP_FP(j, 4))+ V.row(MAP_FP(j, 6)))/(double) 2;
		  num++;
		  MAP_AP(j, 4) = count;
		}
		if (MAP_FP(j, 6) == i) {
		  avg_F += SSV.row(MAP_FP(j, 8));
		  avg_R += (V.row(MAP_FP(j, 6))+ V.row(MAP_FP(j, 0)))/(double) 2;
		  num++;
		  MAP_AP(j, 6) = count;
		}
	  }
	  new_p = (avg_F/(double)num+2*(avg_R/(double)num)+(num-3)*V.row(i))/(double)num;
	  SSV.row(count) = new_p;
	  count++;
	}

	// Update SSF
	for (int i = 0; i < F.rows(); i++) {
	  SSF.row(i*4) = Eigen::RowVector4i(MAP_AP(i, 0), MAP_AP(i, 1), MAP_AP(i, 8), MAP_AP(i, 7));
	  SSF.row(i*4+1) = Eigen::RowVector4i(MAP_AP(i, 1), MAP_AP(i, 2), MAP_AP(i, 3), MAP_AP(i, 8));
	  SSF.row(i*4+2) = Eigen::RowVector4i(MAP_AP(i, 8), MAP_AP(i, 3), MAP_AP(i, 4), MAP_AP(i, 5));
	  SSF.row(i*4+3) = Eigen::RowVector4i(MAP_AP(i, 7), MAP_AP(i, 8), MAP_AP(i, 5), MAP_AP(i, 6));
	}
	

	// Recursive call
	catmull_clark(SSV, SSF, num_iters-1, SV, SF);
  }
}
