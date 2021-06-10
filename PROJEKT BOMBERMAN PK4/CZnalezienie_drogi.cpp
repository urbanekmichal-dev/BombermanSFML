#include "CZnalezienie_drogi.h"

CZnalezienie_drogi::CZnalezienie_drogi()
{
}
CZnalezienie_drogi::~CZnalezienie_drogi()
{
}

CZnalezienie_drogi::CZnalezienie_drogi(CTablica* tablica)
	:wsk_tablica(tablica)
{
	
}

void CZnalezienie_drogi::wyczysc()
{
	lista_glowna.clear();
	sciezka.clear();
}

list<kierunek> CZnalezienie_drogi::znajdz(int x, int y, int cel_x, int cel_y)
{
	bool goal_reached = false;						//W algorytmie tworzona jest lista wszytskich pozycji, z kt�rych mo�na doj�c do celu.
	int counter = 0;
	lista_glowna.push_back(koordynowac(cel_x, cel_y, counter));		//Pole na kt�rym znajduje si� cel
	do
	{
		counter++;
		list<koordynowac>::iterator main_iter = lista_glowna.begin();
		list<koordynowac>::iterator main_iter_end = lista_glowna.end();
		list<list<koordynowac>> all_neighbours;
		while (main_iter != main_iter_end)
		{
			koordynowac ngb1((*main_iter).x - 1, (*main_iter).y, counter);
			koordynowac ngb2((*main_iter).x + 1, (*main_iter).y, counter);
			koordynowac ngb3((*main_iter).x, (*main_iter).y - 1, counter);		//Dla danego pola na li�cie g��wnej, utworzone zostaj� nowe pola s�siednie.
			koordynowac ngb4((*main_iter).x, (*main_iter).y + 1, counter);
			list<koordynowac> neighbour_list;
			neighbour_list.push_back(ngb1);
			neighbour_list.push_back(ngb2);
			neighbour_list.push_back(ngb3);
			neighbour_list.push_back(ngb4);
			list<koordynowac>::iterator ngb_iter = neighbour_list.begin();
			for (int i = 0; i < 4; i++)
			{
				bool incremented = false;
				if (wsk_tablica->zwrocPole((*ngb_iter).x, (*ngb_iter).y) != 2 && wsk_tablica->zwrocPole((*ngb_iter).x, (*ngb_iter).y) < 4)
				{
					list<koordynowac>::iterator tmp_iter = ngb_iter;
					ngb_iter++;													//Je�eli dane pole na li�cie p�l s�siednich jest przeszkod�, to jest ono usuwane.
					neighbour_list.erase(tmp_iter);
					continue;
				}
				list<koordynowac>::iterator tmp_main_iter = lista_glowna.begin();
				list<koordynowac>::iterator tmp_main_iter_end = lista_glowna.end();
				while (tmp_main_iter != tmp_main_iter_end)
				{
					if ((*ngb_iter).rownyXYaleNieWiekszy(*tmp_main_iter))
					{
						list<koordynowac>::iterator tmp_iter = ngb_iter;
						ngb_iter++;
						neighbour_list.erase(tmp_iter);					//Je�eli dane pole na li�cie p�l s�siednich ma takie same koordynaty jak pole ju� na li�cie g�ownej
						incremented = true;								//oraz licznik mniejszy lub r�wny, to jest on r�wnie� usuwany.
						break;
					}
					tmp_main_iter++;
				}
				list<list<koordynowac>>::iterator all_ngb_iter = all_neighbours.begin();
				list<list<koordynowac>>::iterator all_ngb_iter_end = all_neighbours.end();
				while (all_ngb_iter != all_ngb_iter_end)
				{
					list<koordynowac>::iterator added_ngb_iter = (*all_ngb_iter).begin();
					list<koordynowac>::iterator added_ngb_iter_end = (*all_ngb_iter).end();
					while (added_ngb_iter != added_ngb_iter_end)
					{
						if ((*ngb_iter).rownyXYaleNieWiekszy(*added_ngb_iter))
						{
							list<koordynowac>::iterator tmp_iter = ngb_iter;		//To samo jest sprawdzane dla p�l znajduj�cych si� na listach p�l, jeszcze nie dodanych do listy g�ownej
							ngb_iter++;
							neighbour_list.erase(tmp_iter);
							incremented = true;
							break;
						}
						added_ngb_iter++;
					}
					all_ngb_iter++;
					if (incremented)
						break;
				}
				if (!incremented)
					ngb_iter++;
			}
			all_neighbours.push_back(neighbour_list);			//Pola, kt�re nie zosta�y usuni�te, zostaj� dodane do listy "utrwalonych" s�siad�w.
			main_iter++;
		}
		list<list<koordynowac>>::iterator all_ngb_iter = all_neighbours.begin();
		list<list<koordynowac>>::iterator all_ngb_iter_end = all_neighbours.end();
		while (all_ngb_iter != all_ngb_iter_end)
		{
			list<koordynowac>::iterator ngb_iter = (*all_ngb_iter).begin();
			list<koordynowac>::iterator ngb_iter_end = (*all_ngb_iter).end();
			while (ngb_iter != ngb_iter_end)
			{
				lista_glowna.push_back(*ngb_iter);					//Do g�ownej listy p�l zostaj� dodanw wszystkie pola, kt�re zosta�y dopisane do listy "utrwalonych" s�siad�w.
				ngb_iter++;
			}
			all_ngb_iter++;
		}
		main_iter = lista_glowna.begin();
		while (main_iter != main_iter_end)
		{
			if ((*main_iter).rownyXY(x, y))
			{
				goal_reached = true;							//Sprawdzane jest, czy po�o�enie pocz�tkowe zosta�o ju� osi�gniete. Je�eli tak, to poszukiwania zosta�y zako�czone.
				break;
			}
			main_iter++;
		}
	} while (!goal_reached);
	if (goal_reached)
		konwertujNaSciezke(x, y, cel_x, cel_y, counter);
	return sciezka;
}
void CZnalezienie_drogi::konwertujNaSciezke(int x, int y, int cel_x, int cel_y, int licznik)
{
	do
	{									//Zadaniem funkcji jest zamiana listy p�l po kt�rych ma si� porusza� posta�, na list� konkretnych kierunk�w.
		licznik--;
		koordynowac left(x - 1, y, licznik);			//Tworzone s� pola s�siednie, do kt�rych mo�e prowadzi� �cie�ka.
		koordynowac right(x + 1, y, licznik);
		koordynowac up(x, y - 1, licznik);
		koordynowac down(x, y + 1, licznik);
		list<koordynowac>::iterator main_iter = lista_glowna.begin();
		list<koordynowac>::iterator main_iter_end = lista_glowna.end();
		while (main_iter != main_iter_end)
		{												//Dla listy wszystkich p�l sprawdzane jest, kt�ry z nich jest r�wny kt�remu� z s�siednich p�l 
			if ((*main_iter) == left)
			{
				x--;
				sciezka.push_front(kierunek::lewo);		//Na podstawie znalezionego pola do listy kierunk�w jest dodawany odpowiedni kierunek
				break;									//oraz nast�puje zmiana pozycji.			
			}
			else if ((*main_iter) == right)
			{
				x++;
				sciezka.push_front(kierunek::prawo);
				break;
			}
			else if ((*main_iter) == up)
			{
				y--;
				sciezka.push_front(kierunek::gora);
				break;
			}
			else if ((*main_iter) == down)
			{
				y++;
				sciezka.push_front(kierunek::dol);
				break;
			}
			main_iter++;
		}
	} while (licznik);
}
