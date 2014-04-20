#pragma once

#include<vector>
#include<iostream>
#include "utils/json.hpp"
using namespace std;

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
