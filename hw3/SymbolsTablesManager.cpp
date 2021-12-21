#include "SymbolsTablesManager.hpp"
#include "hw3_output.hpp"
#include <iostream>

using namespace std;
using namespace output;

extern int yylineno;


string& Symbol::get_name() {return name;}
string& Symbol::get_type() {return type;}
bool& Symbol::get_const() {return is_const;}
int Symbol::get_offset() {return offset;}
void Symbol::set_offset(int offset_num) {offset = offset_num;}
void Symbol::print_symbol() {
    printID(name, offset, type);
}

void Function::add_arg(string type){
    argTypes.push_back(type);
}
int Function::get_args_count(){
    return argTypes.size();
}
int Function::get_next_args_offset(){
    return get_offset() - 1 - argTypes.size();
}
const vector<string>& Function::get_args_types(){
    return argTypes;
}
void Function::print_symbol() {
    printID(get_name(), get_offset(), makeFunctionType(get_type(), argTypes));
}

void SymbolsTable::add_symbol(Symbol* sym, int curr_offset) {
    sym->set_offset(curr_offset);
    table.push_back(sym);
}
void SymbolsTable::print_id_by_declaration_order() {
    for(auto& sym : table) {
        sym->print_symbol();
    }
}
unordered_set<string> SymbolsTable::get_table_ids(){
    unordered_set<string> syms_names;
    for(auto& sym : table) {
        syms_names.insert(sym->get_name());
    }
    return syms_names;
}

SymbolsTable::~SymbolsTable() {
    for (auto sym : table) {
     delete sym;
   }
}


SymbolsTableManager::SymbolsTableManager()
{
    tables.push(SymbolsTable());
    offsets.push(0);
    vector<string> args = {"STRING"};
    add_function("print", "VOID", args);
    args = {"INT"};
    add_function("printi", "VOID", args);
    //cout << "Initial Manager check" << endl; //for debug
}

void SymbolsTableManager::new_scope() {
    tables.push(SymbolsTable());
    int new_offset = offsets.top();
    offsets.push(new_offset);
}

bool  SymbolsTableManager::add_symbol(string name, string type, bool is_const){
    //return false on failure - name already exists.
    if (id_exists(name)){
        return false;
    }
    Symbol* new_symbol = new Symbol(name, type, is_const);
    declared_symbols.insert({name, new_symbol});
    tables.top().add_symbol(new_symbol, offsets.top());
    offsets.top()++;

    return true;
}

bool SymbolsTableManager::add_function(string name, string ret_type, vector<string> arg_types){
    //return false on failure - name already exists.
    if (id_exists(name)){
        return false;
    }
    Function* new_func = new Function(name, ret_type, arg_types);

    ret_type_last_func = ret_type;
    main_func_was_declared = main_func_was_declared || is_function_main(new_func);

    declared_symbols.insert({name, new_func});
    tables.top().add_symbol(new_func, offsets.top());

    return true;
}

bool SymbolsTableManager::is_function_main(Function* func){
    if(func->get_name() == "main"){
        if((func->get_type() == "VOID") && (func->get_args_count() == 0)){
            return true;
        }
    }
    return false;
}

string SymbolsTableManager::add_function_args_symbols(vector<string> arg_names, vector<string> arg_types, vector<bool> arg_consts){
    if(arg_names.empty()) return "";
    for(size_t i = 0; i < arg_names.size() ; i++)
    {
        if (id_exists(arg_names[i])){
            return arg_names[i];
        }
        Symbol* new_symbol = new Symbol(arg_names[i], arg_types[i], arg_consts[i]);
        declared_symbols.insert({arg_names[i], new_symbol});
        tables.top().add_symbol(new_symbol, -(i+1));
    }
    return "";
}

bool SymbolsTableManager::id_exists(string id){
    if (declared_symbols.find(id) == declared_symbols.end())
        return false;
    return true;
}

string SymbolsTableManager::get_type_by_id(string id){
    if (declared_symbols.find(id) == declared_symbols.end())
        return "UNDEFINED";
    return declared_symbols[id]->get_type();
}

bool SymbolsTableManager::get_const_by_id(string id){
    if (declared_symbols.find(id) == declared_symbols.end())
        return false;
    return declared_symbols[id]->get_const();
}

Function* SymbolsTableManager::get_function_by_id(string id){
    if(id_exists(id)){
        return dynamic_cast<Function*>(declared_symbols[id]);
    }
    return nullptr;
}

void SymbolsTableManager::delete_scope() {
    endScope();
    tables.top().print_id_by_declaration_order(); //print_ids
    unordered_set<string> ids_in_top_scope = tables.top().get_table_ids();
    for(auto& name : ids_in_top_scope) {
        declared_symbols.erase(name);
    }
    tables.pop();
    offsets.pop();
}
