#pragma once
class Input
{
private:
	struct Mouse
	{
		int x, y;
		bool left;
		bool right;
		bool middle;
	};
public:
	void setKeyDown(int key);
	void setKeyUp(int key);
	bool isKeyDown(int key);

	void setMouseLeft(bool b) { mouse.left = b; };
	void setMouseRight(bool b) { mouse.right = b; };
	void setMouseMiddle(bool b) { mouse.middle = b; };
	void setMousePosition(int lx, int ly) { setMouseX(lx); setMouseY(ly); };
	void setMouseX(int lx) { mouse.x = lx; };
	void setMouseY(int ly) { mouse.y = ly; };

	bool isMouseLeftDown() { return mouse.left; };
	bool isMouseRightDown() { return mouse.right; };
	bool isMouseMiddleDown() { return mouse.middle; };
	int getMouseX() { return mouse.x; };
	int getMouseY() { return mouse.y; };

private:
	bool keys[256]{ false };
	Mouse mouse;
};

