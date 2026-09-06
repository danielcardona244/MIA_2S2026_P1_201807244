#include "scanner.h"
#include "shared.h"
#include "disk.h"
#include "mount.h"

#include <iostream>
#include <sstream>

using namespace std;

// separar tokens por espacios, respetando comillas dobles
vector<string> tokenize(const string& input) {
    vector<string> tokens;
    string current;
    bool in_quotes = false;

    for (int i = 0; i < input.size(); i++) {
        char c = input[i];
        if (c == '"') {
            in_quotes = !in_quotes;
            // no incluir las comillas en el token
        } else if (c == ' ' && !in_quotes) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }
    return tokens;
}

// separar un token tipo "-param=valor" en key y value
pair<string, string> split_param(const string& token) {
    string t = token;

    // quitar el guion inicial
    if (!t.empty() && t[0] == '-') {
        t = t.substr(1);
    }

    size_t eq = t.find('=');
    if (eq == string::npos) {
        // parametro tipo flag (sin valor), como -r o -p
        return {to_lower(t), ""};
    }

    string key = to_lower(t.substr(0, eq));
    string value = t.substr(eq + 1);
    return {key, value};
}

// procesar una sola linea de comando
void process_line(const string& line) {
    string trimmed = trim(line);

    // linea vacia
    if (trimmed.empty()) {
        return;
    }

    // comentario
    if (trimmed[0] == '#') {
        cout << trimmed << endl;
        return;
    }

    // tokenizar
    vector<string> tokens = tokenize(trimmed);
    if (tokens.empty()) return;

    // primer token es el comando
    string cmd = to_lower(tokens[0]);

    // extraer parametros
    map<string, string> params;
    for (int i = 1; i < tokens.size(); i++) {
        auto [key, value] = split_param(tokens[i]);
        if (params.find(key) != params.end()) {
            cout << "[ERROR] parametro " << key << " repetido" << endl;
            return;
        }
        params[key] = value;
    }

    // despachar al comando correspondiente
    if (cmd == "mkdisk") {
        cmd_mkdisk(params);
    } else if (cmd == "rmdisk") {
        cmd_rmdisk(params);
    } else if (cmd == "fdisk") {
        cmd_fdisk(params);
    } else if (cmd == "mount") {
        cmd_mount(params);
    } else if (cmd == "mounted") {
        cmd_mounted();
    } else if (cmd == "exit") {
        // se maneja en main
    } else {
        cout << "[ERROR] comando no reconocido: " << cmd << endl;
    }
}

// procesar bloque de texto completo (varias lineas)
void process_input(const string& text) {
    istringstream stream(text);
    string line;
    while (getline(stream, line)) {
        process_line(line);
    }
}
