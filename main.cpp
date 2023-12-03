#include <iostream>
#include "CUserInterface.h"
int main() {

    cout << "Choose the type of the tree:" << endl;
    cout << "1 - Int" << endl;
    cout << "2 - Double" << endl;
    cout << "3 - String" << endl;
    int i_input;
    cin >> i_input;

    if(i_input==1){
        CUserInterface<int> c_ui;
        c_ui.vRun();
    }
    else if(i_input==2){
        CUserInterface<double> c_ui;
        c_ui.vRun();
    }
    else if(i_input==3){
        CUserInterface<string> c_ui;
        c_ui.vRun();
    }

    else{
        cout << "Wrong input was given" << endl;
    }

//    CTree<int> c_tree("+ 1 2");
//    return 0;
}

/// zmiany kiedy jest zamiana przy uzyciu atoi: Interfejs i Tree (done)
/// napisac opcje wyboru typu w menu (done)
/// zgeneryzowac wartosc podstawiana przy naprawie drzewa (done)
/// dzialania na string nie dziala dla pustego znaku "" (done)
// zabezpieczyc program przed wpisaniem compa na poczatku
// pozamieniac stringi i wartosci na stale
// dodac zeby sinus i cosinus byly nazwami zmiennych a nie operacjami dla string

////////////////////////////////////////////////////

// Jesli chcielibysmy dodac nowy typ to:
// sprawdzamy czy trzeba zmienic funkcje chceckDivisionByZero
// piszemy implementacje funkcji getValueFromString (dla drzewa i interfejsu)
// piszemy implementacje bIsTTypeValue
// piszemy odpowiednie uzupenianie drzewa przy naprawie


