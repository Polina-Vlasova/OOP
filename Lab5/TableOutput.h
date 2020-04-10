#pragma once

#include <string>
#include <list>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <Windows.h>

#include "libxl.h"

#include "Converter.h"
 

using namespace std; 

class TableOutput
{
public:
	TableOutput(string name, list<list<string>> data, list<int> widths);
	TableOutput(const TableOutput& table) :
		mName(table.mName), mData(table.mData), mWidths(table.mWidths), mWidth(table.mWidth) {}

	libxl::Format* createFormat(libxl::Book* book);
	libxl::Format* createFormatName(libxl::Book* book);
	libxl::Format* createFormatHeader(libxl::Book* book);
	libxl::Format* createFormatData(libxl::Book* book);

	void write(libxl::Book* book, libxl::Sheet* sheet, int& row, int& col, bool bVertical = false);
	void write(ostream& out, bool bVertical = false);



private:
	string mName; 
	list<list<string>> mData;
	list<int> mWidths;
	int mWidth;
};