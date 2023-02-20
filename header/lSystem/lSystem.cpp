#include "lSystem.h"
#include "../util.h"
#include <random>

void LSystem::reset() {
    this->state = this->axiom;
}

void LSystem::step() {

    if (this->extra) {
      
        vector<string> ex = this->extract(this->state);

        unsigned int idx = 0;
        string newState = "";
        unsigned int i = 0;
       
        while (i < this->state.size()) {
            if (this->state[i] == '(') {
                if (this->rules.find(ex[idx]) != this->rules.end()) {
               
                    newState += this->rules.find(ex[idx])->second;
                }
                i += ex[idx].size() + 2;
                idx += 1;
                
            }
            else {
                newState += this->state[i];
                i += 1;
            }
        }
        this->state = newState;
    }
    else {
        string newState = "";
        for (int i = 0; i < this->state.size(); i++) {
            string s = &this->state[i];
            auto result = this->rules.find(s);
            if (result != this->rules.end())
                newState += result->second;
            else
                newState += s;
        }
    }
}

vector<Limb*> LSystem::genPos( float alpha_, float delta, glm::vec3 initDirection, map<char, string> colors) {
    //cout << alpha << " " << delta << endl;
    string str = this->state;
    vector<Limb*> allLimb;
    glm::vec3 prevPos(0, 0, 0);
    glm::vec3 direction = initDirection;
    string color;
    int f = 0, ff = 0, maxF = -1;
    float alpha = alpha_;
    Limb* prev = nullptr;
    vector<Limb*> leafStack, topStack;
    vector<int> fStat;
    vector<pairVec3>savedStates;
    for (char x : str) {
        //float alpha = rand()%2==0?1:-1 * (rand()%5+ 8.0f) / 10.0f * alpha_;
      
        if (x == 'F') {
            f += 1;
            glm::vec3 newPos = prevPos + direction;
            glm::vec3 _newPos = newPos;
            //cout << Util::printVec3(direction);4
            // Prevent Leaf has same position as Bark
            if (color == "g") {
                glm::vec3 tmpDirection(glm::rotate(glm::mat4(1.0f) , glm::radians(alpha), glm::vec3(1.0f, 0.0f, 1.0f)) * glm::vec4(direction, 1.0));
               // tmpDirection = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(tmpDirection, 1.0));
                _newPos = prevPos + tmpDirection;
            }
            if (allLimb.size() >= 1) {
                Limb* lastLimb = allLimb[allLimb.size() - 1];
                //if (lastLimb->startPos != prevPos && lastLimb->endPos != _newPos)
                    allLimb.push_back(new Limb(nullptr, color == "a" ? "leaf" : "bark", prevPos, _newPos, f));
            }
            else {
                allLimb.push_back(new Limb(nullptr, color == "a" ? "leaf" : "bark", prevPos, _newPos, f));
            }
            if (color != "g ") {
                if (f < ff && prev != nullptr)
                    prev->isCore = false;
                prev = allLimb.back();
                if (prev ==nullptr)
                    cout<< "Yes" << endl;
                ff = f;
            }
            else
                leafStack.push_back(allLimb.back());
            if (allLimb.back()->type == "bark")
                if (topStack.size() == 0)
                    topStack.push_back(allLimb.back());
                else
                    topStack[topStack.size() - 1] = allLimb.back();
            prevPos = newPos;
            alpha = alpha_;
           
        }
        else if (x == '+')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(direction, 1.0));
        else if (x == '-')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-alpha), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(direction, 1.0));
        else if (x == '&')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(direction, 1.0));
        else if (x == '^')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-alpha), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(direction, 1.0));
        else if (x == '<')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(direction, 1.0));
        else if (x == '>')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-alpha), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(direction, 1.0));
        else if (x == '*')
            direction *= delta;
        else if (x == '/')
            direction /= delta;
        else if (x == '|') {
            float rad180 = glm::radians(180.0f);
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), rad180, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(direction, 1.0));
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), rad180, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(direction, 1.0));
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), rad180, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(direction, 1.0));
        }
        else if (x == '.') {
            alpha /= 2.0f;
        }
        else if (x == '[') {
            savedStates.push_back(pairVec3(prevPos, direction));
            fStat.push_back(f);
            topStack.push_back(nullptr);
        }
        else if (x == ']') {
            pairVec3 tmp = savedStates.back();
            prevPos = tmp.first;
            direction = tmp.second;
            savedStates.pop_back();
            f = fStat.back();
            fStat.pop_back();
            //change top is topBark
            if (topStack.size() != 0 && topStack.back() != nullptr) {
                //cout << topStack.back()->type << endl;
                topStack.back()->type = "topBark";
                if (leafStack.size() >= 2) {
                    leafStack.back()->type = "topLeaf";
                    leafStack[leafStack.size() - 2]->type = "topLeaf";
                    leafStack.clear();
                }
            }
            if (topStack.size() != 0)
                topStack.pop_back();
        }
        else if (colors.find(x) != colors.end()) {
            color = (colors.find(x)->second);
        }
    }
   
    if (prev != nullptr)
        prev->isCore = false;

    return allLimb;
}

