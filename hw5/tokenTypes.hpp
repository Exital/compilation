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
    reg get_reg(){return reg;}
    Exp(const string &type, const reg &reg = "", const bpList &true_list = bpList(), const bpList &false_list = bpList(), string next_label = "") : type(type), reg(reg), true_list(true_list), false_list(false_list), next_label(next_label);
};

class M : public Node
{
public:
    string next_inst;
    explicit M(const string& val) : next_inst(val) {}
};

class N : public Node
{
public:
    bpList next_list;
    explicit N(const bpList &nextLst = bpList()) : next_list(nextLst) {}
};

#endif //TOKENTYPES_H
