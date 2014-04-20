#pragma once

#include "mesh.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

using namespace std;

// ref-counted handle to indexed vertex array buffers in video memory

class VertexArrayHandle{
	// allocates vertex buffer object in video memory
	GLuint make_vbo(vector<GLfloat> vertices){
		GLuint ret;
		glGenBuffers(1, &ret);
		glBindBuffer(GL_ARRAY_BUFFER, ret);
		glBufferData(GL_ARRAY_BUFFER,
					 vertices.size() * sizeof(GLfloat),
					 &vertices[0],
					 GL_STATIC_DRAW);
		return ret;
	}
	// allocates index buffer object in video memory
	GLuint make_ibo(vector<GLushort> indices){
		GLuint ret;
		glGenBuffers(1, &ret);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 indices.size() * sizeof(GLushort),
					 &indices[0], GL_STATIC_DRAW);
		return ret;
	}
	// allocate vertex array object in video memory
	// and binds it to the vbo and ibo
	GLuint make_vao(){
		GLuint ret;
		glGenVertexArrays(1, &ret);
		glBindVertexArray(ret);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		return ret;
	}
public:
	const unsigned short nrOfIndices;
	const GLuint vbo;
	const GLuint ibo;
	const GLuint vao;
	VertexArrayHandle(const Mesh mesh)
		:nrOfIndices(mesh.indices.size()),
		 vbo(make_vbo(mesh.vertices)),
		 ibo(make_ibo(mesh.indices)),
		 vao(make_vao()),
		 refcount(new unsigned(1)){
	}
	VertexArrayHandle(const VertexArrayHandle& other)
		:nrOfIndices(other.nrOfIndices),
		 vbo(other.vbo),
		 ibo(other.ibo),
		 vao(other.vao),
		 refcount(other.refcount){
		(*refcount)++;
	}
	VertexArrayHandle operator=(const VertexArrayHandle& other) = delete;

	~VertexArrayHandle(){
		(*refcount)--;
		if( (*refcount) == 0){
			glDeleteBuffers(1,&vbo);
			glDeleteBuffers(1,&ibo);
			glDeleteVertexArrays(1,&vao);
		}
	}
private:
	unsigned * const refcount;
};
