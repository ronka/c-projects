echo "Cleaning last executable files"
make clean
echo "Remaking project"
make

echo ""
echo ""

echo "Testing on files: test1 test2"
./assembler test1 test2 test3