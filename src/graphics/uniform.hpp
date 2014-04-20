#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
using glm::mat4;
using glm::vec3;

static void uniform_set(GLint loc,mat4  data){
	float array[16] = {
		data[0][0],data[0][1],data[0][2],data[0][3],
		data[1][0],data[1][1],data[1][2],data[1][3],
		data[2][0],data[2][1],data[2][2],data[2][3],
		data[3][0],data[3][1],data[3][2],data[3][3]
	};
	glUniformMatrix4fv(loc,1,false,array);
}
static void uniform_set(GLint loc,vec3  data){
	glUniform3f(loc,data.x,data.y,data.z);
}

template<typename T>
class Uniform{
	GLint loc;
	T data;
public:
	Uniform(GLint prog, string name):
		loc(glGetUniformLocation(prog,name.c_str())){}
	T get(){return data;}
	void set(T t){
		data = t;
		uniform_set(loc,t);
	}
	void operator=(T t){
		data = t;
		uniform_set(loc,t);
	}
};
