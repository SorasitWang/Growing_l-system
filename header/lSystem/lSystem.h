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
#include "../object/object.h"
#include "../object/limb.h"
#include "../object/buld.h"
#include "../object/baseObject.h"
#include "../lSystem/baseLSystem.h"
using namespace std;
typedef std::pair < glm::vec3, glm::vec3 > pairVec3;
class LSystem : public BaseLSystem
{
public:
    string state,axiom;
    bool extra;

    LSystem() : BaseLSystem() {};
    ~LSystem() {};
    LSystem(string axiom, map<string, string> rules, bool extra=true) : BaseLSystem(axiom,rules){
        this->state = axiom;
        this->extra = extra;
    };
   
    void step();
    void reset();
    vector<Limb*> genPos(float alpha, float delta, glm::vec3 initDirection, map<char, string> colors);
   
private:

    };
#endif // !LSYSTEM_H
