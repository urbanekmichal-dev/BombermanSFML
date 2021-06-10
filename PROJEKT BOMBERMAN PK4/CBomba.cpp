#include "CBomba.h"

CBomba::CBomba()
{
	
}

CBomba::CBomba(CTablica* wsk_tab, int _rozmiar)
	:wsk_tablica(wsk_tab), rozmiar(_rozmiar), postep(0)
{
	zegar.restart();
}

void CBomba::podlozBombe(int _x, int _y)
{
	x = _x;
	y = _y;
}

bool CBomba::sprawdzCzyZniszczono(Vector2f vec)
{
	bool damaged;				//Dla podanego jako argument wektora, kt�ry jest po�o�eniem postaci lub przeciwnika,
	int x = vec.x / 40;		//sprawdzane jest czy zosta�o mu zadane obra�enie w wyniku wybuchu bomby. Je�eli tak zwraca na jest warto�� true.
	int y = vec.y / 40;
	if (wsk_tablica->zwrocPole(x, y) > 18 && wsk_tablica->zwrocPole(x, y) < 36)
		damaged = true;
	else if (wsk_tablica->zwrocPole(x + 1, y) > 18 && wsk_tablica->zwrocPole(x + 1, y) < 36)
		damaged = true;
	else if (wsk_tablica->zwrocPole(x, y + 1) > 18 && wsk_tablica->zwrocPole(x, y + 1) < 36)
		damaged = true;
	else if (wsk_tablica->zwrocPole(x + 1, y + 1) > 18 && wsk_tablica->zwrocPole(x + 1, y + 1) < 36)
		damaged = true;
	else
		damaged = false;
	return damaged;
}

void CBomba::wysadz()
{
	int expl_size[4] = { 0 };
	przeliczRozmiarEksplozjii(expl_size);				//Obliczane s� d�ugo�ci wybuch�w w poszczeg�lnych kierunkach.
	wsk_tablica->ustawPole(x, y, 19);
	if (expl_size[kierunek::lewo])
	{
		int x1 = x - expl_size[kierunek::lewo], y1 = y;
		if (wsk_tablica->zwrocPole(x1 - 1, y1) == 3)			//Je�eli w polu za kt�rym ko�czy si� wybuch jest skrzynia,
			wsk_tablica->zniszSkrzynke(x1 - 1, y1);			//To wywwo�ywana jest funkcja niszcz�ca skrzyni�
		if (expl_size[kierunek::lewo] == 1)
			wsk_tablica->ustawPole(x1, y1, 23);				//Odpowiednio od d�ugo�ci wybuchu ustawiane s� odpowiednie pola na p
		else												//z czego ka�dy przedstawia inn� tekstur�
		{
			wsk_tablica->ustawPole(x1, y1, 20);
			wsk_tablica->ustawPole(x - 1, y, 22);				//Ta sama procedura jest powtarzana dla pozosta�ych kierunk�w
		}
		for (int i = 0; i < expl_size[kierunek::lewo] - 2; i++)
		{
			x1++;
			wsk_tablica->ustawPole(x1, y1, 21);
		}
	}
	else if (wsk_tablica->zwrocPole(x - 1, y) == 3)
		wsk_tablica->zniszSkrzynke(x - 1, y);
	if (expl_size[kierunek::dol])
	{
		int x1 = x, y1 = y + expl_size[kierunek::dol];
		if (wsk_tablica->zwrocPole(x1, y1 + 1) == 3)
			wsk_tablica->zniszSkrzynke(x1, y1 + 1);
		if (expl_size[kierunek::dol] == 1)
			wsk_tablica->ustawPole(x1, y1, 27);
		else
		{
			wsk_tablica->ustawPole(x1, y1, 24);
			wsk_tablica->ustawPole(x, y + 1, 26);
		}
		for (int i = 0; i < expl_size[kierunek::dol] - 2; i++)
		{
			y1--;
			wsk_tablica->ustawPole(x1, y1, 25);
		}
	}
	else if (wsk_tablica->zwrocPole(x, y + 1) == 3)
		wsk_tablica->zniszSkrzynke(x, y + 1);
	if (expl_size[kierunek::prawo])
	{
		int x1 = x + expl_size[kierunek::prawo], y1 = y;
		if (wsk_tablica->zwrocPole(x1 + 1, y1) == 3)
			wsk_tablica->zniszSkrzynke(x1 + 1, y1);
		if (expl_size[kierunek::prawo] == 1)
			wsk_tablica->ustawPole(x1, y1, 31);
		else
		{
			wsk_tablica->ustawPole(x1, y1, 28);
			wsk_tablica->ustawPole(x + 1, y, 30);

		}
		for (int i = 0; i < expl_size[kierunek::prawo] - 2; i++)
		{
			x1--;
			wsk_tablica->ustawPole(x1, y1, 29);
		}
	}
	else if (wsk_tablica->zwrocPole(x + 1, y) == 3)
		wsk_tablica->zniszSkrzynke(x + 1, y);
	if (expl_size[kierunek::gora])
	{
		int x1 = x, y1 = y - expl_size[kierunek::gora];
		if (wsk_tablica->zwrocPole(x1, y1 - 1) == 3)
			wsk_tablica->zniszSkrzynke(x1, y1 - 1);
		if (expl_size[kierunek::gora] == 1)
			wsk_tablica->ustawPole(x1, y1, 35);
		else
		{
			wsk_tablica->ustawPole(x1, y1, 32);
			wsk_tablica->ustawPole(x, y - 1, 34);
		}
		for (int i = 0; i < expl_size[kierunek::dol] - 2; i++)
		{
			y1++;
			wsk_tablica->ustawPole(x1, y1, 33);
		}
	}
	else if (wsk_tablica->zwrocPole(x, y - 1) == 3)
		wsk_tablica->zniszSkrzynke(x, y - 1);
}

void CBomba::przeliczRozmiarEksplozjii(int rozm_ekspolozji[])
{
	int x1 = x, y1 = y;
	x1--;
	while (wsk_tablica->zwrocPole(x1, y) == 2)
	{
		rozm_ekspolozji[kierunek::lewo]++;				//W p�tli sprawdzane jak jak daleko b�dzie si�ga� wybuch bomby.
		if (rozm_ekspolozji[kierunek::lewo] == rozmiar)	//Je�eli warto�� ta osi�gnie maksymaln� warto�� wybuchu, p�tla jest przerywana.
			break;									//Obliczenia si� powtarzane dla wszystkich czterech mo�liwych kierunk�w.
		x1--;
	}
	x1 = x + 1;
	while (wsk_tablica->zwrocPole(x1, y) == 2)
	{
		rozm_ekspolozji[kierunek::prawo]++;
		if (rozm_ekspolozji[kierunek::prawo] == rozmiar)
			break;
		x1++;
	}
	y1--;
	while (wsk_tablica->zwrocPole(x, y1) == 2)
	{
		rozm_ekspolozji[kierunek::gora]++;
		if (rozm_ekspolozji[kierunek::gora] == rozmiar)
			break;
		y1--;
	}
	y1 = y + 1;
	while (wsk_tablica->zwrocPole(x, y1) == 2)
	{
		rozm_ekspolozji[kierunek::dol]++;
		if (rozm_ekspolozji[kierunek::dol] == rozmiar)
			break;
		y1++;
	}
}

bool CBomba::wysadzBombe()
{
	return koniec;
}

void CBomba::zmienPostep()
{
	Time time = zegar.getElapsedTime();
	Time delay;
	if (postep< 9)
		delay = milliseconds(150);
	else
		delay = milliseconds(250);
	if (time > delay)					//Wprowadzone op�nienei czasowe, pozwalaj�ce na odpowiedni� animacj� bomby.
	{
		if (postep < 9)
			postep++;
		else
			koniec = true;
		zegar.restart();
	}
}

void CBomba::aktualizujTablice()
{
	if (postep == 9)
		wysadz();
	else
		wsk_tablica->ustawPole(x, y, postep + 10);
	zmienPostep();
}
CBomba::~CBomba()
{
	wsk_tablica->wyczyscEkspolozje();
}