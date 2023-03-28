#include "database.h"
#include <iostream>
#include <fstream>

// for testing purpose only, disable to avoid compile error
int main(){
    loadmap();
}

// actual function for interpreting
void loadmap() {
    // variables red from file
    int map_offset;
    std::string name;
    int namelength;

    //open file
    std::ifstream file("../data/maps/test.sub");

    // get map name height
    file.seekg(0, std::ios::beg);
    char a = file.get();
    file.seekg(1, std::ios::beg);
    char b = file.get();
    namelength =  (a - '0') * 10 + (b - '0');

    //get map name
    for (int i = 2; i < namelength + 2;)
    {
        file.seekg(i, std::ios::beg);
        char part_name = file.get();
        name.append(1, part_name);
        i++;
    }

    // get the size of the map
    std::string temp_width;
    std::string temp_length;
    for (int i = 2 + namelength; i < namelength + 10;)
    {
        file.seekg(i, std::ios::beg);
        char part_char = file.get();

        if (i < 6 + namelength)
        {
            temp_width.append(1, part_char);
        }else{
            temp_length.append(1, part_char);
        }
        i++;
    }
    const int width = std::stoi(temp_width);
    const int height = std::stoi(temp_length);
    std::cout << height << std::endl;
    // creates the 2d array and allocates memory
    // DO NOT REPLACE with map[width][height]

    int** map = new int*[width];
    for (int i = 0; i < width; i++) {
        map[i] = new int[height];
        for (int j = 0; j < height; j++) {
            map[i][j] = 0; // initialize each element to 0
        }
    }
    // load the actual map
    map_offset = namelength + 10;
    int c_width = 0;
    int c_length = 0;
    for (int i = map_offset; i < (width*height)*2+map_offset;)
    {
        c_width++;
        if (c_width > width)
        {
            c_width = 1;
            c_length++;
        }

        std::string comb_char;

        file.seekg(i, std::ios::beg);
        char part_char = file.get();
        comb_char.append(1, part_char);
        i++;

        file.seekg(i, std::ios::beg);
        part_char = file.get();
        comb_char.append(1, part_char);

        std::cout << std::stoi(comb_char);
        std::cout << " in line: " << c_length << ", on hoirzontal: " << c_width << std::endl;
        map[c_width][c_length] = std::stoi(comb_char);
        i++;
    }
    // logging the data
    std::cout << "namelength: " << namelength << std::endl;
    std::cout << "name: " << name << std::endl;
    std::cout << "Map size: " << width << " : " << height << std::endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << map[i][j] << " ";
        }
        std::cout << std::endl;
    }
    file.close();

} 