#include <iostream>
#include <fstream>

#include "tipos.h"
#include "campo.h"
#include "drone.h"
#include "sistema.h"

using namespace std;


int main(int argn, char* args[]) {
    sistema S;
    cout << "Ingrese el valor:";
    cin >> S;


    cout << "Resultado:" << endl;
    offcout << S.Sistema();

    return 0;
}