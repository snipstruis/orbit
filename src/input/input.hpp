#pragma once

#include "graphics/camera.hpp"
#include <glm/glm.hpp>

template<class T>
class Command{
public:
	Command(){}
	virtual ~Command(){}
	virtual void execute(T&)=0;
};

class MoveCameraCommand:public Command<Transform>{
	const vec3  translation;//(x,y,z)
	const vec3  rotation;   //(pan,tilt,roll)
public:
	MoveCameraCommand(vec3 trans, vec3 rot)
		:translation(trans),rotation(rot){}
	void execute(Transform& t){
		t.orientation = quat(rotation) * t.orientation;
		t.origin     -= translation    * t.orientation;
	}
};

class Input{
	GLFWwindow* window;
	uint16_t // keybindings
		cam_trans_up	 = GLFW_KEY_LEFT_SHIFT,
		cam_trans_down   = GLFW_KEY_LEFT_CONTROL,
		cam_trans_left   = GLFW_KEY_A,
		cam_trans_right  = GLFW_KEY_D,
		cam_trans_forward= GLFW_KEY_W,
		cam_trans_back   = GLFW_KEY_S,
		cam_rotate_left  = GLFW_KEY_LEFT,
		cam_rotate_right = GLFW_KEY_RIGHT,
		cam_rotate_up    = GLFW_KEY_UP,
		cam_rotate_down  = GLFW_KEY_DOWN,
		cam_rotate_cw    = GLFW_KEY_E,
		cam_rotate_ccw   = GLFW_KEY_Q;
public:
	Input(GLFWwindow* win):window(win){
		setCursor(window,dvec2(getScreenSize(window)/2));
	}
	void poll(){
		glfwPollEvents();
	}
	bool shouldClose(){
		return glfwWindowShouldClose(window);
	}
	MoveCameraCommand moveCameraInput(){
		//translations (x,y,z)
		vec3 position = vec3(0,0,0);
		if(glfwGetKey(window,cam_trans_right))  position += vec3( 0.5, 0, 0);
		if(glfwGetKey(window,cam_trans_left))   position += vec3(-0.5, 0, 0);
		if(glfwGetKey(window,cam_trans_up))     position += vec3( 0, 0.5, 0);
		if(glfwGetKey(window,cam_trans_down))   position += vec3( 0,-0.5, 0);
		if(glfwGetKey(window,cam_trans_back))	position += vec3( 0, 0, 0.5);
		if(glfwGetKey(window,cam_trans_forward))position += vec3( 0, 0,-0.5);

		//rotations (pan,tilt,roll)
		dvec2 cursor = dvec2(getCursor(window)) - dvec2(getScreenSize(window)/2);
		cursor.x /= 100;
		cursor.y /= 100;

		vec3 orientation = vec3(cursor.y,cursor.x,0);
		if(glfwGetKey(window,cam_rotate_down)) orientation += vec3( 0.1, 0, 0);
		if(glfwGetKey(window,cam_rotate_up))   orientation += vec3(-0.1, 0, 0);
		if(glfwGetKey(window,cam_rotate_right))orientation += vec3( 0, 0.1, 0);
		if(glfwGetKey(window,cam_rotate_left)) orientation += vec3( 0,-0.1, 0);
		if(glfwGetKey(window,cam_rotate_cw))   orientation += vec3( 0, 0, 0.1);
		if(glfwGetKey(window,cam_rotate_ccw))  orientation += vec3( 0, 0,-0.1);
		setCursor(window,dvec2(getScreenSize(window)/2));

		return MoveCameraCommand(position,orientation);
	}
};
