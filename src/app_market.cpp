#include "app_market.h"
#include "product.h"
#include <QTextStream>

extern QTextStream qout;
extern QTextStream qin;

void t_app_market::run()
{
	QString cmd;
	forever
	{
		help();
		qout << "enter command :" <<flush ;
		qin >> cmd ;
		system("CLS");
		if ( QString("addproduct").startsWith(cmd.toLower()) )
			t_add_product();
		else if(QString("print").startsWith(cmd.toLower()))
			t_print();
		else if(QString("changecategorize").startsWith(cmd.toLower()))
			t_change_categorize();
		else if(QString("search").startsWith(cmd.toLower()))
			t_search();
		else if(QString("deleteproduct").startsWith(cmd.toLower()))
			t_delete();
		else if(QString("buyproduct").startsWith(cmd.toLower()))
			t_buy();
		else if(QString("editproduct").startsWith(cmd.toLower()))
			t_edit_product();
		else if(QString("signout").startsWith(cmd.toLower()))
			break;
		else if(QString("exit").startsWith(cmd.toLower()))
		{
			ap.save();
			exit(0);
		}
		else
			qout<<"no such command \""<< cmd<<"\""<<endl;
	}
}


void t_app_market::t_add_product()
{
	QString qsl[4];
	qout<<"enter name of product:"<<endl;
	qin>>qsl[0];
	if (ap.size() != 0)
	{
		auto itr=ap.find("all");
		if (itr->find(qsl[0]) != itr->end())
		{
			qout<<"your product has already exist!"<<endl;
			(*itr)[qsl[0]].print_all();
			qout<<"do you want to edit it?(Y/N)"<<endl;
			QString cmd;
			while(true)
			{
				qin>>cmd;
				if("y" == cmd.toLower())
				{
					edit(qsl[0]);
					return;
				}
				else if("n" == cmd.toLower())
					return;
				else
					qout<<"try again"<<endl;
			}
		}
	}
	qout<<"enter type of product:"<<endl;
	qin>>qsl[1];
	qout<<"enter producer of product:"<<endl;
	qin>>qsl[2];
	qout<<"enter number of product:"<<endl;
	qin>>qsl[3];
	while (qsl[3].toInt()<1)
	{
		qout<<"cant be this! please enter positive number"<<endl;
		qin>>qsl[3];
	}
	ap["all"][qsl[0]] = product {qsl[0],qsl[1],qsl[2],qsl[3].toInt()};
	if (ap.cat > 0)
		ap[qsl[ap.cat]][qsl[0]] = product {qsl[0],qsl[1],qsl[2],qsl[3].toInt()};
}

void t_app_market::t_edit_product()
{
	QString p=check_product();
	if(p==" ")
		return;
	edit(p);
}

void t_app_market::t_print()
{
	if (check_data_base())
		return;
	ap.print(get_group());
}

void t_app_market::t_delete()
{
	QString p=check_product();
	if(p==" ")
		return;
	auto itr=ap.find("all");
	QString q;
	if(ap.cat == 1)
	{
		q=(*itr)[p].type;
		ap[q].remove(p);
		if(ap[q].size()==0)
			ap.remove(q);
	}
	else if(ap.cat == 2)
	{
		q=(*itr)[p].producer;
		ap[q].remove(p);
		if(ap[q].size()==0)
			ap.remove(q);
	}
	itr->remove(p);
	if(ap["all"].size()==0)
		ap.remove("all");
	qout<<"delete done."<<endl;
}

void t_app_market::t_buy()
{
	QString p=check_product();
	if(p==" ")
		return;
	auto itr=ap.find("all");
	qout<<(*itr)[p].num<< "number of products is exists"<<endl;
	qout<<"how many do you want?"<<endl;
	int n;
	qin>>n;
	while(((*itr)[p].num - n) < 0)
	{
		qout<<"there isnt enough products. enter (0) to return or try again"<<endl;
		qin>>n;
		if(n == 0)
			return;
	}
	if (((*itr)[p].num - n) == 0)
	{
		QString q;
		if(ap.cat == 1)
		{
			q=(*itr)[p].type;
			ap[q].remove(p);
			if(ap[q].size()==0)
				ap.remove(q);
		}
		else if(ap.cat == 2)
		{
			q=(*itr)[p].producer;
			ap[q].remove(p);
			if(ap[q].size()==0)
				ap.remove(q);
		}
		itr->remove(p);
		if(ap["all"].size()==0)
			ap.remove("all");
	}
	else
		(*itr)[p].num = (*itr)[p].num - n;
	qout<<"done."<<endl;
}

void t_app_market::t_search()
{
	if(check_data_base())
		return;
	search(get_group());
}

void t_app_market::t_change_categorize()
{
	ap.categorize();
	ap.save();
	ap.clear();
	ap.load();
}

QString t_app_market::get_group()
{
	if(ap.cat==0)
		return "all";
	QString qs;
	qout<<"choose your group:"<<endl;
	qout<<"_____________________________________"<<endl;
	for(auto itr=ap.begin();itr!=ap.end();++itr)
		qout<<itr.key()<<endl;
	qout<<"_____________________________________"<<endl;
	qin>>qs;
	while (ap.find(qs) == ap.end())
	{
		qout<<"no such group " << qs<<"\ntry again"<<endl;
		qin>>qs;
	}
	return qs;
}

QString t_app_market::check_product()
{
	if(check_data_base())
		return " ";
	QString p;
	ap.print("all");
	qout<<"choose name of product:"<<endl;
	auto itr=ap.find("all");
	qin>>p;
	while (itr->find(p) == itr->end())
	{
		qout<<"no such product " << p << endl;
		qout<<"write (r) to return or try again"<<endl;
		qin>>p;
		if("r"==p.toLower())
			return " ";
	}
	return p;
}

bool t_app_market::check_data_base()
{
	if(ap.size()==0)
	{
		qout<<"data base is empty! do you want to add product?(Y/N)"<<endl;
		QString cmd;
		while(true)
		{
			qin>>cmd;
			if("y" == cmd.toLower())
			{
				t_add_product();
				return true;
			}
			else if("n" == cmd.toLower())
				return true;
			else
				qout<<"try again"<<endl;
		}
	}
	return false;
}

void t_app_market::edit(QString p)
{
	QString qsl[3];
	qout<<"enter type of product:"<<endl;
	qin>>qsl[0];
	qout<<"enter producer of product:"<<endl;
	qin>>qsl[1];
	qout<<"enter number of product:"<<endl;
	qin>>qsl[2];
	while (qsl[2].toInt() < 1)
	{
		qout<<"cant be this! please enter positive number"<<endl;
		qin>>qsl[2];
	}
	if (ap.cat == 1)
	{
		QString x=ap["all"][p].type;
		ap[x].remove(p);
		if(ap[x].size() == 0)
			ap.remove(x);
		ap[qsl[ap.cat - 1]][p] = product {p,qsl[0],qsl[1],qsl[2].toInt()};
	}
	else if (ap.cat == 2)
	{
		QString x=ap["all"][p].producer;
		ap[x].remove(p);
		if(ap[x].size() == 0)
			ap.remove(x);
		ap[qsl[ap.cat - 1]][p] = product {p,qsl[0],qsl[1],qsl[2].toInt()};
	}
	ap["all"][p]=product {p,qsl[0],qsl[1],qsl[2].toInt()};
	qout<<"edit done."<<endl;
	ap["all"][p].print_all();
}

void t_app_market::search(QString gp)
{
	qout<<"enter key word:"<<endl;
	QString key,p=QString();
	qin>>key;
	bool z;
	auto b = ap[gp].begin();
	auto e = ap[gp].end();
	while (p != "0")
	{
		key=key+p;
		system("CLS");
		qout<<"result of search for \""<<key<<"\""<<endl;
		z=false;
		for(auto x = b; x != e; ++x)
		{
			if(x.value().name.contains(key))
			{
				x.value().print_all();
				z=true;
			}
			else if(x.value().type.contains(key) && !(ap.cat == 1 && gp != "all"))
			{
				x.value().print_all();
				z=true;
			}
			else if(x.value().producer.contains(key) && !(ap.cat == 2 && gp != "all"))
			{
				x.value().print_all();
				z=true;
			}
		}
		if(!z)
			qout<<"no result!"<<endl;
		qout<<"[0]: to back to menu\n"
		   <<"[-1]: to backspace\n"
		  <<"[1]: to search new key word\n"
		 <<"or write letters to add to key word"<<endl;
		qin>>p;
		if(p == "-1")
		{
			key.resize(key.size()-1);
			p=QString();
		}
		if(p == "1")
		{
			key=QString();
			p=QString();
		}
		if(key.size() == 0)
		{
			qout<<"enter new key word"<<endl;
			qin>>key;
		}
	}
}

void t_app_market::help()
{
	qout<<"categerized by "<<flush;
	if(ap.cat==0)
		qout<<"none"<<endl;
	else if(ap.cat==1)
		qout<<"type"<<endl;
	else if(ap.cat==2)
		qout<<"producer"<<endl;
	qout<<"**********************************************"<<endl;
	qout<<"addProduct:         to add new product"<<endl;
	qout<<"editProduct:        to edit product"<<endl;
	qout<<"deleteProduct:      to delete product"<<endl;
	qout<<"buyProduct:         to buy products"<<endl;
	qout<<"changeCategorize:   to delete product"<<endl;
	qout<<"search:             to search products"<<endl;
	qout<<"print:              to print products"<<endl;
	qout<<"signOut:            to leave acount"<<endl;
	qout<<"exit:               to exit application"<<endl;
	qout<<"**********************************************"<<endl;
}
