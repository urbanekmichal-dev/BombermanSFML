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
	bool goal_reached = false;						//W algorytmie tworzona jest lista wszytskich pozycji, z których mo¿na dojœc do celu.
	int counter = 0;
	lista_glowna.push_back(koordynowac(cel_x, cel_y, counter));		//Pole na którym znajduje siê cel
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
			koordynowac ngb3((*main_iter).x, (*main_iter).y - 1, counter);		//Dla danego pola na liœcie g³ównej, utworzone zostaj¹ nowe pola s¹siednie.
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
					ngb_iter++;													//Je¿eli dane pole na liœcie pól s¹siednich jest przeszkod¹, to jest ono usuwane.
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
						neighbour_list.erase(tmp_iter);					//Je¿eli dane pole na liœcie pól s¹siednich ma takie same koordynaty jak pole ju¿ na liœcie g³ownej
						incremented = true;								//oraz licznik mniejszy lub równy, to jest on równie¿ usuwany.
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
							list<koordynowac>::iterator tmp_iter = ngb_iter;		//To samo jest sprawdzane dla pól znajduj¹cych siê na listach pól, jeszcze nie dodanych do listy g³ownej
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
			all_neighbours.push_back(neighbour_list);			//Pola, które nie zosta³y usuniête, zostaj¹ dodane do listy "utrwalonych" s¹siadów.
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
				lista_glowna.push_back(*ngb_iter);					//Do g³ownej listy pól zostaj¹ dodanw wszystkie pola, które zosta³y dopisane do listy "utrwalonych" s¹siadów.
				ngb_iter++;
			}
			all_ngb_iter++;
		}
		main_iter = lista_glowna.begin();
		while (main_iter != main_iter_end)
		{
			if ((*main_iter).rownyXY(x, y))
			{
				goal_reached = true;							//Sprawdzane jest, czy po³o¿enie pocz¹tkowe zosta³o ju¿ osi¹gniete. Je¿eli tak, to poszukiwania zosta³y zakoñczone.
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
	{									//Zadaniem funkcji jest zamiana listy pól po których ma siê poruszaæ postaæ, na listê konkretnych kierunków.
		licznik--;
		koordynowac left(x - 1, y, licznik);			//Tworzone s¹ pola s¹siednie, do których mo¿e prowadziæ œcie¿ka.
		koordynowac right(x + 1, y, licznik);
		koordynowac up(x, y - 1, licznik);
		koordynowac down(x, y + 1, licznik);
		list<koordynowac>::iterator main_iter = lista_glowna.begin();
		list<koordynowac>::iterator main_iter_end = lista_glowna.end();
		while (main_iter != main_iter_end)
		{												//Dla listy wszystkich pól sprawdzane jest, który z nich jest równy któremuœ z s¹siednich pól 
			if ((*main_iter) == left)
			{
				x--;
				sciezka.push_front(kierunek::lewo);		//Na podstawie znalezionego pola do listy kierunków jest dodawany odpowiedni kierunek
				break;									//oraz nastêpuje zmiana pozycji.			
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
