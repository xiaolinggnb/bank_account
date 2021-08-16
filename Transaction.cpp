#include"Transaction.h"
#include<iostream>
using namespace std;
Transaction::Transaction(){}

Transaction::Transaction(Date date,string id,double money,string desc){
	this->date = date;
	this->id = id;
	this->money = money;
	this->desc = desc;
}

void Transaction::show(){
	cout<<this->date<<'\t'<<this->id<<'\t'<<this->money<<'\t'<<this->desc;
}

Date Transaction::get_date() const{
	return this->date;
}
