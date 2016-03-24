make re;
for (( i = 0; i < 5000; i++ )); do
    ./lemipc ./ $((RANDOM%6+1)) &
done
