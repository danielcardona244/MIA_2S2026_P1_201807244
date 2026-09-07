#include "scanner.h"
#include "shared.h"
#include "disk.h"
#include "mount.h"

#include <iostream>
#include <sstream>
#include <set>
#include <cctype>

using namespace std;

// parametros permitidos por cada comando del sprint 1
static const map<string, set<string>> allowed_params = {
    {"mkdisk",  {"size", "fit", "unit", "path"}},
    {"rmdisk",  {"path"}},
    {"fdisk",   {"size", "unit", "path", "type", "fit", "name"}},
    {"mount",   {"path", "name"}},
    {"mounted", {}},
};

// separar tokens por espacios, respetando comillas dobles
vector<string> tokenize(const string& input) {
    vector<string> tokens;
    string current;
    bool in_quotes = false;

    for (int i = 0; i < (int)input.size(); i++) {
        char c = input[i];
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (isspace(static_cast<unsigned char>(c)) && !in_quotes) {
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
        // flag sin valor, como -r o -p (se usa en comandos futuros)
        return {to_lower(t), ""};
    }

    string key = to_lower(t.substr(0, eq));
    string value = t.substr(eq + 1);
    return {key, value};
}

// procesar una sola linea de comando
void process_line(const string& line) {
    string trimmed = trim(line);

    if (trimmed.empty()) {
        return;
    }

    // comentario
    if (trimmed[0] == '#') {
        cout << trimmed << endl;
        return;
    }

    // detectar comillas sin cerrar
    int quote_count = 0;
    for (char c : trimmed) {
        if (c == '"') quote_count++;
    }
    if (quote_count % 2 != 0) {
        cout << "[ERROR] comillas dobles sin cerrar" << endl;
        return;
    }

    // tokenizar
    vector<string> tokens = tokenize(trimmed);
    if (tokens.empty()) return;

    // primer token es el comando
    string cmd = to_lower(tokens[0]);

    // extraer parametros
    map<string, string> params;
    for (int i = 1; i < (int)tokens.size(); i++) {
        // cada parametro debe iniciar con '-'
        if (tokens[i][0] != '-') {
            cout << "[ERROR] " << to_upper(cmd)
                 << ": parametro debe iniciar con '-': " << tokens[i] << endl;
            return;
        }

        auto [key, value] = split_param(tokens[i]);

        // key vacio: caso "-=10"
        if (key.empty()) {
            cout << "[ERROR] " << to_upper(cmd)
                 << ": parametro mal formado: " << tokens[i] << endl;
            return;
        }

        // tiene '=' pero sin valor: caso "-size="
        bool has_eq = (tokens[i].find('=') != string::npos);
        if (has_eq && value.empty()) {
            cout << "[ERROR] " << to_upper(cmd)
                 << ": parametro sin valor: " << tokens[i] << endl;
            return;
        }

        // parametro repetido
        if (params.find(key) != params.end()) {
            cout << "[ERROR] " << to_upper(cmd)
                 << ": parametro -" << key << " repetido" << endl;
            return;
        }
        params[key] = value;
    }

    // validar parametros permitidos para el comando
    if (allowed_params.count(cmd)) {
        const auto& allowed = allowed_params.at(cmd);
        for (auto& [key, val] : params) {
            if (allowed.find(key) == allowed.end()) {
                cout << "[ERROR] " << to_upper(cmd)
                     << ": parametro no reconocido -" << key << endl;
                return;
            }
            // en Sprint 1 todos los parametros requieren valor (no hay flags)
            if (val.empty()) {
                cout << "[ERROR] " << to_upper(cmd)
                     << ": parametro -" << key << " requiere un valor" << endl;
                return;
            }
        }
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
