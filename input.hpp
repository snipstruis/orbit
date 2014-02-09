#pragma once
#include "camera.hpp"

template<class T>
class Command{
public:
	Command(){}
	virtual ~Command(){}
	virtual void execute(T&)=0;
};

class MoveCameraCommand:public Command<Camera>{
	vec3  translation;
	vec3  rotationAxis;
	float rotationAmmount;
public:
	MoveCameraCommand(vec3 trans, vec3 axis, float ammount)
		:translation(trans),rotationAxis(axis),rotationAmmount(ammount){
	}
	void execute(Camera& cam){
		cam.transform.translate(translation);
		cam.transform.rotate(rotationAxis,rotationAmmount);
	}
};

class Input{
	GLFWwindow* window;
	uint16_t // keybindings
		cam_up	   = GLFW_KEY_LEFT_SHIFT,
		cam_down   = GLFW_KEY_LEFT_CONTROL,
		cam_left   = GLFW_KEY_A,
		cam_right  = GLFW_KEY_D,
		cam_forward= GLFW_KEY_W,
		cam_back   = GLFW_KEY_S;
public:
	Input(GLFWwindow* win):window(win){}
	void poll(){
		glfwPollEvents();
	}
	bool shouldClose(){
		return glfwWindowShouldClose(window);
	}
	MoveCameraCommand moveCameraInput(){
		vec3 position = vec3(0,0,0);
		if(glfwGetKey(window,cam_up))     position += vec3( 0, 0,-1);
		if(glfwGetKey(window,cam_down))	  position += vec3( 0, 0, 1);
		if(glfwGetKey(window,cam_left))	  position += vec3( 1, 0, 0);
		if(glfwGetKey(window,cam_right))  position += vec3(-1, 0, 0);
		if(glfwGetKey(window,cam_forward))position += vec3( 0,-1, 0);
		if(glfwGetKey(window,cam_back))   position += vec3( 0, 1, 0);
		return MoveCameraCommand(position,vec3(0.f),0.f);
	}
};
