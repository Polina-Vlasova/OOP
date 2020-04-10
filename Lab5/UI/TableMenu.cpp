#include "TableMenu.h"

TableMenu::TableMenu(string title, vector<vector<string>> data) :
	Menu(title), mData(data)
{
	mSize = data.size();
	for (int i = 0; i < data.size(); i++) {
		mCellsWeight.push_back(vector<int>(data[i].size(), 1)); 
	}
}


void TableMenu::draw()
{
	if (mData.size() == 0)
		return;

	HDC hDC = GetDC(mhWnd);
	RECT rect;
	GetClientRect(mhWnd, &rect);
	HBRUSH brush = CreateSolidBrush(sTheme == Dark ? RGB(120, 120, 120) : RGB(170, 170, 170));
	FillRect(hDC, &rect, brush);


	HDC memDC = CreateCompatibleDC(hDC); 
	FillRect(memDC, &rect, brush);
	HBITMAP bitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
	HFONT font = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
	HFONT oldFont = (HFONT)SelectObject(memDC, font);
	vector<int> cellWidth;
	for (int i = 0; i < mData.size(); i++) {
		for (int j = 0; j < mData[i].size(); j++) {
			if (cellWidth.size() <= j)
				cellWidth.push_back(0);
			SIZE size; 
			GetTextExtentPoint32(memDC, s2ws(mData[i][j]).c_str(), mData[i][j].size(), &size);
			if (cellWidth[j] < size.cx + 20 && mCellsWeight[i][j] == 1)
				cellWidth[j] = size.cx + 20;
		}
	}
	int width = DISTANCE * 2;
	int height = CAPTION_SIZE + CELL_HEIGHT * mData.size() + DISTANCE * 2;

	for (int i = 0; i < cellWidth.size(); i++) {
		width += cellWidth[i];
	}
	rect.left = (rect.right - rect.left - width) / 2;
	rect.top = 20;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	Window(rect, mTitle).draw(memDC);

	RECT cellRect = { rect.left + DISTANCE,
		rect.top + CAPTION_SIZE + DISTANCE,
		rect.left + DISTANCE + cellWidth[0],
		rect.top + CAPTION_SIZE + CELL_HEIGHT + DISTANCE };

	for (int i = 1; i < mCellsWeight[0][0]; i++)
		cellRect.right += cellWidth[i];

	for (int i = 0; i < mData.size(); i++) {
		for (int j = 0; j < mData[i].size(); j++) {
			drawCell(memDC, cellRect, mData[i][j]);
			cellRect.left = cellRect.right;

			if (j + 1 != mData[i].size())
				for (int k = 0; k < mCellsWeight[i][j + 1] && (j + 1 + k < cellWidth.size()); k++)
					cellRect.right += cellWidth[j + mCellsWeight[i][j] + k];
		}
		cellRect.left = rect.left + DISTANCE;
		cellRect.right = rect.left + DISTANCE + cellWidth[0];
		if (i + 1 != mData.size())
			for (int j = 1; j < mCellsWeight[i + 1][0]; j++)
				cellRect.right += cellWidth[j];
		cellRect.top += CELL_HEIGHT;
		cellRect.bottom += CELL_HEIGHT;
	}

	SetStretchBltMode(hDC, HALFTONE);
	StretchBlt(hDC, rect.left, rect.top, rect.right - rect.left,
		rect.bottom - rect.top, memDC, rect.left, rect.top, rect.right - rect.left,
		rect.bottom - rect.top, SRCCOPY);
	SelectObject(memDC, oldBitmap);
	SelectObject(memDC, oldFont);
	DeleteObject(bitmap);
	DeleteObject(brush);
	DeleteObject(font);
	DeleteDC(memDC);

	ReleaseDC(mhWnd, hDC);
}


void TableMenu::drawCell(HDC& hDC, RECT rect, string text)
{
	HBRUSH brush = CreateSolidBrush(sTheme == Dark ? RGB(83, 83, 83) : RGB(240, 240, 240));
	HPEN pen = CreatePen(PS_SOLID, 1, sTheme == Dark ? RGB(95, 95, 95) : RGB(221, 221, 221));
	HPEN oldPen = (HPEN)SelectObject(hDC, pen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, brush);

	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, sTheme == Dark ? RGB(255, 255, 255) : RGB(90, 90, 90)); 
	DrawText(hDC, s2ws(text).c_str(), text.size(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	SelectObject(hDC, oldPen);
	SelectObject(hDC, oldBrush);
	DeleteObject(pen);
	DeleteObject(brush);
}
