

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "tree.h"

LSystem Tree::genSystemForType(string type,int step,map<char,string>& colors) {
    map<string, string>rules;
    string axiom;
    if (type == "A") {
        axiom = "(0X)";
        rules = { { "0X", "bFF(1X)"}};
        LSystem::genRule(
            {
                {"1X", "1(g)F[&+(2X)](2b)[&-(2X)][^(2X)]"},
                {"1b" , "1(b)FF[&+(2X)](2b)FF[&-(2X)][^(2X)]"},
            },
            step,
            rules
            );
        colors = { {'g', "g"}, {'b' , "#654321"}};
    }
    else if (type == "B") {
        axiom = "(1X)";
        LSystem::genRule(
            {
                {"1X", "..+1(g)(2X)F[+(2X)F][-(2X)F]<2(g)FF[&(2X)F][^(2X)F]&2(g)F"},
                
            },
            step,
            rules
            );
        colors = { {'g', "b"}};
    }
    else if (type == "C") {
        axiom = "(1X)";
        rules = {
          {"1X", "..+(a)F[[++(1X)F]++(a)F][[--(1X)F]--(a)F][[&&(1X)F]&&(a)F][[^^(1X)F]^^(a)F]"},
             //{"1X", "..+(a)F[[++(1X)F]++(a)F]"},
            {"a","a"}
        };
        colors = { {'a', "b"} };
    }
   
    LSystem lSystem = LSystem::LSystem(
        axiom,
        rules,
        true
    );
    return lSystem;
}


NewLSystem Tree::genNewSystemForType(string type){
    string axiom;
    map<string, string> rules;
    map<string, glm::vec3> colors;
    if (type == "C") {
        axiom = "X";
        rules = {
         {"X", "..+aF[[++XF]++bF][[--XF]--bF][[>XF]>bF][[<XF]<bF]"},
             {"b","a"}
        };
        colors = {
            {"a", glm::vec3(1,0,0) },
            {"b", glm::vec3(0,1,0)}
        };
    }
    return NewLSystem(axiom,rules,colors);
}