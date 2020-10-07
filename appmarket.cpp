#include "appmarket.h"
#include "app_market.h"
#include <QTextStream>
#include <QFile>

extern QTextStream qout;
extern QTextStream qin;

appmarket::appmarket()
{
	load ();
}

appmarket::~appmarket()
{
	save();
}

void appmarket::run()
{
	QString cmd;
	forever
	{
		help();
		qout << "enter command :" <<flush ;
		qin >> cmd ;
		system("CLS");
		if ( QString("signin").startsWith(cmd.toLower()) )
			sign_in();
		else if ( QString("login").startsWith(cmd.toLower()) )
			log_in();
		else if(QString("exit").startsWith(cmd.toLower()))
			break;
		else
			qout<<"no such command \""<< cmd<<"\""<<endl;
	}
}

void appmarket::log_in()
{
	QString user,pass;
	qout<<"enter username: "<<flush;
	qin>>user;
	qout<<"enter your password: "<<flush;
	qin>>pass;
	auto itr = qmss.find(user);
	forever
	{
		if (itr != qmss.end())
		{
			if(itr.value()==pass)
			{
				run_app_market();
				return;
			}
		}
		qout<<"your username doesn't exist!!\nplease try again or \nsignIn: to sign in\nexit: to exit"<<endl;
		qin>>user;
		if(QString("signin").startsWith(user.toLower()))
		{
			sign_in();
			return;
		}
		else if(QString("exit").startsWith(user.toLower()))
		{
			save();
			exit(0);
		}
		else
		{
			qout<<"enter your password: "<<flush;
			qin>>pass;
			itr=qmss.find(user);
		}
	}
}

void appmarket::load()
{
	QFile file ("users.txt");
	file.open(QFile::Text|QFile::ReadOnly) ;
	if ( !file.isOpen() )
	{
		qout<<"can't open file"<<endl;
		return ;
	}
	QTextStream qts (&file);

	while (!qts.atEnd())
	{
		QStringList qsl = qts.readLine().split(',');
		if(qsl.size()!= 2 )
			continue ;
		qmss[qsl[0]] = qsl[1];
	}
	file.close();
}

void appmarket::run_app_market()
{
	save();
	system("CLS");
	t_app_market my_app_market;
	my_app_market.run();
}

void appmarket::save()
{
	QFile file ("users.txt") ;
	file.open(QFile::Text|QFile::WriteOnly) ;
	if ( !file.isOpen() )
	{
		qout << " cant write into file " << endl ;
		return ;
	}
	QTextStream qts (&file);

	for (auto itr=qmss.begin();itr != qmss.end();++itr)
		qts<<itr.key()<<","<<itr.value()<<endl;
	file.close();
}

void appmarket::help()
{
	qout<<"**********************************************"<<endl;
	qout<<"logIn:    to log in existing acount"<<endl;
	qout<<"signIn:   to create new acount"<<endl;
	qout<<"exit:     to exit application"<<endl;
	qout<<"**********************************************"<<endl;
}

void appmarket::sign_in()
{
	QString user,pass,cpass;
	qout<<"enter username: "<<flush;
	qin>>user;
	while(qmss.find(user)!=qmss.end())
	{
		qout<<"this username has been signed in. enter another username"<<endl;
		qin>>user;
	}
	while(true)
	{
		qout<<"enter your password: "<<flush;
		qin>>pass;
		qout<<"confirm your password: "<<flush;
		qin>>cpass;
		if(cpass==pass)
			break;
		qout<<"your password doesn't match please try again"<<endl;
	}
	qmss[user]=pass;
	run_app_market();
}
