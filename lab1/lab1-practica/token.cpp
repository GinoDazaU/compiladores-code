#include <iostream>
#include "token.h"

using namespace std;

// -----------------------------
// Constructores Actualizados
// -----------------------------

Token::Token(Type type, int line, int col) 
    : type(type), text(""), line(line), col(col) { }

Token::Token(Type type, char c, int line, int col) 
    : type(type), text(string(1, c)), line(line), col(col) { }

Token::Token(Type type, const string& source, int first, int last, int line, int col) 
    : type(type), text(source.substr(first, last)), line(line), col(col) { }

// -----------------------------
// Sobrecarga de operador << (Incluyendo ubicación)
// -----------------------------

ostream& operator<<(ostream& outs, const Token& tok) {
    // Imprimimos la ubicación primero: [Fila:Col]
    outs << "[" << tok.line << ":" << tok.col << "] ";

    switch (tok.type) {
        case Token::PLUS:      outs << "TOKEN(PLUS, \""      << tok.text << "\")"; break;
        case Token::MINUS:     outs << "TOKEN(MINUS, \""     << tok.text << "\")"; break;
        case Token::MUL:       outs << "TOKEN(MUL, \""       << tok.text << "\")"; break;
        case Token::DIV:       outs << "TOKEN(DIV, \""       << tok.text << "\")"; break;
        case Token::NUM:       outs << "TOKEN(NUM, \""       << tok.text << "\")"; break;
        case Token::LPAREN:    outs << "TOKEN(LPAREN, \""    << tok.text << "\")"; break;
        case Token::RPAREN:    outs << "TOKEN(RPAREN, \""    << tok.text << "\")"; break;
        case Token::POW:       outs << "TOKEN(POW, \""       << tok.text << "\")"; break;
        case Token::SEMICOLON: outs << "TOKEN(SEMICOLON, \"" << tok.text << "\")"; break;
        case Token::ID:        outs << "TOKEN(ID, \""        << tok.text << "\")"; break;
        case Token::FLOAT:     outs << "TOKEN(FLOAT, \""     << tok.text << "\")"; break;
        case Token::ERR:       outs << "TOKEN(ERR, \""       << tok.text << "\")"; break;
        case Token::END:       outs << "TOKEN(END)"; break;
    }
    return outs;
}

ostream& operator<<(ostream& outs, const Token* tok) {
    if (!tok) return outs << "TOKEN(NULL)";
    return outs << *tok;
}