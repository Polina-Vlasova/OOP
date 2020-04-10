#include "ComboBox.h"

ComboBox & ComboBox::operator=(const ComboBox & comboBox)
{
	mContents = comboBox.mContents;
	mIndex = comboBox.mIndex;
	mCurSel = comboBox.mCurSel;
	mPressed = comboBox.mPressed;
	return *this;
}

void ComboBox::draw(HDC& hDC)
{ 
	COLORREF backColor = Menu::sTheme == Menu::Theme::Dark ? RGB(74, 74, 74) : RGB(245, 245, 245);
	COLORREF borderColor = Menu::sTheme == Menu::Theme::Dark ? RGB(95, 95, 95) : RGB(221, 221, 221);
	if (mState == PressedState)
	{
		backColor = Menu::sTheme == Menu::Theme::Dark ? RGB(44, 44, 44) : RGB(190, 190, 190);
		borderColor = Menu::sTheme == Menu::Theme::Dark ? RGB(95, 95, 95) : RGB(206, 206, 206);
	}
	else if (mState == SelectedState)
	{
		backColor = Menu::sTheme == Menu::Theme::Dark ? RGB(89, 89, 89) : RGB(255, 255, 255);
		borderColor = Menu::sTheme == Menu::Theme::Dark ? RGB(30, 162, 241) : RGB(0, 119, 234);
	}
	HBRUSH backBrush = CreateSolidBrush(backColor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, backBrush);
	HPEN borderPen = CreatePen(PS_SOLID, 1, borderColor);
	HPEN oldPen = (HPEN)SelectObject(hDC, borderPen);

	Rectangle(hDC, mLeft, mTop, mRight, mBottom);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, Menu::sTheme == Menu::Theme::Dark ? RGB(255, 255, 255) : RGB(90, 90, 90));
	RECT textRect = { mLeft + 5, mTop, mRight, mBottom }; 
	DrawText(hDC, Menu::s2ws(mContents[mCurSel]).c_str(), mContents[mCurSel].size(), &textRect, DT_SINGLELINE | DT_VCENTER);

	RECT arrowRect = { mRight - Height(), mTop + 1, mRight - 1, mBottom - 1 };
	RECT memRect = { 0, 0, (arrowRect.right - arrowRect.left) * 4, (arrowRect.bottom - arrowRect.top) * 4 };
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP bitmap = CreateCompatibleBitmap(hDC, memRect.right, memRect.bottom);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
	HPEN pen = CreatePen(PS_SOLID, 5, Menu::sTheme == Menu::Theme::Dark ? RGB(151, 151, 151) : RGB(151, 151, 151));
	HPEN oldPen2 = (HPEN)SelectObject(memDC, pen);
	FillRect(memDC, &memRect, CreateSolidBrush(backColor));
	MoveToEx(memDC, memRect.left + (memRect.right - memRect.left) / 2, memRect.top + (memRect.bottom - memRect.top) / 3 * 2 - 6, NULL);
	LineTo(memDC, memRect.left + (memRect.right - memRect.left) / 2 - 20, memRect.top + (memRect.bottom - memRect.top) / 3 + 6);
	MoveToEx(memDC, memRect.left + (memRect.right - memRect.left) / 2, memRect.top + (memRect.bottom - memRect.top) / 3 * 2 - 6, NULL);
	LineTo(memDC, memRect.left + (memRect.right - memRect.left) / 2 + 20, memRect.top + (memRect.bottom - memRect.top) / 3 + 6);
	SetStretchBltMode(hDC, HALFTONE);
	StretchBlt(hDC, arrowRect.left, arrowRect.top, arrowRect.right - arrowRect.left,
		arrowRect.bottom - arrowRect.top, memDC, 0, 0, memRect.right, memRect.bottom, SRCCOPY);
	SelectObject(memDC, oldBitmap);
	SelectObject(memDC, oldPen2);
	DeleteObject(bitmap);
	DeleteObject(pen);
	DeleteDC(memDC);

	if (mState == PressedState) {
		int size = mContents.size();
		int start = mIndex / 15;
		RECT dropdownRect = { mLeft, mBottom, mRight, mBottom + COMBOITEM_HEIGHT *  min(size - 15 * start, 15) + 2 };
		Rectangle(hDC, dropdownRect.left, dropdownRect.top, dropdownRect.right, dropdownRect.bottom);
		RECT itemRect = { mLeft + 1, mBottom + 1, mRight - 1, mBottom + 1 + COMBOITEM_HEIGHT };
		for (int i = 15 * start; i < 15 * start + min(size - 15 * start, 15); i++) {
			State itemState = EnabledState;
			if (mIndex == i)
				itemState = mPressed ? PressedState : SelectedState;
			drawItem(hDC, itemRect, mContents[i], itemState, mCurSel == i);
			itemRect.top += COMBOITEM_HEIGHT;
			itemRect.bottom += COMBOITEM_HEIGHT;
		}
	}

	SelectObject(hDC, oldBrush);
	SelectObject(hDC, oldPen);
	DeleteObject(backBrush);
	DeleteObject(borderPen);
}


void ComboBox::drawItem(HDC& hDC, RECT rect, string legend, State state, bool checked)
{
	RECT markRect = rect;
	markRect.right = markRect.left + markRect.bottom - markRect.top;

	COLORREF backColor = Menu::sTheme == Menu::Theme::Dark ? RGB(73, 73, 73) : RGB(230, 230, 230);
	COLORREF captionColor = Menu::sTheme == Menu::Theme::Dark ? RGB(255, 255, 255) : RGB(49, 49, 49);

	if (checked)
		captionColor = Menu::sTheme == Menu::Theme::Dark ? RGB(51, 179, 238) : RGB(83, 83, 83);
	if (state == SelectedState)
		backColor = Menu::sTheme == Menu::Theme::Dark ? RGB(95, 95, 95) : RGB(215, 215, 215);
	else if (state == PressedState)
		backColor = Menu::sTheme == Menu::Theme::Dark ? RGB(42, 42, 42) : RGB(190, 190, 190);

	FillRect(hDC, &rect, CreateSolidBrush(backColor));

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, captionColor);
	RECT textRect = { markRect.right + 5, rect.top, rect.right, rect.bottom }; 
	DrawText(hDC, Menu::s2ws(legend).c_str(), legend.size(), &textRect, DT_SINGLELINE | DT_VCENTER);

	if (checked) {
		RECT memRect = { 0, 0, (markRect.right - markRect.left) * 4, (markRect.bottom - markRect.top) * 4 };
		HDC memDC = CreateCompatibleDC(hDC);
		HBITMAP bitmap = CreateCompatibleBitmap(hDC, memRect.right, memRect.bottom);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
		HBRUSH brush = CreateSolidBrush(backColor);
		FillRect(memDC, &memRect, brush);
		HPEN pen = CreatePen(PS_SOLID, (memRect.right - memRect.left) / 8,
			Menu::sTheme == Menu::Theme::Dark ? RGB(196, 196, 196) : RGB(49, 49, 49));
		HPEN oldPen = (HPEN)SelectObject(memDC, pen);

		MoveToEx(memDC, memRect.left + (memRect.right - memRect.left) / 3.5, memRect.top + (memRect.bottom - memRect.top) / 3.5 * 1.5, NULL);
		LineTo(memDC, memRect.left + (memRect.right - memRect.left) / 3.5 * 1.5, memRect.top + (memRect.bottom - memRect.top) / 3.5 * 2);
		LineTo(memDC, memRect.left + (memRect.right - memRect.left) / 3.5 * 2.5, memRect.top + memRect.top + (memRect.bottom - memRect.top) / 3.5);

		SetStretchBltMode(hDC, HALFTONE);
		StretchBlt(hDC, markRect.left, markRect.top, markRect.right - markRect.left,
			markRect.bottom - markRect.top, memDC, 0, 0, memRect.right, memRect.bottom, SRCCOPY);
		SelectObject(memDC, oldPen);
		SelectObject(memDC, oldBitmap);
		DeleteObject(brush);
		DeleteObject(pen);
		DeleteObject(bitmap);
		DeleteDC(memDC);
	}
}


bool ComboBox::keyProcessing(int code, char unicodeChar, bool bKeyDown)
{
	if ((code == 72 || code == 75) && bKeyDown) {
		mIndex--;
	}
	else if ((code == 80 || code == 77) && bKeyDown) {
		mIndex++;
	}
	else if (code == 28) {
		mPressed = bKeyDown;
		if (bKeyDown) {
			mCurSel = mIndex;
			return false;
		}
	}
	mIndex = (mIndex + mContents.size()) % mContents.size();
	return true;
}