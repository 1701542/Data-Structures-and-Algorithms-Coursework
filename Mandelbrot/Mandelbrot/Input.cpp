#include "Input.h"

void Input::setKeyDown(int key)
{
	if (key >= 0)
	{
		keys[key] = true;
	}
}
void Input::setKeyUp(int key)
{
	if (key >= 0)
	{
		keys[key] = false;
	}
}
bool Input::isKeyDown(int key)
{
	return keys[key];
}