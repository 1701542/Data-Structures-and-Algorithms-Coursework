#pragma once
#include <SFML/Graphics.hpp>
#include <complex>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
typedef std::chrono::steady_clock the_clock;

///
/*
Mandelbrot.h handles calculation of the
mandelbrot image. It holds the relevant 
variables required to display and 
manipulat it. For the set to be drawn it
needs to become a texture which is
applied to a polygon shape. An image was
used to hold information on mandelbrot,
was turned into a texture, then a rectangle
shape was textured with it.
*/
///
class Mandelbrot
{
public:

	//	The init function assigns variable to each
	//	other and declares the qualities of the
	//	pixel in the centre of the screen.
	Mandelbrot(sf::RenderWindow* window);
	~Mandelbrot();

	//	Returns the already textured rectangle shape.
	sf::RectangleShape GetImage();

	//	Divides image into slices and assigns threads to calculate them.
	//	After joining the threads the image is turned into texture then applied to the rectangle.
	void CalculateSet(double left_, double right_, double top_, double bottom_);

private:

	//	Calculates mandelbrot as well as determines
	//	the colour of the pixels based on the number
	//	of iterations it went through. Is not called
	//	or needed outside the class so it is a
	//	private variable.
	void MandelbrotSet(double left_, double right_, double top_, double bottom_, int startY_, int finY_);

	sf::Image image;
	sf::Texture imageTexture;
	sf::RectangleShape imageRect;
	sf::Vertex centrePoint;

	const int WIDTH = 800,
			HEIGHT = 600;
	
	double left = -2.f,
		right = 1.f,
		top = 1.125f,
		bottom = -1.125f;

	const int MAX_ITERATIONS = 50;
	static const int numOfThreads = 24;

	std::thread threads[numOfThreads];
	std::mutex mandelbrot_mutex;
};

