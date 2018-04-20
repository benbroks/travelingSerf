#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <set>

using namespace std;

vector<pair<long double, long double> > coords(ifstream& input){
	vector<pair<long double, long double> > coordinates;
	string line;
	string num;
	string garbage;
	stringstream ss;

	for(int a = 0; a < 14; a++){
		getline(input, line);
		ss << line;
		ss >> num;
		long double lat = atof(num.c_str());
		ss >> garbage;
		ss >> num;
		long double lon = atof(num.c_str());
		pair<long double,long double> loc = make_pair(lat, lon);
		coordinates.push_back(loc);
		ss.clear();
	}
	return coordinates;
}

void solutionGenerator(set<int>& discovered, vector<int>& path, vector< vector<long double>* > distances, int node, long double solDistance, long double allTimeMin, ofstream& output){
	if(discovered.size() == 13){
		solDistance += distances[node]->at(13);
		if(solDistance <= allTimeMin){
			allTimeMin = solDistance;
			//output smallest solution so far
			output << "Solution found" << endl;
			for(unsigned int a = 0; a < path.size(); a++){
				output << path[a] << " ";
			}
			output << endl;
			output << solDistance << endl;
		}
	}
	for(int i = 0; i < 13; i++){
		if(discovered.find(i) == discovered.end()){
			discovered.insert(i);
			path.push_back(i);
			solDistance += distances[node]->at(i);
			if(solDistance <= allTimeMin){
				solutionGenerator(discovered, path, distances, i, solDistance, allTimeMin, output);
			}
			discovered.erase(i);
			path.pop_back();
			solDistance -= distances[node]->at(i);
		}
	}
}


int main(int argc, char *argv[]){
	if(argc < 2){
		cout << "Input file necessary" << endl;
		return -1;
	}
	ifstream input(argv[1]);
	ofstream output("output.txt");
	vector<pair<long double,long double> > coordinates = coords(input);
	output.precision(10);

	//computing averages
	long double averageLong = 0;
	long double averageLat = 0;
	for(unsigned int i = 0; i < coordinates.size(); i++){
		averageLong += coordinates[i].second;
		averageLat += coordinates[i].first;
	}
	averageLat /= coordinates.size();
	averageLong /= coordinates.size();
	output << "Average Latitude: " << averageLat << endl;
	output << "Average Longitude: " << averageLong << endl;

	//creating cartesian coords
	//x = 2PIr/360*delta(long)*cos(avgLat)
	//y = 2PIr/360*delta(lat)
	vector<pair<long double,long double> > cart;
	long double r = 3958.8;
	for(unsigned int i = 0; i < coordinates.size(); i++){
		coordinates[i].first = coordinates[i].first - averageLat;
		coordinates[i].second = coordinates[i].second - averageLong;
		
		cout.precision(10);
		//x coord
		long double x = 2*M_PI*r*coordinates[i].second/360*cos(averageLat*M_PI/180);
		//y coord
		long double y = 2*M_PI*r*coordinates[i].first/360;
		pair<long double, long double> p = make_pair(x,y);
		cart.push_back(p);
		output << x << ", " << y << endl;
	}

	//initializing edge weights
	vector< vector<long double>* > distances;
	for(unsigned int a = 0; a < cart.size(); a++){
		vector <long double>* edgeLengths = new vector<long double>;
		for(unsigned int b = 0; b < cart.size(); b++){
			if(a == b){
				edgeLengths->push_back(0);
			}
			else {
				long double deltax = cart[a].first - cart[b].first;
				long double deltay = cart[a].second - cart[b].second;
				long double edge = sqrt(deltax*deltax + deltay*deltay);
				edgeLengths->push_back(edge);
			}
		}
		distances.push_back(edgeLengths);
	}

	//create bounds (greedily choose minimum edge from each node to create an upper bound)
	//13 refers to n-1 or 14 - 1
	long double maxDistance = 0;
	set<int> discovered;
	discovered.insert(13);
	int node = 13;
	while(discovered.size() < 13){
		long double smallest = 100;
		for(unsigned int a = 0; a < distances.size(); a++){
			//hasn't been discovered yet
			if(discovered.find(a) == discovered.end()){
				if(distances[node]->at(a) < smallest){
					smallest = distances[node]->at(a);
					node = a;
				}
			}
		}
		maxDistance += smallest;
		discovered.insert(node);
		cout << node << endl;
	}
	maxDistance += distances[node]->at(13);
	cout << maxDistance << endl;

	//generate every solution that is smaller than the upper bound

	vector<int> path;
	path.push_back(13);
	discovered.clear();
	long double solDistance = 0;
	long double allTimeMin = maxDistance;
	solutionGenerator(discovered, path, distances, 13, solDistance, allTimeMin, output);
	
	for(unsigned int i = 0; i < distances.size(); i++){
		delete distances[i];
	}


	return 1;
}