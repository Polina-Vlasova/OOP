#pragma once

#include <string>
#include <ctime>
#include <Windows.h> 

#include "Converter.h"

using namespace std;

class FileManager
{
public:
	FileManager(string name = "") : mName(name) {}
	FileManager(const FileManager& manager) : mName(manager.mName) {}

	void setName(string name) { mName = name; }
	string getName() const { return mName; }
	void setFullPath(string path) { mFullPath = path; }
	string getFullPath() const { return mFullPath; }
	void setFilter(string filter) { mFilter = filter; }
	string getFilter() const { return mFilter; }

	bool openSaveDialog()
	{
		time_t now = time(0);
		tm gmtm;
		gmtime_s(&gmtm, &now);
		wchar_t filename[260];
		wchar_t	date[260];
		if (mName == "") {
			wsprintf(date, L"%04d%02d%02d_%02d%02d%02d", 1900 + gmtm.tm_year, gmtm.tm_mon, gmtm.tm_mday,
				gmtm.tm_hour, gmtm.tm_min, gmtm.tm_sec);
		}
		else
		{
			lstrcpyW(date, Converter::toWstring(mName).c_str());
		}
		lstrcpyW(filename, date);

		OPENFILENAME ofn;
		char szFileName[MAX_PATH] = "";

		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = L"ќбычный текст (*.txt)\0*.txt\0 нига Excel (*.xlsx)\0*.xlsx\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		ofn.lpstrTitle = L"—охранить отчЄт как";
		GetSaveFileName(&ofn);

		if (lstrcmpW(date, ofn.lpstrFile) == 0)
			return false;

		mFullPath = Converter::toString(filename);
		if (ofn.nFilterIndex == 1)
		{
			mFilter = ".txt"; 
		}
		else {
			mFilter = ".xlsx";
		}
		return true;
	}

private:
	string mName;
	string mFullPath;
	string mFilter;
};