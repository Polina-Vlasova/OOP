#pragma once

#include "Button.h"
#include "ComboBox.h"
#include "Edit.h"
#include "Date.h"
#include "MessageMenu.h"

class InputMenu : public Menu
{
public:
	enum ItemType { LabelType, EditType, ComboBoxType, ButtonType, DateType, NumericEditType, PasswordType, MultiLineType };

	InputMenu() : Menu() {}
	InputMenu(string title, vector<string> options, vector<ItemType> types);
	InputMenu(string title, vector<string> options, vector<ItemType> types, vector<vector<string>> data);
	InputMenu(const InputMenu& inputMenu) : Menu(inputMenu), mData(inputMenu.mData), mTypes(inputMenu.mTypes),
		mInactiveControls(inputMenu.mInactiveControls), mSearchMode(inputMenu.mSearchMode), mFilterMode(inputMenu.mFilterMode) {}
	~InputMenu();

	void				setData(vector<vector<string>> data) { mData = data; }
	vector<vector<string>>	getData() const { return mData; }
	void				setSearchMode(bool searchMode) { mSearchMode = searchMode; }
	bool				getSearchMode() const { return mSearchMode; }
	void				setFilterMode(bool filterMode) { mFilterMode = filterMode; rebuild(); }
	bool				getFilterMode() const { return mFilterMode; }
	 
	void			setDataItem(int index, vector<string> data) { mData[index] = data; rebuild(); }
	vector<string>	getResults() const;
	void			setComboIndex(int index, int comboIndex);
	void			setDate(int index, Date date);

	InputMenu& operator= (const InputMenu& inputMenu);

	bool keyProcessing(int code, char unicodeChar, bool bKeyDown);
	bool mouseProcessing(bool pressed, int x, int y);
	void draw();
	void rebuild();
	bool checkResults(); 
	
private:
	vector<vector<string>>	mData; 
	vector<ItemType>		mTypes;
	vector<Control*>		mInactiveControls;
	bool				mSearchMode;
	bool				mFilterMode;
};