#pragma once

#include "shader.hpp"
#include "json.hpp"
#include <istream>
#include <vector>

#include "mesh.hpp"
#include "vertexarrayhandle.hpp"

class SimpleObject{
	VertexArrayHandle vertexArray;
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
