#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <cctype>
#include <stdlib.h>

using namespace std;

string masterParser(string expression, int position);
string exponentiationParser(string expression);
string multiplicationParser(string expression);
string additionParser(string expression);
string removeDecZeroes(string s);

int layer = 0;

int main(){
    string expression = "";
    double answer = 0;

    cout << "Current expression - Procession part - Layers deep (parenthesis)\n";
    cout << "Please write in a mathematical expression. nth-root format is n%num\n";
    getline(cin, expression);

    cout << "";

    int position = expression.find(' ');
    while(position > -1){
        expression.erase(position, 1);
        position = expression.find(' ');
    }

    try{
        for(int i = 0; i < expression.length(); i++){
            if(!isdigit(expression[i]) && expression[i] != '(' && expression[i] != ')' && expression[i] != '^' && expression[i] != '%' && expression[i] != '*' && expression[i] != '/' && expression[i] != '+' && expression[i] != '-' && expression[i] != '.'){
                throw "Unexpected Symbol";
            }
        }
        answer = stod((masterParser(expression, 0).c_str()));
    }catch(const char* eS){
        cerr << "Error: " << eS << endl;

        cout << "\nPress enter to exit\n";
        string dummy;
        getline(cin, dummy);
        return EXIT_FAILURE;
    }catch(...){
        cerr << "Unexpected Error, Please confirm that the expression is correct\n";

        cout << "\nPress enter to exit\n";
        string dummy;
        getline(cin, dummy);
        return EXIT_FAILURE;
    }

    cout << answer << " - " << "Answer" << endl;

    cout << "\nPress enter to exit\n";
    string dummy;
    getline(cin, dummy);
    return 0;
}

string masterParser(string expression, int position){
    if(expression.find(')') != -1){
        for(int i = position; i < expression.length()+1; i++){
            if(expression[i] == '('){
                layer++;
                cout << expression << " - " << "Parenthesis" << " - " << layer << endl;
                expression = masterParser(expression, i+1);
                i = position;
            }else if(expression[i] == ')'){
                expression.replace(position-1, i-position+2, additionParser(multiplicationParser(exponentiationParser(expression.substr(position, i-position)))));
                layer--;
                return expression;
            }
        }
    }
    return additionParser(multiplicationParser(exponentiationParser(expression)));
}

string exponentiationParser(string expression){
    int start = 0;
    int secondStart = 0;
    double a = 0;
    double b = 0;
    string toNumber = "";
    bool powerTo = false;
    bool afterSymbol = false;

    for(int i = 0; i < expression.length()+1; i++){
        if(!afterSymbol){
            if(isdigit(expression[i]) || expression[i] == '.' || (expression[i] == '-' && i == start)){
                toNumber += expression[i];
            }else if(expression[i] == '^'){
                powerTo = true;
                afterSymbol = true;
                secondStart = i+1;
                a = stod(toNumber.c_str());
                toNumber = "";
            }else if(expression[i] == '%'){
                afterSymbol = true;
                secondStart = i+1;
                a = stod(toNumber.c_str());
                toNumber = "";
            }else{
                start = i+1;
                a = 0;
                b = 0;
                toNumber = "";
            }
        }else{
            if(isdigit(expression[i]) || expression[i] == '.' || (expression[i] == '-' && i == secondStart)){
                toNumber += expression[i];
            }else{
                b = stod(toNumber.c_str());
                toNumber = "";
                expression.replace(start, i-start, to_string((powerTo) ? pow(a, b) : pow(b, (double)1/a)));
                a = 0;
                b = 0;
                toNumber = "";
                powerTo = false;
                afterSymbol = false;
                start = 0;
                secondStart = 0;
                i = -1;
            }
        }
    }
    expression = removeDecZeroes(expression);
    cout << expression << " - " << "Exponentiation" << " - " << layer << endl;
    return expression;
}

string multiplicationParser(string expression){
    int start = 0;
    int secondStart = 0;
    double a = 0;
    double b = 0;
    string toNumber = "";
    bool multiplication = false;
    bool afterSymbol = false;

    for(int i = 0; i < expression.length()+1; i++){
        if(!afterSymbol){
            if(isdigit(expression[i]) || expression[i] == '.' || (expression[i] == '-' && i == start)){
                toNumber += expression[i];
            }else if(expression[i] == '*'){
                multiplication = true;
                afterSymbol = true;
                secondStart = i+1;
                a = stod(toNumber.c_str());
                toNumber = "";
            }else if(expression[i] == '/'){
                afterSymbol = true;
                secondStart = i+1;
                a = stod(toNumber.c_str());
                toNumber = "";
            }else{
                start = i+1;
                a = 0;
                b = 0;
                toNumber = "";
            }
        }else{
            if(isdigit(expression[i]) || expression[i] == '.' || (expression[i] == '-' && i == secondStart)){
                toNumber += expression[i];
            }else{
                b = stod(toNumber.c_str());
                toNumber = "";
                expression.replace(start, i-start, to_string((multiplication) ? a*b : a/b));
                a = 0;
                b = 0;
                toNumber = "";
                multiplication = false;
                afterSymbol = false;
                start = 0;
                secondStart = 0;
                i = -1;
            }
        }
    }
    expression = removeDecZeroes(expression);
    cout << expression << " - " << "Multiplication" << " - " << layer << endl;
    return expression;
}

string additionParser(string expression){
    int start = 0;
    int secondStart = 0;
    double a = 0;
    double b = 0;
    string toNumber = "";
    bool addition = false;
    bool afterSymbol = false;

    for(int i = 0; i < expression.length()+1; i++){
        if(!afterSymbol){

            if(isdigit(expression[i]) || expression[i] == '.' || (expression[i] == '-' && i == start)){
                toNumber += expression[i];
            }else if(expression[i] == '+'){
                addition = true;
                afterSymbol = true;
                secondStart = i+1;
                a = stod(toNumber.c_str());
                toNumber = "";
            }else if(expression[i] == '-'){
                afterSymbol = true;
                secondStart = i+1;
                a = stod(toNumber.c_str());
                toNumber = "";
            }else{
                start = i+1;
                a = 0;
                b = 0;
                toNumber = "";
            }
        }else{
            if(isdigit(expression[i]) || expression[i] == '.' || (expression[i] == '-' && i == secondStart)){
                toNumber += expression[i];
            }else{
                b = stod(toNumber.c_str());
                toNumber = "";
                expression.replace(start, i-start, to_string((addition) ? a+b : a-b));
                a = 0;
                b = 0;
                toNumber = "";
                addition = false;
                afterSymbol = false;
                start = 0;
                secondStart = 0;
                i = -1;
            }
        }
    }
    expression = removeDecZeroes(expression);
    cout << expression << " - " << "Addition" << " - " << layer << endl;
    return expression;
}

string removeDecZeroes(string str){
    int aDec = -1;
    int dEnd = 0;

    for(int i = 0; i < str.length()+1; i++){
        if(aDec > -1){
            if(dEnd == 0 && i-aDec > 1 && str[i] == '0'){
                aDec = i;
                dEnd = i;
            }else if(str[i] == '0'){
                dEnd = i;
            }else if(isdigit(str[i]) && dEnd > 0 && str[i] != '0'){
                aDec = i+1;
            }else if(!isdigit(str[i])){
                str.erase(aDec, dEnd);
                aDec = -1;
                dEnd = 0;
            }
        }
        if(str[i] == '.')
            aDec = i;
    }
    return str;
}
