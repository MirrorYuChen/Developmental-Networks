/*
 * @Description: 
 * @Date: 2022-05-21 19:25:10
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-22 01:12:15
 * @FilePath: \MILN\include\ClassifierCortex.h
 */
#pragma once

#include "RectLayer.h"

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
class RectLayer;
class ClassifierCortex {
public:
    API ClassifierCortex(int xDim, int yDim, int topK, double beta);
    API ~ClassifierCortex() = default;

    API void Update(const double* x, const int& y);
    API bool Classify(const double* x, const int& y);

private:
    int xDim_, yDim_, topK_;
    RectLayer hiddenLayer_, outputLayer_;    
};

} // namespace mirror