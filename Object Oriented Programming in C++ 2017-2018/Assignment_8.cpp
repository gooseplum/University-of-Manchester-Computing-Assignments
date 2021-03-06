// Assignment_8.cpp : Eigth assignemnt of OOP in C++ course.
// written by Angus Plummer
//
// A class heirarchy for shapes

// Questions for demonstrators:
// Is my method of having an info() function acceptable or is the data of each shape in the array supposed to be shown through different means?

#include "stdafx.h"
#include<cmath>
#include<iostream>

const double pi = 3.141592653589793238462643383279502884; // Define pi

// Abstract base class for shapes
class Shape {
public:
	// ctor
	Shape() {}
	// virtual dtor
	virtual ~Shape() {}
	// pure virtual functions for area and volume of shapes
	virtual double area() const = 0;
	virtual double volume() const = 0;
	// pure virtual function for printing out info on the shape
	virtual void info() const = 0;
};

// Abstract derived class for 2D shapes. Doesn't contain width and height so that can add stranger 2D shapes in future
class Shape2D : public Shape {
public:
	Shape2D() {}
	// 2D shapes have zero volume
	double volume() const { return 0; } 
	// virtual function for perimeter of 2D shapes
	virtual double perimeter() const = 0;
};

// Derived class for rectangles
class Rectangle : public Shape2D {
protected:
	// member variables for dimensions of rectangle
	double width_;
	double height_;
public:
	// ctor takes width and height
	Rectangle(double const width, double const height) : width_(width), height_(height) {};
	// dtor prints out message when called
	~Rectangle() { std::cout << "Rectangle destructor called" << std::endl; }
	// area of rectangle
	double area() const { return width_ * height_; }
	// perimeter of rectangle
	double perimeter() const { return 2 * width_ + 2 * height_;}
	// output info of Rectangle
	void info() const {
		std::cout << "Rectangle of dimensions " << width_ << "x" << height_ << ": area = " << area() << std::endl;
	}
};

// Derived class for squares (special case of rectangle)
class Square : public Rectangle {
public:
	// ctor: square is a rectangle with both dimensions the same length
	Square(double const size) : Rectangle(size, size) {};
	// dtor prints out message when called
	~Square() { std::cout << "Square destructor called" << std::endl; }
	// output info of Square
	void info() const {
		std::cout << "Square of dimensions " << width_ << "x" << height_ << ": area = " << area() << std::endl;
	}
};
// Derived class for ellipses
class Ellipse : public Shape2D {
protected:
	// member variables for semiminor and semimajor axes. Called width and height as don't depend on which is larger
	double width_;
	double height_;
public:
	// ctor takes width and height as args
	Ellipse(double const width, double const height) : width_(width), height_(height) {};
	// dtor prints out message when called
	~Ellipse() { std::cout << "Ellipse destructor called" << std::endl; }
	// area of ellipse pi * ab for semiminor and semimajor a and b
	double area() const { return pi * width_ * height_; }
	// circumference of an ellipse. Exact answer requires summing an infinite series so just use one of Ramanujans approximations
	// p ~= pi * [ 3(a+b) - sqrt[(3a+b)(a+3b)] ]
	double perimeter() const {
		double component = pow((3 * width_ + height_)*(width_ + 3 * height_), 0.5); // sqrt[(3a+b)(a+3b)]
		return pi * (3 * (width_ + height_) - component); // pi * [ 3(a+b) - component]
	}
	// output info of Ellipse
	void info() const {
		std::cout << "Ellipse of dimensions " << width_ << "x" << height_ << ": area = " << area() << std::endl;
	}
};

// Derived class for circles (special case of ellipse)
class Circle : public Ellipse {
public:
	// ctor: circle is an ellipse with both dimensions the same length
	Circle(double const radius) : Ellipse(radius, radius) {};
	// dtor prints out message when called
	~Circle() { std::cout << "Circle destructor called" << std::endl; }
	// override perimeter fucntion as dont need to use approximation
	double perimeter() const { return 2 * pi * width_; }
	// output info of Circle
	void info() const {
		std::cout << "Circle of radius " << width_ << ": area = " << area() << std::endl;
	}
};

// Abstract derived class for 3D shapes
class Shape3D : public Shape {
public:
	// ctor
	Shape3D() {}
};

// Derived class for cuboids
class Cuboid : public Shape3D {
protected:
	// member variables for size in each of three dimensions
	double width_;
	double height_;
	double depth_;
public:
	// ctor takes width, height, depth as args
	Cuboid(double const width, double const height, double depth) : width_(width), height_(height), depth_(depth) {};
	// dtor prints out message when called
	~Cuboid() { std::cout << "Cuboid destructor called" << std::endl; }
	// area is surface area of cuboid (2wh + 2wd + 2hd for sides w,h,d)
	double area() const { return (2 * width_ * height_) + (2 * width_ * depth_) + (2 * height_ * depth_);}
	// volume is whd for sides w,h,d
	double volume() const { return width_ * height_ * depth_; }
	// output info of Cuboid
	void info() const {
		std::cout << "Cuboid of dimensions " << width_ << "x" << height_ << "x" << depth_ << ": surface area = " << area() << ", volume = " << volume() << std::endl;
	}
};

// Derived class for cubes (special case of cuboid)
class Cube : public Cuboid {
public:
	// ctor: cube is cuboid with length the same in all three dimensions
	Cube(double const size) : Cuboid(size, size, size) {};
	// dtor prints out message when called
	~Cube(){ std::cout << "Cube destructor called" << std::endl; }
	// output info of Cube
	void info() const {
		std::cout << "Cube of dimensions " << width_ << "x" << height_ << "x" << depth_ << ": surface area = " << area() << ", volume = " << volume() << std::endl;
	}
};

// Derived class for ellipsoids
class Ellipsoid : public Shape3D {
protected:
	// member variables for size in each of three dimensions
	double width_;
	double height_;
	double depth_;
public:
	// ctor takes width, height, and depth as args
	Ellipsoid(double width, double height, double depth) : width_(width), height_(height), depth_(depth) {};
	// dtor prints out message when called
	~Ellipsoid() { std::cout << "Ellipsoid destructor called" << std::endl; }
	// volume of ellipsoid (4/3 * pi * abc for dimensions a,b,c)
	double volume() const { return (4. / 3.) * pi * width_ * height_ * depth_; }
	// surface area of ellipsoid is actually complex to solve. Instead use an approximation:
	// S = 4pi * ( ( (ab)^p) + (ac)^p + (bc)^p ) / 3 )^(1/p)  (p = 1.6075) , yields error of at most 1.061%
	double area() const {
		// const to hold the power in calculation
		const double power(1.6075);
		// component of calculation: (ab)^p) + (ac)^p + (bc)^p )
		double component = pow(width_*height_, power) + pow(width_ * depth_, power) + pow(height_ * depth_, power);
		// plug in to rest of calculation
		return 4 * pi * pow(component/3., 1./power);
	}
	// output info of Ellipsoid
	void info() const {
		std::cout << "Ellipsoid of dimensions " << width_ << "x" << height_ << "x" << depth_ << ": surface area = " << area() << ", volume = " << volume() << std::endl;
	}
};

// Derived class for spheres (special case of ellipsoid)
class Sphere : public Ellipsoid {
public:
	// ctor: sphere is ellipsoid with length the same in all three dimensions
	Sphere(double radius) : Ellipsoid(radius, radius, radius) {};
	// dtor prints out message when called
	~Sphere() { std::cout << "Sphere destructor called" << std::endl; }
	// override the area function as do not need to use approximation. S = 4 * pi * r^2
	double area() const { return 4. * pi * width_ * width_; } // width_ = radius
	// output info of Sphere
	void info() const {
		std::cout << "Sphere of radius " << width_ << ": surface area = " << area() << ", volume = " << volume() << std::endl;
	}
};

// Derived class for Prisms
class Prism : public Shape3D {
protected:
	double depth_;
	Shape2D* base_;
public:
	// ctor takes the length of the prism and the 2D shape that defines its end faces
	Prism(double depth, Shape2D* base) : depth_(depth), base_(base) {};
	// dtor deletes base shape and prints out message when called
	~Prism() { 
		delete base_;
		std::cout << "Prism destructor called" << std::endl; 
	} 
	// volume of prism is area of main face * length
	double volume() const { return base_->area() * depth_; }
	// area of prism is 2 * area of end faces + length * perimeter of end faces
	double area() const { return 2 * base_->area() + base_->perimeter() * depth_; }
	// output info of Prism
	void info() const {
		// prints out info of base and then for whole prism
		std::cout << "Prism of length " << depth_ << " with base: ";
		base_->info();
		std::cout << " -> " << ": prism surface area = " << area() << ", prism volume = " << volume() << std::endl;
	}
};


// main function to demonstrate the classes working correctly
int main(){
	// number of shapes
	int const num_shapes(9);
	// array to hold one of each type of shape
	Shape** shape_array = new Shape*[num_shapes];
	// 2D shapes
	shape_array[0] = new Rectangle(1, 2);
	shape_array[1] = new Square(3);
	shape_array[2] = new Ellipse(1, 2);
	shape_array[3] = new Circle(2);
	// 3D shapes
	shape_array[4] = new Cuboid(1, 2, 3);
	shape_array[5] = new Cube(2);
	shape_array[6] = new Ellipsoid(1, 2, 3);
	shape_array[7] = new Sphere(2);
	shape_array[8] = new Prism(2, new Rectangle(1, 3)); // prism with rectangle base
	// iterate through array and print out the area and volume of each shape
	for (int i = 0; i < num_shapes; i++) {
		shape_array[i]->info();
		std::cout << std::endl;
	}
	// --- clean-up ---
	// iterate through array and delete each element
	for (int i = 0; i < num_shapes; i++) {
		delete shape_array[i];
	}
	// delete the shape array
	delete[] shape_array;

    return 0;
}

