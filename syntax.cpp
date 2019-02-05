#include "bits/stdc++.h"
#include "state_machine.h"

#define DB(x) cout<<(x)<<"\n";
#define DBE(x) cout<<"Error: "<<(x)<<"\n";
#define DBG(x) cout<<#x<<" = "<<x<<'\n';
using namespace std;

int status = 0;
vector<string>::iterator it , endit , now;
//all funcs after work return +1 iterstor!!
const string TEXT = "text";
const string NAME = "name";
const string INT = "integer";
const string TYPE = "type";
const string BEGINW = "begin_words";
const string END = "end";
const string SPESIALS = "operation";
const string STRUCT = "struct";
const vector<string> ONESPESIALWORD = {"break" , "continue"};
const vector<string> SPESIALCOMANDS = {"return" , "bipl"};

bool new_paramtr();

bool new_variable();

bool block();

// check (import) "the bipl"
bool begin_words() {
    DB("begin words");
    string stat = *++it;
    ++it;
    return stat == TEXT;
}

//spesial commands
bool spesial() {
    for (int i = 0; i < SPESIALCOMANDS.size(); i++) {
        if (*it == SPESIALCOMANDS[i]) {
            ++it;
            if (*it == NAME || *it == TEXT || *it == INT) {
                ++it;
                return 1;
            } else return 0;
        }
    }
    for (int i = 0; i < ONESPESIALWORD.size(); i++) {
        if (*it == ONESPESIALWORD[i]) {
            ++it;
            return 1;
        }
    }
    return 0;
}

//expression check
bool expression() {
    bool con = 0 , r = 0;
    while (1) {
        DB("exp: " + *it);
        if ((*it == NAME || *it == TEXT || *it == INT) && con == 0) {
            con = 1;
            r = 1;
            ++it;
        } else if (con == 1 and *it == SPESIALS) {
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

bool struct_check() {
    if (*it != STRUCT) {
        return 0;
    }
    ++it;
    if (*it != NAME) {
        return 0;
    } else {
        DBE("not found `name` in the struct");
    }
    ++it;
    if (*it != "{")return 0;
    else {
        DBE("not found `{` in the struct");
    }
    ++it;
    while (new_variable()) { DB(*it); }
    if (*it != "}")return 0;
    else {
        DBE("not found `}` in the struct");
    }
    ++it;
    return 1;

}

// if check
bool ifcheck() {
    DB("If");
    bool r = 1;
    if (*it != "if")return 0;
    ++it;
    if (*it == "(") {
        ++it;
        r = r && expression();
    } else return 0;

    if (*it != ")") {
        DBE("in IF not `)`");
        return 0;
    }
    ++it;
    r = r && block();
    if (*it == "else") {
        r = r && block();
    }

    return r;
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
bool whilecheck() {
    DB("while");
    if (*it != "while")return 0;
    ++it;
    if (*it != "(")return 0;
    ++it;
    if (!expression())return 0;
    if (*it != ")")return 0;
    ++it;

    return block();

}

//checkfunk
bool checkfunk() {
    now = it;
    DB("checkfunk");
    if (*it != NAME) {
        it = now;
        return 0;
    }
    ++it;
    if (*it != "(") {
        it = now;
        return 0;
    }
    ++it;
    while (*it == NAME || *it == TEXT || *it == INT) {
        ++it;
    }
    if (*it != ")") {
        it = now;
        return 0;
    }
    ++it;
    return 1;
}

bool block() {
    DB("block");
    if (*it != "{")return 0;
    ++it;
    bool r = 1 , com = 0;

    while (*it != "}") {

        if (*it == TYPE) {
            if (!new_variable()) {
                return 0;
            };
        } else if (*it == "if") {
            if (!ifcheck()) {
                return 0;
            };
        } else if (*it == "for") {
            if (!forcheck()) {
                return 0;
            };
        } else if (*it == "while") {
            if (!whilecheck()) {
                return 0;
            };
        } else if (checkfunk()) {
            com = 1;
        } else if (spesial()) {

            com = 1;
        } else
            //if it is expression, we need add `;`
        if (expression()) {
            com = 1;
        } else {
            DBE(*it);
            return 0;
        }
        //every command must end as ';'
        if (com) {
            com = 0;
            if (*it != ";") {
                DBE("not `;`");
                return 0;
            }
            ++it;
        }
        DB("new word:" + *it);
    }
    ++it;

    return r;
}

//new values
bool new_variable() {
    DB("new_value");
    bool r = 1;
    if (*(it) == TYPE) {
        ++it;
    }
    while (*it != ";") {
        if (*(it) == NAME) {
            ++it;
            if (*it == ",") {
                ++it;
            }
        } else {
            DBE("not found `;` in new_variable")
            return 0;
        }
    }
    ++it;
    return r;
}

//parametrs on function
bool new_paramtr() {
    DB("new_paramtr");
    bool r = 1;

    while (*it != ")") {
        if (*(it) == TYPE and *(++it) == NAME) {
            it += 1;

            if (*it == ",") {
                ++it;
            }
        } else {
            DBE(" `)` is no close");
            return 0;
        };

    }
    ++it;
    return r;
}

//check function
bool funk() {
    DB("funk");
    bool r = 1;
    if (*it != TYPE) {
        return 0;
    };
    ++it;
    if (*it != NAME)return 0;
    ++it;
    if (*it != "(")return 0;
    ++it;
    r = r && new_paramtr();

    r = r && block();

    return r;
}

//check programm
bool programm() {

    bool r = 1;
    while (it < endit) {

        if (*it == BEGINW) {
            r = r && begin_words();
        } else if (*it == STRUCT) {
            r = r && struct_check();
        } else if (*it == TYPE) {
            r = r && funk();
            if (!r)DBE("fuction is not correct");
        } else if (*it == END)return r;
        else {
            DBE(*it);
            return 0;
        }
        if (!r)return 0;
    }

    return r;

}

//begin - prdword
//funk
//
vector<string> split(string text , char d) {
    vector<string> v;
    string word = "";
    for (int i = 0; i < text.size(); i++) {

        if (text[i] != d)word += text[i];
        if (text[i] == d) {
            v.push_back(word);
            word = "";
        }
    }
    return v;
}

string read_file(string name) {
    ifstream code_file("test_code.txt");
    string code = "";
    char c;
    while (code_file.get(c)) {
        code += c;
    }
    code += '\n';
    code_file.close();
    return code;
}

int main() {
    string code = read_file("test_code.txt");
    //DBG(code);
    beginning_preparation();
    code = syntax_string(code);
    
    vector<string> tokens;

    tokens = split(code , ' ');

    it = tokens.begin();
    endit = tokens.end();

    DB("start");

    try {
        if (programm()) {
            cout << "OK";
        } else {
            cout << "Bad";
        }
    }
    catch (...) {
        cout << "error";
    }


}