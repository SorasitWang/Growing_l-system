
#include "newLSystem.h"

BaseObject* NewLSystem::genHierachy2(int step, float alpha, float delta) {
    BaseObject* parent = new BaseObject(nullptr, nullptr, "", 15, 1);

    this->recurGenHierachy2(parent, this->axiom, step, alpha, delta, this->colors, this->rules);
    return parent;
}


void NewLSystem::recurGenHierachy2(BaseObject* parent, string txt, int it, float alpha, float delta, map<string,glm::vec3> colors, map<string, string> rules) {
    if (it < 0)return;
    it -= 1;
    string dirStack = "";
    vector<BaseObject*> prevStack;
    prevStack.push_back(nullptr);
    int bracketStack = 0;
    /* if (tmp != rules.end())
         cout << tmp->second << endl;*/
    for (char x_ : txt) {

        string x = std::string() + x_;
        if (x_ == 'F') {

        }
        else if (colors.find(x) != colors.end()) {

            BaseObject* newLimb = new BaseObject(parent, prevStack.back(), dirStack, colors.find(x)->second, alpha, delta);
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
            recurGenHierachy2(newBuld, rules.find(x)->second, it, alpha, delta, colors, rules);
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
 }
