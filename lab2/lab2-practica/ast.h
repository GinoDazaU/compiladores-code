#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <list>
#include <ostream>

using namespace std;

// Operadores binarios soportados 
enum BinaryOp { 
    PLUS_OP, 
    MINUS_OP, 
    MUL_OP, 
    DIV_OP,
    POW_OP  // Añadido para potencia (**)
};

// Clase abstracta Exp
class Exp {
public:
    virtual ~Exp() = 0;  
    virtual void toDot(std::ostream& out, int& id) const = 0;  
    static string binopToChar(BinaryOp op);  
};

// Expresión binaria (nodos internos)
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;

    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();
    void toDot(std::ostream& out, int& id) const override; 
};

// Expresión numérica (hojas del árbol)
class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    ~NumberExp();
    void toDot(std::ostream& out, int& id) const override; 
};

// 4. Soporte para variables (Identificadores)
class IdentifierExp : public Exp {
public:
    string name;
    IdentifierExp(const string& n);
    ~IdentifierExp();
    void toDot(std::ostream& out, int& id) const override;
};

// 5. Soporte para funciones matemáticas (ej: sqrt(arg))
class FunctionExp : public Exp {
public:
    string name;
    Exp* arg;
    FunctionExp(const string& n, Exp* a);
    ~FunctionExp();
    void toDot(std::ostream& out, int& id) const override;
};

#endif