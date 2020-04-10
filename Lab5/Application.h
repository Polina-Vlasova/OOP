#pragma once

#include <fstream>
#include <cstring>
#include <algorithm>
 
#include "Employee.h" 
#include "ITCompany.h" 

using namespace std;

class Application
{
public:
	Application()  {}
	Application(const Application& administrator) :  mSpecialists(administrator.mSpecialists),
		mEmployees(administrator.mEmployees) {}


	void setEmployees(list<Employee> employees) { mEmployees = employees; }
	list <Employee>	getEmployees() const { return mEmployees; }

	void setSpecialists(set<Specialist> specialists) { mSpecialists = specialists; }
	set <Specialist> getSpecialists() const { return mSpecialists; }

	void setCompanies(map <int, ITCompany> companies) { mCompanies = companies; }
	map <int, ITCompany> getCompanies() const { return mCompanies; }

	 
	void menu();

	void readInfo();
	void writeInfo(); 

	void companyMenu();
	void specialistMenu();
	void employeeMenu();

	void openCompany();
	void addCompany();
	void removeCompany();
	void showCompany();

	void addSpecialist();
	void editSpecialist();
	void findSpecialistList(); 
	void removeSpecialist(); 
	void showSpecialist();

	set<Specialist> chooseSpecialist(); 

	void addEmployee(int id);  
	void removeEmployee(int id);
	void openEmployee(int id);
	void sortEmployee();
	void showEmployee(int id = 0); 

	void writeInfoTable();
	 
	   

private:
	set <Specialist> mSpecialists; 
	list <Employee>	mEmployees;
	map <int, ITCompany> mCompanies;
};