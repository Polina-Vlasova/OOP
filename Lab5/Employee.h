#pragma once

 
#include "Specialist.h"
#include "CompletedProject.h"

#include "TableOutput.h"
#include "FileManager.h"
#include "MSExcel.h"
 


class Employee : public Specialist
{
public: 

	Employee() : Specialist() { };
	Employee(string surname, string name,
		string patronymic, Date date, Status status, int companyID) :
		Specialist(surname, name, patronymic, date,  status), mCompanyID (companyID){}
	Employee(const Employee& employee) : Specialist(employee), 
		mCompanyID(employee.mCompanyID), mCompletedProjects(employee.mCompletedProjects), mProjects(employee.mProjects) {}
	Employee(const Specialist& specialist,  int companyID) : Specialist(specialist),
		mCompanyID(companyID) {}
	  
	void setCompanyID(int id) { mCompanyID = id; }
	int getCompanyID() const { return mCompanyID; }

	virtual ofstream& print(ofstream& ofs) const;
	virtual ifstream& read(ifstream& ifs);
	  
	bool contain(const Employee& employee) const;

	void menu();

	void startProject();
	void finishProject();
	void showProjects();
	void report();
	void saveChart(string path);

	friend int cmp(const Employee& emp1, const Employee& emp2) { return emp1.mCompanyID - emp2.mCompanyID; }



protected:   
	int	mCompanyID;
	vector<CompletedProject> mCompletedProjects;
	vector<Project> mProjects;
};