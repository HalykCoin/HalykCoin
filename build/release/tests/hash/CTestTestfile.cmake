# CMake generated Testfile for 
# Source directory: /home/oleg/Projects/halykcoin/tests/hash
# Build directory: /home/oleg/Projects/halykcoin/build/release/tests/hash
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(hash-fast "/home/oleg/Projects/halykcoin/build/release/tests/hash/hash-tests" "fast" "/home/oleg/Projects/halykcoin/tests/hash/tests-fast.txt")
add_test(hash-slow "/home/oleg/Projects/halykcoin/build/release/tests/hash/hash-tests" "slow" "/home/oleg/Projects/halykcoin/tests/hash/tests-slow.txt")
add_test(hash-slow-1 "/home/oleg/Projects/halykcoin/build/release/tests/hash/hash-tests" "slow-1" "/home/oleg/Projects/halykcoin/tests/hash/tests-slow-1.txt")
add_test(hash-tree "/home/oleg/Projects/halykcoin/build/release/tests/hash/hash-tests" "tree" "/home/oleg/Projects/halykcoin/tests/hash/tests-tree.txt")
add_test(hash-extra-blake "/home/oleg/Projects/halykcoin/build/release/tests/hash/hash-tests" "extra-blake" "/home/oleg/Projects/halykcoin/tests/hash/tests-extra-blake.txt")
add_test(hash-extra-groestl "/home/oleg/Projects/halykcoin/build/release/tests/hash/hash-tests" "extra-groestl" "/home/oleg/Projects/halykcoin/tests/hash/tests-extra-groestl.txt")
add_test(hash-extra-jh "/home/oleg/Projects/halykcoin/build/release/tests/hash/hash-tests" "extra-jh" "/home/oleg/Projects/halykcoin/tests/hash/tests-extra-jh.txt")
add_test(hash-extra-skein "/home/oleg/Projects/halykcoin/build/release/tests/hash/hash-tests" "extra-skein" "/home/oleg/Projects/halykcoin/tests/hash/tests-extra-skein.txt")
