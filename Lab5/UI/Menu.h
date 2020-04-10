#pragma once

#include <windows.h>
#include <conio.h>
#include <functional>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
 
#include "Button.h"
#include "Window.h" 

using namespace std;

class Menu
{
public:
	Menu(string title = "");
	Menu(string title, vector<string> options);
	Menu(string title, vector<string> options, vector<function<void(void)>> actions);
	Menu(const Menu& menu);
	~Menu();

	void			setTitle(string title) { mTitle = title; }
	string			getTitle() const { return mTitle; }
	void			setOptions(vector<string> options) { mOptions = options; }
	vector<string>	getOptions() const { return mOptions; }
	void			setActions(vector<function<void(void)>> actions) { mActions = actions; }
	vector<function<void(void)>> getActions() const { return mActions; }
	void			setIndex(int index) { mIndex = index; }
	int				getIndex() const { return mIndex; }
	void			setPressed(bool pressed) { mPressed = pressed; }
	bool			getPressed() const { return mPressed; }
	void			setMousePressed(bool mousePressed) { mMousePressed = mousePressed; }
	bool			getMousePressed() const { return mMousePressed; }
	void			setSize(int size) { mSize = size; }
	int				getSize() const { return mSize; }
	void			setControls(vector<Control*> controls) { mControls = controls; }
	vector<Control*>	getControls() const { return mControls; }

	void addOption(string option) { mOptions.push_back(option); }

	virtual int run();
	virtual bool keyProcessing(int code, char unicodeChar, bool bKeyDown);
	virtual bool mouseProcessing(bool pressed, int x, int y);
	virtual void draw();
	virtual void rebuild(); 
	 
	enum Theme { Dark, Light };

	static Theme sTheme;

	static wstring s2ws(const string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		wstring r(buf);
		delete[] buf;
		return r;
	}
	
protected:
	string			mTitle;
	vector<string>	mOptions;
	int				mIndex;
	bool			mPressed;
	bool			mMousePressed;
	int				mSize;
	vector<Control*>	mControls;
	HWND			mhWnd;


private:
	vector<function<void(void)>> mActions;
	 
};