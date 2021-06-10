#include "CPostac.h"

CPostac::CPostac():_kierunek(dol),predkosc(0.6),
indeks_tekstury(0),podlozone_bomby(0),max_ilosc_bomb(1),
zycie(3),rozmiar_bomby(1),max_rozmiar_bomby(4),
max_zycia(5),max_predkosc(1.0)
{
	if (liczba_graczy == 4)
		liczba_graczy = 0;
	index_postaci = liczba_graczy;
	liczba_graczy++;
	for (int i = 0; i < 14; i++)
	{
		Texture* new_t = new Texture;
		if (!new_t->loadFromFile(SCIEZKA_POSTACI_GRY, IntRect(15 + 16 * i, 304 + (index_postaci % 4) * 17, 16, 16)))
			throw "Nie mozna wczytac pliku tekstur postaci!";
		vec_wsk_tex_graczy.push_back(new_t);
	}
	Texture* new_t = new Texture;
	if (!new_t->loadFromFile(SCIEZKA_POSTACI_GRY, IntRect(136, 149, 16, 16)))
		throw "Nie mozna wczytac pliku tekstur postaci!!";
	vec_wsk_tex_graczy.push_back(new_t);
	serce.setScale(2.5f, 2.5f);
	vector<Texture*>::iterator tex_iter = vec_wsk_tex_graczy.begin();
	sprite.setTexture(**tex_iter);
	sprite.setScale(2.5f, 2.5f);
	sprite.setPosition(40, 54);
	zlapany_przez_przeciwnika = Vector2f(40, 54);
	if (index_postaci == 0)
		sprite.setPosition(40, 40);
	else if (index_postaci == 1)
		sprite.setPosition(40, 720);
	else if (index_postaci == 2)
		sprite.setPosition(720, 720);
	else
		sprite.setPosition(720, 40);
	zegar.restart();
}

CPostac::~CPostac()
{
	if (liczba_graczy > 0)
		liczba_graczy--;
	for (int i = 13; i >= 0; i--)
	{
		delete vec_wsk_tex_graczy[i];
		vec_wsk_tex_graczy.pop_back();
	}
}

int CPostac::liczba_graczy = 0;

void CPostac::ustawTekstury()
{
	Time time = zegar.getElapsedTime();
	bool zmiana = false;
	if (_poprzedni_kierunek != _kierunek)
	{
		_poprzedni_kierunek = _kierunek;
		if (_kierunek == kierunek::lewo)					//Ustalenie indeksu startowego tekstury, zale¿nie od kierunku.
			indeks_tekstury = 13;
		else if (_kierunek == kierunek::prawo)
			indeks_tekstury = 3;
		else if (_kierunek == kierunek::gora)
			indeks_tekstury = 9;
		else
			indeks_tekstury = 0;
		zmiana = true;
	}
	else if (time > milliseconds(200))
	{
		if (_kierunek == kierunek::lewo)
		{															//W zale¿noœci od kierunku odpowiedniojest inkrementowany lub dekrementowany
			if (indeks_tekstury > 10 && indeks_tekstury <= 13)			//indeks tekstury, tak aby utworzona by³a animacja.
				indeks_tekstury--;
			else
				indeks_tekstury = 13;
		}
		else if (_kierunek == kierunek::prawo)
		{
			if (indeks_tekstury >= 3 && indeks_tekstury < 6)
				indeks_tekstury++;
			else
				indeks_tekstury = 3;
		}
		else if (_kierunek == kierunek::gora)
		{
			if (indeks_tekstury > 7 && indeks_tekstury <= 9)
				indeks_tekstury--;
			else
				indeks_tekstury = 9;
		}
		else
		{
			if (indeks_tekstury >= 0 && indeks_tekstury < 2)
				indeks_tekstury++;
			else
				indeks_tekstury = 0;
		}
		zegar.restart();
		zmiana = true;
	}
	if (zmiana)
		sprite.setTexture(*vec_wsk_tex_graczy[indeks_tekstury]);
}

Vector2f CPostac::zwrocPozycje()
{
	return sprite.getPosition();
}

bool CPostac::ruch(CTablica* tab_wsk, kierunek nowy_kierunek)
{
	bool unikaj_przeszkody = true;
	_kierunek = nowy_kierunek;
	ustawTekstury();
	Vector2f vector = sprite.getPosition();
	if (_kierunek == kierunek::lewo)			//Do aktualnego po³o¿enia zostaje dodana wartoœæ przemieszczenia o prêdkoœæ.
		vector.x -= (float)predkosc;
	else if (_kierunek == kierunek::prawo)
		vector.x += (float)predkosc;
	else if (_kierunek == kierunek::gora)
		vector.y -= (float)predkosc;
	else
		vector.y += (float)predkosc;
	int x = (int)vector.x / 40;
	int y = (int)vector.y / 40;
	if (tab_wsk->zwrocPole(x, y) == 2 || tab_wsk->zwrocPole(x, y) > 3)			//Sprawdzenie mo¿liwych kolizji z innymi obiektami na planszy
	{
		x = (int)(vector.x + 39) / 40;
		y = (int)vector.y / 40;
		if (tab_wsk->zwrocPole(x, y) == 2 || tab_wsk->zwrocPole(x, y) > 3)
		{
			x = (int)vector.x / 40;
			y = (int)(vector.y + 39) / 40;
			if (tab_wsk->zwrocPole(x, y) == 2 || tab_wsk->zwrocPole(x, y) > 3)
			{
				x = (int)(vector.x + 39) / 40;
				y = (int)(vector.y + 39) / 40;
				if (tab_wsk->zwrocPole(x, y) == 2 || tab_wsk->zwrocPole(x, y) > 3)
				{
					sprite.setPosition(vector);					//Je¿eli kolizja nie wystêpuje, to nastêpuje przemieszczenie postaci
					unikaj_przeszkody = false;
				}
			}
		}
	}
	x = (int)(vector.x + 20) / 40;
	y = (int)(vector.y + 20) / 40;
	if (tab_wsk->zwrocPole(x, y) < 10 && tab_wsk->zwrocPole(x, y) > 3)
		zbierzUlepszenie(tab_wsk, x, y);				//Przy wejœciu na power-up, zostaje on zebrany.
	if (unikaj_przeszkody)
		uniknijPrzeszkody(tab_wsk, vector);				//Je¿eli wyst¹pi³a kolizja, wywo³ywana jest funkcja sprawdzajaca czy mo¿liwe jest jesnak przesuniêcie siê.
	return unikaj_przeszkody;
}



void CPostac::zbierzUlepszenie(CTablica* tab_wsk, int x, int y)
{
	int index = tab_wsk->zwrocPole(x, y);
	if (index == 4)
		max_ilosc_bomb++;
	else if (index == 5)					//Dla odpowiedniego power-upu jedt zwiêkszana okreœlona wartoœæ.
		predkosc += 0.1;
	else if (index == 6 && rozmiar_bomby < max_rozmiar_bomby)
		rozmiar_bomby++;
	else if (index == 7 && zycie < 5)
		zycie++;
	else if (index == 8)
		zycie--;
	else if (index == 9)
		nastepny_poziom = true;
	tab_wsk->ustawPole(x, y, 2);
}

void CPostac::uniknijPrzeszkody(CTablica* tab_wsk, Vector2f vector)
{

	int x = (int)vector.x / 40;
	int y = (int)vector.y / 40;
	Vector2f centre;
	centre.x = (float)x * 40;
	centre.y = (float)y * 40;
	float measure, margin = 15.0;
	if (_kierunek == kierunek::lewo)
	{
		if (tab_wsk->zwrocPole(x - 1, y) == 2 || tab_wsk->zwrocPole(x - 1, y) > 3)
		{
			measure = (vector.y + 40) - (y + 1) * 40;
			if (measure > 0 && measure < margin)
			{
				centre.x += 40;
				sprite.setPosition(centre);
			}
		}
		if (tab_wsk->zwrocPole(x - 1, y + 1) == 2 || tab_wsk->zwrocPole(x - 1, y + 1) > 3)
		{
			measure = (y + 1) * 40 - vector.y;
			if (measure > 0 && measure < margin)
			{
				centre.y += 40;
				centre.x += 40;
				sprite.setPosition(centre);
			}
		}
	}
	if (_kierunek == kierunek::prawo)
	{
		if (tab_wsk->zwrocPole(x + 1, y) == 2 || tab_wsk->zwrocPole(x + 1, y) > 3)
		{
			measure = (vector.y - 40) - (y - 1) * 40;
			if (measure > 0 && measure < margin)
				sprite.setPosition(centre);
		}
		if (tab_wsk->zwrocPole(x + 1, y - 1) == 2 || tab_wsk->zwrocPole(x + 1, y - 1) > 3)
		{
			measure = (y + 1) * 40 - vector.y;
			if (measure > 0 && measure < margin)
			{
				centre.y += 40;
				sprite.setPosition(centre);
			}
		}
	}
	if (_kierunek == kierunek::gora)
	{
		if (tab_wsk->zwrocPole(x, y - 1) == 2 || tab_wsk->zwrocPole(x, y - 1) > 3)
		{
			measure = (vector.x + 40) - (x + 1) * 40;
			if (measure > 0 && measure < margin)
			{
				centre.y += 40;
				sprite.setPosition(centre);
			}
		}
		if (tab_wsk->zwrocPole(x + 1, y - 1) == 2 || tab_wsk->zwrocPole(x + 1, y - 1) > 3)
		{
			measure = (x + 1) * 40 - vector.x;
			if (measure > 0 && measure < margin)
			{
				centre.x += 40;
				centre.y += 40;
				sprite.setPosition(centre);
			}
		}
	}
	if (_kierunek == kierunek::dol)
	{
		if (tab_wsk->zwrocPole(x, y + 1) == 2 || tab_wsk->zwrocPole(x, y + 1) > 3)
		{
			measure = (vector.x + 40) - (x + 1) * 40;
			if (measure > 0 && measure < margin)
				sprite.setPosition(centre);
		}
		if (tab_wsk->zwrocPole(x + 1, y + 1) == 2 || tab_wsk->zwrocPole(x + 1, y + 1) > 3)
		{
			measure = (x + 1) * 40 - vector.x;
			if (measure > 0 && measure < margin)
			{
				centre.x += 40;
				sprite.setPosition(centre);
			}
		}
	}
}

void CPostac::rysuj(RenderWindow* w_wsk)
{
	serce.setTexture(*vec_wsk_tex_graczy[0]);			//W zale¿noœci od tego, który indeks ma dana postaæ
	if (index_postaci == 0)					//To w odpowiednim miejscu narysowana zostanie postaæ
		serce.setPosition(0, 800);				//oraz wartoœæ jej ¿ycia.
	if (index_postaci == 1)
		serce.setPosition(400, 800);
	if (index_postaci == 2)
		serce.setPosition(0, 840);
	if (index_postaci == 3)
		serce.setPosition(400, 840);
	w_wsk->draw(serce);
	serce.setTexture(*vec_wsk_tex_graczy[14]);
	for (int i = 0; i < zycie; i++)
	{
		serce.move(40, 0);
		w_wsk->draw(serce);					//Rysowanie zyæ w postaci serc.
	}
	w_wsk->draw(sprite);						//Rysowanie postaci.
}
void CPostac::zarzadzajBombami()
{
	for (int i = 0; i < podlozone_bomby; i++)		//Nastêpuje uaktulanienie stanu wszystkich bomb.
		bomby[i]->aktualizujTablice();
}
bool CPostac::czyZywy()
{
	if (zycie > 0)
		return true;
	else
		return false;
}
CBomba* CPostac::bombaEkspolodowala()
{

	bool exploded = false;
	for (int i = 0; i < podlozone_bomby; i++)			//Dla wszystkich bomb jest sprawdzane, czy dana bomba ju¿ eksplodowa³a.
	{
		if (bomby[i]->wysadzBombe())
			return bomby[i];
	}
	return nullptr;									//Zwracany jest wskaŸnik na dan¹ bombê lub nullptr.
}
void CPostac::zadajObrazenia(int obrazenia)
{
	zycie -= obrazenia;
}
void CPostac::WyczyscBombe(CBomba* wsk_bomby)
{
	delete wsk_bomby;
	bomby.pop_back();
	podlozone_bomby--;
}
bool CPostac::podlozBombe(CTablica* wsk_tab)
{
	if (podlozone_bomby < max_ilosc_bomb)
	{
		CBomba* bomb_ptr = new CBomba(wsk_tab, rozmiar_bomby);	//Funkcja jest odpowiedzialna za dodanie nowej bomby do listy bomb postaci 		
		bomby.push_front(bomb_ptr);							//oraz umieszczenie bomby na planszy, w miejscu, w którym aktualnie znajduje siê postaæ.
		Vector2f vector = sprite.getPosition();
		vector.x += 20;
		vector.y += 20;
		int x = (int)vector.x / 40;
		int y = (int)vector.y / 40;
		bomb_ptr->podlozBombe(x, y);
		podlozone_bomby++;
		return true;
	}
	return false;
}
bool CPostac::zlap()
{
	Vector2f vec1 = sprite.getPosition();
	int x1 = (int)vec1.x / 40;
	int y1 = (int)vec1.y / 40;
	int x2 = (int)zlapany_przez_przeciwnika.x / 40;
	int y2 = (int)zlapany_przez_przeciwnika.y / 40;
	if (x1 == x2 && y1 == y2)
		return false;
	else
	{
		zlapany_przez_przeciwnika = vec1;
		return true;
	}
}
int CPostac::zwrocIndeksPostaci()
{
	return index_postaci;
}
bool CPostac::idzDoNAstepnegoPoziomu()
{
	if (nastepny_poziom)
	{
		nastepny_poziom = false;
		return true;
	}
	else return false;
}
