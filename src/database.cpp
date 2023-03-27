#include "database.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

// for testing purpose only, disable to avoid compile error
int main(){
    loadmap();
}

// actual function for interpreting
void loadmap() {
    // variables red from file
    int i = 0;
    std::string bytes;
    int namelength;
    int width;
    int length;
    std::string name;
    //int map[width][length] = {};

    //open file
    std::ifstream file("../data/maps/test.sub");

    // get map name length
    file.seekg(0, std::ios::beg);
    char a = file.get();
    file.seekg(1, std::ios::beg);
    char b = file.get();
    namelength =  (a - '0') * 10 + (b - '0');


    std::cout << namelength << std::endl;
    file.close();

} 