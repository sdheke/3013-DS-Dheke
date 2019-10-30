//****************************************************************
// Sabin Dheke
// Assignment 3-Graphs
// CMPS 3013
// Professor Terry Griffin
// Due Date: 10/28/2019
// This project is based on priority queues. In this project, 
// Email: sabindheke3@gmail.com
// Files: Geo.hpp, JsonFacade.hpp, Heap.hpp, cities.json, json.hpp, map_parts.hpp
// graphs.hpp and graph_builder.cpp
// This is project, we read a file that consist the name of the cities from cities.json
// which their detail and found the the cities that is closest to them and print
// three cities as a result.
//****************************************************************
// most part of the code is provided by Dr. Griffin
// made some changes in the code as per requirment
//****************************************************************

#include <iostream>
#include <vector>
#include <string.h>
#include "graph.hpp"
#include "json_helper.hpp"
#include <stdlib.h>
#include "map_parts.hpp"
#include <iomanip>
#include <fstream>
#include <stdlib.h>

using namespace std;

City** LoadCities(string filename) {
	json j;
	City *c;

	JsonHelper jhelp(filename);
	City **CityArray = new City*[jhelp.getSize()];

	for (int i = 0; i < jhelp.getSize(); i++) {
		j = jhelp.getIth(i);
		CityArray[i] = new City(j, i);

	}
	return CityArray;
}

int main(int argc, char **argv) {
	int max_outdegree = 5;

	char *p = const_cast<char *>("");
	//****************************************************************
	// Check for errors: e.g., the string does not represent an integer
	// or the integer is larger than int
	//****************************************************************
	if (argc > 1) {
		long conv = strtol(argv[1], &p, 10);
		max_outdegree = conv;
	}
	if (*p != '\0') {
		cout << "You need to give number of nearest cities.\n";
		exit(-1);
	}

	City** CityArray = LoadCities("cities.json");
	//****************************************************************
	// load the cities from cities.json file
	//****************************************************************
	typedef pair< int, int> citypair;
	vector< citypair > distances;

	int distance;
	int id;
	//****************************************************************
	// intilazing the int to distnce and their id
	//****************************************************************
	
	ofstream myfile;
	//****************************************************************
	// opening the file "cities_connections.txt", where we output the
	// result for the assignment.
	//****************************************************************
	myfile.open("cities_connections.txt");
	
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			if (CityArray[i] == CityArray[j]) { continue; }
			distance = CityArray[i]->Distance(CityArray[j]->lat, CityArray[j]->lon);
			distances.push_back(pair<int, int>(distance, CityArray[j]->id));
		}
		sort(distances.begin(), distances.end());

		int out_paths = 0;
		if (i < 10 or i> 990)
	//****************************************************************
	// thsi will print the first 10 cities and the last 10 
	// cities as per assignment's requirment
	//****************************************************************
		{
			cout << setfill('0') << setw(4) << i + 1 << ": " << CityArray[i]->name << endl;
		}
		myfile << setfill('0') << setw(4) << i + 1 << ": " << CityArray[i]->name << endl;
		for (auto& it : distances) {
			if (i < 10 or i> 990) {
				cout << out_paths + 1 << ") " << CityArray[it.second]->name << " " << it.first << " (distance)" << endl;
			}
			myfile << out_paths + 1 << ") " << CityArray[it.second]->name << " " << it.first << " (distance)" << endl;
			out_paths++;
			if (out_paths > max_outdegree - 1)
				break;
		}


		distances.clear();
	}
}