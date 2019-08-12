#!/bin/bash

((index=2))
nm_options=""
while [ $index -lt $(($# + 1)) ];
do
	nm_options="$nm_options ${!index}"
	index=$((index+1))
done;

if [ -z $1 ];
then
	echo usage: $0 binaries_directory
	exit 1
elif [ -d $dir ]
then
	dir=$1
else
	echo "$1 directory does not exists"
	exit 1
fi

./compute_valgrind_supps.sh
valgrind_supps=my_supp.supp
error_exit_code=23


green="\033[32m"
red="\033[91m"
eoc="\033[39m"

nm_dir=..
nm_name=ft_nm

log_dir=logs/valgrind_logs

make -C $nm_dir >/dev/null 2>&1

mkdir -p $log_dir
rm -rf ${log_dir}/*

((i=0))
((j=0))
for file in $dir/*;
do
	echo "processing ===> $file <===="
	basename=$(basename $file)
	trace_folder="${log_dir}/${basename}"
	mkdir -p $trace_folder
	trace=$trace_folder/valgrind_trace
	#echo "processing: valgrind --leak-check=full --error-exitcode=$error_exit_code --suppressions=$valgrind_supps --log-file=$tmp_trace ./asm $file > /dev/null 2>&1"
	valgrind --leak-check=full --error-exitcode=$error_exit_code --suppressions=$valgrind_supps --log-file=$trace ./$nm_dir/$nm_name $nm_options $file > /dev/null 2>&1
	ret=$?
	if [ $ret -eq $error_exit_code ] || ([ $ret -ne 1 ] && [ $ret -ne 0 ]);
	then
		echo -e "${red}valgrind errors for file ${file} logged in ${trace}${eoc}"
	else
		echo -e "${green}no valgrind error for file ${file}${eoc}"
		rm -rf ${trace_folder}
		i=$((i+1))
	fi
	j=$((j+1))
done

if [ $i -eq $j ]
then
	echo -e "${green}no valgrind errors at all ! ($i/$j tests passed) !${eoc}"
else
	echo -e "${red}only $i/$j tests passed${eoc}"
fi
