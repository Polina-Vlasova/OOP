#include "Button.h"

Button & Button::operator=(const Button & button)
{
	mLegend = button.mLegend;
	return *this;
}

void Button::draw(HDC& hDC)
{
	COLORREF backgroundColor = Menu::sTheme == Menu::Theme::Dark ? RGB(95, 95, 95) : RGB(230, 230, 230);
	COLORREF borderColor = Menu::sTheme == Menu::Theme::Dark ? RGB(117, 117, 117) : RGB(204, 204, 204);

	if (mState == PressedState) {
		backgroundColor = Menu::sTheme == Menu::Theme::Dark ? RGB(42, 42, 42) : RGB(190, 190, 190);

	}
	else if (mState == SelectedState) {
		backgroundColor = Menu::sTheme == Menu::Theme::Dark ? RGB(110, 110, 110) : RGB(250, 250, 250);
	}

	HBRUSH backBrush = CreateSolidBrush(backgroundColor);
	HPEN borderPen = CreatePen(PS_SOLID, 1, borderColor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, backBrush);
	HPEN oldPen = (HPEN)SelectObject(hDC, borderPen);

	Rectangle(hDC, mLeft, mTop, mRight, mBottom);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, Menu::sTheme == Menu::Theme::Dark ? RGB(255, 255, 255) : RGB(90, 90, 90)); 
	DrawText(hDC, Menu::s2ws(mLegend).c_str(), mLegend.size(), &Rect(), DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	SelectObject(hDC, oldBrush);
	SelectObject(hDC, oldPen);
	DeleteObject(backBrush);
	DeleteObject(borderPen);
}


bool Button::keyProcessing(int code, char unicodeChar, bool bKeyDown)
{
	return false;
}