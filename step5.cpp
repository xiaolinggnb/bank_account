#include<iostream>
#include"Account.h"
#include<vector>
#include<algorithm>
using namespace std;

struct deleter{
	template<class T>void operator()(T*p){delete p;}
};

int main(){
	Date date(2008,11,1);

	vector<Account*>accounts;	//创建账户数组，元素个数为0
		
	cout<<"(a)add account (d)deposit (w)withdraw (s)show (c)change day (n)next month (q)query (e)exit"<<endl;
	
	char cmd;
	do{
		//show--date & total money
		cout<<'\n';
		date.show();
		cout<<"\tTotal: "<<Account::getTotal()<<"\tcommand>";
		//
		//输入变量
		char type;
		int index,day;
		double amount,credit,rate,fee;
		string id,desc;
		Account* account;
		Date date1,date2;
		//
		//输入命令，回车读取
		cin>>cmd;
		cout<<'\n';
		//根据命令cmd执行操作
		switch(cmd){
			case'a'://add account
				cout<<"Please cin account type('s' or 'c') & account id.\t";			
				cin>>type>>id;
				if(type == 's'){
					cout<<"Please cin account interest rate(yearly).\t"
					cin>>rate;
					account = new savingsAccount(date,id,rate);	
					accounts.push_back(account);
				}
				else if(type == 'c'){
					cout<<"Please cin account credit & interest rate(daily) & fee(yearly).\t";
					cin>>credit>>rate>>fee;
					account = new creditAccount(date,id,credit,rate,fee);	
					accounts.push_back(account);
				}
				else{
					cout<<"Sorry that we don't have the account TYPE you cin. Operation failed."<<endl;
				}
				break;
			case'd'://deposit cash
				cout<<"Please cin index of accounts && amount of deposit cash: "<<endl;
				cin>>index>>amount;
				getline(cin,desc);
				accounts[index]->deposit(date,amount,desc);cout<<endl;
				accounts[index]->show();
				cout<<"deal discription: "<<desc<<endl;
				break;
			case'w'://withdraw cash
				cout<<"Please cin index of accounts && amount of withdraw cash: "<<endl;
				cin>>index>>amount;
				getline(cin,desc);
				bool flag;
				flag = accounts[index]->withdraw(date,amount,desc);
				if(flag){
					accounts[index]->show();
					cout<<"deal discription: "<<desc<<endl;
				}
				break;
			case's'://serach all accounts info.
				for(int i = 0;i < n;i++){
					cout<<"["<<i<<"]";
					accounts[i]->show();
					cout<<endl;
				}
				break;
			case'c'://change date
				cout<<"You want to change to which day?\t";cin>>day;
				if(day<date.getDay())
					cout<<"You cannot specify a previous day";
				else if(day>date.getMaxDay())
					cout<<"The day you cin is an invalid day";
				else{
					date=Date(date.getYear(),date.getMonth(),day);
					for(int i=0;i<n;i++)
						accounts[i]->settle(date);
				}
				break;
			case'n'://go to next month
				if(date.getMonth()==12)
					date=Date(date.getYear()+1,1,1);
				else
					date=Date(date.getYear(),date.getMonth()+1,1);
			
				for(vector<Account*>::iterator iter = accounts.begin();iter != accounts.end();++iter)
					(*iter)->settle(date);	
				break;
			case'q'://查询一段时间内的账目
				date1 = Date::read();
				date2 = Date::read();
				Account::query(date1,date2);
				break;
		}
	}while(cmd!='e');
	
	for_each(accounts.begin(),accounts.end(),deleter());

}
