#include "bits/stdc++.h"
#include "state_machine.h"


int main(){
    ///OPEN CODE FILE FOR EXAMPLE
    ifstream code_file("test_code.txt");
    string code = "";
    char c;
    while(code_file.get(c))code += c;
    code += '\n';
    code_file.close();

    ///!!!!!!! FOR BEGIN WORKING
    begining_preparation();

    vector<pair<string, string>>tokens;

    ///GET TOKENIZING VECTOR
    tokens = tokenization(code);


    ///VISUALIZING RESULT
    for(int i = 0; i < tokens.size(); i++){
        cout<<tokens[i].first<<" "<<tokens[i].second<<'\n';
    }

}
