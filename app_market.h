#ifndef APP_MARKET_H
#define APP_MARKET_H
#include "product.h"
#include <QString>

class t_app_market
{
public:
	all_product ap;

	t_app_market() {}
	void help();
	void run();
	QString get_group();//exception
	QString check_product();
	bool check_data_base();
	void edit(QString p);
	void search(QString gp);

	void t_add_product();
	void t_edit_product();
	void t_print();
	void t_delete();
	void t_buy();
	void t_search();
	void t_change_categorize();
};
#endif // APP_MARKET_H
