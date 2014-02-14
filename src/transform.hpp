#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform{
public:
	shared_ptr<Transform> parent;
	vec3 origin;
	vec3 scale;
	quat orientation;
	vec3 position;
public:
	Transform():parent(nullptr),scale(vec3(1.f)),orientation(quat()),position(vec3(0.f)){}
	Transform(shared_ptr<Transform> t):parent(t),scale(vec3(1.f)),orientation(quat()),position(vec3(0.f)){}
	Transform(vec3 startpos) :parent(nullptr),scale(vec3(1.f)),origin(vec3(0.f)),orientation(quat()),position(startpos){}
public:
	void translate(vec3 pos){
		position += pos;
	}
	void rotate(float ammount,vec3 axis){
		if(ammount!=0) orientation=glm::rotate(orientation,ammount,axis);
	}
	mat4 getMatrix()const{
		return glm::translate(mat4(),position)
			 * mat4_cast(orientation)
			 * glm::scale(mat4(),scale)
			 * glm::translate(mat4(),origin)
			 * (parent?parent->getMatrix():mat4());
	}
};
