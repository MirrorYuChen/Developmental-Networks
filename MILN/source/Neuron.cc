/*
 * @Description: 
 * @Date: 2022-05-21 14:11:36
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-22 11:34:29
 * @FilePath: \MILN\MILN\Neuron.cc
 */
#include "Neuron.h"
#include <iostream>

namespace mirror {
Neuron::Neuron(double alpha, double beta, double gamma, double delta) :
    alpha_(alpha), beta_(beta), gamma_(gamma), delta_(delta) {
    age_ = 1;
    response_ = 0;
    w1_ = 0;
    w2_ = 1.0;
}

double Neuron::Amnesic(int t) {
    double t1 = 10.0, t2 = 1000.0, c = 2.0, r = 10000.0;
    double ret = 0.0;
    if (t <= t1) {
        ret = 0.0;
    } else if (t <= t2) {
        ret = c * (double)(t - t1) / (t2 - t1);
    } else {
        ret = c + (double)(t - t2) / r;
    }
    return ret;
}

double Neuron::ComputeResponse(const std::vector<Connect>& c) {
    double sum = 0.0, normX = 0.0, normW = 0.0;
    if (c.size() == 0) {
        std::cout << "Connect is empty." << std::endl;
        return 0.0;
    }
    for (int i = 0; i < c.size(); ++i) {
        float xi = c[i].from ? c[i].from->getResponse() : c[i].input;
        if (xi < 0) {
            std::cout << "error Neuron::ComputeResponse." << std::endl;
            return 0.0;
        }
        sum += c[i].weight * xi;
        normX += xi * xi;
        normW += c[i].weight * c[i].weight;
    }
    if (normW == 0 || normX == 0) {
        return 0.0;
    }
    normW = sqrt(normW);
    normX = sqrt(normX);
    return sum / (normW * normX);
}

void Neuron::setPosition(int x, int y, int z) {
    positionX_ = x;
    positionY_ = y;
    positionZ_ = z;
}

void Neuron::AddConnect(Connect c, int type) {
    switch(type) {
    case BOTTOM_UP:
        wb_.push_back(c);
        break;
    case TOP_DOWN:
        wt_.push_back(c);
        break;
    }
}

void Neuron::Update() {
    for (int i = 0; i < wb_.size(); ++i) {
        wb_[i].weight = w1_ * wb_[i].weight + w2_ * response_ * (
            wb_[i].from ? wb_[i].from->getResponse() : wb_[i].input
        );
        if (wb_[i].weight < 0) {
            std::cout << "Neuron::Update() negatove weight" << std::endl;
        }
    }
    age_++;
    double temp = Amnesic(age_);
    w1_ = (double) (age_ - 1 - temp) / (double)(age_);
    w2_ = (1.0 + temp) / (double)(age_);
}

void Neuron::Fire() {
    if (delta_ == 0) {
        response_ = Active(alpha_ * ComputeResponse(wb_));
    } else {
        response_ = Active(alpha_ * ComputeResponse(wb_) + delta_ * ComputeResponse(wt_));
    }
}

double Neuron::Active(double x) {
    return x;
}

} // namespace mirror