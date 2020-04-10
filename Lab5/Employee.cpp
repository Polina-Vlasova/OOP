#include "Employee.h"

using namespace Gdiplus;

ofstream& Employee::print(ofstream& ofs) const
{
	Specialist::print(ofs);
	ofs << endl << mCompanyID;
	ofs << endl << mProjects.size();
	for (auto const &proj : mProjects) {
		ofs << endl << proj;
	}
	ofs << endl << mCompletedProjects.size();
	for (auto const &proj : mCompletedProjects) {
		ofs << endl << proj;
	}
	return ofs;
}


ifstream& Employee::read(ifstream& ifs)
{
	Specialist::read(ifs);
	ifs >> mCompanyID;
	int size;
	mProjects.clear();
	ifs >> size;
	for (int i = 0; i < size; i++) {
		Project project;
		ifs >> project;
		mProjects.push_back(project);
	}
	mCompletedProjects.clear();
	ifs >> size;
	for (int i = 0; i < size; i++) {
		CompletedProject project;
		ifs >> project;
		mCompletedProjects.push_back(project);
	}
	return ifs;
}


bool Employee::contain(const Employee& employee) const
{
	bool result = Specialist::contain(employee);
	if (employee.mCompanyID != mCompanyID && employee.mCompanyID != 0)
		result = false;
	return result;
}



void Employee::menu()
{
	Menu(mSurname + string(" ") + mName,
	{ "������ ������",
		"��������� ������",
		"�����������",
		"�����",
		"��������� �����" },
		{ bind(&Employee::startProject, this),
		bind(&Employee::finishProject, this),
		bind(&Employee::showProjects, this),
		bind(&Employee::report, this) }).run();
}



void Employee::startProject()
{
	string name;
	Date start, finish;
	InputMenu menu("������ ������", { "��������", "���� ������", "�������" },
	{ InputMenu::EditType, InputMenu::DateType, InputMenu::DateType });
	bool isSuccessful = false;
	while (!isSuccessful) {
		menu.setData({ {name}, {start}, {finish} });
		menu.run();
		vector<string> res = menu.getResults();
		name = res[0];
		start = res[1];
		finish = res[2];
		if (start > finish) {
			MessageMenu("�����������", "��������� ����").run();
			continue;
		}
		mProjects.push_back(Project(name, start, finish));
		isSuccessful = true;
	}

	MessageMenu("�����������", "������ ������� ��������").run();
}



void Employee::finishProject()
{
	if (mProjects.empty()) {
		MessageMenu("�����������", "�������� �� �������").run();
		return;
	}

	vector<string> data = { "�������" };
	for (auto &proj : mProjects)
		data.push_back(proj.getName());
	InputMenu menu("��������� ������", { "������", "���� ����������" }, { InputMenu::ComboBoxType, InputMenu::DateType }, { data });
	bool isSuccessful = false;
	Date finish;
	while (!isSuccessful) {
		menu.setDataItem(1, { finish });
		menu.run();
		vector<string> res = menu.getResults();
		int ind = stoi(res[0]) - 1;
		finish = res[1];
		if (mProjects[ind].getStart() > finish) {
			MessageMenu("�����������", "��������� ����").run();
			continue;
		}
		mCompletedProjects.push_back(CompletedProject(mProjects[ind], finish));
		mProjects.erase(mProjects.begin() + ind);
		isSuccessful = true;
	}

	MessageMenu("�����������", "������ ��������").run();
}



void Employee::showProjects()
{
	if (mProjects.empty()) {
		MessageMenu("�����������", "�������� �� �������").run();
		return;
	}

	vector<vector<string>> data = { { "��������", "���� ������", "�������", "��������", "���� ���������", "�������" } };
	for (auto &proj : mCompletedProjects)
		data.push_back({ proj.getName(), proj.getStart(), proj.getDeadline(), "��", proj.getEnd(), proj.getEnd() <= proj.getDeadline() ? "��" : "���" });
	for (auto &proj : mProjects)
		data.push_back({ proj.getName(), proj.getStart(), proj.getDeadline(), "���", " ", " " });
	TableMenu("�������", data).run();
}


void Employee::report()
{
	if (mProjects.empty()) {
		MessageMenu("�����������", "�������� �� �������").run();
		return;
	}

	list<list<string>> data = { { "��������", "���� ������", "�������", "��������", "���� ���������", "�������" } };
	for (auto &proj : mCompletedProjects)
		data.push_back({ proj.getName(), proj.getStart(), proj.getDeadline(), "��", proj.getEnd(), proj.getEnd() <= proj.getDeadline() ? "��" : "���" });
	for (auto &proj : mProjects)
		data.push_back({ proj.getName(), proj.getStart(), proj.getDeadline(), "���", " ", " " });
	list<int> w = { 1, 1, 1, 1, 1, 1 };
	TableOutput table("�������", data, w);

	FileManager fm;
	if (fm.openSaveDialog()) {
		string path = fm.getFullPath();
		
		if (fm.getFilter() == ".xlsx") {
			string imagePath = path + ".png";
			saveChart(imagePath);
			path += ".xlsx";
			libxl::Book* book = xlCreateXMLBook();
			book->setKey(L"GCCG", L"windows-282123090cc0e6036db16b60a1o3p0h9");
			if (book)
			{
				libxl::Sheet* sheet = book->addSheet(L"����1");
				if (sheet)
				{
					sheet->setCol(0, 5, 20);
					int row = 0;
					int col = 0;
					table.write(book, sheet, row, col);
					int id = book->addPicture(Converter::toWstring(imagePath).c_str());
					if(id != -1)
						sheet->setPicture(1, 7, id);
				}

				book->save(Menu::s2ws(path).c_str());
				book->release();
				MessageMenu("�����������", "����� ������� ������\n" + path).run();

				int openFileChoice = Menu("������� �����?", { "��", "���" }).run();
				if (openFileChoice == 0) {
					ExcelLoader xls;
					xls.LoadExcel(path, 1);
					xls.Detach();
				}
			}

		}
	}
	else
		MessageMenu("�����������", "����� �� ��� ��������").run();
}



void Employee::saveChart(string path)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap* image = new Bitmap(500, 370);
	Graphics *g = Graphics::FromImage(image);
	Pen * whitePen = new Pen(Color::White, 2);
	SolidBrush * blackBr = new SolidBrush(Color::Black);
	SolidBrush * whiteBr = new SolidBrush(Color::White);
	SolidBrush * redBr = new SolidBrush(Color::Tomato);
	SolidBrush * greenBr = new SolidBrush(Color::PaleGreen);
	SolidBrush * yellowBr = new SolidBrush(Color::Gold);
	Gdiplus::Font * font = new Gdiplus::Font(L"Times New Roman", 12);
	Gdiplus::Font * fontHeader = new Gdiplus::Font(L"Times New Roman", 16); 

	g->FillRectangle(whiteBr, Rect(0, 0, 500, 370));	 

	int success = 0;
	for (auto &proj : mCompletedProjects)
		if (proj.getEnd() <= proj.getDeadline())
			success++;
	double projRate = 360.0 / (mProjects.size() + mCompletedProjects.size()) * mProjects.size();
	double successRate = 360.0 / (mProjects.size() + mCompletedProjects.size()) * success;  

	Rect rect(150, 50, 200, 200);
	g->FillPie(yellowBr, rect, 0, projRate);
	g->FillPie(greenBr, rect, projRate, successRate);
	g->FillPie(redBr, rect, projRate + successRate, 360 - projRate - successRate);
	g->DrawPie(whitePen, rect, 0, projRate);
	g->DrawPie(whitePen, rect, projRate, successRate);
	g->DrawPie(whitePen, rect, projRate + successRate, 360 - projRate - successRate);

	g->DrawString(L"����������", 10, fontHeader, PointF(200, 10), blackBr);
	g->DrawString(L"������� �������", 15, font, PointF(160, 280), blackBr);
	g->DrawString(L"������, ����������� � ����", 26, font, PointF(160, 310), blackBr);
	g->DrawString(L"��������� �������", 17, font, PointF(160, 340), blackBr);

	g->FillRectangle(yellowBr, Rect(130, 280, 20, 20));
	g->FillRectangle(greenBr, Rect(130, 310, 20, 20));
	g->FillRectangle(redBr, Rect(130, 340, 20, 20));

	CLSID myClsId;
	int retVal = Converter::GetEncoderClsid(L"image/png", &myClsId);
	image->Save(Converter::toWstring(path).c_str(), &myClsId, NULL);

	delete font;
	delete fontHeader;
	delete blackBr;
	delete whiteBr;
	delete yellowBr;
	delete redBr;
	delete greenBr;
	delete whitePen;
	delete g;
	delete image;
	 
	GdiplusShutdown(gdiplusToken);
}