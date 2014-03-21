#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

#include <stdexcept>
using namespace std;

class BadScene:public exception{
public:	const string reason;
	BadScene(string _reason):reason(_reason){}
	virtual const char* what() const throw(){
		return string("Bad scene: ").append(reason).c_str();
	}
};

class BadJson:public exception{
public:	const string errormsg;
	BadJson(Json::Reader reader) :errormsg(reader.getFormatedErrorMessages()){}
	virtual const char* what() const throw(){
		return string("JSON parse error: ").append(errormsg).c_str();
	}
};

string readFile(const char *filename){
	ifstream in(filename, ios::in | ios::binary);
	if(in){
		string contents;
		in.seekg(0, ios::end);
		contents.resize(in.tellg());
		in.seekg(0, ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

double loadDouble(Json::Value container,string name){
	if( container[name].isNumeric() ){
		return container[name].asDouble();
	}
	else throw BadScene(
		string("no numeric value called \"")
		.append(name).append("\"")
	);
}

string loadString(Json::Value container, string name){
	if(container[name].isString()){
		return container[name].asString();
	}
	else throw BadScene(
		string("no string called \"")
		.append(name).append("\"")
	);
}

vector<float> loadFloatArray(Json::Value container,string name){
	Json::Value jval = container[name];
	if(!jval.isArray()) throw BadScene(string("no numeric array called ").append(name));

	unsigned size = jval.size();

	vector<float> v(size);

	for(unsigned i=0; i<size; i++){
		if( !jval.get(i,Json::nullValue).isNumeric() ){
			throw BadScene( string("\tinvalid numeric array: ")
					.append(name).append("[").append(to_string(i)).append("]"));
		}
		v[i] = static_cast<float>(jval.get(i,0).asDouble());
	}

	return v;
}

vector<uint16_t> loadU16Array(Json::Value container, string name){

	Json::Value jval = container[name];

	if(!jval.isArray()) throw BadScene(string("no unsigned array called ").append(name));

	unsigned size = jval.size();
	vector<uint16_t> v(size);

	for(unsigned i=0; i<size; i++){
		if(jval.get(i,NAN)==NAN){
			throw BadScene( string("\tarray element unreadable: ")
				.append(name).append("[").append(to_string(i)).append("]"));
		}
		if(!jval.get(i,0).isConvertibleTo(Json::uintValue) ){
			throw BadScene( string("\tarray element not unsigned integer: ")
				.append(name).append("[").append(to_string(i)).append("]"));
		}
		if( jval.get(i,0).asUInt() > USHRT_MAX ){
			throw BadScene( string("\tu16 array overflow: ")
				.append(name).append("[").append(to_string(i)).append("]"));
		}
		v[i] = static_cast<uint16_t>( jval.get(i,0).asUInt() );
	}

	return v;
}

vec3 loadVec3(Json::Value container,string name){
	if(container[name].isArray()
	&&(container[name].size()==3)
	&& container[name].get(unsigned(0),Json::nullValue).isNumeric()
	&& container[name].get(unsigned(1),Json::nullValue).isNumeric()
	&& container[name].get(unsigned(2),Json::nullValue).isNumeric()){
		return vec3(
			container[name].get(unsigned(0),NAN).asDouble(),
			container[name].get(unsigned(1),NAN).asDouble(),
			container[name].get(unsigned(2),NAN).asDouble()
		);
	}else throw BadScene(string("no numeric array of size 3 called \"")
						 .append(name).append("\""));
}

quat loadQuat(Json::Value container, string name){
	if(container[name].isArray()
	&&(container[name].size()==4)
	&& container[name].get(unsigned(0),Json::nullValue).isNumeric()
	&& container[name].get(unsigned(1),Json::nullValue).isNumeric()
	&& container[name].get(unsigned(2),Json::nullValue).isNumeric()
	&& container[name].get(unsigned(3),Json::nullValue).isNumeric()){
		return quat(
			container[name].get(unsigned(0),NAN).asDouble(),
			container[name].get(unsigned(1),NAN).asDouble(),
			container[name].get(unsigned(2),NAN).asDouble(),
			container[name].get(unsigned(3),NAN).asDouble()
		);
	}else throw BadScene(string("no numeric array of size 4 called \"")
						 .append(name).append("\""));
}
