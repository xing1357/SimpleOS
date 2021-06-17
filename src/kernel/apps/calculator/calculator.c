/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "calculator.h"
#include "../apps.h"

int getfirstNumber(){
    print_string("\nEnter First Number: ");
    string num1 = readStr();
    int numb1 = str_to_int(num1);
    kfree(num1);
    return numb1;
}

int getsecondNumber(){
    print_string("\nEnter Second Number: ");
    string num2 = readStr();
    int numb2 = str_to_int(num2);
    kfree(num2);
    return numb2;
}

void calculator(){
    clearScreen();
    print_string("Calculator\n");
    print_string("1. Addition\n2. Subtraction\n3. Multiplication\n4. Division\n5. Power\n");
    print_string("Enter Operation Of Your Choice: ");
    string op = readStr();
    if(strcmp(op,"1")){
        int num1 = getfirstNumber();
        int num2 = getsecondNumber();
        int ans = add(num1, num2);
        print_string("\n");
        print_int(ans);
        print_string("\n");
    }
    
    else if(strcmp(op,"2")){
        int num1 = getfirstNumber();
        int num2 = getsecondNumber();
        int ans = sub(num1, num2);
        print_string("\n");
        print_int(ans);
        print_string("\n");
    }
    
    else if(strcmp(op,"3")){
        int num1 = getfirstNumber();
        int num2 = getsecondNumber();
        int ans = mult(num1, num2);
        print_string("\n");
        print_int(ans);
        print_string("\n");
    }

    else if(strcmp(op,"4")){
        int num1 = getfirstNumber();
        int num2 = getsecondNumber();
        int ans = div(num1, num2);
        print_string("\n");
        print_int(ans);
        print_string("\n");
    }

    else if(strcmp(op,"5")){
        int num1 = getfirstNumber();
        int num2 = getsecondNumber();
        int ans = pow(num1, num2);
        print_string("\n");
        print_int(ans);
        print_string("\n");
    }
}