#include "bits/stdc++.h"
#include "compiler.h"

#define print(x) cout<<x<<endl;
string POLIZ_FILE_NAME = "POLIZ.txt";

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

string align_output(string str, int size=10){
    size -= str.size();
    for(int i = 0; i < size; i++)
        str += ' ';
    return str;
}


int main(int argc, char *argv[]) {
//    string code_file = argv[1];
    string code_file = "test_code.txt";
//    cout<<code_file;
    string code = read_file(code_file);
//    cout<<code<<endl;

    result_report result = compile(code);


// Генерирую json строку для IDE. И вывожу в cout

//    cout<<"{\n";
//    cout<<"    \"was_error\" : "<<result.was_error<<", \n";
//    cout<<"    \"error_line\" : "<<result.line<<", \n";
//    cout<<"    \"error_text\" : \""<<result.error<<"\", \n";
//    cout<<"    \"error_token\" : \""<<result.error_token<<"\", \n";
//    cout<<"    \"error_position\" : "<<result.position<<"\n";
//    //cout<<"    \"re\" : "<<result.position<<"\n";
//    cout<<"}";

//    Дебаговый вывод результата компиляции программы.
    if(!result.was_error) {
        cout << "SUCCESS COMPILING!" << '\n';
        ofstream poliz_file(POLIZ_FILE_NAME);

//        Генерируем нормальное представление полиза из массива piliz
        int poliz_string_count = 0;
        for(vector<poliz_unit>::iterator unit = poliz.begin(); unit != poliz.end(); ++unit){
            poliz_file << poliz_string_count ++ << '\t';
            poliz_file << align_output(unit->status, 12)<<align_output(unit->type, 15)<<align_output(unit->value);
            poliz_file << '\n';
        }
        poliz_file.close();
    }else {
        print("ERROR");
        print(result.error);
        cout << ("Line: ") << result.line << "\n";
        cout << "Position: " << result.position << '\n';
    }

    return 0;
}
