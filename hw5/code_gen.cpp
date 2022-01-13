#include "code_gen.hpp"

unsigned long long register_count = 0;

string get_return_type_of_binop(Exp* e1, Exp* e2){
  return (e1->get_type() == e2->get_type()) && (e1->get_type == "BYTE") ? "BYTE" : "INT";
}


reg allocate_register(){ return "%t" + to_string(++register_count); }
reg allocate_global_register(){ return "g" + to_string(++register_count); }

Exp* emitAddSub(Exp* e1, Value* op, Exp* e2){
  string op = (op->get_str() == "+") ? "add" : "sub";
  CodeBuffer& buffer = CodeBuffer::instance();
  string result_reg = allocate_register();
  buffer.emit(result_reg + " = " + op + " i32" + e1->get_reg() + ", " + e2->get_reg());

  string result_type = get_return_type_of_binop(e1, e2);
  if (result_type == "BYTE"){
      new_result_reg = allocate_register();
      buffer.emit(new_result_reg + " = and i32 255, " + result_reg);
      result_reg = new_result_reg;
  }
  return new Exp(result_type, result_reg);
}

Exp* emitMulDiv(Exp* e1, Value* op, Exp* e2){
  string op = (op->get_str() == "*") ? "mul" : "sdiv";

  if (op == "sdiv"){
    //TODO: check if there is div by zero
  }
}
