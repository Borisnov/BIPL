# BIPL

## Структура проекта.

### 1. gramma.cpp

+ Тут осуществляется проверка программы на логику и граматику. Должны отлавливаться ошибки незакрытых скобочек, точек с зяпятой, непраильное использование встроеных функций и нарушение общей структуры программы.
+ Проверка реализована рекурсивным спуском в соответсвующие функции, самые низкоуровневые из которых осуществляют разбор на уровне последовательности логических субъедениц.
+ Для анализа программа использует подготовленый заранее токен из служебных слов и символов.

Пример запуска проверки синтаксиса:
```
  ifstream code_file("test_code.txt");
  string code = "";
  char c;
  while(code_file.get(c) ){
      //code_file>>c;
      code += c;
      //cout <<c;
  }

  code += '\n';
  code_file.close();

  begining_preparation();
  code = syntax_string(code);
  cout<<code<<"\n";

  vector <string> tokens;
  tokens=split(code,' ');
  it = tokens.begin();
  endit=tokens.end();


  try{
      if(programm()){
          cout<<"OK";
      } else{
          cout<<"Bad";
      }
  }
  catch (...){
      cout<<"error";
  }

```
