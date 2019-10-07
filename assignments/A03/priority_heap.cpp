//****************************************************************
// Sabin Dheke
// Assignment 2
// CMPS 3013
// Professor Terry Griffin
// Due Date: 10/07/2019
// This project is based on priority queues. In this project, 
// we used the binary heap for data structure, we read the data file 
// from the "animals.json" file and calcuate the priority and print 
// the result in the "output.txt" file. We calcuate the priority
// based on the information from json file, like length of the animal
// names, their latitude and longitude, adjustor and validated.
//****************************************************************

// most part of the code is provided by Dr. Griffin
// made some changes in the code as per requirment

#include "json_helper.cpp"
#include <fstream>
#include <iostream>
#include <string>



#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;

// constant no. for radius of the earth 
const static double EarthRadiusKm = 6372.8;

inline double DegreeToRadian(double angle) {
	return M_PI * angle / 180.0;
}


class Coordinate {
public:
	Coordinate(double latitude, double longitude) : myLatitude(latitude), myLongitude(longitude) {}

	double Latitude() const {
		return myLatitude;
	}

	double Longitude() const {
		return myLongitude;
	}

private:
	double myLatitude;
	double myLongitude;
};

double HaversineDistance(const Coordinate &p1, const Coordinate &p2) {
	double latRad1 = DegreeToRadian(p1.Latitude());
	double latRad2 = DegreeToRadian(p2.Latitude());
	double lonRad1 = DegreeToRadian(p1.Longitude());
	double lonRad2 = DegreeToRadian(p2.Longitude());

	double diffLa = latRad2 - latRad1;
	double doffLo = lonRad2 - lonRad1;

	double computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
	return 2 * EarthRadiusKm * computation;
}

struct Animal {
	string animal_name;
	long date;
	double latitude;
	double longitude;
	float adjuster;
	bool validated;
	string version;
	float priority;

	Animal() {
		animal_name = "";
		date = 0;
		latitude = 0.0;
		longitude = 0.0;
		priority = 0.0;
		validated = 0;
		version = "";
		adjuster = 0.0;
	}

	Animal(string name, long _date, double lat, double lon, float adj, bool val, string ver) {
		animal_name = name;
		date = _date;
		latitude = lat;
		longitude = lon;
		//priority = pri;
		validated = val;
		version = ver;
		adjuster = adj;
		priority = 0.0; 
	}

	Animal(json j) {
		animal_name = j["animal_name"];
		date = j["date"];
		latitude = j["latitude"];
		longitude = j["longitude"];
		//priority = j["priority"];
		validated = j["validated"];
		version = j["version"];
		adjuster = j["adjuster"];
	    priority = 0.0;
		
	}
	bool operator<(Animal& x) {
		return this->priority < x.priority;
	}

	// calcuating the priority queues
	void calculatePriority() {
		// coordinate for latitude and longitude 
		Coordinate x(latitude, longitude);
		Coordinate y(33.9137, -98.4934);
		
		// formula for distance
		double d = HaversineDistance(x, y);

		// formula for priority
		this->priority = (6372.8 - d) * adjuster / animal_name.size();
		// if the validated is false
		this->priority = (validated) ? -1 * priority  : priority;
		//if (this->validated == true) this->priority = this->priority * -1; 
	}
};

class AnimalHelper {
private:
	Animal **Animals;
	JsonHelper *J;
	json obj;
	int size;

public:
	AnimalHelper(string filename) {
		J = new JsonHelper(filename);
		size = J->getSize();
		Animals = new Animal *[size];
		for (int i = 0; i < size; i++) {
			obj = J->getNext();
			Animals[i] = new Animal(obj);
			// calloing the calculate Priority
			Animals[i]->calculatePriority();
		}
	}
	Animal** getAnimalsArray() {
		return Animals;
	}
	int getAnimalsArraySize() {
		return size;
	}

	void PrintAnimals() {

		for (int i = 0; i < size; i++) {

			cout << Animals[i]->animal_name << " "
				<< Animals[i]->latitude << " "
				<< Animals[i]->longitude << " "
				<< Animals[i]->date << " "
				<< Animals[i]->adjuster << " "
				<< Animals[i]->validated << " "
				<< Animals[i]->version <<" "
				<< Animals[i]->priority << endl;
		
		}
	}
};

class BHeap {
private:
	vector <Animal> heap;
	int left(int parent);
	int right(int parent);
	int parent(int child);
	void heapifyUp(int index);
	void heapifyDown(int index);
public:
	BHeap() {}
	void Insert(Animal element);
	void DeleteMax();
	Animal getMax();
	void print();
	int size();
};

int BHeap::size() {
	return heap.size();
}

void BHeap::Insert(Animal ele) {
	heap.push_back(ele);
	heapifyUp(heap.size() - 1);
}

// when deleting the heap
// grap the root
// swap with the element of the index
// decrement index
void BHeap::DeleteMax() {
	if (heap.size() == 0) {
		return;
	}
	heap[0] = heap.at(heap.size() - 1);
	heap.pop_back();
	heapifyDown(0);
}
Animal BHeap::getMax() {
	if (heap.size() == 0) {
		return Animal();
	}
	else
		return heap[0];
}

//leftChild(n) = 2n
//rightChild(n) = 2n + 1
//parent(n) = n / 2

// print the heap based on priority
void BHeap::print() {
	for (int i = 0; i < heap.size(); i++) {
		cout << heap[i].animal_name << " " << heap[i].priority << ",";
	}
	cout << endl;
}

//leftChild(n) = 2n
int BHeap::left(int parent) {
	int lnode = 2 * parent + 1;
	if (lnode < heap.size())
		return lnode;
	else
		return -1;
}

//rightChild(n) = 2n + 1
int BHeap::right(int parent) {
	int rNode = 2 * parent + 2;
	if (rNode < heap.size())
		return rNode;
	else
		return -1;
}

//parent(n) = n / 2
int BHeap::parent(int child) {
	int p = (child - 1) / 2;
	if (child <= 0)
		return -1;
	else
		return p;
}
void BHeap::heapifyUp(int in) {
	
	 

	if (in > 0 && heap[parent(in)] < heap[in]) {
		Animal temp = heap[in];
		heap[in] = heap[parent(in)];
		heap[parent(in)] = temp;
		heapifyUp(parent(in));
	}
}

void BHeap::heapifyDown(int in) {
	int child = left(in);
	int child1 = right(in);
	if (child >= 0 && child1 >= 0 && heap[child] < heap[child1]) {
		child = child1;
	}
	if (child > 0 && heap[in] < heap[child]) {
		Animal t = heap[in];
		heap[in] = heap[child];
		heap[child] = t;
		heapifyDown(child);
	}
}


int calculateBucket(Animal& anim, int numberBuckets) {
	return abs(anim.date) % numberBuckets;
}


int main(int argc, char **argv) {

	// open the file 
	ofstream outfile;
	outfile.open("output.txt");
	{

		int numberOfBuckets = atoi(argv[1]);
		BHeap**heaps = new BHeap*[numberOfBuckets];
		for (int i = 0; i < numberOfBuckets; i++) {
			heaps[i] = new BHeap();
		}
		AnimalHelper AH("animals.json");
		for (int i = 0; i < AH.getAnimalsArraySize(); i++) {
			heaps[calculateBucket(*AH.getAnimalsArray()[i], numberOfBuckets)]->Insert(*AH.getAnimalsArray()[i]);
		}
		int heapSize;
		// print the output in the file
		for (int i = 0; i < numberOfBuckets; i++) {
			outfile << "\n" << "Heap " << i << endl;
			outfile << "===================" << endl;
			heapSize = heaps[i]->size();

			// it create no. of animals in certain heap 
			for (int j = 0; j < 5; j++)
			{
				outfile << heaps[j]->getMax().animal_name << " " << heaps[j]->getMax().priority << endl;

				heaps[j]->DeleteMax();
			}
			cout << endl;


		}

		//AnimalHelper AH("animals.json");
		//char ch;
		//AH.PrintAnimals();
		
		// close the file
		outfile.close();
		
	}

	system("pause");

	return 0;
}
