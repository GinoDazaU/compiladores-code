#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <string>

using namespace std;

class Token {
public:
    enum Type {
        ID, PLUS, MINUS, MUL, DIV, POW, LPAREN, RPAREN, SEMICOLON, NUM, FLOAT, ERR, END
    };

    // --- NUEVOS ATRIBUTOS (Ejercicio 5) ---
    Type type;
    string text;
    int line;
    int col;

    // Constructores actualizados para recibir línea y columna
    Token(Type type, int line, int col);
    Token(Type type, char c, int line, int col);
    Token(Type type, const string& source, int first, int last, int line, int col);

    friend ostream& operator<<(ostream& outs, const Token& tok);
    friend ostream& operator<<(ostream& outs, const Token* tok);
};

#endif