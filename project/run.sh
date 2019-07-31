echo "Cleaning last executable files"
make clean
echo "Remaking project"
make

echo ""
echo ""

echo "Testing on files: test1"
./assembler test