#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED

#include <stdio.h>

///wskaznik pusty
#define nullptr ((void*)0)
///masksymalna dlugosc znakow dla imienia/nazwiska
#define DLUGOSC_NAZWY 20
///masksymalna dlugosc znakow dla miejscowowosci
#define MAX_MIASTO 30
///masksymalna dlugosc znakow dla nazwy ulicy
#define MAX_ULICA 30
///masksymalna dlugosc znakow dla numeru domu
#define MAX_DOM 4
///dlugosc kodu pocztowego
#define MAX_KOD 5
///masksymalna dlugosc znakow dla grupy
#define MAX_GRUPA 20
///maksymalna dlugosc znakow dla telefonu
#define DLUGOSC_TELEFONU 20
///masksymalna dlugosc znakow dla emaila
#define DLUGOSC_EMAIL 20
///umowna maksymalna liczba grup
#define ILOSC_GRUP_MAX 10

///struktura przechowujaca id,imie,nazwisko,adres,telefon,email oraz wskaznik na poprzedni oraz nastepny kontakt
struct Kontakt {
    struct Kontakt *prev;
	struct Kontakt *next;

	int id;
    char imie[DLUGOSC_NAZWY + 1];							// 1 na znak '\0'
    char nazwisko[DLUGOSC_NAZWY + 1];						// 1 na znak '\0'
    char kodPocztowy[MAX_KOD + 1];							// 1 na znak '\0'
    char miasto[MAX_MIASTO + 1];							// 1 na znak '\0'
    char ulica[MAX_ULICA + 1];								// 1 na znak '\0'
    char numerDomu[MAX_DOM + 1];							// 1 na znak '\0'
    int numerMieszkania;

    char **telefony;											// wskaznik do tablicy telefonow
    int iloscNumerow;											// ilosc numerow telefonow

    char **email;												// wskaznik do tablicy adresow e-mail
    int iloscEmail;												// ilosc adresow e-mail
};

///Struktura reprezentujaca pojedynczy wezel listy jednokierunkowej
///Lista kierunkowa jest z definicji posortowana po ID struktury element
struct Element
{
	struct Kontakt *element;								// wskaznik do Kontaktu
	struct Element *next;									// wskaznik do kolejnego elementu
};

///Struktura reprezentujaca grupe
struct Grupa
{
	char nazwaGrupy[MAX_GRUPA + 1];								// nazwa grupy
	struct Element *first;										// wskaznik do 1 elementu grupy, nullptr, gdy grupa pusta
};


///Funkcja czysci numery telefonow i zwalnia tablice wskaznikow do numerow telefonu
///@param wskaznik do rekordu Kontakt zawierajacego numery telefonow
void usunTelefony(struct Kontakt* kontakt);

///Funkcja dodaje telefon do tablicy numerow w kontakcie
///@param wskaznik do rekordu Kontakt zawierajacego numery telefonow
///@param dodawany numer
void dodajKolejnyNumer(struct Kontakt* kontakt, char numer[DLUGOSC_TELEFONU]);

///Funkcja czysci adresy email i zwalnia tablice wskaznikow do adresow e-mail
///@param wskaznik do rekordu Kontakt zawierajacego adresy e-mail
void usunEmaile(struct Kontakt* kontakt);

///Funkcja dodaje e-mail do tablicy e-mail w kontakcie
///@param wskaznik do rekordu Kontakt zawierajacego e-maile
///@param dodawany e-mail
void dodajKolejnyEmail(struct Kontakt* kontakt, char email[DLUGOSC_EMAIL]);

///Zwolnienie pamieci kontaktu
///@param wskaznik do kontaktu
void zwolnijKontakt(struct Kontakt *kontakt);


///Pomocnicza metoda z C
///@param
///@param wskaznik do kontaktu
///@param wskaznik do otwartego pliku
///@return
int getline2(char** lineptr, size_t* n, FILE* stream);


/**
 * Operacje na listach
*/

///funkcja zwracajaca ostatni element z listy
///@param wskaznik na pierwszy element struktury listy
///@return zwraca ostatni element struktury
struct Kontakt* getLast(struct Kontakt *head);

///funkcja wypisuje rekord bardziej opisowo, pomocnicze przy edycji
///@param wskaznik na rekord Kontakt
void wypiszRekordPionowo(struct Kontakt* st);

///Wypisanie rekordu w jednej linijce, gdy wypisujemy kilka rekordow, rekordy w grupie lub cala baze
///@param wskaznik na rekord Kontakt
void wypiszRekordPoziomo(struct Kontakt* st);


///funkcja wypisujaca wszystkie elementy w konsoli
///@param wkaznik na pierwszy element struktury listy
void wypiszListe(struct Kontakt *head);

///funkcja usuwajaca podany element z listy po ID
///@param wkaznik na pierwszy element struktury listy
///@param stala liczbowa przechowujaca wartosc liczby calkowitej ID
void usunZListyPoID(struct Kontakt *head, const int id);


///funkcja usuwajaca wszystkie elementy listy poza pierwszym elementem
///@param wkaznik na pierwszy element struktury listy
void zwolnijPamiec(struct Kontakt *head);


///funkcja inicjujaca pierwszy element listy
///@param wkaznik na pierwszy element struktury listy
void inicjalizacjaListy(struct Kontakt *head);

///funkcja dodaje kazdy nowy element listy na koniec
///@param wkaznik na pierwszy element struktury listy
///@param stala liczbowa przechowujaca wartosc liczby calkowitej ID
///@param stala przechowujaca wartosc liczby calkowitej ID
///@param stala przechowujaca tablice znakow dla imienia
///@param stala przechowujaca tablice znakow dla nazwiska
///@param stala przechowujaca tablice znakow dla kodu pocztowego
///@param stala przechowujaca tablice znakow dla miejscowosci
///@param stala przechowujaca tablice znakow dla ulicy
///@param stala przechowujaca tablice znakow dla numeru domu
///@param liczba przechowujaca numer mieszkania
///@param wskaznik do tablicy przechowujacej wskazniki do numerow telefonu
///@param liczba numerow telefonu
///@param wskaznik do tablicy przechowujacej wskazniki do adresow e-mail
///@param liczba numerow e-mail
void dodajNaKoniecListy(struct Kontakt *head, const int id, const char imie[], const char nazwisko[], const char kodPocztowy[],
						const char miasto[], const char ulica[], const char numerDomu[], int numerMieszkania, char **telefony, int iloscTel, char **email, int iloscEmail);


/**
 * Operacje na plikach
*/

///funkcja pomocnicza czyszacza podany plik
///@param zmienna tablicowa przechowujaca nazwe pliku
void wyczyscPlik(const char nazwaPliku[]);

///Zapis elementow tablicy rozdzielonych znakiem ";" lub zapis "-" gdy tablica pusta
///@param fd - wskaznik do otwartego pliku
///@param tab - tablica lancuchow
///@param iloscElementow - ilosc elementow tablicy
void zapiszTablice(FILE *fd, char **tab, int iloscElementow);

///funkcja przenosi zawartosc listy do podanego pliku
///@param wkaznik na pierwszy element struktury listy
///@param zmienna tablicowa przechowujaca nazwe pliku
void listaDoPliku(struct Kontakt *head, const char nazwaPliku[]);

///funkcja tworzy z lancucha tablice lancuchow podzielonych wzgledem znaku spearatora
///@param wskaznik na tekst do podzialu
///@param separator - znak podzialu
///@param ilosc wydzielonych podlancuchow
char **podzial(char* tekst, char separator, int* ilosc);

///funkcja pozwalajaca przeniesc zawartosc podanego pliku do listy
///@param wkaznik na pierwszy element struktury listy
///@param zmienna tablicowa przechowujaca nazwe pliku
///@param pomocnicza zmienna calkowita ktora dla wartosci rownej 1 zwalnia pamiec
void plikDoListy(struct Kontakt *head, const char nazwaPliku[], int czyZwolnic);

///funkcja wypisuje zawartosc pliku do konsoli
///@param zmienna tablicowa przechowujaca nazwe pliku
void wypiszZPliku(const char nazwaPliku[]);

///funkcja pozwalajaca usunac podany kontakt po ID
///@param zmienna calkowita przechowujaca ID
///@param zmienna tablicowa przechowujaca nazwe pliku
void usunZPlikuPoId(int id, const char nazwaPliku[]);

/// funkcja sprawdzajaca czy znak moze byc znakiem adresu dla funkcji wczytajSekwencje
/// imie i nazwisko to litery, oraz spacja, 2 imiona
///@param kod ascii sprawdzanego znaku
///@return prawda gdy spelnia warunki; falsz, gdy nie spelnia
int znakiImieniaNazwiska(int ascii);

///funkcja sprawdzajaca czy znak moze byc znakiem adresu dla funkcji wczytajSekwencje
///adres moze miec dowolne znaki
///@param kod ascii sprawdzanego znaku
///@return prawda gdy spelnia warunki; falsz, gdy nie spelnia
int znakiAdresu(int ascii);

///funkcja sprawdzajaca czy znak moze byc znakiem numeru telefonu dla funkcji wczytajSekwencje
///telefon zawiera cyfry i myslniki
///@param kod ascii sprawdzanego znaku
///@return prawda gdy spelnia warunki; falsz, gdy nie spelnia
int znakiTelefonu(int ascii);

///funkcja sprawdzajaca czy znak moze byc znakiem email dla funkcji wczytajSekwencje
///email zawiera litery, cyfry, @, kropki
///@param kod ascii sprawdzanego znaku
///@return prawda gdy spelnia warunki; falsz, gdy nie spelnia
int znakiEmail(int ascii);

///Funkcja wczytujaca do tablicy wyraz ciagu o dlugosci [minZnakow, maxZnakow]
///@param zmienna tablicowa przechowujaca wyraz
///@param zmienna calkowita przechowujaca minimalna liczbe znakow, inna dla login i dla hasla
///@param zmienna calkowita przechowujaca maksymalna liczbe znakow
///@param wskaYnik na stala znakowa
///@param wskaznik do funkcji sprawdzjacej znak sekwencji
void wczytajSekwencje(char wyraz[], int minZnakow, int maxZnakow, const char* coWczytujemy, int (*check)(int));

///Funkcja dodaje login i haslo do pliku o nazwie loginy
///@param nazwa pliku przechowujacego pary login i haslo
void rejestruj(const char* loginy);

///Funkcja logowania do bazy danych
///@param nazwa pliku przechowujacego pary login i haslo
///@return -1, gdy nie da sie otworzyć pliku @param, 1, gdy zalodowano, 0 gdy nie zalogowano
int zaloguj(const char* loginy);

///Sortowanie listy dwukierunkowej, metoda przez wybieranie
///@param headPtr - wskaznik do wskaznika na poczatek listy
void sortujPoID(struct Kontakt **headPtr);

///szukanie rekordu po unikalnym numerze id,
///@param wskaznik na element, od ktorego szukamy
///@param unikalny numer ID rekordu Kontakt
///@return adres znalezionego elementu lub nullptr, gdy nie znaleziono
struct Kontakt* znajdz(struct Kontakt* tmp, int id);

///wczytuje liczbe z zakresu [0, max]
///@param lancuch znakow podpowiadajacy, co uzytkownik powinien wpisac
///@param maksymalna prawidlowa wartosc,
///@return wartosc calkowita z przedzialu [0, max]
int wczytajLiczbe(const char *komunikat, int max);

///Funkcja zmienia adres poprzez opcjonalna modyfikacje danych adresowych
///@param zmianiany rekord
///@param pomocniczy bufor z funkcji nadrzednej edytuj
void edycjaAdresu(struct Kontakt* st, char bufor[MAX_MIASTO + 1]);

///Funkcja pyta uzytkownika ile numerow telefonu pobrac od uzytkownika.
///Tworzy dynamiczna tablice wskaznikow do odczytanych numerow telefonow
///@param pomocnicza tablica na wczytywany numer
///@param wskaznik do liczby przechowywanych numerow, aktualizowanej w tej funkcji.
///@return wskaznik do gotowej dynamicznej tablicy wskaznikow do istniejacych numerow, lub nullptr, gdy nie wprowadzono numerow
char **wczytajTelefony(char tmpTelefon[DLUGOSC_TELEFONU + 1], int *ileNumerow);

///Funkcja wybiera numer telefonu z listy numerow w kontakcie
///@param wskaznik do kontaktu zawierajacego numery
///@return -1, gdy brak numerow; 0, gdy anulowano wybor, 1 lub wiecej - wybrany numer
int wybierzTelefon(struct Kontakt* kontakt);

///Funkcja zmienia numer telefonu wskazany przez uzytkownika
///@param wskaznik do Kontaktu, w ktorym zmieniamy numer telefonu
///@param pomocnicza tablica do wczytywania nowego numeru telefonu na miejsce starego
void zmienNumerTelefonu(struct Kontakt* kontakt, char tmpTelefon[DLUGOSC_TELEFONU + 1]);

///Funkcja usuwa numer telefonu z dynamicznej tablicy wskaznikow
///@param wskaznik do kontaktu w ktorym usuwamy telefon
void usunNumerTelefonu(struct Kontakt* kontakt);

///Funkcja sprawdza, czy kontakt zawiera numer telefonu
///@param przeszukiwany kontakt
///@param sprawdzany numer telefony w kontakcie
///@return 0 - nie zawiera numeru, 1 - zawiera numer
int czyZawieraTelefon(struct Kontakt* kontakt, char telefonTmp[DLUGOSC_TELEFONU + 1]);

///Funkcja sprawdza, czy kontakt zawiera e-mail
///@param przeszukiwany kontakt
///@param sprawdzany e-mail w kontakcie
///@return 0 - nie zawiera e-mail, 1 - zawiera e-mail
int czyZawieraEmail(struct Kontakt* kontakt, char emailTmp[DLUGOSC_EMAIL + 1]);

///Funkcja wybiera e-mail z listy e-mail w kontakcie
///@param wskaznik do kontaktu zawierajacego e-mail
///@return -1, gdy brak e-mail; 0, gdy anulowano wybor, 1 lub wiecej - wybrany e-mail
int wybierzEmail(struct Kontakt* kontakt);

///Funkcja zmienia e-mail wskazany przez uzytkownika
///@param wskaznik do Kontaktu, w ktorym zmieniamy e-mail
///@param pomocnicza tablica do wczytywania nowego e-mail na miejsce starego
void zmienEmail(struct Kontakt* kontakt, char tmpEmail[DLUGOSC_EMAIL + 1]);

///Funkcja usuwa e-mail telefonu z dynamicznej tablicy wskaznikow
///@param wskaznik do kontaktu w ktorym usuwamy e-mail
void usunEmail(struct Kontakt* kontakt);

///Funkcja pyta uzytkownika ile adresow e-mail pobrac od uzytkownika.
///Tworzy dynamiczna tablice wskaznikow do odczytanych adresow e-mail
///@param pomocnicza tablica na wczytywany e-mail
///@param wskaznik do liczby przechowywanych e-mail, aktualizowanej w tej funkcji.
///@return wskaznik do gotowej dynamicznej tablicy wskaznikow do istniejacych e-mail, lub nullptr, gdy nie wprowadzono numerow
char **wczytajEmaile(char tmpEmail[DLUGOSC_EMAIL + 1], int *ileMail);

///Zmiana numerow telefonu
///@param zmianiany rekord
///@param pomocniczy bufor z funkcji nadrzednej edytuj
void edycjaNumeruTelefonu(struct Kontakt *k, char bufor[MAX_MIASTO + 1]);

///Zmiana adresow e-mail
///@param zmianiany rekord
///@param pomocniczy bufor z funkcji nadrzednej edytuj
void edycjaEmail(struct Kontakt *k, char bufor[MAX_MIASTO + 1]);

///Edycja rekordu kontakt
///@param wskaznik do pierwszego elementu listy kontaktow
void edytuj(struct Kontakt *head);

///zamiana duzych liter na male
///@param wskaznik do tablicy zawierajacej lanuch znakow
void zamienNaMaleLitery(char *bufor);

///Funkcja przeszukujaca baze kontaktow po wybranym kryterium przez uzytkownika, wyswietla rekordy spelniajace zadanie
///@param wskaznik na poczatek listy kontaktow
void wyszukaj(struct Kontakt *head);

/**
 * Operacje na grupach
*/

///Funkcja usuwa grupe, ktora jest lista.
///Najpierw zwalniamy wezly listy, na koncu strukture grupy.
///@param wskaznik do usuwanej grupy
void skasujGrupe(struct Grupa *g);

///Funkcja pobiera od uzytkownika nazwe grupy skladajaca sie z liter
///Tworzy strukture grupy bez wezlow przechowyjacych adresy Kontaktow
///@param tablica wskaznikow do grup
///@param wskaznik do aktualnej liczby grup, aktualizowany w tej funkcji o 1
///Grupa nie jest tworzona, gdy osiagnieto limit
void utworzGrupe(struct Grupa* grupy[ILOSC_GRUP_MAX], int *iloscGrup);

///Funkcja usuwa grupe o zadanym indeksie. Usuwa grupe z pamieci, a nastepnie usuwa ja z tablicy grup
///@param tablica wskaznikow do grup
///@param wskaznik do aktualnej ilosci grup, dekrementowany, przy usuwaniu
///@param indeks usuwanej grupy z tablicy
void usunGrupe(struct Grupa* grupy[ILOSC_GRUP_MAX], int *iloscGrup, int index);

///Usuniecie wszystkich grup
///@param tablica wskaznikow do grup
///@param wskaznik do aktualnej ilosci grup, ustawianej na 0, po zakonczeniu usuwania
void skasujGrupy(struct Grupa* grupy[ILOSC_GRUP_MAX], int *iloscGrup);

///Pomicnicza funkcja umozliwiajaca identyfikacje grupy poprzez wybor numeru
///@param tablica wskaznikow do grup
///@param aktualna ilosci grup
///@return 0, gdy anulowano wybor, od 1 - wybrany numer grupy, bedacy indeksem w tablicy powiekszonym o 1
int wyborGrupy(struct Grupa* grupy[ILOSC_GRUP_MAX], int iloscGrup);

///Wyswietlenie wszystkich kontaktow wchodzacych w sklad grupy
///@param wskaznik do wyswietlanej grupy
void wyswietlGrupe(struct Grupa* grupa);

///Funkcja dodaje wezel Element zawierajacy wskaznik do kontaktu, do listy w grupie
///Tworzona jest lista jednokierunkowa z definicji posortowana o zadanym ID zawartym w strukturze Kontakt
///@param wskaznik do grupy, do ktorej dodajemy wezel Element skojarzony z kontaktem
///@param wskaznik do Kontaktu, ktory dodajemy do tej grupy
int dodajDoGrupy(struct Grupa* grupa, struct Kontakt* kontaktDodawany);

///Funkcja usuwa kontakt grupy poprzez usuniecie wezla Element z listy jednokierunkowej grupy
///Wyszukuje wezel Element majacy wskaznik do struktury Kontakt o zadanym ID
///@param wskaznik do grupy, z ktorej usuwamy element o podanym ID
///@param id usuwanego wezla Element
///@return 1 gdy usunieto, 0, gdy nie usunieto
int usunZGrupy(struct Grupa* grupa, int id);

///Funkcja usuwa ze wszystkich grup element o zadanym ID
///Funkcja wywolywana, gdy usuwamy s listy element o podanym ID
///@param tablica wskaznikow do grup
///@param aktualna ilosc grup
///@param id usuwanego elementu z grupy
void usunZeWszystkichGrupID(struct Grupa* grupy[ILOSC_GRUP_MAX], int iloscGrup, int id);

///Funkcja zapisuje do pliku, wszystkie grupy
///Kazda linijka w pliku stanowi nazwe grupy wraz z numerami ID elementow grupy
///@param nazwa pliku grupy
///@param tablica wskkaznikow do istniejacych grup
///@param aktualne ilosc istniejscych grup
///@return -1, gdy nie otwarto pliku; 0, gdy grupy zapisano do pliku
int zapiszGrupyDoPliku(const char* grupyPlik, struct Grupa* grupy[ILOSC_GRUP_MAX], int iloscGrup);

///Funkcja tworzy dynamiczne listy grup zapisane w tablicy wskaznikow na podstawie wczytanej juz wczesniej listy Kontaktow z pliku
///@param nazwa pliku z grupami
///@param tablica wskaznikow do tworzonych grup
///@param wskaznik do aktualizowanej liczby grup
///@param wskaznik do listy kontaktow w ktorej szukamy adresow kontaktow skojarzonych z ID z pliku grup
///@return -1, gdy nie mozna otworzyc pliku z zapisanymi grupami; 0, gdy udalo sie odczytac plik z grupami
int wczytajGrupeZpliku(const char* grupyPlik, struct Grupa* grupy[ILOSC_GRUP_MAX], int *iloscGrup, struct Kontakt *head);


#endif // FUNKCJE_H_INCLUDED
