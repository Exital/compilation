//
// Created by Tal Rosenseweig on 22/12/2021.
//

#include "SymbolTable.hpp"
#include "hw3_output.hpp"
#include <iostream>

using namespace std;
using namespace output;
extern int yylineno;

void Symbol::print_symbol() {
    printID(id, offset, type);
}

void Function::print_symbol() {
    printID(get_id(), get_offset(), makeFunctionType(get_type(), argTypes));
}

void SymbolTable::new_scope() {
    Scope scope;
    scopes.push(scope);
    int new_offset = offsets.top();
    offsets.push(new_offset);
}

bool SymbolTable::id_exists(string id){return !(declared_symbols.find(id) == declared_symbols.end());}

bool SymbolTable::add_symbol(string id, string type, bool is_const){
    if (id_exists(id)){
        return false;
    }
    Symbol* new_symbol = new Symbol(id, type, is_const);
    declared_symbols.insert({id, new_symbol});

    //add symbol
    new_symbol->set_offset(offsets.top());
    scopes.top().push_back(new_symbol);
    offsets.top()++;
    return true;
}

bool SymbolTable::add_function(string id, string type, vector<string> argTypes){
    if (id_exists(id)){
        return false;
    }
    Function* new_func = new Function(id, type, argTypes);

    last_func_return_type = type;
    was_main_declared = was_main_declared || new_func->get_is_main();

    declared_symbols.insert({id, new_func});
    new_func->set_offset(offsets.top());
    scopes.top().push_back(new_func);

    return true;
}

string SymbolTable::add_function_args_symbols(vector<string> arg_ids, vector<string> argTypes, vector<bool> arg_consts){
    if(arg_ids.empty()) return "";

    for(size_t i = 0; i < arg_ids.size() ; i++)
    {
        if (id_exists(arg_ids[i])){
            return arg_ids[i];
        }
        Symbol* new_symbol = new Symbol(arg_ids[i], argTypes[i], arg_consts[i]);
        declared_symbols.insert({arg_ids[i], new_symbol});
        new_symbol->set_offset(-(i+1));
        scopes.top().push_back(new_symbol);
    }
    return "";
}

string SymbolTable::get_type_by_id(string id){
    if (!id_exists(id)) return "ERROR";
    return declared_symbols[id]->get_type();
}

bool SymbolTable::get_const_by_id(string id){
    if (!id_exists(id)) return false;
    return declared_symbols[id]->get_const();
}

Function* SymbolTable::get_function_by_id(string id){
    if(!id_exists(id)) return nullptr;
    return dynamic_cast<Function*>(declared_symbols[id]);
}

void SymbolTable::delete_scope() {
    endScope();

    for(auto& symbol : scopes.top()) {
        symbol->print_symbol();
        declared_symbols.erase(symbol->get_id());
    }

    scopes.pop();
    offsets.pop();
}