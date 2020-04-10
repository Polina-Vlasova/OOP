#pragma once

#include "Menu.h"
#include "Window.h"

class TableMenu : public Menu
{
public:
	TableMenu() : Menu() {}
	TableMenu(string title, vector<vector<string>> data);
	TableMenu(const TableMenu& tableMenu) : Menu(tableMenu), mData(tableMenu.mData), mCellsWeight(tableMenu.mCellsWeight) {}

	void				setData(vector<vector<string>> data) { mData = data; }
	vector<vector<string>>	getData() const { return mData; }
	void				setCellsWeight(vector<vector<int>> cellsWeight) { mCellsWeight = cellsWeight; }
	vector<vector<int>>		getCellsWeight() const { return mCellsWeight; }

	void draw();
	void drawCell(HDC& hDC, RECT rect, string text);
	bool keyProcessing(int code, char unicodeChar, bool bKeyDown) { return !bKeyDown; } 

	void setCellWeight(int i, int j, int weight) { mCellsWeight[i][j] = weight; }

private:
	vector<vector<string>>	mData;
	vector<vector<int>>		mCellsWeight;
};