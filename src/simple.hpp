#pragma once

#include "shader.hpp"
#include "json.hpp"
#include <istream>
#include <vector>

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
	SimpleObject(vector<GLfloat>  vertices,
				 vector<GLushort> indices,
				 Transform trans,vec3 _color)
			:transform(trans),
			 color(_color),
			 size(indices.size()){
		upload(vertices,indices);
	}
	SimpleObject(Json::Value object)
			:transform(object["transform"]),
			 color(loadVec3(object,"color")){
		string meshname = mesh_path+loadString(object,"meshfile");
		cout<<"loading mesh: "<<meshname<<endl;

		Json::Reader reader;
		Json::Value  mesh;

		if(!reader.parse(readFile(meshname),mesh)) throw BadJson(reader);

		auto vertices = loadFloatArray(mesh,"vertices");
		auto indices  = loadU16Array(mesh,"indices");

		size = indices.size();

		upload( vertices, indices );
	}
	void draw() const{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES,size,GL_UNSIGNED_SHORT,(void*)0);
	}
private:
	// ONLY USE ONCE -- will leak video memory when used more than once per instance
	void upload(vector<GLfloat> vertices, vector<GLushort> indices){

		cout<<"uploading "<<vertices.size()<<" vertices"<<endl;
		cout<<"      and "<<indices.size() <<" indices"<<endl;

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
