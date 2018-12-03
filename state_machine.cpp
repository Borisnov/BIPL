#include "bits/stdc++.h"
#include "extra_files/json.hpp"

using namespace std;

vector<pair<string, string>>result_tokens;
string curr_token = "";
set<string>all_tokens;
map<string, string>token_type;

void add(string type){
    result_tokens.push_back(make_pair(type, curr_token));
    curr_token = "";
}

bool find_token(string token){
    return all_tokens.count(token);
}

int main(){
    ///Open input code file
    ifstream code_file("test_code.txt");
    string code = "";
    char c;
    while(code_file.get(c))code += c;
    code_file.close();

    ///Initializing all necessary

    int code_length = code.length();
    int char_index = 0;

    string state = "begin";

    ///Loop of whole code string
    try{
        while(char_index < code_length){
            c = code[char_index];

            if(state == "begin"){
                if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_')
                    state = "name";
                else if(c >= '0' && c <= '9')
                    state = "number";

            }else if(state == "integer"){
                if(c >= '0' || c <= '9'){
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
                        throw "Wrong float record";
                }
            }else if(state == "name"){
                if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ||
                c >= '0' || c <= '9' || c == '_'){
                    curr_token += c;
                    char_index ++;
                }
                else{
                    state = "begin";
                    string type = "name";
                    if(find_token(curr_token))
                        type = token_type[curr_token];
                    add(type);
                }
            }
        }
    }catch(string error){
        cout<<"Error position = "<<char_index<<'\n';
        cout<<error;
    }



    return 0;
}
