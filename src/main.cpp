#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <array>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>
#include <cstdint>
#include <chrono>
using namespace std;
using namespace glm;

// paths relative to executable
const string shader_path = "shaders/";
const string scene_path  = "scenes/";
const string mesh_path   = "meshes/";

#include "utils.hpp"
#include "uniform.hpp"
#include "glfwcallbacks.hpp"
#include "glfwadapter.hpp"

#include "transform.hpp"
#include "input.hpp"
#include "shader.hpp"
#include "simple.hpp"
#include "camera.hpp"
#include "graphics.hpp"
#include "scheduler.hpp"

GLFWwindow* createWindow(ivec2 windowsize){
	GLFWwindow* window;

	// create window with an OpenGL 3.1 context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
	window = glfwCreateWindow(windowsize.x, windowsize.y, "floating_window", NULL, NULL);

	// check if window has been created
	if (!window){ glfwTerminate(); exit(EXIT_FAILURE); }
	else cout<<"GLFW: window succesfully created"<<endl;

	// assign context to current thread
	glfwMakeContextCurrent(window);

	return window;
}

int main(){
	if (!glfwInit()) exit(EXIT_FAILURE);
	else cout<<"GLFW: succesfully initialized"<<endl;

	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = createWindow(ivec2(800,600));
	ivec2 screensize = getScreenSize(window);

	// initialise GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		cerr<<"GLEW: "<<glewGetErrorString(err)<<endl;
		exit(EXIT_FAILURE);
	} else cout<<"GLEW: succesfully initialized"<<endl;

	// glfw callbacks
	glfwSetKeyCallback(window,key_callback);
	glfwSetMouseButtonCallback(window,mouse_button_callback);
	glfwSetScrollCallback(window,scroll_callback);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

	// set GL states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_FRONT);

	cout<<"keybindings:"<<endl
		<<"w:     move forward  | q:     roll counter-clockwise"<<endl
		<<"s:     move backward | e:     roll clockwise"<<endl
		<<"a:     move left     | up:    tilt up"<<endl
		<<"d:     move right    | down:  tilt down"<<endl
		<<"shift: move up       | left:  pan left"<<endl
		<<"ctrl:  move down     | right: pan right"<<endl;

	// create camera
	Transform camera_transform;
	camera_transform.origin = vec3(0,10.f,-2.8f);
	camera_transform.rotate(-90.f,vec3(1.f,0.f,0.f)); // rotate so you look along +y axis
	Camera camera(camera_transform,60.f,screensize.x/float(screensize.y));

	// load scene
	SimpleShader simple = SimpleShader( readFile(scene_path + "scene.json") );

	// connect components to taskmanager and run it
	Input input(window);
	Graphics graphics(window,simple,camera,input);
	Scheduler taskmgr(input,graphics);
	taskmgr.run();

	// cleanup
	glfwTerminate();
}
