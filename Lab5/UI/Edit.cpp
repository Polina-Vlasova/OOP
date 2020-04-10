#include "Edit.h"

Edit & Edit::operator=(const Edit & edit)
{
	mLegend = edit.mLegend;
	mNumeric = edit.mNumeric;
	mPassword = edit.mPassword;
	mCursor = edit.mCursor;
	return *this;
}

void Edit::draw(HDC& hDC)
{
	COLORREF backgroundColor = Menu::sTheme == Menu::Theme::Dark ? RGB(67, 67, 67) : RGB(245, 245, 245);
	COLORREF borderColor = Menu::sTheme == Menu::Theme::Dark ? RGB(95, 95, 95) : RGB(220, 220, 220);

	if (mState == SelectedState) {
		backgroundColor = Menu::sTheme == Menu::Theme::Dark ? RGB(95, 95, 95) : RGB(255, 255, 255);
	}
	else	if (mState == PressedState) {
		borderColor = Menu::sTheme == Menu::Theme::Dark ? RGB(30, 162, 241) : RGB(0, 120, 226);
	}
	HBRUSH brush = CreateSolidBrush(backgroundColor);
	HPEN pen = CreatePen(PS_SOLID, 1, borderColor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, brush);
	HPEN oldPen = (HPEN)SelectObject(hDC, pen);

	Rectangle(hDC, mLeft, mTop, mRight, mBottom);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, Menu::sTheme == Menu::Theme::Dark ? RGB(255, 255, 255) : RGB(70, 70, 70));
	RECT textRect = Rect();
	textRect.left += 5;
	textRect.right -= 5;
	string temp = mLegend;
	if (mPassword) {
		for (int i = 0; i < temp.size(); i++)
			temp[i] = '*';
	}
	if (mState == PressedState)
		temp.insert(mCursor, 1, '|');
	if (mMultiLine) { 
		int hsize = 1;
		int curwsize = 0;  
		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] == '\n') {
				hsize++; 
				curwsize = 0;
			}
			else
				curwsize++;
			if(curwsize == 20 && i != temp.size() - 1)
				temp.insert(i + 1, 1, '\n');
		} 
		SIZE size;
		GetTextExtentPoint32(hDC, Menu::s2ws(temp).c_str(), temp.size(), &size); 
		int height = max(size.cy * hsize, Height());
		textRect.bottom = textRect.top + height;
		mBottom = textRect.bottom;
	}
	UINT uFormat = DT_VCENTER;
	if (!mMultiLine)
		uFormat |= DT_SINGLELINE;
	DrawText(hDC, Menu::s2ws(temp).c_str(), temp.size(), &textRect, uFormat);

	SelectObject(hDC, oldBrush);
	SelectObject(hDC, oldPen);
	DeleteObject(brush);
	DeleteObject(pen);
}


bool Edit::keyProcessing(int code, char unicodeChar, bool bKeyDown)
{
	if (mMultiLine && bKeyDown && ((unicodeChar >= 'a' && unicodeChar <= 'z') || (unicodeChar >= 'A' && unicodeChar <= 'Z')
		|| (unicodeChar >= 'à' && unicodeChar <= 'ÿ') || (unicodeChar >= '0' && unicodeChar <= '9') || unicodeChar == '.' ||
		(unicodeChar >= 'À' && unicodeChar <= 'ß') || unicodeChar == '-' || unicodeChar == ' ' || code == 29)) {
		if (code == 29)
			mLegend.insert(mCursor, 1, '\n');
		else
			mLegend.insert(mCursor, 1, unicodeChar);
		mCursor++;
	}
	else if ((mPassword || !mNumeric) && bKeyDown && ((unicodeChar >= 'a' && unicodeChar <= 'z') || (unicodeChar >= 'A' && unicodeChar <= 'Z')
		|| (unicodeChar >= 'à' && unicodeChar <= 'ÿ') || (unicodeChar >= 'À' && unicodeChar <= 'ß') || unicodeChar == '-') && mLegend.size() < 20) {
		mLegend.insert(mCursor, 1, unicodeChar);
		mCursor++;
	}
	else if ((mPassword || mNumeric) && bKeyDown && ((unicodeChar >= '0' && unicodeChar <= '9') || unicodeChar == '.' || unicodeChar == '-') && mLegend.size() < 20) {
		mLegend.insert(mCursor, 1, unicodeChar);
		mCursor++;
	}
	else if (code == 28 && bKeyDown)
		return false;
	else if (code == 75 && !bKeyDown) {
		mCursor--;
	}
	else if (code == 77 && !bKeyDown) {
		mCursor++;
	}
	else if (code == 83 && !bKeyDown) {
		mLegend.erase(mCursor, 1);
	}
	else if (code == 14 && !bKeyDown && mCursor != 0) {
		mLegend.erase(mCursor - 1, 1);
		mCursor--;
	}
	if (mCursor > mLegend.size()) mCursor = mLegend.size();
	if (mCursor < 0) mCursor = 0;
	return true;
}