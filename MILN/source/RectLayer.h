/*
 * @Description: 
 * @Date: 2022-05-21 15:28:16
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-21 22:56:14
 * @FilePath: \MILN\source\RectLayer.h
 */
#pragma once

#include <vector>
#include "Neuron.h"

namespace mirror {
class RectLayer {
public:
    RectLayer(int width=0, int height=0, double beta=0);
    ~RectLayer() = default;

    void AddNeuron(Neuron n);
    inline size_t Size() const {
        return neuronVec_.size();
    }

public:
    std::vector<Neuron> neuronVec_;
    int width_, height_;
};


} // namespace mirror