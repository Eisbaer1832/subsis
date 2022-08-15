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
int train1_left, train1_aproximet, train1_length, train1_current, train1_time, train1_speed;


//statistic integerts
int lost_money, error_count;


//integers for (possible) losses
int possible_loss;

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

void best_option() {
	//cout << "calculating best option" << endl;
	cout << (train1_left + train1_current) - train1_aproximet << endl;

	if ((train1_left + train1_current) - train1_aproximet >= 60 &&  (train1_left + train1_current) - train1_aproximet < 120){
		cout << "One train is at least 60 minutes late!" << endl;
	}
	if ((train1_left + train1_current) - train1_aproximet >= 120){
		cout << "One train is at least 120 minutes late!" << endl;
	}


}
void log() {				
	cout << "left: " << train1_left << endl;
	cout << "current: " << train1_current <<endl;
	cout << "aproximet left: " <<  (train1_aproximet - train1_current / (train1_speed / 60)) - 1<< endl; 
	cout << "length: " << train1_length << endl;
	cout << "aproximet" << train1_aproximet << endl;
}


//main function
int main() {
	train1_length = 50;
	train1_speed = 400;
	train1_aproximet = (train1_length / (train1_speed / 60)) + 1;	 
	cout << "In this simultaion one second is equal to one minute!" << endl; 
	
	// Only for testing purposes, to trigger delays :
	train1_speed = 200;		
	for(;;)
	{

		log()
	
		train1_left = (train1_length - train1_current) / (train1_speed / 60) ;	
	
		if (train1_current >= train1_length){
			finished();
		}else{
			if (train1_left > (train1_aproximet - train1_current / (train1_speed / 60)) - 1){
				// A train probaply wont arrive in time!
				cout << "A train wont arrive in time." << endl;
				best_option();
	
			}
			train1_time++;
			train1_current = train1_current + train1_speed / 60;
				
		}

		Sleep(100);
	}
}

