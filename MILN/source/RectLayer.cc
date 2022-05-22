/*
 * @Description: 
 * @Date: 2022-05-21 15:28:22
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-21 22:52:36
 * @FilePath: \MILN\source\RectLayer.cc
 */
#include "RectLayer.h"

namespace mirror {
RectLayer::RectLayer(int width, int height, double beta) :
    width_(width), height_(height) {
    for (int h = 0; h < height_; ++h) {
        for (int w = 0; w < width_; ++w) {
            AddNeuron(Neuron(1-beta, 0, 0, beta));
            neuronVec_[h * width_ + w].setPosition(w, h, 0);
        }
    }
}

void RectLayer::AddNeuron(Neuron n) {
    neuronVec_.push_back(n);
}


} // namespace mirror