#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
#include <cctype>
#include <cmath>
using namespace std;

int prioridad(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

string infijoAPostfijo(string expr) {
    stack<char> pila;
    string salida;
    for (size_t i = 0; i < expr.size(); i++) {
        char c = expr[i];

        if (isspace(c)) continue;

        if (isdigit(c) || c == '.') {
            while (i < expr.size() && (isdigit(expr[i]) || expr[i] == '.')) {
                salida += expr[i++];
            }
            salida += " ";
            i--;
        }
        else if (c == '(') pila.push(c);
        else if (c == ')') {
            while (!pila.empty() && pila.top() != '(') {
                salida += pila.top(); salida += " ";
                pila.pop();
            }
            if (!pila.empty()) pila.pop();
        }
        else { // operadores
            while (!pila.empty() && prioridad(pila.top()) >= prioridad(c)) {
                if (c == '^' && pila.top() == '^') break;
                salida += pila.top(); salida += " ";
                pila.pop();
            }
            pila.push(c);
        }
    }
    while (!pila.empty()) {
        salida += pila.top(); salida += " ";
        pila.pop();
    }
    return salida;
}

double evalPostfija(string post) {
    stack<double> pila;
    stringstream ss(post);
    string token;

    while (ss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
            if (pila.size() < 2) {
                cerr << "Error: expresión mal formada (faltan operandos)\n";
                return 0;
            }
            double b = pila.top(); pila.pop();
            double a = pila.top(); pila.pop();

            if (token == "+") pila.push(a + b);
            else if (token == "-") pila.push(a - b);
            else if (token == "*") pila.push(a * b);
            else if (token == "/") {
                if (b == 0) {
                    cerr << "Error: división entre 0\n";
                    return 0;
                }
                pila.push(a / b);
            }
            else if (token == "^") pila.push(pow(a, b));
        } 
        else if (!token.empty() && (isdigit(token[0]) || token[0] == '-' || token[0] == '.')) {
            try {
                double num = stod(token);
                pila.push(num);
            } catch (...) {
                cerr << "Error: token inválido -> '" << token << "'\n";
                return 0;
            }
        } else {
            cerr << "Error: operador o token desconocido -> '" << token << "'\n";
            return 0;
        }
    }

    if (pila.size() != 1) {
        cerr << "Error: expresión mal formada (sobran operandos)\n";
        return 0;
    }

    return pila.top();
}

void procesarExpresion(const string& expr) {
    string post = infijoAPostfijo(expr);
    double resultado = evalPostfija(post);

    cout << "Expresión: " << expr << "\n";
    cout << "Postfija:  " << post << "\n";
    cout << "Resultado: " << resultado << "\n\n";
}

int main() {
    cout << "=== Conversor Infijo -> Postfijo y Evaluador ===\n";
    cout << "Escriba una expresión o un archivo a evaluar (o 'salir' para terminar):\n";
    string entrada;
    
    while (true) {
        cout << "\n> ";
        getline(cin, entrada);

        if (entrada == "salir") break;
        if (entrada.empty()) continue;

        ifstream file(entrada);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                string expr;
                stringstream ss(line);
                while (getline(ss, expr, ';')) {  // 👈 separa por ;
                    if (expr.empty()) continue;
                    procesarExpresion(expr);
                }
            }
            file.close();
        } else {
            string expr;
            stringstream ss(entrada);
            while (getline(ss, expr, ';')) {  // 👈 también separa varias expresiones escritas en consola
                if (expr.empty()) continue;
                procesarExpresion(expr);
            }
        }
    }

    return 0;
}
