#pragma once

#include <sstream>

#import "C:\Program Files\Microsoft Office\root\vfs\ProgramFilesCommonX86\Microsoft Shared\OFFICE16\MSO.DLL" \
rename("RGB","_RGB")\
rename("DocumentProperties", "_DocumentProperties")\
rename("SearchPath","_SearchPath")

#import "C:\Program Files\Microsoft Office\root\vfs\ProgramFilesCommonX86\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"

#import "C:\Program Files\Microsoft Office\root\Office16\EXCEL.EXE" \
rename("DialogBox","_DialogBox") \
rename("RGB","_RGB") \
rename("CopyFile", "_CopyFile") \
rename("ReplaceText", "_ReplaceText") \
no_auto_exclude

#define THROW(msg) throw std::exception(msg);

/**
*
*/
class ExcelLoader {
public:
	virtual ~ExcelLoader() {
		Close();
	}

	Excel::_ApplicationPtr excel_;
	Excel::_WorksheetPtr sheet_;
	Excel::RangePtr range_; 

	//отвязываение от Excel, чтобы решение о закрытии и сохранении принял пользователь
	void Detach() {
		if (!excel_)
			return;

		range_.Detach();
		range_ = 0;

		sheet_.Detach();
		sheet_ = 0;

		excel_.Detach();
		excel_ = 0;

		::CoUninitialize();
	}

	//закрывает без сохранения ранее открытую книгу через LoadExcel
	void Close() {
		if (!excel_)
			return;

		try {
			excel_->DisplayAlerts[0] = false;
			excel_->Quit();
			excel_->DisplayAlerts[0] = true;

			Detach();

			//todo: с MS Excel 2010 обнаружилось, что CoUninitialize не всегда закрывает процесс
			std::system("taskkill /F /IM Excel.exe");

			//ожидаем закрытия процесса EXCEL
			while (FindWindow((LPCWSTR)"XLMAIN", NULL)) {};
		}
		catch (_com_error &er) {
			THROW((char*)er.ErrorMessage());
		}
	}

	//сохраняет ранее открытую книгу через LoadExcel
	void Save() {
		if (!excel_)
			return;

		excel_->DisplayAlerts[0] = false;
		try {
			excel_->Save();
		}
		catch (_com_error &er) {
			THROW((char*)er.ErrorMessage());
		}
		excel_->DisplayAlerts[0] = true;
	}

	//инстанцирует Excel и открывает лист с индексом ws_index (>= 1) в книге fname
	void LoadExcel(const std::string &fname, int ws_index) {
		if (FAILED(::CoInitialize(NULL)))
			THROW("CoInitialize failure");

		if (FAILED(excel_.CreateInstance("Excel.Application"))) {
			std::stringstream ss;
			ss << "CreateInstance failed: " << GetLastError();
			std::string msg = ss.str();
			THROW(msg.c_str());
		}

		excel_->Visible[0] = TRUE;

		Excel::_WorkbookPtr book = excel_->Workbooks->Open(fname.c_str());
		if (!book)
			THROW(std::string("Can't open ").append(fname).append(": Workbooks->Open method failed").c_str());

		sheet_ = excel_->ActiveSheet;
		if (ws_index < 1 || excel_->Sheets->Count < ws_index)
			THROW("ws_index_ must be in [1, Sheets.Count]");

		sheet_ = excel_->Sheets->Item[ws_index];

		if (!sheet_)
			THROW("Failed to get a pointer to the active sheet");

		range_ = sheet_->Cells;
		if (!range_)
			THROW("Failed to get a pointer to the cells on the active sheet");
	}
};
