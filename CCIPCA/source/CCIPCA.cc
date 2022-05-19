/*
 * @Description: 
 * @Date: 2022-05-19 10:35:10
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-19 10:35:26
 * @FilePath: \Developmental-Networks\CCIPCA\source\CCIPCA.cc
 */
#include "CCIPCA.h"
#include <iostream>

namespace mirror {
CCIPCA::CCIPCA(int topK, int xDim) :
	topK_(topK), xDim_(xDim) {
	step_ = 1;
	eigenVecs_ = RowMajorMatrix::Random(topK, xDim);
	mean_ = eigenVecs_.colwise().mean();
	eigenVecs_.rowwise() -= mean_;
}

void CCIPCA::Update(const float* x) {
	if (x == NULL) {
		std::cout << "input empty." << std::endl;
		return;
	}

	// 1. update mean
	float tOverN = (float)(step_ - 1) / (float)(step_);
	float oneOverN = 1.0f / (float)(step_);
	Eigen::RowVectorXf inputX = Eigen::Map<const Eigen::RowVectorXf>(x, xDim_);
	mean_ = tOverN * mean_ + oneOverN * inputX;
	
	if (step_ > 1) {
		inputX -= mean_;
		int k = step_ < topK_ ? step_ : topK_;
		float w1 = 0.0f, w2 = 0.0f;
		Amnesic(step_, w1, w2);
		for (int i = 0; i < k; ++i) {
			auto v = eigenVecs_.row(i);
			auto vn = v;
			if (i == k - 1) {
				v = inputX;
				vn = v / v.norm();
			} else {
				v = w1 * v + w2 * inputX * v.transpose() / v.norm() * inputX;
				vn = v / v.norm();
			}
			// Gram-Schimdt
			inputX -= inputX * vn.transpose() * vn;
			eigenVecs_.row(i) = v;
		}
	}
	++step_;
}

const std::vector<std::vector<float>> CCIPCA::getEigenVectors() const {
	std::vector<std::vector<float>> results(topK_);
	for (int i = 0; i < topK_; ++i) {
		results[i].resize(xDim_);
		for (int j = 0; j < xDim_; ++j) {
			results[i][j] = eigenVecs_(i, j);
		}
	}
	return results;
}

void CCIPCA::Amnesic(int t, float& w1, float& w2) {
	float t1 = 20.0f, t2 = 200.0f, r = 2000.0f, c = 2.0f, mu = 0.0f;
	if (t < t1) {
		mu = 0;
	}
	else if (t >= t1 && t < t2) {
		mu = c * (t - t1) / (t2 - t1);
	} else {
		mu = c + (t - t2) / r;
	}
	w1 = (t - 1 - mu) / t;
	w2 = (1 + mu) / t;
}

} // namespace mirror