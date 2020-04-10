#pragma once

#include "Control.h" 
#include "Menu.h"

class Window : public Control
{
public:
	Window(int left = 0, int top = 0, int right = 0, int bottom = 0, string title = "") :
		Control(left, top, right, bottom), mTitle(title) {}
	Window(RECT rect, string title) : Control(rect), mTitle(title) {}
	Window(const Window& window) : Control(window), mTitle(window.mTitle) {}

	string	getTitle() const { return mTitle; }
	void	setTitle(string title) { mTitle = title; }

	Window& operator= (const Window& window);

	void draw(HDC& hDC);
	bool keyProcessing(int code, char unicodeChar, bool bKeyDown) { return false; }

	vector<string> getData() const { return vector<string>(1, mTitle); }

protected:
	string mTitle;
};