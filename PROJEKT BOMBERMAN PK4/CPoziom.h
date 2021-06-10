#pragma once

#include "Biblioteki.h"
#include "EKierunek.h"
#include "CElementy_rysowalne.h"
#include "CTablica.h"
#include "CPrzeciwnik.h"
class CPoziom
{
	list<CElementy_rysowalne*> lista_elementow_rysowalnych;
	string wiadomosc_bledu;
	Sprite przycisk;
	bool wroc_do_menu = false,
		blad=false,
		nastepny_poziom=false,
		koniec_gry=false;
	int liczba_postaci,
		liczba_przeciwnikow,
		liczba_graczy,
		podlozone_bomby;
	Texture* wsk_tex_guzik_wyjscia;
	Sprite guzik;
	Font czcionka;
	Text napis;
	Clock zegar;
public:
	CPoziom();
	CPoziom(int liczba_graczy,string suffix);
	~CPoziom();
	void WcisnietoKlawisz(Vector2i pozycja);
	void ruch(int numer_postaci, kierunek _kierunek);
	void podlozBombe(int numer_postaci);
	bool rozpocznijNowaGre();
	bool WystapilBlad(string&  wiadomosc);

	void RuchPrzeciwnikow();
	void SprawdzKolizje();
	void SprawdzBomby();
	void SprawdzZycie();
	void SprawdzCzyKoniecPoziomu();
	void rysuj(RenderWindow* window);
	bool przeciwnikOsiagnalPostac(CPostac * wsk_postac,CPrzeciwnik * wsk_przeciwnik);
	bool idzDoMenu();
	bool nastepnyPoziom();

};