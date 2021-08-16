#ifndef __DATE__H__
#define __DATE__H__ 
#include<string>
#include<iostream>
using namespace std;
class Date{
private:
    int year;
    int month;
    int day;
	const int day_of_month_in_non_leapYear[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	const int day_of_month_in_leapYear[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
	void DateAdder();
public:
    Date();
    Date(int year,int month,int day);
	Date(string,string,string); 
    void show();
    void operator=(Date date);
    int operator-(Date date);
    bool operator!=(Date date);
    bool operator==(Date date);
	bool isLeapYear();
	bool operator>(Date date);
	bool operator<(Date date);
	bool operator<=(Date date);
	int getMonth();
	int getDay();
	int getMaxDay();
	int getYear();
	static Date read();
	friend ostream& operator<<(ostream& out,const Date& ref){
		out<<ref.year<<'-'<<ref.month<<'-'<<ref.day<<'.';
		return out;
	}
};
#endif
