/*
 * @Description: 
 * @Date: 2022-05-19 20:02:25
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-19 20:02:43
 * @FilePath: \Developmental-Networks\CCILCA\source\CCILCA.cc
 */
#include "CCILCA.h"
#include <iostream>

namespace mirror {
CCILCA::CCILCA(int topK, int xDim) : topK_(topK), xDim_(xDim) {
	lobeVecs_ = RowMajorMatrix::Random(topK_, xDim_);
	mean_ = lobeVecs_.colwise().mean();
	lobeVecs_.rowwise() -= mean_;
	ages_.resize(topK_, 1);
	step_ = 1;
}

void CCILCA::Update(const float* x) {
	if (x == NULL) {
		std::cout << "input empty." << std::endl;
		return;
	}
	// 1. update mean
	float tOverN = (float)(step_ - 1) / (float)(step_);
	float oneOverN = 1.0f / (float)(step_);
	Eigen::RowVectorXf inputX = Eigen::Map<const Eigen::RowVectorXf>(x, xDim_);
	mean_ = tOverN * mean_ + oneOverN * inputX;
	inputX -= mean_;

	if (step_ > 1) {
		// 2. init topk lobe
		if (step_ - 2 < topK_) {
			lobeVecs_.row(step_ - 2) = inputX;
		} else {
			// 3.update lobe
			int maxIndex = -1;
			double maxRes = ComputeMaxResponse(inputX, maxIndex);
			int age = ++ages_[maxIndex];
			float w1 = 0.0f, w2 = 0.0f;
			Amnesic(age, w1, w2);
			lobeVecs_.row(maxIndex) = w1 * lobeVecs_.row(maxIndex) +
				w2 * maxRes * inputX;
		}
	}
	
	step_++;
}

const std::vector<std::vector<float>> CCILCA::getLobeVectors() const {
	std::vector<std::vector<float>> results(topK_);
	for (int i = 0; i < topK_; ++i) {
		results[i].resize(xDim_);
		for (int j = 0; j < xDim_; ++j) {
			results[i][j] = lobeVecs_(i, j);
		}
	}
	return results;
}
void CCILCA::Amnesic(int t, float& w1, float& w2) {
	float t1 = 20.0f, t2 = 200.0f, r = 5000.0f, mu = 0.0f;
	if (t >= t1 && t < t2) {
		mu = 2.0f * (t - t1) / (t2 - t1);
	}
	else if (t >= t2) {
		mu = 2.0f + (t - t2) / r;
	}
	else {
		mu = t;
	}
	w1 = (t - 1 - mu) / t;
	w2 = (1 + mu) / t;
}

float CCILCA::ComputeMaxResponse(const Eigen::RowVectorXf& inputX, int& index) {
	auto result = lobeVecs_ * inputX.transpose();
	Eigen::ArrayXXf absResult = result.array().abs();
	float maxRes = 0, maxIndex = 0;
	for (int i = 0; i < absResult.size(); ++i) {
		if (absResult(i) > maxRes) {
			maxRes = absResult(i);
			maxIndex = i;
		}
	}
	index = maxIndex;
	maxRes /= lobeVecs_.row(maxIndex).norm();
	return maxRes;
}
} // namespace mirror