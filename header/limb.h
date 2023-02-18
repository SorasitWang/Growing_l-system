#pragma once
#ifndef LIMB_H
#define LIMB_H


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "object.h"
using namespace std;
class Limb : public Object
{
public:
	string type;
	glm::vec3 startPos;
	glm::vec3 endPos;
	int level;
	bool isCore;


	Limb() {};
	Limb(Object* parent,string type, glm::vec3 startPos, glm::vec3 endPos, int level) : Object(parent,nullptr) {
			this->type = type;
			this->endPos = endPos;
			this->startPos = startPos;
			this->level = level;
			this->isCore = true;
	}
	Limb(Object* parent, Object* prev,string type, glm::vec3 direction) :Object(parent,prev) {
		this->type = type;
		if (this->prev == nullptr)
			this->startPos = glm::vec3(0);
		else
			this->startPos = this->prev->endPos;
		this->endPos = this->startPos + direction;
		this->level = -1;
		this->isCore = true;
	}

	//static const vector<string> avaGeoObjects;

private:
};
#endif