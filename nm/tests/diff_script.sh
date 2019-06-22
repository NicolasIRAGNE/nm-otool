#!/bin/bash

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

green="\033[32m"
red="\033[91m"
eoc="\033[39m"

nm_dir=..
nm_name=ft_nm

log_dir=logs/diff_logs # WARNING gets deleted recursively
diff_file_list="${log_dir}/diff_files.log"

make -C $nm_dir >/dev/null 2>&1

mkdir -p $log_dir
rm -rf ${log_dir}/*
rm $diff_file_list

((i=0))
((j=0))

for file in $dir/*;
do
	echo "processing ===> $file <===="
	basename=$(basename $file)
	trace_folder="${log_dir}/${basename}"
	mkdir -p $trace_folder

	my_trace="${trace_folder}/my_output"
	nm_trace="${trace_folder}/nm_output"
		
	my_err="${trace_folder}/my_err"
	nm_err="${trace_folder}/nm_err"

	./$nm_dir/$nm_name $file > $my_trace 2>$my_err
	nm $file > $nm_trace 2>$nm_err

	diff_trace=${trace_folder}/diff_trace
	diff $my_trace $nm_trace > $diff_trace
	if [ $? -ne 0 ];
	then
		echo -e "${red}diff for file ${file} logged in ${trace_folder}${eoc}"
		echo $file >> $diff_file_list
	else
		echo -e "${green}no diff for file ${file}${eoc}"
		rm -rf ${trace_folder}
		i=$((i+1))
	fi
	j=$((j+1))
done

if [ ! -f $diff_file_list ]
then
	echo -e "${green}no diffs at all ! ($i/$j tests passed) !${eoc}"
else
	echo -e "${red}only $i/$j tests passed${eoc}"
fi
