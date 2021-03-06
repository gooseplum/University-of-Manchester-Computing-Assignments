// Assignment_7.cpp : Seventh assignemnt of OOP in C++ course.
// written by Angus Plummer
//
// A class for n dimensional vectors
// A class for 4-vectors that inherits from the n dimensional vector class
// A particle class

// Questions for demonstrators:
// For some reason the inherited move constructors and assignment operators function differently to the originals. Why?
#include "stdafx.h"
#include<cmath>
#include<iostream>

using namespace std;

class Vector {

protected:
	// Member data
	int dimensions_;
	double *vData_;
public:
	// Default ctor. 0D null vector
	Vector() : dimensions_(0), vData_(nullptr) {}
	// Parametrised ctor (intialises as n dimensional array of zeros)
	Vector(int const dim) : dimensions_(dim), vData_(new double[dim]{}) {}
	// Copy ctor (deep)
	Vector(Vector const &vec) : dimensions_(vec.dimensions_) {
		vData_ = new double[dimensions_];
		// Deep copy over data from vec
		for (int i = 0; i < dimensions_; i++) {
			vData_[i] = vec.vData_[i];
		}
	}
	// Move ctor. Takes Rvalue vector, moves over data to new vector and sets vec to be 0D null vector
	Vector(Vector &&vec) : dimensions_(vec.dimensions_), vData_(vec.vData_) {
		vec.dimensions_ = 0;
		vec.vData_ = nullptr;
	}
	// Dtor. Deletes array of vector components
	~Vector() { delete[] vData_; }

	// Returns number of dimensions
	int getdimensions() const { return dimensions_; }
	// Returns magnitude of matrix
	double magnitude() const {
		double result = 0; // temp variable to hold sum
		// iterate over components and use N-D pythagoras theorem to get magnitude
		for (int i = 0; i < dimensions_; i++) {
			result += vData_[i] * vData_[i];
		}
		return pow(result, 0.5);
	}
	// Returns nth element of vector
	double & operator[](int const n) const {
		// If within bounds then return
		if (n >= 0 && n < dimensions_) { return vData_[n]; }
		// Else print error message and exit
		else {
			cout << "Error: Element " << n << " out of range.\n";
			exit(1);
		}
	}
	// Copy  Assignment operator
	Vector& operator=(Vector const &vec);

	// Move Assignment operator
	Vector& operator=(Vector&& vec);

	// Dot product
	double dot(Vector const &vec) const;

	// friend function for output to stream overloading
	friend ostream& operator<<(ostream &os, Vector const &vec);
};

// Copy assigment operator
Vector& Vector::operator=(Vector const &vec){
	// If self assigned then do nothing and return *this
	if (&vec == this) {
		return *this;
	}
	// Delete vector components and copy dimensions
	delete[] vData_;
	dimensions_ = vec.dimensions_;
	// Create new components with new dimensions and deep copy over
	vData_ = new double[dimensions_];
	for (int i = 0; i < dimensions_; i++) {
		vData_[i] = vec.vData_[i];
	}
	return *this;
}

// Move assignment operator
Vector& Vector::operator=(Vector&& vec) {
	// If not self assigned then swap data
	if (this != &vec) {
		// Swap all member data
		swap(dimensions_, vec.dimensions_);
		swap(vData_, vec.vData_);
	}
	return *this;
}

// Dot product
double Vector::dot(Vector const &vec) const {
	// If vectors have same dimension then calc and return dot product
	if (dimensions_ == vec.dimensions_) {
		double result = 0; // temp variable for holding sum
		for (int i = 0; i < dimensions_; i++) {
			result += vData_[i] * vec[i];
		}
		return result;
	}
	// otherwise print error message and quit
	else {
		cout << "Error: Different matrix dimensions (" << dimensions_ << " =/= " << vec.dimensions_ << ")\n";
		exit(1);
	}
}

// Overload insertion to output stream for matrices
ostream& operator<<(ostream &os, Vector const &vec) {
	// If vector has non zero dimensions..
	if (vec.dimensions_ > 0) {
		cout << '(';
		// Print out each row with commas between elements
		for (int i = 0; i < vec.dimensions_ - 1; i++) {
			cout << vec[i] << ",";
		}
		// last element has bracket close after instead of comma
		cout << vec[vec.dimensions_ - 1] << ")";
	}
	// If vector has zero dimension then print "null vector"
	else {
		cout << "null vector";
	}
	return os;
}

// 4-Vector class. Inherits from general vector class
class Vector4 : public Vector {
public:
	// Default ctor. Makes 4D vector of zeros
	Vector4() : Vector(4) {}
	// Parameterised constructor 1: takes time component and a 3 vector
	Vector4(double const ct, Vector const r) : Vector(4) {
		// If argument vector is 3D then copy in data
		if (r.getdimensions() == 3) {
			vData_[0] = ct;
			vData_[1] = r[0];
			vData_[2] = r[1];
			vData_[3] = r[2];
		}
		// If not 3D then print error and exit
		else {
			cout << "Error: Constructing 4 vector from " << r.getdimensions() << " dimensional vector.\n";
			exit(1);
		}
	}
	// Parameterised constructor 2: takes all 4 components as args
	Vector4(double const ct, double const x, double const y, double const z) : Vector(4) {
		vData_[0] = ct;
		vData_[1] = x;
		vData_[2] = y;
		vData_[3] = z;
	}
	// Dot product. (ct)^2 - r^2
	double dot(Vector4 const &vec) const {
		double result = 0; // to hold result as summed up
		result += vData_[0] * vec[0]; // (ct)^2
		for (int i = 1; i < 4; i++) {
			result -= vData_[i] * vec[i]; //(r_i)^2
		}
		return result;
	}
	// Lorentz boost
	Vector4 boost(Vector const &beta) const {
		// boost vector must be 3D and have mag less than 1
		if (beta.getdimensions() == 3 && beta.magnitude() < 1.) {
			// If magnitude of boost is 0 then return copy of (this) vector (avoids division by zero)
			if (beta.magnitude() == 0) {
				Vector4 boosted(*this); // copy of (this)
				return boosted;
			}
			// Components of 4-vector to be boosted
			double ct = vData_[0];
			Vector r(3); // space components as 3 vector to do dot products
			r[0] = vData_[1];
			r[1] = vData_[2];
			r[2] = vData_[3];
			// Resulting boosted 4-vector
			Vector4 boosted;
			// gamma = 1 / sqrt(1 - B^2)
			double gamma = pow(1 - pow(beta.magnitude(), 2.), -0.5);
			// ct' = gamma * (ct - B.r)
			boosted[0] = gamma * (ct - beta.dot(r));
			// r' = r + ( (gamma-1)(r.B/B^2) - gamma*ct ) * B
			for (int i = 0; i < 3; i++) {
				boosted[i + 1] = r[i] + ((gamma - 1.) * r.dot(beta) * pow(beta.magnitude(), -2.) - gamma * ct) * beta[i];
			}
			return boosted;
		}
		else {
			// magnitude >= 1 then print error and quit
			if (beta.magnitude() >= 1.){
				cout << "Error: Lorentz boost with v >= c.\n";
				exit(1);
			}
			// If B isnt 3D then print error and quit
			else {
				cout << "Error: Lorentz boost with " << beta.getdimensions() << " dimensional vector\n";
				exit(1);
			}
		}
	}
};

// Particle class.
class Particle {
private:
	// Member variables
	Vector4 position_;
	double mass_;
	Vector beta_;
public:
	// Parameterised ctor takes position, mass (in energy units MeV/c^2), and velocity
	Particle(Vector4 const pos, double const mass, Vector const beta) : position_(pos) { // consts as deep copies anyway
		// mass cannot be negative
		if (mass >= 0) {
			mass_ = mass;
		}
		// if negative then print wrror message and quit
		else {
			cout << "Error: Particle with negative mass (" << mass << ")\n";
			exit(1);
		}
		// velocity cannot exceed c (beta=1)
		if (beta.magnitude() <= 1) {
			beta_ = beta;
		}
		// if v>c print error message and quit
		else {
			cout << "Error: Particle velocity greater than c";
			exit(1);
		}
		// massless particles must have v=c, massive particles cannot have v=c
		// if wrong then print error message and quit
		if ((beta.magnitude() == 1 && mass != 0) || (mass == 0 && beta.magnitude() != 1)) {
			cout << "Error: m=0 <=> v=c not held\n";
			exit(1);
		}
	}
	// return the gamma factor
	double gamma() const {
		// gamma = 1 / sqrt(1-B^2)
		return pow(1 - pow(beta_.magnitude(), 2.), -0.5);
	}
	// returns the energy E = gamma * mc^2
	double energy() const {
		return gamma() * mass_;
	}
	// returns the momentum p = gamma * mv in energy units (MeV/c)
	double momentum() const {
		return gamma() * mass_ * beta_.magnitude();
	}

};


int main(){
	cout << endl;// spacing

	// N-D Vector class
	cout << "--------N-D Vectors:--------" << endl << endl;
	
	// Default constructor
	cout << "Default constructor:" << endl << "Vector v1; ->" << endl;
	Vector v1;
	cout << "v1 = " << v1 << endl;
	cout << endl;

	// Parameterised constructor
	cout << "Parameterised constructor:" << endl << "Vector v2(3); ->" << endl;
	Vector v2(3);
	cout << "v2 = " << v2 << endl;
	cout << "Fill with values ->" << endl;
	for (int i = 0; i < v2.getdimensions(); i++){
		v2[i] = i*i;
	}
	cout << "v2 = " << v2 << endl;
	cout << endl;

	// Copy constructor
	cout << "Copy constructor:" << endl << "Vector v3(v2); ->" << endl;
	Vector v3(v2);
	cout << "v3 = " << v3 << endl;
	cout << "v3[0]=10; ->" << endl;
	v3[0] = 10;
	cout << "v2 = " << v2 << endl;
	cout << "v3 = " << v3 << endl;
	cout << endl;

	// Copy assigment
	cout << "Copy assignment:" << endl << "v3 = v2; ->" << endl;
	v3 = v2;
	cout << "v2 = " << v2 << endl;
	cout << "v3 = " << v3 << endl;
	cout << endl;

	// Move constructor
	cout << "Move constructor:" << endl << "Vector v4(move(v3)); ->" << endl;
	Vector v4(move(v3));
	cout << "v3 = " << v3 << endl;
	cout << "v4 = " << v4 << endl;
	cout << endl;

	// Move assignment
	cout << "Move assignment:" << endl << "v3 = move(v4); ->" << endl;
	v3 = move(v4);
	cout << "v3 = " << v3 << endl;
	cout << "v4 = " << v4 << endl;
	cout << endl;

	// Accessing vector contents
	cout << "Accessing vector components:" << endl;
	cout << "v2[2] = " << v2[2] << endl;
	cout << endl;

	// Dot product
	cout << "Dot product:" << endl;
	for (int i = 0; i < v3.getdimensions(); i++){
		v3[i] = i+1; 	// Fill v3 with new values
	}
	cout << "v2 = " << v2 << endl;
	cout << "v3 = " << v3 << endl;
	cout << "v2.dot(v3) = " << v2.dot(v3) << endl;
	cout << endl;

	// ostream output. Already demonstrated.

	// Minkowski 4-Vectors
	cout << "--------Minkowski 4-Vectors:--------" << endl << endl;
	
	// Default constructor
	cout << "Default constructor:" << endl << "Vector4 vec4; ->" << endl;
	Vector4 vec4a;
	cout << "vec4a = " << vec4a << endl;
	cout << endl;

	// Parameterised constructors
	cout << "Parameterised constructors:" << endl;
	cout << "Vector4 vec4b(1,-1,1,-1); ->" << endl;
	Vector4 vec4b(1, -1, 1, -1);
	cout << "vec4b = " << vec4b << endl;
	cout << "v2 = " << v2 << endl;
	cout << "Vector4 vec4c(2, v2); ->" << endl;
	Vector4 vec4c(2, v2);
	cout << "vec4c = " << vec4c << endl;
	cout << endl;

	// Copy constructor
	cout << "Copy constructor:" << endl << "Vector4 vec4d(vec4c); ->" << endl;
	Vector4 vec4d(vec4c);
	cout << "vec4d = " << vec4d << endl;
	cout << "vec4d[0]=10; ->" << endl;
	vec4d[0] = 10;
	cout << "vec4c = " << vec4c << endl;
	cout << "vec4d = " << vec4d << endl;
	cout << endl;

	// Copy assigment
	cout << "Copy assignment:" << endl << "vec4d = vec4c; ->" << endl;
	vec4d = vec4c;
	cout << "vec4c = " << vec4c << endl;
	cout << "vec4d = " << vec4d << endl;
	cout << endl;

	// Move constructor
	cout << "Move constructor:" << endl << "Vector4 vec4e(move(vec4d)); ->" << endl;
	Vector4 vec4e(move(vec4d));
	cout << "vec4d = " << vec4d << endl;
	cout << "vec4e = " << vec4e << endl;
	cout << endl;

	// Move assignment
	cout << "Move assignment:" << endl << "vec4d = move(vec4e); ->" << endl;
	vec4d = move(vec4e);
	cout << "vec4d = " << vec4d << endl;
	cout << "vec4e = " << vec4e << endl;
	cout << endl;

	// Accessing vector contents
	cout << "Accessing vector components:" << endl;
	cout << "vec4b = " << vec4b << endl;
	cout << "vec4b[2] = " << vec4b[2] << endl;
	cout << endl;

	// Dot product
	cout << "Dot product:" << endl;
	cout << "vec4b = " << vec4b << endl;
	cout << "vec4c = " << vec4c << endl;
	cout <<"vec4b.dot(vec4c) = " << vec4b.dot(vec4c) << endl;
	cout << endl;

	// Lorentz boost
	cout << "Lorentz boost:" << endl;
	Vector beta(3);
	beta[0] = 0.8;
	cout << "vec4b = " << vec4b << endl;
	cout << "beta = " << beta << endl;
	cout << "Vector4 vboost(vec4b.boost(beta)); ->" << endl;
	Vector4 vboost(vec4b.boost(beta));
	cout << "vboost = " << vboost << endl;
	cout << endl;

	// Particles
	cout << "--------Particles:--------" << endl << endl;
	cout << "position = " << vec4b << endl;
	cout << "p_mass = " << 938.27231 << endl;
	cout << "beta = " << beta << endl;
	cout << endl;
	cout << "Particle proton(position, p_mass, beta); ->" << endl;
	Particle proton(vec4b, 938.27231, beta);
	cout << endl;
	cout << "proton.gamma() = " << proton.gamma() << endl
		<< "proton.energy() = " << proton.energy() << " MeV." << endl
		<< "proton.momentum() = " << proton.momentum() << " MeV/c." << endl;
	cout << endl;

	return 0;
}
