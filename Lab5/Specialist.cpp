#include "Specialist.h"
  
const string Specialist::sStatuses[] = { "Безработный", "Работает" };


int cmp(const Specialist& sp1, const Specialist& sp2)
{
	int result = cmp((Person&)sp1, (Person&)sp2); 
	if (result == 0) 
		result = sp1.mStatus - sp2.mStatus;
	return result;
}

 

ofstream& Specialist::print(ofstream& ofs) const
{
	Person::print(ofs);
	ofs << endl  << mStatus;
	return ofs;
}


ifstream& Specialist::read(ifstream& ifs)
{
	Person::read(ifs);  
	ifs >> mStatus;
	return ifs;
}
 

ifstream& operator >> (ifstream& ifs, Specialist::Status& status)
{ 
	int temp;
	ifs >> temp;
	status = Specialist::Status(temp);
	return ifs;
}


bool Specialist::contain(const Specialist& specialist) const
{
	bool result = true;
	if (mSurname != specialist.mSurname && specialist.mSurname != "")
		result = false;
	if (mName != specialist.mName && specialist.mName != "")
		result = false;
	if (mPatronymic != specialist.mPatronymic && specialist.mPatronymic != "")
		result = false;
	if (mDate.getDay() != specialist.mDate.getDay() && specialist.mDate.getDay() != 0)
		result = false;
	if (mDate.getMonth() != specialist.mDate.getMonth() && specialist.mDate.getMonth() != 0)
		result = false;
	if (mDate.getYear() != specialist.mDate.getYear() && specialist.mDate.getYear() != 0)
		result = false; 
	if (mStatus != specialist.mStatus && specialist.mStatus != 0)
		result = false;
	return result;
}
