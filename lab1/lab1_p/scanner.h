#ifndef SCANNER_H
#define SCANNER_H

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

#include "token.h"

using namespace std;

// espacio ' ', tabulacion '\t', nueva linea '\n'
bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

class Scanner {
private:
    string input;
    int start;     // antes 'first'
    int current;

public:
    Scanner(const string& input) : input(input), start(0), current(0) {}

    Token* nextToken() {
        // saltar espacios en blanco
        while (current < input.length() && is_white_space(input[current])) {
            current++;
        }

        if (current >= input.length()) {
            return new Token(Token::Type::END);
        }

        char c = input[current];

        // numeros
        if (isdigit(c)) {
            int start_num = current;
            while (current < input.length() && isdigit(input[current])) {
                current++;
            }
            return new Token(Token::Type::NUM, input, start_num, current - start_num);
        }

        // operadores y parentesis
        if (c == '+') {
            current++;
            return new Token(Token::Type::PLUS, '+');
        }

        if (c == '-') {
            current++;
            return new Token(Token::Type::MINUS, '-');
        }

        if (c == '*') {
            current++;
            return new Token(Token::Type::MUL, '*');
        }

        if (c == '/') {
            current++;
            return new Token(Token::Type::DIV, '/');
        }

        if (c == '^') {
            current++;
            return new Token(Token::Type::POW, '^');
        }

        if (c == '(') {
            current++;
            return new Token(Token::Type::LPAREN, '(');
        }

        if (c == ')') {
            current++;
            return new Token(Token::Type::RPAREN, ')');
        }

        // simbolo no reconocido
        return new Token(Token::Type::ERR, c);
    }
};

void run_scanner(const string& input_path) {
    ifstream in_file(input_path);
    if (!in_file) {
        cerr << "No se pudo abrir el archivo: " << input_path << endl;
        return;
    }

    string output_path = input_path + "_tokens.txt";
    ofstream out_file(output_path);
    if (!out_file) {
        cerr << "No se pudo crear el archivo: " << output_path << endl;
        return;
    }

    string content = "";
    string line;
    while (getline(in_file, line)) {
        content += line + '\n';
    }
    in_file.close();

    Scanner scanner(content);

    Token* token = scanner.nextToken();
    while (token->type != Token::Type::END && token->type != Token::Type::ERR) {
        out_file << *token << endl;
        delete token;
        token = scanner.nextToken();
    }

    if (token->type == Token::Type::END) {
        out_file << *token << endl;
        out_file << "Scanner completado con exito" << endl;
    }

    if (token->type == Token::Type::ERR) {
        out_file << *token << endl;
        out_file << "Error: Ultimo simbolo no reconocido" << endl;
    }
    
    delete token;
    out_file.close();
}

#endif