#include "Menu.h" 
 
Menu::Theme Menu::sTheme = Menu::Theme::Dark; 


Menu::Menu(string title) : mTitle(title) 
{
	mIndex = 0;
	mPressed = false;
	mMousePressed = false;
	mSize = 0; 
	mhWnd = GetConsoleWindow();
	rebuild();
}



Menu::Menu(string title, vector<string> options) : mTitle(title), mOptions(options) 
{
	mIndex = 0;
	mPressed = false;
	mMousePressed = false;
	mSize = mOptions.size();
	mhWnd = GetConsoleWindow();
	rebuild();
}



Menu::Menu(string title, vector<string> options, vector<std::function<void(void)>> actions) :
	mTitle(title), mOptions(options), mActions(actions)
{
	mIndex = 0;
	mPressed = false;
	mMousePressed = false;
	mSize = mOptions.size();
	mhWnd = GetConsoleWindow();
	rebuild();
} 



Menu::Menu(const Menu& menu) : mTitle(menu.mTitle), mOptions(menu.mOptions), mActions(menu.mActions) 
{
	mIndex = menu.mIndex;
	mPressed = menu.mPressed;
	mMousePressed = menu.mMousePressed;
	mSize = menu.mSize;
	mhWnd = GetConsoleWindow();
	rebuild();
}



int Menu::run()
{
	DWORD cNumRead;
	INPUT_RECORD irInBuf[128];
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD fdwMode = ENABLE_WINDOW_INPUT;
	bool exit = false;
	while (!exit) {
		draw();
		ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead);
		wchar_t wc;
		char c;
		switch (irInBuf[0].EventType) {
		case KEY_EVENT:
			wc = irInBuf[0].Event.KeyEvent.uChar.UnicodeChar;
			c = wc;
			if (wc >= 1040 && wc <= 1071)
				c = 'À' + (wc - 1040);
			else if (wc >= 1072 && wc <= 1103)
				c = 'à' + (wc - 1072);
			exit = keyProcessing(irInBuf[0].Event.KeyEvent.wVirtualScanCode,
				c, irInBuf[0].Event.KeyEvent.bKeyDown);
			break;
		case MOUSE_EVENT:
			bool pressed = false;
			if (irInBuf[0].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
				pressed = true;
			//	exit = mouseProcessing(pressed, irInBuf[0].Event.MouseEvent.dwMousePosition.X,
			//		irInBuf[0].Event.MouseEvent.dwMousePosition.Y);

			break;
		}
	}
	draw();
	return mIndex;
}

bool Menu::keyProcessing(int code, char unicodeChar, bool bKeyDown)
{
	if ((code == 72 || code == 75) && bKeyDown)
		mIndex--;
	else if ((code == 80 || code == 77) && bKeyDown)
		mIndex++;
	else if (code == 28) {
		mPressed = bKeyDown;
		if (!bKeyDown) {
			if (mIndex == mSize - 1 || mActions.size() == 0)
				return true;
			else if (mIndex < mActions.size())
				mActions[mIndex]();
		}
	}
	mIndex = (mIndex + mSize) % mSize;
	return false;
}



bool Menu::mouseProcessing(bool pressed, int x, int y)
{ 
	RECT rect;
	GetClientRect(mhWnd, &rect);
	POINT p = { rect.left + (double)(rect.right - rect.left) / 263 * x,
		rect.top + (double)(rect.bottom - rect.top) / 63 * y };
	for (int i = 1; i < mControls.size(); i++) {
		if (PtInRect(&(mControls[i]->Rect()), p))
			mIndex = i - 1;
	}
	if (!pressed && mPressed) {
		mPressed = pressed;
		if (mIndex == mSize - 1 || mActions.size() == 0)
			return true;
		else if (mIndex < mActions.size())
			mActions[mIndex]();
	}
	mPressed = pressed;
	return false;
}
 


void Menu::draw()
{ 
	HDC hDC = GetDC(mhWnd);
	RECT rect;
	GetClientRect(mhWnd, &rect);

	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP bitmap = CreateCompatibleBitmap(hDC, rect.right - rect.left, rect.bottom - rect.top);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);

	HBRUSH backBrush = CreateSolidBrush(sTheme == Dark ? RGB(120, 120, 120) : RGB(170, 170, 170));
	FillRect(memDC, &rect, backBrush);
	HFONT font = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
	HFONT oldFont = (HFONT)SelectObject(memDC, font);
	for (int i = 0; i < mOptions.size(); i++) {
		if (mIndex == i)
			mControls[i + 1]->setState(mPressed ? Control::PressedState : Control::SelectedState);
		else
			mControls[i + 1]->setState(Control::EnabledState);
	}
	for (int i = 0; i < mControls.size(); i++)
		mControls[i]->draw(memDC);
	 
	BitBlt(hDC, rect.left, rect.top, rect.right - rect.left,
		rect.bottom - rect.top, memDC, rect.left, rect.top, SRCCOPY);
	SelectObject(memDC, oldBitmap);
	SelectObject(memDC, oldFont);
	DeleteObject(bitmap);
	DeleteObject(backBrush);
	DeleteObject(font);
	DeleteDC(memDC);

	ReleaseDC(mhWnd, hDC);
}

void Menu::rebuild()
{
	mControls.clear(); 
	int height = CAPTION_SIZE + BUTTON_HEIGHT * mOptions.size() + DISTANCE * (mOptions.size() + 1);
	 
	HDC hDC = GetDC(mhWnd);
	RECT rect;
	GetClientRect(mhWnd, &rect);

	HFONT font = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
	HFONT oldFont = (HFONT)SelectObject(hDC, font);
	int itemWidth = 0;
	for (int i = 0; i < mOptions.size(); i++) { 
		SIZE size; 
		GetTextExtentPoint32(hDC, s2ws(mOptions[i]).c_str(), mOptions[i].size(), &size);
		if (itemWidth < size.cx + 20)
			itemWidth = size.cx + 20;

	}

	int width = max(itemWidth + 2 * DISTANCE, 300);

	rect.left = (rect.right - rect.left - width) / 2;
	rect.top = 20;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	mControls.push_back(new Window(rect, mTitle));

	RECT btnRect = { rect.left + DISTANCE,
		rect.top + CAPTION_SIZE + DISTANCE,
		rect.right - DISTANCE,
		rect.top + CAPTION_SIZE + BUTTON_HEIGHT + DISTANCE };
	for (int i = 0; i < mOptions.size(); i++) {
		mControls.push_back(new Button(btnRect, mOptions[i]));
		btnRect.top += BUTTON_HEIGHT + DISTANCE;
		btnRect.bottom += BUTTON_HEIGHT + DISTANCE;
	}

	SelectObject(hDC, oldFont);
	DeleteObject(font);
	ReleaseDC(mhWnd, hDC);
}



Menu::~Menu()
{
	for (int i = 0; i < mControls.size(); i++)
		delete mControls[i];
}