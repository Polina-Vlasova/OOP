#pragma once

#include "Menu.h"
#include "Label.h"

class MessageMenu : public Menu
{
public: 
	MessageMenu(string title = "", string message = "");
	MessageMenu(const MessageMenu& messageMenu) : Menu(messageMenu), mMessage(messageMenu.mMessage) {}

	void	setMessage(string message) { mMessage = message; }
	string	getMessage() const { return mMessage; }

	void draw();
	void rebuild(); 

private:
	string mMessage;
};