#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "scanner.h"

class Parser {
   private:
    Scanner* scanner;
    Token* current;
    Token* previous;

    // indica si estamos en el ultimo token
    // se deberia usar al principio de los demas
    // metodos para cortar el flujo si ya estas en el final
    bool isAtEnd() {
        if (current->type == Token::ERR) throw runtime_error("Error lexico");
        return current->type == Token::END;
    }

    // retorna true si el current es el mismo token
    bool check(Token::Type ttype) {
        if (isAtEnd()) return false;
        return current->type == ttype;
    }

    // intenta avanzar previous y current, retorna true si ocurre
    bool advance() {
        if (isAtEnd()) return false;
        if (previous) delete previous;
        Token* temp = current;           // dos punteros a uno
        current = scanner->nextToken();  // ahora solo hay un puntero
        previous = temp;                 // dos punteros a uno denuevo pero el temp se borrara
        if (check(Token::ERR)) throw runtime_error("Error lexico");
        return true;
    }

    // si el token es el mismo que el token actual
    // avanza previous, current y retorna true
    bool match(Token::Type ttype) {
        if (isAtEnd()) return false;
        if (check(ttype)) {
            advance();
            return true;
        }
        return false;
    }

   public:
    Parser(Scanner* scanner) : scanner(scanner) {
        current = scanner->nextToken();
        previous = nullptr;
    }

    Exp* parseProgram() {
        Exp* ast;
        ast = parseP();
        cout << "Parseo exitoso" << endl;
        return ast;
    }

    Exp* parseP() {
        Exp* l = parseE();
        while (match(Token::PLUS)) {
            Exp* r = parseE();
            l = new BinaryExp(l, r, BinaryOp::PLUS_OP);
        }
        return l;
    }

    Exp* parseE() {
        Exp* l = parseF();
        while(match(Token::MUL)) {
            Exp* r = parseF();
            l = new BinaryExp(l, r, BinaryOp::MUL_OP);
        }
        return l;
    }

    Exp* parseF() {
        if(!check(Token::NUM)) throw runtime_error("Error sintactico");
        Exp* e = new NumberExp(stoi(current->text));
        advance();
        return e;
    }
};

#endif