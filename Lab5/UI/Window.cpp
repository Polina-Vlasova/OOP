#include "Window.h"

Window & Window::operator=(const Window & window)
{
	mTitle = window.mTitle;
	return *this;
}

void Window::draw(HDC& hDC)
{
	HBRUSH backBrush = CreateSolidBrush(Menu::sTheme == Menu::Theme::Dark ? RGB(83, 83, 83) : RGB(240, 240, 240));
	HPEN borderPen = CreatePen(PS_SOLID, 1, Menu::sTheme == Menu::Theme::Dark ? RGB(53, 53, 53) : RGB(204, 204, 204));
	HPEN oldPen = (HPEN)SelectObject(hDC, borderPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, backBrush);
	Rectangle(hDC, mLeft, mTop, mRight, mBottom);

	HBRUSH captionBrush = CreateSolidBrush(Menu::sTheme == Menu::Theme::Dark ? RGB(64, 64, 64) : RGB(221, 221, 221));
	SelectObject(hDC, captionBrush);
	RECT captionRect = Rect();
	captionRect.bottom = mTop + CAPTION_SIZE; 
	Rectangle(hDC, captionRect.left, captionRect.top, captionRect.right, captionRect.bottom);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, Menu::sTheme == Menu::Theme::Dark ? RGB(177, 177, 177) : RGB(121, 121, 121)); 
	DrawText(hDC, Menu::s2ws(mTitle).c_str(), mTitle.size(), &captionRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	SelectObject(hDC, oldPen);
	SelectObject(hDC, oldBrush);
	DeleteObject(backBrush);
	DeleteObject(borderPen);
	DeleteObject(captionBrush);
}