#ifndef LLVM_H
#define LLVM_H

#include "tokenTypes.hpp"
#include "symbolTable.hpp"
#include "bp.hpp"
typedef string reg

string get_return_type_of_binop(Exp* e1, Exp* e2);

void initBuffer();

// register allocations
reg allocate_register();
reg allocate_global_register();


Exp* emitAddSub(Exp* e1, Value* op, Exp* e2);
void generate_check_div_by_zero_code(Exp* e2);
Exp* emitMulDiv(Exp* e1, Value* op, Exp* e2);










#endif //LLVM_H
