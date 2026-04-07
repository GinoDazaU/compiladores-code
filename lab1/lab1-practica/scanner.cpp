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

    if (current >= input.length()) 
        return new Token(Token::END);

    char c = input[current];
    first = current;

    // --- Lógica para Identificadores (ID) ---
    if (isalpha(c)) {
        current++;
        while (current < input.length() && (isalnum(input[current])))
            current++;
        return new Token(Token::ID, input, first, current - first);
    }

    // --- Lógica para Números (NUM y FLOAT) ---
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        
        // ¿Es un flotante? (punto seguido de dígitos)
        if (current < input.length() && input[current] == '.') {
            current++; // Consumir el '.'
            while (current < input.length() && isdigit(input[current]))
                current++;
            return new Token(Token::FLOAT, input, first, current - first);
        }
        return new Token(Token::NUM, input, first, current - first);
    }

    // --- Lógica para Potencia (**) y Operadores ---
    if (c == '*') {
        current++;
        if (current < input.length() && input[current] == '*') {
            current++;
            return new Token(Token::POW, input, first, current - first);
        }
        return new Token(Token::MUL, c);
    }

    // Otros operadores de un solo carácter
    if (strchr("+/-();", c)) {
        current++;
        switch (c) {
            case '+': return new Token(Token::PLUS, c);
            case '-': return new Token(Token::MINUS, c);
            case '/': return new Token(Token::DIV, c);
            case '(': return new Token(Token::LPAREN, c);
            case ')': return new Token(Token::RPAREN, c);
            case ';': return new Token(Token::SEMICOLON, c);
        }
    }

    current++;
    return new Token(Token::ERR, c);
}
*/

Token* Scanner::nextToken() {
    Token* token;
    char c;
    state = 0;
    first = current;

    while (true) {
        switch (state) {
            case 0: 
                c = nextChar();
                if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { first = current; state = 0; }
                else if (c == '\0') return new Token(Token::END);
                else if (c == '(') return new Token(Token::LPAREN, c);
                else if (c == ')') return new Token(Token::RPAREN, c);
                else if (c == '+') return new Token(Token::PLUS, c);
                else if (c == '-') return new Token(Token::MINUS, c);
                else if (c == '/') return new Token(Token::DIV, c);
                else if (c == ';') return new Token(Token::SEMICOLON, c);
                else if (c == '*') state = 10;
                else if (isdigit(c)) state = 20; // Posible NUM o FLOAT
                else if (isalpha(c)) state = 30; // Posible ID
                else return new Token(Token::ERR, c);
                break;

            // --- POTENCIA ---
            case 10: 
                c = nextChar();
                if (c == '*') return new Token(Token::POW, input, first, current - first);
                rollBack();
                return new Token(Token::MUL, '*');

            // --- NÚMEROS (NUM / FLOAT) ---
            case 20: 
                c = nextChar();
                if (isdigit(c)) state = 20;
                else if (c == '.') state = 22; // Es un flotante
                else state = 21; // Es un entero
                break;

            case 21: // Fin de Entero
                rollBack();
                return new Token(Token::NUM, input, first, current - first);

            case 22: // Parte decimal
                c = nextChar();
                if (isdigit(c)) state = 23;
                else { // Error: punto sin dígitos (ej. "12.")
                    return new Token(Token::ERR, input, first, current - first);
                }
                break;

            case 23:
                c = nextChar();
                if (isdigit(c)) state = 23;
                else state = 24;
                break;

            case 24: // Fin de Flotante
                rollBack();
                return new Token(Token::FLOAT, input, first, current - first);

            // --- IDENTIFICADORES (ID) ---
            case 30:
                c = nextChar();
                if (isalnum(c)) state = 30; // Letras o dígitos después de la primera letra
                else state = 31;
                break;

            case 31:
                rollBack();
                string keyword = input.substr(first, current - first);
                return new Token(Token::ID, input, first, current - first);
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
