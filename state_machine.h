#include "bits/stdc++.h"
//#include "extra_files/json.hpp"

using namespace std;
//using json = nlohmann::json;

///For more convenient work
#define DBG(x) cout<<#x<<" = "<<x<<'\n';

map< string, set <string> >tokens_in_category;
vector<pair<string, string> >result_tokens;
string curr_token = "";
set<string>all_tokens;
map<string, string>token_type;


void add(string type){
    result_tokens.push_back(make_pair(type, curr_token));
    curr_token = "";
}

bool find_token(string category, string token){
    return tokens_in_category[category].count(token);
}
vector<pair<string, vector<string>>>tokens_initializing = {
        {"syntax",{
            "{",
            "}",
            "(",
            ")",
            "<",
            ">",
            ";",
            ",",
            "[",
            "]",
            "//",
            "/*",
            "*/"
        }},
        {"operation",{
            "+",
            "-",
            "*",
            "/",
            "&",
            "^",
            "!",
            "=",
            "||",
            "|",
            "&&",
            "%",
            "==",
            "++",
            "--",
            "+=",
            "*=",
            "/=",
            "%="
        }},
        {"type",{
            "int",
            "string",
            "double"
         }},
        {"reserved_word",{
            "in",
            "for",
            "while",
            "if",
            "else",
            "return",
            "bipl",
            "break",
            "continue"
        }}
    };
///Start one time before using tokenization function;
void begining_preparation(){
    ///Preparing types from json data file
    //json data_json;
    //ifstream data_types("tokens_types.json");
    //data_types >> data_json;
    //data_types.close();


    for(auto it : tokens_initializing){
        string key = it.first;
        for(auto t : it.second){
            string value = t;
            all_tokens.insert(value);
            token_type[value] = key;
            tokens_in_category[key].insert(value);
        }
    }
}
///main function to tokenizing code string
vector<pair<string, string>> tokenization(string code){
    code += '\n';
    vector<pair<string, string>>empty_tokens;
    ///Initializing all necessary
    result_tokens.clear();
    curr_token = "";
    int code_length = code.length();
    int char_index = 0;

    string state = "begin";
    char c;
    ///Loop of whole code string
    try{
        while(char_index < code_length){
            c = code[char_index];

            if(state == "begin"){
                if(c == ' ' || c == '\n' || c == '\t'){
                    char_index ++;
                }else if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_')
                    state = "name";
                else if(c >= '0' && c <= '9')
                    state = "integer";
                else if(all_tokens.count(string(1, c)))
                    state = "combination";
                else if(c == '"' || string(1, c) == "'")
                    state = "string";
                else throw string("Unexpected symbol " + c);
            }
            else if(state == "combination"){
                if(all_tokens.count(curr_token + c)){
                    curr_token += c;
                    char_index ++;
                }else{
                    state = "begin";
                    if(curr_token == "//")
                        state = "line_comment",
                        curr_token = "";
                    else if(curr_token == "/*")
                        state = "multiline_comment",
                        curr_token = "";
                    else if(curr_token == "*/")
                        throw string("Unexpected (*/) closing multi line comment");
                    else
                        add(token_type[curr_token]);
                }
            }else if(state == "line_comment"){
                if(c == '\n')state = "begin";
                else char_index ++;
            }else if(state == "multiline_comment"){
                if(c == '*' && code[char_index + 1] == '/'){
                    char_index += 2;
                    state = "begin";
                }else char_index ++;
            }else if(state == "integer"){
                if(c >= '0' && c <= '9'){
                    curr_token += c;
                    char_index ++;
                }else if(c == '.'){
                    char_index ++;
                    curr_token += c;
                    state = "float";
                }else{
                    state = "begin";
                    add("integer");
                }
            }else if(state == "float"){
                if(c >= '0' && c <= '9'){
                    curr_token += c;
                    char_index ++;
                }else{
                    if(code[char_index - 1] == '.')
                        throw string("Wrong float record");
                    add("integer");///float
                    state = "begin";
                }
            }else if(state == "string"){
                if(curr_token == "" || c != curr_token[0]){
                    curr_token += c;
                    char_index ++;
                }else{
                    curr_token += c;
                    char_index ++;
                    state = "begin";
                    add("text");
                }
            }else if(state == "name"){
                if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ||
                c >= '0' && c <= '9' || c == '_'){
                    curr_token += c;
                    char_index ++;
                }
                else{
                    state = "begin";
                    string type = "name";
                    if(token_type.find(curr_token) != token_type.end()){
                        type = token_type[curr_token];
                    }
                    add(type);
                }
            }
        }
        if(state == "multiline_comment")
            throw string("Multi line comments wasn't closed");
        else if(state == "string")
            throw string("String constant wasn't closed");
        else if(state != "begin")
            throw string("UNEXPECTED ERROR! state = " + state);
    }catch(string error){
        return empty_tokens;
        //ofstream result_pair("code_tokens.txt");
        //result_pair<<"Error position = "<<char_index<<'\n';
        //result_pair<<error<<'\n';
        //result_pair.close();
        //return 0;
    }
    return result_tokens;
}

string syntax_string(string code){
    vector <pair<string, string>> tokens;

    ///GET TOKENIZING VECTOR
    tokens = tokenization(code);
    string res= "";
    for(auto token : tokens){
        if(token.first == "reserved_word" || token.first == "syntax"){
            res += token.second;
        }else{
            res += token.first;
        }
        res += ' ';
    }
    return res;
}
