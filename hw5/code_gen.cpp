#include "code_gen.hpp"

unsigned long long register_count = 0;
reg function_sp = "";

string get_return_type_of_binop(Exp* e1, Exp* e2){
  return (e1->get_type() == e2->get_type()) && (e1->get_type == "BYTE") ? "BYTE" : "INT";
}

void initBuffer(){
  CodeBuffer& buffer = CodeBuffer::instance();
  string errorMsg = "Error division by zero";

  string declarations = "declare i32 @printf(i8*, ...)\n";
  declarations += "declare void @exit(i32)\n";
  declarations += "@div_by_zero_error = constant [" + std::to_string(errorMsg.length()+1) + " x i8] c\"" + errorMsg + "\\00\"";
  buffer.emitGlobal(declarations);

  string specifiers = "@.str_specifier = internal constant [4 x i8] c\"%s\\0A\\00\"\n"
  specifiers += "@.int_specifier = internal constant [4 x i8] c\"%d\\0A\\00\"\n";
  buffer.emitGlobal(specifiers);

  string print = "define void @print(i8*) {\n";
  print += "    call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)\n";
  print += "    ret void\n}";
  buffer.emit(print);

  string printi = "define void @printi(i32) {\n";
  printi += "    call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)\n";
  printi += "    ret void\n}";
  buffer.emit(printi);
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

void generate_check_div_by_zero_code(Exp* e2){
  reg tmp_reg = allocate_register();
  buffer.emit(tmp_reg + " = icmp eq i32 0, " + e2->get_reg());
  int check_zero_br = buffer.emit("br i1 " + tmp_reg + ", label @, label @");
  string error_label = buffer.genLabel();
  buffer.emit("call void @print(i8* getelementptr ([" + len + " x i8], [" + len + " x i8]* " + "@div_by_zero_error" + ", i32 0, i32 0))");
  buffer.emit("call void @exit(i32 1)");
  int cont_br = buffer.emit("br label @"); // end of error block
  string cont_label = buffer.genLabel();

  bpList is_zero =  makelist(bpItem(check_zero_br, FIRST));
  bpList not_zero = merge(makelist(bpItem(check_zero_br, SECOND), makelist(bpItem(cont_br, FIRST)));

  buffer.bpatch(is_zero, error_label);
  buffer.bpatch(not_zero, cont_label);
}

Exp* emitMulDiv(Exp* e1, Value* op, Exp* e2){
  string op = (op->get_str() == "*") ? "mul" : "sdiv";

  if (op == "sdiv") {
    generate_check_div_by_zero_code(e2);
  }

  reg res_reg = allocate_register();
  buffer.emit(res_reg + " = " + op + " i32 " + e1->get_reg() + ", " + e2->get_reg());

  string res_type = get_return_type_of_binop(e1,e2);
  if (res_type == "BYTE"){
    reg new_res_reg = allocate_register();
    buffer.emit(new_res_reg + " = and i32 255, " + res_reg);
    res_reg = new_res_reg;
  }

  return new Exp(res_type, res_reg);
}

Exp* emitRelop(Exp* e1, Value* op, Exp* e2){
  string op = llvm_relop_op[op->get_str()];

  reg tmp_reg = allocate_register();
  buffer.emit(tmp_reg + " = icmp " + op + " i32 " + e1->get_reg() + ", " + e2->get_reg());
  int cond_br = buffer.emit("br i1 " + tmp_reg + ", label @, label @");

  return new Exp("BOOL", "", makelist(bpItem(cond_br, FIRST)), makelist(bpItem(cond_br, SECOND)));
}

Exp* emitLoad(string id, string type){
  int offset = st.get_offset(id);
  if (offest < 0){
      int reg_num = abs(id_offset) - 1;
      reg exp_reg = "%"+to_string(reg_num)
  } else{
    reg ptr_reg = allocate_register();
    reg exp_reg = allocate_register();
    buffer.emit(ptr_reg + " = getelementptr [50 x i32] , [50 x i32]* " + function_sp + ", i32 0, i32 " + to_string(offset));
    buffer.emit(exp_reg + " = load i32, i32* " + ptr_reg);
  }

  if (type == "BOOL"){
    reg res_reg = newReg();
    buffer.emit(res_reg + " = trunc i32 " + exp_reg + " to i1");
    int cond_br = buffer.emit("br i1 " + res_reg + ", label @, label @");

    return new Exp(type, res_reg, makelist(bpItem(cond_br, FIRST)), makelist(bpItem(cond_br, SECOND)));
  }

  return new Exp(type, exp_reg);
}

void emitStore(string id, string type, string val){
  int offset = st.get_offset(id);
  reg ptr_reg = allocate_register();
  buffer.emit(ptr_reg + " = getelementptr [50 x i32] , [50 x i32]* " + function_sp + ", i32 0, i32 " + to_string(offset));
  buffer.emit("store i32 " + val + ", i32* " + ptr_reg);
}
