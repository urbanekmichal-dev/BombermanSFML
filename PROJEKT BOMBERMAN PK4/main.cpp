#include "Biblioteki.h"
#include "CGra.h"

int main()
{
	CGra * nowa = new CGra();
	nowa->Start();
	delete nowa;
	system("pause");
	return 0;
}