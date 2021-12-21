#ifndef COMPILATION_TOKENCLASSES_H
#define COMPILATION_TOKENCLASSES_H
#include <string>
#include <vector>
using namespace std;
class Node
{
public:
    Node* next;
    explicit Node() : next(NULL) {}
    virtual ~Node() = default;
};

class Singelton : public Node
{
public:
    string val;
    explicit Singelton(const string& val) : val(val) {}
};

class Number : public Node
{
public:
    int val;
    explicit Number(const string& str_val) {
        val = stoi(str_val);
    }
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
    //FormalDecl(Singelton *pType, Singelton *pId) : type(pType->val), id(pId->val) {}
};

class Exp : public Node
{
public:
    string type;
    explicit Exp(const string& type) : type(type) {}
};

#endif //COMPILATION_TOKENCLASSES_H
