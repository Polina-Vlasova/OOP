#include "Date.h"

const vector<string> Date::sDays = { "ƒень", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17",
									"18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31" };
const vector<string> Date::sMonthes = { "ћес€ц", "январь", "‘евраль",  "ћарт",  "јпрель",  "ћай",  "»юнь",  "»юль",  "јвгуст",
										"—ент€брь",  "ќкт€брь",  "Ќо€брь",  "ƒекабрь", };
const vector<string> Date::sYears = { "√од",  
									"1961", "1962", "1963", "1964", "1965", "1966", "1967",
									"1969", "1970", "1971", "1972", "1973", "1974", "1975", "1976", "1977", "1978", "1979",
									"1981", "1982", "1983", "1984", "1985", "1986", "1987",
									"1988", "1989", "1990", "1991", "1992", "1993", "1994", "1995", "1996", "1997", "1998", "1999",
									"2000", "2001", "2002", "2003", "2004", "2005", "2006", "2007",
									"2008", "2009", "2010", "2011", "2012", "2013", "2014", "2015", "2016", "2017", "2018", "2019", "2020" };

Date::Date(string date)
{
	if (date.size() < 10) {
		mDay = mMonth = mYear = 0;
		return;
	}
	mDay = (int)(date[0] - '0') * 10 + (int)(date[1] - '0');
	mMonth = (int)(date[3] - '0') * 10 + (int)(date[4] - '0');
	mYear = (int)(date[6] - '0') * 1000 + (int)(date[7] - '0') * 100
		+ (int)(date[8] - '0') * 10 + (int)(date[9] - '0');
	if (mDay < 0 || mDay > 31)
		mDay = 0;
	if (mMonth < 0 || mMonth > 12)
		mMonth = 0;
	if (mYear < 0 || mYear > 2020)
		mYear = 0;
}

bool operator> (const Date& date1, const Date& date2)
{
	return cmp(date1, date2) > 0;
}


bool operator>= (const Date& date1, const Date& date2)
{
	return cmp(date1, date2) >= 0;
}


bool operator< (const Date& date1, const Date& date2)
{
	return cmp(date1, date2) < 0;
}


bool operator<= (const Date& date1, const Date& date2)
{
	return cmp(date1, date2) <= 0;
}


bool operator== (const Date& date1, const Date& date2)
{
	return cmp(date1, date2) == 0;
}


Date& Date::operator= (const Date& date)
{
	mYear = date.mYear;
	mMonth = date.mMonth;
	mDay = date.mDay;

	return (*this);
}


Date Date::operator+ (const Date& date)
{
	Date result(mYear + date.mYear, mMonth + date.mMonth, mDay + date.mDay);
	result.mMonth += (result.mDay - 1) / daysInMonth(mMonth, mYear);
	result.mYear += (result.mMonth - 1) / 12;
	result.mMonth = (result.mMonth - 1) % 12 + 1;
	result.mDay = (result.mDay - 1) % daysInMonth(mMonth, mYear) + 1;
	return result;
}


Date Date::operator+ (const int& n)
{
	return ((*this) + Date(0, 0, n));
}


Date Date::operator- (const Date& date)
{
	Date result(mYear - date.mYear, mMonth - date.mMonth, mDay - date.mDay);
	if (result.mDay < 0) {
		result.mMonth--;
		result.mDay += daysInMonth(mMonth, mYear);
	}
	if (result.mMonth < 0) {
		result.mYear--;
		result.mMonth += 12;
	}
	result.mDay = (result.mDay - 1) % daysInMonth(mMonth, mYear) + 1;
	return result;
}


Date Date::operator- (const int& n)
{
	return ((*this) - Date(0, 0, n));
}


Date& Date::operator+= (const Date& date)
{
	mYear += date.mYear;
	mMonth += date.mMonth;
	mDay += date.mDay;

	return (*this);
}


Date& Date::operator-= (const Date& date)
{
	mYear -= date.mYear;
	mMonth -= date.mMonth;
	mDay -= date.mDay;

	return (*this);
}


int cmp(const Date& date1, const Date& date2)
{
	int result = date1.mYear - date2.mYear;
	if (result == 0)
		result = date1.mMonth - date2.mMonth;
	if (result == 0)
		result = date1.mDay - date2.mDay;
	return result;
}
 
bool Date::isLeap(int year)
{
	return ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0));
	//год, номер которого кратен 400, Ч високосный;
	//остальные годы, номер которых кратен 100, Ч невисокосные;
	//остальные годы, номер которых кратен 4, Ч високосные.
}


int Date::dayOfWeek()
{
	int monthIndex[] = { 1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6 };
	int yearCode = ((6 - (mYear % 4) * 2) + mYear % 100 + (mYear % 100) / 4) % 7;
	int code = (mDay + monthIndex[mMonth - 1] + yearCode - 3) % 7;
	if (isLeap(mYear) && mMonth <= 2)
		code = (code + 1) % 7;
	return code;
}


bool Date::isDayOff()
{
	if (dayOfWeek() == 6)
		return true;
	if ((mDay == 7 && mMonth == 11) ||
		(mDay == 1 && mMonth == 1) ||
		(mDay == 25 && mMonth == 12))
		return true;
	return false;
}

int Date::daysInMonth(int month, int year)
{
	if (month == 4 || month == 6 || month == 9 || month == 11)
		return 30;
	if (month == 2)
		return isLeap(year) ? 29 : 28;
	return 31;
}

Date::operator string() const
{
	string result = string(mDay < 10 ? "0" : "") + to_string(mDay) + string(".") +
		string(mMonth < 10 ? "0" : "") + to_string(mMonth) + string(".") + to_string(mYear);
	return result;
}


Date Date::currentDate()
{
	time_t now = time(0);
	tm gmtm;
	gmtime_s(&gmtm, &now);
	return Date(1900 + gmtm.tm_year, gmtm.tm_mon, gmtm.tm_mday);
}


bool Date::isCorrect()
{
	if (mDay == 0 || mMonth == 0 || mYear == 0)
		return false;
	if ((mMonth == 4 || mMonth == 6 || mMonth == 9 || mMonth == 11) && mDay == 31)
		return false;
	if (mMonth == 2 && ((isLeap(mYear) && mDay > 29) || mDay > 28))
		return false;

	return true;
}


istream& operator >> (istream& ifs, Date& date)
{
	string temp;
	ifs >> temp;
	date = Date(temp);
	return ifs;
}

ostream & operator<<(ostream & out, const Date & date)
{
	out << (string)date;
	return out;
}
