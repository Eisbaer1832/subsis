using namespace std;
#include "./database.h"
#include <fstream>
#include <iostream>
#include <math.h>
#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif
#include "../imgui/imgui.h"
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>
#include <thread>
#define STB_IMAGE_IMPLEMENTATION
#include "../backends/stb_image.h"


// Declares how many Trains should be created
int trains_to_create = 4;
string train [] = {};
int train_left [] = {};
int train_problem_time [] = {};
int train_problem_cycle [] = {};
int train_railroad [] = {};
int train_aproximet [] = {};
int train_ticket_price [] = {};
int train_passengers [] = {};
int train_length [] = {};
int train_current [] = {};
int train_time [] = {};
float train_speed [] = {};
int train_saved_speed [] = {};
bool train_direction [] = {};
// arays arent  used in code yet.

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

string menu_time;
// int field_type_1, field_type_2, field_type_3, field_type_4, field_type_5, field_type_6, field_type_7, field_type_8, field_type_9, field_type_10, field_type_11, field_type_12, field_type_13, field_type_14, field_type_15, ield_type_16, field_type_17, field_type_18, field_type_19, field_type_20;




//integers for (possible) losses
int possible_loss, delay_passenger;

//variables for clock and timing 
int global_hour = 0;
int global_minute = 0;
string global_string_hour = "00";
string global_string_minute = "00";
string global_clock = "00:00";

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
void finished(int chosen_train) {

	if (train_time[chosen_train] != train_aproximet[chosen_train]){
		cout << "One Train arrived too late" << endl;
		error_count++;
	}else{
		cout << "Train arrived in time" << endl;
	}
	
	if (train_direction[chosen_train] == true) 
	{
		train_direction[chosen_train] = false;
	}else{
		train_direction[chosen_train] = true;
	}
	train_time[chosen_train] = 0;
}


// Calculates best option when a train wont arrive in time.
void best_option(int i) {
	//cout << "calculating best option" << endl;
	if ((train_left[i] + train_current[i]) - train_aproximet[i] >= 60 &&  (train_left[i] + train_current[i]) - train_aproximet[i] < 120){
		cout << "One train is at least 60 minutes late!" << endl;
		// 25% loss of income per ticket
	}
	if ((train_left[i] + train_current[i]) - train_aproximet[i] >= 120){
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
}


// Logs all important values to console.
void log(int i) {			
	cout << "left: " << train_left[i] << endl;
	cout << "current: " << train_current[i] <<endl;
	cout << "aproximet left: " <<  (train_aproximet[i] - train_current[i] / (train_speed[i])) << endl; 
	cout << "length: " << train_length[i] << endl;
	cout << "aproximet: " << train_aproximet[i] << endl;
	cout << "cycle: " << cycle << endl;

}


// Creates Problems
void create_problem(int i) {
	if (1 + (float)(rand()) / ((float)(RAND_MAX/(2 - 1))) <= 1.5){
		cout << "A problem occured." << endl;	
		train_speed[i] = 10;
		train_problem_time[i] = (1 + (float)(rand()) / ((float)(RAND_MAX/(2 - 1))) <= 5);
	}
}

void train_initiation(int i)
{
	while(train_aproximet[i] == train_aproximet[i])
	{
		// Resets existing problems
		train_problem_cycle[i]++;

		if (train_problem_cycle[i] >= train_problem_time[i]) {
			train_speed[i] = train_saved_speed[i];
			train_problem_cycle[i] = 0;
		} 

		// Triggers creation of roblems
		create_problem(i);	
		
		// Used for logging
		cycle++;
			
		// Moves the train
		train_time[i]++;
		if (train_direction[i] == true){
			train_current[i] = train_current[i] + train_speed[i];	
			train_left[i] = (train_length[i] - train_current[i]) / (train_speed[i]);
				
			// Checks if train will arrive in time
			if (train_left[i] > (train_aproximet[i] - train_current[i] / (train_speed[i]))){

				// A train probaply wont arrive in time!
				cout << "A train wont arrive in time." << endl;
				best_option(i);
			}

			// Checks if Train arrived its destination
			if (train_current[i] >= train_length[i]){
				finished(i);	
			}	
		}else{
			train_current[i] = train_current[i] - train_speed[i];	
			train_left[i] = (train_length[i] - (train_length[i] - train_current[i])) / (train_speed[i]);
				
			// Checks if train will arrive in time
			if (train_left[i] > (train_aproximet[i] - (train_length[i] - train_current[i]) / (train_speed[i]))){
				// A train probaply wont arrive in time!
				cout << "A train wont arrive in time." << endl;
				best_option(i);
			}
				
			// Checks if Train arrived its destination
			if ((train_length[i] - train_current[i]) >= train_length[i]){
				finished(i);	
			}	
		}

		//uncomment for logging to console
		//log(i);
		
			
		global_minute++;

		if (global_minute == 60) 
		{
			global_minute = 0;
			global_hour++;
			
			if (global_hour == 24)
			{
				global_hour = 0;
			}
		}

		if (global_minute < 10) 
		{
			global_string_minute = "0" + std::to_string(global_minute);
			cout << global_string_minute << endl;
		}else{
			global_string_minute = std::to_string(global_minute);
		}
			
			
		if (global_hour < 10) 
		{
			global_string_hour = "0" + std::to_string(global_hour);
		}else{
			global_string_hour = std::to_string(global_hour);
		}

		global_clock = global_string_hour +  ":" + global_string_minute + " Uhr";
		cout << global_clock << endl;
		menu_time = global_clock;
		cout << "###################################" << endl;
		Sleep(1000);
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//										    //
//				main function				            //
//									            //
//////////////////////////////////////////////////////////////////////////////////////

//main function
int main(int, char**) {
	loadmap();

	int i = 0;
	while (i < trains_to_create)
	{
        i++;
		train_direction[i] = true;
		train_problem_cycle[i] = 0;
		train_speed[i] = 25;
		train_passengers[i] = 120;
		train_aproximet[i] = (train_length[i] / train_speed[i]);	  
		train_saved_speed[i] = train_speed[i];
        train_length[i] = 500;

		// Info
		cout << "In this simultaion one second is equal to one minute!" << endl;

		//cout << "Would you like to rebuild the database and rebuild the database? y/n" << endl; 
		//cin >> answer;
	
		if (answer == "y"){
			cout << "Rebuilding database ..." << endl;
		}
		std::thread x([&i]() { train_initiation(i); });
		x.detach();
	    cout << "i: " << i << endl;
	}


//////////////////////////////////////////////////////////////////////////////////////
//      		                                                                    //
// 	       		Section to setup imgui dependencys	                                //
//										                                            //
//////////////////////////////////////////////////////////////////////////////////////

	// Setup window
    if (!glfwInit())
        return 1;

	// Decide GL+GLSL versions
if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
elif defined(__APPLE__)
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


	GLFWwindow* window = glfwCreateWindow(480, 120, "Stoerungs und Beeintraechtigungs Simulation im Schienenverkehr", NULL, NULL);
    if (window == NULL)
        return 1;
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    SetupImGuiStyle();
	
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       

   


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// image config
	int my_image_width = 0;
	int my_image_height = 0;
	GLuint type_1 = 0;
	GLuint type_2 = 0;
	GLuint type_3 = 0;
	GLuint type_4 = 0;
	GLuint type_5 = 0;
	GLuint type_6 = 0;
	GLuint type_7 = 0;
	GLuint type_8 = 0;
	GLuint type_9 = 0;
	GLuint type_10 = 0;
	GLuint type_11 = 0;
	GLuint type_12 = 0;
	GLuint type_13 = 0;
	GLuint type_14 = 0;
	GLuint type_15 = 0;
	GLuint type_16 = 0;

	//bool ret = LoadTextureFromFile("../type_1.jpg", &type_1, &my_image_width, &my_image_height);
	//bool ret2 = LoadTextureFromFile("../type_2.jpg", &type_2, &my_image_width, &my_image_height);
	//bool ret3 = LoadTextureFromFile("../type_3.jpg", &type_3, &my_image_width, &my_image_height);
	//bool ret4 = LoadTextureFromFile("../type_4.jpg", &type_4, &my_image_width, &my_image_height);
	//bool ret5 = LoadTextureFromFile("../type_5.jpg", &type_5, &my_image_width, &my_image_height);
	//bool ret6 = LoadTextureFromFile("../type_6.jpg", &type_6, &my_image_width, &my_image_height);
	//bool ret7 = LoadTextureFromFile("../type_7.jpg", &type_7, &my_image_width, &my_image_height);
	//bool ret8 = LoadTextureFromFile("../type_8.jpg", &type_8, &my_image_width, &my_image_height);
	//bool ret9 = LoadTextureFromFile("../type_9.jpg", &type_9, &my_image_width, &my_image_height);
	//bool ret10 = LoadTextureFromFile("../type_10.jpg", &type_10, &my_image_width, &my_image_height);
	//bool ret11 = LoadTextureFromFile("../type_11.jpg", &type_11, &my_image_width, &my_image_height);
	//bool ret12 = LoadTextureFromFile("../type_12.jpg", &type_12, &my_image_width, &my_image_height);	
	//bool ret13 = LoadTextureFromFile("../type_13.jpg", &type_13, &my_image_width, &my_image_height);
	//bool ret14 = LoadTextureFromFile("../type_14.jpg", &type_14, &my_image_width, &my_image_height);
	//bool ret15 = LoadTextureFromFile("../type_15.jpg", &type_15, &my_image_width, &my_image_height);
	//bool ret16 = LoadTextureFromFile("../type_16.jpg", &type_16, &my_image_width, &my_image_height);
	
	
	bool train_window_active = false;
	bool log_window_active = false;
	bool map_window_active = false;

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
				
						
			if (ImGui::Button("Toggle train"))
			{
				if (train_window_active == true)
				{
					train_window_active = false;
				}else{
					train_window_active = true;
				}
			}

			if (ImGui::Button("Toggle Log"))
			{
				if (log_window_active == true)
				{
					log_window_active = false;
				}else{
			if (ImGui::Button("Toggle Map"))
			{
				if (map_window_active == true)
				{
					map_window_active = false;
				}else{
					map_window_active = true;
				}
			}
        ImGui::SameLine(ImGui::GetWindowWidth()-70);
					log_window_active = true;
				}
			}

        ImGui::AlignTextToFramePadding();
        ImGui::Text(menu_time.c_str());
	ImGui::EndMainMenuBar();
		
//////////////////////////////////////////////////////////////////////////////////////
//										                                            //
// 										                                            //
//	                Train window 🚆		🦆🦆🦆🦆🦆🦆🦆                              //
//										    //
//										    //
//////////////////////////////////////////////////////////////////////////////////////

	if (train_window_active == true)
	{
		ImGui::Begin("train Window");
		int train_count = 2;
		string train_name[2];
		string cycle_read;
		string train_temp_name;
		while (type_cycle < train_count)
		{
			// Reads 16 Ints form json file
			type_cycle++;
			std::string cycle_read = std::to_string(type_cycle);
			train_temp_name = train_name[type_cycle];
		}
       	{
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::BeginChild("train list", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);
	static int item_current_idx = 0;
	if (ImGui::BeginListBox("##Trains", ImVec2(-FLT_MIN, 100 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int n = 0; n < IM_ARRAYSIZE(train_name); n++)
			{
				bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable("test", is_selected))
					item_current_idx = n;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}
	          	ImGui::EndChild();
		}

	        ImGui::SameLine();
        	{
			ImGui::BeginChild("train list2", ImVec2(0, 260), false);
			const char* items[] = { "train1", "train2", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
			static int item_current_idx = 0;
			if (ImGui::BeginListBox("Trains", ImVec2(-FLT_MIN, 50 * ImGui::GetTextLineHeightWithSpacing())))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{	
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}		
			ImGui::EndChild();
		}
	ImGui::End();
	}

	if (log_window_active == true)
	{
		int train_logging;
		ImGui::Begin("Logs!");
		while (train_logging < trains_to_create)
		{
			train_logging++;
			ImGui::Text("Train being logged: ", i);
			ImGui::Text("train_current = %d", train_current[i]);
			ImGui::Text("train_left = %d", train_left[i]);
			ImGui::Text("train_direction = %d", train_direction[i]);
			ImGui::Text("train_aproximet = %d", train_aproximet[i]);
			ImGui::Text("train_length = %d", train_length[i]);
			ImGui::Text("cycle = %d", cycle);
		}
		ImGui::End();
	}


//////////////////////////////////////////////////////////////////////////////////////
//	   									    //
//										    //
//				Map window					    //
//		  								    //
//		  								    //
//////////////////////////////////////////////////////////////////////////////////////

if (map_window_active == true)
		{
		ImGui::Begin("Map");
        

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
