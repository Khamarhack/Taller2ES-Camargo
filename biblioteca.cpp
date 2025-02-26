/*******************************************
*    Fecha: 26 de febrero 2025             *
*    Autor: Santiago Camargo               *
*    Tema: Taller2 Tads biblioteca         *
*    Estructuras de datos                  *
*******************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <sstream>

using namespace std;

// TAD Palabra
class Palabra {
private:
    string palabra;
    unsigned int n_linea;

public:
    Palabra(string n_palabra = "", unsigned int n_num = 0) : palabra(n_palabra), n_linea(n_num) {}
    void FijarPalabra(string n_palabra) { palabra = n_palabra; }
    void FijarNumLinea(unsigned int n_num) { n_linea = n_num; }
    string ObtenerPalabra() const { return palabra; }
    unsigned int ObtenerNumLinea() const { return n_linea; }
};

// TAD ArchivoTexto
class ArchivoTexto {
private:
    vector<vector<string>> lineasTexto;
    string limpiarPalabra(const string& palabra);
    string invertirCadena(const string& cadena);

public:
    void FijarListaLineas(const vector<vector<string>>& n_lista) { lineasTexto = n_lista; }
    vector<vector<string>> ObtenerListaLineas() const { return lineasTexto; }
    unsigned int ObtenerNumLineas() const { return lineasTexto.size(); }
    void AgregarListaPals(const vector<string>& n_lista) { lineasTexto.push_back(n_lista); }
    vector<Palabra> BuscarPrincipio(const string& subcadena);
    vector<Palabra> BuscarContiene(const string& subcadena);
    vector<Palabra> BuscarContieneInvertida(const string& subcadena);
};

string ArchivoTexto::limpiarPalabra(const string& palabra) {
    string resultado;
    for (char c : palabra) {
        if (isalpha(c) || c == '-') resultado += tolower(c);
    }
    return resultado;
}

string ArchivoTexto::invertirCadena(const string& cadena) {
    string invertida = cadena;
    reverse(invertida.begin(), invertida.end());
    return invertida;
}

vector<Palabra> ArchivoTexto::BuscarPrincipio(const string& subcadena) {
    vector<Palabra> resultado;
    queue<Palabra> cola;
    string subcadenaLower = subcadena;
    transform(subcadenaLower.begin(), subcadenaLower.end(), subcadenaLower.begin(), ::tolower);
    
    for (unsigned int numLinea = 0; numLinea < lineasTexto.size(); numLinea++) {
        for (string& palabra : lineasTexto[numLinea]) {
            string palabraLimpia = limpiarPalabra(palabra);
            if (!palabraLimpia.empty() && palabraLimpia.find(subcadenaLower) == 0)
                cola.push(Palabra(palabra, numLinea + 1));
        }
    }
    
    while (!cola.empty()) {
        resultado.push_back(cola.front());
        cola.pop();
    }
    return resultado;
}

vector<Palabra> ArchivoTexto::BuscarContiene(const string& subcadena) {
    vector<Palabra> resultado;
    queue<Palabra> cola;
    string subcadenaLower = subcadena;
    transform(subcadenaLower.begin(), subcadenaLower.end(), subcadenaLower.begin(), ::tolower);
    
    for (unsigned int numLinea = 0; numLinea < lineasTexto.size(); numLinea++) {
        for (string& palabra : lineasTexto[numLinea]) {
            string palabraLimpia = limpiarPalabra(palabra);
            if (!palabraLimpia.empty() && palabraLimpia.find(subcadenaLower) != string::npos)
                cola.push(Palabra(palabra, numLinea + 1));
        }
    }
    
    while (!cola.empty()) {
        resultado.push_back(cola.front());
        cola.pop();
    }
    return resultado;
}

vector<Palabra> ArchivoTexto::BuscarContieneInvertida(const string& subcadena) {
    string subcadenaInvertida = invertirCadena(subcadena);
    return BuscarContiene(subcadenaInvertida);
}

vector<string> dividirEnPalabras(const string& linea) {
    vector<string> palabras;
    stringstream ss(linea);
    string palabra;
    while (ss >> palabra) palabras.push_back(palabra);
    return palabras;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " archivo.txt" << endl;
        return 1;
    }

    ifstream archivo(argv[1]);
    if (!archivo) {
        cerr << "Error: No se pudo abrir el archivo " << argv[1] << endl;
        return 1;
    }

    int n;
    archivo >> n;
    archivo.ignore();
    string subcadena;
    getline(archivo, subcadena);

    ArchivoTexto archivoTexto;
    for (int i = 0; i < n - 1 && !archivo.eof(); i++) {
        string linea;
        getline(archivo, linea);
        archivoTexto.AgregarListaPals(dividirEnPalabras(linea));
    }
    archivo.close();

    vector<Palabra> resultadosPrincipio = archivoTexto.BuscarPrincipio(subcadena);
    vector<Palabra> resultadosContiene = archivoTexto.BuscarContiene(subcadena);
    vector<Palabra> resultadosInvertida = archivoTexto.BuscarContieneInvertida(subcadena);
    
    cout << "Palabras que comienzan con '" << subcadena << "': " << resultadosPrincipio.size() << endl;
    for (Palabra p : resultadosPrincipio)
        cout << "   " << p.ObtenerPalabra() << " (línea " << p.ObtenerNumLinea() << ")" << endl;
    
    cout << "Palabras que contienen '" << subcadena << "': " << resultadosContiene.size() << endl;
    for (Palabra p : resultadosContiene)
        cout << "   " << p.ObtenerPalabra() << " (línea " << p.ObtenerNumLinea() << ")" << endl;
    
    cout << "Palabras que contienen la subcadena invertida '" << subcadena << "': " << resultadosInvertida.size() << endl;
    for (Palabra p : resultadosInvertida)
        cout << "   " << p.ObtenerPalabra() << " (línea " << p.ObtenerNumLinea() << ")" << endl;
    
    return 0;
}
