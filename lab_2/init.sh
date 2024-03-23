gcc simple.c
./a.out output file*
rm a.out

sleep 1

if test -f output; then
    cat output
else
    echo "Some error occured"
fi

# rm output
