#pragma once
#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "model.h"
#include "../util.h"
#include <time.h>
using namespace std;

class BaseObject
{
public:
	BaseObject* parent;
	BaseObject* prev;
	glm::vec3 startPos,direction,endPos;
	glm::mat4 dirMatrix;
	bool isBuld;
	string type,dirText;
	int level;
	bool isCore,alreadyAdded;
	vector<BaseObject*> childBuld;
	vector<BaseObject*> childLimb;
	float size,normRand;
	Model* model;

	BaseObject() {};
	BaseObject(BaseObject* parent, BaseObject* prev, string dirText,string type,float alpha,float delta) {

		this->parent = parent;
		this->prev = prev;
		this->isBuld = false;
		this->dirText = dirText;
		if (parent == nullptr) {
			this->direction = BaseObject::interpretDirection(dirText, alpha, delta, this->parent);
			this->dirMatrix = BaseObject::interpretDirection2(dirText, alpha, delta, this->parent);
		}
		else {
			this->direction = BaseObject::interpretDirection(dirText, alpha, delta, this->parent, this->parent->direction);
			this->dirMatrix = BaseObject::interpretDirection2(dirText, alpha, delta, this->parent, this->parent->dirMatrix);
		}
		//this->direction /= 2.0f;
		this->calPos();
		this->normRand = (float)rand() / RAND_MAX;
		this->level = this->parent == nullptr ? normRand*3 : this->parent->level + normRand*3;
		//this->endPos = this->startPos + direction;
		this->type = type;
		this->alreadyAdded = false;
		this->size =  1.0f + normRand*3;
		//this->model = new Model("./res/cylinder_8v.obj");
		//cout << this->size << endl;
		//this->endPos = endPos;
	}

	BaseObject(BaseObject* parent, BaseObject* prev,string dirText,float alpha, float delta) {
		//srand(time(0));
		this->parent = parent;
		this->prev = prev;
		this->isBuld = true;
		this->dirText = dirText;
		this->calPos();
		this->normRand = (float)rand() / RAND_MAX;
		this->level = this->parent == nullptr ? 1+normRand*3 : this->parent->level + 1+normRand*3;
		if (parent == nullptr) {
			this->direction = BaseObject::interpretDirection(dirText, alpha, delta, this->parent);
			this->dirMatrix = BaseObject::interpretDirection2(dirText, alpha, delta, this->parent);
		}
		else {
			this->direction = BaseObject::interpretDirection(dirText, alpha, delta, this->parent, this->parent->direction);
			this->dirMatrix = BaseObject::interpretDirection2(dirText, alpha, delta, this->parent, this->parent->dirMatrix);
		}
		this->size = 1.0f + normRand * 3.0f;
		/*if (parent != nullptr)
		cout << this->direction.x << " " << this->direction.y << " " << this->direction.z << " | "
			<< this->parent->direction.x << " " << this->parent->direction.y << " " << this->parent->direction.z << endl;
		*/
		this->alreadyAdded = false;
	
	}

	void calPos(float scale=1.0f) {
		if (this->isBuld) {
			if (this->prev == nullptr) {
				if (this->parent == nullptr)
					this->endPos = glm::vec3(0.0f);
				else
					this->endPos = this->parent->endPos;
			}
			else {
				this->endPos = this->prev->endPos;
			}
		}
		else {
			if (this->prev == nullptr) {
				if (this->parent == nullptr)
					this->startPos = glm::vec3(0.0f);
				else 
					if (this->parent->prev == nullptr)
						this->startPos = this->parent->endPos;
					else 
						this->startPos = this->parent->prev->endPos;
			}
			else {
				this->startPos = this->prev->endPos;
			}
			//cout << Util::printVec3(this->startPos) << endl;
			this->endPos = this->startPos + this->direction*scale;
			glm::vec3 center = (this->startPos + this->endPos) / 2.0f;
			//pair<glm::vec3, float> rotateParams = this->findRotationParam(this->direction);
			
		
			//this->dirMatrix = glm::mat4(1.0f);
			
			/*this->dirMatrix = glm::scale(this->dirMatrix, glm::vec3(0.1, 0.1f, 1.0))*/
			
			//otateParams.first = glm::vec3(0,0, -1);
			/*this->dirMatrix = glm::translate(this->dirMatrix, center);
			this->dirMatrix = glm::rotate(this->dirMatrix, rotateParams.second, rotateParams.first);
			this->dirMatrix = glm::scale(this->dirMatrix, glm::vec3(scale, 1, 1));

			this->dirMatrix = glm::rotate(this->dirMatrix, glm::radians(90.0f), glm::vec3(0, 0,1));
			this->dirMatrix = glm::scale(this->dirMatrix, glm::vec3(0.1, 1.0f, .1f));*/

			//cout << Util::printVec3(this->direction) << endl;
			////cout << Util::printVec3(glm::cross(glm::vec3(1,0,0), this->direction)) << endl;
			//cout << Util::printVec3(rotateParams.first) << " " << rotateParams.second << endl;
			//cout << endl;

		}
	}
	static pair<glm::vec3, float>findRotationParam(glm::vec3 destVec) {
		for (unsigned int i = 0; i < 3; i++) {
			/*if (abs(destVec[i]) <= 1e-6) {
				cout << i << endl;
				destVec[i] = 0.01;
			}*/
		}
		destVec = glm::normalize(destVec);
		glm::vec3 originVec = glm::vec3(1, 0, 0);
		if (abs(destVec[2]) <= 1e-6) {
			cout << "zz" << endl;
			originVec = glm::vec3(0, 0, 1);
		}
		glm::vec3 crossVec = glm::cross(originVec, destVec);
		cout << "=>" << Util::printVec3(originVec) << " " << Util::printVec3(destVec) << " " << Util::printVec3(crossVec) << endl;
		float normCross = crossVec.length();
		glm::vec3 axis = glm::normalize(crossVec);
		float angle = glm::degrees(glm::asin(glm::clamp(normCross, -1.0f, 1.0f)));
		 if (destVec.x < 0) {
			 angle += 180.0f;
			 axis *= -1;
		 }
		 if (abs(destVec[2]) <= 1e-6) {
			 angle += 180.0f;
		 }
		 //swap(axis.x, axis.y);
		/* if (axis.z == 1.0f)
			 swap(axis.x, axis.z);*/
		return pair<glm::vec3, float>(axis, glm::radians(angle));
	}

	void genPos(vector<float>& allPos,float scale,vector<glm::mat4>& allMatrix) {
		this->calPos(scale);
		
		if (this->alreadyAdded)
			return;
		/*if (scale >= 1.0f) {
			this->alreadyAdded = true;
			cout << this->alreadyAdded << this->startPos.x << " " << this->startPos.y << " " << this->startPos.z << endl;
		}*/
			allPos.push_back(this->startPos.x );
		allPos.push_back(this->startPos.y );
		allPos.push_back(this->startPos.z );

		allPos.push_back(this->endPos.x);
		allPos.push_back(this->endPos.y);
		allPos.push_back(this->endPos.z);

		if (this->type == "bark") {
			allPos.push_back(0.75f + this->normRand /4);
			allPos.push_back(0.0f);
			allPos.push_back(0.0f);
		}
		else   if (this->type == "topBark") {
			allPos.push_back(0.0f);
			allPos.push_back(0.75f + this->normRand /4);
			allPos.push_back(0.0f);
		}
		glm::mat4 mat(1.0f);
		

		mat = glm::translate(mat, (this->startPos + this->endPos) / 2.0f);
		mat *= this->dirMatrix ;
		mat = glm::scale(mat, glm::vec3(1, scale , 1));
		mat = glm::scale(mat, glm::vec3(0.1f, 1.0f, 0.1f));
		allMatrix.push_back(mat);
	}


	void parse(vector<float>& allPos, float step,vector<glm::mat4>& allMatrix) {
		if (step < this->level)
			return;
		float scale = min(this->size,step - this->level);
		//cout << this->size << " " << step << " " << this->level << endl;
		
		for (BaseObject* limb : this->childLimb) {
			
				limb->genPos(allPos, scale, allMatrix);
		}
		for (BaseObject* buld : this->childBuld) {
			buld->parse(allPos,step, allMatrix);
		}
	}


	static glm::vec3 interpretDirection(string str, float alpha,float delta,BaseObject* parent,glm::vec3 initDir = glm::vec3(0,1,0)) {
		glm::vec3 direction = initDir;
		for (auto x : str) {
			 if (x == '+')
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
		}
		/*if (parent != nullptr)
		cout << str << " " << parent->endPos.x << " " << parent->endPos.y << " " << parent->endPos.z << " | "
			<< initDir.x << " " << initDir.y << " " << initDir.z << " | "<<
			direction.x << " " << direction.y << " " << direction.z << " " << endl;*/
		return direction;
	}


	static glm::mat4 interpretDirection2(string str, float alpha, float delta, BaseObject* parent, glm::mat4 initDir = glm::mat4(1.0f)) {
		glm::mat4 direction = initDir;
		for (auto x : str) {
			if (x == '+')
				direction = glm::rotate(direction, glm::radians(alpha), glm::vec3(1.0f, 0.0f, 0.0f));
			else if (x == '-')
				direction = glm::rotate(direction, glm::radians(-alpha), glm::vec3(1.0f, 0.0f, 0.0f));
			else if (x == '&')
				direction = glm::rotate(direction, glm::radians(alpha), glm::vec3(0.0f, 1.0f, 0.0f));
			else if (x == '^')
				direction = glm::rotate(direction, glm::radians(-alpha), glm::vec3(0.0f, 1.0f, 0.0f));
			else if (x == '<')
				direction = glm::rotate(direction, glm::radians(alpha), glm::vec3(0.0f, 0.0f, 1.0f));
			else if (x == '>')
				direction = glm::rotate(direction, glm::radians(-alpha), glm::vec3(0.0f, 0.0f, 1.0f));
			else if (x == '*')
				direction *= delta;
			else if (x == '/')
				direction /= delta;
			else if (x == '|') {
				float rad180 = glm::radians(180.0f);
				/*direction = glm::vec3(glm::rotate(glm::mat4(1.0f), rad180, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(direction, 1.0));
				direction = glm::vec3(glm::rotate(glm::mat4(1.0f), rad180, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(direction, 1.0));
				direction = glm::vec3(glm::rotate(glm::mat4(1.0f), rad180, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(direction, 1.0));*/
			}
			else if (x == '.') {
				alpha /= 2.0f;
			}
		}
		/*if (parent != nullptr)
		cout << str << " " << parent->endPos.x << " " << parent->endPos.y << " " << parent->endPos.z << " | "
			<< initDir.x << " " << initDir.y << " " << initDir.z << " | "<<
			direction.x << " " << direction.y << " " << direction.z << " " << endl;*/
		return direction;
	}
private:
};
#endif