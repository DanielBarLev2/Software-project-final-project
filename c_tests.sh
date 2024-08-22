make 2> tmp.txt >/dev/null
test_count=0
test() {
    local command="$1"
    local required_output_file="$2"

    test_count=$((test_count + 1))

    eval "$command" >tests/my_output.txt
    diff "$required_output_file" tests/my_output.txt >/dev/null
    if [ $? -ne 0 ]; then
        echo "test $test_count - FAIL"
    else
        echo "test $test_count - SUCCESS"
    fi
}


echo "########## C TESTS ##########"
if grep -q "error:" tmp.txt; then
    echo "compilation errors - FAIL"
elif grep -q "warning:" tmp.txt; then
    echo "compilation warnings - FAIL"
else
    echo "compilation - SUCCESS"
fi
test "./symnmf norm tests/input_4.txt" "tests/norm_output_4.txt"
test "./symnmf ddg tests/input_4.txt" "tests/ddg_output_4.txt"
test "./symnmf sym tests/input_4.txt" "tests/sym_output_4.txt"
test "./symnmf norm tests/inpt_4.txt" "tests/an_error_has_acc.txt"
test "./symnmf symnmf tests/input_4.txt" "tests/an_error_has_acc.txt"
test "./symnmf a tests/input_4.txt" "tests/an_error_has_acc.txt"

rm tmp.txt
