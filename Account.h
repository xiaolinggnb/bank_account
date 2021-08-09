#include<string>
#include"Date.h"
using namespace std;
class Account{
protected:
	static double bank_total;
public:	
	static double getTotal();
	virtual	void deposit(Date,double,string) = 0;
	virtual void show() = 0;
	virtual bool settle(Date) = 0;
	virtual bool withdraw(Date,double,string) = 0;
};
class savingsAccount:public Account{
private:
	Date creation_date;
	Date record_date;
	string id;
	double rate;
	double balance_per_day_this_year;
	double balance;	//balance can't be neg.
	int lastYear;	//年初结算时(settle)用
	void interestCounter(Date,double);	//infulence: record_date,balance,balance_per_day_this_year
public:
	savingsAccount(){}
	savingsAccount(Date creation_date,string id,double rate);
	void show();
	bool settle(Date settle_date);
	void deposit(Date deposit_date,double deposit_money,string detail);
	bool withdraw(Date withdraw_date,double withdraw_money,string detail);
};

class creditAccount:public Account{
private:
	Date creation_date;
	Date record_date;
	string id;
	double credit;
	double rate;
	double fee;
	double principal;	//本金
	double balance;		//negative number means have loan //利息和年费也算在余额里
	double available_credit;	//available_credit=credit+balance;>=0 required;
	bool fee_checker(Date);		//will influence -- balance
	int lastYear;	//relation to fee.
	int lastMonth;
	void interestCounter(Date,double);
	double principal_accumulate_this_month;
public:
	creditAccount(Date creation_date,string id,double credit,double rate,double fee);
	creditAccount(){}
	void show();
	bool settle(Date settle_date);
	bool withdraw(Date withdraw_date,double withdraw_money,string detail);
	void deposit(Date deposit_date,double deposit_money,string detail);
	
};
