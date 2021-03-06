// Assignment_5.cpp : Fifth assignemnt of OOP in C++ course.
// written by Angus Plummer
//
// Simple class for complex numbers

#include "stdafx.h"
#include<iostream>
#include<cmath>
using namespace std;

class Complex
{
private:
	double re_, im_; // Trailing underscore to indicate these are member variables

public:
	// Default constructor creates 0
	Complex() { re_ = im_ = 0; }
	// Parameterised constructor for any a + ib
	Complex(double r, double i) { re_ = r; im_ = i; }
	// Destructor
	~Complex() {}

	// Return real component
	double get_im() const {
		return im_;
	}
	// Return imaginary component
	double get_re() const {
		return re_;
	}
	// Return modulus
	double modulus() const {
		return pow(re_*re_ + im_ * im_, 0.5);
	}
	// Return argument
	double arg() const {
		return atan2(im_, re_);
	}
	// Return complex conjugate
	Complex conjugate() const {
		Complex conj(re_, -im_);
		return conj;
	}
	// Overload + operator for addition 
	Complex operator+(const Complex c_num) const {
		Complex sum(re_ + c_num.re_, im_ + c_num.im_);
		return sum;
	}
	// Overload - operator for subtraction
	Complex operator-(const Complex c_num) const {
		Complex sub(re_ - c_num.re_, im_ - c_num.im_);
		return sub;
	}
	// Overload * operator for multiplication, z1*z2
	Complex operator*(const Complex c_num) const {
		Complex prod(re_ * c_num.re_ - im_ * c_num.im_, re_*c_num.im_ + im_ * c_num.re_);
		return prod;
	}
	// Overload / operator for division, z1/z2
	Complex operator/(const Complex c_num) const {
		// z1/z2 = (z1 x z2*) / ( |z2|^2 )
		Complex div(*this * c_num.conjugate());
		div.re_ /= pow(c_num.modulus(), 2.);
		div.im_ /= pow(c_num.modulus(), 2.);
		return div;
	}

	// Friend functions to overload operator<< and operator>>
	friend ostream & operator<<(ostream &os, const Complex &z);
	friend istream & operator>>(istream &os, Complex &z);

};

// Function to overload << operator for complex numbers
ostream & operator<<(ostream & os, const Complex & z){
	// if imaginary component is positive then just print separated by +
	if (z.im_ >= 0){
		os << z.re_ << " + " << z.im_ << "i";
	}
	// if imaginary component is negative then flip sign and print separated by -
	else {
		os << z.re_ << " - " << (z.im_ * -1.) << "i";
	}
	return os;
}

// Function to overload >> operator for complex numbers (must be in format a+ib or a-ib). Will leave z unchanged if input is bad
istream & operator>>(istream & is, Complex & z) {
	bool positive;
	double re;
	double im;
	
	is >> re; // first element is real component

	// if next char is + then im part is positive, if - then its negative. If neither then input format is not correct
	if (is.peek() == '+') {
		positive = true;
	}
	else if (is.peek() == '-') {
		positive = false;
	}
	else {
		cout << "Error: complex number format must be a+bi or a-bi. c = " << z << " is unchanged." << endl;
		return is; // if neither a + or - sign then quit
	}
	is.ignore(); // ignore sign

	/*// if next character is i then ignore it and read in
	if (is.peek() == 'i') { // a + ib
		is.ignore();
		is >> im;
	}
	// if next char is not i then read in
	else { // a + bi
	*/
		is >> im;
		// if final char is not i then input is wrong so quit
		if (is.peek() != 'i') {
			cout << "Error: complex number format must be a+bi or a-bi. c = " << z << " is unchanged." << endl;
			return is;
		//}
	}
	// if there was a negative sign then make imaginary component negative
	if (!positive) {
		im *= -1.;
	}

	// assign z its new values
	z.re_ = re;
	z.im_ = im;

	return is;
}

int main()
{
	// set precision for all cout to 3sf
	cout.precision(3);

	// Create two complex numbers
	Complex a(3, 0), b(0, -2);

	// Get real and imaginary components, modulus and argument
	cout << "For a = " << a << ":" << endl
		<< "Real component = " << a.get_re() << endl
		<< "Imaginary component = " << a.get_im() << endl
		<< "Modulus = " << a.modulus() << endl
		<< "Argument = " << a.arg() << " rad" << endl << endl;

	cout << "For b = " << b << ":" << endl
		<< "Real component = " << b.get_re() << endl
		<< "Imaginary component = " << b.get_im() << endl
		<< "Modulus = " << b.modulus() << endl
		<< "Argument = " << b.arg() << " rad" << endl << endl;

	// Get conjugates
	cout << "conjugate of " << a << ": " << a.conjugate() << endl;
	cout << "conjugate of " << b << ": " << b.conjugate() << endl << endl;


	// Get sum, difference, product and quotient of a and b
	cout << "Overloading operators:" << endl
		<< "a + b = " << a + b << endl
		<< "a - b = " << a - b << endl
		<< "a * b = " << a * b << endl
		<< "a / b = " << a / b << endl << endl;

	// Test input (>>) and print result to user
	Complex c;
	cout << "Please enter a complex number (a+ib or a-ib): ";
	cin >> c; // user input
	cout << "c = " << c << endl;
	// repeat 4 times
	for (int i(0); i < 4; i++) {
		cout << "Please enter a complex number (a+ib or a-ib): ";
		cin.clear(); // clear cin fail bit
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore rest of line of cin
		cin >> c;
		cout << "c = " << c << endl;
	}

	return 0;
}
