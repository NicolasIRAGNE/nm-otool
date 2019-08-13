#!/bin/sh

nm_binary="../ft_nm"
file_to_test="__tmp_file__"

if [ -z $1 ];
then
	echo usage: $0 binary
	exit 1;
elif [ ! -f $1 ]
then
	echo "$1 is not a file"
	exit 1
fi

make -C ..
size=$( wc -c < $1 | sed s/\ //g) 

i=1
ret=0;
while [ $i -le $size ]
do
	#echo "cat $1 | head -c $i > $file_to_test && ./${nm_binary} $file_to_test"
	
	echo "testing for $i/$size..."

	cat $1 | head -c $i > $file_to_test && nm $file_to_test
	cat $1 | head -c $i > $file_to_test && ./${nm_binary} $file_to_test
	ret=$?
	if [ ! $ret -eq 0 ] && [ ! $ret -eq 1 ];
	then
		echo "olalalalalala for $i"
		exit 1;
	fi
	echo "OK for i = ${i}/${size}"
	i=$(($i+1))
done
echo "OK for all!"
