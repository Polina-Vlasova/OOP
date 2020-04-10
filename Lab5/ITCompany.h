#pragma once

#include "Employee.h"

class ITCompany
{
public:
	ITCompany() : mID(0) {}
	ITCompany(string name) : mName(name), mID (companyID++) {}
	ITCompany(const ITCompany& company) : 
		mName(company.mName), mID(company.mID) {}


	void	setName(string name) { mName = name; }
	string	getName() const { return mName; } 

	void setID (int ID) { mID = ID; };
	int getID() const { return mID; }


	friend ofstream& operator << (ofstream& ofs, const ITCompany& company);
	friend ifstream& operator >> (ifstream& ifs, ITCompany& company);
	 

	static int companyID;

private:
	string mName;
	int mID;
};