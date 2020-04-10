#pragma once

#include <string>

#include "Converter.h"

#include "UI/Date.h"

using namespace std;

class Project
{
public:
	Project() {}
	Project (string name, Date start, Date deadline) :
		mName (name), mStart(start), mDeadline(deadline) {}
	Project (const Project& project) :
		mName(project.mName), mStart(project.mStart), mDeadline(project.mDeadline) {}

	void setName(string name) { mName = name; }
	string getName() const { return mName; }
	void setStart(Date start) { mStart = start; }
	Date getStart() const { return mStart; }
	void setDeadline(Date deadline) { mDeadline = deadline; }
	Date getDeadline() const { return mDeadline; }
	
	friend ostream& operator <<(ostream& ofs, const Project& project) { project.print(ofs); return ofs; }
	friend istream& operator >> (istream& ifs, Project& project) { project.read(ifs); return ifs; }

	virtual istream& read(istream& ifs);
	virtual ostream& print(ostream& ofs) const;

protected:
	string mName;
	Date mStart;
	Date mDeadline;
};
