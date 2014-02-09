#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void error_callback(int error, const char* description){
	cerr<<"GLFW error "<<error<<": "<<description<<endl;
}

void key_callback(GLFWwindow* window, int key,
							  int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

}

void scroll_callback(GLFWwindow* window,double x, double y){

}

void framebuffer_size_callback(GLFWwindow* window, int width,int height){
	glViewport(0, 0, width, height);
}

#pragma clang diagnostic pop
