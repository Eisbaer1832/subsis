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

// intagersfor example train 1
int train1_aproximet, train1_length, train1_current, train1_time, train1_speed;


//statisticc integerts
int lost_money, error_count;

// function if train is too late 
void finished() {

	if (train1_time != train1_aproximet){
		cout << "One Train is too late" << endl;
		error_count++;
	}else{
		cout << "Train arrived in time" << endl;
	}
}

void drive() {
	cout << "test";
}


//main function
int main() {
	train1_length = 50;
	train1_speed = 400;
	train1_aproximet = (50 / (train1_speed / 60)) + 1;	 
	cout << "In this simultaion one second is equal to one minute!" << endl; 
		
	for(;;)
	{
		if (train1_current > train1_length){
			finished();
		}else{
			train1_time++;
			train1_current = train1_current + train1_speed / 60;
			cout << train1_current << endl;
		}

		Sleep(100);
	}
}

