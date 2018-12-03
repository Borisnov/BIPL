#include "bits/stdc++.h"
#include "extra_files/json.hpp"

using namespace std;

vector<pair<string, string>>result_tokens;
string curr_token = "";

void add(string type, string value){
    result_tokens.push_back(make_pair(type, value));
    curr_token = "";
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
    while(char_index < code_length){
        c = code[char_index];

        if(state == "begin"){
            if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_')
                state = "name";
            else if(c >= '0' && c <= '9')
                state = "number"

        }else if(state == "number"){

        }else if(state == "name"){
            if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' ||
            c >= '0' || c <= '9' || c == '_'){
                curr_token += c;
                char_index ++;
            }
            else{
                state = "begin";
                add(curr_token);
            }
        }
    }


    return 0;
}
