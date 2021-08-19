#include<iostream>
#include"Account.h"
#include<vector>
#include<algorithm>
#include<fstream>
#include<stdexcept>
using namespace std;

struct deleter{
	template<class T>void operator()(T*p){delete p;}
};

string string_cut(string &str,int &i_start,int &i_end){
	while(str[i_end] != ' ' && str[i_end] != '\0') i_end++;
	string dest_str = str.substr(i_start,i_end - i_start);
	if(i_end + 1< str.length()){
		i_end++;i_start = i_end;	//让i_start和i_end指向下一个目标
	}
	return dest_str;
}

int main(){
	Date date(2008,11,1);

	vector<Account*>accounts;	//创建账户数组，元素个数为0
	fstream cmdfile;
	cmdfile.open("cmdfile.txt",ios::app | ios::in | ios::out);
	//preset 预处理程序
	string action_line;
	while(getline(cmdfile,action_line)){		
		//输入变量
		char type;
		int index,day;
		double amount,credit,rate,fee;
		string id,desc;
		Account* account;
		Date date1,date2;
		int i_start = 0,i_end = 0;
		//
        switch(action_line[0]){
            case'a'://add account
                type = action_line[2];
				i_start = i_end = 4;
				id = string_cut(action_line,i_start,i_end);
                if(type == 's'){
                    rate = stod(string_cut(action_line,i_start,i_end));
                    account = new savingsAccount(date,id,rate);
                    accounts.push_back(account);
                }
                else if(type == 'c'){
                    credit = stod(string_cut(action_line,i_start,i_end)); 
					rate = stod(string_cut(action_line,i_start,i_end)); 
					fee = stod(string_cut(action_line,i_start,i_end)); 
                    account = new creditAccount(date,id,credit,rate,fee);
                    accounts.push_back(account);
                }
                break;
            case'd'://deposit cash
				i_start = i_end = 2;
                index = stoi(string_cut(action_line,i_start,i_end));
				amount = stod(string_cut(action_line,i_start,i_end)); 
                desc = action_line.substr(i_start,action_line.length() - i_start); 
                accounts[index]->deposit(date,amount,desc);
                break;
            case'w'://withdraw cash
				i_start = i_end = 2;
                index = stoi(string_cut(action_line,i_start,i_end));
				amount = stod(string_cut(action_line,i_start,i_end)); 
                desc = action_line.substr(i_start,action_line.length() - i_start); 
				accounts[index]->withdraw(date,amount,desc);	//放心，不一定会成功，withdraw的返回值是个bool值，不过这里不用显示，直接执行完就行
                break;
           	  case'c'://change date
 			  	  i_start = i_end = 2;
                  day = stoi(string_cut(action_line,i_start,i_end)); ;
                  if(day > date.getDay() && day < date.getMaxDay())
                      date=Date(date.getYear(),date.getMonth(),day);
                  break;
            case'n'://go to next month
                if(date.getMonth() == 12)
                    date = Date(date.getYear()+1,1,1);
                else
                    date = Date(date.getYear(),date.getMonth()+1,1);

                for(vector<Account*>::iterator iter = accounts.begin();iter != accounts.end();++iter)
                    (*iter)->settle(date);
                break;
			default:break;
        }
	}
	//重新打开文件
	cmdfile.close();
	cmdfile.open("cmdfile.txt",ios::app|ios::in|ios::out);
	//
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
		//
		//根据命令cmd执行操作
		switch(cmd){
			case'a'://add account
				cout<<"Please cin account type('s' or 'c') & account id.\t";			
				cin>>type>>id;
				try{
					if(type == 's'){
						cout<<"Please cin account interest rate(yearly).\t";
						account = new savingsAccount(date,id,rate);	
						accounts.push_back(account);
						cmdfile<<cmd<<' ' <<type<<' '<<id<<' '<<rate<<endl;
					}
					else if(type == 'c'){
						cout<<"Please cin account credit & interest rate(daily) & fee(yearly).\t";
						cin>>credit>>rate>>fee;
						account = new creditAccount(date,id,credit,rate,fee);	
						accounts.push_back(account);
						cmdfile<<cmd<<' '<<type<<' '<<id<<' '<<credit<<' '<<rate<<' '<<fee<<endl;
					}
					else{
						cout<<"Sorry that we don't have the account TYPE you cin. Operation failed."<<endl;
					}
				}
				catch(...){cout<<"account creation FAIL!"<<endl;}	
				break;
			case'd'://deposit cash
				cout<<"Please cin index of accounts && amount of deposit cash && description: "<<endl;
				cin>>index>>amount;
				getline(cin,desc);
				try{
					accounts[index]->deposit(date,amount,desc);cout<<endl;
					accounts[index]->show();
					cout<<"deal discription: "<<desc<<endl;
					cmdfile<<cmd<<' '<<index<<' '<<amount<<' '<<desc<<endl;
				}
				catch(...){
					cout<<"deposit FAIL."<<endl;
				}
				break;
			case'w'://withdraw cash
				cout<<"Please cin index of accounts && amount of withdraw cash && description: "<<endl;
				cin>>index>>amount;
				getline(cin,desc);
				bool flag;
				flag = accounts[index]->withdraw(date,amount,desc);
				if(flag){
					accounts[index]->show();
					cout<<"deal discription: "<<desc<<endl;
					cmdfile<<cmd<<' '<<index<<' '<<amount<<' '<<desc<<endl;
				}
				break;
			case's'://serach all accounts info.
				for(int i = 0;i < accounts.size();i++){
					cout<<"["<<i<<"]";
					accounts[i]->show();
					cout<<endl;
				}
				break;
			case'c'://change date
				cout<<"You want to change to which day?\t";
         		while(!(cin>>day)){
             		cin.clear();         //重置cin
             		while(cin.get()!='\n')         //删除错误输入
                 		continue;
             		cout<<"Please re-enter a legal number: ";  //提示用户再输入
         		}		
				if(day<date.getDay())
					cout<<"You cannot specify a previous day";
				else if(day>date.getMaxDay())
					cout<<"The day you cin is an invalid day";
				else{
					date = Date(date.getYear(),date.getMonth(),day);
					cmdfile<<cmd<<' '<<day<<endl;
				}
				break;
			case'n'://go to next month
				if(date.getMonth()==12)
					date=Date(date.getYear()+1,1,1);
				else
					date=Date(date.getYear(),date.getMonth()+1,1);
			
				for(vector<Account*>::iterator iter = accounts.begin();iter != accounts.end();++iter)
					(*iter)->settle(date);	
				cmdfile<<cmd<<endl;
				break;
			case'q'://查询一段时间内的账目
				try{
					date1 = Date::read();
					date2 = Date::read();
				}catch(...){
					cout<<"appear an EXCEPTION in Date recognition."<<endl;
					break;
				}
				Account::query(date1,date2);
				break;
		}
	}while(cmd!='e');
	
	for_each(accounts.begin(),accounts.end(),deleter());
	cmdfile.close();
}
