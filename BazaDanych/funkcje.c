#include "funkcje.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>					/// funkcja isblank
#include <limits.h>					/// INT_MAX, INT_MIN

///maksymalna ilosc znakow dla loginu
#define MAX_LOGIN 20
///maksymalna ilosc znakow dla hasla
#define MAX_HASLO 30
///zdefiniowana wartosc true rowna 1
#define TRUE 1
///zdefiniowana wartosc false rowna 0
#define FALSE 0
///zdefiniowana ilosc prob przy probie logowania do bazy
#define PROB 3
///maksymalna liczba numerow telefonow i adresow e-mail umowna, ale wiecej nie potrzebujemy
#define MAX_EMAIL_TELEFON 5

///separator rekordu w pliku
const char separatorRekordu = '#';

///Funkcja czysci numery telefonow i zwalnia tablice wskaznikow do numerow telefonu
///@param wskaznik do rekordu Kontakt zawierajacego numery telefonow
void usunTelefony(struct Kontakt* kontakt)
{
	if(kontakt->telefony != nullptr)
	{
		for(int i = 0; i < kontakt->iloscNumerow; ++i)
			free(kontakt->telefony[i]);						// zwolnienie pojedynczego telefonu
		free(kontakt->telefony);							// zwolnienie tablicy wskaznikow na telefony
	}
	kontakt->telefony = nullptr;
	kontakt->iloscNumerow = 0;
}

///Funkcja dodaje telefon do tablicy numerow w kontakcie
///@param wskaznik do rekordu Kontakt zawierajacego numery telefonow
///@param dodawany numer
void dodajKolejnyNumer(struct Kontakt* kontakt, char numer[DLUGOSC_TELEFONU])
{
	if(kontakt->telefony == nullptr)
	{
		kontakt->iloscNumerow = 1;
		kontakt->telefony = (char**)malloc(sizeof(char*));
		kontakt->telefony[0] = (char*)malloc(strlen(numer) + 1);
		strcpy(kontakt->telefony[0], numer);
	}
	else
	{
		char **telTmp = (char**)malloc(sizeof(char*) * (kontakt->iloscNumerow + 1));
		for(int i = 0; i < kontakt->iloscNumerow; ++i)
			telTmp[i] = kontakt->telefony[i];
		telTmp[kontakt->iloscNumerow] = (char*)malloc(strlen(numer) + 1);
		strcpy(telTmp[kontakt->iloscNumerow], numer);
		++(kontakt->iloscNumerow);
		free(kontakt->telefony);
		kontakt->telefony = telTmp;
	}
}

///Funkcja czysci adresy email i zwalnia tablice wskaznikow do adresow e-mail
///@param wskaznik do rekordu Kontakt zawierajacego adresy e-mail
void usunEmaile(struct Kontakt* kontakt)
{
	if(kontakt->email != nullptr)
	{
		for(int i = 0; i < kontakt->iloscEmail; ++i)
			free(kontakt->email[i]);
		free(kontakt->email);
	}
	kontakt->email = nullptr;
	kontakt->iloscEmail = 0;
}


///Funkcja dodaje e-mail do tablicy e-mail w kontakcie
///@param wskaznik do rekordu Kontakt zawierajacego e-maile
///@param dodawany e-mail
void dodajKolejnyEmail(struct Kontakt* kontakt, char email[DLUGOSC_EMAIL])
{
	if(kontakt->email == nullptr)
	{
		kontakt->iloscEmail = 1;
		kontakt->email = (char**)malloc(sizeof(char*));
		kontakt->email[0] = (char*)malloc(strlen(email) + 1);
		strcpy(kontakt->email[0], email);
	}
	else
	{
		char **emailTmp = (char**)malloc(sizeof(char*) * (kontakt->iloscEmail + 1));
		for(int i = 0; i < kontakt->iloscEmail; ++i)
			emailTmp[i] = kontakt->email[i];
		emailTmp[kontakt->iloscEmail] = (char*)malloc(strlen(email) + 1);
		strcpy(emailTmp[kontakt->iloscEmail], email);
		++(kontakt->iloscEmail);
		free(kontakt->email);
		kontakt->email = emailTmp;
	}
}


///Zwolnienie pamieci kontaktu
///@param wskaznik do kontaktu
void zwolnijKontakt(struct Kontakt *kontakt)
{
	usunTelefony(kontakt);
	usunEmaile(kontakt);
	free(kontakt);
}

///Pomocnicza metoda z C
///@param
///@param wskaznik do kontaktu
///@param wskaznik do otwartego pliku
///@return
int getline2(char** lineptr, size_t* n, FILE* stream)
{
    static char line[256];
    char* ptr;
    unsigned int len;

    if (ferror(stream))
        return -1;

    if (feof(stream))
        return -1;

    fgets(line, 256, stream);

    ptr = strchr(line, '\n');
    if (ptr)
        *ptr = '\0';

    len = strlen(line);

    if ((len + 1) < 256)
    {
        ptr = realloc(*lineptr, 256);
        if (ptr == NULL)
            return(-1);
        *lineptr = ptr;
        *n = 256;
    }
    strcpy(*lineptr, line);
    return(len);
}


/**
 * Operacje na listach
*/

///Funkcja zwracajaca ostatni element z listy
///@param wskaznik na pierwszy element struktury listy
///@return zwraca ostatni element struktury
struct Kontakt* getLast(struct Kontakt *head) {
    struct Kontakt *last = head;
    while(last->next != nullptr) {
        last = last->next;
    }
    return last;
}

///Funkcja wypisuje rekord bardziej opisowo, pomocnicze przy edycji
///@param wskaznik na rekord Kontakt
void wypiszRekordPionowo(struct Kontakt* st)
{
	printf("%12s: %d:\n", "ID", st->id);
	printf("%12s: %s\n", "Imie", st->imie);
	printf("%12s: %s\n", "Nazwisko", st->nazwisko);

	printf("%12s: %c%c-%c%c%c\n", "Kod pocztowy", st->kodPocztowy[0],st->kodPocztowy[1],st->kodPocztowy[2], st->kodPocztowy[3], st->kodPocztowy[4]);
	printf("%12s: %s\n", "Miejscowosc", st->miasto);
	printf("%12s: %s\n", "Ulica", st->ulica);
	printf("%12s: %s\n", "Numer domu", st->numerDomu);
	if(st->numerMieszkania > 0)
		printf("%12s: %d\n", "Numer lokalu", st->numerMieszkania);


	if(st->iloscNumerow > 0)
	{
		printf("%12s:", "Telefony");
		for(int i = 0; i < st->iloscNumerow; ++i)
			printf(" %s", st->telefony[i]);
		putchar('\n');
	}
	else
		puts("Brak numerow telefonu");

	if(st->iloscEmail > 0)
	{
		printf("%12s:", "E-mail");
		for(int i = 0; i < st->iloscEmail; ++i)
			printf(" %s", st->email[i]);
		putchar('\n');
	}
	else
		puts("Brak adresow e-mail.");
}

///Wypisanie rekordu w jednej linijce, gdy wypisujemy kilka rekordow, rekordy w grupie lub cala baze
///@param wskaznik na rekord Kontakt
void wypiszRekordPoziomo(struct Kontakt* st)
{
	printf("%d\t", st->id);
	printf("%s\t", st->imie);
	printf("%s\t", st->nazwisko);

	printf("%c%c-%c%c%c, ", st->kodPocztowy[0],st->kodPocztowy[1],st->kodPocztowy[2], st->kodPocztowy[3], st->kodPocztowy[4]);
	printf("%s, ", st->miasto);
	printf("%s ", st->ulica);
	printf("%s", st->numerDomu);
	if(st->numerMieszkania > 0)
		printf("/%d\t", st->numerMieszkania);
		else
	putchar('\t');


	if(st->iloscNumerow > 0)
	{
		for(int i = 0; i < st->iloscNumerow; ++i)
			printf("%s ", st->telefony[i]);
		putchar('\t');
	}
	else
		printf("brak telefonu\t");

	if(st->iloscEmail > 0)
	{
		for(int i = 0; i < st->iloscEmail; ++i)
			printf("%s ", st->email[i]);
		putchar('\n');
	}
	else
		puts("brak email");
}


///Funkcja wypisujaca wszystkie elementy w konsoli
///@param wkaznik na pierwszy element struktury listy
void wypiszListe(struct Kontakt *head) {
	struct Kontakt *tmp = head;
	printf("\nKontakty z listy:\n");

	while(tmp->next != nullptr) {
        tmp = tmp->next;
        wypiszRekordPoziomo(tmp);
	}
}

///Funkcja usuwajaca podany element z listy po ID
///@param wkaznik na pierwszy element struktury listy
///@param stala liczbowa przechowujaca wartosc liczby calkowitej ID
void usunZListyPoID(struct Kontakt *head, const int id) {
    struct Kontakt *tmp = head;
    int flagaUsunieto = 0;

    while(tmp->next != nullptr) { // Sprawdzenie dla wszystkich elementow w liscie za wyjatkiem ostatniego
        if(tmp->id == id) {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
            zwolnijKontakt(tmp);
            flagaUsunieto = 1;
            printf("\nElement zostal usuniety!\n");
            break;
        }

        tmp = tmp->next;
    }

    if(tmp->id == id && flagaUsunieto == 0) { // Sprawdzenie dla ostatniego elementu w liscie
        tmp->prev->next = nullptr;
        zwolnijKontakt(tmp);
        printf("\nElement zostal usuniety!\n");
    }
}


///Funkcja usuwajaca wszystkie elementy listy poza pierwszym elementem
///@param wkaznik na pierwszy element struktury listy
void zwolnijPamiec(struct Kontakt *head) {
    struct Kontakt *tmp = getLast(head);
    struct Kontakt *last;

    while(tmp != head) { // Od konca listy, do pierwszego elementu listy
        last = tmp;
        tmp = tmp->prev;
        tmp->next = nullptr;
        zwolnijKontakt(last);
    }
}


///Funkcja inicjujaca pierwszy element listy
///@param wkaznik na pierwszy element struktury listy
void inicjalizacjaListy(struct Kontakt *head) {
	head->prev = nullptr;
	head->next = nullptr;
	head->id = -1;
	head->imie[0] = '\0';
	head->nazwisko[0] = '\0';

	head->kodPocztowy[0] = -1;
    head->miasto[0] = '\0';
    head->ulica[0] = '\0';
    head->numerDomu[0] = '\0';
    head->numerMieszkania = -1;


	head->telefony = nullptr;				// brak telefonow
	head->iloscNumerow = 0;

	head->email = nullptr;					// brak email
	head->iloscEmail = 0;
}


///Funkcja dodaje kazdy nowy element listy na koniec
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
						const char miasto[], const char ulica[], const char numerDomu[], int numerMieszkania, char **telefony, int iloscTel, char **email, int iloscEmail)
{
	struct Kontakt *last = getLast(head);
	struct Kontakt *nowyEl = malloc(sizeof(struct Kontakt));

	nowyEl->prev = last;
    nowyEl->next = nullptr;
	strcpy(nowyEl->imie, imie);
	strcpy(nowyEl->nazwisko, nazwisko);
	strcpy(nowyEl->kodPocztowy, kodPocztowy);
	strcpy(nowyEl->miasto, miasto);
	strcpy(nowyEl->ulica, ulica);
	strcpy(nowyEl->numerDomu, numerDomu);
	nowyEl->numerMieszkania = numerMieszkania;

	nowyEl->iloscNumerow = iloscTel;
	nowyEl->telefony = telefony;

	nowyEl->iloscEmail = iloscEmail;
	nowyEl->email = email;


	nowyEl->id = id;

	last->next = nowyEl;
}

/**
 * Operacje na plikach
*/

///Funkcja pomocnicza czyszacza podany plik
///@param zmienna tablicowa przechowujaca nazwe pliku
void wyczyscPlik(const char nazwaPliku[]) {
    FILE *fptr;
    if ((fptr = fopen(nazwaPliku,"w")) == nullptr) { //Blad
       printf("Nie mozna otworzyc pliku!\n");
       exit(1);
    }
    fclose(fptr);
}

///Zapis elementow tablicy rozdzielonych znakiem ";" lub zapis "-" gdy tablica pusta
///@param fd - wskaznik do otwartego pliku
///@param tab - tablica lancuchow
///@param iloscElementow - ilosc elementow tablicy
void zapiszTablice(FILE *fd, char **tab, int iloscElementow)
{
	if(iloscElementow > 0)
	{
		int i;
		for(i = 0; i < iloscElementow - 1; ++i)
			fprintf(fd, "%s;", tab[i]);
		fprintf(fd, "%s", tab[i]);
	}
	else
		fputc('-', fd);
}



///funkcja przenosi zawartosc listy do podanego pliku
///@param wkaznik na pierwszy element struktury listy
///@param zmienna tablicowa przechowujaca nazwe pliku
void listaDoPliku(struct Kontakt *head, const char nazwaPliku[])
{
    FILE *fptr;
    if ((fptr = fopen(nazwaPliku,"w")) == nullptr) { //Blad
       printf("Nie mozna otworzyc pliku!\n");
       exit(1);
    }

    struct Kontakt *tmp = head;
    while(tmp->next != nullptr)
	{ //Zapisuje po kolei wszystkie elementy
        tmp = tmp->next;
        fprintf(fptr, "%d#%s#%s#%s#%s#%s#%s#%d#", tmp->id, tmp->imie, tmp->nazwisko, tmp->kodPocztowy, tmp->miasto, tmp->ulica, tmp->numerDomu, tmp->numerMieszkania);
		zapiszTablice(fptr, tmp->telefony, tmp->iloscNumerow);		// zapis telefonow odzielonych spacja
		fputc(separatorRekordu, fptr);
		zapiszTablice(fptr, tmp->email, tmp->iloscEmail);		// zapis adresow email oddzielonych spacja

        if(tmp->next != nullptr)
            fprintf(fptr, "\n");
    }

    printf("Baza danych zostala zaktualizowana!\n");
	fclose(fptr);
}

///Funkcja tworzy z lancucha tablice lancuchow podzielonych wzgledem znaku spearatora
///@param wskaznik na tekst do podzialu
///@param separator - znak podzialu
///@param ilosc wydzielonych podlancuchow
char **podzial(char* tekst, char separator, int* ilosc)
{
	*ilosc = 0;
	if(strcmp(tekst, "-") == 0)
		return nullptr;				// pusty lancuch


	//zliczenie ilosci wystapien znaku separator, zeby okreslic ilosc elementow tablicy wskaznikow
	char *wystapienie = strchr(tekst, separator);
	++(*ilosc);
	while(wystapienie != nullptr)
	{
		++wystapienie;
		wystapienie = strchr(wystapienie, separator);
		++(*ilosc);

	}
	//separator w postaci lancuchowej w tablicy
	char delim[2] = {separator, '\0'};

	char **tablica = (char**)malloc(sizeof(char*) * (*ilosc));	//utworzenie tablicy dynamicznej

	//skopiowanie podzielonego lancucha tekst na lancuchy w tablicy
	char *ptr = strtok(tekst, delim);
	tablica[0] = (char*)malloc(strlen(ptr) + 1);
	strcpy(tablica[0], ptr);

	for(int i = 1; i < *ilosc; ++i)
	{
		char *ptr = strtok(nullptr, delim);
		tablica[i] = (char*)malloc(strlen(ptr) + 1);
		strcpy(tablica[i], ptr);
	}
	return tablica;
}


///Funkcja pozwalajaca przeniesc zawartosc podanego pliku do listy
///@param wkaznik na pierwszy element struktury listy
///@param zmienna tablicowa przechowujaca nazwe pliku
///@param pomocnicza zmienna calkowita ktora dla wartosci rownej 1 zwalnia pamiec
void plikDoListy(struct Kontakt *head, const char nazwaPliku[], int czyZwolnic)
{
    if(czyZwolnic == 1)
        zwolnijPamiec(head);


    const char delim[] = {separatorRekordu, '\0'};										// # jest separatorem
    char *line = NULL;
    size_t len = 0;
    FILE *fptr;
    if ((fptr = fopen(nazwaPliku,"r")) == nullptr) {
       printf("Nie mozna otworzyc pliku!\n");
       exit(1);
    }

    int id;
    int numerMieszkania;
    char tmp[6][MAX_ULICA];

    while(getline2(&line, &len, fptr) != -1) { //Czy jest kolejny kontakt


        for(size_t i = 0; i < len; i++) {
            if(line[i] == '\n')
                line[i] = '\0';
        }

        char *ptr = strtok(line, delim);
        id = atoi(ptr);

        for(int i = 0; i < 6; i++) {
			ptr = strtok(nullptr, delim);
			strcpy(tmp[i], ptr);

        }
        ptr = strtok(nullptr, delim);
        numerMieszkania = atoi(ptr);

        ptr = strtok(nullptr, delim);														// ptr wskazuje na telefony
        char *telefonyTmp = (char*)malloc(strlen(ptr) + 1);								// pamiec telefonow przed podzialem
        strcpy(telefonyTmp, ptr);

		ptr = strtok(nullptr, delim);															// ptr wskazuje na telefony
        char *emaileTmp = (char*)malloc(strlen(ptr) + 1);								// pamiec telefonow przed podzialem
        strcpy(emaileTmp, ptr);


        int iletelefonow;
        char **telefony = podzial(telefonyTmp, ';', &iletelefonow);

		int ileEmail;
        char **emaile = podzial(emaileTmp, ';', &ileEmail);
		dodajNaKoniecListy(head, id, tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], numerMieszkania, telefony, iletelefonow, emaile, ileEmail);

        free(telefonyTmp);
        free(emaileTmp);
    }

    printf("Lista zostala zaktualizowana!\n");
	fclose(fptr);
}


///Funkcja wypisuje zawartosc pliku do konsoli
///@param zmienna tablicowa przechowujaca nazwe pliku
void wypiszZPliku(const char nazwaPliku[]) {
    char *line = NULL;
    size_t len = 0;
    FILE *fptr;
    if ((fptr = fopen(nazwaPliku,"r")) == nullptr) {
       printf("Nie mozna otworzyc pliku!\n");
       exit(1);
    }
	const char delim[] = {separatorRekordu, '\0'};										// # jest separatorem
    while(getline2(&line, &len, fptr) != -1) { //sprawdzenie czy znajduje sie kolejny kontakt
        char *ptr;

        printf("%s\t", strtok(line, delim));
		printf("%s\t", strtok(nullptr, delim));
		printf("%s\t", strtok(nullptr, delim));
		ptr = strtok(nullptr, delim);
		printf("%c%c-%c%c%c, ", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4]);
		printf("%s, ", strtok(nullptr, delim));
		printf("%s ", strtok(nullptr, delim));
		printf("%s", strtok(nullptr, delim));
		ptr = strtok(nullptr, delim);
		if(strcmp(ptr, "0") != 0)
			printf("/%s\t", ptr);
		else
			putchar('\t');

		ptr = strtok(nullptr, delim);
		char *ptrMail = strtok(nullptr, delim);

		if(strcmp(ptr, "-") != 0)
		{
			ptr = strtok(ptr, ";");
			printf("%s ", ptr);

			while((ptr = strtok(nullptr, ";"))!=nullptr)
				printf("%s ", ptr);
		}
		else
			printf("brak telefonu");

		putchar('\t');

		if(strcmp(ptrMail, "-") != 0)
		{
			ptr = strtok(ptrMail, ";");
			printf("%s ", ptr);
			while((ptr = strtok(nullptr, ";"))!=nullptr)
				printf("%s ", ptr);
		}
		else
			puts("brak email");

        putchar('\n');							// znak enter po calym rekordzie

    }

    fclose(fptr);
}

///Funkcja pozwalajaca usunac podany kontakt po ID
///@param zmienna calkowita przechowujaca ID
///@param zmienna tablicowa przechowujaca nazwe pliku
void usunZPlikuPoId(int id, const char nazwaPliku[]) {
    struct Kontakt *headTmp = malloc(sizeof(struct Kontakt));
    inicjalizacjaListy(headTmp);

    plikDoListy(headTmp, nazwaPliku, 0);
    usunZListyPoID(headTmp, id);
    wyczyscPlik(nazwaPliku);
    listaDoPliku(headTmp, nazwaPliku);

    zwolnijPamiec(headTmp);
    free(headTmp);
    printf("\nElement zostal usuniety!\n");
}

///Funkcja sprawdzajaca czy znak moze byc znakiem adresu dla funkcji wczytajSekwencje
///imie i nazwisko to litery, oraz spacja
///@param kod ascii sprawdzanego znaku
///@return prawda gdy spelnia warunki; falsz, gdy nie spelnia
int znakiImieniaNazwiska(int ascii)
{
	return isalpha(ascii) || ascii == ' ';
}



///Funkcja sprawdzajaca czy znak moze byc znakiem adresu dla funkcji wczytajSekwencje
///adres moze miec dowolne znaki
///@param kod ascii sprawdzanego znaku
///@return prawda gdy spelnia warunki; falsz, gdy nie spelnia
int znakiAdresu(int ascii)
{
	return ascii != '#'; /// zakladamy ze kazdy spelnia oprocz separatora
}

///Funkcja sprawdzajaca czy znak moze byc znakiem numeru telefonu dla funkcji wczytajSekwencje
///telefon zawiera cyfry i myslniki
///@param kod ascii sprawdzanego znaku
///@return prawda gdy spelnia warunki; falsz, gdy nie spelnia
int znakiTelefonu(int ascii)
{
	return isdigit(ascii) || (ascii == '-');
}



///Funkcja sprawdzajaca czy znak moze byc znakiem email dla funkcji wczytajSekwencje
///email zawiera litery, cyfry, @, kropki
///@param kod ascii sprawdzanego znaku
///@return prawda gdy spelnia warunki; falsz, gdy nie spelnia
int znakiEmail(int ascii)
{
	return isalpha(ascii) || ascii == '@' || ascii == '.';
}


///Funkcja wczytujaca do tablicy wyraz ciagu o dlugosci [minZnakow, maxZnakow]
///@param zmienna tablicowa przechowujaca wyraz
///@param zmienna calkowita przechowujaca minimalna liczbe znakow, inna dla login i dla hasla
///@param zmienna calkowita przechowujaca maksymalna liczbe znakow
///@param wskaYnik na stala znakowa
///@param wskaznik do funkcji sprawdzjacej znak sekwencji
void wczytajSekwencje(char wyraz[], int minZnakow, int maxZnakow, const char* coWczytujemy, int (*check)(int))
{
	while(TRUE)
	{
		printf("Wprowadz %s:", coWczytujemy);
		fgets(wyraz, maxZnakow + 1, stdin); // wczytuje maxZnakow znakow, +1 na znak '\0'

		// sprawdzenie czy ENTER sie zmiescil
		char *pos = strchr(wyraz, '\n');

		if(pos == NULL)						// ENTER zostal w buforze stdin -> nie zmiescil sie
			while(getchar() != '\n')		// wyczyszczenie bufora, by w kolejny wczytaniu nie zostalo wczytane
				continue;
		else
			*pos = '\0';					// ENTER wczytany do tablicy login -> zamieniamy go na '\0'

		pos = wyraz;
		int dlugoscWyrazu = 0;
		int znakiDozwolone = TRUE;
		while(*pos != '\0')
		{
			if(!check(*pos))				// jesli nie jest znakiem spelniajacym kryterium funkcji wskazywanej przez wskaznik check
			{
				znakiDozwolone = FALSE;
				break;
			}
			++dlugoscWyrazu;				// zliczanie dlugosci wyrazu
			++pos;							// przejscie do kolejnego znaku tablicy
		}

		// Wczytano poprawny login -> wychodzimy
		if(znakiDozwolone)
		{
			// wszystkie znaki byly poprawne, wystarczy sprawdzic czy dlugosc sekwencji spelnia minZnakow
			if(dlugoscWyrazu >= minZnakow)
				return;						// spelnia, wychodzimy z funkcji
			else // za krotki login
			{
				puts("Za mala liczba znakow.");
				continue;
			}
		}
		else // zawiera znaki niedozwolone
		{
			puts("Niedozwolone znaki w sekwencji.");
			continue;
		}
	}
}

///Funkcja dodaje login i haslo do pliku o nazwie loginy
///@param nazwa pliku przechowujacego pary login i haslo
void rejestruj(const char* loginy)
{
	FILE *fd = fopen(loginy, "a+");				// otwarcie pliku w trybie dopisywania
	if(!fd)
	{
		puts("Nie mozna otworzyc pliku logowania.");
		return;
	}
	static char login[MAX_LOGIN + 1];					// bufor na login i '\0'
	static char haslo[MAX_HASLO + 1];					// bufor na haslo i '\0'
	static char pozycja[MAX_LOGIN + MAX_HASLO + 3];		// login + '\t' + haslo + '\n' + '\0'


	while(TRUE)
	{
		wczytajSekwencje(login, 3, MAX_LOGIN, "login", isalnum);	// wczytanie loginu o dlugosci 3 do 20 znakow
		rewind(fd);										// przejscie na poczatek pliku, celem sprawdzenia, czy login sie nie powtarza
		int powtorzonyNick = FALSE;						// zakladamy, ze sie nie powtarza
		while(!powtorzonyNick)
		{
			fgets(pozycja, MAX_LOGIN + MAX_HASLO + 3, fd);	// wczytanie kolejnego wiersza pliku zawierajacego: login + '\t' + haslo + '\n'
			if(feof(fd))								// wczytanie nie udalo sie
				break;

			char *loginPos = strtok(pozycja, "\t");		// wskaznik na kolejny login z pliku
			if(strcmp(loginPos, login) == 0)			// jesli login z pliku jest taki sam jak wczytany login, to musimy ponownie wczytac login
			{
				puts("Podany login jest zajety");
				powtorzonyNick = TRUE;
				break;
			}
		}
		if(!powtorzonyNick)
		{
			wczytajSekwencje(haslo, 8, MAX_HASLO, "haslo", isalnum); // wczytanie poprawnego hasla
			fprintf(fd, "%s\t%s\n", login, haslo);
			break;
		}
	}
	fclose(fd);											// zamykamy plik
}

///Funkcja logowania do bazy danych
///@param nazwa pliku przechowujacego pary login i haslo
///@return -1, gdy nie da sie otworzyć pliku @param, 1, gdy zaladowano, 0 gdy nie zalogowano
int zaloguj(const char* loginy)
{
	FILE *fd = fopen(loginy, "r");						// otwarcie pliku do odczytu
	if(!fd)
	{
		puts("Brak zarejestrowanych uzytkownikow.");
		return -1;										// brak pliku, nalezy sie najpierw zarejestrowac
	}

	static char login[MAX_LOGIN + 1];
	static char haslo[MAX_HASLO + 1];
	static char pozycja[MAX_LOGIN + MAX_HASLO + 3];

	for(int j = 0; j < PROB; ++j) // 3 proby
	{
		wczytajSekwencje(login, 3, MAX_LOGIN, "login", isalnum);	// wczytanie loginu
		int znalezionyNick = FALSE;						// informacja, czy znaleziono login
		rewind(fd);										// przejscie na poczatek pliku
		while(!znalezionyNick)
		{
			fgets(pozycja, MAX_LOGIN + MAX_HASLO + 3, fd);
			if(feof(fd))								// wczytanie nie powiodlo sie
				break;


			// znakiem rozdzielajacym jest TABULACJA lub ENTER
			// rozbicie wiersza z pliku na login i haslo
			char *loginPos = strtok(pozycja, "\t\n");
			char *hasloPos = strtok(NULL, "\t\n");


			// znaleziona pozycja o zadanym loginie
			if(strcmp(loginPos, login) == 0)			// zadany login jest w pliku, musimy sprawdzic haslo
			{
				znalezionyNick = TRUE;
				for(int i = 0; i < PROB; ++i) // max 3 proby
				{
					wczytajSekwencje(haslo, 8, MAX_HASLO, "haslo", isalnum);	// uzytkownik podaje haslo
					if(strcmp(hasloPos, haslo) == 0)		// jesli haslo sie zgadza, to jestesmy zalogowani -> konczymy funkcje
					{
						puts("Zalogowano.");
						fclose(fd);							// zamykamy plik, przed wyjsciem z funkcji
						return 1;
					}
					puts("Niepoprawne haslo. Sprobuj ponownie.");
				}
				break;									// 3 razy niepoprawne haslo, wracamy do ponownego wczytania loginu
			}
		}
		if(!znalezionyNick)
			puts("Login nie jest zarejestrowany");

	}
	fclose(fd);
	return 0; // nie zalogowano
}

///Sortowanie listy dwukierunkowej, przez wybieranie
///@param headPtr - wskaznik do wskaznika na poczatek listy
void sortujPoID(struct Kontakt **headPtr)
{
	// gdy brak listy lub jest 1 element, to nic nie robimy
	if(*headPtr == NULL || (*headPtr)->next == NULL)
		return;

	/*Pierwszy krok sortowania - zamiana glowy z elementem najmniejszym sposrod pozostalych elementow listy,
	jesli id wezla minimum jest mniejsze od id glowy */

	// conajmniej 2 elementy na liscie
	// minimum - wskaznik na najmniejszy element listy od 2 elementu do ostatniego
	struct Kontakt *minimum = (*headPtr)->next;

	// tmp - wskaznik pomocniczy
	struct Kontakt *tmp = minimum->next;

	// poszukiwanie wezla listy o najmniejszy id i zapamietanie go pod minimum,
	while(tmp)
	{
		if(tmp->id < minimum->id)
			minimum = tmp;
		tmp = tmp->next;
	}

	// jesli znalezione id jest mniejsze od id glowy listy, to przepinamy wezly
	if((*headPtr)->id > minimum->id)
	{
		if((*headPtr)->next == minimum)			// gdy glowa listy sasiaduje ze znalezionym wezlem o minimalnym id
		{
			(*headPtr)->next = minimum->next;
			if((*headPtr)->next != NULL)		// gdy znaleziony wezel minimum nie jest ostatni
				(*headPtr)->next->prev = *headPtr;
			minimum->prev = NULL;
			minimum->next = *headPtr;
			(*headPtr)->prev = minimum;

		}
		else	// glowa listy nie sasiaduje ze znalezionym minimum
		{
			(*headPtr)->prev = minimum->prev;
			minimum->prev->next = *headPtr;
			(*headPtr)->next->prev = minimum;
			struct Kontakt *hnext = (*headPtr)->next;
			(*headPtr)->next = minimum->next;
			if((*headPtr)->next != NULL)
				(*headPtr)->next->prev = (*headPtr);

			minimum->next = hnext;
			minimum->prev = NULL;
		}
		*headPtr = minimum;
	}

	struct Kontakt *pivot = (*headPtr)->next;
	while(pivot->next != NULL)					// jest pivot stanie sie NULL, to lista jest posortowana
	{
		minimum = pivot->next;					// poszukiwanie elementu najmniejszego sposorod wezlow bedacych za pivot
		tmp = minimum->next;
		while(tmp)
		{
			if(tmp->id < minimum->id)
				minimum = tmp;

			tmp = tmp->next;
		}

		if(pivot->id > minimum->id)
		{
			if(pivot->next == minimum)
			{
				pivot->next = minimum->next;
				if(pivot->next != NULL)
					pivot->next->prev = pivot;
				minimum->prev = pivot->prev;
				minimum->next = pivot;
				pivot->prev->next = minimum;
				pivot->prev = minimum;

			}
			else
			{
				struct Kontakt *pivot_prev = pivot->prev;
				struct Kontakt *pivot_next = pivot->next;

				pivot->prev = minimum->prev;
				minimum->prev->next = pivot;
				pivot->next->prev = minimum;

				pivot->next = minimum->next;
				if(pivot->next != NULL)
					pivot->next->prev = pivot;

				minimum->next = pivot_next;
				minimum->prev = pivot_prev;
				pivot_prev->next = minimum;
			}
			pivot = minimum;
		}
		pivot = pivot->next;				// przesuniecie wskaznika pivot na nastepny wezel
	}
}

///szukanie rekordu po unikalnym numerze id,
///@param wskaznik na element, od ktorego szukamy
///@param unikalny numer ID rekordu Kontakt
///@return adres znalezionego elementu lub nullptr, gdy nie znaleziono
struct Kontakt* znajdz(struct Kontakt* tmp, int id)
{
	while(tmp != nullptr)
	{
		if(tmp->id == id)	// znaleziono
			break;
		tmp = tmp->next;
	}
	return tmp;
}


///wczytuje liczbe z zakresu [0, max]
///@param lancuch znakow podpowiadajacy, co uzytkownik powinien wpisac
///@param maksymalna prawidlowa wartosc,
///@return wartosc calkowita z przedzialu [0, max]
int wczytajLiczbe(const char *komunikat, int max)
{
	int id;				// zmienna na wprowadzana liczbe
	double wartosc;		// liczba dowolnego typu
	int status;			// sprawdzenie czy wczytano liczbe
	char znak;			// pomocniczy znak do analizy poprawnosci

	while(TRUE)
	{
		printf("%s: ", komunikat);
		// usuniecie spacji i tabulacji
		while(TRUE)
		{
			znak = getchar();
			if(!isblank(znak))
				break;

		}
		// wprowadzono biale znaki
		if(znak == '\n')
			continue;
		else					// jest jakis znak drukowalny
			ungetc(znak, stdin);

		status = scanf("%lf", &wartosc);

		while(getchar() != '\n')		// wyczyszczenie bufora, by w kolejny wczytaniu nie zostalo wczytane
			continue;
		if(status == 0)
		{
			puts("Niepoprawne dane");
			continue;					// ponownie wczytujemy
		}

		id = (int)wartosc;			// pobranie czesci calkowitej
		if((double)id != wartosc)		// jesli nie podano liczby calkowitej
		{
			puts("Niepoprawne dane");
			continue;
		}


		if(id > max)
		{
			puts("Niepoprawny zakres liczby");
			continue;
		}
		break;
	}
	return id;
}

///Funkcja zmienia adres poprzez opcjonalna modyfikacje danych adresowych
///@param zmianiany rekord
///@param pomocniczy bufor z funkcji nadrzednej edytuj
void edycjaAdresu(struct Kontakt* st, char bufor[MAX_MIASTO + 1])
{
	int opcja;
	printf("%s: %c%c-%c%c%c\n", "Kod pocztowy", st->kodPocztowy[0],st->kodPocztowy[1],st->kodPocztowy[2], st->kodPocztowy[3], st->kodPocztowy[4]);
	opcja = wczytajLiczbe("Czy zmienic kod pocztowy?(0 - NIE, 1 - TAK)", 1);
	if(opcja == 1)
	{
		wczytajSekwencje(bufor, 5, MAX_KOD, "kod pocztowy (5 - cyfr)", isdigit);						// 5 cyfr kodu pocztowego
		strcpy(st->kodPocztowy, bufor);
	}

	printf("%s: %s\n", "Miejscowosc", st->miasto);
	opcja = wczytajLiczbe("Czy zmienic miejscowosc?(0 - NIE, 1 - TAK)", 1);
	if(opcja == 1)
	{
		wczytajSekwencje(bufor, 3, MAX_MIASTO, "miejscowosc", znakiImieniaNazwiska);					// minimum 3 litery
		strcpy(st->miasto, bufor);
	}

	printf("%s: %s\n", "Ulica", st->ulica);
	opcja = wczytajLiczbe("Czy zmienic nazwe ulicy?(0 - NIE, 1 - TAK)", 1);
	if(opcja == 1)
	{
		wczytajSekwencje(bufor, 3, MAX_ULICA, "ulice", znakiImieniaNazwiska);							// minimum 3 litery
		strcpy(st->ulica, bufor);
	}

	printf("%s: %s\n", "Numer domu", st->numerDomu);
	opcja = wczytajLiczbe("Czy zmienic numer domu?(0 - NIE, 1 - TAK)", 1);
	if(opcja == 1)
	{
		wczytajSekwencje(bufor, 1, MAX_DOM, "numer domu", isalnum);									// minimum 1 cyfra i litera
		strcpy(st->numerDomu, bufor);
	}


	if(st->numerMieszkania > 0)
	{
		printf("%s: %d\n", "Numer lokalu", st->numerMieszkania);
		opcja = wczytajLiczbe("Czy zmienic numer lokalu?(0 - NIE, 1 - TAK)", 1);
		if(opcja == 1)
			st->numerMieszkania = wczytajLiczbe("Podaj numer lokalu(0 - brak numeru lokalu)", INT_MAX); // dodolne nieujemne ID
	}
	else
	{
		opcja = wczytajLiczbe("Czy dodac numer lokalu?(0 - NIE, 1 - TAK)", 1);
		if(opcja == 1)
			st->numerMieszkania = wczytajLiczbe("Podaj numer lokalu(0 - brak numeru lokalu)", INT_MAX); // dodolne nieujemne ID
	}

}

///Funkcja pyta uzytkownika ile numerow telefonu pobrac od uzytkownika.
///Tworzy dynamiczna tablice wskaznikow do odczytanych numerow telefonow
///@param pomocnicza tablica na wczytywany numer
///@param wskaznik do liczby przechowywanych numerow, aktualizowanej w tej funkcji.
///@return wskaznik do gotowej dynamicznej tablicy wskaznikow do istniejacych numerow, lub nullptr, gdy nie wprowadzono numerow
char **wczytajTelefony(char tmpTelefon[DLUGOSC_TELEFONU + 1], int *ileNumerow)
{
	char **telefony = nullptr;
	int iloscNumerow = wczytajLiczbe("Ile numerow telefonu(0 - brak telefonu)", MAX_EMAIL_TELEFON); // dodolne nieujemne ID
	if(iloscNumerow > 0)
	{
		telefony = (char**)malloc(sizeof(char*) * iloscNumerow);
		for(int i = 0; i < iloscNumerow; ++i)
		{
			wczytajSekwencje(tmpTelefon, 3, DLUGOSC_TELEFONU, "numer telefonu", znakiTelefonu);
			telefony[i] = (char*)malloc(strlen(tmpTelefon) + 1);
			strcpy(telefony[i], tmpTelefon);
		}
	}
	*ileNumerow = iloscNumerow;
	return telefony;
}

///Funkcja wybiera numer telefonu z listy numerow w kontakcie
///@param wskaznik do kontaktu zawierajacego numery
///@return -1, gdy brak numerow; 0, gdy anulowano wybor, 1 lub wiecej - wybrany numer
int wybierzTelefon(struct Kontakt* kontakt)
{
	if(kontakt->telefony == nullptr)
		return -1;

	for(int i = 0; i < kontakt->iloscNumerow; ++i)
		printf("%d -> %s\n", i + 1, kontakt->telefony[i]);
	int opcja = wczytajLiczbe("Wybierz numer(0 - anuluj)", kontakt->iloscNumerow);
	return opcja;
}

///Funkcja zmienia numer telefonu wskazany przez uzytkownika
///@param wskaznik do Kontaktu, w ktorym zmieniamy numer telefonu
///@param pomocnicza tablica do wczytywania nowego numeru telefonu na miejsce starego
void zmienNumerTelefonu(struct Kontakt* kontakt, char tmpTelefon[DLUGOSC_TELEFONU + 1])
{
	int indeks = wybierzTelefon(kontakt);
	if(indeks == -1)
	{
		puts("Brak numerow telefonu w kontakcie.");
		return;
	}

	if(indeks == 0)
	{
		puts("Anulowano operacje.");
		return;
	}

	--indeks;									// tablice numerujemy od 0
	free(kontakt->telefony[indeks]);			// usuniecie starego numeru
	wczytajSekwencje(tmpTelefon, 3, DLUGOSC_TELEFONU, "numer telefonu", znakiTelefonu);		// wczytanie nowego numeru
	kontakt->telefony[indeks] = (char*)malloc(strlen(tmpTelefon) + 1);	// utworzenie pamieci na nowy numer
	strcpy(kontakt->telefony[indeks], tmpTelefon);	// zapisanie nowego numeru z pamieci pomocniczej
}

///Funkcja usuwa numer telefonu z dynamicznej tablicy wskaznikow
///@param wskaznik do kontaktu w ktorym usuwamy telefon
void usunNumerTelefonu(struct Kontakt* kontakt)
{
	int indeks = wybierzTelefon(kontakt);
	if(indeks == -1)
	{
		puts("Brak numerow telefonu w kontakcie.");
		return;
	}

	if(indeks == 0)
	{
		puts("Anulowano operacje.");
		return;
	}
	--indeks;
	free(kontakt->telefony[indeks]);						// usuniecie numeru telefonu
	--(kontakt->iloscNumerow);								// aktualizacja liczby telefonow

	if(kontakt->iloscNumerow == 0)							// zwolnienie tablicy wskaznikow do numerow telefow, bo brak numerow telefonu
	{
		free(kontakt->telefony);
		kontakt->telefony = nullptr;
	}
	else
	{

		for(int j = indeks; j < kontakt->iloscNumerow; ++j)
			kontakt->telefony[j] = kontakt->telefony[j + 1];

		char **telefonyTmp = (char**)malloc(sizeof(char*) * (kontakt->iloscNumerow));	// utworzenie tablicy dynamicznej pomniejszonej o 1 element
		for(int i = 0; i < kontakt->iloscNumerow; ++i)									// przepisanie wskaznikow
			telefonyTmp[i] = kontakt->telefony[i];

		free(kontakt->telefony);														// zwolnienie starej tablicy
		kontakt->telefony = telefonyTmp;												// aktualizacja tablicy numerow w Kontakcie
	}
}


///Funkcja sprawdza, czy kontakt zawiera numer telefonu
///@param przeszukiwany kontakt
///@param sprawdzany numer telefony w kontakcie
///@return 0 - nie zawiera numeru, 1 - zawiera numer
int czyZawieraTelefon(struct Kontakt* kontakt, char telefonTmp[DLUGOSC_TELEFONU + 1])
{
	for(int i = 0; i < kontakt->iloscNumerow; ++i)
	{
		if(strcmp(kontakt->telefony[i], telefonTmp) == 0)			// numer telefonu pasuje
			return 1;
	}
	return 0;		// brak numerow nie zawiera
}


///Funkcja sprawdza, czy kontakt zawiera e-mail
///@param przeszukiwany kontakt
///@param sprawdzany e-mail w kontakcie
///@return 0 - nie zawiera e-mail, 1 - zawiera e-mail
int czyZawieraEmail(struct Kontakt* kontakt, char emailTmp[DLUGOSC_EMAIL + 1])
{
	for(int i = 0; i < kontakt->iloscEmail; ++i)
	{
		if(strcmp(kontakt->email[i], emailTmp) == 0)			// numer telefonu pasuje
			return 1;
	}
	return 0;		// brak numerow nie zawiera
}


///Funkcja wybiera e-mail z listy e-mail w kontakcie
///@param wskaznik do kontaktu zawierajacego e-mail
///@return -1, gdy brak e-mail; 0, gdy anulowano wybor, 1 lub wiecej - wybrany e-mail
int wybierzEmail(struct Kontakt* kontakt)
{
	if(kontakt->email == nullptr)
		return -1;

	for(int i = 0; i < kontakt->iloscEmail; ++i)
		printf("%d -> %s\n", i + 1, kontakt->email[i]);
	int opcja = wczytajLiczbe("Wybierz e-mail(0 - anuluj)", kontakt->iloscEmail);
	return opcja;
}

///Funkcja zmienia e-mail wskazany przez uzytkownika
///@param wskaznik do Kontaktu, w ktorym zmieniamy e-mail
///@param pomocnicza tablica do wczytywania nowego e-mail na miejsce starego
void zmienEmail(struct Kontakt* kontakt, char tmpEmail[DLUGOSC_EMAIL + 1])
{
	int indeks = wybierzEmail(kontakt);
	if(indeks == -1)
	{
		puts("Brak adresow e-mail w kontakcie.");
		return;
	}

	if(indeks == 0)
	{
		puts("Anulowano operacje.");
		return;
	}

	--indeks;																	// tablice numerujemy od 0
	free(kontakt->email[indeks]);												// usuniecie starego e-mail
	wczytajSekwencje(tmpEmail, 3, DLUGOSC_EMAIL, "adres e-mail", znakiEmail);	// wczytanie nowego e-mail
	kontakt->email[indeks] = (char*)malloc(strlen(tmpEmail) + 1);				// utworzenie pamieci na nowy e-mail
	strcpy(kontakt->email[indeks], tmpEmail);									// zapisanie nowego e-mail z pamieci pomocniczej
}

///Funkcja usuwa e-mail telefonu z dynamicznej tablicy wskaznikow
///@param wskaznik do kontaktu w ktorym usuwamy e-mail
void usunEmail(struct Kontakt* kontakt)
{
	int indeks = wybierzEmail(kontakt);
	if(indeks == -1)
	{
		puts("Brak adresow e-mail w kontakcie.");
		return;
	}

	if(indeks == 0)
	{
		puts("Anulowano operacje.");
		return;
	}
	--indeks;												// tablice numerujemy od 0
	free(kontakt->email[indeks]);							// usuniecie e-mail
	--(kontakt->iloscEmail);								// aktualizacja liczby e-mail

	if(kontakt->iloscEmail == 0)							// zwolnienie tablicy wskaznikow do e-mail
	{
		free(kontakt->email);
		kontakt->email = nullptr;
	}
	else
	{
		for(int i = indeks; i < kontakt->iloscEmail; ++i)
			kontakt->email[i] = kontakt->email[i + 1];

		char **emaileTmp = (char**)malloc(sizeof(char*) * (kontakt->iloscEmail));	// utworzenie tablicy dynamicznej pomniejszonej o 1 element
		for(int i = 0; i < kontakt->iloscEmail; ++i)								// przepisanie wskaznikow
			emaileTmp[i] = kontakt->email[i];

		free(kontakt->email);														// zwolnienie starej tablicy
		kontakt->email = emaileTmp;													// aktualizacja tablicy e-mail w Kontakcie
	}
}



///Funkcja pyta uzytkownika ile adresow e-mail pobrac od uzytkownika.
///Tworzy dynamiczna tablice wskaznikow do odczytanych adresow e-mail
///@param pomocnicza tablica na wczytywany e-mail
///@param wskaznik do liczby przechowywanych e-mail, aktualizowanej w tej funkcji.
///@return wskaznik do gotowej dynamicznej tablicy wskaznikow do istniejacych e-mail, lub nullptr, gdy nie wprowadzono numerow
char **wczytajEmaile(char tmpEmail[DLUGOSC_EMAIL + 1], int *ileMail)
{
	char **emaile = nullptr;
	int iloscEmail = wczytajLiczbe("Ile adresow e-mail(0 - brak email)", MAX_EMAIL_TELEFON); // dodolne nieujemne ID
	if(iloscEmail > 0)
	{
		emaile = (char**)malloc(sizeof(char*) * iloscEmail);
		for(int i = 0; i < iloscEmail; ++i)
		{
			wczytajSekwencje(tmpEmail, 0, DLUGOSC_EMAIL, "email", znakiEmail);
			emaile[i] = (char*)malloc(strlen(tmpEmail) + 1);
			strcpy(emaile[i], tmpEmail);
		}
	}
	*ileMail = iloscEmail;
	return emaile;
}


///Zmiana numerow telefonu
///@param zmianiany rekord
///@param pomocniczy bufor z funkcji nadrzednej edytuj
void edycjaNumeruTelefonu(struct Kontakt *k, char bufor[MAX_MIASTO + 1])
{
	int opcja;				// wczytywana opcja
	while(TRUE)
	{
		if(k->iloscNumerow == 0)
			k->telefony = wczytajTelefony(bufor, &(k->iloscNumerow));
		else // istnieja jakies numery telefonu
		{
			puts("1 -> Dodanie kolejnego numeru");
			puts("2 -> Usuniecie numeru");
			puts("3 -> Zmiana numeru");
			puts("4 -> Usuniecie wszystkich numerow");
			puts("0 -> Anuluj");

			opcja = wczytajLiczbe("Wybierz opcje[0 - 4]", 4);
			switch(opcja)
			{
			case 1:
				wczytajSekwencje(bufor, 3, DLUGOSC_TELEFONU, "numer telefonu", znakiTelefonu);
				dodajKolejnyNumer(k, bufor);
				break;
			case 2:
				usunNumerTelefonu(k);
				break;
			case 3:
				zmienNumerTelefonu(k, bufor);
				break;
			case 4:
				usunTelefony(k);
				break;
			}
		}
		int opcja = wczytajLiczbe("Czy zakonczyc?(0 - TAK, 1 - NIE)", 1);
		if(opcja == 0)
			break;
	}
}

///Zmiana adresow e-mail
///@param zmianiany rekord
///@param pomocniczy bufor z funkcji nadrzednej edytuj
void edycjaEmail(struct Kontakt *k, char bufor[MAX_MIASTO + 1])
{
	int opcja;				// wczytywana opcja
	while(TRUE)
	{
		if(k->iloscEmail == 0)
			k->email = wczytajEmaile(bufor, &(k->iloscEmail));
		else // istnieja jakies numery telefonu
		{
			puts("1 -> Dodanie kolejnego e-mail");
			puts("2 -> Usuniecie e-mail");
			puts("3 -> Zmiana e-mail");
			puts("4 -> Usuniecie wszystkich e-mail");
			puts("0 -> Anuluj");

			opcja = wczytajLiczbe("Wybierz opcje[0 - 4]", 4);
			switch(opcja)
			{
			case 1:
				wczytajSekwencje(bufor, 3, DLUGOSC_EMAIL, "e-mail", znakiEmail);
				dodajKolejnyEmail(k, bufor);
				break;
			case 2:
				usunEmail(k);
				break;
			case 3:
				zmienEmail(k, bufor);
				break;
			case 4:
				usunEmaile(k);
				break;
			}
		}
		int opcja = wczytajLiczbe("Czy zakonczyc?(0 - TAK, 1 - NIE)", 1);
		if(opcja == 0)
			break;
	}
}


///Edycja rekordu kontakt
///@param wskaznik do pierwszego elementu listy kontaktow
void edytuj(struct Kontakt *head)
{
	int id = wczytajLiczbe("Podaj numer ID rekordu", INT_MAX);

	struct Kontakt *znalezionyRekord = znajdz(head, id);
	if(znalezionyRekord == NULL)
	{
		printf("Nie znaleziono rekordu o ID: %d\n", id);
		return;
	}
	static char bufor[MAX_MIASTO + 1];

	wypiszRekordPionowo(znalezionyRekord);
	puts("1 -> zmiana imienia");
	puts("2 -> zmiana nazwiska");
	puts("3 -> zmiana adresu");
	puts("4 -> zmiana telefonu");
	puts("5 -> zmiana email");
	puts("0 -> niczego nie zmieniaj");

	int opcja = wczytajLiczbe("Wprowadz opcje", 5); // opcja z zakresu [0; 5]
	switch(opcja)
	{
	case 1:
		wczytajSekwencje(bufor, 3, DLUGOSC_NAZWY, "nowe imie", znakiImieniaNazwiska); // imie tylko z liter i spacji
		strcpy(znalezionyRekord->imie, bufor);	// zmiana imienia w rekordzie
		break;
	case 2:
		wczytajSekwencje(bufor, 3, DLUGOSC_NAZWY, "nowe nazwisko", znakiImieniaNazwiska); // nazwisko tylko z liter i spacji
		strcpy(znalezionyRekord->nazwisko, bufor);	// zmiana nazwiska w rekordzie
		break;
	case 3:
		edycjaAdresu(znalezionyRekord, bufor);
		break;
	case 4:
		edycjaNumeruTelefonu(znalezionyRekord, bufor);
		break;
	case 5:
		edycjaEmail(znalezionyRekord, bufor);
		break;
	case 0:
		puts("Anulowano zmiany.");

	}
}


///Funkcja zmieniajaca duze litery na male
///@param wskaznik do tablicy zawierajacej lanuch znakow
void zamienNaMaleLitery(char *bufor)
{
	for(int i = 0; bufor[i] != '\0'; ++i)
		bufor[i] = tolower(bufor[i]);					// zamiana pojedynczego znaku na maly, jesli jest litera
}

///Funkcja przeszukujaca baze kontaktow po wybranym kryterium przez uzytkownika, wyswietla rekordy spelniajace zadanie
///@param wskaznik na poczatek listy kontaktow
void wyszukaj(struct Kontakt *head)
{
	puts("1 -> wyszukiwanie po id");
	puts("2 -> wyszukiwanie po imieniu");				// bez rozroznienia wielkosci liter
	puts("3 -> wyszukiwanie po nazwisku");				// bez rozroznienia wielkosci liter
	puts("4 -> wyszukiwanie po adresie");				// bez rozroznienia wielkosci liter
	puts("5 -> wyszukiwanie po numerze telefonu");
	puts("6 -> wyszukiwanie po email");					// rozroznia wielkosci liter
	puts("0 -> powrot");

	int opcja = wczytajLiczbe("Wprowadz opcje", 6);     // opcja z zakresu [0; 6]
	int id;
	struct Kontakt *temp;	// wskaznik pmocniczy

	// bufor na wprowadzone dane z klawiatury
	static char zapytanie[MAX_MIASTO + 1];

	static char miejscowosc[MAX_MIASTO + 1];
	static char kod[MAX_KOD + 1];
	static char ulica[MAX_ULICA + 1];
	static char dom[MAX_DOM + 1];
	int numerMieszkania;

	static char pomniejszone[MAX_MIASTO + 1];

	switch(opcja)
	{
	case 1:
		id = wczytajLiczbe("Podaj numer ID rekordu", INT_MAX); // rekord [0, do MAX_INT], -1 zarezerwowane
		temp = znajdz(head, id);
		if(temp)
			wypiszRekordPionowo(temp);
		else
			printf("Brak rekordu o id: %d\n", id);
		break;
	case 2:
		wczytajSekwencje(zapytanie, 3, DLUGOSC_NAZWY, "szukane imie", znakiImieniaNazwiska); // imie tylko z liter i spacji
		zamienNaMaleLitery(zapytanie);
		// szukanie elementow na liscie
		for(temp = head; temp != NULL; temp = temp->next)
		{
			strcpy(pomniejszone, temp->imie);			// skopiowanie imienia do pomniejszenia liter
			zamienNaMaleLitery(pomniejszone);			// pomniejszenie liter
			if(strcmp(zapytanie, pomniejszone) == 0)	// imiona pasuja, wiec wypisujemy
				wypiszRekordPoziomo(temp);
		}
		break;
	case 3:
		wczytajSekwencje(zapytanie, 3, DLUGOSC_NAZWY, "szukane nazwisko", znakiImieniaNazwiska); // imie tylko z liter i spacji
		zamienNaMaleLitery(zapytanie);
		// szukanie elementow na liscie
		for(temp = head; temp != NULL; temp = temp->next)
		{
			strcpy(pomniejszone, temp->nazwisko);		// skopiowanie nazwiska do pomniejszenia liter
			zamienNaMaleLitery(pomniejszone);			// pomniejszenie liter
			if(strcmp(zapytanie, pomniejszone) == 0)	// nazwiska pasuja, wiec wypisujemy
				wypiszRekordPoziomo(temp);
		}

		break;
	case 4:



		strcpy(kod, "");
		opcja = wczytajLiczbe("Czy uwzglednic kod pocztowy?(0 - NIE, 1 - TAK)", 1);
		if(opcja == 1)
			wczytajSekwencje(kod, 5, MAX_KOD, "kod pocztowy (5 - cyfr)", isdigit);						// 5 cyfr kodu pocztowego

		strcpy(miejscowosc, "");
		opcja = wczytajLiczbe("Czy uwzglednic miejscowosc?(0 - NIE, 1 - TAK)", 1);
		if(opcja == 1)
		{
			wczytajSekwencje(miejscowosc, 3, MAX_MIASTO, "miejscowosc", znakiImieniaNazwiska);					// minimum 3 litery
			zamienNaMaleLitery(miejscowosc);
		}



		strcpy(ulica, "");
		opcja = wczytajLiczbe("Czy uwzglednic ulice?(0 - NIE, 1 - TAK)", 1);
		if(opcja == 1)
		{
			wczytajSekwencje(ulica, 3, MAX_ULICA, "ulice", znakiImieniaNazwiska);							// minimum 3 litery
			zamienNaMaleLitery(ulica);
		}


		strcpy(dom, "");
		opcja = wczytajLiczbe("Czy uwzglednic ulice?(0 - NIE, 1 - TAK)", 1);
		if(opcja == 1)
		{
			wczytajSekwencje(dom, 1, MAX_DOM, "numer domu", isalnum);									// minimum 1 cyfra i litera
			zamienNaMaleLitery(dom);
		}

		numerMieszkania = wczytajLiczbe("Podaj numer lokalu(0 - brak numeru lokalu)", INT_MAX); // dowolne nieujemne ID


		for(temp = head; temp != NULL; temp = temp->next)
		{
			if(strcmp(kod, "") != 0)			//sprawdzenie kodu pocztowego
			{
				strcpy(zapytanie, temp->kodPocztowy);
				zamienNaMaleLitery(zapytanie);
				if(strcmp(kod, zapytanie) != 0)	// jesli nie pasuje to dalej nie sprawdzamy rekordu
					continue;

			}

			if(strcmp(miejscowosc, "") != 0)		// sprawdzenie miasta
			{
				strcpy(zapytanie, temp->miasto);
				zamienNaMaleLitery(zapytanie);
				if(strcmp(miejscowosc, zapytanie) != 0)	// jesli nie pasuje to dalej nie sprawdzamy rekordu
					continue;

			}

			if(strcmp(ulica, "") != 0)			// sprawdzenie ulicy
			{
				strcpy(zapytanie, temp->ulica);
				zamienNaMaleLitery(zapytanie);
				if(strcmp(ulica, zapytanie) != 0)	// jesli nie pasuje to dalej nie sprawdzamy rekordu
					continue;

			}

			if(strcmp(dom, "") != 0)			// sprawdzenie numeru domu
			{
				strcpy(zapytanie, temp->numerDomu);
				zamienNaMaleLitery(zapytanie);
				if(strcmp(dom, zapytanie) != 0)	// jesli nie pasuje to dalej nie sprawdzamy rekordu
					continue;

			}

			if(numerMieszkania > 0)				// sprawdzenie numeru lokalu
			{
				if(numerMieszkania != temp->numerMieszkania)
					continue;
			}

			//tu przeszlo warunki
			wypiszRekordPoziomo(temp);

		}

		break;
	case 5:
		wczytajSekwencje(zapytanie, 3, DLUGOSC_TELEFONU, "szukany numer telefonu", znakiTelefonu); // tylko cyfry i myslniki
		for(temp = head; temp != NULL; temp = temp->next)
			if(czyZawieraTelefon(temp, zapytanie))
				wypiszRekordPoziomo(temp);
		break;
	case 6:
		wczytajSekwencje(zapytanie, 0, DLUGOSC_EMAIL, "szukany email", znakiEmail); // tylko litery, cyfry, @, .
		for(temp = head; temp != NULL; temp = temp->next)
			if(czyZawieraEmail(temp, zapytanie))
				wypiszRekordPoziomo(temp);
		break;
	case 0:
		puts("Anulowano zmiany.");
	}
}


/**
 * Operacje na grupach
*/

///Funkcja usuwa grupe, ktora jest lista.
///Najpierw zwalniamy wezly listy, na koncu strukture grupy.
///@param wskaznik do usuwanej grupy
void skasujGrupe(struct Grupa *g)
{
	struct Element *pierwszy = g->first;
	struct Element *tmp;
	while(pierwszy)
	{
		tmp = pierwszy;
		pierwszy = pierwszy->next;
		free(tmp);
	}
	free(g);
}

///Funkcja pobiera od uzytkownika nazwe grupy skladajaca sie z liter
///Tworzy strukture grupy bez wezlow przechowyjacych adresy Kontaktow
///@param tablica wskaznikow do grup
///@param wskaznik do aktualnej liczby grup, aktualizowany w tej funkcji o 1
///Grupa nie jest tworzona, gdy osiagnieto limit
void utworzGrupe(struct Grupa* grupy[ILOSC_GRUP_MAX], int *iloscGrup)
{
	if(*iloscGrup == ILOSC_GRUP_MAX)
	{
		puts("Osiagnieta maksymalna ilosc grup");
		return;
	}
	struct Grupa *nowa = (struct Grupa*)malloc(sizeof(struct Grupa));				// utworzenie grupy w pamieci
	nowa->first = nullptr;															// poczatkowo brak pozycji w grupie
	wczytajSekwencje(nowa->nazwaGrupy, 3, MAX_GRUPA, "nazwe grupy", isalpha);		// wczytanie nazwy grupy skladajacej sie z liter
	grupy[*iloscGrup] = nowa;														// wpisanie adresu grupy do tablicy grup
	++(*iloscGrup);																	// aktualizacja ilosci grup
}


///Funkcja usuwa grupe o zadanym indeksie. Usuwa grupe z pamieci, a nastepnie usuwa ja z tablicy grup
///@param tablica wskaznikow do grup
///@param wskaznik do aktualnej ilosci grup, dekrementowany, przy usuwaniu
///@param indeks usuwanej grupy z tablicy
void usunGrupe(struct Grupa* grupy[ILOSC_GRUP_MAX], int *iloscGrup, int index)
{
	skasujGrupe(grupy[index]);														// usuniecie grupy
	for(int i = index; i < *iloscGrup - 1; ++i)
		grupy[i] = grupy[i + 1];
	--(*iloscGrup);																	// zmniejszenie liczby grup o 1
}

///usuniecie wszystkich grup
///@param tablica wskaznikow do grup
///@param wskaznik do aktualnej ilosci grup, ustawianej na 0, po zakonczeniu usuwania
void skasujGrupy(struct Grupa* grupy[ILOSC_GRUP_MAX], int *iloscGrup)
{
	for(int i = 0; i < *iloscGrup; ++i)
		skasujGrupe(grupy[i]);
	*iloscGrup = 0;

}


///Pomicnicza funkcja umozliwiajaca identyfikacje grupy poprzez wybor numeru
///@param tablica wskaznikow do grup
///@param aktualna ilosci grup
///@return 0, gdy anulowano wybor, od 1 - wybrany numer grupy, bedacy indeksem w tablicy powiekszonym o 1
int wyborGrupy(struct Grupa* grupy[ILOSC_GRUP_MAX], int iloscGrup)
{
	for(int i = 0; i < iloscGrup; ++i)
		printf("%d - %s\n", i + 1, grupy[i]->nazwaGrupy);

	int grupa = wczytajLiczbe("Wybierz grupe(0 - anuluj)", iloscGrup);
	return grupa;
}

///Wyswietlenie wszystkich kontaktow wchodzacych w sklad grupy
///@param wskaznik do wyswietlanej grupy
void wyswietlGrupe(struct Grupa* grupa)
{
	struct Element *tmp = grupa->first;
	while(tmp != nullptr)
	{
		wypiszRekordPoziomo(tmp->element);
		tmp = tmp->next;
	}
}

///Funkcja dodaje wezel Element zawierajacy wskaznik do kontaktu, do listy w grupie
///Tworzona jest lista jednokierunkowa z definicji posortowana o zadanym ID zawartym w strukturze Kontakt
///@param wskaznik do grupy, do ktorej dodajemy wezel Element skojarzony z kontaktem
///@param wskaznik do Kontaktu, ktory dodajemy do tej grupy
int dodajDoGrupy(struct Grupa* grupa, struct Kontakt* kontaktDodawany)
{
	if(grupa->first == nullptr)
	{
		grupa->first = (struct Element*)malloc(sizeof(struct Element));
		grupa->first->next = nullptr;
		grupa->first->element = kontaktDodawany;
	}
	else
	{
		struct Element* tmp = grupa->first;
		struct Element* tmp_prev = nullptr;
		while(tmp != nullptr)
		{
			if(tmp->element->id == kontaktDodawany->id)
				return 0;												// element juz istnieje
			else if(tmp->element->id < kontaktDodawany->id)
			{
				struct Element *nowy = (struct Element*)malloc(sizeof(struct Element));
				nowy->element = kontaktDodawany;
				if(tmp_prev == nullptr)
				{
					nowy->next = grupa->first;
					grupa->first = nowy;
				}
				else
				{
					tmp_prev->next = nowy;
					nowy->next = tmp;
				}
				return 1;
			}

			tmp_prev = tmp;
			tmp = tmp->next;

		}

		//trace(kontaktDodawany->id)
		struct Element *nowy = (struct Element*)malloc(sizeof(struct Element));
		nowy->element = kontaktDodawany;
		nowy->next = nullptr;
        tmp_prev->next = nowy;
	}
	return 1;
}

///Funkcja usuwa kontakt grupy poprzez usuniecie wezla Element z listy jednokierunkowej grupy
///Wyszukuje wezel Element majacy wskaznik do struktury Kontakt o zadanym ID
///@param wskaznik do grupy, z ktorej usuwamy element o podanym ID
///@param id usuwanego wezla Element
///@return 1 gdy usunieto, 0, gdy nie usunieto
int usunZGrupy(struct Grupa* grupa, int id)
{
	struct Element* tmp = grupa->first;
	struct Element* tmp_prev = nullptr;
	while(tmp != nullptr)
	{
		if(tmp->element->id == id)
		{
			if(tmp_prev == nullptr)
				grupa->first = grupa->first->next;
			else
				tmp_prev->next = tmp->next;
			free(tmp);
			return 1;
		}
		else if(tmp->element->id > id)
			break;


		tmp_prev = tmp;
		tmp = tmp->next;
	}
	return 0;		//nie usunieto, bo nie znaleziono
}

///Funkcja usuwa ze wszystkich grup element o zadanym ID
///Funkcja wywolywana, gdy usuwamy s listy element o podanym ID
///@param tablica wskaznikow do grup
///@param aktualna ilosc grup
///@param id usuwanego elementu z grupy
void usunZeWszystkichGrupID(struct Grupa* grupy[ILOSC_GRUP_MAX], int iloscGrup, int id)
{
	for(int i = 0; i < iloscGrup; ++i)
		usunZGrupy(grupy[i], id);
}

///Funkcja zapisuje do pliku, wszystkie grupy
///Kazda linijka w pliku stanowi nazwe grupy wraz z numerami ID elementow grupy
///@param nazwa pliku grupy
///@param tablica wskkaznikow do istniejacych grup
///@param aktualne ilosc istniejscych grup
///@return -1, gdy nie otwarto pliku; 0, gdy grupy zapisano do pliku
int zapiszGrupyDoPliku(const char* grupyPlik, struct Grupa* grupy[ILOSC_GRUP_MAX], int iloscGrup)
{
	FILE *fd = fopen(grupyPlik, "w");
	if(fd == nullptr)
	{
		printf("Blad otwarcia pliku grup: %s\n", grupyPlik);
		return -1;
	}
	for(int i = 0; i < iloscGrup; ++i)
	{
		fprintf(fd, "%s", grupy[i]->nazwaGrupy);
		struct Element *tmp = grupy[i]->first;
		while(tmp != nullptr)
		{
			fprintf(fd, " %d", tmp->element->id);
			tmp = tmp->next;
		}
		fputc('\n', fd);
	}
	fclose(fd);
	return 0;
}

///Funkcja tworzy dynamiczne listy grup zapisane w tablicy wskaznikow na podstawie wczytanej juz wczesniej listy Kontaktow z pliku
///@param nazwa pliku z grupami
///@param tablica wskaznikow do tworzonych grup
///@param wskaznik do aktualizowanej liczby grup
///@param wskaznik do listy kontaktow w ktorej szukamy adresow kontaktow skojarzonych z ID z pliku grup
///@return -1, gdy nie mozna otworzyc pliku z zapisanymi grupami; 0, gdy usalo sie odczytac plik z grupami
int wczytajGrupeZpliku(const char* grupyPlik, struct Grupa* grupy[ILOSC_GRUP_MAX], int *iloscGrup, struct Kontakt *head)
{
	FILE *fd = fopen(grupyPlik, "r");
	if(fd == nullptr)
	{
		printf("Blad otwarcia pliku grup: %s\n", grupyPlik);
		return -1;
	}
	char buff[32];
	while(fscanf(fd, "%s", buff) > 0)
	{
		int id = atoi(buff);
		if(id == 0)					// nie powiodlo sie
		{
			struct Grupa *nowa = (struct Grupa*)malloc(sizeof(struct Grupa));
			nowa->first = nullptr;
			strcpy(nowa->nazwaGrupy, buff);
			grupy[*iloscGrup] = nowa;
			++(*iloscGrup);
		}
		else
		{
			struct Kontakt *dodawany = znajdz(head->next, id);
			if(dodawany != nullptr)
				dodajDoGrupy(grupy[*iloscGrup - 1], dodawany);
		}
	}
	fclose(fd);
	return 0;
}
