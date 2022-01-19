#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "tokenTypes.hpp"
#include "symbolTable.hpp"
#include "bp.hpp"
typedef string reg
extern SymbolTable st;

unordered_map<string, string> llvm_relop_op =
        {
            {"==", "eq"},
            {"!=", "ne"},
            {"<", "slt"},
            {">", "sgt"},
            {"<=", "sle"},
            {">=", "sge"},
        };

string get_return_type_of_binop(Exp* e1, Exp* e2);

void initBuffer();

// register allocations
reg allocate_register();
reg allocate_global_register();


Exp* emitAddSub(Exp* e1, Value* op, Exp* e2);
void generate_check_div_by_zero_code(Exp* e2);
Exp* emitMulDiv(Exp* e1, Value* op, Exp* e2);


Exp* emitRelop(Exp* e1, Value* op, Exp* e2);

Exp* emitLoad(string id, string type);
void emitStore(string id, string type, string val);

void bool_handler(Exp* e1);

Exp* emitCallFunc(string r_type, string func_id, const vector<string>& param_types = vector<string>(), const vector<string>& param_regs=vector<string>());



#endif //CODE_GEN_H
