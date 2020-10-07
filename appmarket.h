#ifndef APPMARKET_H
#define APPMARKET_H
#include <QMap>
#include <QString>

class appmarket
{
public:
	QMap<QString,QString> qmss;
	void log_in();
	void help();
	void sign_in();
	void load();
	void run_app_market();
	void save();
	appmarket();
	~appmarket();
	void run();
};

#endif // APPMARKET_H
