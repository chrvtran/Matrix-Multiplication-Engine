binary="bench interactive"

make clean; make

score=0

for file in $binary; do
    if [[ ! -f "$file" ]]; then
        echo "FAIL: $file not made"
        echo "SCORE: $score/7"
        break 
    fi
done


for (( i = 0; i < 2; i++ )); do
    echo "TEST: ./test $i"
    ./test $i > /dev/null
    if [ "$?" -eq 1 ]; then
    ((score+=1))
    else
    echo "  --FAIL!"
    fi
done



echo "TEST: plotted images are present"
if [ -f "plot-single.png" ]; then
    ((score+=1))
else
    echo "  --FAIL!"
fi

echo "TEST: plotted images are present"
if [ -f "plot-multi.png" ]; then
    ((score+=1))
else
    echo "  --FAIL!"
fi

echo "TEST: nice benchmarking"
if [ -f "bench-nice.sh" ] && [ -f "plot-nice.png" ]; then
    ((score+=1))
else
    echo "  --FAIL!"
fi

echo "TEST: taskset benchmarking"
if [ -f "bench-taskset.sh" ] && [ -f "plot-taskset.png" ]; then
    ((score+=1))
else
    echo "  --FAIL!"
fi

for (( i = 2; i < 5; i++ )); do
    echo "TEST: ./test $i"
    output=$(./test $i)
    if [ "$?" -eq 1 ]; then
    ((score+=1))
    else
    echo "  --FAIL!"
    echo "$output"
    fi
done

echo "TEST: questions.txt is non-empty and contains questions (1) through (N)"
N=3
if [ -f questions.txt ] && [ "$(wc -c < questions.txt)" -gt 200 ]; then
    passed=true
    for ((i=1; i<=N; i++)); do
        if ! grep -q "^($i)" questions.txt; then
            echo "  --FAIL: questions.txt missing ($i)"
            passed=false
        fi
    done
    if $passed; then
        ((score+=1))
    fi
else
    echo "  --FAIL!"
fi

echo "SCORE: $score/10"
