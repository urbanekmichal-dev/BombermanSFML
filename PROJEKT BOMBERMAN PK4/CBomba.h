#pragma once
#include "Biblioteki.h"
#include"CTablica.h"
#include "EKierunek.h"
class CBomba
{
	int x, y;
	CTablica* wsk_tablica;
	Clock zegar;
	bool koniec;
	int rozmiar, 
		postep;
public:
	CBomba();
	CBomba(CTablica* wsk_tab, int rozmiar);
	void podlozBombe(int _x, int _y);
	void przeliczRozmiarEksplozjii(int rozm_expl[]);
	bool sprawdzCzyZniszczono(Vector2f vec);
	void wysadz();
	bool wysadzBombe();
	void zmienPostep();
	void aktualizujTablice();
	~CBomba();
};