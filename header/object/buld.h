#pragma once
#ifndef BULD_H
#define BULD_H


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "object.h"
using namespace std; 

class Buld : public Object{
public :
	vector<Object*> childBuld;
	vector<Object*> childLimb;
	glm::vec3 position;


	Buld() {};
	Buld(Object* parent, Object* prev) : Object(parent,prev){
		if (prev == nullptr)
			this->position = glm::vec3(0.0f);
		else
			this->position = prev->endPos;

	};

};

#endif