#ifndef __TRANSACATION__H__
#define __TRANSACATION__H__
#include"Date.h"
#include<string>
class Transaction{
private:
	Date date;
	string id;
	double money;
	string desc;
public:
	Transaction();
	Transaction(Date,string id,double,string desc);
	void show();
	Date get_date() const;
};
#endif
