#!/bin/bash
try() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" != "$expected" ]; then
        echo "$input $expected, but got $actual"
        exit 1
    fi
}

try 0 '0;'
try 42 '42;'
try 21 '5+20-4;'
try 41 '12 + 34 - 5;'
try 144 '12*12;'
try 6 '36/6;'
try 47 '5+6*7;'
try 15 '5*(9-6);'
try 4 '(3+5)/2;'
try 4 'a=2; a+2;'
try 10 'a=2; b=3+2; a*b;'
try 1 '3==3;'
try 0 '3==2;'
try 1 '3!=2;'
try 1 'a=2; b=3+2; a*b==10;'

echo OK
