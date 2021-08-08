#ifndef __DATE__H__
#define __DATE__H__ 
class Date{
private:
    int year;
    int month;
    int day;
	const int day_of_month_in_non_leapYear[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	const int day_of_month_in_leapYear[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
public:
    Date();
    Date(int year,int month,int day);
    void show();
    void operator=(Date date);
    int operator-(Date date);
    bool operator!=(Date date);
    bool operator==(Date date);
	void DateAdder();
	bool isLeapYear();
	bool operator>(Date date);
	bool operator<(Date date);
	bool operator<=(Date date);
	int getMonth();
	int getDay();
	int getMaxDay();
	int getYear();
};
#endif
