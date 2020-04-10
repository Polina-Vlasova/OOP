#include "CompletedProject.h"

istream & CompletedProject::read(istream & ifs)
{
	Project::read(ifs);
	ifs >> mEnd;
	return ifs;
}

ostream & CompletedProject::print(ostream & ofs) const
{
	Project::print(ofs);
	ofs << endl << mEnd;
	return ofs;
}
