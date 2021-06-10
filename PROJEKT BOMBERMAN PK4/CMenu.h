#pragma once
#include "Biblioteki.h"
class CMenu
{
	Font czcionka;
	bool rysuj_klawisze[9] = { false };
	vector<Sprite*> guziki;
	vector<Texture*> t_guziki;
	vector<Texture*>vec_wsk_tex_graczy;
	vector<Text*>vec_wsk_tekst;
	int nr_wcisnietego_klawisza = 0;
	bool zaden_klawisz_nie_wcisniety=true;
	int liczba_graczy_w_grze = 0;
	bool zamknij_menu = false;
	Texture* t_tlo;
	Sprite s_tlo;
	Sprite postac;
public:
	CMenu();
	~CMenu();
	void rysuj(RenderWindow * wsk);
	void WcisnietoKlawisz(Vector2i pozycja);
	void ZwolnionoKlawisz();
	void inicjalizacja();
	bool wyjscie();
	int zwrocIloscGraczy();
};
