#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using glm::dvec2;
using glm::ivec2;

dvec2 getCursor(GLFWwindow* window){
	dvec2 t;
	glfwGetCursorPos(window,&t.x,&t.y);
	return t;
}

void setCursor(GLFWwindow* window,dvec2 t){
	glfwSetCursorPos(window,t.x,t.y);
}

ivec2 getScreenSize(GLFWwindow* window){
	ivec2 t;
	glfwGetFramebufferSize(window,&t.x,&t.y);
	return t;
}
