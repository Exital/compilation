#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H

#include <stack>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Symbol{
    string id;
    string type;
    bool is_const;
    int offset = 0;

public:
    Symbol(string id, string type, bool is_const=false) : id(id), type(type), is_const(is_const) {}
    string& get_id() {return id;}
    string& get_type() {return type;}
    bool& get_const() {return is_const;}
    int get_offset() {return offset;}
    void set_offset(int offset_num) {offset = offset_num;}
    virtual void print_symbol();
    virtual ~Symbol() = default;
};

class Function : public Symbol{
    vector<string> argTypes;
    bool is_main = false;

public:
    Function(string id, string type, vector<string> argTypes, bool is_const) : Symbol(id, type, is_const), argTypes(argTypes) {
        is_main = (id == "main") && (type == "VOID") && (argTypes.empty());
    }
    bool& get_is_main(){return is_main;}
    void print_symbol() override;
    ~Function() = default;
};

typedef vector<Symbol*> Scope;

class SymbolTable {

    stack<Scope> scopes;
    stack<int> offsets;
    unordered_map<string, Symbol*> declared_symbols;

    string last_func_return_type;
    bool was_main_declared = false;

public:
    SymbolTable();

    void new_scope();
    bool id_exists(string id);
    bool add_symbol(string id, string type, bool is_const=false);
    bool add_function(string id, string ret_type, vector<string> argTypes);
    string add_function_args_symbols(vector<string> arg_ids, vector<string> argTypes, vector<bool> arg_consts);
    string get_type_by_id(string id);
    bool get_const_by_id(string id);
    Function* get_function_by_id(string id);
    void delete_scope();
    bool get_main_declared() {return was_main_declared;}
    string get_last_return_type() {return last_func_return_type;}
    ~SymbolsTableManager() = default;
};

#endif //HW3_SYMBOLTABLE_H