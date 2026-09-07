#include <iostream>
#include <string>
#include "scanner.h"
#include "shared.h"

using namespace std;

int main() {
    cout << "ExtreamFS - Sistema de archivos EXT2" << endl;
    cout << "Ingrese comandos (exit para salir):" << endl;
    cout << endl;

    string line;
    while (true) {
        cout << ">> ";
        if (!getline(cin, line)) {
            // fin de archivo
            break;
        }

        string trimmed = trim(line);
        if (to_lower(trimmed) == "exit") {
            cout << "Saliendo..." << endl;
            break;
        }

        process_line(line);
        cout << endl;
    }

    return 0;
}
