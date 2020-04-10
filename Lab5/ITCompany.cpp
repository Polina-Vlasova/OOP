#include "ITCompany.h"

int ITCompany::companyID = 100;


ofstream & operator<<(ofstream & ofs, const ITCompany & company)
{
	ofs << company.mID << endl << company.mName;
	return ofs;
}

ifstream & operator >> (ifstream & ifs, ITCompany & company)
{
	ifs >> company.mID >> company.mName;
	return ifs;
}
