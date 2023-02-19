#include "lSystem.h"
#include "../util.h"
#include <random>
void LSystem::step() {

    if (this->extra) {
        vector<string> ex = this->extract(this->state);
        unsigned int idx = 0;
        string newState = "";
        unsigned int i = 0;
        //for (auto s : ex)
        //    cout << s << " ";
        while (i < this->state.size()) {
            //cout << this->state[i] << endl;
            if (this->state[i] == '(') {
                if (this->rules.find(ex[idx]) != this->rules.end()) {
                    //cout << "found " << this->rules.find(ex[idx])->second << endl;
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
        cout << "newState " << newState << endl;
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
    cout << this->state << endl;
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
            //cout << "genPos" << prevPos.x << " " << prevPos.y << " " << prevPos.z << endl;
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
       //else if (x == 'X') {
       //     //allPos.push_back(new Object(color == "a" ? "leaf" : "bark", prevPos, _newPos, f));
       //     color = "X";
       // }

    }
   
    if (prev != nullptr)
        prev->isCore = false;

    return allLimb;
}




/*
Buld LSystem::genHierachy(int step,string state) {
    float alpha_,alpha, delta;
    glm::vec3 initDirection, direction = initDirection;
    map<char, string> colors;
    string start = this->axiom;
    Buld* parent = nullptr;
    vector<string> ex = this->extract(start);
    unsigned int i = 0,idx=0;
    int f = 0, ff = 0, maxF = -1;
    Limb* prev = nullptr;
    vector<Limb*> leafStack, topStack;
    vector<int> fStat;
    vector<Limb*> savedPrev = { prev };
    vector<Buld*> savedParent = { parent };
    glm::vec3 prevPos(0, 0, 0);
    string color;
    while (i < state.size()) {
        if (state[i] == '(') {
            if (this->rules.find(ex[idx]) != this->rules.end()) {
                f += 1;
                Buld* newBuld = new Buld(parent, prev);
                parent->childBuld.push_back(newBuld);
                savedParent.push_back(newBuld);
                parent = newBuld;
            }
        }
        else if (state[i] == 'a') {
            Limb* newLimb = new Limb(parent,prev, "bark", direction);
            parent->childLimb.push_back(newLimb);
            savedPrev.push_back(newLimb);
            prev = newLimb;
        }
        else if (state[i] == '+')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(direction, 1.0));
        else if (state[i] == '-')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-alpha), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(direction, 1.0));
        else if (state[i] == '&')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(direction, 1.0));
        else if (state[i] == '^')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-alpha), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(direction, 1.0));
        else if (state[i] == '<')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(direction, 1.0));
        else if (state[i] == '>')
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-alpha), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(direction, 1.0));
        else if (state[i] == '*')
            direction *= delta;
        else if (state[i] == '/')
            direction /= delta;
        else if (state[i] == '|') {
            float rad180 = glm::radians(180.0f);
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), rad180, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(direction, 1.0));
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), rad180, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(direction, 1.0));
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), rad180, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(direction, 1.0));
        }
        else if (state[i] == '.') {
            alpha /= 2.0f;
        }
        else if (state[i] == '[') {
            savedPrev.push_back(prev);
        }
        else if (state[i] == ']') {
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
        else if (colors.find(state[i]) != colors.end()) {
            color = (colors.find(state[i])->second);
        }
    }
    if (this->extra) {
        vector<string> ex = this->extract(this->state);
        unsigned int idx = 0;
        string newState = "";
        unsigned int i = 0;
        //for (auto s : ex)
        //    cout << s << " ";
        while (i < this->state.size()) {
            //cout << this->state[i] << endl;
            if (this->state[i] == '(') {
                if (this->rules.find(ex[idx]) != this->rules.end()) {
                    //cout << "found " << this->rules.find(ex[idx])->second << endl;
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
        //cout << "newState " << newState << endl;
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
    return parent;
    
}*/


BaseObject* LSystem::genHierachy1(int step) {
    BaseObject* parent = new BaseObject(nullptr,nullptr, "",15, 1);
    
    this->recurGenHierachy(parent,step);

    //for (unsigned int i = 0; i < 8; i++) {
    //    BaseObject* limb11 = new BaseObject(newParent, nullptr, glm::vec3(0.0f, 0.8f, 0.15f), "bark");
    //    newParent->childLimb.push_back(limb11);

    //    BaseObject* limb12 = new  BaseObject(newParent, limb11, glm::vec3(0.0f, 0.9, -0.1f), "topBark");
    //    newParent->childLimb.push_back(limb12);

    //    BaseObject* buld11 = new  BaseObject(newParent, limb11);
    //    newParent->childBuld.push_back(buld11);

    //    newParent = buld11;
    //    // recur

    //    BaseObject* limb22 = new  BaseObject(newParent, limb11, glm::vec3(0.0f, 0.9, -0.1f), "topBark");
    //    newParent->childLimb.push_back(limb12);

    //    BaseObject* buld12 = new  BaseObject(newParent, limb22);
    //    newParent->childBuld.push_back(buld11);
    //    // recur
    //}

    
    return parent;
}


void LSystem::recurGenHierachy(BaseObject* parent, int it) {
    if (it < 0)return;
    it -= 1;
    
    BaseObject* limb11 = new BaseObject(parent, nullptr, "..+", "bark", 15, 1);
    parent->childLimb.push_back(limb11);

    BaseObject* limb12 = new  BaseObject(parent, limb11, "++", "topBark", 15, 1);
    parent->childLimb.push_back(limb12);
    BaseObject* buld1 = new  BaseObject(parent, limb11, "++", 15, 1);
    parent->childBuld.push_back(buld1);
    recurGenHierachy(buld1, it);
    return;
    BaseObject* limb22 = new  BaseObject(parent, limb11, "--", "topBark", 12.5, 1);
    parent->childLimb.push_back(limb22);
    BaseObject* buld2 = new  BaseObject(parent, limb11, "--", 15, 1);
    parent->childBuld.push_back(buld2);
    recurGenHierachy(buld2, it);

    BaseObject* limb32 = new  BaseObject(parent, limb11, ".<", "topBark", 15, 1);
    parent->childLimb.push_back(limb32);
    BaseObject* buld3 = new  BaseObject(parent, limb11, ".<", 15, 1);
    parent->childBuld.push_back(buld3);
    recurGenHierachy(buld3, it);

    BaseObject* limb42 = new  BaseObject(parent, limb11, ".>", "topBark", 18, 1);
    parent->childLimb.push_back(limb42);
    BaseObject* buld4 = new  BaseObject(parent, limb11, ".>", 15, 1);
    parent->childBuld.push_back(buld4);
    recurGenHierachy(buld4, it);
}



BaseObject* LSystem::genHierachy2() {
    map<string, string> rules = {
        {"X", "..+aF[[++XF]++bF][[--XF]--bF][[.>XF].>bF][[.<XF].<bF]"},
            {"b","a"}
    };
    map<string, string> types = {
        {"a" ,"bark" },
        {"b","topBark"}
    };
    BaseObject* parent = new BaseObject(nullptr, nullptr, "", 15, 1);

    this->recurGenHierachy2(parent,this->axiom,7,15,1, types,rules);
    return parent;
}


void LSystem::recurGenHierachy2(BaseObject* parent,string txt,int it,float alpha,float delta, map<string, string> types, map<string, string> rules) {
    if (it < 0)return;
    it -= 1;
    string dirStack = "";
    vector<BaseObject*> prevStack;
    prevStack.push_back(nullptr);
    int bracketStack = 0;
    auto tmp = rules.find("X");
   /* if (tmp != rules.end())
        cout << tmp->second << endl;*/
    for (char x_ : txt) {
       
        string x = std::string() + x_;
        if (x_ == 'F') {

        }
        else if (types.find(x) != types.end()) {

            BaseObject* newLimb = new BaseObject(parent, prevStack.back(), dirStack,types.find(x)->second, alpha, delta);
            parent->childLimb.push_back(newLimb);
            if (prevStack.size() <= bracketStack)
                prevStack.push_back(newLimb);
            else {
                prevStack.pop_back();
                prevStack.push_back(newLimb);
            }
            dirStack = "";
        }
        else if (rules.find(x) != rules.end()) {
        //else if (rules.find(x) != rules.end()) {
            BaseObject* newBuld = new BaseObject(parent, prevStack.back(), dirStack, alpha, delta);
            parent->childBuld.push_back(newBuld);
            recurGenHierachy2(newBuld, rules.find(x)->second, it, alpha, delta, types, rules);
            dirStack = "";
        }
        else if (x_ == '[') {
            bracketStack += 1;
        }
        else if (x_ == ']') {
            if (prevStack.size() > bracketStack) {
                prevStack.pop_back();
            }
            bracketStack -= 1;
        }
        else {
            dirStack = dirStack + x_;
         
        }
    }
    //cout << it << endl;

}
