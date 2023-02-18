

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "header/tree.h"

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
          //{"1X", "..+1(a)F[[(1X)F](a)F][[+&>(1X)F]+&>(a)F][[<-^(1X)F]<-^(a)F]"},
         //{"X", "..+(a)F[[(X)F](a)F]"},
          //{"1X", "..+(a)F[[++(1X)F]++(a)F][[--(1X)F]--(a)F][[&&(1X)F]&&(a)F][[^^(1X)F]^^(a)F]"},
             {"1X", "..+(a)F[[++(1X)F]++(a)F]"},
            {"a","a"}
        };

     /*   LSystem::genRule(
            {
                {"1X", "1(b)F[&+(2b)](2b)"},
                {"1b" , "1(b)[&+(2X)](2b)FF[&-(2X)][^(2X)]F"},
            },
            step,
            rules
            );*/
        colors = { {'a', "b"} };
    }
    for (auto pair : rules)
	    cout << pair.first << " : " <<pair.second << endl;
    LSystem lSystem = LSystem::LSystem(
        axiom,
        rules,
        true
    );
    return lSystem;
}