#include "Person.h"

bool operator> (const Person& person1, const Person& person2)
{
	return cmp(person1, person2) > 0;
}


bool operator< (const Person& person1, const Person& person2)
{
	return cmp(person1, person2) < 0;
}


bool operator== (const Person& person1, const Person& person2)
{
	return cmp(person1, person2) == 0;
}

Person::operator string() const
{
	string result = mSurname + string(" ") + mName + string(" ") + mPatronymic;
	return result;
}

int cmp(const Person& person1, const Person& person2)
{
	int result = strcmp(person1.mSurname.c_str(), person2.mSurname.c_str());
	if (!result)
		result = strcmp(person1.mName.c_str(), person2.mName.c_str());
	if (!result)
		result = strcmp(person1.mPatronymic.c_str(), person2.mPatronymic.c_str());
	if (!result)
		result = cmp(person1.mDate, person2.mDate);
	return result;
}


ifstream& Person::read(ifstream& ifs)
{
	ifs >> mSurname;
	ifs >> mName;
	ifs >> mPatronymic;
	ifs >> mDate;
	return ifs;
}


ofstream& Person::print(ofstream& ofs) const
{
	ofs << mSurname << endl
		<< mName << endl
		<< mPatronymic << endl
		<< mDate;
	return ofs;
}
