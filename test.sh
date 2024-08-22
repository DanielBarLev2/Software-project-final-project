chmod +x clean.sh c_tests.sh python_tests.sh valgrind_tests.sh
./clean.sh #insure no leftovers executables before running

./c_tests.sh
./python_tests.sh
./valgrind_tests.sh

./clean.sh # clean executables after test