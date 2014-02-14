#pragma once

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
