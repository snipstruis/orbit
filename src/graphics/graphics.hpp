#pragma once

#include "input/input.hpp"
#include "simple.hpp"

class Graphics{
	GLFWwindow*   window;
	SimpleShader  simple;
	Camera        camera;
	Input		  input;
public:
	Graphics(GLFWwindow* w,SimpleShader s,Camera c,Input in)
		:window(w),simple(s),camera(c),input(in){}
	void fixedTick(){
		input.moveCameraInput().execute(camera.transform);
		simple.setCamera(camera);
	}
	void freeStep(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		simple.draw();
		glfwSwapBuffers(window);
	}
};
