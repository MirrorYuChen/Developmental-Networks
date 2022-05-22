/*
 * @Description: 
 * @Date: 2022-05-21 14:11:28
 * @Author: chenjingyu
 * @LastEditTime: 2022-05-21 15:03:49
 * @FilePath: \MILN\source\Neuron.h
 */
#pragma once
#include <vector>
namespace mirror {
class Neuron;

enum ConnectType {
    BOTTOM_UP,
    TOP_DOWN,
    LATERAL_INHIBIT,
    LATERAL_EXCIT
};

struct Connect {
    Connect(double w, Neuron *f=NULL) {
        weight = w;
        from = f;
    }
    double weight;
    Neuron *from;
    double input;
};

class Neuron {
public:
    Neuron(double alpha=0, double beta=0, double gamma=0, double delta=0);

    void setPosition(int x=0, int y=0, int z=0);
    void AddConnect(Connect c, int type);

    inline double getResponse() const {
        return response_;
    }

    void Update();
    void Fire();


private:
    double Amnesic(int t);
    double ComputeResponse(const std::vector<Connect>& c);
    double Active(double x);

public:
    double alpha_, beta_, gamma_, delta_;
    int age_;
    double response_, w1_, w2_;
    double positionX_, positionY_, positionZ_;
    std::vector<Connect> wb_;
    std::vector<Connect> wt_;

};



} // namespace mirror
