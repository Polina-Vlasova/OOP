#pragma once

#include "Control.h" 
#include "Menu.h"

#define BUTTON_HEIGHT 40

class ComboBox : public Control
{
public:
	ComboBox(int left = 0, int top = 0, int right = 0, int bottom = 0, 
		vector<string> contents = vector<string>(1, ""), int index = 0, State state = EnabledState) :
		Control(left, top, right, bottom, state), mContents(contents), mIndex(index) {
		mCurSel = 0;
	}
	ComboBox(RECT rect, vector<string> contents = vector<string>(1, ""), int index = 0, State state = EnabledState) :
		Control(rect, state), mContents(contents), mIndex(index){
		mCurSel = 0;
	}
	ComboBox(const ComboBox& comboBox) : Control(comboBox), mContents(comboBox.mContents), mIndex(comboBox.mIndex) {
		mCurSel = comboBox.mCurSel;
	}

	vector<string>	getContents() const { return mContents; }
	void			setContents(vector<string> contents) { mContents = contents; }
	int				getIndex() const { return mIndex; }
	void			setIndex(int index) { mIndex = index; }
	int				getCurSel() const { return mCurSel; }
	void			setCurSel(int curSel) { mCurSel = curSel; mIndex = curSel; }
	bool			getPressed() const { return mPressed; }
	void			setPressed(bool pressed) { mPressed = pressed; }

	ComboBox& operator= (const ComboBox& comboBox);

	string	getLegend() const { return mContents[mCurSel]; }

	void draw(HDC& hDC);
	bool keyProcessing(int code, char unicodeChar, bool bKeyDown);
	vector<string> getData() const { return mContents; } 

protected:
	void drawItem(HDC& hDC, RECT rect, string legend, State state, bool checked);

protected:
	vector<string> mContents; 
	int mIndex;
	int mCurSel;
	bool mPressed;
};
