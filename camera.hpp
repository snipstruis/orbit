#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform{
public:
	shared_ptr<Transform> parent;
	vec3 scale;
	quat orientation;
	vec3 position;
public:
	Transform(shared_ptr<Transform> t):parent(t),scale(vec3(1.f)),orientation(quat()),position(vec3(0.f)){}
	Transform(vec3 startpos) :parent(nullptr),scale(vec3(1.f)),
		orientation(quat()),position(startpos){}
public:
	void translate(vec3 pos){
		position += pos;
	}
	void rotate(vec3 axis,float ammount){
		if(ammount!=0) orientation=glm::rotate(orientation,ammount,axis);
	}
	mat4 getMatrix()const{
		return (parent?parent->getMatrix():mat4())
			 * glm::scale(mat4(),scale)
			 * mat4_cast(orientation)
			 * glm::translate(mat4(),position);
	}
};

class Camera{
public:
	Camera(Transform transform_, float fov_y, float aspect, float near=0.1f, float far=1000.f)
		 :transform(transform_), vertical_fov(fov_y), aspect_ratio(aspect), near_clip(near), far_clip(far){}
public:
	Transform transform;
	mat4 getProjection(){
		return glm::perspective(vertical_fov,aspect_ratio,near_clip,far_clip);
	}
public:
	float vertical_fov;
	float aspect_ratio;
	float near_clip;
	float far_clip;
};
