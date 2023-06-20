#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
RESULT=0
DIFF_RES=""

declare -a tests=(
"s test_grep/test_text_grep.txt VAR"
"for e_grep.c e_grep.h Makefile VAR"
"for e_grep.c VAR"
"-e for -e ^int e_grep.c e_grep.h Makefile VAR"
"-e for -e ^int e_grep.c VAR"
"-e regex -e ^print e_grep.c VAR -f test_grep/test_ptrn_grep.txt"
"-e while -e void e_grep.c Makefile VAR -f test_grep/test_ptrn_grep.txt"
"VAR test_grep/no_file.txt"
)

declare -a extra=(
"-n for test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-n for test_grep/test_1_grep.txt"
"-n -e ^\} test_grep/test_1_grep.txt"
"-c -e \/ test_grep/test_1_grep.txt"
"-ce ^int test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-e ^int test_grep/test_1_grep.txt"
"-nivh = test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-e"
"-ie INT test_grep/test_5_grep.txt"
"-echar test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-ne = -e out test_grep/test_5_grep.txt"
"-iv int test_grep/test_5_grep.txt"
"-in int test_grep/test_5_grep.txt"
"-c -l aboba test_grep/test_1_grep.txt test_grep/test_5_grep.txt"
"-v test_grep/test_1_grep.txt -e ank"
"-noe ) test_grep/test_5_grep.txt"
"-l for test_grep/test_1_grep.txt test_grep/test_2_grep.txt"
"-o -e int test_grep/test_4_grep.txt"
"-e = -e out test_grep/test_5_grep.txt"
"-noe ing -e as -e the -e not -e is test_grep/test_6_grep.txt"
"-e ing -e as -e the -e not -e is test_grep/test_6_grep.txt"
"-c -e . test_grep/test_1_grep.txt -e '.'"
"-l for no_file.txt test_grep/test_2_grep.txt"
"-f test_grep/test_3_grep.txt test_grep/test_5_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    leaks -quiet -atExit -- ./e_grep $t > test_e_grep.log
    leak=$(grep -A100000 leaks test_e_grep.log)
    (( COUNTER++ ))
    if [[ $leak == *"0 leaks for 0 total leaked bytes"* ]]
    then
      (( SUCCESS++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
#        echo "$leak"
    fi
    rm test_e_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
