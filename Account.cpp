#include<iostream>
#include"Account.h"
#include<iomanip>
#include<math.h>
using namespace std;

double Account::bank_total=0;

vector<Transaction> Account::transaction_record;

double Account::getTotal(){return bank_total;}

void Account::query(Date date1,Date date2){
	cout<<endl<<"The transaction record is below."<<endl;
	for(int i = 0;i < Account::transaction_record.size();i++){
		if(date1 <= Account::transaction_record[i].get_date() && Account::transaction_record[i].get_date() <= date2){
			Account::transaction_record[i].show();
			cout<<endl;
		}		
	}	
}

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
	cout<<"account creation date is: "<<this->creation_date<<endl;
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
	
	//transaction record PART
	Transaction transaction(deposit_date,this->id,deposit_money,detail);
	Account::transaction_record.push_back(transaction);
}

void savingsAccount::interestCounter(Date new_date,double money){
	int date_length = new_date - this->record_date;
	this->record_date = new_date;
	this->balance_per_day_this_year += (date_length * this->balance) / 365;
	this->balance += money;
}

bool savingsAccount::withdraw(Date withdraw_date,double withdraw_money,string detail){		//withdraw_money is positive num WHEN cin.
	if(this->balance >= withdraw_money){													//but turn to neg WHEN using in member functions.
		this->interestCounter(withdraw_date,-withdraw_money);
		Account::bank_total -= withdraw_money;	

		//transaction record PART
		Transaction transaction(withdraw_date,this->id,-withdraw_money,detail);
		Account::transaction_record.push_back(transaction);
		//

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
	cout<<"account member year fee is: "<<this->fee<<endl;
}

bool creditAccount::settle(Date settle_date){
	bool money_flag = false;	//如果发现有费用相关变化，标志变为真

	if(this->lastMonth != settle_date.getMonth()){
		if(this->principal < 0){	//信用卡账户按月结息
			money_flag = true;
			this->interestCounter(settle_date,0);
			double interest = this->principal_accumulate_this_month * this->rate;	//patm set to be neg.so interest is neg.
			this->principal += interest;	//balance & interest both are neg.
			this->principal_accumulate_this_month = 0;
			//about available_credit part
			if(-this->principal >= this->credit) this->available_credit = 0;	//principal在负数域时小于信用额度
			else this->available_credit = this->credit + this->principal;
		
			Account::bank_total += interest;
		}
		else{	//不欠钱时，账户金额信息不用变，更新时间信息就行
			this->record_date = settle_date;
		}	
	}
	
	if(this->lastYear != settle_date.getYear()){
		money_flag = true;
		double fee_owed = this->fee * (settle_date.getYear() - this->lastYear);
		this->principal -= fee_owed;
		Account::bank_total -= fee_owed;
		this->lastYear = settle_date.getYear();
		//about available_credit part
		if(-this->principal >= this->credit) this->available_credit = 0;	//principal在负数域时小于信用额度
		else this->available_credit = this->credit + this->principal;
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
		this->available_credit -= withdraw_money;
		Account::bank_total -= withdraw_money;
		
		//transaction record PART
		Account::transaction_record.push_back(Transaction(withdraw_date,this->id,-withdraw_money,detail));
		//
	}
	return money_flag;
}

void creditAccount::deposit(Date deposit_date,double deposit_money,string detail){
	if(this->principal >= 0) this->principal += deposit_money;
	else{
		this->interestCounter(deposit_date,deposit_money);
		//credit PART 
		if(this->principal = 0)this->available_credit = this->credit;
		else if(-this->principal > this->credit ) this->available_credit = 0;
		else if(-this->principal <= this->credit) this->available_credit = this->credit + this->principal;
	}

	Account::bank_total += deposit_money;	
	
	//transaction record PART
	Account::transaction_record.push_back(Transaction(deposit_date,this->id,deposit_money,detail));
	//	
}
