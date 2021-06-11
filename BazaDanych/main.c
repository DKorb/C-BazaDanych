#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <limits.h>					/// INT_MAX, INT_MIN
#include <ctype.h>					/// funkcja isblank

#include "funkcje.h"


int wejscieDobazy()
{
	const char *nazwaPliku = "baza.txt"; // Przechowuje nazwe pliku z baza
	const char *plikGrupy = "grupy.txt";

	int id;
    char tmpImie[DLUGOSC_NAZWY + 1]; // Przechowuje imie(pomocniczo)
    char tmpNazwisko[DLUGOSC_NAZWY + 1]; // Przechowuje nazwisko(pomocniczo)

	char tmpKodPocztowy[MAX_KOD + 1];
    char tmpMiasto[MAX_MIASTO + 1];
    char tmpUlica[MAX_ULICA + 1];
    char tmpNumerDomu[MAX_DOM + 1];
    int numerMieszkania;


    char tmpTelefon[DLUGOSC_TELEFONU + 1]; // Przechowuje telefon(pomocniczo)
    char tmpEmail[DLUGOSC_EMAIL + 1]; // Przechowuje emial(pomocniczo)

    struct Kontakt *head = malloc(sizeof(struct Kontakt)); // Pierwszy element listy - glowa
    inicjalizacjaListy(head);

    // kazda grupa jest lista wskaznikow do Kontaktu
    struct Grupa* grupy[ILOSC_GRUP_MAX];			// tablica wskaznikow do grup, zakladamy maksymalnie ILOSC_GRUP_MAX grup
    int iloscGrup = 0;


    int klawisz = -1;
    while(TRUE) { //menu
        printf("\nBAZA KONTAKTOW\n");
        puts("Opcje:");
        puts("1 -> DODAJ KONTAKT DO LISTY");
        puts("2 -> USUN KONTAKT Z LISTY PO ID");
        puts("3 -> WYPISZ ZAWARTOSC LISTY");
        //-----------------------------------------------
        puts("5 -> PRZENIES ZAWARTOSC PLIKU DO LISTY");
        puts("6 -> PRZENIES ZAWARTOC LISTY DO PLIKU");
        //-----------------------------------------------
        puts("8 -> USUN KONTAKT Z PLIKU PO ID");
        puts("9 -> WYSWIETL ZAWARTOSC PLIKU");
        puts("10 -> SORTUJ LISTE PO ID");
        puts("11 -> EDYCJA ELEMENTU O PODANYM ID");
        puts("12 -> WYSZUKIWANIE REKORDOW");
        //----------------------------------------------
        puts("13 -> DODAJ GRUPE");
        puts("14 -> WYSWIETL GRUPY");
        puts("15 -> DODAJ KONTAKT DO GRUPY");
        puts("16 -> USUN KONTAKT Z GRUPY");
		puts("17 -> USUN GRUPE");

        puts("0 -> WYJSCIE Z PROGRAMU");
        klawisz = wczytajLiczbe("Wybierz opcje", 17); // opcja 0 - 17

        //opcje menu
        if(klawisz == 1) {
			while(TRUE)
			{
				id = wczytajLiczbe("Podaj id", INT_MAX); // dowolne nieujemne ID
				// sprawdzenie niepowtorzenia ID
				if(znajdz(head, id) == NULL)
					break;

				puts("Powtorzony ID");
			}

			//wczytywanie pol.

			wczytajSekwencje(tmpImie, 3, DLUGOSC_NAZWY, "imie", znakiImieniaNazwiska);
			wczytajSekwencje(tmpNazwisko, 3, DLUGOSC_NAZWY, "nazwisko", znakiImieniaNazwiska);

			wczytajSekwencje(tmpKodPocztowy, 5, MAX_KOD, "kod pocztowy (5 - cyfr)", isdigit);					// 5 cyfr kodu pocztowego
			wczytajSekwencje(tmpMiasto, 3, MAX_MIASTO, "miejscowosc", znakiImieniaNazwiska);					// minimum 3 litery
			wczytajSekwencje(tmpUlica, 3, MAX_ULICA, "ulice", znakiImieniaNazwiska);							// minimum 3 litery
			wczytajSekwencje(tmpNumerDomu, 1, MAX_DOM, "numer domu", isalnum);									// minimum 1 cyfra i litera
			numerMieszkania = wczytajLiczbe("Podaj numer lokalu(0 - brak numeru lokalu)", INT_MAX); // dodolne nieujemne ID

			int iloscNumerow = 0;
			char **telefony = wczytajTelefony(tmpTelefon, &iloscNumerow);

			int iloscEmail = 0;
			char **emaile = wczytajEmaile(tmpEmail, &iloscEmail);


            dodajNaKoniecListy(head, id, tmpImie, tmpNazwisko, tmpKodPocztowy, tmpMiasto, tmpUlica, tmpNumerDomu, numerMieszkania, telefony, iloscNumerow, emaile, iloscEmail);

        }
        else if(klawisz == 2) {
			id = wczytajLiczbe("Podaj id", INT_MAX);
            usunZListyPoID(head, id);
            usunZeWszystkichGrupID(grupy, iloscGrup, id);

        }
        else if(klawisz == 3) {
            wypiszListe(head);
        }
        //---------------------
        else if(klawisz == 5) {
            plikDoListy(head, nazwaPliku, 1);
            skasujGrupy(grupy, &iloscGrup);
            wczytajGrupeZpliku(plikGrupy, grupy, &iloscGrup, head);
        }
        else if(klawisz == 6) {
            listaDoPliku(head, nazwaPliku);
            zapiszGrupyDoPliku(plikGrupy, grupy, iloscGrup);
        }
        else if(klawisz == 7) {
            zwolnijPamiec(head);
        }
        //---------------------
        else if(klawisz == 8) {
            id = wczytajLiczbe("Podaj id", INT_MAX);

            usunZPlikuPoId(id, nazwaPliku);
        }
        else if(klawisz == 9) {
            wypiszZPliku(nazwaPliku);
        }
		else if(klawisz == 10)
		{
			sortujPoID(&head);
		}
		else if(klawisz == 11)
		{
			edytuj(head);
		}
		else if(klawisz == 12)
		{
			wyszukaj(head);
		}
		else if(klawisz == 13)
		{
			utworzGrupe(grupy, &iloscGrup);
		}
		else if(klawisz == 14)						// wyswietlenie grupy
		{
			int numerGrupy = wyborGrupy(grupy, iloscGrup);
			if(numerGrupy > 0)
				wyswietlGrupe(grupy[numerGrupy - 1]);
		}
		else if(klawisz == 15)						// dodanie kontaktu do grupy
		{
			int numerGrupy = wyborGrupy(grupy, iloscGrup);
			if(numerGrupy > 0)
			{
				struct Grupa* gr = grupy[numerGrupy - 1];
				wyswietlGrupe(gr);
				int id;
				while(TRUE)
				{
					id = wczytajLiczbe("Podaj id do dodania(0 - anuluj dodawanie)", INT_MAX); // dowolne nieujemne ID
					if(id == 0)
						break;
					// sprawdzenie niepowtorzenia ID
					struct Kontakt *kontaktDoDodania = znajdz(head, id);
					if(kontaktDoDodania == nullptr)
					{
						printf("Brak kontaktu o ID:%d\n", id);
						continue;
					}

					if(dodajDoGrupy(gr, kontaktDoDodania) > 0)
						puts("Dodano kontakt do grupy");
					else
						puts("Element byl juz w grupie");

					break;
				}
			}
		}
		else if(klawisz == 16)						// usuniecie kontaktu z grupy
		{
		int numerGrupy = wyborGrupy(grupy, iloscGrup);
			if(numerGrupy > 0)
			{
				struct Grupa* gr = grupy[numerGrupy - 1];
				wyswietlGrupe(gr);
				int id = wczytajLiczbe("Podaj id do usuniecia(0 - anuluj dodawanie)", INT_MAX); // dowolne nieujemne ID
				if(id > 0)
				{
					if(usunZGrupy(gr, id) > 0)
						puts("Usunieto kontakt z grupy\n");
					else
						puts("Kontaktu nie bylo w grupie\n");
				}

				wyswietlGrupe(gr);
			}
		}
		else if(klawisz == 17)						// usuniecie grupy
		{
			int numerGrupy = wyborGrupy(grupy, iloscGrup);
			if(numerGrupy > 0)
			{
				usunGrupe(grupy, &iloscGrup, numerGrupy - 1);
				puts("Grupa zostala usunieta!");
			}
		}

        else if(klawisz == 0) {
			skasujGrupy(grupy, &iloscGrup);
            zwolnijPamiec(head);
            free(head);
            return 1;
        }
    }
    skasujGrupy(grupy, &iloscGrup);
    zwolnijPamiec(head);
    free(head);
    return 0;

}

/**
 * Funkcja glowna main
*/
int main()
{

    const char *loginy = "dane.txt";

    int wybor;
    while(wybor != 0) {
        printf("\nLOGOWANIE\n");
        printf("1 -> zaloguj sie\n");
        printf("2 -> zarejestruj sie\n");
        scanf("%d", &wybor);

        while(getchar() != '\n') continue;

        switch(wybor)
		{
			case 1:
				if(zaloguj(loginy) == 1)
					wejscieDobazy();
				break;
			 case 2:
				rejestruj(loginy);
				break;

			default:
				printf("PODAJ WLASCIWA OPCJE");
				Sleep(1200);
				break;
		}
    }
}
