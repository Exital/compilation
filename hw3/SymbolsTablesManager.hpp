#ifndef _236360_SYMBOL_TABLE_MANAGER_
#define _236360_SYMBOL_TABLE_MANAGER_

#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>


using namespace std;

class Symbol{
    string name;
    string type;
    bool is_const;
    int offset = 0;

public:
    Symbol(string name, string type, bool is_const=false) : name(name), type(type), is_const(is_const) {}
    string& get_name();
    string& get_type();
    bool& get_const();
    int get_offset();
    void set_offset(int offset_num);
    virtual void print_symbol();
    virtual ~Symbol() = default;
};

class Function : public Symbol{
    vector<string> argTypes;

public:
    Function(string name, string type, vector<string> argTypes) : Symbol(name, type), argTypes(argTypes) {}
    void add_arg(string type);
    int get_args_count();
    int get_next_args_offset();
    const vector<string>& get_args_types();
    void print_symbol() override;
    ~Function() = default;
};


class SymbolsTable {
    vector<Symbol*> table;

public:
    void add_symbol(Symbol* sym, int curr_offset);
    void print_id_by_declaration_order();
    unordered_set<string> get_table_ids();
    ~SymbolsTable();
};

class SymbolsTableManager{
public:
    stack<SymbolsTable> tables;
    stack<int> offsets;
    unordered_map<string, Symbol*> declared_symbols;

    string ret_type_last_func;
    bool main_func_was_declared = false;

    SymbolsTableManager();

    void new_scope();
    bool add_symbol(string name, string type, bool is_const = false);
    bool add_function(string name, string ret_type, vector<string> arg_types); //return false on failure - name already exists.
    string add_function_args_symbols(vector<string> arg_names, vector<string> arg_types, vector<bool> arg_consts);
    bool id_exists(string id);
    string get_type_by_id(string id);
    bool get_const_by_id(string id);
    Function* get_function_by_id(string id);
    void delete_scope();
    void delete_funcs_scope();
    ~SymbolsTableManager() = default;

    bool is_function_main(Function* func);

};


#endif
