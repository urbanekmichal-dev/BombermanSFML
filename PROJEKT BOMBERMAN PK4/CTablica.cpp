#include "CTablica.h"

CTablica::CTablica()
{

}
CTablica::CTablica(string suffix)
{
	srand(time(0));
	for (int i = 0; i < 4; i++)
	{
		Texture* new_t = new Texture;
		if (!new_t->loadFromFile("Bomberman_sprite.png", IntRect(68 + 17 * i, 149, 16, 16)))			//£adowanie odpowiednich tekstur i sprawdzenie czy zakoñczone zosta³o powaodzeniem.	
			throw "Nie udalo sie wczytac tekstur muru i skrzynek";													//Je¿eli nie, to wyrzucany jest wyj¹tek.
		vec_wsk_tex_tla.push_back(new_t);
	}

	for (int i = 0; i < 7; i++)
	{
		Texture* new_t = new Texture;
		if (!new_t->loadFromFile("Bomberman_sprite.png", IntRect(68 + 17 * i, 115, 16, 16)))
			throw"Nie udalo sie wczytac tekstur bonusow";
		vec_wsk_tex_bonusy.push_back(new_t);
	}

	for (int i = 0; i < 6; i++)
	{
		Texture* new_t = new Texture;
		if (!new_t->loadFromFile("Bomberman_sprite.png", IntRect(65 + 16 * i, 288, 16, 16)))
			throw "Nie udalo się wczytac tekstur bomb";
		vec_wsk_tex_bomby.push_back(new_t);
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Texture* new_t = new Texture;
			if (!new_t->loadFromFile("Bomberman_sprite.png", IntRect(17 * j, 115 + 17 * i, 16, 16)))
				throw "Nie udalo sie wczytac animacji wybuchu bomby";
			vec_wsk_tex_bomby.push_back(new_t);
		}
	}
	tlo_gry.setScale(2.5f, 2.5f);			//Ustawienie odpowiednich wartoœci skali dla spritów.
	bomba.setScale(2.5f, 2.5f);
	bonus.setScale(2.0f, 2.0f);
	nazwa_pliku += suffix;						//Dodanie do nazwy pliku odpowiedniego przyrostka, przekazanego przez argument, 
	nazwa_pliku += ".txt";						//aby mo¿liwe by³o za³adowanie odpowiedniej planszy dla poziomu.
	ifstream file(nazwa_pliku);
	if (!file.fail())
	{
		for (int i = 0; i < rozmiar; i++)
			for (int j = 0; j < rozmiar; j++)
			{
				file >> tablica[j][i];			//Odczyt zapisanej w pliku planszy do tabeli.
				if (tablica[j][i] == 3)
					liczba_skrzyni++;
			}
	}
	else
		throw "Nie mozna wczytac planszy";
}


CTablica::~CTablica()
{
	for (int i = 0; i < 26; i++)
		delete vec_wsk_tex_bomby[i];
	for (int i = 0; i < 4; i++)
		delete vec_wsk_tex_tla[i];
	for (int i = 0; i < 7; i++)
		delete vec_wsk_tex_bonusy[i];
	vec_wsk_tex_bomby.clear();
	vec_wsk_tex_tla.clear();
	vec_wsk_tex_bonusy.clear();
}

int CTablica::zwrocPole(int x, int y)
{
	return tablica[x][y];
}

void CTablica::ustawPole(int x, int y, int wartosc)
{
	tablica[x][y] = wartosc;
}


void CTablica::rysuj(RenderWindow *w_ptr)
{

	for (int i = 0; i < rozmiar; i++)
		for (int j = 0; j < rozmiar; j++)
		{
			int x = 40 * i;
			int y = 40 * j;
			if (tablica[i][j] < 4)
			{												//Je¿eli wartoœc pola planszy jest mniejsza od 4, to jest to t³o.
				int index = tablica[i][j];					//Nastêpuje wyrysowanie odpowiedniego spritu.
				tlo_gry.setPosition(x, y);
				tlo_gry.setTexture(*vec_wsk_tex_tla[index]);
				w_ptr->draw(tlo_gry);
			}
			if (tablica[i][j] > 3 && tablica[i][j] < 10)
			{
				tlo_gry.setPosition(x, y);				//Je¿eli wartoœæ pola planszy jest mniejsza od 10 i wieksza od 3, to jest to power-up.
				tlo_gry.setTexture(*vec_wsk_tex_tla[2]);	//Nastêpuje wyrysowanie trawy, a nastêpnie na tym odpowiedniego spritu.
				w_ptr->draw(tlo_gry);
				int index = tablica[i][j] - 4;
				bonus.setPosition(x + 4, y + 4);
				bonus.setTexture(*vec_wsk_tex_bonusy[index]);
				w_ptr->draw(bonus);
			}
			else if (tablica[i][j] > 9)
			{												//Je¿eli wartoœæ pola planszy jest wiêksza od 9, to jest to bomba 
				tlo_gry.setPosition(x, y);				//Nastêpuje wyrysowanie trawy, a nastêpnie na tym odpowiedniego spritu.
				tlo_gry.setTexture(*vec_wsk_tex_tla[2]);
				w_ptr->draw(tlo_gry);
				int index_bomb = tablica[i][j] - 10;
				bomba.setPosition(x, y);
				bomba.setTexture(*vec_wsk_tex_bomby[index_bomb]);
				w_ptr->draw(bomba);
			}
		}
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 2; j++)
		{
			tlo_gry.setPosition(i * 40, 800 + j * 40);		//Wyrysowanie t³a, dla pól w których znajduj¹ siê aktualne poziomy ¿ycia postaci.
			tlo_gry.setTexture(*vec_wsk_tex_tla[2]);
			w_ptr->draw(tlo_gry);
		}

}

void CTablica::ustawTektureBomby(int index)
{
	bomba.setTexture(*vec_wsk_tex_bomby[index]);
}

void CTablica::wyczyscEkspolozje()
{
	for (int i = 1; i < rozmiar - 1; i++)
		for (int j = 1; j < rozmiar - 1; j++)
			if (tablica[i][j] > 10)
				tablica[i][j] = 2;
}

void CTablica::zniszSkrzynke(int x,int y)
{
	liczba_skrzyni--;						//Sprawdzana jest liczba skrzyñ na planszy,
	if (liczba_skrzyni)					//Poniewa¿, gdy osi¹gnie ona 0, to konieczne jest wyrysowanie drzwi do nastepnego poziomu.
	{
		int random = rand() % 12 + 4;		//Nastêpuje losowanie, odpowiedniego power-upu.
		if (random >= 9)					//Je¿eli jego numer jest wiêkszy od 9, to nie zosta³ wylosowany ¿aden power-up.
			tablica[x][y] = 2;
		else
			tablica[x][y] = random;
	}
	else tablica[x][y] = 9;
}