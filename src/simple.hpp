#pragma once

#include "shader.hpp"
#include "json.hpp"
#include <istream>
#include <vector>

struct Mesh{
	Mesh(string meshfile){
		cout<<"loading mesh: "<<meshfile<<endl;

		Json::Reader reader;
		Json::Value  mesh;

		if(!reader.parse(readFile(meshfile),mesh)) throw BadJson(reader);

		vertices = loadFloatArray(mesh,"vertices");
		indices  = loadU16Array(mesh,"indices");
	}
	vector<GLfloat>  vertices;
	vector<GLushort> indices;
};

class VertexArray{
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
	GLuint make_ibo(vector<GLushort> indices){
		GLuint ret;
		glGenBuffers(1, &ret);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 indices.size() * sizeof(GLushort),
					 &indices[0], GL_STATIC_DRAW);
		return ret;
	}
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
	VertexArray(Mesh mesh)
		:nrOfIndices(mesh.indices.size()),
		 vbo(make_vbo(mesh.vertices)),
		 ibo(make_ibo(mesh.indices)),
		 vao(make_vao()){}
};

class SimpleObject{
	VertexArray vertexArray;
public:
	Transform transform;
	vec3   color;
public:
	SimpleObject(Json::Value object)
		:transform(object["transform"]),
		 color(loadVec3(object,"color")),
		 vertexArray(mesh_path+loadString(object,"meshfile")){}
	void draw() const{
		glBindVertexArray(vertexArray.vao);
		glDrawElements(GL_TRIANGLES,vertexArray.nrOfIndices,GL_UNSIGNED_SHORT,(void*)0);
	}
};

class SimpleScene{
public:
	vector<SimpleObject> objects;
	SimpleScene(vector<SimpleObject> o)
		: objects(o){}
	SimpleScene(string jsonstring){
		cout<<"loading scene"<<endl;
		Json::Value scene;
		Json::Reader reader;

		if(!reader.parse(jsonstring,scene)) throw BadJson(reader);

		cout<<"loading meshes"<<flush;
		if(!scene["meshes"].isArray()) throw BadScene("no array called \"meshes\"");
		cout<<"("<<scene["meshes"].size()<<")"<<endl;

		for(unsigned i=0; i<scene["meshes"].size(); i++){
			objects.push_back(SimpleObject(scene["meshes"].get(i,Json::nullValue)));
		}
		// TODO tie together all transform dependencies
	}
};

class SimpleShader:public Shader{
	Uniform<vec3> color;
	SimpleScene activeScene;
public:
	SimpleShader(SimpleScene s)
		:Shader(make_shader(readFile(shader_path+"/simple.vert"),
							readFile(shader_path+"/simple.frag"))),
		 color(Uniform<vec3>(program,"color")),
		 activeScene(s){}
	void setCamera(Camera c){
		view	   = c.transform.getMatrix();
		projection = c.getProjection();
	}
	void draw(){
		for(const SimpleObject &o : activeScene.objects){
			color=o.color;
			model=o.transform.getMatrix();
			o.draw();
		}
	}
};
