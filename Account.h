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
	double balance_per_day_this_period;
	double balance;	//balance can't be neg.
	bool isBalanceEnough(double withdraw_money);
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
	double debt;		//总欠款，=本金+利息
	double daily_debt_recently;		//loan_interest = daily_debt_recently * rate
	double loan_interest;	//贷款利息
	double principal;	//本金
	double balance;		//negative number means have loan //利息和年费也算在余额里
	double available_credit;	//available_credit=credit+balance;>=0 required;
	bool fee_checker(Date);		//will influence -- balance
	int year_fee_checked;	//the latest year which fee has been checked and added to balance in [settle] function
public:
	creditAccount(Date creation_date,string id,double credit,double rate,double fee);
	creditAccount(){}
	void show();
	bool settle(Date settle_date);
	bool withdraw(Date withdraw_date,double withdraw_money,string detail);
	bool isOverLoan(double withdraw_money);
	bool isOverCredit(double deposit_money);
	void deposit(Date deposit_date,double deposit_money,string detail);
};


