#include "Application.h"


void Application::menu()
{
	Menu("Лабораторная №5",
	{ "IT-компании", "Специалисты", "Сотрудники", "Выйти" },
	{ bind(&Application::companyMenu, this),
		bind(&Application::specialistMenu, this),
		bind(&Application::employeeMenu, this) }).run();
}



void Application::companyMenu()
{
	Menu("IT-компании",
	{ "Выбрать",
		"Добавить",
		"Удалить",
		"Список",
		"Вернуться назад" },
		{ bind(&Application::openCompany, this),
		bind(&Application::addCompany, this),
		bind(&Application::removeCompany, this),
		bind(&Application::showCompany, this) }).run();
}



void Application::specialistMenu()
{
	Menu("Специалисты",
	{ "Добавить",
		"Изменить",
		"Найти",
		"Удалить",
		"Список",
		"Вернуться назад" },
		{ bind(&Application::addSpecialist, this),
		bind(&Application::editSpecialist, this),
		bind(&Application::findSpecialistList, this),
		bind(&Application::removeSpecialist, this),
		bind(&Application::showSpecialist, this) }).run();
}



void Application::employeeMenu()
{
	Menu("Работники",
	{ "Сортировать" ,
		"Список" ,
		"Вернуться назад" },
		{ bind(&Application::sortEmployee, this),
			bind(&Application::showEmployee, this, 0) }).run();
}


void Application::openCompany()
{
	if (mCompanies.size() == 0) {
		MessageMenu("Ошибка", "Компаний не найдено!").run();
		return;
	}

	vector<string> data = { { "Выбрать" } };
	for (auto &company : mCompanies) {
		data.push_back({ company.second.getName() });
	}
	InputMenu menu("Удалить ИТ-компанию", { "Название" }, { InputMenu::ComboBoxType }, { data });
	menu.run();
	vector<string> res = menu.getResults();
	int index = stoi(res[0]) - 1;
	auto it = next(mCompanies.begin(), index);

	Menu("IT-компания \"" + it->second.getName() + "\"",
	{ "Нанять сотрудника",
		"Уволить сотрудника",
		"Список сотрудников",
		"Выбрать сотрудника",
		"Вернуться назад" },
		{ bind(&Application::addEmployee, this, it->first),
		bind(&Application::removeEmployee, this, it->first),
		bind(&Application::showEmployee, this, it->first),
		bind(&Application::openEmployee, this, it->first) }).run();
}



void Application::addCompany()
{
	InputMenu menu("Добавить IT-компанию", { "Название" }, { InputMenu::EditType });
	menu.run();
	vector<string> results = menu.getResults();
	ITCompany company(results[0]);
	mCompanies.insert({ company.getID(), company });

	MessageMenu("Уведомление", string("Данные добавлены успешно")).run();
}



void Application::removeCompany()
{
	if (mCompanies.size() == 0) {
		MessageMenu("Ошибка", "Компаний не найдено!").run();
		return;
	}

	vector<string> data = { { "Выбрать"} };
	for (auto &company : mCompanies) {
		data.push_back({ company.second.getName() });
	}
	InputMenu menu("Удалить ИТ-компанию", { "Название" }, { InputMenu::ComboBoxType }, { data });
	menu.run();
	vector<string> res = menu.getResults();
	int index = stoi(res[0]) - 1;
	auto it = next(mCompanies.begin(), index);

	for (auto &employee = mEmployees.begin(); employee != mEmployees.end(); employee++) {
		if (employee->getCompanyID() == it->first) {
			auto itSp = mSpecialists.find(*employee);
			Specialist sp(*itSp);
			sp.setStatus(Specialist::Status::Unemployed);
			mSpecialists.erase(itSp);
			mSpecialists.insert(sp);
			mEmployees.erase(employee);
		}
	}
	mCompanies.erase(it);

	MessageMenu("Уведомление", string("Данные удалены успешно")).run();
}




void Application::showCompany()
{
	map<int, int> amount;
	for (auto const & employee : mEmployees)
	{
		if (amount.count(employee.getCompanyID()) == 0)
			amount.insert({ employee.getCompanyID(), 0 });
		amount[employee.getCompanyID()]++;
	}

	vector<vector<string>> data = { { "Название", "Количество сотрудников" } };
	for (auto &company : mCompanies) {
		data.push_back({ company.second.getName(), to_string(amount[company.first]) });
	}
	if (mCompanies.size() == 0)
		MessageMenu("Ошибка", "Компаний не найдено!").run();
	else
		TableMenu("Компании", data).run();
}




void Application::addSpecialist()
{
	vector<string> results;
	vector<vector<string>> data;
	bool exit = false;
	while (!exit) {
		InputMenu menu("Добавить специалиста",
		{ "Фамилия", "Имя", "Отчество", "Дата рождения" },
		{ InputMenu::EditType, InputMenu::EditType, InputMenu::EditType, InputMenu::DateType }, data);
		menu.run();
		results = menu.getResults();
		data = menu.getData();
		try {
			if (Date(results[3]) > Date::currentDate() - Date(18, 0, 0))
				throw runtime_error("Специалист должен быть совершеннолетним!");
			exit = true;
		}
		catch (exception& e) {
			MessageMenu("Ошибка", e.what()).run();
		}
	}
	Specialist specialist(results[0], results[1], results[2], Date(results[3]), Specialist::Status::Unemployed);
	mSpecialists.insert(specialist);

	MessageMenu("Уведомление", string("Данные добавлены успешно")).run();
}



void Application::editSpecialist()
{
	if (mSpecialists.size() == 0) {
		MessageMenu("Ошибка", "Специалистов не найдено!").run();
		return;
	}

	int choice = Menu("Изменить специалиста", { "Выбрать из списка", "Найти", "Вернуться назад" }).run();
	int index;
	set<Specialist> chosen;
	switch (choice) {
	case 0:
		chosen = mSpecialists;
		break;
	case 1:
		chosen = chooseSpecialist();
		break;
	case 2:
		return;
	}

	if (chosen.size() == 0) {
		MessageMenu("Ошибка", "Специалистов не найдено!").run();
		return;
	}
	vector<string> chosenInfo;
	for (auto const &sp : chosen)
		chosenInfo.push_back(sp.getSurname() + " " + sp.getName() + " " + sp.getPatronymic());
	index = Menu("Изменить специалиста", chosenInfo).run();
	auto itSp = mSpecialists.find(*next(chosen.begin(), index));

	vector<string> results;
	vector<vector<string>> data = { { itSp->getSurname()}, {itSp->getName()},
	{itSp->getPatronymic()}, {(string)itSp->getDate()} };

	bool exit = false;
	while (!exit) {
		InputMenu menu("Изменить специалиста",
		{ "Фамилия", "Имя", "Отчество", "Дата рождения" },
		{ InputMenu::EditType, InputMenu::EditType, InputMenu::EditType, InputMenu::DateType }, data);
		menu.run();
		results = menu.getResults();
		data = menu.getData();
		try {
			if (Date(results[3]) > Date::currentDate() - Date(18, 0, 0))
				throw runtime_error("Специалист должен быть совершеннолетним!");
			exit = true;
		}
		catch (exception& e) {
			MessageMenu("Ошибка", e.what()).run();
		}
	}

	Specialist::Status status = itSp->getStatus();
	if (status == Specialist::Status::Working)
	{
		for (auto &emp : mEmployees) {
			if ((Specialist)emp == *itSp) {
				emp.setSurname(results[0]);
				emp.setName(results[1]);
				emp.setPatronymic(results[2]);
				emp.setDate(results[3]);
			}
		}
	}

	mSpecialists.erase(itSp);
	mSpecialists.insert(Specialist(results[0], results[1], results[2], Date(results[3]), status));

	MessageMenu("Уведомление", "Данные изменены успешно").run();
}



set<Specialist> Application::chooseSpecialist()
{
	InputMenu menu("Найти специалиста",
	{ "Фамилия", "Имя", "Отчество", "Дата рождения" },
	{ InputMenu::EditType, InputMenu::EditType, InputMenu::EditType, InputMenu::DateType });
	menu.setSearchMode(true);
	menu.run();

	vector<string> data = menu.getResults();
	Specialist specialist(data[0], data[1], data[2], Date(data[3]));

	set<Specialist> chosen;
	for (auto const &sp : mSpecialists) {
		if (sp.contain(specialist))
			chosen.insert(sp);
	}
	return chosen;
}




void Application::findSpecialistList()
{
	if (mSpecialists.size() == 0) {
		MessageMenu("Ошибка", "Специалистов не найдено!").run();
		return;
	}

	auto chosen = chooseSpecialist();
	vector<vector<string>> data = { { "Фамилия", "Имя", "Отчество", "Дата рождения", "Статус работы" } };
	for (auto &sp : chosen) {
		data.push_back({ sp.getSurname(), sp.getName(), sp.getPatronymic(), (string)sp.getDate(), sp.getStatusName() });
	}
	if (chosen.size() == 0)
		MessageMenu("Ошибка", "Специалистов не найдено!").run();
	else
		TableMenu("Специалисты", data).run();
}





void Application::removeSpecialist()
{
	if (mSpecialists.size() == 0) {
		MessageMenu("Ошибка", "Специалистов не найдено!").run();
		return;
	}

	int choice = Menu("Удалить специалиста", { "Выбрать из списка", "Найти", "Вернуться назад" }).run();
	int index;
	set<Specialist> chosen;
	switch (choice) {
	case 0:
		chosen = mSpecialists;
		break;
	case 1:
		chosen = chooseSpecialist();
		break;
	case 2:
		return;
	}

	if (chosen.size() == 0) {
		MessageMenu("Ошибка", "Специалистов не найдено!").run();
		return;
	}
	vector<string> chosenInfo;
	for (auto const &sp : chosen)
		chosenInfo.push_back(sp.getSurname() + " " + sp.getName() + " " + sp.getPatronymic());
	index = Menu("Изменить специалиста", chosenInfo).run();
	auto itSp = mSpecialists.find(*next(chosen.begin(), index));


	Specialist::Status status = itSp->getStatus();
	if (status == Specialist::Status::Working)
	{
		for (auto emp = mEmployees.begin(); emp != mEmployees.end(); emp++) {
			if ((Specialist)*emp == *itSp) {
				mEmployees.erase(emp);
			}
		}
	}

	mSpecialists.erase(itSp);
	MessageMenu("Уведомление", "Данные удалены успешно").run();
}




void Application::showSpecialist()
{
	vector<vector<string>> data = { {"Фамилия", "Имя", "Отчество", "Дата рождения", "Статус работы"} };
	for (auto &sp : mSpecialists) {
		data.push_back({ sp.getSurname(), sp.getName(), sp.getPatronymic(), (string)sp.getDate(), sp.getStatusName() });
	}
	if (mSpecialists.size() == 0)
		MessageMenu("Ошибка", "Специалистов не найдено!").run();
	else
		TableMenu("Специалисты", data).run();
}



void Application::addEmployee(int id)
{
	vector<Specialist> specialists;
	for (auto const& sp : mSpecialists) {
		if (sp.getStatus() == Specialist::Status::Unemployed) {
			specialists.push_back(sp);
		}
	}

	if (specialists.size() == 0) {
		MessageMenu("Ошибка", "Специалистов не найдено!").run();
		return;
	}

	vector<string> chosenInfo;
	for (auto const &sp : specialists)
		chosenInfo.push_back(sp.getSurname() + " " + sp.getName() + " " + sp.getPatronymic());
	int index = Menu("Изменить специалиста", chosenInfo).run();

	auto it = mSpecialists.find(specialists[index]);
	Specialist sp(*it);
	mSpecialists.erase(it);
	sp.setStatus(Specialist::Status::Working);
	mSpecialists.insert(sp);

	mEmployees.push_back(Employee(sp, id));
	MessageMenu("Уведомление", "Данные добавлены успешно").run();
}



void Application::removeEmployee(int id)
{
	vector<string> info;
	for (auto const &emp : mEmployees) {
		if (emp.getCompanyID() == id)
			info.push_back(emp.getSurname() + " " + emp.getName());
	}

	if (info.size() == 0) {
		MessageMenu("Ошибка", "Сотрудников не найдено!").run();
		return;
	}

	int index = Menu("Удалить специалиста", info).run();

	int curIndex = 0;
	for (auto employee = mEmployees.begin(); employee != mEmployees.end(); employee++) {
		if (employee->getCompanyID() == id) {
			if (curIndex == index) {
				auto itSp = mSpecialists.find(*employee);
				Specialist sp(*itSp);
				sp.setStatus(Specialist::Status::Unemployed);
				mSpecialists.erase(itSp);
				mSpecialists.insert(sp);
				mEmployees.erase(employee);
				MessageMenu("Уведомление", "Сотрудник уволен").run();
				break;
			}
			curIndex++;
		}
	}
}



void Application::sortEmployee()
{
	int choice = Menu("Сортировать сотрудников", { "Фамилия", "Имя", "Отчество", "Дата рождения" }).run();
	vector<function<bool(Employee e1, Employee e2)>> cmp = { cmpSurname, cmpName, cmpPatronymic, cmpDate };
	mEmployees.sort(cmp[choice]);
	showEmployee();
}



void Application::showEmployee(int id)
{
	vector<vector<string>> data = { { "Фамилия", "Имя", "Отчество", "Дата рождения", "Название ИТ-компании" } };
	for (auto const &emp : mEmployees) {
		if (id == 0 || id == emp.getCompanyID())
			data.push_back({ emp.getSurname(), emp.getName(), emp.getPatronymic(), emp.getDate(), mCompanies[emp.getCompanyID()].getName() });
	}
	if (data.size() == 1)
		MessageMenu("Ошибка", "Сотрудников не найдено!").run();
	else
		TableMenu("Сотрудники", data).run();
}

void Application::openEmployee(int id)
{
	vector<string> data;
	for (auto const &emp : mEmployees)
		if (id == emp.getCompanyID())
			data.push_back(emp.getSurname() + " " + emp.getName() + " " + emp.getPatronymic());
	int index = Menu("Изменить специалиста", data).run();

	int curIndex = 0;
	for (auto employee = mEmployees.begin(); employee != mEmployees.end(); employee++) {
		if (employee->getCompanyID() == id) {
			if (curIndex == index) {
				employee->menu();
				break;
			}
			curIndex++;
		}
	}
}



void Application::readInfo()
{
	ifstream ifsSp("specialists.txt");
	if (ifsSp.is_open()) {
		while (!ifsSp.eof()) {
			Specialist sp;
			ifsSp >> sp;
			if (sp.getName() != "")
				mSpecialists.insert(sp);
		}
		ifsSp.close();
	}

	ifstream ifsComp("companies.txt");
	if (ifsComp.is_open()) {
		while (!ifsComp.eof()) {
			ITCompany company;
			ifsComp >> company;
			if (company.getID() != 0) {
				mCompanies.insert({ company.getID(), company });
				ITCompany::companyID = company.getID() + 1;
			}
		}
		ifsComp.close();
	}

	ifstream ifsEmp("employees.txt");
	if (ifsEmp.is_open()) {
		while (!ifsEmp.eof()) {
			Employee emp;
			ifsEmp >> emp;
			if (emp.getName() != "")
				mEmployees.push_back(emp);
		}
		ifsEmp.close();
	}
}


void Application::writeInfo()
{
	ofstream ofsSp("specialists.txt");
	if (ofsSp.is_open()) {
		for (auto const & sp : mSpecialists)
			ofsSp << sp << endl;
		ofsSp.close();
	}

	ofstream ofsComp("companies.txt");
	if (ofsComp.is_open()) {
		for (auto const & company : mCompanies)
			ofsComp << company.second << endl;
		ofsComp.close();
	}

	ofstream ofsEmp("employees.txt");
	if (ofsEmp.is_open()) {
		for (auto const & emp : mEmployees)
			ofsEmp << emp << endl;
		ofsEmp.close();
	}
}



void Application::writeInfoTable()
{
	ofstream ofsSp("specialists.txt");
	if (ofsSp.is_open()) {
		list<list<string>> data = { { "Фамилия", "Имя", "Отчество", "Статус" } };
		for (auto const & sp : mSpecialists)
			data.push_back({ sp.getSurname(), sp.getName(), sp.getPatronymic(), sp.getStatusName() });
		ofsSp.close();
	}

	ofstream ofsComp("companies.txt");
	if (ofsComp.is_open()) {
		for (auto const & company : mCompanies)
			ofsComp << company.second << endl;
		ofsComp.close();
	}

	ofstream ofsEmp("employees.txt");
	if (ofsEmp.is_open()) {
		for (auto const & emp : mEmployees)
			ofsEmp << emp << endl;
		ofsEmp.close();
	}
}
