#include "appmarket.h"
#include <QTextStream>

QTextStream qout (stdout);
QTextStream qin (stdin);

int main()
{
    appmarket p_app_market;
	p_app_market.run();
}
