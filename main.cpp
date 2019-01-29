#include "bits/stdc++.h"
#include "state_machine.h"
using namespace std;

int main(){
    //cout<<"test";
    //return  0;
    ///OPEN CODE FILE FOR EXAMPLE
    ifstream code_file("test_code.txt");
    string code = "";
    char c;
    //code_file>>c;
    //cout<<c;
    while(code_file.get(c) ){
        //code_file>>c;
        code += c;
        //cout <<c;
    }

    code += '\n';

    //cout<<code;
    code_file.close();

    ///!!!!!!! FOR BEGIN WORKING
    begining_preparation();
    //cout<<code;
    //vector <pair<string, string>> tokens;

    ///GET TOKENIZING VECTOR
    auto tokens = tokenization(code);
    cout<<tokens.size()<<endl;
    ///VISUALIZING RESULT

    for ( int i = 0; i < tokens.size(); i++){
        cout<<tokens[i].first<<" "<<tokens[i].second<<'\n';
    }
    cout<<endl;
    cout<<syntax_string(code);

}
