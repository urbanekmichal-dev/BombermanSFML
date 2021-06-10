#pragma once
#include "Biblioteki.h"
#include "CMenu.h"
#include "CPoziom.h"
class CGra
{
	list<CPoziom*> lista_poziomow;
	RenderWindow* w_ptr;
	vector<Clock> clock_vec;
	vector<Time> time_vec;
	string wiadomosc_bledu;
	bool tryb_menu = true;
	bool poziom_stworzony = false;
	CMenu* wsk_menu;
	int liczba_graczy;
	int numer_poziomu = 0;
	Sound muzyka_menu;


public:
	CGra();
	~CGra();
	void Start();
	void CzytajKlawiature(CPoziom * wsk_poziom);
	bool StorzPoziomy(list<CPoziom*>::iterator& iterator_poziomow);
	void ZarzadzajPoziomami(list<CPoziom*>::iterator& level_iter, RenderWindow* ptr);
	void WystapilBlad(RenderWindow*w_ptr);
};
