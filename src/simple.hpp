#pragma once

#include "shader.hpp"
#include <istream>

class SimpleObject{
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
public:
	Transform transform;
	vec3   color;
private:
	unsigned short size;
public:
	void upload(vector<GLfloat> vertices, vector<GLushort> indices){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	}
	SimpleObject(vector<GLfloat>  vertices,
				 vector<GLushort> indices,
				 Transform trans,vec3 _color)
		:transform(trans),color(_color),size(indices.size()){
		upload(vertices,indices);
	}
	void draw() const{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES,size,GL_UNSIGNED_SHORT,(void*)0);
	}
};

class SimpleShader:public Shader{
	Uniform<vec3> color;
public:
	vector<SimpleObject> objects;
public:
	SimpleShader():
		Shader(make_shader(readFile(shader_path+"/simple.vert"),
						   readFile(shader_path+"/simple.frag"))),
			   color(Uniform<vec3>(program,"color") ){}
	void setCamera(Camera cam){
		view = cam.transform.getMatrix();
		projection = cam.getProjection();
	}
	void draw(){
		// rotate objects for demonstration
		objects.at(1).transform.rotate(0.1f,vec3(0,0,1));
		objects.at(2).transform.rotate(0.2f,vec3(0,0,1));
		objects.at(3).transform.rotate(0.4f,vec3(0,0,1));

		// draw objects
		for(const SimpleObject &o:objects){
			color=o.color;
			model=o.transform.getMatrix();
			o.draw();
		}
	}
};
