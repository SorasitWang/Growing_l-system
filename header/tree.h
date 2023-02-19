#pragma once
#ifndef TREE_H
#define TREE_H


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "lSystem.h"

using namespace std;
class Tree 
{
public :
    LSystem lSystem;

    Tree() {};
    Tree(string type, unsigned int step,float alpha=17.5f,float delta=8.0f,glm::vec3 initDirection= glm::vec3(.0f, 1.0f, .0f)) {
        this->step = step;
        this->type = type;
        this->lSystem = Tree::genSystemForType(this->type,this->step,this->colors);
        this->alpha = alpha;
        this->delta = delta;
        this->initDirection = initDirection;
    };
	
    void runAll() {
        for (unsigned int i = 0; i < this->step; i++) {
            this->lSystem.step();
            /* allPos = plotA3d(
                 system.state,
                 step,
                 17.5,
                 8,
                 np.array([0, 0, 1.0]),
                 { "g": "g", "b" : "#654321" },
                 gen = False,
                 )*/
        }
    }
    
    void runOneStep() {
        this->lSystem.step();
    }

    vector<Limb*> draw() {
        return this->lSystem.genPos(this->alpha,this->delta,this->initDirection,this->colors);
    }

    static LSystem genSystemForType(string type, int step, map<char, string>& colors) ;
    
private :
    
    unsigned int step;
    string type;
    float delta, alpha;
    glm::vec3 initDirection;
    map<char, string> colors;
};


#endif