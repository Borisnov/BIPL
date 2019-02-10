#include "bits/stdc++.h"
#include "state_machine.h"
#include "../../../../Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.14.26428/include/utility"

#define DB(x) cout<<(x)<<"\n";
#define DBE(x) cout<<"Error: "<<(x)<<"\n";
#define DBG(x) cout<<#x<<" = "<<x<<'\n';
using namespace std;

const string TEXT = "text";
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
//bool try_start(void (*f)());

///build poliz
struct poliz_unit{
    string status;
    string type;
    string value;
};
vector<pair<string, string>>poliz;

void poliz_add(string status, string type, string value){
    poliz_unit action;
    action.status = status;
    action.type = type;
    action.value = value;
}

void poliz_jump_here(int index){
    poliz[index].value = to_string(poliz.size());
}

///semantic functions

string curr_function_return_value = "void";
vector<map<string, string>> variables (1);  ///find index of var by its name;
map<string, string>var_type; /// find type of var by its index;
map<string, vector<string>>functions_types;

void add_function(string name, string type){
    vector<string>t;
    t.push_back(type);
    if(functions_types.find(name) != functions_types.end()){
        throw "Function with this name already exists";
    }
    functions_types[name] = t;
}

void add_empty_visibility_area(){
    map<string, string>a;
    variables.push_back(a);
}

void delete_last_visabiliy_area(){
    variables.back().clear();
    variables.pop_back();
}

vector<string>parents = {"main"};
//bool visibility_area;
int variable_index = 0;
string new_var_index(){
    string index = "var_" + to_string(variable_index);
    variable_index ++;
}

void add_variable(string name, string type){
    if(variables.back().find(name) != variables.back().end()){
        throw "Re initialization of variable";
    }
    string new_index = new_var_index();
    variables.back()[name] = new_index;
    var_type[new_index] = type;
}

string get_var_index(string name){
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
        throw ("Unexpected token " + value +
            ", expected to see " + need_value).c_str();
    }
    next();
}
pair<string, string> token_accurate_type(){
    if(type == TEXT)
}
//expression check
string expression() {
    bool begin = true;
    vector<string>stack;
    int expressiong_begin = poliz.size();
    while(true) {
        pair<string, string>curr_token = token_accurate_type();

        if(begin){
            if(curr_token.first == "value"){
                if(curr_token.second == "var") {
                    check_item(value);
                }else if(curr_token.second == "value"){
                    poliz_add("value", type, value);
                }else if(curr_token.second == "func"){

                }
            }else if(value == "-"){
                stack.push_back("b-");
            }else if(value == "("){
                stack.push_back(value);
            }else{
                throw "Wrong expression";
            }
            begin = !begin;
            next();
        }else{
            if(curr_token.first == "post") {
                string last_var_type = check_poliz(0).type;
                if(){
                    poliz_add("execute" , "" , value);
                }else
                    throw "Incorrect use of postfix operations";
            }else if(value == ")"){
                while(true){
                    if(stack.size() == 0)
                        throw "The balance of brackets is not observed";
                    if(stack.back() == "("){
                        stack.pop_back();
                        break;
                    }
                    poliz_add("execute", "", "")
                }
            }
        }
        if(b && token_accurate_type() != 1)
            throw "Wrong expression constant";
        else if(!b && token_accurate_type() != 0)
            throw "Wrong expression operator";
        next();
        b = !b;
    }
    if(b)
        throw "Wrong end of expression";
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
    add_empty_visibility_area();
    check_current("for");
    check_current("(");
    new_variable();
    check_current(";");
    int skip_action_expression=poliz.size();
    poliz_add("value", "int", "");
    poliz_add("execute", "", "transition_abs");

    int loop_begin_point = poliz.size();
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
}

void while_check() {
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

    poliz_jump_here(skip_while_block);
}

void function_parameters(string func_name) {
    while(true){
        if(type != TYPE) throw "Wrong function parameter";
        functions_types[func_name].push_back(value);
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
    delete_last_visabiliy_area();
}

void expression_action(){
    expression();
    check_current(";");
}

void one_action(){
    if(value == "if") if_check();
    else if(value == "for") for_check();
    else if(value == "while") while_check();
    else if(value == "break"){
        string parent = parents.back();
        if(!(parent == "while" || parent == "for")){
            throw "Break can be only in while or for loop";
        }
        check_current(";");
    }
    else if(value == "continue"){
        string parent = parents.back();
        if(!(parent == "while" || parent == "for")){
            throw "Break can be only in while or for loop";
        }
        check_current(";");
    }
    else if(value == "return"){
        if(curr_function_return_value == "void"){
        }else{
            string exp_type = expression();
        }
        check_current(";");
    }
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
    string var_type = type;
    next();
    while (true) {
        if(type != NAME) throw "Expected variable name";
        string var_name = value;
        next();
        if(value == "="){
            next();
            string val_type = expression();
            if(val_type != var_type)
                throw "Value of variable must be the same type as variable";
            add_item(var_name, )
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
        else throw ("Unexpected token " + value).c_str();
    }
}

string read_file(string name) {
    ifstream code_file(name);
    string code = "";
    char c;
    while (code_file.get(c)) {
        code += c;
    }
    code_file.close();
    return code;
}
///Full compilation
int main() {
    string code = read_file("test_code.txt");
    cout<<code<<endl;
    beginning_preparation();

    vector < token > tokens;

    tokens = tokenization(code);

    it = tokens.begin();
    end_it = tokens.end();
    if(it == end_it){
        cout<<"Code is empty";
        return 0;
    }
    value = it->value;
    type = it->type;

    try {
        program_body();
    }
    catch (const char* error) {
        cout << "ERROR" << '\n';
        if(it >= end_it){
            cout<<"Error has occurred at the end of file\n";
        }else{
            cout<<"Error has occurred at"
            <<"\nString num: "<<it->string_num
            <<" character number: "<<it->pos_num
            <<"\nToken = "<<value<<" token_type = "<<type;
            cout<<"\n\n";
        }
        cout << error<<'\n';
        return 0;
    }
    cout<<"SUCCESS COMPILING!" << '\n';


}