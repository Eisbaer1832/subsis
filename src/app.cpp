#include <fstream>
#include "../nlohmann/json.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>
#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif
using json = nlohmann::json;
using namespace std;
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

string train1, train2, train3;

// intagers and bools for example train 1
int train1_left, train1_problem_time, train1_problem_cycle, train1_railroad, train1_aproximet, train1_ticket_price, train1_passengers, train1_length, train1_current, train1_time, train1_speed, train1_saved_speed;
bool train1_direction;


// intagers and bools for example train 2
int train2_left, train2_railroad, train2_aproximet, train2_ticket_price, train2_passengers, train2_length, train2_current, train2_time, train2_speed, train2_saved_speed;
bool train2_direction;

// int and bools for railroad system
int railroad1_length, railroad2_length;
bool point;
bool signal;

// int time and space
int clock
int min



// json file strings
string answer, file;

//statistic integers
int cycle, lost_money, error_count;


//integers for (possible) losses
int possible_loss, delay_passenger;



// function if train is too late 
void finished() {

	if (train1_time != train1_aproximet){
		cout << "One Train arrived too late" << endl;
		error_count++;
	}else{
		cout << "Train arrived in time" << endl;
	}
	
	if (train1_direction == true) 
	{
		train1_direction = false;
	}else{
		train1_direction = true;
	}
	train1_time = 0;
}


// Calculates best option when a train wont arrive in time.
void best_option() {
	//cout << "calculating best option" << endl;

	if ((train1_left + train1_current) - train1_aproximet >= 60 &&  (train1_left + train1_current) - train1_aproximet < 120){
		cout << "One train is at least 60 minutes late!" << endl;
		// 25% loss of income per ticket
	}
	if ((train1_left + train1_current) - train1_aproximet >= 120){
		cout << "One train is at least 120 minutes late!" << endl;
		// 50% loss of income per ticket
	}
}

// Initialise example railroad system
void railroad_init() {
	railroad1_length = 5000;
	railroad2_length = 3000;
	int railroad1_crossing_2 = 2300;
	int railroad2_crossing_1 = 300;

	train1_railroad = 1;
	train2_railroad = 2;

}


// Logs all important values to console.
void log() {				
	cout << "left: " << train1_left << endl;
	cout << "current: " << train1_current <<endl;
	cout << "aproximet left: " <<  (train1_aproximet - train1_current / (train1_speed)) << endl; 
	cout << "length: " << train1_length << endl;
	cout << "aproximet: " << train1_aproximet << endl;
	cout << "cycle: " << cycle << endl;

}


// Used to create / recreate the database
void rebuild_database(){

	json jsonfile;

	jsonfile["train"] = "train";

	std::ofstream file("data.json");
	file << std::setw(4) << jsonfile << endl;
}



// Creates Problems
void create_problem() {
	if (1 + (float)(rand()) / ((float)(RAND_MAX/(2 - 1))) <= 1.5 && train1_problem_cycle == 0){
		cout << "A problem occured." << endl;	
		train1_speed = 10;
		train1_problem_time = (1 + (float)(rand()) / ((float)(RAND_MAX/(2 - 1))) <= 5);
	}
}

//main function
int main() {

	float buf;
	int f;

	ImGui::Text("Hello, world %d", 123);
	if (ImGui::Button("Save"))
		MySaveFunction();
	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

	// Sets values for example train1
	train1_length = 500;
	train1_speed = 25;
	train1_direction = true;
	train1_problem_cycle = 0;
	train1_passengers = 120;
	train1_aproximet = (train1_length / train1_speed);	  
	cout << "In this simultaion one second is equal to one minute!" << endl;
	train1_saved_speed = train1_speed;
	//cout << "Would you like to rebuild the database and rebuild the database? y/n" << endl; 
	//cin >> answer;
	
	if (answer == "y"){
		cout << "Rebuilding database ..." << endl;
		rebuild_database();
	}

	for(;;)
	{	// Resets existing problems
		train1_problem_cycle++;

		if (train1_problem_time == train1_problem_cycle) {
			train1_speed = train1_saved_speed;
			train1_problem_cycle = 0;

		} 

		// Triggers creation of roblems
		create_problem();	
	
		// Used for logging
		cycle++;
		
		// Moves the train
		train1_time++;
		if (train1_direction == true){
			train1_current = train1_current + train1_speed;	
			train1_left = (train1_length - train1_current) / (train1_speed);
			
			// Checks if train will arrive in time
			if (train1_left > (train1_aproximet - train1_current / (train1_speed))){

				// A train probaply wont arrive in time!
				cout << "A train wont arrive in time." << endl;
				best_option();
			}

			// Checks if Train arrived its destination
			if (train1_current >= train1_length){
			finished();	
			}	
		}else{
			train1_current = train1_current - train1_speed;	
			train1_left = (train1_length - (train1_length - train1_current)) / (train1_speed);
			
			// Checks if train will arrive in time
			if (train1_left > (train1_aproximet - (train1_length - train1_current) / (train1_speed))){
				// A train probaply wont arrive in time!
				cout << "A train wont arrive in time." << endl;
				best_option();
			}
			
			// Checks if Train arrived its destination
			if ((train1_length - train1_current) >= train1_length){
			finished();	
			}	
		}




	log();
	
	cout << "###################################" << endl;
	
	Sleep(1000);
	
	}
}