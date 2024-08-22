make > /dev/null
test_count=0
test_command() {
    command="$1"

    test_count=$((test_count + 1))

    valgrind --leak-check=full $command 2> valgrind_output.txt > /dev/null
    if grep -q "in use at exit: 0 bytes in 0 blocks" valgrind_output.txt && grep -q "All heap blocks were freed -- no leaks are possible" valgrind_output.txt; then
        echo "test $test_count - SUCCESS"
    else
        echo "test $test_count - FAIL"
    fi
}

echo "########## VALGRIND TESTS ##########"
test_command "./symnmf norm tests/input_4.txt"
test_command "./symnmf ddg tests/input_4.txt"
test_command "./symnmf sym tests/input_4.txt"
test_command "./symnmf norm tests/inpt_4.txt"
test_command "./symnmf symnmf tests/input_4.txt"
test_command "./symnmf a tests/input_4.txt"

rm valgrind_output.txt
