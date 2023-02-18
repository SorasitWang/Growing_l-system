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
using namespace std;
typedef std::pair < glm::vec3, glm::vec3 > pairVec3;
class LSystem 
{
public:
    string state,axiom;
    map<string, string> rules;
    bool extra;

    LSystem() {};
    LSystem(string axiom, map<string, string> rules, bool extra) {
        this->state = axiom;
        this->rules = rules;
        this->extra = extra;
    };
    void reset() {
        this->state = this->axiom;
    }

    static vector<string> extract(string state) {
        /*
        input : "1(g)F/[+(2B)]-(2B)"
        output : [g, 2B, 2B]
        
        Beware!inside() must be 1 number(optional) + 1 character only
        */
        int start = 0;
        vector<string> re;
        while (state.find("(", start) != string::npos) {
            start = state.find("(", start);
            int end = state.find(")", start);
            start += 1;
            re.push_back(state.substr(start, end-start));

            //print(start, end, state[start + 1:end])
        }
        return re;
        
    }


    void step();

    static void genRule(map<string, string> _initRules, int num, map<string, string> &rules) {

        /*
        for each rule, increment every numeric
            numeric value must in range(1, 10)-- > 1 digit
        */
        vector<map<string, string>> initRules{ _initRules };
        for (unsigned int i = 0; i < num; i++) {
            vector<map<string, string>> newInitRules;
            for (map<string, string> initRule : initRules) {
                for (auto pair : initRule) {
                    string key = pair.first;
                    string value = pair.second;
                    rules[key] = value;
                    string rule = "";
                    for (char c : value) {
                        if (isdigit(c))
                            rule += to_string(stoi(&c) + 1);
                        else
                            rule += c;
                    }
                    string new_key = to_string(stoi(&key[0]) + 1) +key[1];
                    newInitRules.push_back({{new_key,rule}});
                }
               
            }
            initRules = newInitRules;
        }
        //return rules;
       
    }
    
    vector<Limb*> genPos(float alpha, float delta, glm::vec3 initDirection, map<char, string> colors);
    Buld genHierachy(int step, string state);
    BaseObject* genHierachy1(int step);
    void recurGenHierachy(BaseObject* parent, int it);

private:

    };
#endif // !LSYSTEM_H
