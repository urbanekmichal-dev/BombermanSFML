#pragma once
#include "Biblioteki.h"
#include "CPostac.h"
#include "CZnalezienie_drogi.h"
class CPrzeciwnik :public CPostac
{
	Clock zegar1,zegar2;
	CPostac* wsk_postaci;
	bool losowo,
		stop,
		sledz_postac,
		postac_osiagnieta,
		rozpoczecie;
	list<kierunek> sciezka;
	Vector2f miejsce_docelowe;
	CZnalezienie_drogi* znajdz_droge_wsk;
	int x, y, ruch_cnt;
public:
	CPrzeciwnik();
	CPrzeciwnik(CPostac* wsk_postaci, CTablica* wsk_tab, int cnt);
	~CPrzeciwnik();
	bool ruch(CTablica *tab_wsk,kierunek _nowy_kierunek=kierunek::dol);
	bool ruchLosowy(CTablica *wsk);
	bool ruchPoSciezce(CTablica *wsk);
	bool zmianaPlytki();
	void obliczDroge(CPostac* wsk_postaci);
	void ustawMiejsceDocelowe(int x1, int y1);
	void ustawTypPrzeciwnika(bool losowo);
	bool czyTOSledzonaPozstac(CPostac* wsk_postaci);
	bool postacOsiagnieta();

};