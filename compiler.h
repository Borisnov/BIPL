#include "bits/stdc++.h"
#include "state_machine.h"

using namespace std;

#define DBG(x) cout << "DBG output : "<<#x << " = " << x << endl;

const string TEXT = "string";
const string NAME = "name";
const string INT = "integer";
const string TYPE = "type";
const string OPERATION = "operation";
const string STRUCT = "struct";


vector<token>::iterator it , end_it;
string type, value;

void new_variable();
void block();
void one_action();

///build poliz

struct poliz_unit{
    string status;
    string type;
    string value;
};
vector<poliz_unit>poliz;
vector<int>break_loop;
int last_loop_begin;

void poliz_add(string status, string type, string value){
    poliz_unit action;
    action.status = status;
    action.type = type;
    action.value = value;
    poliz.push_back(action);
}

poliz_unit check_poliz(int begin, int k){
    if(poliz.size() - 1 - k < begin)
        throw "Expression begin error";
    if(k >= poliz.size()){
        throw "Error checking old tokens";
    }
    return poliz[poliz.size() - 1 - k];
}

void poliz_jump_here(int index){
    poliz[index].value = to_string(poliz.size());
}

void poliz_break_here() {
    for(int i = 0; i < break_loop.size(); i++){
        poliz_jump_here(break_loop[i]);
    }
    break_loop.clear();
}

///semantic functions

struct function_data{
    string return_type;
    vector<string>arg_types;
};

string curr_function_return_value = "void";
vector<map<string, string>> variables (1);  ///find index of var by its name;
map<string, string>get_var_type; /// find type of var by its index;
map<string, function_data>functions_types;
vector<string>parents = {"main"};

bool was_return_value;
int variable_index = 0;

bool check_function(string name){
    return (functions_types.find(name) != functions_types.end());
}

void add_function(string name, string type){
    function_data f;
    f.return_type = type;
    if(check_function(name)){
        throw "Function with this name already exists";
    }
    functions_types[name] = f;
}

void add_empty_visibility_area(){
    map<string, string>a;
    variables.push_back(a);
}

void delete_last_visibility_area(){
    variables.back().clear();
    variables.pop_back();
}

string new_var_index(){
    string index = "var_" + to_string(variable_index);
    variable_index ++;
    return index;
}

void add_variable(string name, string type){
    if(variables.back().find(name) != variables.back().end()){
        throw "Re initialization of variable";
    }
    string new_index = new_var_index();
    variables.back()[name] = new_index;
    get_var_type[new_index] = type;
}

string check_variable(string name){
    if(check_function(name))
        throw "Variable have same name as function";
    for(int i = variables.size() - 1; i >= 0; i--){
        if(variables[i].find(name) != variables[i].end()){
            return variables[i][name];
        }
    }
    throw "Unknown variable was found";
}

///syntax functions

void next(){
    if(it == end_it){
        throw "Unexpected end of file";
    }
    ++it;
    if(it == end_it){
        type = "END_OF_FILE";
        value = "END_OF_FILE";
        return;
    }

    type = it->type;
    value = it->value;
}

void check_current(string need_value){
    if(value != need_value){
        throw "Unexpected token detected";
    }
    next();
}

/// main part of recursive descent

// for expression building
int op_priority(string op){
    if(op == "||") return 1;
    if(op == "&&") return 2;
    if(op == "==" || op == "!=") return 3;
    if(op == ">" || op == "<" || op == ">=" || op == "<=")
        return 4;
    if(op == "+" || op == "-") return 5;
    if(op == "*" || op == "/") return 6;
    return -1;
}

string curr_token_status(){
    string t = value;
    if(check_function(value)) return "begin";
    if(type == TEXT || type == INT) return "begin";
    if(type == NAME) return "begin";
    if(value == "(" || value == ")") return "brackets";
    if(t == "++" || t == "--") return "post";
    if(t == "=") return "equal";
    if(t == "+=" || t == "-=") return "equal";
    if(t == "*=" || t == "/=") return "equal";
    if(op_priority(value) != -1) return "operation";
    return "not_expression";
}
string expression();

string call_function(){
    string func_name = value;
    next();
    int ars_size = functions_types[func_name].arg_types.size();
    string return_type = functions_types[func_name].return_type;
    check_current("(");
//    DBG(ars_size);
    for(int i = 0; i < ars_size; i++){
        string exp_type = expression();
        string arg_type = functions_types[func_name].arg_types[i];
//        DBG(arg_type);
        if(arg_type != exp_type)
            throw "Function call parameters are different from accepted";
        if(i == ars_size - 1){
            if(value != ")")
                throw "Wrong function call";
        }
        else
            check_current(",");
    }
    poliz_add("func", return_type, func_name);
    return return_type;
}

string expression() {
    bool must_value = true;
    vector<string>stack;
    int expression_begin = poliz.size();
    while(true) {
        string status = curr_token_status();
        //begin not_expression post equal operation
//        DBG(status)
        if(status == "not_expression")
            break;
        if(must_value){
            if(status == "begin"){
                if(check_function(value)){
                    call_function();
                }else if(type == NAME){
                    string ind = check_variable(value);
                    poliz_add("ptr", get_var_type[ind], ind);
                }else if(type == INT){
                    poliz_add("value", "int", value);
                }else if(type == TEXT){
                    poliz_add("value", "string", value);
                }
                must_value = false;
            }else if(value == "-"){
                stack.push_back("-b");
            }else if(value == "("){
                stack.push_back(value);
            }else{
                throw "Wrong expression";
            }
        }else{
            if(status == "post") {

                string last_token_status = check_poliz(expression_begin, 0).status;
                if(last_token_status == "ptr"){
                    poliz_add("execute" , "operation" , value);
                }else
                    throw "Incorrect use of postfix operations";
            }else if(value == ")"){
                bool exp_end = false;
                while(true){
                    if(stack.size() == 0){
                        exp_end = true;
                        break;
                    }
                        //throw "The balance of brackets is not observed";
//                    DBG(stack.back())
                    if(stack.back() == "("){
                        stack.pop_back();
                        break;
                    }
                    poliz_add("execute", "operation", stack.back());
                    stack.pop_back();
                }
                if(exp_end)break;
            }else if(status == "equal"){
                //return branch
                string curr_val = value;
                if(stack.size() != 0)
                    throw "Before equal operation must be only one variable";
                poliz_unit last = check_poliz(expression_begin, 0);
                if(last.status != "ptr")
                    throw "Before equal operation must be variable";
                next();
                string res_type = expression();
                if(res_type != last.type)
                    throw "Wrong type of equal statement";
                else{
                    poliz_add("execute", "equal", curr_val);
                    return res_type;
                }

            }else if(status == "operation"){
                must_value = true;
                int curr_prior = op_priority(value);
                while(stack.size() > 0 && op_priority(stack.back()) > curr_prior){
                    poliz_add("execute", "operation", stack.back());
                    stack.pop_back();
                }
                stack.push_back(value);
            } else
                throw "Unexpected token status in expression";
        }
        next();
    }
    if(must_value)
        throw "Wrong end of expression";

    while(stack.size()){
        poliz_add("execute", "operation", stack.back());
        stack.pop_back();
    }

    ////check types of all operation operators
    vector<string>types;
    for(int i = expression_begin; i < poliz.size(); i++) {
        poliz_unit curr = poliz[i];
//        DBG(curr.value)
//        DBG(curr.type)
        if (curr.status == "value" || curr.status == "ptr") {
            types.push_back(curr.type);
        }else if(curr.status == "func"){
            for(int arg = 0; arg <
                functions_types[curr.value].arg_types.size();
                arg ++){
                types.pop_back();
            }
            types.push_back(functions_types[curr.value].return_type);
        }else if (curr.status == "execute") {
            string v = curr.value;
            if (v == "++" || v == "--") {
                if (types.size() == 0)throw "Expression unknown error";
                if (types.back() != "int")
                    throw "Post operations must be integer type";
            } else if (v == "-" || v == "*" || v == "/") {
                if (types.size() < 2)throw "Expression unknown error";
                if (types.back() != "int" || types[types.size() - 2] != "int")
                    throw "Arithmetic operation must be using with numeric types";
                types.pop_back();
            }else if (op_priority(v) > 0 && op_priority(v) <= 4 || v == "+") {
                if (types.size() < 2)throw "Expression unknown error";
                string t1 = types.back();
                types.pop_back();
                string t2 = types.back();
                types.pop_back();
                if(v == "+"){
                    types.push_back(t1);
                    if(t1 != t2) throw "Plus statements must be one type";
                    if(t1 != "int" && t1 != "string")
                        throw "+ can be applied only with string or int";
                    continue;
                }
                if (t1 != t2) {
                    throw "Compare operations must use one type values";
                }
                types.push_back("bool");
            } else {
                throw "Unknown expression error";
            }
        }else
            throw "Unknown token status in expression";
    }
    if(types.size() != 1){
        throw "Expression size error";
    }
    return types[0];
}

void struct_check() {
    check_current("struct");
    if(type != NAME) throw "Skipped name of struct";
    next();
    check_current("{");
    while(value != "}")
        new_variable();
    check_current("}");
}

void if_check() {
    check_current("if");
    check_current("(");
    string exp_type = expression();
    if(exp_type != "bool")
        throw "Is statement must have bool type";
    check_current(")");
    int skip_if_body_pointer = poliz.size();
    poliz_add("value", "int", "");
    poliz_add("execute", "", "transition_lie");
    parents.push_back("if");
    if( value == "{")
        block();
    else
        one_action();

    int skip_else_statement = poliz.size();
    poliz_add("value", "int", "");
    poliz_add("execute", "", "transition_abs");

    poliz_jump_here(skip_if_body_pointer);

    if(value == "else"){
        next();
        if(value == "{")
            block();
        else
            one_action();
    }
    poliz_jump_here(skip_else_statement);
    parents.pop_back();
}

void for_check() {
    parents.push_back("for");
    add_empty_visibility_area();

    check_current("for");
    check_current("(");
    new_variable();
    check_current(";");
    int skip_action_expression=poliz.size();
    poliz_add("value", "int", "");
    poliz_add("execute", "", "transition_abs");

    int loop_begin_point = poliz.size();
    last_loop_begin = loop_begin_point;
    expression();
    check_current(";");
    poliz_jump_here(skip_action_expression);

    string exp_type = expression();
    if(exp_type != "bool"){
        throw "<FOR> last condition must be bool";
    }
    check_current(")");
    int skip_loop_block = poliz.size();
    poliz_add("value", "int", "");
    poliz_add("execute", "", "transition_lie");
    block();
    poliz_add("value", "int", to_string(loop_begin_point));
    poliz_add("execute", "", "transition_abs");
    poliz_jump_here(skip_loop_block);
    poliz_break_here();
    parents.pop_back();
    delete_last_visibility_area();
}

void while_check() {
    parents.push_back("while");
    int begin_while_loop = poliz.size();
    last_loop_begin = begin_while_loop;

    check_current("while");
    check_current("(");
    string exp_type = expression();
    if(exp_type != "bool"){
        throw "<While> condition must be bool";
    }
    check_current(")");
    int skip_while_block = poliz.size();
    poliz_add("value", "int", "");
    poliz_add("execute", "", "transition_lie");
    block();

    poliz_add("value", "int", to_string(begin_while_loop));
    poliz_add("execute", "", "transition_abs");

    poliz_break_here();
    poliz_jump_here(skip_while_block);
    parents.pop_back();
}

void function_parameters(string func_name) {
    if(value == ")"){
        return;
    }
    while(true){
        if(type != TYPE) throw "Wrong function parameter";
        functions_types[func_name].arg_types.push_back(value);
        next();
        if(type != NAME) throw "Wrong function parameter name";
        next();
        if(value == ")"){
            return;
        }
        if(value != ",") throw "Unexpected symbol in function parameters";
        next();
    }
}

void func_check() {
    was_return_value = false;
    parents.push_back("func");
    add_empty_visibility_area();

    if(type != TYPE) throw "Wrong function declaration";

    curr_function_return_value = value;
    next();

    if(type != NAME) throw "Wrong function name";
    string func_name = value;
    add_function(value, curr_function_return_value);
    next();

    check_current("(");
    function_parameters(func_name);
    check_current(")");

    block();
    curr_function_return_value = "void";
    delete_last_visibility_area();
    parents.pop_back();

    if(curr_function_return_value != "void" && !was_return_value)
        throw "This function must return some value";
}

void expression_action(){
    expression();
    check_current(";");
}

void break_check() {
    check_current("break");
    string parent = parents.back();
    if(!(parent == "while" || parent == "for")){
        throw "Break can be only in while or for loop";
    }
    check_current(";");
    break_loop.push_back(poliz.size());
    poliz_add("value", "int", "");
    poliz_add("execute", "", "transition_abs");
}

void continue_check(){
    check_current("continue");
    check_current(";");
    string parent = parents.back();
    if(!(parent == "while" || parent == "for")){
        throw "continue can be only in while or for loop";
    }
    poliz_add("value", "int", to_string(last_loop_begin));
    poliz_add("execute", "", "transition_abs");
}

void return_check(){
    check_current("return");
    was_return_value = true;
    if(curr_function_return_value != "void"){
        string exp_type = expression();
        if(curr_function_return_value != exp_type)
            throw "Return value must be same type as function";
    }
    check_current(";");
}

void one_action(){
    if(value == "if") if_check();
    else if(value == "for") for_check();
    else if(value == "while") while_check();
    else if(value == "break") break_check();
    else if(value == "continue")continue_check();
    else if(value == "return")return_check();
    else if(type == TYPE) new_variable();
    else expression_action();
}

void block() {
    check_current("{");
    while (value != "}") {
        one_action();
    }
    check_current("}");
}

void new_variable() {
    if(type != TYPE) throw "Skipped type of variable";
    string var_type = value;
    next();
    while (true) {
        if(type != NAME) throw "Expected variable name";
        string var_name = value;
        add_variable(var_name, var_type);

        next();
        if(value == "="){
            next();
            string val_type = expression();
            if(val_type != var_type)
                throw "Value of variable must be the same type as variable";
        }
        if(value == ";"){
            next();
            return;
        }
        if(value != ",") throw "Expected ,";
        next();
    }
}

void global_block(){
    block();
}

void program_body() {
    while (it < end_it) {
        if (value == STRUCT) struct_check();
        else if (type == TYPE) func_check();
        else if (value == "{") global_block();
        else{
            throw "Unexpected token in program body";
        }
    }
}




struct result_report{
    bool was_error = false;
    string error;
    int line;
    int position;
    string error_token;
};

///Full compilation
result_report compile(string code) {
    result_report report;

    beginning_preparation();
    vector < token > tokens;
    pair<string, vector < token >>diagnostic = tokenization(code);

    try {
        tokens = diagnostic.second;
        it = tokens.begin();
        end_it = tokens.end();


        if(it == end_it){
            if(diagnostic.first != ""){
                report.error = diagnostic.first;
                report.was_error = true;
                report.line = 0;
                report.position = 0;
                report.error_token = "_null_";
            }
            return report;
        }
        value = it->value;
        type = it->type;
        program_body();
    }
    catch (const char* error) {
        report.was_error = true;
        report.error = (string)error;

        if(it >= end_it)
            it = --end_it;

        report.line = it->string_num;
        report.position = it->pos_num;
        report.error_token = it->value;
    }
    return report;
}