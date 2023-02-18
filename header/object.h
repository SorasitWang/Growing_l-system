#pragma once
#ifndef OBJECT_H
#define OBJECT_H


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "model.h"
using namespace std;
class Object
{
public:
	Object* parent;
	Object* prev;
	glm::vec3 endPos;


	Object() {};
	Object(Object* parent, Object* prev) {
		this->parent = parent;
		this->prev = prev;
		this->endPos = glm::vec3(0.0f);
		//this->endPos = endPos;
	}

private:
};
#endif