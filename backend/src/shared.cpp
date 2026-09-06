#include "shared.h"
#include <algorithm>
#include <locale>

string to_upper(const string& s) {
    string result = s;
    for (int i = 0; i < result.size(); i++) {
        result[i] = toupper(result[i]);
    }
    return result;
}

string to_lower(const string& s) {
    string result = s;
    for (int i = 0; i < result.size(); i++) {
        result[i] = tolower(result[i]);
    }
    return result;
}

// comparacion sin importar mayusculas o minusculas
bool compare_ci(const string& a, const string& b) {
    return to_lower(a) == to_lower(b);
}

string trim(const string& s) {
    int start = 0;
    int end = s.size() - 1;
    while (start <= end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r')) {
        start++;
    }
    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r')) {
        end--;
    }
    if (start > end) return "";
    return s.substr(start, end - start + 1);
}
