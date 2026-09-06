#ifndef SHARED_H
#define SHARED_H

#include <string>
#include <vector>

using namespace std;

// funciones utilitarias comunes
string to_upper(const string& s);
string to_lower(const string& s);
bool compare_ci(const string& a, const string& b);
string trim(const string& s);

#endif
