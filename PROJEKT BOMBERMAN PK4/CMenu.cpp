#include "Biblioteki.h"
#include "CMenu.h"

CMenu::CMenu()
{
	

	//Ustawienie i wczytanie tła menu
	t_tlo = new Texture;
	if (!t_tlo->loadFromFile(SCIEZKA_TLO_MENU))
		throw "Nie mozna wczytac tekstur tla menu!";
	s_tlo.setTexture(*t_tlo);
	s_tlo.setScale(1.0f, 1.0f);

	//Wczytanie 9 guzików menu, dodanie ich do sprajtów
	int polozenie_lewy_rog_guzika[9] = { 1899, 2,411,837,1132, 1427,1720, 2262, 2325 };
	int szerokosc_guzika[9] = { 364,407,423,293,293,293,178, 62, 61 };
	for (int i = 0; i < 9; i++)
	{ 
		//Tekstura
		Texture* tekstura_guzika_wsk = new Texture;
		if (!tekstura_guzika_wsk->loadFromFile(SCIEZKA_GUZIKI_MENU, IntRect(polozenie_lewy_rog_guzika[i], 1, szerokosc_guzika[i], 63)))
			throw "Nie mozna wczytac tekstur guzikow!";
		t_guziki.push_back(tekstura_guzika_wsk);
		//Sprajt
		Sprite* spr_guzika_wsk = new Sprite;
		spr_guzika_wsk->setTexture(*tekstura_guzika_wsk);
		spr_guzika_wsk->setScale(0.8, 0.8);
		guziki.push_back(spr_guzika_wsk);
	}
	//Wczytanie tekstur 4 postaci 
	for (int i = 0; i < 4; i++)
	{
		Texture* tekstura_postaci_wsk = new Texture;
		if (!tekstura_postaci_wsk->loadFromFile(SCIEZKA_POSTACI_GRY, IntRect(1, 1 + (i * 23), 21, 20)))
			throw "Nie mozna wczytac tekstur postaci z gry";
		vec_wsk_tex_graczy.push_back(tekstura_postaci_wsk);
	}
	//Wczytanie czcionki 
	if (!czcionka.loadFromFile(SCIEZKA_CZCIONKI_ARCADE))
		throw "Nie mozna wczytac pliku czcionki ttf";
	//Dodanie 5 ciagów napisów
	for (int i = 0; i < 5; i++)
	{
		Text* tekst_wsk = new Text;
		tekst_wsk->setFont(czcionka);
		tekst_wsk->setCharacterSize(40);
		tekst_wsk->setOutlineThickness(3);
		tekst_wsk->setOutlineColor(Color::Black);
		vec_wsk_tekst.push_back(tekst_wsk);
	}
	postac.setTexture(*vec_wsk_tex_graczy[0]);
	postac.setScale(2.5, 2.5);
	inicjalizacja();
}

void CMenu::inicjalizacja() //inicjalizacja napisów i położenia przycisków
{
	guziki[0]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC/2)-(guziki[0]->getGlobalBounds().width/2),
		(ROZMIAR_OKNA_GRY_WYSOKOSC/2)-(guziki[0]->getGlobalBounds().height/2));	//rozpocznij
	guziki[1]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC / 2) - (guziki[1]->getGlobalBounds().width / 2),
		(ROZMIAR_OKNA_GRY_WYSOKOSC / 2) - (guziki[1]->getGlobalBounds().height / 2)-35);	//jeden gracz	
	guziki[2]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC / 2) - (guziki[2]->getGlobalBounds().width / 2),
		(ROZMIAR_OKNA_GRY_WYSOKOSC / 2) - (guziki[2]->getGlobalBounds().height / 2)+35);	//wielu graczy
	guziki[3]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC / 2) - (guziki[3]->getGlobalBounds().width / 2)-300,
		(ROZMIAR_OKNA_GRY_WYSOKOSC / 2) - (guziki[3]->getGlobalBounds().height / 2)+110);	//2 graczy
	guziki[4]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC / 2) - (guziki[4]->getGlobalBounds().width / 2),
		(ROZMIAR_OKNA_GRY_WYSOKOSC / 2) - (guziki[4]->getGlobalBounds().height / 2)+110);	//3 graczy
	guziki[5]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC / 2) - (guziki[5]->getGlobalBounds().width / 2)+300,
		(ROZMIAR_OKNA_GRY_WYSOKOSC / 2) - (guziki[5]->getGlobalBounds().height / 2)+110);	//4 graczy
	guziki[6]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC / 2) - (guziki[6]->getGlobalBounds().width / 2),
		(ROZMIAR_OKNA_GRY_WYSOKOSC / 2) - (guziki[6]->getGlobalBounds().height / 2)+185);	//graj
	guziki[7]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC ) - (guziki[0]->getGlobalBounds().width / 2),
		(ROZMIAR_OKNA_GRY_WYSOKOSC ) - (guziki[0]->getGlobalBounds().height / 2)-100);	// pomoc
	guziki[8]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC)-(guziki[0]->getGlobalBounds().width / 2),
		(ROZMIAR_OKNA_GRY_WYSOKOSC)-(guziki[0]->getGlobalBounds().height / 2) - 100);	//menu 
	vec_wsk_tekst[0]->setString("Witaj w grze Bomberman!");
	vec_wsk_tekst[0]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC / 2) - (vec_wsk_tekst[0]->getGlobalBounds().width / 2),
		(ROZMIAR_OKNA_GRY_WYSOKOSC / 4) - (vec_wsk_tekst[0]->getGlobalBounds().height / 2));
	vec_wsk_tekst[1]->setString("Instrukcja");
	vec_wsk_tekst[1]->setPosition((ROZMIAR_OKNA_GRY_SZEROKOSC / 2) - (vec_wsk_tekst[1]->getGlobalBounds().width / 2), 20);
	vec_wsk_tekst[2]->setString("Sterowanie\nRuch:             Bomba:");
	vec_wsk_tekst[2]->setPosition(30, 100);
	vec_wsk_tekst[3]->setString("Strzalki     Spacja\nW  S  A  D     Q\nI  K  J   L      U\nG  B  V  N      F\n");
	vec_wsk_tekst[3]->setPosition(80, 190);
	string s_instrukcja = "W trybie gry dla jednego gracza  \n";
	s_instrukcja += "naszym zadaniem jest wysadzenie\n";
	s_instrukcja += "wszystkich przeciwnikow oraz\n";
	s_instrukcja += "znalezienie kluczas do nastepnego poziomu\n";
	s_instrukcja += "ukrytego w jednej ze skrzyni\n";
	s_instrukcja += "W trybie gry dla wielu graczy\n";
	s_instrukcja += "każdy gracz steruje swoja postacia,\n";
	s_instrukcja += "wybrana sekcja klawiszy\n";
	s_instrukcja += "Celem jest wyeliminowanie przeciwnikow!";
	vec_wsk_tekst[4]->setString(s_instrukcja);
	vec_wsk_tekst[4]->setScale(Vector2f(0.6f, 0.6f));
	vec_wsk_tekst[4]->setPosition(30, 370);
	rysuj_klawisze[0] = true;
}

void CMenu::WcisnietoKlawisz(Vector2i pozycja)
{
	Vector2f pozycja_myszki;
	pozycja_myszki.x = pozycja.x;
	pozycja_myszki.y = pozycja.y;
	for (int i = 0; i < 9; i++)
	{
		if (rysuj_klawisze[i])
		{
			FloatRect bounds = guziki[i]->getLocalBounds();
			Vector2f button_position = guziki[i]->getPosition();		//Pêtla sprawdzająca, czy dany przycisk został naciśniêty.
			bounds.top = button_position.y;
			bounds.left = button_position.x;
			if (bounds.contains(pozycja_myszki))
			{
				nr_wcisnietego_klawisza = i;
				guziki[nr_wcisnietego_klawisza]->move(0, 5);		//Instrukcja pozwalająca ukazanie animacji wciskania przycisku.
				zaden_klawisz_nie_wcisniety = false;
				break;
			}
		}
	}
	if (zaden_klawisz_nie_wcisniety)
		return;
	if (nr_wcisnietego_klawisza == 0 || nr_wcisnietego_klawisza == 8)			//Instrukcje warunkowe, w których nastêpuje ustawienie wyświetlanaia odpowiednich przycisków.
	{
		rysuj_klawisze[1] = true;
		rysuj_klawisze[2] = true;
		rysuj_klawisze[7] = true;
		rysuj_klawisze[0] = false;
		rysuj_klawisze[8] = false;
	}
	else if (nr_wcisnietego_klawisza == 1)
	{
		rysuj_klawisze[6] = true;
		rysuj_klawisze[3] = false;
		rysuj_klawisze[4] = false;
		rysuj_klawisze[5] = false;
		liczba_graczy_w_grze = 1;
	}
	else if (nr_wcisnietego_klawisza == 2)
	{
		rysuj_klawisze[6] = false;
		rysuj_klawisze[3] = true;
		rysuj_klawisze[4] = true;
		rysuj_klawisze[5] = true;
	}
	else if (nr_wcisnietego_klawisza > 2 && nr_wcisnietego_klawisza < 6)
	{
		rysuj_klawisze[6] = true;
		liczba_graczy_w_grze = nr_wcisnietego_klawisza - 1;
	}
	else if (nr_wcisnietego_klawisza == 6)
		zamknij_menu = true;
	else if (nr_wcisnietego_klawisza == 7)
	{
		for (int i = 1; i < 8; i++)
			rysuj_klawisze[i] = false;
		rysuj_klawisze[8] = true;
	}
}

void CMenu::ZwolnionoKlawisz()
{
	if (!zaden_klawisz_nie_wcisniety)
	{
		guziki[nr_wcisnietego_klawisza]->move(0, -5);		//Instrukcja pozwalająca ukazanie animacji wciskania przycisku.
		zaden_klawisz_nie_wcisniety = true;	//Po puszczeniu przycisku ustwania jest zmienna, mówiąca, że żaden przycisk nie jest wciśniêty.
	}
}

bool CMenu::wyjscie()
{
	if (zamknij_menu)
	{
		zamknij_menu = false;
		for (int i = 1; i < 9; i++)
			rysuj_klawisze[i] = false;			//Przy wyjściu z funkcji, układ wyświetlanych przycisków jest przywracany do stanu początkowego.
		rysuj_klawisze[0] = true;
		return true;
	}
	else return false;
}

void CMenu::rysuj(RenderWindow* wsk_okna)
{
	wsk_okna->draw(s_tlo);
	for (int i = 0; i < 9; i++)
	{
		if (rysuj_klawisze[i])						//Wyrysowane są odpowiednie przyciski.	
			wsk_okna->draw(*guziki[i]);
	}
	if (rysuj_klawisze[8])
	{
		for (int i = 0; i < 4; i++)
		{
			postac.setTexture(*vec_wsk_tex_graczy[i]);		//W przypadku, gdy wybrana jest opcja pomocy,
			postac.setPosition(30, 190 + 40 * i);	//wyświetla jest instrukcja, w postaci którkiego opisu oraz sterowania odpowiednimi postaciami.
			postac.setScale(Vector2f(2.2f, 2.2f));
			wsk_okna->draw(postac);
		}
		for (int i = 1; i < 5; i++)
			wsk_okna->draw(*vec_wsk_tekst[i]);
	}
	else
		wsk_okna->draw(*vec_wsk_tekst[0]);
}

CMenu::~CMenu()
{
	for (int i = 0; i < 5; i++)
		delete vec_wsk_tekst[i];
	vec_wsk_tekst.clear();
	for (int i = 0; i < 4; i++)
		delete vec_wsk_tex_graczy[i];
	vec_wsk_tex_graczy.clear();
	for (int i = 0; i < 9; i++)
	{
		delete t_guziki[i];
		delete guziki[i];
	}
	t_guziki.clear();
	guziki.clear();
}

int CMenu::zwrocIloscGraczy()
{
	return liczba_graczy_w_grze;
}