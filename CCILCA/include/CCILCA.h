/*
 * @Description: 
 * @Date: 2022-05-19 20:02:15
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-19 20:06:49
 * @FilePath: \Developmental-Networks\CCILCA\include\CCILCA.h
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
class CCILCA {
public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="topK">特征向量数目</param>
	/// <param name="xDim">特征向量维度</param>
	API CCILCA(int topK, int xDim);
	API ~CCILCA() {}

	/// <summary>
	/// 更新叶特征向量
	/// </summary>
	/// <param name="x">当前输入</param>
	API void Update(const float* x);

	/// <summary>
	/// 获取提取到的叶特征向量
	/// </summary> 
	API const std::vector<std::vector<float>> getLobeVectors() const;

private:
	void Amnesic(int t, float& w1, float& w2);
	float ComputeMaxResponse(const Eigen::RowVectorXf& inputX, int& index);

private:
	int topK_, xDim_;
	RowMajorMatrix lobeVecs_;
	Eigen::RowVectorXf lobeVecsNorm_;
	Eigen::RowVectorXf mean_;
	std::vector<int> ages_;
	int step_;
};

} // namespace mirror