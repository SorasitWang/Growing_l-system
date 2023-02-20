#ifndef NEWLSYSTEM_H
#define NEWLSYSTEM_H


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
class NewLSystem : public BaseLSystem
{
public:
    map<string,glm::vec3> colors;


    NewLSystem() {};
    ~NewLSystem() {};
    NewLSystem(string axiom, map<string, string> rules, map<string, glm::vec3> colors ) : BaseLSystem(axiom,rules){
        this->colors = colors;
    };


    BaseObject* genHierachy2(int step,float alpha,float delta);
    void recurGenHierachy2(BaseObject* parent, string txt, int it, float alpha_, float delta, map<string, glm::vec3> colors, map<string, string> rules);

private:

};
#endif // !NEWLSYSTEM_H
