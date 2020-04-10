#include "InputMenu.h"

InputMenu::InputMenu(string title, vector<string> options, vector<ItemType> types) :
	Menu(title, options), mTypes(types)
{
	mData.resize(mOptions.size());
	for (int i = 0; i < mData.size(); i++) {
		mData[i].push_back("");
	}
	mSize = mOptions.size();
	mSearchMode = false;
	mFilterMode = false;
	rebuild();
}


InputMenu::InputMenu(string title, vector<string> options, vector<ItemType> types, vector<vector<string>> data) :
	Menu(title, options), mTypes(types), mData(data)
{
	mData.resize(mOptions.size());
	for (int i = 0; i < mData.size(); i++) {
		if (mData[i].size() == 0)
			mData[i].push_back("");
	}
	mSize = mOptions.size();
	mSearchMode = false;
	mFilterMode = false;
	rebuild();
}


bool InputMenu::keyProcessing(int code, char unicodeChar, bool bKeyDown)
{
	if (mPressed) {
		mPressed = mControls[mIndex]->keyProcessing(code, unicodeChar, bKeyDown);
		/*if (mIndex < mControls.size() - 1) {
			vector<string> data = mControls[mIndex]->getData();
			if(data[0] != "День" && data[0] != "Месяц" && data[0] != "Год")
				mData[mIndex] = mControls[mIndex]->getData();
		}*/
		if (!mPressed && mIndex == mControls.size() - 1 && checkResults()) {
			return true;
		}
		return false;
	}
	if ((code == 72 || code == 75) && bKeyDown) {
		mIndex--;
	}
	else if ((code == 80 || code == 77) && bKeyDown) {
		mIndex++;
	}
	else if (code == 28) {
		mPressed = bKeyDown;
	}
	mIndex = (mIndex + mControls.size()) % mControls.size();

	return false;
}



bool InputMenu::mouseProcessing(bool pressed, int x, int y)
{
	if (pressed) {
		mPressed = !mPressed;
		return false;
	}
	else if (mPressed) {
		if (mIndex == mControls.size() - 1 && checkResults()) {
			return true;
		}
		return false;
	}
	RECT rect;
	GetClientRect(mhWnd, &rect);
	POINT p = { rect.left + (double)(rect.right - rect.left) / 263 * x,
		rect.top + (double)(rect.bottom - rect.top) / 63 * y };
	for (int i = 0; i < mControls.size(); i++) {
		if (PtInRect(&(mControls[i]->Rect()), p))
			mIndex = i;
	}
	return false;
}



void InputMenu::draw()
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
	for (int i = 0; i < mControls.size(); i++) {
		if (mIndex == i)
			mControls[i]->setState(mPressed ? Control::PressedState : Control::SelectedState);
		else
			mControls[i]->setState(Control::EnabledState);
	}

	//int addHeight = 0;
	//for (int i = 0; i < mControls.size(); i++) {
	//	if (addHeight != 0) {
	//		mControls[i]->setTop(mControls[i]->Top() + addHeight);
	//		mControls[i]->setBottom(mControls[i]->Bottom() + addHeight);
	//		if (i < mInactiveControls.size() - 1) {
	//			mInactiveControls[i + 1]->setTop(mInactiveControls[i + 1]->Top() + addHeight);
	//			mInactiveControls[i + 1]->setBottom(mInactiveControls[i + 1]->Bottom() + addHeight);
	//		}
	//	}
	//	if (i != mControls.size() - 1 && mControls[i]->Bottom() + DISTANCE > mControls[i + 1]->Top()) {
	//		addHeight += mControls[i]->Bottom() - mControls[i + 1]->Top() + DISTANCE;
	//	}
	//}
	//if (addHeight != 0)
	//	mInactiveControls[0]->setBottom(mInactiveControls[0]->Bottom() + addHeight);

	for (int i = 0; i < mInactiveControls.size(); i++)
		mInactiveControls[i]->draw(memDC);
	for (int i = mControls.size() - 1; i >= 0; i--)
		mControls[i]->draw(memDC);

	BitBlt(hDC, rect.left, rect.top, rect.right - rect.left,
		rect.bottom - rect.top, memDC, rect.left, rect.top, SRCCOPY);
	SelectObject(memDC, oldFont);
	SelectObject(memDC, oldBitmap);
	DeleteObject(bitmap);
	DeleteObject(backBrush);
	DeleteObject(font);
	DeleteDC(memDC);
	ReleaseDC(mhWnd, hDC);
}


void InputMenu::rebuild()
{
	mInactiveControls.clear();
	mControls.clear();

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
	int editWidth = 260;
	int width = DISTANCE + itemWidth + editWidth + DISTANCE;
	if (mFilterMode)
		width += editWidth;
	int height = CAPTION_SIZE + (BUTTON_HEIGHT + DISTANCE) * (mOptions.size() + 1) + DISTANCE;

	rect.left = (rect.right - rect.left - width) / 2;
	rect.top = 20;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	mInactiveControls.push_back(new Window(rect, mTitle));

	RECT labelRect = { rect.left + DISTANCE,
		rect.top + DISTANCE + CAPTION_SIZE,
		rect.left + itemWidth + DISTANCE,
		rect.top + DISTANCE + CAPTION_SIZE + BUTTON_HEIGHT };
	RECT itemRect = { rect.left + DISTANCE + itemWidth,
		rect.top + DISTANCE + CAPTION_SIZE,
		rect.left + DISTANCE + itemWidth + editWidth,
		rect.top + DISTANCE + CAPTION_SIZE + BUTTON_HEIGHT };
	RECT itemRect2 = { rect.left + DISTANCE + itemWidth + editWidth,
		rect.top + DISTANCE + CAPTION_SIZE,
		rect.left + DISTANCE + itemWidth + 2 * editWidth,
		rect.top + DISTANCE + CAPTION_SIZE + BUTTON_HEIGHT };
	for (int i = 0; i < mOptions.size(); i++) {
		mInactiveControls.push_back(new Label(labelRect, mOptions[i]));
		if (mTypes[i] == LabelType) {
			mInactiveControls.push_back(new Label(itemRect, mData[i][0]));
			if (mFilterMode)
				mInactiveControls.push_back(new Label(itemRect2, mData[i][0]));
		}
		else if (mTypes[i] == EditType || mTypes[i] == NumericEditType || mTypes[i] == PasswordType || mTypes[i] == MultiLineType) {
			mControls.push_back(new Edit(itemRect, mData[i][0], mTypes[i] == NumericEditType ? true :
				false, mTypes[i] == PasswordType ? true : false, mTypes[i] == MultiLineType ? true : false));
			if (mFilterMode)
				mControls.push_back(new Edit(itemRect2, mData[i][0], mTypes[i] == NumericEditType ? true :
					false, mTypes[i] == PasswordType ? true : false, mTypes[i] == MultiLineType ? true : false));
		}
		else if (mTypes[i] == ComboBoxType) {
			mControls.push_back(new ComboBox(itemRect, mData[i]));
			if (mFilterMode)
				mControls.push_back(new ComboBox(itemRect2, mData[i]));
		}
		else if (mTypes[i] == DateType) {
			Date date = mData[i][0];
			RECT dateRect = itemRect;
			dateRect.right = dateRect.left + 85;
			mControls.push_back(new ComboBox(dateRect, Date::sDays));
			((ComboBox*)mControls.back())->setCurSel(date.getDay());
			dateRect.left += 85;
			dateRect.right = dateRect.left + 100;
			mControls.push_back(new ComboBox(dateRect, Date::sMonthes));
			((ComboBox*)mControls.back())->setCurSel(date.getMonth());
			dateRect.left += 100;
			dateRect.right = dateRect.left + 75;
			mControls.push_back(new ComboBox(dateRect, Date::sYears));
			int year = date.getYear() - stoi(Date::sYears[1]);
			((ComboBox*)mControls.back())->setCurSel(max(year, 0));
			if (mFilterMode) {
				dateRect = itemRect2;
				dateRect.right = dateRect.left + 85;
				mControls.push_back(new ComboBox(dateRect, Date::sDays));
				((ComboBox*)mControls.back())->setCurSel(date.getDay());
				dateRect.left += 85;
				dateRect.right = dateRect.left + 100;
				mControls.push_back(new ComboBox(dateRect, Date::sMonthes));
				((ComboBox*)mControls.back())->setCurSel(date.getMonth());
				dateRect.left += 100;
				dateRect.right = dateRect.left + 75;
				mControls.push_back(new ComboBox(dateRect, Date::sYears));
				((ComboBox*)mControls.back())->setCurSel(max(year, 0));
			}
		}
		labelRect.top += BUTTON_HEIGHT + DISTANCE;
		labelRect.bottom += BUTTON_HEIGHT + DISTANCE;
		itemRect.top += BUTTON_HEIGHT + DISTANCE;
		itemRect.bottom += BUTTON_HEIGHT + DISTANCE;
		itemRect2.top += BUTTON_HEIGHT + DISTANCE;
		itemRect2.bottom += BUTTON_HEIGHT + DISTANCE;
	}
	labelRect.right = mFilterMode ? itemRect2.right : itemRect.right;
	mControls.push_back(new Button(labelRect, "Продолжить"));

	SelectObject(hDC, oldFont);
	DeleteObject(font);
	ReleaseDC(mhWnd, hDC);
}



void InputMenu::setComboIndex(int index, int comboIndex)
{
	if (mTypes[index] == ComboBoxType) {
		int j = 0;
		for (int i = 0; i < index && j < mControls.size(); i++) {
			if (mTypes[i] == LabelType)
				continue;
			if (mTypes[i] == DateType)
				j += 3;
			else
				j++;
		}
		if (mFilterMode) {
			((ComboBox*)mControls[j * 2])->setCurSel(comboIndex);
			((ComboBox*)mControls[j * 2 + 1])->setCurSel(comboIndex);
		}
		else
			((ComboBox*)mControls[j])->setCurSel(comboIndex);
	}
}



void InputMenu::setDate(int index, Date date)
{
	if (mTypes[index] == DateType) {
		int j = 0;
		for (int i = 0; i < index - 1 && j < mControls.size(); i++) {
			if (mTypes[i] == LabelType)
				continue;
			if (mTypes[i] == DateType)
				j += 3;
			else
				j++;
		}
		int year = date.getYear() - stoi(Date::sYears[1]) + 1;
		if (mFilterMode) {
			((ComboBox*)mControls[j * 2])->setIndex(date.getDay());
			((ComboBox*)mControls[++j * 2])->setIndex(date.getMonth());
			((ComboBox*)mControls[++j * 2])->setIndex(max(year, 0));
			((ComboBox*)mControls[++j * 2 + 1])->setIndex(date.getDay());
			((ComboBox*)mControls[++j * 2 + 1])->setIndex(date.getMonth());
			((ComboBox*)mControls[++j * 2 + 1])->setIndex(max(year, 0));
		}
		else {
			((ComboBox*)mControls[j++])->setIndex(date.getDay());
			((ComboBox*)mControls[j++])->setIndex(date.getMonth());
			((ComboBox*)mControls[j++])->setIndex(max(year, 0));
		}
	}
}

InputMenu & InputMenu::operator=(const InputMenu & inputMenu)
{
	mTitle = inputMenu.mTitle;
	mOptions = inputMenu.mOptions;
	mIndex = inputMenu.mIndex;
	mPressed = inputMenu.mPressed;
	mMousePressed = inputMenu.mMousePressed;
	mSize = inputMenu.mSize;
	mControls = inputMenu.mControls;
	mhWnd = inputMenu.mhWnd;
	mData = inputMenu.mData;
	mTypes = inputMenu.mTypes;
	mInactiveControls = inputMenu.mInactiveControls;
	mSearchMode = inputMenu.mSearchMode;
	mFilterMode = inputMenu.mFilterMode;
	return *this;
}


vector<string> InputMenu::getResults() const
{
	vector<string> result;
	int index = 0;
	for (int i = 0; i < mOptions.size() && index < mControls.size(); i++) {
		if (mTypes[i] == EditType || mTypes[i] == NumericEditType || mTypes[i] == PasswordType || mTypes[i] == MultiLineType) {
			result.push_back(((Edit*)mControls[index++])->getLegend());
			if (mFilterMode)
				result.push_back(((Edit*)mControls[index++])->getLegend());
		}
		else if (mTypes[i] == ComboBoxType) {
			result.push_back(to_string(((ComboBox*)mControls[index++])->getCurSel()));
			if (mFilterMode)
				result.push_back(to_string(((ComboBox*)mControls[index++])->getCurSel()));
		}
		else if (mTypes[i] == DateType) {
			Date date;
			date.setDay(((ComboBox*)mControls[index++])->getCurSel());
			date.setMonth(((ComboBox*)mControls[index++])->getCurSel());
			if (((ComboBox*)mControls[index])->getCurSel() != 0)
				date.setYear(stoi(((ComboBox*)mControls[index++])->getLegend()));
			result.push_back(date);
			if (mFilterMode) {
				date.setDay(((ComboBox*)mControls[index++])->getCurSel());
				date.setMonth(((ComboBox*)mControls[index++])->getCurSel());
				if (((ComboBox*)mControls[index])->getCurSel() != 0)
					date.setYear(stoi(((ComboBox*)mControls[index++])->getLegend()));
				result.push_back(date);
			}
		}
	}
	return result;
}


bool InputMenu::checkResults()
{
	if (mSearchMode || mFilterMode)
		return true;
	bool result = true;
	int index = 0;
	string errorMessage;
	for (int i = 0; i < mOptions.size() && index < mControls.size(); i++) {
		if (mTypes[i] == EditType || mTypes[i] == NumericEditType || mTypes[i] == PasswordType || mTypes[i] == MultiLineType) {
			if (((Edit*)mControls[index])->getLegend() == "") {
				result = false;
				errorMessage += mOptions[i] + "\n";
			}
			else if (mTypes[i] == NumericEditType) {
				string legend = ((Edit*)mControls[index])->getLegend();
				bool r = true;
				if (legend.size() > 10)
					r = false;
				bool b = true;
				for (int i = 0; i < legend.size() && r; i++) {
					if (legend[i] == '-' && i != 0)
						r = false;
					else if (legend[i] == '.') {
						if (b)
							b = false;
						else
							r = false;
					}
				}
				if (!r) {
					result = false;
					errorMessage += mOptions[i] + "\n";
				}
			}
			else if (mTypes[i] == PasswordType) {
				string legend = ((Edit*)mControls[index])->getLegend();
				int size = legend.size();
				if (size < 6)
					errorMessage += "Пароль должен состоять как минимум из 6 символов\n";
				bool lower = false, upper = false, digit = false;
				for (int i = 0; i < size; i++) {
					if ((legend[i] >= 'a' && legend[i] <= 'z') || (legend[i] >= 'а' && legend[i] <= 'я'))
						lower = true;
					else if ((legend[i] >= 'A' && legend[i] <= 'Z') || (legend[i] >= 'А' && legend[i] <= 'Я'))
						upper = true;
					else if (legend[i] >= '0' && legend[i] <= '9')
						digit = true;
				}
				if (!lower)
					errorMessage += "Пароль должен иметь как минимум один символ нижнего регистра\n";
				if (!upper)
					errorMessage += "Пароль должен иметь как минимум один символ верхнего регистра\n";
				if (!digit)
					errorMessage += "Пароль должен иметь как минимум одну цифру\n";
				result = result && (size >= 6) && lower && upper && digit;
			}
			else if (mTypes[i] == MultiLineType) {
				bool mr = true;
				string legend = ((Edit*)mControls[index])->getLegend();
				for (int i = 0; i < legend.size() - 1 && mr; i++) {
					if (legend[i] == '\n' && legend[i + 1] == '\n') {
						mr = false;
					}
				}
				if (legend.back() == '\n')
					mr = false;
				if (!mr) {
					result = false;
					errorMessage += mOptions[i] + "\n";
				}
			}
			index++;
		}
		else if (mTypes[i] == ComboBoxType) {
			if (((ComboBox*)mControls[index++])->getCurSel() == 0) {
				result = false;
				errorMessage += mOptions[i] + "\n";
			}
		}
		else if (mTypes[i] == DateType) {
			Date date;
			date.setDay(((ComboBox*)mControls[index++])->getCurSel());
			date.setMonth(((ComboBox*)mControls[index++])->getCurSel());
			if (((ComboBox*)mControls[index++])->getCurSel() != 0)
				date.setYear(stoi(((ComboBox*)mControls[index - 1])->getLegend()));
			if (!date.isCorrect()) {
				result = false;
				errorMessage += mOptions[i] + "\n";
			}
		}
	}
	if (!result) {
		MessageMenu("Внимание", string("Пожалуйста, заполните корректно следующие поля:\n") + errorMessage).run();
	}
	return result;
}



InputMenu::~InputMenu()
{
	for (int i = 0; i < mInactiveControls.size(); i++)
		delete mInactiveControls[i];
}