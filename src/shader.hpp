#pragma once

#include "camera.hpp"

void shaderSource(GLuint shader,string src){
	const GLint len = src.size();
	const char *cstr = src.c_str();
	glShaderSource(shader, 1, &cstr, &len);
}

bool compiled(GLint shader){
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	return compiled?true:false;
}

string getCompileError(GLint shader){
	GLint log_len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

	GLchar* log_txt = new GLchar[log_len];
	glGetShaderInfoLog(shader, log_len, &log_len, &log_txt[0]);
	return string(log_txt);
}

GLuint make_shader(string vert_src,string frag_src){
	// compile vertex shader
	GLuint vert_o = glCreateShader(GL_VERTEX_SHADER);
	shaderSource(vert_o,vert_src);
	glCompileShader(vert_o);
	if(!compiled(vert_o)){
		cerr<<getCompileError(vert_o)<<endl;
	}

	// compile fragment shader
	GLuint frag_o = glCreateShader(GL_FRAGMENT_SHADER);
	shaderSource(frag_o,frag_src);
	glCompileShader(frag_o);
	if(!compiled(frag_o)){
		cerr<<getCompileError(frag_o)<<endl;
	}

	// link program
	GLint prog = glCreateProgram();
	glAttachShader(prog,vert_o);
	glAttachShader(prog,frag_o);
	glLinkProgram(prog);

	// delete shaders
	glDetachShader(prog,vert_o);
	glDeleteShader(vert_o);
	glDetachShader(prog,frag_o);
	glDeleteShader(frag_o);

	return prog;
}

class Shader{
protected:
	GLuint program;
	Uniform<mat4> model;
	Uniform<mat4> projection;
	Uniform<mat4> view;
public:
	void use(){glUseProgram(program);}
	static GLuint use(GLuint prog){glUseProgram(prog); return prog;}
	Shader(GLuint prog):
		program(use(prog)),
		projection(Uniform<mat4>(prog,"projection")),
		model(Uniform<mat4>(prog,"model")),
		view(Uniform<mat4>(prog,"view")){}
	virtual ~Shader(){}
};
