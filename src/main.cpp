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

using Clock = chrono::high_resolution_clock;

// holds gameloop, will manage threads in future
class TaskManager{
	bool running;
	chrono::microseconds ticktime = chrono::microseconds(1000000/30);
	void fixedTick(){
		input.poll();
		graphics.fixedTick();
	}
	void freeStep(double progress){
		// progress is a value in the range [0,1) representing the point between
		// fixed ticks
		graphics.freeStep();
	}
public:
	TaskManager(Input in, Graphics g):running(false),input(in),graphics(g){}
	void halt(){running = false;}
	void run(){
		running = true;
		auto previous = Clock::now();
		chrono::microseconds lag = chrono::microseconds::zero();
		while(running && !input.shouldClose()){
			auto current = Clock::now();
			auto elapsed = chrono::duration_cast<chrono::microseconds>(current-previous);
			previous = current;
			lag += elapsed;
			// repeat fixed step until we catch up
			while(lag>ticktime){
				fixedTick();
				lag -= ticktime;
			}
			freeStep(lag/ticktime);
		}
	}
public:
	Input    input;
	Graphics graphics;
};

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

/*
	vector<SimpleObject> objects;

	// way to create objects until I implement an OBJ-file loader
	objects.push_back(
		SimpleObject( // ground box
			vector<GLfloat>({
				  -10.0f,	-10.0f,	 0.1f,
				   10.0f,	-10.0f,	 0.1f,
				   10.0f,	 10.0f,	 0.1f,
				  -10.0f,	 10.0f,	 0.1f,
				  -10.0f,	-10.0f,	-0.1f,
				   10.0f,	-10.0f,	-0.1f,
				   10.0f,	 10.0f,	-0.1f,
				  -10.0f,	 10.0f,	-0.1f
			}),vector<GLushort> ({
				 0, 1, 2, 2, 3, 0,
				 3, 2, 6, 6, 7, 3,
				 7, 6, 5, 5, 4, 7,
				 4, 0, 3, 3, 7, 4,
				 0, 1, 5, 5, 4, 0,
				 1, 5, 6, 6, 2, 1
			}),Transform(vec3(0.f,0.f,-0.1f)),
			vec3(20/255.f,30/255.f,20/255.f)
		)
	);
	objects.push_back(
		SimpleObject( // left box (dark blue)
			vector<GLfloat>({
				  -1.0f,	-1.0f,	 1.0f,
				   1.0f,	-1.0f,	 1.0f,
				   1.0f,	 1.0f,	 1.0f,
				  -1.0f,	 1.0f,	 1.0f,
				  -1.0f,	-1.0f,	-1.0f,
				   1.0f,	-1.0f,	-1.0f,
				   1.0f,	 1.0f,	-1.0f,
				  -1.0f,	 1.0f,	-1.0f
			}),vector<GLushort> ({
				 0, 1, 2, 2, 3, 0,
				 3, 2, 6, 6, 7, 3,
				 7, 6, 5, 5, 4, 7,
				 4, 0, 3, 3, 7, 4,
				 0, 1, 5, 5, 4, 0,
				 1, 5, 6, 6, 2, 1
			}),Transform(vec3(-5,0,1)),
			   vec3(50/255.f,00/255.f,180/255.f)
		)
	);
	objects.push_back(
		SimpleObject( // middle box (pale blue)
			vector<GLfloat>({
				  -1.0f,	-1.0f,	 1.0f,
				   1.0f,	-1.0f,	 1.0f,
				   1.0f,	 1.0f,	 1.0f,
				  -1.0f,	 1.0f,	 1.0f,
				  -1.0f,	-1.0f,	-1.0f,
				   1.0f,	-1.0f,	-1.0f,
				   1.0f,	 1.0f,	-1.0f,
				  -1.0f,	 1.0f,	-1.0f
			}),vector<GLushort> ({
				 0, 1, 2, 2, 3, 0,
				 3, 2, 6, 6, 7, 3,
				 7, 6, 5, 5, 4, 7,
				 4, 0, 3, 3, 7, 4,
				 0, 1, 5, 5, 4, 0,
				 1, 5, 6, 6, 2, 1
			}),Transform(vec3(0,0,1)),
			   vec3(50/255.f,95/255.f,180/255.f)
		)
	);
	objects.push_back(
		SimpleObject( // right box (purple)
			vector<GLfloat>({
				  -1.0f,	-1.0f,	 1.0f,
				   1.0f,	-1.0f,	 1.0f,
				   1.0f,	 1.0f,	 1.0f,
				  -1.0f,	 1.0f,	 1.0f,
				  -1.0f,	-1.0f,	-1.0f,
				   1.0f,	-1.0f,	-1.0f,
				   1.0f,	 1.0f,	-1.0f,
				  -1.0f,	 1.0f,	-1.0f
			}),vector<GLushort> ({
				 0, 1, 2, 2, 3, 0,
				 3, 2, 6, 6, 7, 3,
				 7, 6, 5, 5, 4, 7,
				 4, 0, 3, 3, 7, 4,
				 0, 1, 5, 5, 4, 0,
				 1, 5, 6, 6, 2, 1
			}),Transform(vec3(5,0,1)),
			   vec3(50/255.f,20/255.f,80/255.f)
		)
	);*/

	// create camera
	Transform camera_transform;
	camera_transform.origin = vec3(0,10.f,-2.8f);
	camera_transform.rotate(-90.f,vec3(1.f,0.f,0.f)); // rotate so you look along +y axis
	Camera camera(camera_transform,60.f,screensize.x/float(screensize.y));

	// load scene
	SimpleShader simple = SimpleShader( readFile(scene_path + "scene.json") );
	//SimpleShader simple = SimpleShader(objects);

	// connect components to taskmanager and run it
	Input input(window);
	Graphics graphics(window,simple,camera,input);
	TaskManager taskmgr(input,graphics);
	taskmgr.run();

	// cleanup
	glfwTerminate();
}
