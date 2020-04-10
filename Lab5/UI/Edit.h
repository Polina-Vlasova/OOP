#pragma once

#include "Control.h" 
#include "Menu.h"

class Edit : public Control
{
public:
	Edit(int left = 0, int top = 0, int right = 0, int bottom = 0, string legend = "",
		bool numeric = false, bool password = false, bool multiLine = false, State state = EnabledState) :
		Control(left, top, right, bottom, state), mLegend(legend), mNumeric(numeric), mPassword(password), mMultiLine(multiLine) {
		mCursor = mLegend.size();
	}
	Edit(RECT rect, string legend = "", bool numeric = false, bool password = false, bool multiLine = false, State state = EnabledState) :
		Control(rect, state), mLegend(legend), mNumeric(numeric), mPassword(password), mMultiLine(multiLine) {
		mCursor = mLegend.size();
	}
	Edit(const Edit& edit) :
		Control(edit), mLegend(edit.mLegend), mNumeric(edit.mNumeric), mPassword(edit.mPassword), 
		mCursor(edit.mCursor), mMultiLine(edit.mMultiLine) {}

	string	getLegend() const { return mLegend; }
	void	setLegend(string legend) { mLegend = legend; }
	bool	getNumeric() const { return mNumeric; }
	void	setNumeric(bool numeric) { mNumeric = numeric; }
	bool	getPassword() const { return mPassword; }
	void	setPassword(bool password) { mPassword = password; }
	bool	getMultiLine() const { return mMultiLine; }
	void	setMultiLine(bool multiLine) { mMultiLine = multiLine; }
	int		getCursor() const { return mCursor; }
	void	setCursor(int cursor) { mCursor = cursor; }

	Edit& operator= (const Edit& edit);

	vector<string> getData() const { return vector<string>(1, mLegend); }

	void draw(HDC& hDC);
	bool keyProcessing(int code, char unicodeChar, bool bKeyDown);

protected:
	string	mLegend;
	bool	mNumeric;
	bool	mPassword;
	int		mCursor;
	bool	mMultiLine;
};
