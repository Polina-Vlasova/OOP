#pragma once

#include "Project.h"

class CompletedProject : public Project
{
public:
	CompletedProject() {}
	CompletedProject(string name, Date start, Date deadline, Date end) :
		Project(name, start, deadline), mEnd(end) {}
	CompletedProject(Project project, Date end) :
		Project(project), mEnd(end) {}
	CompletedProject(const CompletedProject& project) : 
		Project(project), mEnd(project.mEnd) {}

	void setEnd(Date end) { mEnd = end; }
	Date getEnd() const { return mEnd; }


	virtual istream& read(istream& ifs);
	virtual ostream& print(ostream& ofs) const;

private:
	Date mEnd;
};