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
int watch;
int min;


// json file strings
string answer, file;

//statistic integers
int cycle, lost_money, error_count;


//integers for (possible) losses
int possible_loss, delay_passenger;


   
//////////////////////////////////////////////////////////////////////////////////////
//																					//
//																					//
//						Actual code for train stuff									//
//																					//
//																					//
//////////////////////////////////////////////////////////////////////////////////////



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
//////////////////////////////////////////////////////////////////////////////////////
//																					//
//																					//
//									main function									//
//																					//
//																					//
//////////////////////////////////////////////////////////////////////////////////////


//main function
int main(int, char**) {
//////////////////////////////////////////////////////////////////////////////////////
//																					//
//																					//
//					Section to setup imgui dependencys								//
//																					//
//																					//
//////////////////////////////////////////////////////////////////////////////////////



    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

	// Setup window
    if (!glfwInit())
        return 1;

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Stoerungs und Beeintraechtigungs Simulation im Schienenverkehr", NULL, NULL);
    if (window == NULL)
        return 1;
	 glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           
    ImGui::StyleColorsLight();
 	
	ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



	// Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Overwiev!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.0f ms/frame (%.120f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

		{

            ImGui::Begin("Logs!");

            ImGui::Text("train1_current = %d", train1_current);               // Display some text (you can use a format strings too
            ImGui::Text("train1_left = %d", train1_left);              // Display some text (you can use a format strings too
            ImGui::Text("train1_direction = %d", train1_direction);               // Display some text (you can use a format strings too
            ImGui::Text("train1_aproximet = %d", train1_aproximet);               // Display some text (you can use a format strings too
            ImGui::Text("train1_length = %d", train1_length);               // Display some text (you can use a format strings too
            ImGui::Text("cycle = %d", cycle);               // Display some text (you can use a format strings too

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }


//////////////////////////////////////////////////////////////////////////////////////
//																					//
//																					//
//							more train stuff										//	
//																					//
//																					//
//////////////////////////////////////////////////////////////////////////////////////

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
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}