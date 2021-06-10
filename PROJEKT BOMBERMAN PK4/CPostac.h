#pragma once
#include "Biblioteki.h"
#include "CElementy_rysowalne.h"
#include "EKierunek.h"
#include "CTablica.h"
#include "CBomba.h"
class CPostac : public CElementy_rysowalne
{
private:
	deque<CBomba*> bomby;
	int podlozone_bomby,
		max_ilosc_bomb,
		index_postaci,
		rozmiar_bomby;
	static int liczba_graczy;
	Vector2f zlapany_przez_przeciwnika;
protected:
	Clock zegar;
	Sprite serce, sprite;
	vector<Texture*> vec_wsk_tex_graczy;
	int indeks_tekstury,
		max_zycia,
		zycie,
		max_rozmiar_bomby;
	double predkosc,
		max_predkosc;
	bool nastepny_poziom;

	kierunek _kierunek, 
		_poprzedni_kierunek;

public:
	CPostac();
	~CPostac();
	Vector2f zwrocPozycje();
	virtual bool ruch(CTablica* tab_wsk, kierunek nowy_kierunek = kierunek::dol);
	void ustawTekstury();
	void zbierzUlepszenie(CTablica* tab_wsk, int x, int y);
	void uniknijPrzeszkody(CTablica *tab_wsk,Vector2f vector);
	
	void rysuj(RenderWindow* w_wsk);
	void zarzadzajBombami();
	bool czyZywy();
	CBomba* bombaEkspolodowala();
	void zadajObrazenia(int obrazenia);
	void WyczyscBombe(CBomba* wsk_bomby);
	bool podlozBombe(CTablica* wsk_tab);
	bool zlap();
	int zwrocIndeksPostaci();
	bool idzDoNAstepnegoPoziomu();
};