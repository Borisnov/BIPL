#include "bits/stdc++.h"
#include "extra_files/json.hpp"

using namespace std;
using json = nlohmann::json;

//For more convenient work
#define DBG(x) cout<<#x<<" = "<<x<<'\n';

map<string, set<string>>tokens_in_category;
vector<pair<string, string>>result_tokens;
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

int main(){
    ///Preparing types from json data file
    json data_json;
    ifstream data_types("tokens_types.json");
    data_types >> data_json;
    data_types.close();
    for(json::iterator it = data_json.begin(); it != data_json.end(); ++it){
        string key = it.key();
        for(auto t : it.value()){
            string value = t;
            all_tokens.insert(value);
            token_type[value] = key;
            tokens_in_category[key].insert(value);
        }
    }

    ///Open input code file
    ifstream code_file("test_code.txt");
    string code = "";
    char c;
    while(code_file.get(c))code += c;
    code_file.close();
    //code += (char)(0);
    //DBG(code)

    ///Initializing all necessary
    int code_length = code.length();
    int char_index = 0;
    string state = "begin";

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
                else if(find_token("syntax", string(1, c))){
                    char_index ++;
                    curr_token += c;
                    add("syntax");
                }else if(find_token("operation", string(1, c)))
                    state = "operation";
                else if(c == '"' || string(1, c) == "'")
                    state = "string";
                else throw string("Unexpected symbol " + c);
            }
            else if(state == "operation"){
                if(find_token("operation", curr_token + c)){
                    curr_token += c;
                    char_index ++;
                }else{
                    state = "begin";
                    add("operation");
                }
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
                if(c >= '0' || c <= '9'){
                    curr_token += c;
                    char_index ++;
                }else{
                    if(code[char_index - 1] == '.')
                        throw string("Wrong float record");
                    add("float");
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
                    add("string");
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
                    if(find_token("reserved_word", curr_token))
                        type = token_type[curr_token];
                    add(type);
                }
            }
        }
    }catch(string error){
        cout<<"Error position = "<<char_index<<'\n';
        cout<<error<<'\n';
    }
    ofstream result_pair("code_tokens.txt");
    for(auto t : result_tokens){
        cout<<t.second<<" "<<t.first<<endl;
        result_pair<<t.first<<" "<<t.second<<'\n';
    }
    result_pair.close();


    return 0;
}
