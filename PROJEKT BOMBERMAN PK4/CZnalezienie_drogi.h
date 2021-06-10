#pragma once
#include "Biblioteki.h"
#include "CTablica.h"
#include "EKierunek.h"
class CZnalezienie_drogi
{
	struct koordynowac
	{
		int x, y, licznik;
	public:
		koordynowac(int _x, int _y, int _licznik) :
			x(_x), y(_y), licznik(_licznik){}
		bool rownyXYaleNieWiekszy(koordynowac coord)
		{
			if (x == coord.x && y == coord.y && licznik >= coord.licznik)
				return true;
			else
				return false;
		}
		bool rownyXY(int _x, int _y)
		{
			if (x == _x && y == _y)
				return true;
			else
				return false;
		}
		bool operator == (koordynowac coord)
		{
			if (x == coord.x && y == coord.y && licznik == coord.licznik)
				return true;
			else
				return false;
		}
	};
	CTablica* wsk_tablica;
	list<koordynowac> lista_glowna;
	list<kierunek> sciezka;
public:
	CZnalezienie_drogi();
	CZnalezienie_drogi(CTablica* wsk_tablica);
	~CZnalezienie_drogi();
	void wyczysc();
	void konwertujNaSciezke(int x,int y,int cel_x,int cel_y,int licznik);
	list<kierunek> znajdz(int x, int y, int cel_x, int cel_);
};

