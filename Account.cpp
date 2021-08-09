#include<iostream>
#include"Account.h"
#include<iomanip>
#include<math.h>
using namespace std;

double Account::bank_total=0;

double Account::getTotal(){return bank_total;}

savingsAccount::savingsAccount(Date creation_date,string id,double rate){
	this->creation_date=creation_date;
	this->record_date=creation_date;
	this->id=id;
	this->rate=rate;
	this->balance = 0;
	this->lastYear = creation_date.getYear();
	this->balance_per_day_this_year=0;
}

void savingsAccount::show(){
	cout<<"account creation date is: ";this->creation_date.show();cout<<endl;
	cout<<"account id is: "<<this->id<<endl;
	cout<<"account profit rate is: "<<this->rate<<endl;
	cout<<"account's balance is:"<<this->balance<<endl;
}

bool savingsAccount::settle(Date settle_date){
	if(this->lastYear != settle_date.getYear()){
		this->interestCounter(settle_date,0);	
		//settle account	
		double profit = this->balance_per_day_this_year * this->rate;
		profit = floor(profit * 100 + 0.500001) / 100;	//函数功能:四舍五入&精确到小数点后两位
		this->balance += profit;
		Account::bank_total += profit;
		this->lastYear = settle_date.getYear();
		this->balance_per_day_this_year = 0;
		return true;
	}
	else return false;
}
void savingsAccount::deposit(Date deposit_date,double deposit_money,string detail){
	this->interestCounter(deposit_date,deposit_money);	
	Account::bank_total += deposit_money;
}

void savingsAccount::interestCounter(Date new_date,double money){
	int date_length = new_date - this->record_date;
	this->record_date = new_date;
	this->balance_per_day_this_year += (date_length * this->balance) / 365;
	this->balance += money;
}

bool savingsAccount::withdraw(Date withdraw_date,double withdraw_money,string detail){
	if(this->balance >= withdraw_money){
		this->interestCounter(withdraw_date,-withdraw_money);
		Account::bank_total -= withdraw_money;	
		return true; 
	}
	else{
		cout<<"Account balance not enough. Operation cancelled."<<endl;
		return false;
	}
}

creditAccount::creditAccount(Date creation_date,string id,double credit,double rate,double fee){
	this->creation_date = creation_date;
	this->record_date = creation_date;
	this->id = id;
	this->credit = credit;
	this->rate = rate;
	this->fee = fee;
	this->principal = 0;
	this->balance = 0;
	this->available_credit = credit;
	this->lastYear = creation_date.getYear();
	this->principal_accumulate_this_month = 0;
}

void creditAccount::show(){
	cout<<"account creation date is: ";this->creation_date.show();cout<<endl;
	cout<<"account id is: "<<this->id<<endl;
	cout<<"account credit is: "<<this->credit<<endl;
	cout<<"account rate is: "<<this->rate<<endl;
	cout<<"account available credit is: "<<this->available_credit<<endl;
	cout<<"account principal is: "<<this->principal<<endl;
	cout<<"account balance is: "<<this->balance<<endl;
	cout<<"account member year fee is: "<<this->fee<<endl;
}

bool creditAccount::settle(Date settle_date){
	bool money_flag = false;	//如果发现有费用相关变化，标志变为真

	if(this->lastYear != settle_date.getYear()){
		money_flag = true;
		double fee_owed = this->fee * (settle_date.getYear() - this->lastYear);
		this->balance -= fee_owed;
		Account::bank_total -= fee_owed;
		this->lastYear = settle_date.getYear();
	}

	if(this->lastMonth != settle_date.getMonth() && this->balance < 0){	//信用卡账户按月结息
		money_flag = true;
		this->interestCounter(settle_date,0);
		double interest = this->principal_accumulate_this_month * this->rate;	//patm set to be neg.so interest is neg.
		this->balance += interest;	//balance & interest both are neg.

		//about available_credit part
		if(-this->balance >= this->credit) this->available_credit = 0;	//balance在负数域时小于信用额度
		else this->available_credit = this->credit + this->balance;
		
		this->principal = this->balance;	
		Account::bank_total += interest;
	}
	
	return money_flag;
}
void creditAccount::interestCounter(Date new_date,double money){	//取款，money为负，存款，money为正
	int date_length = new_date - this->record_date;
	this->principal_accumulate_this_month += this->principal * date_length;		//principal直接按负数来算
	this->record_date = new_date;
	this->principal += money;
	if(this->principal > 0) this->principal = 0;	//principal属于(负数,0]范围
}

bool creditAccount::withdraw(Date withdraw_date,double withdraw_money,string detail){
	bool money_flag = false;	//to see if with money change
	if(this->available_credit >= withdraw_money){
		money_flag = true;
		this->interestCounter(withdraw_date,-withdraw_money);
		this->balance -= withdraw_money;
		Account::bank_total -= withdraw_money;
	}
	return money_flag;
}

void creditAccount::deposit(Date deposit_date,double deposit_money,string detail){
	this->balance += deposit_money;
	if(this->balance < 0) this->interestCounter(deposit_date,deposit_money);
	Account::bank_total += deposit_money;	
}
