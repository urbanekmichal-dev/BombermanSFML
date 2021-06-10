#include "CPrzeciwnik.h"

CPrzeciwnik::CPrzeciwnik()
{

}

CPrzeciwnik::CPrzeciwnik(CPostac* _wsk_postaci, CTablica* wsk_tab, int cnt)
	:wsk_postaci(_wsk_postaci), x(18), losowo(false), sledz_postac(true),
	postac_osiagnieta(false), rozpoczecie(true),y(1)
{
	zegar1.restart();
	zegar2.restart();
	srand((unsigned int)time(0));
	_kierunek = kierunek::dol;
	_poprzedni_kierunek = kierunek::lewo;
	predkosc = 1.0;
	zycie = 2;
	for (int i = 0; i < 14; i++)
	{
		if (!vec_wsk_tex_graczy[i]->loadFromFile("Bomberman_sprite.png", IntRect(16 * i, 48, 16, 16)))
			throw "Nie mozna wczytac tekstur postaci!";						//£adowanie tekstur z pliku.
	}
	vector<Texture*>::iterator tex_iter = vec_wsk_tex_graczy.begin();
	sprite.setTexture(**tex_iter);
	sprite.setScale(2.5f, 2.5f);
	if (cnt == 0)							//Ustawienie pocz¹tkowej pozycji, w zale¿noœci od przekazanego argumentu.
		sprite.setPosition(720, 40);
	else if (cnt == 1)
		sprite.setPosition(720, 720);
	else
		sprite.setPosition(40, 720);
	znajdz_droge_wsk = new CZnalezienie_drogi(wsk_tab);
}

CPrzeciwnik::~CPrzeciwnik()
{
	delete znajdz_droge_wsk;
}

bool CPrzeciwnik::ruch(CTablica *tab_wsk, kierunek _nowy_kierunek)
{
	if (losowo)
		return ruchLosowy(tab_wsk);
	else
		return ruchPoSciezce(tab_wsk);
}

bool CPrzeciwnik::ruchPoSciezce(CTablica* tab_wsk)
{
	Time time = zegar1.getElapsedTime();
	if (sciezka.empty() && (stop || zmianaPlytki()))
	{
		stop = true;
		obliczDroge(wsk_postaci);
		if (!sciezka.empty())							//Gdy lista kierunków jest pusta, konieczne jest wyznaczeni nowej trasy.
		{
			_kierunek = sciezka.back();
			sciezka.pop_back();
			Vector2f vec = sprite.getPosition();
			int x1 = (int)vec.x / 40;
			int y1 = (int)vec.y / 40;
			ustawMiejsceDocelowe(x1, y1);
		}
	}
	if (!sciezka.empty())
		stop = false;					//Je¿eli lista kierunków nadal jest pusta, oznacza to,
	if (zmianaPlytki())					//¿e cel w postaci dotarcia do postaci zosta³ osi¹gniêty i przeciwnik nie powinien siê poruszaæ.
	{
		if (!sciezka.empty())				//Je¿eli, krótkotrwa³y cel zosta³ osi¹gniêty, to nowy kierunek pobierany jest z lisy kierunków.
		{
			_kierunek = sciezka.back();
			sciezka.pop_back();
		}
		Vector2f vec = sprite.getPosition();
		int x1 = (int)vec.x / 40;
		int y1 = (int)vec.y / 40;
		ustawMiejsceDocelowe(x1, y1);
	}
	if (!stop && time > milliseconds(5))
	{
		CPostac::ruch(tab_wsk, _kierunek);	//Przeciwnik porusza siê, gdy nast¹pi³o odpowiednie opóŸnienie czasowe i nie jest on zatrzymany.
		zegar1.restart();
		return true;
	}
	else return false;
}

bool CPrzeciwnik::ruchLosowy(CTablica* tab_wsk)

{
	Time time = zegar1.getElapsedTime();
	if (time > milliseconds(5))
	{
		time = zegar2.getElapsedTime();
		if (CPostac::ruch(tab_wsk, _kierunek) || time > seconds(2))			//Przecinnik porusza siê w danym kierunku
		{
			Vector2f vector = sprite.getPosition();							//Je¿eli nie mo¿e siê poruszyæ, lub minie okreslony czas, nastêpuje zmiana kierunku
			int x = (int)vector.x / 40;
			int y = (int)vector.y / 40;
			bool possible_directions[4] = { false };
			if (tab_wsk->zwrocPole(x - 1, y) == 2 || tab_wsk->zwrocPole(x - 1, y) > 9)	//Ustalane jest w jakich kierunkach przeciwnik mo¿e siê poruszyæ
				possible_directions[kierunek::lewo] = true;
			if (tab_wsk->zwrocPole(x + 1, y) == 2 || tab_wsk->zwrocPole(x + 1, y) > 9)
				possible_directions[kierunek::prawo] = true;
			if (tab_wsk->zwrocPole(x, y - 1) == 2 || tab_wsk->zwrocPole(x, y - 1) > 9)
				possible_directions[kierunek::gora] = true;
			if (tab_wsk->zwrocPole(x, y + 1) == 2 || tab_wsk->zwrocPole(x, y + 1) > 9)
				possible_directions[kierunek::dol] = true;
			int i = 0;
			int tmp;
			do
			{
				tmp = rand() % 4;								//Nastêpuje losowanie odpowiedniego kierunku poruszania siê.
				i++;
			} while (!possible_directions[tmp] && i < 100);
			if (tmp == 0)
				_kierunek = kierunek::lewo;					//Ustawienie wylosowanego kierunku.
			else if (tmp == 1)
				_kierunek = kierunek::prawo;
			else if (tmp == 2)
				_kierunek = kierunek::gora;
			else
				_kierunek = kierunek::dol;
			zegar2.restart();
			return true;
		}
		zegar1.restart();
		return true;
	}
	else
		return false;
}

bool CPrzeciwnik::zmianaPlytki()
{
	if (rozpoczecie)
	{
		rozpoczecie = false;
		return true;
	}
	else
	{


		if (miejsce_docelowe == sprite.getPosition())
			return true;
		else
			return false;
	}
}

void CPrzeciwnik::obliczDroge(CPostac* wsk_postaci)
{
	Vector2f vec = sprite.getPosition();
	int x1 = (int)vec.x / 40;			//Wyznaczane są pola, startowe oraz końcowe dla drogi, która ma zostać wyznaczona.
	int y1 = (int)vec.y / 40;
	int docelowy_x;
	int docelowy_y;
	Vector2f vec1;
	vec1 = wsk_postaci->zwrocPozycje();
	docelowy_x = (int)vec1.x / 40;
	docelowy_y = (int)vec1.y / 40;
	sciezka = znajdz_droge_wsk->znajdz(x1, y1, docelowy_x, docelowy_y);		//Wyznaczenie odpowiedniej œcie¿ki.
	znajdz_droge_wsk->wyczysc();				//Wyczyszczenie powsta³ej œcie¿ki.
}

void CPrzeciwnik::ustawMiejsceDocelowe(int x1, int y1)
{
	Vector2f vec;
	if (_kierunek == kierunek::lewo)				//Zgodnie z obencym kierunkiem, ustawiany jest obecny cel poruszania siê.
		x1--;
	else if (_kierunek == kierunek::prawo)
		x1++;
	else if (_kierunek == kierunek::gora)
		y1--;
	else if (_kierunek == kierunek::dol)
		y1++;
	vec.x = (float)x1 * 40;
	vec.y = (float)y1 * 40;
	miejsce_docelowe = vec;
}

void CPrzeciwnik::ustawTypPrzeciwnika(bool _losowo)
{
	losowo = _losowo;
}

bool CPrzeciwnik::czyTOSledzonaPozstac(CPostac* _wsk_postac)
{
	if (wsk_postaci == _wsk_postac)
		return true;
	else return false;
}

bool CPrzeciwnik::postacOsiagnieta()
{
	bool tmp = postac_osiagnieta;
	if (postac_osiagnieta)
	{
		postac_osiagnieta = false;
		sledz_postac = false;
	}
	return  tmp;
}