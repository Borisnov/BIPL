#include "bits/stdc++.h"
#include "state_machine.h"

#define DB(x) cout<<(x)<<"\n";
#define DBE(x) cout<<"Error: "<<(x)<<"\n";
#define DBG(x) cout<<#x<<" = "<<x<<'\n';
using namespace std;


//all funcs after work return +1 iterstor!!
const string TEXT = "text";
const string NAME = "name";
const string INT = "integer";
const string TYPE = "type";
const string BEGINW = "begin_words";
const string END = "end";
const string OPERATION = "operation";
const string STRUCT = "struct";
const vector<string> ONESPESIALWORD = {"break" , "continue"};
const vector<string> SPESIALCOMANDS = {"return" , "bipl"};



//int status = 0;
vector<token>::iterator it , end_it;
string type, value;

void new_paramtr();
void new_variable();
void block();

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
        throw "Unexpected token " + value;
    }
    next();
}

//expression check
bool expression() {
    bool state = 1;
    while (true) {
        if(state)
        DB("exp: " + *it);
        if ((*it == NAME || *it == TEXT || *it == INT) && con == 0) {
            con = 1;
            r = 1;
            ++it;
        } else if (con == 1 and *it == OPERATION) {
            con = 0;
            r = 1;
            ++it;
        } else {
            break;
        }
    }
    if (!con && r) {
        DBE("expression is not correct")
    }
    DB("expression");
    return r;
}

void structcheck() {
    check_current("struct");
    if(type != TYPE) throw "Skipped name of struct";
    check_current("{");
    while(type != "}")
        new_variable();
    check_current("}");
}


bool ifcheck() {
    check_current("if");
    check_current("(");
    expression();
    check_current(")");
    if( value == "{")
        block();
    else{
        one_action();
    }
}

//for
bool forcheck() {
    DB("for");
    if (*it != "for")return 0;
    ++it;
    if (*it != "(")return 0;
    ++it;
    if (*it != NAME)return 0;
    ++it;
    if (*it != "in")return 0;
    ++it;
    if (*it != NAME)return 0;
    ++it;
    if (*it != ")")return 0;
    ++it;

    return block();

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
        if(value == "}"){
            next();
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
void one_action(){
    if(value == "if") ifcheck();
    else if(value == "for") forcheck();
    else if(value == "while") whilecheck();
    else if(type == TYPE) new_variable();
}

void block() {
    check_current("{");

    while (value != "}") {
        one_action();
    }
}

void new_variable() {
    if(type != TYPE) throw "Skipped name of variable";
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




//check program_body
void program_body() {
    while (it < end_it) {
        if (value == STRUCT) structcheck();
        else if (type == TYPE) funccheck();
        else throw "Unexpected token " + value;
    }
}

string read_file(string name) {
    ifstream code_file("test_code.txt");
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
    //DBG(code);
    beginning_preparation();

    vector < token > tokens;

    tokens = tokenization(code);

    it = tokens.begin();
    end_it = tokens.end();
    value = it->value;
    type = it->type;

    try {
        program_body();
    }
    catch (const char* error) {
        cout << "ERROR" << '\n';
        cout << error;
    }


}