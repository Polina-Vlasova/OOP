#pragma once

#include "Control.h" 
#include "Menu.h"

class Label : public Control
{
public:
	Label(int left = 0, int top = 0, int right = 0, int bottom = 0, State state = EnabledState, string legend = "", bool multiLine = false) :
		Control(left, top, right, bottom, state), mLegend(legend), mMultiLine(multiLine) { }
	Label(RECT rect, string legend = "", State state = EnabledState, bool multiLine = false) :
		Control(rect, state), mLegend(legend), mMultiLine(multiLine) {}
	Label(const Label& label) : Control(label), mLegend(label.mLegend), mMultiLine(label.mMultiLine) {}

	bool	getMultiLine() const { return mMultiLine; }
	void	setMultiLine(bool multiLine) { mMultiLine = multiLine; }
	string	getLegend() const { return mLegend; }
	void	setLegend(string legend) { mLegend = legend; }

	Label& operator= (const Label& label);

	void draw(HDC& hDC);
	bool keyProcessing(int code, char unicodeChar, bool bKeyDown) { return false; }

	vector<string> getData() const { return vector<string>(1, mLegend); }

protected:
	string	mLegend; 

private:
	bool	mMultiLine;
};
