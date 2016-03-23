make re;
for (( i = 0; i < 50; i++ )); do
    ./lemipc ./ $((RANDOM%6+1)) &
done
