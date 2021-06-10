#include "CPoziom.h"
CPoziom::CPoziom()
{

}
CPoziom::CPoziom(int _liczba_graczy, string suffix)
	:podlozone_bomby(0),liczba_postaci(_liczba_graczy),
	liczba_graczy(_liczba_graczy),
	koniec_gry(false),nastepny_poziom(false),
	wroc_do_menu(false),blad(false)
{
	try
	{
		liczba_przeciwnikow = 4 - liczba_postaci;
		CPostac** wsk_tablica_postaci = new CPostac * [liczba_postaci];
		CTablica* wsk_tablicy_gry = new CTablica(suffix);
		lista_elementow_rysowalnych.push_back(wsk_tablicy_gry);
		for (int i = 0; i < liczba_postaci; i++)
		{
			CPostac* wsk_postaci = new CPostac;				//Tworzenie odpowiendiej liczby postaci, zgodnie z przekazanym argumentem.
			wsk_tablica_postaci[i] = wsk_postaci;
			lista_elementow_rysowalnych.push_back(wsk_postaci);
		}
		for (int i = 0; i < liczba_przeciwnikow; i++)
		{
			CPrzeciwnik* wsk_przeciwnik;									//Tworzenie odpowiendiej liczby przeciwników, zgodnie z przekazanym argumentem.
			if (liczba_przeciwnikow == 3)						//W zaleŸnoœci od liczby przeciwników, ich rodzaj jest odpowiednio ustawiany.
			{
				wsk_przeciwnik = new CPrzeciwnik(wsk_tablica_postaci[0], wsk_tablicy_gry, i);
				wsk_przeciwnik->ustawTypPrzeciwnika(true);
			}
			else
				wsk_przeciwnik = new CPrzeciwnik(wsk_tablica_postaci[i], wsk_tablicy_gry, i);
			lista_elementow_rysowalnych.push_back(wsk_przeciwnik);
		}
		wsk_tex_guzik_wyjscia = new Texture;
		if (!wsk_tex_guzik_wyjscia->loadFromFile(SCIEZKA_GUZIKI_MENU, IntRect(1439, 1, 61, 63)))
			throw "Nie mozna wczytac tekstury przycisku !";
		guzik.setTexture(*wsk_tex_guzik_wyjscia);
		guzik.setScale(0.8, 0.8);
		guzik.setPosition(740, 815);
		if (!czcionka.loadFromFile(SCIEZKA_CZCIONKI_ARCADE))
			throw "Nie mozna wczytac czcionki z pliku!";
		napis.setFont(czcionka);
		napis.setString("Koniec Gry!");
		napis.setFillColor(Color::Red);
		napis.setOutlineColor(Color::Black);
		napis.setOutlineThickness(3);
		napis.setCharacterSize(60);
		napis.setPosition(250, 300);
		delete[]wsk_tablica_postaci;
	}
	catch (const char* error_message)
	{
		wiadomosc_bledu = error_message;
		blad = true;
	}
}


CPoziom::~CPoziom()
{
	delete wsk_tex_guzik_wyjscia;
	list<CElementy_rysowalne*>::iterator ent_iter = lista_elementow_rysowalnych.begin();
	list<CElementy_rysowalne*>::iterator ent_iter_end = lista_elementow_rysowalnych.end();
	while (ent_iter != ent_iter_end)
	{
		list<CElementy_rysowalne*>::iterator erase = ent_iter;
		delete(*erase);
		ent_iter++;
	}
	lista_elementow_rysowalnych.clear();
}

void CPoziom::podlozBombe(int numer_postaci)
{
	list<CElementy_rysowalne*>::iterator ent_iter = lista_elementow_rysowalnych.begin();
	CTablica* b_ptr = dynamic_cast<CTablica*>(*ent_iter);
	if (b_ptr == nullptr)
		throw "Tablica była nullptr'em";
	ent_iter++;
	for (int i = 0; i < liczba_postaci; i++)
	{
		CPostac* ch_ptr = dynamic_cast<CPostac*>(*ent_iter);
		if (ch_ptr->zwrocIndeksPostaci() == numer_postaci - 1)
		{
			if (ch_ptr->podlozBombe(b_ptr))
				podlozone_bomby++;
			break;
		}
		ent_iter++;
	}
}

void CPoziom::ruch(int numer_postaci,kierunek _kierunek)
{
	list<CElementy_rysowalne*>::iterator ent_iter = lista_elementow_rysowalnych.begin();
	CTablica* b_ptr = dynamic_cast<CTablica*>(*ent_iter);
	ent_iter++;
	for (int i = 0; i < liczba_postaci; i++)
	{
		CPostac* ch_ptr = dynamic_cast<CPostac*>(*ent_iter);	//Dla wszystkich dstêpnych postaci nastêpuje sprawdzenie,
		if (ch_ptr->zwrocIndeksPostaci() == numer_postaci - 1)	//który z nich powinien zostaæ przesuniêty.
		{
			ch_ptr->ruch(b_ptr, _kierunek);
			if (liczba_graczy == 1)
				nastepny_poziom = ch_ptr->idzDoNAstepnegoPoziomu();				//Dla trybu singleplayer zostaje sprawdzenie, czy postaæ znalaz³a drzwi.
			break;
		}
		ent_iter++;
	}
}

bool CPoziom::rozpocznijNowaGre()
{
	if (liczba_graczy == 1 && liczba_postaci == 0)
		return true;
	else
		return false;
}

bool CPoziom::WystapilBlad(string& wiadomosc)
{
	if (blad)
	{
		wiadomosc = wiadomosc_bledu;
		return true;
	}
	else
		return false;
}

void CPoziom::RuchPrzeciwnikow()
{
	list<CElementy_rysowalne*>::iterator iterator_elementow_rysowalnych = lista_elementow_rysowalnych.begin();
	CTablica* wsk_tablicy_gry = dynamic_cast<CTablica*>(*iterator_elementow_rysowalnych);
	for (int i = 0; i <= liczba_postaci; i++)
		iterator_elementow_rysowalnych++;
	for (int i = 0; i < liczba_przeciwnikow; i++)
	{
	 CPrzeciwnik* wsk_przeciwnik = dynamic_cast<CPrzeciwnik*>(*iterator_elementow_rysowalnych);		//Wywo³ane zostaj¹ funkcje przesuwaj¹ce dla wszystkich przeciwników.	
		wsk_przeciwnik->ruch(wsk_tablicy_gry);
		iterator_elementow_rysowalnych++;
	}
}
void CPoziom::SprawdzKolizje()
{
	list<CElementy_rysowalne*>::iterator ent_iter = lista_elementow_rysowalnych.begin();
	CTablica* b_ptr = dynamic_cast<CTablica*>(*ent_iter);
	ent_iter++;
	list<CElementy_rysowalne*>::iterator ent_iter_tmp = ent_iter;
	for (int i = 0; i < liczba_postaci; i++)
	{
		CPostac* ch_ptr = dynamic_cast<CPostac*>(*ent_iter_tmp);
		ent_iter = ent_iter_tmp;
		for (int j = i; j < liczba_postaci; j++)
			ent_iter++;
		for (int j = 0; j < liczba_przeciwnikow; j++)
		{
			CPrzeciwnik* e_ptr = dynamic_cast<CPrzeciwnik*>(*ent_iter);
			if (przeciwnikOsiagnalPostac(ch_ptr, e_ptr))				//Sprawdzenie, czy dana postaæ zosta³a z³apana przez przeciwnika
			{
				if (ch_ptr->zlap())								//Zabezpieczenie przed wielokrotnymi obra¿eniami przy jednej kolizji.
					ch_ptr->zadajObrazenia(1);
			}
			ent_iter++;
		}
		ent_iter_tmp++;
	}
}

void CPoziom::SprawdzBomby()
{
	if (podlozone_bomby)
	{
		list<CElementy_rysowalne*>::iterator ent_iter = lista_elementow_rysowalnych.begin();
		list<CElementy_rysowalne*>::iterator ent_iter_end = lista_elementow_rysowalnych.end();
		ent_iter++;
		list<CElementy_rysowalne*>::iterator ent_iter_tmp = ent_iter;
		while (ent_iter != ent_iter_end)
		{
			CPostac* ch_ptr = dynamic_cast<CPostac*>(*ent_iter);
			ch_ptr->zarzadzajBombami();						//Dla wszystkich postaci, zostaj¹ wywo³ane funkcje, odpowiedzialne za zarz¹dzanie ich bombami.
			ent_iter++;
		}
		ent_iter = ent_iter_tmp;
		while (ent_iter != ent_iter_end)
		{
			CPostac* ch_ptr = dynamic_cast<CPostac*>(*ent_iter);
			CBomba* bomb_ptr = ch_ptr->bombaEkspolodowala();
			if (bomb_ptr != nullptr)					//Sprawdzane jest czy bomba którejœ z postaci eksplodowa³a
			{

				while (ent_iter_tmp != ent_iter_end)	//Je¿eli tak, to dla wszystkich postaci i przeciwników na plaszy nastêpuje sprawdzenie,
				{										//czy zosta³y im zadane im obra¿enia.
					CPostac* ch_ptr_tmp = dynamic_cast<CPostac*>(*ent_iter_tmp);
					if (bomb_ptr->sprawdzCzyZniszczono(ch_ptr_tmp->zwrocPozycje()))
						ch_ptr_tmp->zadajObrazenia(1);		//Funkcja odejmuj¹ zadan¹ wartoœæ ¿ycia.
					ent_iter_tmp++;
				}
				ch_ptr->WyczyscBombe(bomb_ptr);			//Usuwanie bomby danej postaci.
				podlozone_bomby--;
				ent_iter_tmp = lista_elementow_rysowalnych.begin();
			}
			ent_iter++;
		}
	}

}
void CPoziom::SprawdzZycie()
{
	bool already_incremented = false;
	int cnt = 0;
	list<CElementy_rysowalne*>::iterator ent_iter = lista_elementow_rysowalnych.begin();
	list<CElementy_rysowalne*>::iterator ent_iter_end = lista_elementow_rysowalnych.end();
	CTablica* b_ptr = dynamic_cast<CTablica*>(*ent_iter);
	ent_iter++;
	while (ent_iter != ent_iter_end)
	{
		CPostac* ch_ptr = dynamic_cast<CPostac*>(*ent_iter);
		if (!ch_ptr->czyZywy())												//Sprawdzenie, czy dana postaæ lub przeciwnik jest jeszcze ¿ywy
		{																	//Je¿eli nie, to jest on usuwany z listy wszystkich postaci w tym poziomie
			if (dynamic_cast<CPrzeciwnik*>(*ent_iter))
				liczba_przeciwnikow--;
			else
			{
				list<CElementy_rysowalne*>::iterator ent_iter_tmp = ent_iter;
				for (int i = cnt; i < liczba_postaci; i++)
					ent_iter_tmp++;
				while (ent_iter_tmp != ent_iter_end)
				{
					CPrzeciwnik* e_ptr = dynamic_cast<CPrzeciwnik*>(*ent_iter_tmp);		//Sprawdzanie, czy dana postaæ jest œledzona przez któregoœ z przeciwników.
					if (e_ptr->czyTOSledzonaPozstac(ch_ptr))
						e_ptr->ustawTypPrzeciwnika(true);							//Je¿eli tak, to typ danego przeciwnika jest poruszanie siê przypadkowe.
					ent_iter_tmp++;
				}
				liczba_postaci--;
				cnt--;
			}
			delete* ent_iter;												//Usuniêcie postaci lub przeciwnika.
			ent_iter = lista_elementow_rysowalnych.erase(ent_iter);
			already_incremented = true;
		}
		if (!already_incremented)
			ent_iter++;
		already_incremented = false;
		cnt++;
	}
}
void CPoziom::SprawdzCzyKoniecPoziomu()
{
	if (liczba_graczy == 1 && liczba_postaci == 0)
	{
		if (!koniec_gry)							//Je¿eli wybrany jest tryb gry singleplayer i nie ma ju¿ przeciwników na planszy,
		{										//to nastêpuje wyjœcie do menu.
			zegar.restart();
			koniec_gry = true;
		}
		Time time = zegar.getElapsedTime();
		if (time > seconds(2))
			wroc_do_menu = true;
	}
	else if ((liczba_graczy > 1 && liczba_postaci <= 1) || nastepny_poziom || koniec_gry)
	{
		if (!koniec_gry)								//Je¿eli wybrany jest try gry multiplayer i liczba graczy jest równa jeden,
		{											//to nastêpuje przejœcie do natêpnego poziomu
			napis.setString("Next  level");
			napis.setFillColor(Color::Green);
			zegar.restart();
			koniec_gry = true;
			nastepny_poziom = false;
		}
		Time time = zegar.getElapsedTime();
		if (time > seconds(2))
			nastepny_poziom = true;
	}
}
void CPoziom::rysuj(RenderWindow* window)
{
	list<CElementy_rysowalne*>::iterator ent_iter = lista_elementow_rysowalnych.begin();
	list<CElementy_rysowalne*>::iterator ent_iter_end = lista_elementow_rysowalnych.end();
	while (ent_iter != ent_iter_end)
	{
		(*ent_iter)->rysuj(window);		//Wyrysowywane s¹ wszystkie rysowalne obiekty.
		ent_iter++;
	}
	if (koniec_gry)					//Je¿eli jest to te¿ koniec poziomu, to rysowany jest te¿ odpowiedni tekst.
		window->draw(napis);
	window->draw(guzik);
}
bool CPoziom::idzDoMenu()
{
	if (wroc_do_menu)
	{
		wroc_do_menu = false;
		return true;
	}
	else
		return false;
}
bool CPoziom::nastepnyPoziom()
{
	return nastepny_poziom;
}


void CPoziom::WcisnietoKlawisz(Vector2i pozycja)
{
	Vector2f pozycja_myszki;					//Sprawdzane jest, czy wciśniety zstał przycisk powrotu do menu.
	pozycja_myszki.x = pozycja.x;
	pozycja_myszki.y = pozycja.y;
	FloatRect bounds = przycisk.getLocalBounds();
	Vector2f pozycja_przycisku = przycisk.getPosition();
	bounds.top = pozycja_przycisku.y;
	bounds.left = pozycja_przycisku.x;
	if (bounds.contains(pozycja_myszki))
		wroc_do_menu = true;
}

bool CPoziom::przeciwnikOsiagnalPostac(CPostac* wsk_postaci, CPrzeciwnik* wsk_przeciwnika)
{
	Vector2f vec1 = wsk_postaci->zwrocPozycje();	//Obliczane jest jakie pole zajmuj¹ przecinik i postaæ.
	Vector2f vec2 = wsk_przeciwnika->zwrocPozycje();
	int x1 = vec1.x / 40;
	int y1 = vec1.y / 40;
	int x2 = vec2.x / 40;
	int y2 = vec2.y / 40;
	if (x1 == x2 && y1 == y2)				//Je¿eli to samo, to zwracan jest prawda.
		return true;
	else
		return false;
}