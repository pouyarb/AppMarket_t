#include "product.h"
#include <QTextStream>
#include <QFile>

extern QTextStream qout;
extern QTextStream qin;

void product::print(int i)
{
	if(i == 1)
	{
		qout<<"--------------------------------"<<endl;
		qout<<"["<<name<<"]"<<endl;
		qout<<"  producer: "<<producer<<endl;
		qout<<"  number: "<<num<<endl;
		qout<<"--------------------------------"<<endl;
	}
	if(i == 2)
	{
		qout<<"--------------------------------"<<endl;
		qout<<"["<<name<<"]"<<endl;
		qout<<"  type: "<<type<<endl;
		qout<<"  number: "<<num<<endl;
		qout<<"--------------------------------"<<endl;
	}
}

void product::print_all()
{
	qout<<"--------------------------------"<<endl;
	qout<<"["<<name<<"]"<<endl;
	qout<<"  type: "<<type<<endl;
	qout<<"  producer: "<<producer<<endl;
	qout<<"  number: "<<num<<endl;
	qout<<"--------------------------------"<<endl;
}
/////////////////////////////////////////

void group_product::print(int i)
{
	qout<<"================================="<<endl;
	for(auto itr = begin() ; itr != end() ; ++ itr)
		itr.value().print(i);
	qout<<"================================="<<endl;
}
////////////////////////////////////////

all_product::all_product()
{
	categorize();
	load();
}

all_product::~all_product()
{
	save();
}

void all_product::save()
{
	QFile file ("products.txt") ;
	file.open(QFile::Text|QFile::WriteOnly) ;
	if ( !file.isOpen() )
	{
		qout << " cant write into file of data!" << endl ;
		return ;
	}
	QTextStream qts (&file);

	for (auto itr = (*this)["all"].begin() ; itr != (*this)["all"].end() ; ++itr)
		qts << itr.key() << "," << itr.value().type << "," << itr.value().producer << "," << itr.value().num<<endl;
	file.close();
}

void all_product::load()
{
	QFile file ("products.txt");
	file.open(QFile::Text|QFile::ReadOnly) ;
	if ( !file.isOpen() )
	{
		qout<<"can't open file of data!"<<endl;
		return ;
	}
	QTextStream qts (&file);

	while (!qts.atEnd())
	{
		QStringList qsl = qts.readLine().split(',');
		if(qsl.size()!= 4 )
			continue ;
		(*this)["all"][qsl[0]] = product {qsl[0],qsl[1],qsl[2],qsl[3].toInt()};
		if (cat > 0)
			(*this)[qsl[cat]][qsl[0]] = product {qsl[0],qsl[1],qsl[2],qsl[3].toInt()};
	}
	file.close();
}

void all_product::print(QString gp)
{
	if(gp!="all")
	{
		qout<<"+++"<<gp<<"+++"<<endl;
		(*this)[gp].print(cat);
	}
	else
	{
		for(auto itr = (*this)[gp].begin(); itr != (*this)[gp].end(); ++itr)
			itr.value().print_all();
	}
}

void all_product::categorize()
{
	int i;
	qout << "do you want to categorize by____\n0: none\n1: type\n2: producer"<<endl;
	qin >>i;
	while (i<0 || i>2)
	{
		qout<<"wrong number!!! try again"<<endl;
		qin>>i;
	}
	cat = i;
}
