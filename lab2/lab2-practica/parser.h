#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "scanner.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

class Parser {
private:
    Scanner* scanner;
    Token* current;
    Token* previous;

    bool isAtEnd() {
        if (current->type == Token::ERR) throw runtime_error("Error lexico");
        return current->type == Token::END;
    }

    bool check(Token::Type ttype) {
        if (isAtEnd()) return false;
        return current->type == ttype;
    }

    bool advance() {
        if (isAtEnd()) return false;
        if (previous) delete previous;
        Token* temp = current;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) throw runtime_error("Error lexico");
        return true;
    }

    bool match(Token::Type ttype) {
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
        Exp* ast = parseP();
        if (!isAtEnd()) throw runtime_error("Error sintactico: se esperaba fin de expresion");
        cout << "Parseo exitoso" << endl;
        return ast;
    }

    // 1. Suma y Resta
    Exp* parseP() {
        Exp* l = parseE();
        while (match(Token::PLUS) || match(Token::MINUS)) {
            Token::Type op = previous->type;
            Exp* r = parseE();
            if (op == Token::PLUS) l = new BinaryExp(l, r, BinaryOp::PLUS_OP);
            else l = new BinaryExp(l, r, BinaryOp::MINUS_OP);
        }
        return l;
    }

    // 2. Multiplicación y División
    Exp* parseE() {
        Exp* l = parsePow();
        while (match(Token::MUL) || match(Token::DIV)) {
            Token::Type op = previous->type;
            Exp* r = parsePow();
            if (op == Token::MUL) l = new BinaryExp(l, r, BinaryOp::MUL_OP);
            else l = new BinaryExp(l, r, BinaryOp::DIV_OP);
        }
        return l;
    }

    // 3. Potencia (Asociativa a la derecha, ej: 2^3^2 es 2^(3^2))
    Exp* parsePow() {
        Exp* l = parseF();
        if (match(Token::POW)) { // Supone que Token::POW es **
            Exp* r = parsePow(); // Llamada recursiva a la derecha
            l = new BinaryExp(l, r, BinaryOp::POW_OP);
        }
        return l;
    }

    // 4. Paréntesis, Variables, Números y Funciones
    Exp* parseF() {
        // Soporte para Paréntesis
        if (match(Token::LPAREN)) {
            Exp* e = parseP();
            if (!match(Token::RPAREN)) throw runtime_error("Error sintactico: se esperaba ')'");
            return e;
        }

        // Soporte para Números
        if (match(Token::NUM)) {
            return new NumberExp(stoi(previous->text));
        }

        // Soporte para Variables e Identificadores (Funciones)
        if (match(Token::ID)) {
            string name = previous->text;

            // Si le sigue un '(', es una Función
            if (match(Token::LPAREN)) {
                Exp* arg = parseP();
                if (!match(Token::RPAREN)) throw runtime_error("Error sintactico: se esperaba ')'");
                return new FunctionExp(name, arg); // Debes tener esta clase en ast.h
            }

            // Si no, es una Variable simple
            return new IdentifierExp(name); // Debes tener esta clase en ast.h
        }

        throw runtime_error("Error sintactico: se esperaba numero, variable o '('");
    }
};

#endif