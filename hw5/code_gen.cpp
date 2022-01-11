#include "code_gen.hpp"


unsigned long long register_count = 0;

reg allocate_register(){ return "%t" + to_string(++register_count); }
reg allocate_global_register(){ return "g" + to_string(++register_count); }
