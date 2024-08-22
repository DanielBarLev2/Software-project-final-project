test_count=0
test() {
    local command="$1"
    local required_output_file="$2"
    
    test_count=$((test_count + 1))

    eval "$command" > tests/my_output.txt
    diff "$required_output_file" tests/my_output.txt > /dev/null
    if [ $? -ne 0 ]; then
        echo "test $test_count - FAIL test case: $command"
    else
        echo "test $test_count - SUCCESS"
    fi
}

python3 setup.py build_ext --inplace 2> tmp.txt >/dev/null
echo "########## PYTHON TESTS ##########"
if grep -q "error:" tmp.txt; then
    echo "build errors - FAIL"
elif grep -q "warning:" tmp.txt; then
    echo "build warnings - FAIL"
else
    echo "build - SUCCESS"
fi
test "python3 symnmf.py 6 norm tests/input_4.txt" "tests/norm_output_4.txt"
test "python3 symnmf.py 6 ddg tests/input_4.txt" "tests/ddg_output_4.txt"
test "python3 symnmf.py 6 sym tests/input_4.txt" "tests/sym_output_4.txt"
test "python3 symnmf.py 6 symnmf tests/input_4.txt" "tests/symnmf_output_4.txt"

rm tmp.txt