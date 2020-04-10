#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#define CAPTION_SIZE 30
#define BUTTON_HEIGHT 40
#define LABEL_HEIGHT 20
#define DISTANCE 20
#define LABEL_WIDTH 150
#define CELL_WIDTH 100
#define CELL_HEIGHT 30
#define COMBO_HEIGHT 25
#define COMBOITEM_HEIGHT 20

using namespace std;

class Control
{
public:
	enum State { EnabledState, PressedState, SelectedState }; 

public:
	Control(int left = 0, int top = 0, int right = 0, int bottom = 0, State state = EnabledState) :
		mLeft(left), mTop(top), mRight(right), mBottom(bottom), mState(state) {}
	Control(RECT rect, State state = EnabledState) :
		mLeft(rect.left), mTop(rect.top), mRight(rect.right), mBottom(rect.bottom), mState(state) {}
	Control(const Control& control) :
		mLeft(control.mLeft), mTop(control.mTop), mRight(control.mRight), mBottom(control.mBottom), mState(control.mState) {}

	int		Left() const { return mLeft; }
	void	setLeft(int left) { mLeft = left; }
	int		Top() const { return mTop; }
	void	setTop(int top) { mTop = top; }
	int		Right() const { return mRight; }
	void	setRight(int right) { mRight = right; }
	int		Bottom() const { return mBottom; }
	void	setBottom(int bottom) { mBottom = bottom; }
	State	getState() const { return mState; }
	void	setState(State state) { mState = state; }

	Control& operator= (const Control& control);

	RECT	Rect() const { return{ mLeft, mTop, mRight, mBottom }; }
	int		Width() const { return mRight - mLeft; }
	int		Height() const { return mBottom - mTop; }

	virtual bool keyProcessing(int code, char unicodeChar, bool bKeyDown) = 0;
	virtual void draw(HDC& hDC) = 0;
	virtual vector<string> getData() const = 0;  


protected:
	int		mLeft;
	int		mTop;
	int		mRight;
	int		mBottom;
	State	mState;
};