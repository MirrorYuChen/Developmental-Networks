/*
 * @Description: 
 * @Date: 2022-05-19 10:34:27
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-19 10:36:05
 * @FilePath: \Developmental-Networks\CCIPCA\include\CCIPCA.h
 */
#pragma once
#include <Eigen/Eigen>
#include <vector>

#if defined (_MSC_VER) || defined (_WIN32) || defined (_WIN64)
#ifndef API_EXPORTS
#define  API __declspec(dllexport)
#else
#define  API __declspec(dllimport)
#endif // API
#else // defined (windows)
#define API
#endif

namespace mirror {
typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RowMajorMatrix;

class CCIPCA {
public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="topK">特征向量数目</param>
	/// <param name="xDim">特征向量维度</param>
	API CCIPCA(int topK, int xDim);
	API ~CCIPCA() {}

	/// <summary>
	/// 更新特征向量
	/// </summary>
	/// <param name="x">当前输入</param>
	API void Update(const float* x);

	/// <summary>
	/// 获取提取到的特征向量
	/// </summary> 
	API const std::vector<std::vector<float>> getEigenVectors() const;

private:
	void Amnesic(int t, float& w1, float& w2);

private:
	int topK_, xDim_;
	RowMajorMatrix eigenVecs_;
	Eigen::RowVectorXf eigenVecsNorm_;
	Eigen::RowVectorXf mean_;
	int step_;
};



}