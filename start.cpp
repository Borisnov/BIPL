#include "bits/stdc++.h"
#include "state_machine.h"
#include "compiler.h"

#define print(x) cout<<x<<endl;

using namespace std;

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



int main() {
    string code = read_file("test_code.txt");

    cout<<code<<endl;

    result_report result = compile(code);

    if(!result.was_error)
        cout<<"SUCCESS COMPILING!" << '\n';
    else {
        print("ERROR");
        print(result.error);
        cout << ("Line: ") << result.line << "\n";
        cout << "Position: " << result.position << '\n';
    }
}