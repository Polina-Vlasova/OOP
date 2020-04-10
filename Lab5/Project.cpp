#include "Project.h"

istream & Project::read(istream & ifs)
{
	ifs >> mName
		>> mStart
		>> mDeadline;
	return ifs;
}

ostream & Project::print(ostream & ofs) const
{
	ofs << mName << endl
		<< mStart << endl
		<< mDeadline;
	return ofs;
}
