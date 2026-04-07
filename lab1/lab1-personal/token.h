#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

using namespace std;

// Ejemplo de funcionamiento del Scanner
// Entrada: 3 + 5
// Token: <Tipo, texto>
// Salida: <NUM, "3"> <PLUS, "+"> <NUM, "5"> <END>

class Token {
   public:
    enum Type {
        NUM,     // Número
        PLUS,    // +
        MINUS,   // -
        MUL,     // *
        DIV,     // /
        POW,     // ^
        LPAREN,  // (
        RPAREN,  // )
        ERR,     // Error léxico
        END      // Fin de archivo
    };

    Type type;
    string text;  // Representa el lexema (secuencia de caracteres concreta)

    Token(Type type) : type(type), text("") {}                    // Para END y ERR
    Token(Type type, char c) : type(type), text(string(1, c)) {}  // Para un solo símbolo (+, -, *, /)
    Token(Type type, const string& source, int first, int length)
        : type(type), text(source.substr(first, length)) {}  // Para tokens con lexemas largos (como números)

    // Convierte el contenido del token en string para escribirlo en el archivo rápidamente.
    // El ostream puede ser un ofstream para escritura directa a ficheros.
    friend ostream& operator<<(ostream& outs, const Token& token) {
        switch (token.type) {
            case Type::NUM:
                outs << "TOKEN(NUM, \"" << token.text << "\")";
                break;
            case Type::PLUS:
                outs << "TOKEN(PLUS, \"" << token.text << "\")";
                break;
            case Type::MINUS:
                outs << "TOKEN(MINUS, \"" << token.text << "\")";
                break;
            case Type::MUL:
                outs << "TOKEN(MUL, \"" << token.text << "\")";
                break;
            case Type::DIV:
                outs << "TOKEN(DIV, \"" << token.text << "\")";
                break;
            case Type::POW:
                outs << "TOKEN(POW, \"" << token.text << "\")";
                break;
            case Type::LPAREN:
                outs << "TOKEN(LPAREN, \"" << token.text << "\")";
                break;
            case Type::RPAREN:
                outs << "TOKEN(RPAREN, \"" << token.text << "\")";
                break;
            case Type::ERR:
                outs << "TOKEN(ERR)";
                break;
            case Type::END:
                outs << "TOKEN(END)";
                break;
        }
        return outs;
    }

    // Sobrecarga para manejar punteros a Token
    friend ostream& operator<<(ostream& outs, const Token* token) {
        if (!token) return outs << "TOKEN(NULL)";
        return outs << *token;  // Delega a la sobrecarga por referencia
    }
};

#endif