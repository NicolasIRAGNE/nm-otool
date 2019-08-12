#!/bin/bash

red=\\033[31m
green=\\033[32m
yellow=\\033[33m
blue=\\033[34m
pink=\\033[35m
cyan=\\033[36m
grey=\\033[37m
eoc=\\033[0m

all_dir="all"
black_list_dirs=(logs all srcs)
black_list_files=(compile.sh)

appendsymboliclinks()
{
	echo -e "${cyan}### $1 ###${eoc}"
	for file in $1/*;
	do
		if [ -f $file ] && [[ ! " ${black_list_files[@]} " =~ " $(basename $file) " ]];
		then
			echo $file
			ln -sfv ../$file $all_dir/$(basename $file)
		elif [ -d $file ] && [[ ! " ${black_list_dirs[@]} " =~ " $(basename $file) " ]]
		then
			appendsymboliclinks $file $(($2 + 1))
		fi
	done
}

for file in ./*;
do
	if [ -d $file ] && [[ ! " ${black_list_dirs[@]} " =~ " $(basename $file) " ]];
	then
		echo $file
		appendsymboliclinks $file 1
		#echo "ln -sf $file $all_dir/$(basename $file)"
	fi
done
