/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "math.h"
int abs(int n);

int add(int num1, int num2){
    int val = num1 + num2;
    return val;
}

int max(int num1, int num2){
    if(num1 > num2){
        return num1;
    }
    else if(num1 < num2){
        return num2;
    }
    else if(num1 = num2){
        return;
    }
}

int sub(int num1, int num2){
    int val = num1 - num2;
    return val;
}

int mult(int num1, int num2){
    int val = num1 * num2;
    return val;
}

int div(int num1, int num2){
    int val = num1 / num2;
    return val;
}

int pow(int base, int power)
{
    if (power == 0)
        return 1;
    else{
        int a = 1;
        for(int i = 0; i < abs(power); i++) { 
            a = a*base;
        }
        if (power < 0) { //Makes all negative powers 0 functionally, but this is what you would hypothetically do to find the real value
            a = 1/a;
        }
        return a;
    }
}

int abs(int num) { //adds abs for above function
    if (num < 0) {
        return num*-1;
    }
    else {
        return num;
    }
}