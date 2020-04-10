/*Разработка автоматизированной системы расчета заработной платы сотрудников ВУЗа*/

#pragma once

#include <ctime>
#include <string>
#include <vector>

using namespace std;

class Date 
{
public:
	Date(int year = 0, int month = 0, int day = 0) :
		mYear(year), mMonth(month), mDay(day) {}
	Date(const Date& date) : mYear(date.mYear), mMonth(date.mMonth), mDay(date.mDay) {}
	Date(string date);

	void	setYear(int year) { mYear = year; }
	int		getYear() const { return mYear; }
	void	setMonth(int month) { mMonth = month; }
	int		getMonth() const { return mMonth; }
	void	setDay(int day) { mDay = day; }
	int		getDay() const { return mDay; }

	Date	operator+ (const Date& date);
	Date	operator+ (const int& n);
	Date	operator- (const Date& date);
	Date	operator- (const int& n);
	Date&	operator+= (const Date& date);
	Date&	operator-= (const Date& date);
	Date&	operator= (const Date& date);

	friend bool operator> (const Date& date1, const Date& date2);
	friend bool operator>= (const Date& date1, const Date& date2);
	friend bool operator< (const Date& date1, const Date& date2);
	friend bool operator<= (const Date& date1, const Date& date2);
	friend bool operator== (const Date& date1, const Date& date2);

	friend istream& operator >> (istream& ifs, Date& date);
	friend ostream& operator << (ostream& out, const Date& date);

	operator string() const;

	friend int cmp(const Date& date1, const Date& date2);

	int		dayOfWeek();
	bool	isDayOff();
	bool	isCorrect();

	static int	daysInMonth(int month, int year = currentDate().getYear());
	static bool isLeap(int year);
	static Date currentDate();

	static const vector<string> sDays;
	static const vector<string> sMonthes;
	static const vector<string> sYears;

private:

	friend class Period;

	int mYear;
	int mMonth;
	int mDay;
};