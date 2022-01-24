#include "code_gen.hpp"

unsigned long long register_count = 0;
reg function_sp = "";

CodeBuffer& buffer = CodeBuffer::instance();

unordered_map<string, string> llvm_relop_op =
        {
            {"==", "eq"},
            {"!=", "ne"},
            {"<", "slt"},
            {">", "sgt"},
            {"<=", "sle"},
            {">=", "sge"},
        };

string get_return_type_of_binop(Exp* e1, Exp* e2){
  return (e1->get_type() == e2->get_type()) && (e1->get_type() == "BYTE") ? "BYTE" : "INT";
}

void initBuffer(){
  string errorMsg = "Error division by zero";

  string declarations = "declare i32 @printf(i8*, ...)\n";
  declarations += "declare void @exit(i32)\n";
  declarations += "@div_by_zero_error = constant [" + std::to_string(errorMsg.length()+1) + " x i8] c\"" + errorMsg + "\\00\"";
  buffer.emitGlobal(declarations);

  string specifiers = "@.str_specifier = internal constant [4 x i8] c\"%s\\0A\\00\"\n";
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
  string op_cmd = (op->get_str() == "+") ? "add" : "sub";
  string result_reg = allocate_register();
  buffer.emit(result_reg + " = " + op_cmd + " i32 " + e1->get_reg() + ", " + e2->get_reg());

  string result_type = get_return_type_of_binop(e1, e2);
  if (result_type == "BYTE"){
      reg new_result_reg = allocate_register();
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
  string errorMsg = "Error division by zero";
  string len = to_string(errorMsg.length()+1);
  buffer.emit("call void @print(i8* getelementptr ([" + len + " x i8], [" + len + " x i8]* " + "@div_by_zero_error" + ", i32 0, i32 0))");
  buffer.emit("call void @exit(i32 1)");
  int cont_br = buffer.emit("br label @"); // end of error block
  string cont_label = buffer.genLabel();

  bpList is_zero =  buffer.makelist(bpItem(check_zero_br, FIRST));
  bpList not_zero = buffer.merge(buffer.makelist(bpItem(check_zero_br, SECOND)), buffer.makelist(bpItem(cont_br, FIRST)));
  buffer.bpatch(is_zero, error_label);
  buffer.bpatch(not_zero, cont_label);
}

Exp* emitMulDiv(Exp* e1, Value* op, Exp* e2){
  string op_cmd = (op->get_str() == "*") ? "mul" : "sdiv";

  if (op_cmd == "sdiv") {
    generate_check_div_by_zero_code(e2);
  }

  reg res_reg = allocate_register();
  buffer.emit(res_reg + " = " + op_cmd + " i32 " + e1->get_reg() + ", " + e2->get_reg());

  string res_type = get_return_type_of_binop(e1,e2);
  if (res_type == "BYTE"){
    reg new_res_reg = allocate_register();
    buffer.emit(new_res_reg + " = and i32 255, " + res_reg);
    res_reg = new_res_reg;
  }

  return new Exp(res_type, res_reg);
}

Exp* emitRelop(Exp* e1, Value* op, Exp* e2){
  string op_cmd = llvm_relop_op[op->get_str()];

  reg tmp_reg = allocate_register();
  buffer.emit(tmp_reg + " = icmp " + op_cmd + " i32 " + e1->get_reg() + ", " + e2->get_reg());
  int cond_br = buffer.emit("br i1 " + tmp_reg + ", label @, label @");

  return new Exp("BOOL", "", buffer.makelist(bpItem(cond_br, FIRST)), buffer.makelist(bpItem(cond_br, SECOND)));
}

Exp* emitLoad(string id, string type){
  int id_offset = st.get_offset_by_id(id_name);
  if(id_offset < 0)
  { // function argument - have %n, n is place of arg decleration in function
      int reg_num = abs(id_offset) - 1;

          if(type == "BOOL"){
              reg exp_reg = allocate_register();
              CodeBuffer::instance().emit(exp_reg + " = trunc i32 " + "%"+to_string(reg_num) + " to i1");
              int index = CodeBuffer::instance().emit("br i1 " + exp_reg + ", label @, label @");
              bpList truelst = CodeBuffer::instance().makelist(bpItem(index, FIRST));
              bpList falselst = CodeBuffer::instance().makelist(bpItem(index, SECOND));
              return new Exp(type, exp_reg, truelst, falselst);
          }

      return new Exp(type, "%"+to_string(reg_num));
  }
  reg ptr_reg = allocate_register();
  CodeBuffer::instance().emit(ptr_reg + " = getelementptr [50 x i32] , [50 x i32]* " + function_sp + ", i32 0, i32 " + to_string(id_offset));
  reg exp_reg = allocate_register();
  CodeBuffer::instance().emit(exp_reg + " = load i32, i32* " + ptr_reg);
  if(type == "BOOL"){
      reg exp_bool_reg = allocate_register();
      CodeBuffer::instance().emit(exp_bool_reg + " = trunc i32 " + exp_reg + " to i1");
      int index = CodeBuffer::instance().emit("br i1 " + exp_bool_reg + ", label @, label @");
      bpList truelst = CodeBuffer::instance().makelist(bpItem(index, FIRST));
      bpList falselst = CodeBuffer::instance().makelist(bpItem(index, SECOND));
      return new Exp(type, exp_bool_reg, truelst, falselst);
  }
  return new Exp(type, exp_reg);
}

void emitStore(string id, string type, string val){
  int id_offset = st.get_offset_by_id(id);
  reg ptr_reg = allocate_register();
  buffer.emit(ptr_reg + " = getelementptr [50 x i32] , [50 x i32]* " + function_sp + ", i32 0, i32 " + to_string(id_offset));
  buffer.emit("store i32 " + val + ", i32* " + ptr_reg);
}

void bool_handler(Exp* e1){
  string true_label = buffer.genLabel();
  buffer.bpatch(e1->true_list, true_label);

  int branch = buffer.emit("br label @\n");
  bpList phi_true = buffer.makelist(bpItem(branch, FIRST));

  string false_label = buffer.genLabel();
  buffer.bpatch(e1->false_list, false_label);

  branch = buffer.emit("br label @\n");
  bpList phi_false = buffer.makelist(bpItem(branch, FIRST));

  string phi_label = buffer.genLabel();
  reg new_reg = allocate_register();
  buffer.emit(new_reg + "= phi i32 [ 1, %" + true_label + "], [ 0, %" + false_label + "]");
  buffer.bpatch(buffer.merge(phi_true, phi_false), phi_label);

  e1->set_reg(new_reg);
}

Exp* emitCallFunc(string r_type, string func_id, const vector<string>& param_types, const vector<string>& param_regs){
  if (func_id == "print"){
    buffer.emit("call void @print(i8* " + param_regs[0] + ")");
    return new Exp(r_type, func_id);
  }
  string return_type = (r_type == "VOID") ? "void" : "i32";
  reg new_exp_id = (r_type != "VOID") ? allocate_register() : func_id;
  string to_emit = (r_type != "VOID") ? (new_exp_id + " = ") : "";

  to_emit += "call " + return_type + " @" + func_id + "(";
  for(size_t i = 0; i < param_types.size(); i++) to_emit += (i == param_types.size()-1) ? "i32 " + param_regs[i] : "i32 " + param_regs[i] + ", ";
  to_emit += ")";
  buffer.emit(to_emit);
  return new Exp(r_type, new_exp_id);
}
