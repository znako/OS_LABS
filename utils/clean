#!/bin/bash

main_dir=$(pwd)
utils_dir="${main_dir}/utils"
legal_dir="${utils_dir}/legal"

if [ ${1} != "main" ]
then
    cd "${main_dir}/${1}"
fi

legal_files=$(cat ${legal_dir}/${1}.txt)
all_files=$(ls)
deleting_files=""

for var1 in ${all_files}
do
    flag=1

    for var2 in ${legal_files}
    do
        if [ ${var1} == ${var2} ]
        then
            flag=0
        fi
    done

    if [ ${flag} == 1 ]
    then
        deleting_files+="${var1} "
    fi
done

echo "[clean] These files will be deleted:"
echo "[clean] ${deleting_files}[1/0]"
read answer

if [ ${answer} == 1 ]
then
    for var in ${deleting_files}
    do
        rm -rf ${var}
    done
fi