#pragma once

#include "graphics.hpp"
#include<chrono>
using namespace std;

class Scheduler{
	using Clock = chrono::high_resolution_clock;
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
	Scheduler(Input in, Graphics g):running(false),input(in),graphics(g){}
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
