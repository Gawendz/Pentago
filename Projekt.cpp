#include <iostream>
#include <cstdlib>
#include <vector>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <chrono>

using namespace std;

vector<vector<char>> poloczenie_planszy(vector<vector<vector<char>>*>* plansza) {               //funkcja do połączenia 4 ćwiartek w jedną planszę zwraca nową planszę, przyjmuje wskaźnik na planszę z odzielonymi ćwiartkami
    vector<vector<char>> new_plansza{};                                 //zmienna przechowująca nową macierz
    vector<char> new_row{};
    for (int row = 0; row < 3; row++) {                       //połączenie dwóch górnych wierszy
        new_row.erase(new_row.begin(), new_row.end());
        for (int q = 0; q < 2; q++) {                       //iteracja po 2 pierwszych ćwiartkach
            for (int col = 0; col < 3; col++) {
                new_row.push_back((*(*plansza)[q])[row][col]);
            }
        }
        new_plansza.push_back(new_row);
    }
    for (int row = 0; row < 3; row++) {                       //połączenie dwóch dolnych wierszy
        new_row.erase(new_row.begin(), new_row.end());
        for (int q = 2; q < 4; q++) {                   //iteracja po 2 kolejnych ćwiartkach
            for (int col = 0; col < 3; col++) {
                new_row.push_back((*(*plansza)[q])[row][col]);
            }
        }
        new_plansza.push_back(new_row);
    }
    return new_plansza;
}



void obrot_planszy(vector<vector<char>>* plansza, char side) {          //funkcja do obracania planszy przyjmuje wskaźnik na jedeną ćwiartkę planszy oraz stronę w która ma być obrót
    vector<vector<char>> wektor_kol{};                                    // zmiennna przechowująca wszystkie kolumny
    vector<char> jedna_kol{};                        // zmiennna przechowująca kolumnę
    int size = (*plansza).size();
    if (side == 'x') {                                          //warunek dla obrotu w lewo
        for (int col = size - 1; col >= 0; col--) {                //pętla warunkująca iterację od najdalszego elementu.
            jedna_kol.erase(jedna_kol.begin(), jedna_kol.end());  //wymazanie wartości z wektora kolumnowego
            for (int row = 0; row < size; row++) {
                jedna_kol.push_back((*plansza)[row][col]);              //dodanie jednej wartości do wektora kolumny
            }
            wektor_kol.push_back(jedna_kol);                                 //dodanie kolumny do wektora kolumn
        }
        for (int i = 0; i < size; i++) {
            (*plansza)[i] = wektor_kol[i];                                     //obrót planszy (kolumny stają się wierszami)
        }
    }

    if (side == 'z') {                                          //warunek dla obrotu w prawo (analogicznie do poprzedniego, różnica w iterowaniu po wierszach macierzy)
        for (int col = 0; col < size; col++) {
            jedna_kol.erase(jedna_kol.begin(), jedna_kol.end());
            for (int row = size - 1; row >= 0; row--) {
                jedna_kol.push_back((*plansza)[row][col]);
            }
            wektor_kol.push_back(jedna_kol);
        }
        for (int i = 0; i < size; i++) {
            (*plansza)[i] = wektor_kol[i];
        }
    }
}
const bool wygrana_w_wierszach(vector<vector<char>>* plansza, int gra) {                // warunek wygrania w wierszu
    vector<char> temp_v{};

    if (gra == 2) {                                                             //warunek dla gry Pentago
        vector<char> red_win{ 'C', 'C', 'C', 'C', 'C' };                        //warunek wygrania czerwonego
        vector<char> blue_win{ 'N', 'N', 'N', 'N', 'N' };                        //warunek wygrania niebieskiego
        for (int row = 0; row < (*plansza).size(); row++) {
            vector<char>::iterator red = search((*plansza)[row].begin(), (*plansza)[row].end(), red_win.begin(), red_win.end());        //szuka nam sekwencji wygranej dla czerwonego w każdym wierszu, analogicznie dla niebieskego poniżej, jeśli znajdzie to zwraca nam iterator na pierwszy element znalezionego ciagu, jesli nie to iterator na koniec.
            vector<char>::iterator blue = search((*plansza)[row].begin(), (*plansza)[row].end(), blue_win.begin(), blue_win.end());

            if (red != (*plansza)[row].end()) {
                cout << "Wygrywa czerwony gracz";
                return true;
            }

            if (blue != (*plansza)[row].end()) {
                cout << "Wygrywa niebieski gracz";
                return true;
            }

        }
        return false;
    }

    if (gra == 1) {                                             // analogicznie do gry w Pentago
        vector<char> red_win{ 'X', 'X', 'X', 'X', 'X','X' };
        vector<char> blue_win{ '0','O', 'O', 'O', 'O', 'O' };
        for (int row = 0; row < (*plansza).size(); row++) {
            vector<char>::iterator x = search((*plansza)[row].begin(), (*plansza)[row].end(), red_win.begin(), red_win.end());
            vector<char>::iterator o = search((*plansza)[row].begin(), (*plansza)[row].end(), blue_win.begin(), blue_win.end());

            if (x != (*plansza)[row].end()) {
                cout << "Wygrywa x";
                return true;
            }

            if (o != (*plansza)[row].end()) {
                cout << "Wygrywa o";
                return true;
            }
        }
        return false;
    }
}

const bool wygrana_w_kolumnach(vector<vector<char>>* plansza, int gra) {         //warunek na wygranie w kolumnie
    vector<vector<char>> wektor_kol{};
    vector<char> jedna_kol{};
    vector<vector<char>> tymczas_plansza{};

    for (int i = 0; i < 6; i++) {
        tymczas_plansza.push_back((*plansza)[i]);                       //kopia danej tablicy tak aby nie zmieniać jej w oryginale
    }

    for (int col = 0; col < 6; col++) {                         // pętle zamieniające kolumny na wiersze
        jedna_kol.erase(jedna_kol.begin(), jedna_kol.end());
        for (int row = 5; row >= 0; row--) {
            jedna_kol.push_back(tymczas_plansza[row][col]);
        }
        wektor_kol.push_back(jedna_kol);
    }
    for (int i = 0; i < 6; i++) {
        tymczas_plansza[i] = wektor_kol[i];
    }
    bool wygrana = wygrana_w_wierszach(&tymczas_plansza, gra);               //po zmianie kolumn na wiersze wykorzystujemy wcześniej napisaną funkję na warunek wygranej w wierszach
    return wygrana;
}

const bool wygrana_na_skosach(vector<vector<char>>* plansza, int gra) {         //funkcja sprawdzenia wygranej na skos
    vector<char>tymczas_pierwsza_przek{};                                // zmienne przechowujące wektor wartości na skosach oraz na przekątnych oraz na skosach wyżej oraz poniżej przekątnej
    vector<char> tymczas_druga_przek{};
    vector<char> tymczas_wek_1{};
    vector<char> tymczas_wek_2{};
    vector<char> tymczas_wek_3{};
    vector<char> tymczas_wek_4{};
    vector<vector<char>> tem_plansza_1{};
    vector<vector<char>> tem_plansza_2{};

    for (int i = 0; i < 6; i++) {                                 //tworzymy macierz z 2 przekątnych a nastepnie sprawdzamy je w funkcji do sprawdzania wierszy
        tymczas_pierwsza_przek.push_back((*plansza)[i][i]);
        tymczas_druga_przek.push_back((*plansza)[5 - i][i]);
    }
    tem_plansza_1.push_back(tymczas_pierwsza_przek);
    tem_plansza_1.push_back(tymczas_druga_przek);
    bool wygrana_1 = wygrana_w_wierszach(&tem_plansza_1, gra);

    if (wygrana_1) {                  //jeżli na skosach są wygrane to zwraca nam wygraną
        return wygrana_1;
    }

    if (not(wygrana_1)) {                //jeżeli nie ma wygranej z przekątnej to sparwdzamy na 4 skosach gdzie jest możliwa wygrana, analogiczie z wektorów stworzonych z wartości na konkretnym skosie tworzymy macierz i później sprawdzamy w funkcji z wygraną w wierszach.
        for (int i = 1; i < 6; i++) {
            tymczas_wek_1.push_back((*plansza)[i][i - 1]);
            tymczas_wek_2.push_back((*plansza)[i - 1][i]);
            tymczas_wek_3.push_back((*plansza)[5 - i][i - 1]);
            tymczas_wek_4.push_back((*plansza)[6 - i][i]);
        }
        tem_plansza_2.push_back(tymczas_wek_1);
        tem_plansza_2.push_back(tymczas_wek_2);
        tem_plansza_2.push_back(tymczas_wek_3);
        tem_plansza_2.push_back(tymczas_wek_4);
        bool wygrana_2 = wygrana_w_wierszach(&tem_plansza_2, gra);
        return wygrana_2;
    }

}

const bool wygrana(vector<vector<vector<char>>*>* plansza_, int gra) {             //funkcja do sprawdzenia warunków wygranej w kazdym wariancie, zebrane do jednej aby ławtiej później było sprawdzać
    vector<vector<char>> new_borad = poloczenie_planszy(plansza_);
    bool wygrana_1 = wygrana_w_wierszach(&new_borad, gra);
    bool wygrana_2 = wygrana_w_kolumnach(&new_borad, gra);
    bool wygrana_3 = wygrana_na_skosach(&new_borad, gra);

    if (wygrana_1) {
        return wygrana_1;
    }

    if (wygrana_2) {
        return wygrana_2;
    }

    if (wygrana_3) {
        return wygrana_3;
    }
    else
        return false;
}
int dodanie_zetonu(vector<vector<char>>* plansza, int numer, string gracz, int gra, int tura_) {      //funkcja wstawiająca na planszę żetony, przyjmuje wskaźnik na ćwiartkę macierzy do której chcemy dodać element, numer miejsca, gracza, grę w którą gramy oraz numer tury
    if ((*plansza)[(9 - numer) / 3][(numer - 1) % 3] == ' ') {                                  //warunek gwarantujący brak nadpisania czyjegoś ruchu
        if (gra == 1) {                                                                    //wstawienie wartości dla gry kólo i krzyżyk
            if (gracz == "x" or gracz == "X") {
                (*plansza)[(9 - numer) / 3][(numer - 1) % 3] = 'X';
            }
            if (gracz == "o" or gracz == "O") {
                (*plansza)[(9 - numer) / 3][(numer - 1) % 3] = 'O';
            }
            return tura_;
        }
        if (gra == 2) {                                                        // warunek dla gry w Pentago
            if (gracz == "czerwony_gracz") {
                (*plansza)[(9 - numer) / 3][(numer - 1) % 3] = 'C';
            }
            if (gracz == "niebieski_gracz") {
                (*plansza)[(9 - numer) / 3][(numer - 1) % 3] = 'N';
            }
            return tura_;
        }
    }
    else {                                                                       //warunek gdy ktoś chce nadpisac pole, zwraca tura-1, ponieważ gracz który wykonał taki ruch musi ponownie rozegrać swoją turę
        cout << "To pole jest juz zajete\n";
        return tura_--;
    }
}

const void Wypisanie_na_konsoli(string* pierwszy_gracz, string* drugi_gracz, vector<vector<vector<char>>*>* plansza, int gra, int tura) {    //funkcja wypisująca nam planszę z aktualizacją danych na niej
    const char rowna_podwojna = 205;                  //zmienne zapisane w cp852, przechowujący dany kształ
    const char pion_podwojna = 186;
    const char rog_gora_lewy = 201;
    const char rog_gora_prawy = 187;
    const char rog_dol_lewy = 200;
    const char rog_dol_prawy = 188;
    const char pion = 179;
    const char pion_roz_prawy = 204;
    const char pion_roz_lewy = 185;

    if (gra == 1) {                                      // wybór planszy oraz nazwy graczy w zależnosci w co gramy
        cout << setw(15) << "KOLKO I KRZYZYK\n";
        for (int i = 0; i < 25; i++)                 //pętla rysujaca nam linie podwójną
            cout << rowna_podwojna;
        cout << "\n";

        cout << setw(15) << "GRACZE\n";           //wypisanie imion graczy i zaznaczenie czym grają
        cout << "Gracz o: " + *pierwszy_gracz + "\n";
        cout << "Gracz x: " + *drugi_gracz + "\n";
    }

    if (gra == 2) {                              // analogicznie do powyższego warunku dla Pentago
        cout << setw(15) << "PENTAGO\n";
        for (int i = 0; i < 25; i++)
            cout << rowna_podwojna;
        cout << "\n";

        cout << setw(15) << "GRACZE\n";
        cout << "Gracz czerwony: " + *drugi_gracz + "\n";
        cout << "Gracz niebieski: " + *pierwszy_gracz + "\n";
    }

    vector<vector<char>> new_plansza = poloczenie_planszy(plansza);              //stworzenie planszy tak aby nie składała sie z czterech fragmentów

    for (int i = 0; i < 25; i++)             //pętla rysujaca nam linie podwójną
        cout << rowna_podwojna;
    cout << "\n";

    cout << setw(15) << "BOARD\n";
    cout << rog_gora_lewy;               //poniższa pętla rysuje nam górną część planszy
    for (int i = 1; i < 24; i++)
        cout << rowna_podwojna;
    cout << rog_gora_prawy << "\n";

    for (int row = 0; row < 4; row++) {     //wpisanie na plansze wartosci z macierzy oraz wykoananie środka
        if (row == 3) {                     //warunek, dzięki któremu rysujemy podwójną linie odzielająca górną częsc planszy od dolnej
            cout << pion_roz_prawy;
            for (int i = 1; i < 24; i++)
                cout << rowna_podwojna;
            cout << pion_roz_lewy << endl;
        }
        else {                                  //warunek wypisujący wartosci dla dwóch górnych macierzy
            for (int col = 0; col < 6; col++) {
                if (col == 0 or col == 3) {
                    cout << pion_podwojna << setw(3) << new_plansza[row][col];
                }
                else
                    cout << pion << setw(3) << new_plansza[row][col];
            }
            cout << pion_podwojna << "\n";
        }
    }
    for (int row = 3; row < 7; row++) {                 //analogicznie do powyższego ale dla dwóch dolnych macierzy
        if (row == 6) {         //warunek tworzący nam dolną część ramki
            cout << rog_dol_lewy;
            for (int i = 1; i < 24; i++)
                cout << rowna_podwojna;
            cout << rog_dol_prawy << endl;
        }
        else {      //warunek wypisujący wartości z macierzy
            for (int col = 0; col < 6; col++) {
                if (col == 0 or col == 3) {
                    cout << pion_podwojna << setw(3) << new_plansza[row][col];
                }
                else
                    cout << pion << setw(3) << new_plansza[row][col];
            }
            cout << pion_podwojna << "\n";
        }
    }
    if (gra == 1) {                  //dwa warunki wypisujące nam, czyja tura jest w zaleznosci od gry.(Zaczyna gracz O oraz czerwony)
        if (tura % 2 == 0) {
            cout << "Tura X\n";
        }
        else
            cout << "Tura O\n";
    }

    if (gra == 2) {
        if (tura % 2 == 0) {
            cout << "Tura gracza czerwonego\n";
        }
        else
            cout << "Tura gracza niebieskiego\n";
    }

}


int main() {            //funkcja główna odpowiedzialna za poprawne działenie programu
    int gra;
    cout << "Wybierz gre:\n 1-> Kolko i krzyzyk \n 2-> Pentago\n";             //wybór w którą gre chcesz zagrać, 1:kółko i krzyżyk, 2:pentago
    cin >> gra;
    string pierwszy_gracz{};
    string drugi_gracz{};
    double czas_pierwszy = 300;                    //czas 5 min na całą grę
    double czas_drugi = 300;

    int tura_ = 0;
    char ruch;
    char wyjscie_z_opcji;
    string gracz;
    char numer;
    int int_numer;
    string cwiartka;


    vector<vector<char>> up_left_plansza{ {' ', ' ', ' '},
                                         {' ', ' ', ' '},
                                         {' ', ' ', ' '} };

    vector<vector<char>> up_right_plansza{ {' ', ' ', ' '},
                                          {' ', ' ', ' '},
                                          {' ', ' ', ' '} };

    vector<vector<char>> down_left_plansza{ {' ', ' ', ' '},
                                           {' ', ' ', ' '},
                                           {' ', ' ', ' '} };

    vector<vector<char>> down_right_plansza{ {' ', ' ', ' '},
                                            {' ', ' ', ' '},
                                            {' ', ' ', ' '} };
    vector<vector<vector<char>>*> plansza{ &up_left_plansza, &up_right_plansza, &down_left_plansza, &down_right_plansza };


    if (gra == 1) {                        //warunek, umożliwia dopisanie imion do konkretnych żetonów w zależności od gry
        cout << "Imie gracza o: \n";
        cin >> pierwszy_gracz;
        cout << "Imie gracza x: \n";
        cin >> drugi_gracz;
    }

    if (gra == 2) {
        cout << "Imie gracza czerwonego: \n";
        cin >> pierwszy_gracz;
        cout << "Imie gracza niebieskiego: \n";
        cin >> drugi_gracz;
    }


     //stworzenie pełnej planszy
    Wypisanie_na_konsoli(&pierwszy_gracz, &drugi_gracz, &plansza, gra, tura_);                          //wyświetlenie pustej planszy

    cout << setw(13) << "HELP\n";         //wypisanie wszystkich opcji programu
    cout << "q: Lewa gorna plansza\n";
    cout << "w: Prawa gorna plansza\n";
    cout << "a: Lewa dolna plansza\n";
    cout << "s: Prawa dolna plansza\n";
    cout << "1-9: Wybor czesci planszy (klawiatura numeryczna)\n";
    cout << "z: Obrot czesci planszy w prawo\n";
    cout << "x: Obrot czesci planszy w lewo\n";
    cout << "p: pauza\n";
    cout << "o: wczytanie predefiniowanej planszy\n";
    cout << "m: opcje\n";
    cout << "h: opis programu\n";


    while (not(wygrana(&plansza, gra))) {                         //pętla umożliwająca grę, aż któryś z graczy nie wygra
        auto begin = chrono::high_resolution_clock::now(); //rozpoczęcie liczenia czasu na turę
        tura_++;             //zwiększenie tury, dzięki temu wiemy który gracz ma teraz grać, poniższe dwa warunki przypisują wartości do zmiennej gracz, tak aby później funkcje wiedziały, kto teraz gra
        if (gra == 1) {
            if (tura_ % 2 == 0)
                gracz = "O";
            else
                gracz = "X";
        }

        if (gra == 2) {
            if (tura_ % 2 == 0)
                gracz = "niebieski_gracz";
            else
                gracz = "czerwony_gracz";
        }


        cout << "Wykonaj ruch: \n";            //wykonanie ruchu, możliwość wyboru m,q,w,a,s,p
        cin >> ruch;

        if (ruch == 'm') {                  //wypisuje nam menu gry
            cout << setw(15) << "HELP\n";
            cout << "q: Lewa gorna plansza\n";
            cout << "w: Prawa gorna plansza\n";
            cout << "a: Lewa dolna plansza\n";
            cout << "s: Prawa dolna plansza\n";
            cout << "1-9: Wybor czesci planszy (klawiatura numeryczna)\n";
            cout << "z: Obrot czesci planszy w prawo\n";
            cout << "x: Obrot czesci planszy w lewo\n";
            cout << "p: pauza\n";
            cout << "o: wczytanie predefiniowanej planszy\n";
            cout << "m: opcje\n";
            cout << "h: opis programu\n";
            tura_--;                     //pozwala na wykonanie ruchu jeszcze raz przez gracza, który włączył menu
        }
        if (ruch == 'p') {                                                    // waruek: pauza
            const char rowna_podwojna = 205;
            auto end = chrono::high_resolution_clock::now();                           //zatrzymanie czasu
            auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);       //zmienna przechowująca ilość czasu

            if (tura_ % 2 == 0) {                                                        //wyliczenie czasu dla gracza niebieskiego
                czas_drugi -= elapsed.count() * 1e-9;
                cout << "Pozostaly czas: " << czas_drugi << endl;
            }

            else {
                czas_pierwszy -= elapsed.count() * 1e-9;                                   //wyliczenie czasu dla gracza czerwonego
                cout << "Pozostaly czas:  " << czas_pierwszy << endl;
            }
            for (int i = 0; i < 25; i++)
                cout << rowna_podwojna;
            cout << "\n";

            cout << setw(15) << "PAZUA\n";

            for (int i = 0; i < 25; i++)
                cout << rowna_podwojna;
            cout << "\n";
            tura_--;
            cin >> ruch;
            auto begin = chrono::high_resolution_clock::now();                     //ponowane włączenie zegara

        }
        if (ruch == 'o') {                                                    //plansza predefiniowana
            dodanie_zetonu(&up_left_plansza, 8, "niebieski_gracz", gra, tura_);
            dodanie_zetonu(&up_left_plansza, 6, "niebieski_gracz", gra, tura_);
            dodanie_zetonu(&up_left_plansza, 4, "czerwony_gracz", gra, tura_);
            dodanie_zetonu(&up_left_plansza, 5, "czerwony_gracz", gra, tura_);
            dodanie_zetonu(&up_left_plansza, 2, "czerwony_gracz", gra, tura_);
            dodanie_zetonu(&up_right_plansza, 8, "czerwony_gracz", gra, tura_);
            dodanie_zetonu(&up_right_plansza, 4, "czerwony_gracz", gra, tura_);
            dodanie_zetonu(&up_right_plansza, 5, "niebieski_gracz", gra, tura_);
            dodanie_zetonu(&down_left_plansza, 7, "czerwony_gracz", gra, tura_);
            dodanie_zetonu(&down_left_plansza, 8, "czerwony_gracz", gra, tura_);
            dodanie_zetonu(&down_left_plansza, 6, "czerwony_gracz", gra, tura_);
            dodanie_zetonu(&down_left_plansza, 4, "niebieski_gracz", gra, tura_);
            dodanie_zetonu(&down_left_plansza, 2, "niebieski_gracz", gra, tura_);
            dodanie_zetonu(&down_right_plansza, 2, "niebieski_gracz", gra, tura_);
            dodanie_zetonu(&down_right_plansza, 3, "niebieski_gracz", gra, tura_);
            dodanie_zetonu(&down_right_plansza, 5, "niebieski_gracz", gra, tura_);
            Wypisanie_na_konsoli(&pierwszy_gracz, &drugi_gracz, &plansza, gra, 1);
            tura_++;
        }
        if (ruch == 'q') {      //wybór ćwiartki
            cout << "Podaj numer komorki 1-9: ";
            cin >> numer;
            int_numer = int(numer) - int('0');
            tura_ = dodanie_zetonu(&up_left_plansza, int_numer, gracz, gra, tura_);         //wpisanie odpowiedniej wartosci na planszę
            if (gra == 2) {            //dla gry w pentago możliwość obrotu planszy
                cout << "podaj cwiartke ktora chcesz obrocic i w ktora strone: ";
                cin >> cwiartka;
                if (cwiartka[0] == 'q')
                    obrot_planszy(&up_left_plansza, cwiartka[1]);
                if (cwiartka[0] == 'w')
                    obrot_planszy(&up_right_plansza, cwiartka[1]);
                if (cwiartka[0] == 'a')
                    obrot_planszy(&down_left_plansza, cwiartka[1]);
                if (cwiartka[0] == 'a')
                    obrot_planszy(&down_right_plansza, cwiartka[1]);

            }
            Wypisanie_na_konsoli(&pierwszy_gracz, &drugi_gracz, &plansza, gra, tura_); //wypisanie planszy, poniższe funkcje działają analogicznie
        }

        if (ruch == 'w') {
            cout << "Podaj numer komorki: ";
            cin >> numer;
            int_numer = int(numer) - int('0');
            tura_ = dodanie_zetonu(&up_right_plansza, int_numer, gracz, gra, tura_);
            if (gra == 2) {
                cout << "podaj cwiartke ktora chcesz obrocic i w ktora strone: ";
                cin >> cwiartka;
                if (cwiartka[0] == 'q')
                    obrot_planszy(&up_left_plansza, cwiartka[1]);
                if (cwiartka[0] == 'w')
                    obrot_planszy(&up_right_plansza, cwiartka[1]);
                if (cwiartka[0] == 'a')
                    obrot_planszy(&down_left_plansza, cwiartka[1]);
                if (cwiartka[0] == 'a')
                    obrot_planszy(&down_right_plansza, cwiartka[1]);
            }
            Wypisanie_na_konsoli(&pierwszy_gracz, &drugi_gracz, &plansza, gra, tura_);
        }
        if (ruch == 'a') {
            cout << "Podaj numer komorki: ";
            cin >> numer;
            int_numer = int(numer) - int('0');
            tura_ = dodanie_zetonu(&down_left_plansza, int_numer, gracz, gra, tura_);
            if (gra == 2) {
                cout << "podaj cwiartke ktora chcesz obrocic i w ktora strone: ";
                cin >> cwiartka;
                if (cwiartka[0] == 'q')
                    obrot_planszy(&up_left_plansza, cwiartka[1]);
                if (cwiartka[0] == 'w')
                    obrot_planszy(&up_right_plansza, cwiartka[1]);
                if (cwiartka[0] == 'a')
                    obrot_planszy(&down_left_plansza, cwiartka[1]);
                if (cwiartka[0] == 'a')
                    obrot_planszy(&down_right_plansza, cwiartka[1]);

            }
            Wypisanie_na_konsoli(&pierwszy_gracz, &drugi_gracz, &plansza, gra, tura_);
        }


        if (ruch == 's') {
            cout << "Podaj numer komorki: ";
            cin >> numer;
            int_numer = int(numer) - int('0');
            tura_ = dodanie_zetonu(&down_right_plansza, int_numer, gracz, gra, tura_);
            if (gra == 2) {
                cout << "podaj cwiartke ktora chcesz obrocic i w ktora strone: ";
                cin >> cwiartka;
                if (cwiartka[0] == 'q')
                    obrot_planszy(&up_left_plansza, cwiartka[1]);
                if (cwiartka[0] == 'w')
                    obrot_planszy(&up_right_plansza, cwiartka[1]);
                if (cwiartka[0] == 'a')
                    obrot_planszy(&down_left_plansza, cwiartka[1]);
                if (cwiartka[0] == 'a')
                    obrot_planszy(&down_right_plansza, cwiartka[1]);
            }
            Wypisanie_na_konsoli(&pierwszy_gracz, &drugi_gracz, &plansza, gra, tura_);
        }
        if (ruch == 'h') {
            cout << "Pentago jest gra dwuosobowa na planszy. Plansza sklada sie z 4 ruchomych czesci, ktore mozna obracac.\n";
            cout << "Kazda cwiartka zawiera 9 miejsc na zetony.\n";
            cout << "Gracze po kolei oddaja ruch skladajacy sie z 2 czesci: \n";
            cout << "-dolozenia swojego zetonu na niezajete pole planszy\n";
            cout << "-obrot jednej z czesci planszy o 90 stopni w dowolnym kierunku\n";
            cout << "Nie mozna zrezygnowac z zadnej czesci ruchu.\n";
            cout << "Wygrywa osoba, ktorej 5 zetonow, po pelnym ruchu, sa ulozone w rzedzie / kolumnie / po skosach(podobnie do gry kolko i krzyzyk).\n"; 
        }



        auto end = chrono::high_resolution_clock::now();                           //zakończenie czasu
        auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);       //czas tury

        if (tura_ % 2 == 0) {                                                        //wyliczenie pozostałego czasu dla graczy
            czas_drugi -= elapsed.count() * 1e-9;
            cout << "Pozostaly czas: " << czas_drugi << endl;
        }
        else {
            czas_pierwszy -= elapsed.count() * 1e-9;
            cout << "Pozostaly czas:  " << czas_pierwszy << endl;
        }

        wygrana(&plansza, gra);
    }
    return 0;
}