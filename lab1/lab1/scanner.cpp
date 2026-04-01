#include <iostream>
#include <cstring>
#include <fstream>
#include "token.h"
#include "scanner.h"

using namespace std;

// -----------------------------
// Constructor
// -----------------------------
Scanner::Scanner(const char* s): input(s), first(0), current(0) { 
    }

// -----------------------------
// Función auxiliar
// -----------------------------

bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

// -----------------------------
// nextToken: obtiene el siguiente token
// -----------------------------

// Ejercicios 1, 2 hechos.

/*

Token* Scanner::nextToken() {
    Token* token;

    // Saltar espacios en blanco
    while (current < input.length() && is_white_space(input[current])) 
        current++;

    // Fin de la entrada
    if (current >= input.length()) 
        return new Token(Token::END);

    char c = input[current];

    first = current;

    // Números
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    }

    else if (current + 1 < input.length() && c == '*' && input[current + 1] == '*') {
        current++;
        current++;
        token = new Token(Token::POW, input, first, current - first);
    }

    // Operadores
    else if (strchr("+/-*();", c)) {
        switch (c) {
            case '+': token = new Token(Token::PLUS,   c);    break;
            case '-': token = new Token(Token::MINUS,  c);    break;
            case '*': token = new Token(Token::MUL,    c);    break;
            case '/': token = new Token(Token::DIV,    c);    break;
            case '(': token = new Token(Token::LPAREN, c);    break;
            case ')': token = new Token(Token::RPAREN, c);    break;
            case ';': token = new Token(Token::SEMICOLON, c); break;
        }
        current++;
    }

    // Carácter inválido
    else {
        token = new Token(Token::ERR, c);
        current++;
    }

    return token;
}
*/

Token* Scanner::nextToken() {
    Token* token;
    char c;
    state = 0;
    first = current;

    while (true) {
        switch (state) {
            // ==========================================================
            // ESTADO INICIAL: Clasificación
            // ==========================================================
            case 0: 
                c = nextChar();
                if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { first = current; state = 0; }
                else if (c == '\0') return new Token(Token::END);
                
                // Casos directos (un solo carácter)
                else if (c == '(') return new Token(Token::LPAREN, c);
                else if (c == ')') return new Token(Token::RPAREN, c);
                else if (c == '+') return new Token(Token::PLUS, c);
                else if (c == '-') return new Token(Token::MINUS, c);
                else if (c == '/') return new Token(Token::DIV, c);
                else if (c == ';') return new Token(Token::SEMICOLON, c);

                // Casos con lógica adicional (Saltan a estados inferiores)
                else if (c == '*')   state = 10; // Ir a lógica de MULT / POW
                else if (isdigit(c)) state = 20; // Ir a lógica de NUM
                
                else return new Token(Token::ERR, c);
                break;

            // ==========================================================
            // BLOQUE DE POTENCIA / MULTIPLICACIÓN (Tus nuevos estados)
            // ==========================================================
            case 10: 
                // PISTA: Aquí ya se leyó el primer '*'. 
                // Debes leer el siguiente y decidir si vas al estado de POW 
                // o si haces rollback y devuelves un MUL.
                c = nextChar();
                if (c == '*') state = 11;
                else {
                    rollBack();
                    rollBack();
                    c = nextChar();
                    return new Token(Token::MUL, c);
                }
                break;

            case 11:
                // Estado para retornar el Token::POW
                return new Token(Token::POW, input, first, current - first);
                break;

            // ==========================================================
            // BLOQUE DE NÚMEROS
            // ==========================================================
            case 20: 
                c = nextChar();
                if (isdigit(c)) state = 20;
                else state = 21;
                break;

            case 21: 
                rollBack();
                return new Token(Token::NUM, input, first, current - first);
        }
    }
}


void Scanner::rollBack() {
    if (input[current] != '\0')
        current--;
}

char Scanner::nextChar() {
    int c = input[current];
    if (c != '\0') current++;
    return c;
}



// -----------------------------
// Destructor
// -----------------------------
Scanner::~Scanner() { }

// -----------------------------
// Función de prueba
// -----------------------------

void ejecutar_scanner(Scanner* scanner, const string& InputFile) {
    Token* tok;

    // Crear nombre para archivo de salida
    string OutputFileName = InputFile;
    size_t pos = OutputFileName.find_last_of(".");
    if (pos != string::npos) {
        OutputFileName = OutputFileName.substr(0, pos);
    }
    OutputFileName += "_tokens.txt";

    ofstream outFile(OutputFileName);
    if (!outFile.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << OutputFileName << endl;
        return;
    }

    outFile << "Iniciando Scanner para archivo: " << InputFile << endl << endl;

    while (true) {
        tok = scanner->nextToken();

        if (tok->type == Token::END) {
            outFile << *tok << endl;
            delete tok;
            outFile << "\nScanner exitoso" << endl << endl;
            outFile.close();
            return;
        }

        if (tok->type == Token::ERR) {
            outFile << *tok << endl;
            delete tok;
            outFile << "Caracter invalido" << endl << endl;
            outFile << "Scanner no exitoso" << endl << endl;
            outFile.close();
            return;
        }

        outFile << *tok << endl;
        delete tok;
    }
}
