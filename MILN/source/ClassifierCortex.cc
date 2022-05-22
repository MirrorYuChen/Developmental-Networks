/*
 * @Description: 
 * @Date: 2022-05-21 19:36:01
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-22 11:45:33
 * @FilePath: \MILN\MILN\ClassifierCortex.cc
 */
#include "ClassifierCortex.h"
#include "Neuron.h"
#include <iostream>
#include <map>
#include <algorithm>

namespace mirror {
ClassifierCortex::ClassifierCortex(int xDim, int yDim, int topK, double beta) : 
    xDim_(xDim), yDim_(yDim), topK_(topK) {
    // 1.create layer
    hiddenLayer_ = RectLayer(40, 40, beta);
    outputLayer_ = RectLayer(1, yDim_, 0.0);
    
    // 2. create connect
    for (int i = 0; i < hiddenLayer_.Size(); ++i) {
        for (int j = 0; j < xDim_; ++j) {
            Connect c(0.0, NULL);
            hiddenLayer_.neuronVec_[i].AddConnect(c, BOTTOM_UP);
        }
        for (int j = 0; j < yDim_; ++j) {
            Connect c(0.0, &(outputLayer_.neuronVec_[j]));
            hiddenLayer_.neuronVec_[i].AddConnect(c, TOP_DOWN);
        }
    }

    for (int i = 0; i < outputLayer_.Size(); ++i) {
        for (int j = 0; j < hiddenLayer_.Size(); ++j) {
            Connect c(0.0, &(hiddenLayer_.neuronVec_[j]));
            outputLayer_.neuronVec_[i].AddConnect(c, BOTTOM_UP);
        }
    }
    
    // 3. init hidden layer weight
    for (int i = 0; i < hiddenLayer_.Size(); ++i) {
        for (int j = 0; j < xDim_; ++j) {
            hiddenLayer_.neuronVec_[i].wb_[j].weight = rand() % 255;
        }
    }
}

void ClassifierCortex::Update(const double* x, const int& y) {
    // 1. set input
    for (int i = 0; i < hiddenLayer_.Size(); ++i) {
        for (int j = 0; j < xDim_; ++j) {
            hiddenLayer_.neuronVec_[i].wb_[j].input = x[j];
        }
    }

    // 2. Clear output response and set current label to related neuron
    for (int i = 0; i < outputLayer_.Size(); ++i) {
        outputLayer_.neuronVec_[i].response_ = 0;
    }
    outputLayer_.neuronVec_[y].response_ = 1.0;

    // 3. compute response and sort
    std::vector<std::pair<int, double>> resVec;
    for (int i = 0; i < hiddenLayer_.Size(); ++i) {
        hiddenLayer_.neuronVec_[i].Fire();
        double res = hiddenLayer_.neuronVec_[i].getResponse();
        resVec.push_back(std::make_pair(i, res));
    }
    std::sort(resVec.begin(), resVec.end(), 
        [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.second > b.second;
        }
    );
    
    // 4.only save topK response
    for (int i = topK_; i < hiddenLayer_.Size(); ++i) {
        hiddenLayer_.neuronVec_[resVec[i].first].response_ = 0;
    }

    // 5.lateral excited 3x3 neighbors
    for (int i = 0; i < hiddenLayer_.Size(); ++i) {
        int positionX = hiddenLayer_.neuronVec_[i].positionX_;
        int positionY = hiddenLayer_.neuronVec_[i].positionY_;

		int topKPositionX = hiddenLayer_.neuronVec_[resVec[0].first].positionX_;
		int topKPositionY = hiddenLayer_.neuronVec_[resVec[0].first].positionY_;
        if (fabs(positionX - topKPositionX) <= 1 &&
            fabs(positionY - topKPositionY) <= 1) {
            hiddenLayer_.neuronVec_[i].Fire();
            hiddenLayer_.neuronVec_[i].Update();
        }

    }

    // 6. recompute topk response 
    for (int i = 0; i < topK_; ++i) {
        hiddenLayer_.neuronVec_[resVec[i].first].response_ = (1.0 / (i + 1.0)) * hiddenLayer_.neuronVec_[resVec[i].first].getResponse();
    }
    outputLayer_.neuronVec_[y].Update();

    // 7.update top down connect
    for (int i = 0; i < hiddenLayer_.Size(); ++i) {
        for (int j = 0; j < yDim_; ++j) {
            hiddenLayer_.neuronVec_[i].wt_[j].weight = outputLayer_.neuronVec_[j].wb_[i].weight;
        }
    }
}

bool ClassifierCortex::Classify(const double* x, const int& y) {
     // 1. set input
    for (int i = 0; i < hiddenLayer_.Size(); ++i) {
        for (int j = 0; j < xDim_; ++j) {
            hiddenLayer_.neuronVec_[i].wb_[j].input = x[j];
        }
    }

    // 2. Clear output response
    for (int i = 0; i < outputLayer_.Size(); ++i) {
        outputLayer_.neuronVec_[i].response_ = 0;
    }

    // 3. compute response and sort
    std::vector<std::pair<int, double>> resVec;
    for (int i = 0; i < hiddenLayer_.Size(); ++i) {
        hiddenLayer_.neuronVec_[i].Fire();
        double res = hiddenLayer_.neuronVec_[i].getResponse();
        resVec.push_back(std::make_pair(i, res));
    }
    std::sort(resVec.begin(), resVec.end(), 
        [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.second > b.second;
        }
    );
    
    // 4.only save topK response
    for (int i = topK_; i < hiddenLayer_.Size(); ++i) {
        hiddenLayer_.neuronVec_[resVec[i].first].response_ = 0;
    }

    // 5. recompute topk response 
    for (int i = 0; i < topK_; ++i) {
        hiddenLayer_.neuronVec_[resVec[i].first].response_ = (1.0 / (i + 1.0)) * hiddenLayer_.neuronVec_[resVec[i].first].getResponse();
    }
    
    double maxResponse = -1.0;
    int maxIndex = -1;
    for (int i = 0; i < outputLayer_.Size(); ++i) {
        outputLayer_.neuronVec_[i].Fire();
        if (outputLayer_.neuronVec_[i].getResponse() > maxResponse) {
            maxResponse = outputLayer_.neuronVec_[i].getResponse();
            maxIndex = i;
        }
    }
    if (maxIndex == y) {
        return true;
    } else {
        return false;
    }
}

}