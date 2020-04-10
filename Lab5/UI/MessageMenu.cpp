#include "MessageMenu.h"

MessageMenu::MessageMenu(string title, string message) : Menu(title), mMessage(message)
{
	mSize = 1;
	rebuild();
}



void MessageMenu::draw()
{ 
	HDC hDC = GetDC(mhWnd);
	RECT rect;
	GetClientRect(mhWnd, &rect);
	//FillRect(hDC, &rect, CreateSolidBrush(RGB(0, 0, 0)));

	HFONT font = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
	HFONT oldFont = (HFONT)SelectObject(hDC, font);
	for (int i = 0; i < mControls.size(); i++)
		mControls[i]->draw(hDC);
	SelectObject(hDC, oldFont);
	DeleteObject(font);
	ReleaseDC(mhWnd, hDC);
}

void MessageMenu::rebuild()
{
	mControls.clear();
	 
	HDC hDC = GetDC(mhWnd);
	HFONT font = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
	HFONT oldFont = (HFONT)SelectObject(hDC, font);
	int wsize = 0;
	int hsize = 1;
	int curwsize = 0;
	string maxstring = "";
	if (mMessage[mMessage.size() - 1] != '\n')
		mMessage += '\n';
	for (int i = 0; i < mMessage.size(); i++) {
		if (mMessage[i] == '\n') {
			hsize++;
			string curstring = mMessage.substr(i - curwsize, curwsize);
			if (curstring.size() > wsize) {
				wsize = curstring.size();
				maxstring = curstring;
			}
			curwsize = 0;
		}
		else
			curwsize++;
	}
	hsize--; 
	SIZE size;
	GetTextExtentPoint32(hDC, s2ws(maxstring).c_str(), maxstring.size(), &size);
	int width = size.cx + 2 * DISTANCE;
	int height = size.cy * hsize + 2 * DISTANCE + CAPTION_SIZE;

	RECT rect;
	GetClientRect(mhWnd, &rect);
	rect.left = (rect.right - rect.left - width) / 2;
	rect.top = 60;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	mControls.push_back(new Window(rect, mTitle));

	RECT messageRect = { rect.left + DISTANCE,
		rect.top + CAPTION_SIZE + DISTANCE,
		rect.right - DISTANCE,
		rect.bottom - DISTANCE };
	mControls.push_back(new Label(messageRect, mMessage, Control::State::EnabledState, true));

	SelectObject(hDC, oldFont);
	DeleteObject(font);
	ReleaseDC(mhWnd, hDC);
}
