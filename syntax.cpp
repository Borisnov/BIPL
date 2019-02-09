#include "bits/stdc++.h"
#include "state_machine.h"

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

void next(){
//    cout<<"next called\n";
    if(it == end_it){
        throw "Unexpected end of file";
    }
    ++it;
    if(it == end_it){
//        throw "End of file";
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
int curr_token_type(){
    if(type == TEXT || type == INT || type == NAME)
        return 1;
    if(type == OPERATION)
        return 0;
    return 2;
}
//expression check
void expression() {
    bool b = true;
    if(curr_token_type() != 1)
        throw "Wrong expression";
    while(curr_token_type() <= 1) {
        if(b && curr_token_type() != 1)
            throw "Wrong expression constant";
        else if(!b && curr_token_type() != 0)
            throw "Wrong expression operator";
        next();
        b = !b;
    }
    if(b)
        throw "Wrong end of expression";
}

void structcheck() {
    check_current("struct");
    if(type != NAME) throw "Skipped name of struct";
    next();
    check_current("{");
    while(value != "}")
        new_variable();
    check_current("}");
}


void ifcheck() {
    check_current("if");
    check_current("(");
    expression();
    check_current(")");
    if( value == "{")
        block();
    else
        one_action();

    if(value == "else"){
        next();
        if(value == "{")
            block();
        else
            one_action();
    }
}
//void check_type(string need_type){
//    if(type != need_type)
//        throw "Unexpected token type";
//    next();
//}
//for
void forcheck() {
    check_current("for");
    check_current("(");
    expression();
    check_current(";");
    expression();
    check_current(";");
    expression();
    block();
}

//while
void whilecheck() {
    check_current("while");
    check_current("(");
    expression();
    check_current(")");
    block();
}

void function_parameters() {
    while(true){
        if(type != TYPE) throw "Wrong function parameter";
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

void funccheck() {
    if(type != TYPE) throw "Wrong function declaration";
    next();
    if(type != NAME) throw "Wrong function name";
    next();
    check_current("(");
    function_parameters();
    check_current(")");
    block();
}
void expression_action(){
    expression();
    check_current(";");
}
void one_action(){
    if(value == "if") ifcheck();
    else if(value == "for") forcheck();
    else if(value == "while") whilecheck();
    else if(type == TYPE) new_variable();
    else if(value == "break"){}
    else if(value == "continue"){}
    else if(value == "return"){}
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
    next();
    while (true) {
        if(type != NAME) throw "Expected variable name";
        next();
        if(value == "="){
            next();
            expression();
        }
        if(value == ";"){
            next();
            return;
        }
        if(value != ",") throw "Expected ,";
        next();
    }
}

//bool try_start(void (*f)()){
//    try{
//        f();
//        return true;
//    }catch(...){
//        return false;
//    }
//}

void global_block(){
    block();
}

void program_body() {
    while (it < end_it) {
        if (value == STRUCT) structcheck();
        else if (type == TYPE) funccheck();
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
    cout<<"SUCCESS COMPILING!";


}