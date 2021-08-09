#include<iostream>
#include"Account.h"
using namespace std;
int main(){
	Date date(2008,11,1);

	//several accounts creation
	savingsAccount s1(date,"xiaobaitu",0.015);
	savingsAccount s2(date,"xiaoxingxing",0.015);
	creditAccount c1(date,"xiaobaitututu",10000,0.0005,50);
	
	Account *accounts[]={&s1,&s2,&c1};
	const int n = sizeof(accounts)/sizeof(Account*);	//total accounts number
	
	cout<<"(d)deposit (w)withdraw (s)show (c)change day (n)next month (e)exit"<<endl;
	
	char cmd;
	do{
	//show--date & total money
		cout<<'\n';
		date.show();
		cout<<"\tTotal: "<<Account::getTotal()<<"\tcommand>";
		int index,day;
		double amount;
		string desc;
		cin>>cmd;
		cout<<'\n';
		switch(cmd){
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
				else
					date=Date(date.getYear(),date.getMonth(),day);
				break;
			case'n'://go to next month
				if(date.getMonth()==12)
					date=Date(date.getYear()+1,1,1);
				else
					date=Date(date.getYear(),date.getMonth()+1,1);
				
				for(int i=0;i<n;i++)
					accounts[i]->settle(date);
				break;
		}
	}while(cmd!='e');
}
