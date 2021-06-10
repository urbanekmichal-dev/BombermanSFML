#include "CGra.h"

CGra::CGra()
{
	//dodanie 4 czasomierzy do wektora i 4 zegarów
	for (int i = 0; i < 4; i++)
	{
		Clock clock;
		clock_vec.push_back(clock);
		clock_vec[i].restart();
		Time time;
		time_vec.push_back(time);
	}
}

void CGra::Start()
{
	SoundBuffer buffer;
	if (!buffer.loadFromFile(SCIEZKA_PLIKU_MUZYCZNEGO_MENU))
		cout << "Nie znaleziono pliku dzwiekowego";
	muzyka_menu.setBuffer(buffer);
	muzyka_menu.setLoop(true);
	muzyka_menu.play();
	RenderWindow window(sf::VideoMode(ROZMIAR_OKNA_GRY_SZEROKOSC, ROZMIAR_OKNA_GRY_WYSOKOSC), NAZWA_GRY);
	//window.setVerticalSyncEnabled(true);
	w_ptr = &window;
	try
	{
		wsk_menu = new CMenu;
	}
	catch (const char* wiad_bledu)
	{
		wiadomosc_bledu = wiad_bledu;				//Zlapanie wyjatku i przejscie do odpowiedniej funckji.
		WystapilBlad(w_ptr);
		return;
	}

	list<CPoziom*>::iterator iterator_poziomow;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			else if (!tryb_menu)
			{
				if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left))
				{
					Vector2i mouse_position = Mouse::getPosition(window);		//Odczyt położenia myszy, dla poziomu.
					(*iterator_poziomow)->WcisnietoKlawisz(mouse_position);
				}
			}
			else
			{
				if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left))
				{
					Vector2i mouse_position = Mouse::getPosition(window);
					wsk_menu->WcisnietoKlawisz(mouse_position);						//Odczyt położenia myszy dla menu.
				}
				else if (event.type == Event::MouseButtonReleased)
				{
					wsk_menu->ZwolnionoKlawisz();
				}
				
			}
		}
		if (tryb_menu && wsk_menu->wyjscie())
			if (!StorzPoziomy(iterator_poziomow))	//Wywołanie funkcji tworzącej poziomy.
			{
				WystapilBlad(w_ptr);			//Złapanie wyjątków i przejście do odpowiedniej funckji.
				return;
			}
		window.clear();
		if (tryb_menu)
			wsk_menu->rysuj(w_ptr);			//Wywołanie funkcji rysującej menu wyboru.
		else
		{
			ZarzadzajPoziomami(iterator_poziomow, w_ptr);	//Wywoływanie funkcji zarządzającej poziomem.
			muzyka_menu.stop();
		}
		
		
		
		window.display();
	}
}

void CGra::WystapilBlad(RenderWindow* w_ptr)
{
	Clock clock;
	Font font;
	Text text;
	font.loadFromFile(SCIEZKA_CZCIONKI_ARCADE);			//Załadowanie odpowiedniej czcionki.
	text.setFont(font);							//Ustawienie odpowiednich parametrów dla wyświetlanego napisu.
	text.setString(wiadomosc_bledu);
	text.setFillColor(Color::Red);
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(3);
	text.setCharacterSize(40);
	text.setPosition(50, 300);
	clock.restart();
	Time time = clock.getElapsedTime();

	while (time < seconds(2))
	{
		w_ptr->clear();
		w_ptr->draw(text);						//Wyświetlanie napisu.
		w_ptr->display();
		time = clock.getElapsedTime();
	}
}

void CGra::CzytajKlawiature(CPoziom* wsk_poziom)
{
	for (int i = 0; i < 4; i++)								//Zadaniem funckji jest odczyt przycisków z klawiatury, dla różnych postaci.
		time_vec[i] = clock_vec[i].getElapsedTime();
	if (Keyboard::isKeyPressed(Keyboard::Left))
		wsk_poziom->ruch(1, kierunek::lewo);
	else if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		wsk_poziom->ruch(1, kierunek::prawo);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Up))
		wsk_poziom->ruch(1, kierunek::gora);
	else if (Keyboard::isKeyPressed(Keyboard::Down))
		wsk_poziom->ruch(1, kierunek::dol);
	if (Keyboard::isKeyPressed(Keyboard::Space) && time_vec[0] > milliseconds(150))		//W przypadku odczytu przycisków odpowiedzialnych za podkładanie bomb,
	{																					//wprowadzane jest opóŸnienie czasowe, które powoduje, 
		wsk_poziom->podlozBombe(1);														//że przy jednym wciśniêciu przycisku, nie nastąpi wielokrotne podłożenie bomby.
		clock_vec[0].restart();
	}
	if (liczba_graczy > 1)
	{
		if (Keyboard::isKeyPressed(Keyboard::A))
			wsk_poziom->ruch(2, kierunek::lewo);
		else if (Keyboard::isKeyPressed(Keyboard::D))
			wsk_poziom->ruch(2, kierunek::prawo);
		else if (Keyboard::isKeyPressed(Keyboard::W))
			wsk_poziom->ruch(2, kierunek::gora);
		else if (Keyboard::isKeyPressed(Keyboard::S))
			wsk_poziom->ruch(2, kierunek::dol);
		if (Keyboard::isKeyPressed(Keyboard::Q) && time_vec[1] > milliseconds(150))
		{
			wsk_poziom->podlozBombe(2);
			clock_vec[1].restart();
		}
		if (liczba_graczy > 2)
		{
			if (Keyboard::isKeyPressed(Keyboard::J))
				wsk_poziom->ruch(3, kierunek::lewo);
			else if (Keyboard::isKeyPressed(Keyboard::L))
				wsk_poziom->ruch(3, kierunek::prawo);
			else if (Keyboard::isKeyPressed(Keyboard::I))
				wsk_poziom->ruch(3, kierunek::gora);
			else if (Keyboard::isKeyPressed(Keyboard::K))
				wsk_poziom->ruch(3, kierunek::dol);
			if (Keyboard::isKeyPressed(Keyboard::U) && time_vec[2] > milliseconds(150))
			{
				wsk_poziom->podlozBombe(3);
				clock_vec[2].restart();
			}
			if (liczba_graczy > 3)
			{
				if (Keyboard::isKeyPressed(Keyboard::V))
					wsk_poziom->ruch(4, kierunek::lewo);
				else if (Keyboard::isKeyPressed(Keyboard::N))
					wsk_poziom->ruch(4, kierunek::prawo);
				else if (Keyboard::isKeyPressed(Keyboard::G))
					wsk_poziom->ruch(4, kierunek::gora);
				else if (Keyboard::isKeyPressed(Keyboard::B))
					wsk_poziom->ruch(4, kierunek::dol);
				if (Keyboard::isKeyPressed(Keyboard::F) && time_vec[3] > milliseconds(150))
				{
					wsk_poziom->podlozBombe(4);
					clock_vec[3].restart();
				}
			}
		}
	}
}

CGra::~CGra()
{
	delete wsk_menu;
	list<CPoziom*>::iterator poziom_iterator = lista_poziomow.begin();
	list<CPoziom*>::iterator poziom_iterator_koniec = lista_poziomow.end();
	while (poziom_iterator != poziom_iterator_koniec)
	{
		list<CPoziom*>::iterator czysc = poziom_iterator;
		delete(*czysc);
		poziom_iterator++;
	}
	lista_poziomow.clear();
}

void CGra::ZarzadzajPoziomami(list<CPoziom*>::iterator& level_iter, RenderWindow* ptr)
{
	CzytajKlawiature(*level_iter);				//Nastepuje wywoływanie funkcji poziomu,
	(*level_iter)->RuchPrzeciwnikow();			//Które zarządzają jego odpowiednimi aspektami.
	(*level_iter)->SprawdzBomby();
	(*level_iter)->SprawdzKolizje();
	(*level_iter)->SprawdzZycie();
	(*level_iter)->SprawdzCzyKoniecPoziomu();
	(*level_iter)->rysuj(ptr);
	if ((*level_iter)->idzDoMenu())			//Sprawdzenie, czy ma wystąpić powrót do menu głowengo.
		tryb_menu = true;
	if ((*level_iter)->nastepnyPoziom())			//Sprawdzenie, czy ma następie przejście do następnego poziomu.
	{										//Jeżeli tak, to usuwany jest aktualny poziom i następuje przejście do nastêpnego.
		list<CPoziom*>::iterator level_iter_end = lista_poziomow.end();
		delete* level_iter;
		level_iter = lista_poziomow.erase(level_iter);
		if (level_iter_end == level_iter)
			tryb_menu = true;
	}
	
}

bool CGra::StorzPoziomy(list<CPoziom*>::iterator& iterator_poziomow)
{
	tryb_menu = false;
	int temp_numer = wsk_menu->zwrocIloscGraczy();
	bool kontynuuj_tworzenie = false, rozpocznij_nowa_gre = false;
	if (!lista_poziomow.empty())
		rozpocznij_nowa_gre = (*iterator_poziomow)->rozpocznijNowaGre();
	if ((poziom_stworzony && temp_numer != liczba_graczy) || rozpocznij_nowa_gre)
	{
		iterator_poziomow = lista_poziomow.begin();								//W przypadku wybrania innego trybu w menu,
		list<CPoziom*>::iterator level_iter_end = lista_poziomow.end();		//usuwane są dotychczasowe poziomy, za co odpowiedzialna jest ta czêśæ kodu.
		while (iterator_poziomow != level_iter_end)
		{
			delete* iterator_poziomow;
			iterator_poziomow = lista_poziomow.erase(iterator_poziomow);
		}
		kontynuuj_tworzenie = true;
	}
	if (!poziom_stworzony || kontynuuj_tworzenie)
	{
		liczba_graczy = temp_numer;
		for (int i = 0; i < 3; i++)
		{													//Tworzone tutaj są nowe poziomy.
			char suffix[3];									//Suffix odpowiada danej wartości iteratora pêtli			
			_itoa_s(i + 1, suffix, 10);						//Jest on przekazywany do konstruktora poziomu, by plansza gry została pobrana z odpowiedniego pliku.
			CPoziom* wsk_poziom = new CPoziom(liczba_graczy, suffix);
			if (wsk_poziom->WystapilBlad(wiadomosc_bledu))		//Sprawdzane jest czy tworzenie poziomu powiodło siê.
				return false;								//Jeżeli wystąpił wyjątek, zwracana jest wartośæ false.
			lista_poziomow.push_back(wsk_poziom);
		}
		iterator_poziomow = lista_poziomow.begin();
		poziom_stworzony = true;
		numer_poziomu++;
		kontynuuj_tworzenie = false;
	}
	return true;
}