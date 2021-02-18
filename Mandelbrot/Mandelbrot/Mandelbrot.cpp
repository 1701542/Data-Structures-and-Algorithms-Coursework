#include "Mandelbrot.h"



Mandelbrot::Mandelbrot(sf::RenderWindow* window)
{
	mandelbrot_mutex.lock();

	//	Positions and assigns the colour of the pixel in the centre.
	centrePoint.color = sf::Color::Red;
	centrePoint.position = sf::Vector2f(WIDTH / 2.f, HEIGHT / 2.f);

	//	Creates an image with a predetermined width and height.
	image.create(WIDTH, HEIGHT, sf::Color::Black);

	//	Creates a rectangle which fills the screen.
	imageRect.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));

	//	Texturises the rectangle to be black.
	imageTexture.loadFromImage(image);
	imageRect.setTexture(&imageTexture);
	mandelbrot_mutex.unlock();
}


Mandelbrot::~Mandelbrot()
{
}


sf::RectangleShape Mandelbrot::GetImage()
{
	return imageRect;
}

void Mandelbrot::CalculateSet(double left_, double right_, double top_, double bottom_)
{
	//for (int i = 0; i < 15; i++)
	//{

		//	Starts timer
		the_clock::time_point start = the_clock::now();

		mandelbrot_mutex.lock();

		//	Loop for dividing the image into slices and calling the thread functions.
		for (int i = (HEIGHT / numOfThreads); i <= HEIGHT; i += (HEIGHT / numOfThreads))
			threads[(int)((i / (HEIGHT / numOfThreads)) - 1)] = std::thread(&Mandelbrot::MandelbrotSet, this, left_, right_, top_, bottom_, i - (HEIGHT / numOfThreads), i);

		//	Joining all the threads
		for (int i = 0; i < numOfThreads; ++i)
			threads[i].join();


		//	Applies image to texture
		imageTexture.loadFromImage(image);

		//	Applies texture to rectangle
		imageRect.setTexture(&imageTexture);

		mandelbrot_mutex.unlock();

		the_clock::time_point end = the_clock::now();
		auto time_taken = duration_cast<milliseconds>(end - start).count();
		std::cout << "\n~~\nTime taken to finish calculating set: " << time_taken << "ms.\n~~\n";
	//}
}

void Mandelbrot::MandelbrotSet(double left_, double right_, double top_, double bottom_, int startY_, int finY_)
{
	for (int y = startY_; y < finY_; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			std::complex<double> c(left_ + (x*(right_ - left_) / WIDTH), top_ + (y * (bottom_ - top_) / HEIGHT));
		
			std::complex<double> z(0.0, 0.0);

			int iterations = 0;
			while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
			{
				z = (z*z) + c;

				++iterations;
			}

			//	Depending on the number of iterations it
			//	took to determine the pixel the colour
			//	will be white or black or in between.
			int iterationColor = static_cast<int>(255 * ((MAX_ITERATIONS - iterations) / static_cast<float>(MAX_ITERATIONS)));
			
			//	Sets currently calculated pixel and colours it.
			image.setPixel(x, y, sf::Color(iterationColor, iterationColor, iterationColor, 255));
		}
	}
	//	Makes sure the centre pixel is always indicated
	image.setPixel(centrePoint.position.x, centrePoint.position.y, centrePoint.color);
}