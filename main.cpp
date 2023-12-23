#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <cstdio>
using namespace std;

string usr;//global variable to held user name over the current session

class updpass//class to change password of users, called from any user
{
public:
    void update()
    {
        struct data
        {
            string pass,type;
        };
        map<string,data> usrmap;
        data dt;
        string id;
        string line;
        fstream file;
        file.open("users.txt",ios::in);
        while(getline(file,line))
        {
            string uid,upass,utype,newpass1,newpass2;
            stringstream stream(line);
            stream>>uid>>upass>>utype;
            dt.pass = upass;
            dt.type = utype;
            id = uid;
            if(uid == usr)
            {
                string oldpass;
                cout<<"Enter Old Password: ";
                cin>>oldpass;
                if(oldpass == upass)
                {
                    cout<<"Enter New Password: ";
                    cin>>newpass1;
                    cout<<"Re-enter New Password: ";
                    cin>>newpass2;
                    if(newpass1 == newpass2)
                    {
                        dt.pass = newpass1;
                    }
                    else{cout<<"Try Again"<<endl;}
                }
                else
                {
                    cout<<"Wrong password"<<endl;
                    return;
                }
            }
            usrmap.insert(make_pair(id,dt));
        }
        file.close();
        map<string,data>::iterator it = usrmap.begin();
        rename("users.txt", "users1.txt");
        ofstream nfile;
        nfile.open("users.txt");
        // Iterate through the map
        while (it != usrmap.end())
        {
            nfile<<it->first<<" "<<it->second.pass<<" "<<it->second.type<<"\n";
            ++it;
        }
        nfile.close();
        remove("users1.txt");
    }
};
class manager
{
    void newuser()
    {
        cout<<"new user"<<endl;
        string id,pass,type;
        int typenum;
        cout<<"Enter user ID: ";
        cin>>id;
        string line;
        fstream dfile;
        dfile.open("users.txt",ios::in);
        string d,p,m;
        while(getline(dfile,line))
        {
            stringstream stream(line);
            stream>>d>>p>>m;
            if(id == d)
            {
                cout<<"id used"<<endl;
                return;
            }
        }
        cout<<"Enter password: ";
        cin>>pass;
        cout<<" 1 teller - 2 employee";
        cout<<" Enter type (choose number): ";
        cin>>typenum;
        if(typenum == 1)
        {
            type ="teller";
        }
        else if(typenum == 2)
        {
            type = "employee";
        }
        else
        {
            cout<<"type error"<<endl;
            return;
        }
        ofstream file;
        file.open("users.txt",ios::app);
        file<<id<<" "<<pass<<" "<<type<<"\n";
        file.close();
    }


    void updatepass()
    {
        updpass up;
        up.update();
    }
public:
    void start()
    {
        int dex =1;
        while(dex > 0)
        {
            cout<<"1.New User"<<endl;
            cout<< "2.Change Password"<<endl;
            cout<<"3.Logout"<<endl;
            if(cin>>dex)
            {
                if(dex == 1)
                {
                    newuser();
                }
                else if(dex == 2)
                {
                    updatepass();
                }
                else if(dex == 3)
                {
                    system("cls");
                    break;
                }
                else
                {
                    cout<<"Enter a valid choice"<<endl;
                    cin.clear();
                    dex = 1;
                    cin.ignore(40,'\n');
                }
            }
            else
            {
                cout<<"Enter a valid choice"<<endl;
                cin.clear();
                dex = 1;
                cin.ignore(40,'\n');
            }
        }
    }
};

class teller
{
    map<string,string>balance;
    void fill()//fill map with accounts data
    {
        fstream bfile;
        bfile.open("balance.txt",ios::in);
        string line;
        while(getline(bfile,line))
        {
            string accnum;
            string bln;//rasid
            stringstream stream(line);
            stream>>accnum>>bln;
            balance.insert({{accnum,bln}});
        }
        bfile.close();
    }
    void save()// save accounts data to accounts file after withdraw or deposit
    {
        map<string,string>::iterator it = balance.begin();
        rename("balance.txt", "balance1.txt");
        ofstream nfile;
        nfile.open("balance.txt",ios::app);
        // Iterate through the map and save data to acc file
        while (it != balance.end())
        {
            nfile<<it->first<<" "<<it->second<<"\n";
            ++it;
        }
        nfile.close();
        remove("balance1.txt");
    }

    string gettime() //get current date and time
    {
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
        std::string str(buffer);
        return str;
    }
    void withdraw()
    {
        fill();
        cout<<"withdraw"<<endl;
        string accno;
        cout<<"Enter Account Number: ";
        cin>>accno;
        double amount;
        cout<<"Enter withdraw amount: ";
        cin>>amount;
        cout<<"are you sure withdraw "<<amount<<" from account number" <<accno<<endl;
        int ansr;
        cout<<"1.agree \n2.cancel"<<endl;
        cin>>ansr;
        if(ansr == 1)
        {
            string tmp = balance.at(accno);
            double stock = stod(tmp);
            if(stock >= amount)
            {
                double res = stock - amount;
                balance[accno] = to_string(res);
                save();
                ofstream tfile;
                tfile.open("trans.txt",ios::app);
                tfile<<accno<<" "<<amount<<" "<<gettime()<<" "<<"withdraw \n";
                tfile.close();
            }
            else
            {
                cout<<"Stock is not enough";
            }
        }
    }

    void deposit()
    {
        fill();
        cout<<"deposit"<<endl;
        string accno;
        cout<<"Enter Account Number: ";
        cin>>accno;
        double amount;
        cout<<"Enter deposit amount: ";
        cin>>amount;
        cout<<"are you sure deposit "<<to_string(amount)<<" from account number " <<accno<<endl;
        int ansr;
        cout<<"1.agree \n2.cancel"<<endl;
        cin>>ansr;
        if(ansr == 1)
        {
            string tmp = balance.at(accno);
            double stock = stod(tmp);
            double res = stock + amount;
            balance[accno] = to_string(res);
            save();
            ofstream tfile;
            tfile.open("trans.txt",ios::app);
            tfile<<accno<<" "<<amount<<" "<<gettime()<<" "<<"deposit \n";
            tfile.close();
        }
    }

    void statement()
    {
        cout<<"statment"<<endl;
        string accnum;
        cout<<"enter account number: ";
        cin>>accnum;
        fstream sfile;
        sfile.open("trans.txt",ios::in);
        string accno,amount,date,tm,type;
        string line;
        while(getline(sfile,line))
        {
            stringstream stream(line);
            stream>>accno>>amount>>date>>tm>>type;
            if(accno == accnum)
            {
                cout<<date<<" "<<tm<<" "<<amount<< " "<<type<<endl;
            }
        }
        sfile.close();
    }

    void getstock()
    {
        cout<<"stock"<<endl;
        string accnum;
        cout<<"enter account number: ";
        cin>>accnum;
        fstream sfile;
        sfile.open("balance.txt",ios::in);
        string accno,amount;
        string line;
        while(getline(sfile,line))
        {
            stringstream stream(line);
            stream>>accno>>amount;
            if(accno == accnum)
            {
                cout<<"Account number: "<<accnum<<" --> Stock: "<<amount<<endl<<endl;
            }
        }
        sfile.close();
    }

    void updatepass()//change password of current user
    {
        cout<<"update password"<<endl;
        updpass up;
        up.update();
    }
public:
    void start()
    {
        int dex = 1;
        while(dex > 0 && dex < 7)
        {
            cout<<"1.Withdraw"<<endl;
            cout<<"2.Deposit"<<endl;
            cout<<"3.Statement"<<endl;
            cout<<"4.get stock"<<endl;
            cout<<"5.update Password"<<endl;
            cout<<"6.Logout"<<endl;
            if((cin>>dex))
            {
                if(dex == 1)
                {
                    withdraw();
                }
                else if(dex == 2)
                {
                    deposit();
                }
                else if(dex == 3)
                {
                    statement();
                }
                else if(dex == 4)
                {
                    getstock();
                }
                else if(dex == 5)
                {
                    updatepass();
                }
                else if(dex == 6)
                {
                    system("cls");
                    break;
                }
                else
                {
                    cout<<"Enter a valid choice"<<endl;
                    cin.clear();
                    dex = 1;
                    cin.ignore(40,'\n');
                }

            }
            else
            {
                cout<<"Enter a valid choice"<<endl;
                cin.clear();
                dex = 1;
                cin.ignore(40,'\n');
            }
        }
    }
};

class empl
{
    void newacc()
    {
        string accnum = to_string(1);
        cout<<"New account"<<endl;
        struct nacc
        {
            string name,tel,mobile,address;
        };
        nacc nac;
        map<string,nacc> accmap;
        string accno;
        fstream file;
        file.open("acc.txt",ios::in);
        string line;
        string aname,atel,amobile,aaddress;
        while(getline(file,line))
        {
            stringstream ss(line);
            ss>>accno>>aname>>atel>>amobile>>aaddress;
            nac.name = aname;
            nac.tel = atel;
            nac.mobile = amobile;
            nac.address = aaddress;
            accmap.insert(make_pair(accno,nac));
        }
        file.close();
        if(!accmap.empty())
        {
            int num =accmap.size() + 1;
            accnum = to_string(num);
        }
        string name,tel,mobile,address;
        cout<<"Account number: "<<accnum<<endl;
        cout<<"enter name: ";
        cin.ignore( 1024, '\n' ) ;
        getline(cin, name ) ;
        cout<<"enter tel: ";
        cin>>tel;
        cout<<"enter mobile: ";
        cin>>mobile;
        cout<<"enter address: ";
        cin.ignore( 1024, '\n' ) ;
        getline(cin, address ) ;
        cout<< "ok"<<endl;
        nacc nc;
        nc.name = name;
        nc.tel = tel;
        nc.mobile = mobile;
        nc.address = address;
        accmap.insert(make_pair(accnum,nc));
        ofstream acfile;
        acfile.open("acc.txt",ios::app);
        acfile<<accnum<<" '"<<name<<"' "<<tel<<" "<<mobile<<" '"<<address<<"'\n";
        acfile.close();
        // اضافة رقم الحساب الى ملف الارصدة
        ofstream bfile;
        bfile.open("balance.txt",ios::app);
        bfile<<accnum<<" "<<0<<"\n";
        bfile.close();
    }

    void updatepass()
    {
        cout<<"Update password"<<endl;
        updpass up;
        up.update();
    }
public:
    void start()
    {
        int dex =1;
        while(dex > 0)
        {
            cout<<"1.New Account"<<endl;
            cout<<"2.update Password"<<endl;
            cout<<"3.Logout"<<endl;
            if(cin>>dex)
            {
                if(dex == 1)
                {
                    newacc();
                }
                else if(dex == 2)
                {
                    updatepass();
                }
                else if(dex == 3)
                {
                    system("cls");
                    break;
                }
                else
                {
                    cout<<"Enter a valid choice"<<endl;
                    cin.clear();
                    dex = 1;
                    cin.ignore(40,'\n');
                }
            }
            else
            {
                cout<<"Enter a valid choice"<<endl;
                cin.clear();
                dex = 1;
                cin.ignore(40,'\n');
            }
        }
    }
};

int main()
{
    string id,pass;
    int dex =1;
    while(dex == 1)
    {
        cout<<"1.Login"<<endl;
        cout<<"2.End"<<endl;
        if(cin>>dex)
        {
            if(dex == 1)
            {
                cout<<"Enter Id: ";
                cin>>id;
                cout<<"Enter password: ";
                cin>>pass;
                string fid,fpass,type;
                string line;
                fstream file;
                file.open("users.txt",ios::in);
                bool y_n = false;
                while(getline(file,line))
                {

                    stringstream stream(line);
                    stream>>fid>>fpass>>type;
                    if(id == fid)
                    {
                        y_n=true;
                        usr = id;
                        if(pass==fpass){
                        if(type == "manager")
                        {
                            manager mg;
                            mg.start();
                        }
                        else if(type == "teller")
                        {
                            teller lr;
                            lr.start();
                        }
                        else if(type == "employee")
                        {
                            empl emp;
                            emp.start();
                        }
                        }
                        else
                        {
                            cout<<"ID or Password in error"<<endl;
                            break;
                        }
                    }
                }
                if(y_n==false){cout<<"User is not found."<<endl;}
                file.close();
            }
            else if(dex == 2)
            {
                break;
            }
            else
            {
                cout<<"Enter a valid choice"<<endl;
                cin.clear();
                dex = 1;
                cin.ignore(40,'\n');
            }
        }
        else
        {
            cout<<"Enter a valid choice"<<endl;
            cin.clear();
            dex = 1;
            cin.ignore(40,'\n');
        }
    }
    return 0;
}
