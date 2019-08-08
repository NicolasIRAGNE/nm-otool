#!/bin/bash

((index=2))
otool_options="td"
while [ $index -lt $(($# + 1)) ];
do
	otool_options="$otool_options ${!index}"
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

green="\033[32m"
red="\033[91m"
eoc="\033[39m"

make -C .. -j8
otool_dir=..
otool_name=ft_otool

log_dir=logs/diff_logs # WARNING gets deleted recursively
diff_file_list="${log_dir}/diff_files.log"

make -C $otool_dir >/dev/null 2>&1

mkdir -p $log_dir
rm -rf ${log_dir}/*
rm $diff_file_list 2>/dev/null

((i=0))
((j=0))

for file in $dir/*;
do
	echo "processing ===> $file <===="
	basename=$(basename $file)
	trace_folder="${log_dir}/${basename}"
	mkdir -p $trace_folder

	my_trace="${trace_folder}/my_output"
	otool_trace="${trace_folder}/otool_output"
		
	my_err="${trace_folder}/my_err"
	otool_err="${trace_folder}/otool_err"

	./$otool_dir/$otool_name $otool_options $file > $my_trace 2>$my_err
	otool -td $otool_options $file 2>$otool_err \
		| sed '/^.*: is not an object file$/d' > $otool_trace 

	diff_trace=${trace_folder}/diff_trace
	diff $my_trace $otool_trace > $diff_trace
	if [ $? -ne 0 ];
	then
		echo -e "${red}diff for file ${file} logged in ${trace_folder}${eoc}"
		echo $file >> $diff_file_list
		cp $file ${trace_folder}
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
