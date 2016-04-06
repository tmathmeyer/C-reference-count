#!/bin/bash
echo "#include \"scut.h\"" > .scut_tmp.c
echo "#include \".scut_tmp.h\"" >> .scut_tmp.c
echo "void RUN_TESTS() { " >> .scut_tmp.c
echo "#ifndef _SCUT_TMP_H_" > .scut_tmp.h
echo "#define _SCUT_TMP_H_" >> .scut_tmp.h
for k in $(gcc -E $1 | grep -o '__scut_fork__[a-zA-Z0-9_]*')
do
    echo $k >> .scut_tmp.c
    echo "char * $k();" >> .scut_tmp.h
    echo "();" >> .scut_tmp.c
done
echo "}" >> .scut_tmp.c
echo "#endif" >> .scut_tmp.h

gcc -c $1 -o $1.o
mv $1.o $1.o.o
gcc -c .scut_tmp.c -o .scut_tmp.o

ld -r $1.o.o .scut_tmp.o -o $1.o

#rm .scut_tmp.c .scut_tmp.h $1.o.o .scut_tmp.o

