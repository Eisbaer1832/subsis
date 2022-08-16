// Some helpful wesbsites
// https://de.statista.com/themen/123/deutsche-bahn/#topicHeader__wrapper
// https://de.statista.com/statistik/daten/studie/254053/umfrage/zugunfaelle-innerhalb-der-eu-nach-unfallkategorie/



#include <iostream>
#include <chrono>
#include <thread>
#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

using namespace std;

string train1, train2, train3;

// intagers for example train 1
int train1_left, train1_aproximet, train1_length, train1_current, train1_time, train1_speed, train1_saved_speed;


//statistic integerts
int cycle, lost_money, error_count;


//integers for (possible) losses
int possible_loss, delay_pessanger;


// function if train is too late 
void finished() {

	if (train1_time != train1_aproximet){
		cout << "One Train arrived too late" << endl;
		error_count++;
	}else{
		cout << "Train arrived in time" << endl;
	}
	train1_current = 0;
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


// Logs all important values to console.
void log() {				
	cout << "left: " << train1_left << endl;
	cout << "current: " << train1_current <<endl;
	cout << "aproximet left: " <<  (train1_aproximet - train1_current / (train1_speed)) << endl; 
	cout << "length: " << train1_length << endl;
	cout << "aproximet: " << train1_aproximet << endl;
	cout << "cycle: " << cycle << endl;

}


// Creates Problems
void create_problem() {

	if (1 + (float)(rand()) / ((float)(RAND_MAX/(2 - 1))) <= 1.5){
		cout << "A problem accured.";		
		train1_speed = 10;
	}


}


//main function
int main() {

	// Sets values for example train1
	train1_length = 50;
	train1_speed = 25;
	train1_aproximet = (train1_length / train1_speed);	 
	cout << "In this simultaion one second is equal to one minute!" << endl; 
	delay_pessanger = 50;
	train1_saved_speed = train1_speed;
	
	for(;;)
	{	// Resets existing problems
		train1_speed = train1_saved_speed;


		// Triggers creation of roblems
		create_problem();	
	
		// Used for logging
		cycle++;
		
		train1_time++;
		train1_current = train1_current + train1_speed;	
		train1_left = (train1_length - train1_current) / (train1_speed );
	
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
	
	log();
	
	cout << "###################################" << endl;
	
	Sleep(1000);
	
	}
}


// Sets values for example train1
