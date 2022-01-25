#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "tokenTypes.hpp"
#include "SymbolTable.hpp"
#include "bp.hpp"
typedef string reg;
extern SymbolTable st;


string get_return_type_of_binop(Exp* e1, Exp* e2);

void init_buffer();

// register allocations
reg allocate_register();
reg allocate_global_register();


Exp* emit_add_sub(Exp* e1, Value* op, Exp* e2);
void generate_check_div_by_zero_code(Exp* e2);
Exp* emit_mul_div(Exp* e1, Value* op, Exp* e2);


Exp* emit_relop(Exp* e1, Value* op, Exp* e2);

Exp* emit_load(string id, string type);
void emit_store(string id, string type, string val);

void bool_handler(Exp* e1);

Exp* emit_function_call(string r_type, string func_id, const vector<string>& param_types = vector<string>(), const vector<string>& param_regs=vector<string>());

Exp* llvm_casting(Exp* e1, string to_type);

#endif //CODE_GEN_H
