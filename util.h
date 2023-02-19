#ifndef  UTIL_H
#define UTIL_H



#include <string>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <time.h>
using namespace std;

class Util {
public:
	
	static string printVec3(glm::vec3 in) {
		string interval = " ";
		return to_string(in.x) + interval + to_string(in.y) + interval + to_string(in.z);
	}

	static float randomInRange(float min,float max) {
		cout << (float)rand() << endl;
		float random = (float)rand() / RAND_MAX;
		return min + random ;
	}

    static pair<glm::vec3, float>findRotationParam(glm::vec3 destVec) {
        for (unsigned int i = 0; i < 3; i++) {
            if (destVec[i] == 0.0f)
                destVec[i] = 0.0000000000001;
        }
        destVec = glm::normalize(destVec);
        glm::vec3 originVec(0, 1, 0);
        glm::vec3 crossVec = glm::cross(originVec, destVec);
        cout << "=>" <<printVec3(originVec) << " " << printVec3(destVec) << " " << printVec3(crossVec) << endl;
        float normCross = crossVec.length();
        glm::vec3 axis = glm::normalize(crossVec);
        float angle = glm::degrees(glm::asin(normCross));
       /* if (destVec.z < 0) {
            angle += 180.0f;
            axis *= -1;
        }*/
        //swap(axis.x, axis.z);
       /* if (axis.z == 1.0f)
            swap(axis.x, axis.z);*/
        return pair<glm::vec3, float>(axis, glm::radians(angle));
    }
    
private :

};

#endif // ! UTIL_H