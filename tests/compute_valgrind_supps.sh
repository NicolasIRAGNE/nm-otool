empty_binary="__empty__binary__"
empty_main="__test_empty__test__.c"
supp_file="my_supp.supp"
echo "int main(void){ return (0); }" > $empty_main
gcc -o $empty_binary $empty_main
valgrind --leak-check=full --gen-suppressions=all "./${empty_binary}" 2>&1 | perl supp_getter.perl > $supp_file
rm $empty_binary
rm $empty_main
rm -rf "${empty_binary}.dSYM"
