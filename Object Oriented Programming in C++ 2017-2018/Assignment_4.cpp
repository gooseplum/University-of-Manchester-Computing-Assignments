// Assignment_4.cpp : Fourth Assignment of OOP in C++ course.
// Written by Angus Plummer
//
// A simple demonstration of a class: a class for galaxies.
// Galaxies classified with following member data:
// Hubble types: E[0-7], S0, S[a-c], SB[a-c], Irr
// Redshift z in range [0,10]
// Total mass M_tot in range [1e7,1e12] M_sun
// Stellar mass fraction f_* in range [0,0.05]

// Questions for supervisors:
// Is there a better way to limit the values of member variables of a class to a specific range? The use of throwing errors seems long-winded

#include "stdafx.h"
#include<iostream>
#include<string>
#include<vector>
#include<cmath>

using namespace std;

// Class for galaxy objects
class Galaxy {
private:
	string hubbleType; // Hubble classification type of galaxy ( E[0-7], S0, S[a-c], SB[a-c], Irr )
	double redshift; // Range [0,10]
	double totalMass; // Range [1e7, 1e12] M_Sun
	double stellarMassFraction; // Range [0, 0.05]
	vector<Galaxy*> satellites; // Vector of pointers to satellites of galaxy (pointers so they contain original galaxies rather than copies)

public:

	// Default constructor (fills member variables with typical values in allowed ranges)
	Galaxy() : hubbleType{ "E0" }, redshift{ 0 }, totalMass{1e10}, stellarMassFraction{0.01} {}
	
	// Parameterised contructor
	Galaxy(string g_hubbleType, double g_redshift, double g_totalMass, double g_stellarMassFraction) {
		
		// Set hubble type
		// If valid hubble type then set.
		if (is_hubbleType(g_hubbleType)) {
			hubbleType = g_hubbleType;
		}
		// else throw invalid argument exception
		else {
			throw invalid_argument("Error: Invalid Hubble type: " + g_hubbleType);
		}

		// For all member doubles: if in allowed range then set, otherwise throw out_of_range error
		// Set redshift
		if (g_redshift <= 10. && g_redshift >= 0) {
			redshift = g_redshift;
		}
		else {
			throw out_of_range("Error: Redshift (" + to_string(g_redshift) + ") not in allowed range: [0,10]");
		}
		// Set mass
		if (g_totalMass <= 1e12 && g_totalMass >= 1e7) {
			totalMass = g_totalMass;
		}
		else {
			throw out_of_range("Error: Mass (" + to_string(g_totalMass) + ") not in allowed range: [1e7, 1e12] M_Sun");
		}
		// Set stellar mass fraction
		if (g_stellarMassFraction <= 0.05 && g_stellarMassFraction >= 0) {
			stellarMassFraction = g_stellarMassFraction;
		}
		else {
			throw out_of_range("Error: Stellar mass fraction (" + to_string(g_stellarMassFraction) + ") not in allowed range: [0,0.05]");
		}
	}

	// Destructor
	~Galaxy() {}//cout << "Destroying " << hubbleType << " galaxy" << endl;
	
	// Function to print data out to terminal (defined outside of class)
	void print_data();
	
	// Set hubble type (defined outside of class)
	void set_hubbleType(const string&);
	
	// Returns the stellar mass
	double stellar_mass() { return totalMass * stellarMassFraction; }
	
	// Function returns true if input string is a valid hubble classification type, else returns false
	static bool is_hubbleType(const string&);

	// Function to add galaxy to list of satellites. If a galaxy is its own satellites (or any of its satellites satellites and so on) then there will be problems...
	void add_satellite(Galaxy &satellite) {
		satellites.push_back(&satellite); // Add address to satellites (as is vector of pointers)
	}
};

void Galaxy::print_data() {
	cout.precision(3); // set sig figs
	// print data out to terminal
	cout << "Galaxy: [H type, z, M, f_*] = [" << hubbleType << ", " << redshift << ", " << totalMass << ", "
		<< stellarMassFraction << "], " << "#Satellites: " << satellites.size() << endl;
	if(satellites.size() > 0){
		cout << "Satellites: " << endl;
		for (auto iterator = satellites.begin(); iterator != satellites.end(); iterator++) {
			(*iterator)->print_data();
		}
	}
}

// Function to set set the Hubble type to a new value
void Galaxy::set_hubbleType(const string &newType) {
	// If input is a valid Hubble type then set to the input. Otherwise, print error statement and do nothing
	if (is_hubbleType(newType)) {
		hubbleType = newType;
	}
	else {
		cout << "Error: Invalid Hubble type: " << newType << endl;
	}
}

// Function to test if string is a valid hubble type
bool Galaxy::is_hubbleType(const string &test_str) {
	// If input string is any of the valid hubble types then return true, else return false
	if (test_str == "E0" || test_str == "E1" || test_str == "E2" || test_str == "E3" || test_str == "E4" 
		|| test_str == "E5" || test_str == "E6" || test_str == "E7" || test_str == "S0" || test_str == "Sa" 
		|| test_str == "Sb" || test_str == "Sc" || test_str == "SBa" || test_str == "SBb" || test_str == "SBc" 
		|| test_str == "Irr"){
		return true;
	}
	else {
		return false;
	}
}

// Function to create a new galaxy object with parametrized constructor and add to vector of galaxies
// Made this separate function to avoid many identical try catch loops in main()
void add_galaxy(vector<Galaxy> &galaxyList, string hubbleType, double redshift, double mass, double stellarFrac) {
	try {
		Galaxy g{ hubbleType, redshift, mass, stellarFrac };
		galaxyList.push_back(g);
	}
	// If there is an error when contructing new galaxy object then output error and do not add to vector
	catch (invalid_argument err) {
		cerr << err.what() << endl;
	}
	catch (out_of_range err) {
		cerr << err.what() << endl;
	}
}

int main(){
	vector<Galaxy> galaxies; // Vector to hold the galaxies

	// Create galaxy with default contructor and print out data
	cout << endl << "Default constructor: " << endl;
	Galaxy defaultGalaxy{};
	defaultGalaxy.print_data();

	// Attempt to change to Spiral but will not work
	cout << endl << "Try to set Hubble type \"Spiral\":" << endl;
	defaultGalaxy.set_hubbleType("Spiral");
	defaultGalaxy.print_data();

	// Change to SBa which will work as is a valid hubble type
	cout << endl << "Try to set Hubble type \"SBa\":" << endl;
	defaultGalaxy.set_hubbleType("SBa");
	defaultGalaxy.print_data();

	// Print stellar mass
	cout << endl << "Stellar Mass = " << defaultGalaxy.stellar_mass() << endl;
	
	// Adding two satellite galaxies and print
	cout << endl << "Add two satellite galaxies:" << endl;
	Galaxy satelliteGalaxy1{ "Irr", 2, 8e7, 0.04 };
	Galaxy satelliteGalaxy2{ "E4", 5, 2e7, 0.03 };
	defaultGalaxy.add_satellite(satelliteGalaxy1);
	defaultGalaxy.add_satellite(satelliteGalaxy2);
	defaultGalaxy.print_data();
	// Alter one of the satellites and print again
	cout << endl << "Alter the second satellite:" << endl;
	satelliteGalaxy2.set_hubbleType("S0");
	defaultGalaxy.print_data();

	// Add new galaxies to vector using add_galaxy function defined above to avoid repitition of try-catch blocks
	cout << endl << "Create and add 4 galaxies to vector of galaxies (with 1 having an invalid constructor):" << endl;
	add_galaxy(galaxies, "Irr", 3, 4.75e8, 0.02);
	add_galaxy(galaxies, "E3", 1, 1.2e9, 0.05);
	add_galaxy(galaxies, "S0", 9.4, 0.4e10, 0.5); // will fail as stellar mass fraction too large
	add_galaxy(galaxies, "SBb", 5, 5e11, 0.005);


	// Print out sorted list of galaxies
	cout << endl << "Galaxies added to vector of galaxies: " << endl;
	for (auto iterator = galaxies.begin(); iterator != galaxies.end(); iterator++) {
		iterator->print_data();
	}

    return 0;
}

