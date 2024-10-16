rm -rf */

/usr/bin/cmake --build /home/vpert/GitRepos/labs/5_cg/lab_2/build --config Debug --target all --

# FS -- Floyd-Steinberg
# SK -- Stucki
# SR -- Sierra

for method in "FS" "SK" "SR";
do
    dirname="test_$method"
    mkdir $dirname
    cd $dirname
    
    for i in $(seq 1 8);
    do
        /home/vpert/GitRepos/labs/5_cg/lab_2/build/bin/main \
            --img=/home/vpert/GitRepos/labs/5_cg/pics/dog_3.png \
            --bpp=$i \
            --out="$i.png" \
            --$method
    done
    
    echo $method
    cd ..
done


