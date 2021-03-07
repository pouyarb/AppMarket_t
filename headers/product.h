#ifndef PRODUCT_H
#define PRODUCT_H
#include <QString>
#include <QMap>

class product
{
public:
	QString name;
	QString type;
	QString producer;
	int num;

	void print(int i);
	void print_all();
};

class group_product : public  QMap<QString,product>
{
public:
	void print(int i);
};

class all_product : public QMap<QString,group_product>
{
public:
	int cat;

	all_product();
	~all_product();
	void save();
	void load();
	void print(QString gp);
	void categorize();
};

#endif // PRODUCT_H
