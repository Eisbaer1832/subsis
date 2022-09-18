#include <fstream>
#include "../nlohmann/json.hpp"
#include <iostream>
#include <chrono>
#include <math.h>
#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif
using json = nlohmann::json;
using namespace std;
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>
#include <thread>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
int type_cycle;
// json file strings
string answer, file;

//statistic integers
int cycle, lost_money, error_count;

// int field_type_1, field_type_2, field_type_3, field_type_4, field_type_5, field_type_6, field_type_7, field_type_8, field_type_9, field_type_10, field_type_11, field_type_12, field_type_13, field_type_14, field_type_15, ield_type_16, field_type_17, field_type_18, field_type_19, field_type_20;




//integers for (possible) losses
int possible_loss, delay_passenger;

//////////////////////////////////////////////////////////////////////////////////////
//																					//
//																					//
//						Image loading												//
//																					//
//																					//
//////////////////////////////////////////////////////////////////////////////////////

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
	{
		// Load from file
		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
		if (image_data == NULL)
			return false;

		// Create a OpenGL texture identifier
		GLuint image_texture;
		glGenTextures(1, &image_texture);
		glBindTexture(GL_TEXTURE_2D, image_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

		// Upload pixels into texture
	#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	#endif
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		stbi_image_free(image_data);

		*out_texture = image_texture;
		*out_width = image_width;
		*out_height = image_height;

		return true;
}
void SetupImGuiStyle()
{
	// Visual Studio style by MomoDeve from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();
	
	style.Alpha = 1.0;
	style.DisabledAlpha = 0.6000000238418579;
	style.WindowPadding = ImVec2(8.0, 8.0);
	style.WindowRounding = 0.0;
	style.WindowBorderSize = 1.0;
	style.WindowMinSize = ImVec2(32.0, 32.0);
	style.WindowTitleAlign = ImVec2(0.0, 0.5);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0;
	style.ChildBorderSize = 1.0;
	style.PopupRounding = 0.0;
	style.PopupBorderSize = 1.0;
	style.FramePadding = ImVec2(4.0, 3.0);
	style.FrameRounding = 0.0;
	style.FrameBorderSize = 0.0;
	style.ItemSpacing = ImVec2(8.0, 4.0);
	style.ItemInnerSpacing = ImVec2(4.0, 4.0);
	style.CellPadding = ImVec2(4.0, 2.0);
	style.IndentSpacing = 21.0;
	style.ColumnsMinSpacing = 6.0;
	style.ScrollbarSize = 14.0;
	style.ScrollbarRounding = 0.0;
	style.GrabMinSize = 10.0;
	style.GrabRounding = 0.0;
	style.TabRounding = 0.0;
	style.TabBorderSize = 0.0;
	style.TabMinWidthForCloseButton = 0.0;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5, 0.5);
	style.SelectableTextAlign = ImVec2(0.0, 0.0);
	
	style.Colors[ImGuiCol_Text] = ImVec4(1.0, 1.0, 1.0, 1.0);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5921568870544434, 0.5921568870544434, 0.5921568870544434, 1.0);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_Border] = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.321568638086319, 0.321568638086319, 0.3333333432674408, 1.0);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3529411852359772, 0.3529411852359772, 0.3725490272045135, 1.0);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3529411852359772, 0.3529411852359772, 0.3725490272045135, 1.0);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.321568638086319, 0.321568638086319, 0.3333333432674408, 1.0);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104, 0.1882352977991104, 0.2000000029802322, 1.0);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971, 0.3098039329051971, 0.3490196168422699, 1.0);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832, 0.2274509817361832, 0.2470588237047195, 1.0);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0, 0.0, 0.0, 0.0);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0, 1.0, 1.0, 0.05999999865889549);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0, 1.0, 1.0, 0.699999988079071);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929, 0.800000011920929, 0.800000011920929, 0.2000000029802322);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
}
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
	if (1 + (float)(rand()) / ((float)(RAND_MAX/(2 - 1))) <= 1.5){
		cout << "A problem occured." << endl;	
		train1_speed = 10;
		train1_problem_time = (1 + (float)(rand()) / ((float)(RAND_MAX/(2 - 1))) <= 5);
	}
}

void train_stuff()
{
		
		while(train1_aproximet == train1_aproximet)
		{	
			// Resets existing problems
			train1_problem_cycle++;

			if (train1_problem_cycle >= train1_problem_time) {
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

//////////////////////////////////////////////////////////////////////////////////////
//																					//
//																					//
//									main function									//
//																					//
//																					//
//////////////////////////////////////////////////////////////////////////////////////

//main function
int main(int, char**) {
	
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
	std::thread x(train_stuff);
	x.detach();



//////////////////////////////////////////////////////////////////////////////////////
//																					//
//																					//
//					Section to setup imgui dependencys								//
//																					//
//																					//
//////////////////////////////////////////////////////////////////////////////////////

	// Setup window
    if (!glfwInit())
        return 1;

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

	
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Stoerungs und Beeintraechtigungs Simulation im Schienenverkehr", NULL, NULL);
    if (window == NULL)
        return 1;
	glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
	SetupImGuiStyle();
	
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("font.ttf", 16);
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       

   
   	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// image config
	int my_image_width = 0;
	int my_image_height = 0;
	GLuint my_image_texture = 0;
	bool main_window_active = true;
	bool log_window_active = false;
	bool map_window_active = false;
	bool grid_test_window_active = false;

	bool ret = LoadTextureFromFile("../Tux.png", &my_image_texture, &my_image_width, &my_image_height);
	IM_ASSERT(ret);
	// Main loop

    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		ImGui::BeginMainMenuBar();
						
			if (ImGui::Button("Toggle Main"))
			{
				if (main_window_active == true)
				{
					main_window_active = false;
				}else{
					main_window_active = true;
				}
			}

			if (ImGui::Button("Toggle Log"))
			{
				if (log_window_active == true)
				{
					log_window_active = false;
				}else{
					log_window_active = true;
				}
			}
			
			if (ImGui::Button("Toggle Map"))
			{
				if (map_window_active == true)
				{
					map_window_active = false;
				}else{
					map_window_active = true;
				}
			}

		ImGui::EndMainMenuBar();
		
		if (main_window_active == true)
		{
			ImGui::Begin("Main Window");
			ImGui::End();
		}

		if (log_window_active == true)
		{
            ImGui::Begin("Logs!");

            ImGui::Text("train1_current = %d", train1_current);
            ImGui::Text("train1_left = %d", train1_left);           
            ImGui::Text("train1_direction = %d", train1_direction);
            ImGui::Text("train1_aproximet = %d", train1_aproximet);
            ImGui::Text("train1_length = %d", train1_length);
            ImGui::Text("cycle = %d", cycle);

            ImGui::End();
        }
		

//////////////////////////////////////////////////////////////////////////////////////
//																					//
//																					//
//								Map window											//
//																					//
//																					//
//////////////////////////////////////////////////////////////////////////////////////

		if (map_window_active == true)
		{
			ImGui::Begin("Map");
	
	
			string cycle_read;
	
			if (ImGui::BeginTable("table2", 3))
			{
				string field_temp_type;
				string type[17];
				int field_types[16];
				int my_image_width = 0;
				int my_image_height = 0;
				GLuint my_image_texture = 0;
				bool ret = LoadTextureFromFile("../type_1.png", &type_1, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_2.png", &type_2, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_3.png", &type_3, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_4.png", &type_4, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_5.png", &type_5, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_6.png", &type_6, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_7.png", &type_7, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_8.png", &type_8, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_9.png", &type_9, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_10.png", &type_10, &my_image_width, &my_image_height);
				bool ret = LoadTextureFromFile("../type_11.png", &type_11, &my_image_width, &my_image_height);
				
				while (type_cycle < 16)
				{
					// Reads 16 Ints form json file
					type_cycle++;
					std::string cycle_read = std::to_string(type_cycle);
					std::ifstream file("./json/mapdata.json");
					json map = json::parse(file);
					field_temp_type = map[cycle_read]["type"];
					type[type_cycle] = field_temp_type;
					field_types[type_cycle] = std::stoi(field_temp_type);
				}
				
				ImGui::TableNextColumn();
				ImGui::TableNextRow();

				while (map_cycle < 17)
				{
					map_cycle++;
					ImGui::TableNextColumn();
					ImGui::Image((void*)(intptr_t)field_types[map_cycle], ImVec2(my_image_width, my_image_height));
				}
				ImGui::EndTable();					
			}
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
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

        glfwSwapBuffers(window);
	
    }



	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
