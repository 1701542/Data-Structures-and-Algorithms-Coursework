#include "Input.h"
#include "Mandelbrot.h"
#include <cassert>

void init();
void initRender();
void handleInput();
void updateRendering();
void updateSet();
void renderSet(sf::RenderWindow* window_pointer);

// Create window 
sf::RenderWindow window(sf::VideoMode(800, 600), "Interactive Mandelbrot Set", sf::Style::Close | sf::Style::Titlebar);  // Initialise level object 
Input input;
std::thread* handleThread = nullptr;

Mandelbrot mandelbrot(&window);
bool update_ready = false;
std::mutex update_mutex;
std::condition_variable update_cv;

bool updateMandelbrot = false;
double left = -2.f,
	right = 1.f,
	top = 1.125f,
	bottom = -1.125f;
float deltaTopBottom,
	deltaLeftRight;

void main()
{
	init();
	initRender();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				if (handleThread->joinable())
					handleThread->join();
				window.close();
				break;
			case sf::Event::KeyPressed:
				input.setKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				input.setKeyUp(event.key.code);
				break;
			case sf::Event::MouseMoved:
				input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					input.setMouseLeft(true);
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRight(true);
				}
				if (event.mouseButton.button == sf::Mouse::Middle)
				{
					input.setMouseMiddle(true);
				}
			default:
				// doesn't handle other events 
				break;
			}
		}

		handleThread = new std::thread(handleInput);
		handleThread->join();

		if (updateMandelbrot)
		{
			updateRendering();
			update_ready = false;
			updateMandelbrot = false;
			std::cout << "\n~~\nFinished updating\n~~\n";
		}
	}

	if (handleThread->joinable())
		handleThread->join();
}

//	Determines if there is any input.
//	Will only accept ESC while
//	recalculating the image.
void handleInput()
{
	if (!updateMandelbrot)
	{
		if (input.isKeyDown(sf::Keyboard::Equal))
		{
			std::cout << "\n~~\nZooming in\n~~\n";
			deltaTopBottom = sqrt((bottom - top)*(bottom - top)) * 0.2f;
			deltaLeftRight = deltaTopBottom * (sqrt((right - left)*(right - left)) / sqrt((bottom - top)*(bottom - top)));
			top -= deltaTopBottom;
			bottom += deltaTopBottom;
			left += deltaLeftRight;
			right -= deltaLeftRight;
			//std::cout << " Aspect Ratio: " << deltaTopBottom / deltaLeftRight << std::endl;
			updateMandelbrot = true;
			input.setKeyUp(sf::Keyboard::Equal);
		}
		else if (input.isKeyDown(sf::Keyboard::Dash))
		{
			std::cout << "\n~~\nZooming out\n~~\n";
			deltaTopBottom = sqrt((bottom - top)*(bottom - top)) * 0.2f;
			deltaLeftRight = deltaTopBottom * (sqrt((right - left)*(right - left)) / sqrt((bottom - top)*(bottom - top)));
			top += deltaTopBottom;
			bottom -= deltaTopBottom;
			left -= deltaLeftRight;
			right += deltaLeftRight;
			//std::cout << " Aspect Ratio: " << deltaTopBottom / deltaLeftRight << std::endl;
			updateMandelbrot = true;
			input.setKeyUp(sf::Keyboard::Dash);
		}
		else if (input.isKeyDown(sf::Keyboard::Up))
		{
			std::cout << "\n~~\nMoving up\n~~\n";
			top += deltaTopBottom * 0.05f;
			bottom += deltaTopBottom * 0.05f;
			updateMandelbrot = true;
			input.setKeyUp(sf::Keyboard::Up);
		}
		else if (input.isKeyDown(sf::Keyboard::Down))
		{
			std::cout << "\n~~\nMoving down\n~~\n";
			top -= deltaTopBottom * 0.05f;
			bottom -= deltaTopBottom * 0.05f;
			updateMandelbrot = true;
			input.setKeyUp(sf::Keyboard::Down);
		}
		else if (input.isKeyDown(sf::Keyboard::Left))
		{
			std::cout << "\n~~\nMoving left\n~~\n";
			left -= deltaLeftRight * 0.05f;
			right -= deltaLeftRight * 0.05f;
			updateMandelbrot = true;
			input.setKeyUp(sf::Keyboard::Left);
		}
		else if (input.isKeyDown(sf::Keyboard::Right))
		{
			std::cout << "\n~~\nMoving right\n~~\n";
			left += deltaLeftRight * 0.05f;
			right += deltaLeftRight * 0.05f;
			updateMandelbrot = true;
			input.setKeyUp(sf::Keyboard::Right);
		}
		else if (input.isKeyDown(sf::Keyboard::R))
		{
			std::cout << "\n~~\nResetting view\n~~\n";
			left = -2.f;
			right = 1.f;
			top = 1.125f;
			bottom = -1.125f;
			deltaTopBottom = sqrt((bottom - top)*(bottom - top));
			deltaLeftRight = deltaTopBottom * (sqrt((right - left)*(right - left)) / sqrt((bottom - top)*(bottom - top)));
			
			updateMandelbrot = true;
			input.setKeyUp(sf::Keyboard::R);
		}
		else if (input.isMouseLeftDown())
		{
			std::cout << "\n~~\nMoving view + Zooming in\n~~\n";
			deltaTopBottom = sqrt((bottom - top)*(bottom - top));
			deltaLeftRight = deltaTopBottom * (sqrt((right - left)*(right - left)) / sqrt((bottom - top)*(bottom - top)));

			left = left + ((deltaLeftRight * ((float)input.getMouseX() / (float)window.getSize().x)) - (deltaLeftRight / 2.f));

			bottom = bottom + ((deltaTopBottom - (deltaTopBottom * ((float)input.getMouseY() / (float)window.getSize().y))) - (deltaTopBottom / 2.f));


			right = left + deltaLeftRight;
			top = bottom + deltaTopBottom;

			deltaTopBottom = sqrt((bottom - top)*(bottom - top)) * 0.2f;
			deltaLeftRight = deltaTopBottom * (sqrt((right - left)*(right - left)) / sqrt((bottom - top)*(bottom - top)));
			top -= deltaTopBottom;
			bottom += deltaTopBottom;
			left += deltaLeftRight;
			right -= deltaLeftRight;

			updateMandelbrot = true;

			input.setMouseLeft(false);
		}
		else if (input.isMouseRightDown())
		{
			std::cout << "\n~~\nMoving view + Zooming out\n~~\n";
			deltaTopBottom = sqrt((bottom - top)*(bottom - top));
			deltaLeftRight = deltaTopBottom * (sqrt((right - left)*(right - left)) / sqrt((bottom - top)*(bottom - top)));
			
			left = left + ((deltaLeftRight * ((float)input.getMouseX() / (float)window.getSize().x)) - (deltaLeftRight / 2.f));

			bottom = bottom + ((deltaTopBottom - (deltaTopBottom * ((float)input.getMouseY() / (float)window.getSize().y))) - (deltaTopBottom / 2.f));

			right = left + deltaLeftRight;
			top = bottom + deltaTopBottom;

			deltaTopBottom = sqrt((bottom - top)*(bottom - top)) * 0.2f;
			deltaLeftRight = deltaTopBottom * (sqrt((right - left)*(right - left)) / sqrt((bottom - top)*(bottom - top)));
			top += deltaTopBottom;
			bottom -= deltaTopBottom;
			left -= deltaLeftRight;
			right += deltaLeftRight;

			updateMandelbrot = true;

			input.setMouseRight(false);
		}
		else if (input.isMouseMiddleDown())
		{
			std::cout << "\n~~\nMoving view\n~~\n";
			deltaTopBottom = sqrt((bottom - top)*(bottom - top));
			deltaLeftRight = deltaTopBottom * (sqrt((right - left)*(right - left)) / sqrt((bottom - top)*(bottom - top)));

			left = left + ((deltaLeftRight * ((float)input.getMouseX() / (float)window.getSize().x)) - (deltaLeftRight / 2.f));

			bottom = bottom + ((deltaTopBottom - (deltaTopBottom * ((float)input.getMouseY() / (float)window.getSize().y))) - (deltaTopBottom / 2.f));

			right = left + deltaLeftRight;
			top = bottom + deltaTopBottom;

			updateMandelbrot = true;

			input.setMouseMiddle(false);
		}
	}
	if (input.isKeyDown(sf::Keyboard::Escape))
	{
		std::cout << "\n~~\nExiting app\n~~\n";
		window.close();
	}
}

//	Starts the threads to recalculate image and redraw it.
void updateRendering()
{
	sf::Context context;
	window.setActive(false);

	std::thread updateSetThread(updateSet);
	std::thread renderSetThread(renderSet, &window);
	
	updateSetThread.join();
	renderSetThread.join();
}

//	Draws the image after it's been calculated
void renderSet(sf::RenderWindow* window_pointer)
{
	std::unique_lock<std::mutex> lock(update_mutex);

	while (!update_ready)
	{
		update_cv.wait(lock);
	}

	window_pointer->clear(sf::Color::Black);
	window_pointer->draw(mandelbrot.GetImage());
	window_pointer->display();
}

//	Calls on mandelbrot to recalculate the image
//	and notifies the render function when the
//	image is ready.
void updateSet()
{
	std::unique_lock<std::mutex> lock(update_mutex);
	
	mandelbrot.CalculateSet(left, right, top, bottom);

	update_ready = true;
	update_cv.notify_one();

}

//	Calculates the image once so that there is something to be drawn.
void init()
{
	mandelbrot.CalculateSet(left, right, top, bottom);
	deltaTopBottom = sqrt((bottom - top)*(bottom - top));
	deltaLeftRight = deltaTopBottom * (sqrt((right - left)*(right - left)) / sqrt((bottom - top)*(bottom - top)));
}

//	Renders the image on opening so that the screen isn't blank.
void initRender()
{
	window.clear(sf::Color::Black);
	window.draw(mandelbrot.GetImage());
	window.display();
}
