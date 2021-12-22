#ifndef TOKENTYPES_H
#define TOKENTYPES_H
#include <string>

using namespace std;
class Node
{
public:
    Node* next;
    explicit Node() : next(NULL) {}
    virtual ~Node() = default;
};

class Value : public Node
{
    string value;

public:
    string get_str(){return value;}
    int get_num() {return stoi(value);}
    explicit Value(const string& value) : value(value) {}
};

class ConstType : public Node
{
    bool is_const;
public:
    bool get_const() {return is_const;}
    explicit ConstType(const bool& is_const) : is_const(is_const) {}
};

class Statement : public Node
{
public:
    string type;
    string id;
    Statement(const string &type, const string &id) : type(type), id(id) {}
    Statement(Singelton *pType, Singelton *pId) : type(pType->val), id(pId->val) {}
};

class FormalDecl : public Node
{
public:
    string type;
    string id;
    bool is_const;
    FormalDecl(const string &type, const string &id, bool is_const=false) : type(type), id(id), is_const(is_const) {}
};

class Exp : public Node
{
public:
    string type;
    explicit Exp(const string& type) : type(type) {}
};

#endif //TOKENTYPES_H
