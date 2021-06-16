/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "math.h"

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
    else if (power%2 == 0)
        return pow(base, power/2)*pow(base, power/2);
    else
        return base*pow(base, power/2)*pow(base, power/2);
}