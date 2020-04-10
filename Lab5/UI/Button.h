#pragma once

#include "Control.h"  
#include "Menu.h"

class Button : public Control
{
public:
	Button(int left = 0, int top = 0, int right = 0, int bottom = 0, string legend = "", State state = EnabledState) :
		Control(left, top, right, bottom, state), mLegend(legend) {}
	Button(RECT rect, string legend = "", State state = EnabledState) : Control(rect, state), mLegend(legend) {}
	Button(const Button& button) : Control(button), mLegend(button.mLegend) {}

	string	getLegend() const { return mLegend; }
	void	setLegend(string legend) { mLegend = legend; }

	Button& operator= (const Button& button);

	void draw(HDC& hDC);
	bool keyProcessing(int code, char unicodeChar, bool bKeyDown);

	vector<string> getData() const { return vector<string>(1, mLegend); }

protected:
	string mLegend;
};
