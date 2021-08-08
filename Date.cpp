#include"Date.h"
#include<iostream>
using namespace std;

Date::Date(){}
Date::Date(int year,int month,int day){
    this->year=year;
    this->month=month;
    this->day=day;
}
void Date::show(){
    cout<<this->year<<'-'<<this->month<<'-'<<this->day<<'.';
}
void Date::operator=(Date date){
    this->year=date.year;
    this->month=date.month;
    this->day=date.day;
}
int Date::operator-(Date date){
	if(*this==date)return 0;
	if(*this<date)return -1;	
	Date tmp=date;
	int interval=0;
	while(*this>tmp){
		tmp.DateAdder();
		interval++;
		if(interval>32766)break;	//int type's max limit is 32767,consider data type,out of range should avoid
	}
	return interval;
}

bool Date::operator!=(Date date){
    if(this->year!=date.year||this->month!=date.month||this->day!=date.day)return true;
    else return false;
}
bool Date::operator==(Date date){
    if(this->year==date.year&&this->month==date.month&&this->day==date.day)return true;
    else return false;
}
bool Date::isLeapYear(){
	if((this->year % 4 == 0 && this->year % 100 != 0)||this->year % 400 == 0) return true;
	else return false;
}
void Date::DateAdder(){
	this->day++;
	if(!this->isLeapYear()){
        if(this->day > this->day_of_month_in_non_leapYear[this->month]){
            this->day=1;
            this->month++;
            if(this->month>12){
				this->month=1;
                this->year++;
            }
        }
    }
	else{	
		if(this->day > this->day_of_month_in_leapYear[this->month]){
			this->day=1;
			this->month++;
			if(this->month>12){
				this->month=1;
				this->year++;
			}
		}
	}	
}
bool Date::operator<(Date date){
	int flag=0;		//default set to 0, means false condition [>=].
	if(this->year<=date.year){
		if(this->month<date.month){
			flag=1;
		}
		else if(this->month==date.month){
			if(this->day<date.day){
				flag=1;
			}
		}
	}
	if(flag) return true;
	else return false;
}
bool Date::operator<=(Date date){
	if(*this<date||*this==date)return true;
	else return false;
}


bool Date::operator>(Date date){
	if(*this<=date)return false;
	else return true;
}

int Date::getMonth(){
	return this->month;
}

int Date::getDay(){
	return this->day;
}

int Date::getMaxDay(){
	if(this->isLeapYear())
		return this->day_of_month_in_leapYear[this->getMonth()];	
	else
		return this->day_of_month_in_non_leapYear[this->getMonth()];
}

int Date::getYear(){
	return this->year;
}
