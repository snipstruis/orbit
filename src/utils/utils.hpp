#pragma once

#include <GL/glew.h>
#include <fstream>
#include <string>
#include <cerrno>
using namespace std;

string readFile(string filename){
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if(in){
		string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}
