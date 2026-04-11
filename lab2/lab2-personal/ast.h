#ifndef AST_H
#define AST_H

#include <iostream>
#include <ostream>
#include <string>

using namespace std;

enum class BinaryOp {
    PLUS_OP,
    MINUS_OP,
    MUL_OP,
    DIV_OP
};

// --- Clase Base ---
class Exp {
public:
    virtual ~Exp() = 0;
    virtual void toDot(ostream& out, int& id) const = 0;

    static string binOpToChar(BinaryOp op) {
        switch (op) {
            case BinaryOp::PLUS_OP:  return "+";
            case BinaryOp::MINUS_OP: return "-";
            case BinaryOp::MUL_OP:   return "*";
            case BinaryOp::DIV_OP:   return "/";
            default:                 return "?";
        }
    }
};

// inline para evitar errores de redefinición
inline Exp::~Exp() {}

// --- Raiz del arbol ---
// --- y Nodos Internos (Operaciones) ---
class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;

    BinaryExp(Exp* l, Exp* r, BinaryOp o) : left(l), right(r), op(o) {}

    ~BinaryExp() {
        delete left;
        delete right;
    }

    void toDot(ostream& out, int& id) const override {
        int myId = id++;
        out << "  node" << myId << " [label=\"" << Exp::binOpToChar(op) << "\"];\n";
        
        if (left) {
            int leftId = id;
            left->toDot(out, id);
            out << "  node" << myId << " -> node" << leftId << ";\n";
        }
        if (right) {
            int rightId = id;
            right->toDot(out, id);
            out << "  node" << myId << " -> node" << rightId << ";\n";
        }
    }
};

// --- Hojas (Numeros) ---
class NumberExp : public Exp {
public:
    int value;

    NumberExp(int v) : value(v) {}
    
    ~NumberExp() {}

    void toDot(ostream& out, int& id) const override {
        int myId = id++;
        out << "  node" << myId << " [label=\"" << value << "\"];\n";
    }
};

#endif