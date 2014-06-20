#version 140

uniform vec3 color;

in  vec3 pos;
out vec4 fragColor;

vec3 lighting(const vec3 normal, const vec3 lightangle){
	return vec3(acos(dot(normal,lightangle)));
}

void main(){
	vec3 normal = normalize(cross(dFdx(pos), dFdy(pos)));
	vec3 lightangle = normalize(vec3(1.0,1.0,1.0));
	fragColor = vec4(color*lighting(normal,lightangle),1.0);
}
