#pragma once
 
#include "Person.h"  

class Specialist : public Person
{
public:  
	enum Status { DefaultStatus, Unemployed, Working };
	 
	Specialist(string surname = "", string name = "", string patronymic = "", 
		Date date = 0, Status status = DefaultStatus) :
		Person(surname, name, patronymic, date),   mStatus(status){}
	Specialist(const Specialist& specialist) : Person(specialist),    
		mStatus(specialist.mStatus) {}
	  
	void	setStatus(Status status) { mStatus = status; }
	Status	getStatus() const { return mStatus; }
	  
	string getStatusName() const { return mStatus == DefaultStatus ? "" : sStatuses[mStatus - 1]; }

	friend int cmp(const Specialist& sp1, const Specialist& sp2);
	 
	virtual ofstream& print(ofstream& ofs) const;
	virtual ifstream& read(ifstream& ifs);  
	friend ifstream& operator >> (ifstream& ifs, Status& status);
	 
	bool contain(const Specialist& specialist) const;

protected:
	static const string sStatuses[];

protected:
	Status	mStatus;  
};