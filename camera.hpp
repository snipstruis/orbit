#pragma once


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
