#ifndef LSYSTEM_H
#define LSYSTEM_H


#include <iostream>
#include <string>
#pragma once
#include <map>
#include <vector>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <ctype.h>
#include "object.h"
#include "limb.h"
#include "buld.h"
#include "baseObject.h"
#include "baseLSystem.h"
using namespace std;
typedef std::pair < glm::vec3, glm::vec3 > pairVec3;
class LSystem : public BaseLSystem
{
public:
    string state,axiom;
    map<string, string> rules;
    bool extra;

    LSystem() : BaseLSystem() {};
    LSystem(string axiom, map<string, string> rules, bool extra) : BaseLSystem(axiom,rules){
        this->state = axiom;
        this->extra = extra;
    };
    void reset() {
        this->state = this->axiom;
    }

    
    void step();

    
    
    vector<Limb*> genPos(float alpha, float delta, glm::vec3 initDirection, map<char, string> colors);
    Buld genHierachy(int step, string state);
    BaseObject* genHierachy1(int step);
    BaseObject* genObjects(float alpha_, float delta, glm::vec3 initDirection, map<char, string> colors);
    void recurGenHierachy(BaseObject* parent, int it);


    BaseObject* genHierachy2();
    void recurGenHierachy2(BaseObject* parent,string txt, int it,float alpha_,float delta,map<char,string> colors,map<string,string> rules);

private:

    };
#endif // !LSYSTEM_H
