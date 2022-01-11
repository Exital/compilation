#ifndef TOKENTYPES_H
#define TOKENTYPES_H
#include <string>
typedef string reg

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
    string type;
    string id;
public:
    string get_type() {return type;}
    string get_id() {return id;}
    Statement(const string &type, const string &id) : type(type), id(id) {}
    Statement(Value *pType, Value *pId) : type(pType->get_str()), id(pId->get_str()) {}
};

class FormalDecl : public Node
{
    string type;
    string id;
    bool is_const;
public:
    string get_type() {return type;}
    string get_id() {return id;}
    bool get_const() {return is_const;}
    FormalDecl(const string &type, const string &id, bool is_const=false) : type(type), id(id), is_const(is_const) {}
};

class Exp : public Node
{
    string type;
    reg reg;
    bpList true_list;
    bpList false_list;
    string next_label;
public:
    string get_type() {return type;}
    bool is_numeric() {return (type == "INT") || (type == "BYTE");}
    bool is_logic() {return type == "BOOL";}
    explicit Exp(const string& type) : type(type) {}
};

#endif //TOKENTYPES_H
