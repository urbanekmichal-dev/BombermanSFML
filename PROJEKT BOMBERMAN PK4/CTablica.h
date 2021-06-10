#pragma once
#include "Biblioteki.h"
#include "CElementy_rysowalne.h"

class CTablica : public CElementy_rysowalne
{
	vector<Texture*> vec_wsk_tex_tla,
		vec_wsk_tex_bomby,
		vec_wsk_tex_bonusy;
	int tablica[20][20] = { 0 },
		rozmiar = 20,
		liczba_skrzyni=0;
	Sprite tlo_gry,
		bomba,
		bonus;
	string nazwa_pliku = "board_";

public:
	CTablica();
	CTablica(string suffix);
	~CTablica();
	void rysuj(RenderWindow *w_ptr);
	int zwrocPole(int x, int y);
	void ustawPole(int x, int y, int wartosc);
	void wyczyscEkspolozje();
	void ustawTektureBomby(int index);
	void zniszSkrzynke(int x, int y);
};