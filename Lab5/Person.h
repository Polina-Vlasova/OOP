#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <map>
#include <set>
#include <list>
#include <Windows.h> 
 

#include "UI/InputMenu.h"
#include "UI/TableMenu.h"

using namespace std;

class Person 
{ 
public:
	Person(string surname = "", string name = "", string patronymic = "", Date date = 0) : 
		mSurname(surname), mName(name), mPatronymic(patronymic), mDate(date) {}
	Person(const Person& person) : mSurname(person.mSurname), mName(person.mName), 
		mPatronymic(person.mPatronymic), mDate(person.mDate) {}

	void	setSurname(string surname) { mSurname = surname; }
	string	getSurname() const { return mSurname; }
	void	setName(string name) { mName = name; }
	string	getName() const { return mName; }
	void	setPatronymic(string patronymic) { mPatronymic = patronymic; }
	string	getPatronymic() const { return mPatronymic; }
	void	setDate(Date date) { mDate = date; }
	Date	getDate() const { return mDate; }

	friend bool operator>  (const Person& person1, const Person& person2);
	friend bool operator<  (const Person& person1, const Person& person2);
	friend bool operator== (const Person& person1, const Person& person2);
	
	operator string() const; 

	friend ofstream& operator <<(ofstream& ofs, const Person& person) { person.print(ofs); return ofs; }
	friend ifstream& operator >> (ifstream& ifs, Person& person) { person.read(ifs); return ifs; }

	virtual ifstream& read(ifstream& ifs);
	virtual ofstream& print(ofstream& ofs) const;
	 
	friend int cmp(const Person& person1, const Person& person2);
	friend bool cmpSurname(const Person& p1, const Person& p2) { return strcmp(p1.mSurname.c_str(), p2.mSurname.c_str()) < 0; }
	friend bool cmpName(const Person& p1, const Person& p2) { return strcmp(p1.mName.c_str(), p2.mName.c_str()) < 0; }
	friend bool cmpPatronymic(const Person& p1, const Person& p2) { return strcmp(p1.mPatronymic.c_str(), p2.mPatronymic.c_str()) < 0; }
	friend bool cmpDate(const Person& p1, const Person& p2) { return cmp(p1.mDate, p2.mDate) < 0; }
	 
protected:
	string  mSurname;
	string	mName;
	string  mPatronymic;
	Date    mDate;
};

