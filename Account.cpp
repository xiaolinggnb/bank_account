#include<iostream>
#include"Account.h"
#include<math.h>
using namespace std;

double Account::bank_total=0;

double Account::getTotal(){return bank_total;}

savingsAccount::savingsAccount(Date creation_date,string id,double rate){
	this->creation_date=creation_date;
	this->record_date=creation_date;
	this->id=id;
	this->rate=rate;
	this->balance_per_day_this_period=0;
	this->balance = 0;
}

void savingsAccount::show(){
	cout<<"account creation date is: ";this->creation_date.show();cout<<endl;
	cout<<"account id is: "<<this->id<<endl;
	cout<<"account profit rate is: "<<this->rate<<endl;
	cout<<"account's balance is:"<<this->balance<<endl;
}

bool savingsAccount::settle(Date settle_date){
	int date_length = settle_date - this->record_date;
	this->record_date = settle_date;
	this->balance_per_day_this_period = (date_length * this->balance) / 365;
	//settle account	
	double profit = this->balance_per_day_this_period * this->rate;
	profit = floor(profit * 100 + 0.5) / 100;	//函数功能:四舍五入&精确到小数点后两位
	this->balance += profit;
	Account::bank_total += profit;
	return true;
}
void savingsAccount::deposit(Date deposit_date,double deposit_money,string detail){
	this->balance += deposit_money;
	Account::bank_total += deposit_money;
}

bool savingsAccount::withdraw(Date withdraw_date,double withdraw_money,string detail){
	if(this->isBalanceEnough(withdraw_money)){
		this->balance -= withdraw_money;	
		Account::bank_total -= withdraw_money;	
		return true; 
	}
	else{
		cout<<"Account balance not enough. Operation cancelled."<<endl;
		return false;
	}

}
bool savingsAccount::isBalanceEnough(double withdraw_money){
	if(this->balance >= withdraw_money) return true;
	else{
		cout<<endl;
		cout<<"Your account don't have so much money.";
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
	this->loan_interest = 0;
	this->daily_debt_recently = 0;
	this->debt = 0;
	this->principal = 0;
	this->balance = 0;
	this->available_credit = credit;
	this->year_fee_checked = creation_date.getYear();
}

void creditAccount::show(){
	cout<<"account creation date is: ";this->creation_date.show();cout<<endl;
	cout<<"account id is: "<<this->id<<endl;
	cout<<"account credit is: "<<this->credit<<endl;
	cout<<"account available credit is: "<<this->available_credit<<endl;
	cout<<"account principal is: "<<this->principal<<endl;
	cout<<"account balance is: "<<this->balance<<endl;
	cout<<"account member year fee is: "<<this->fee<<endl;
}
bool creditAccount::fee_checker(Date check_date){
	if(this->year_fee_checked != check_date.getYear() && check_date.getMonth() == 1 && check_date.getDay() == 1){
		this->year_fee_checked = check_date.getYear();
		this->balance -= this->fee;
		return true;
	}
	else return false;
}
bool creditAccount::settle(Date settle_date){
	int date_length = settle_date - this->record_date;
	this->record_date = settle_date;
	double interest = this->principal * date_length * this->rate;
	this->balance -= interest;
	//fee checker
	this->fee_checker(settle_date);
	//about available_credit part
	if(this->balance < 0){
		if(abs(this->balance) >= this->credit){
			this->available_credit = 0;
		}
		else{
			this->available_credit = this->credit + this->balance;
		}	
	}
	
	Account::bank_total -= interest;
	return true;
}

bool creditAccount::withdraw(Date withdraw_date,double withdraw_money,string detail){
	if(this->balance > 0){	//余额有富余时，取钱先取余额里的钱，再使用额度借钱
		if(withdraw_money <= this->balance){
			this->balance -= withdraw_money;
			Account::bank_total -= withdraw_money;
			return true;
		}
		else{
			if(this->balance + this->credit < withdraw_money){
				cout<<"借款超额（余额和信用额度总额不足），操作失败。"<<endl;
				return false;
			}
			else{
				this->balance -= withdraw_money;
				this->principal = abs(this->balance);
				Account::bank_total -= withdraw_money;
				return true;
			}
		}
	}
	else{	//余额无富余时的情况
		if(this->isOverLoan(withdraw_money)){ //cancel operation this time
			cout<<"借款超出上限，操作失败"<<endl;
			cout<<"剩余借款额度为: "<<this->available_credit<<endl;
			return false;
		}
		else{	//credit test pass,permit operation this time
			//balance && available_credit part
			this->balance -= withdraw_money;
			this->principal += withdraw_money; //ONLY relate to withdraw & deposit
			
			this->available_credit -= withdraw_money;
			
			Account::bank_total -= withdraw_money;
		
			return true;	
		}
	}
}
bool creditAccount::isOverLoan(double withdraw_money){		//credit test when withdraw money
	if(withdraw_money > this->available_credit) return true;
	else return false;
}
void creditAccount::deposit(Date deposit_date,double deposit_money,string detail){
	if(this->balance >= 0){
		this->balance += deposit_money;
	}
	else{
		double repay_first = abs(this->balance) - this->principal;		
		if(deposit_money <= repay_first){	//还的钱少于多出来的利息和年费时
			this->balance += deposit_money;
			if(abs(this->balance) < this->credit){	//如果欠款数回到了额度内
				this->available_credit = this->credit + this->balance;
			}
			else{}
		}
		else{	//还的钱足够把多出来的利息和年费还完时
			this->balance += deposit_money;
			if(this->balance < 0){
				this->principal = abs(this->balance);
				this->available_credit = this->credit + this->balance;
			}
			else{	//把欠款也都还完时
				this->principal = 0;
				this->available_credit = this->credit;
			}
		}
	}
		
	bank_total+=deposit_money;
/*	
	if(this->isOverCredit(deposit_money)){
		this->available_credit = this->credit;
		cout<<"You've got maximum credit."<<endl;
	}
	else{
		this->available_credit += deposit_money;
	}
*/
}


bool creditAccount::isOverCredit(double deposit_money){
	if(this->available_credit+deposit_money>this->credit)return true;
	else return false;
}
