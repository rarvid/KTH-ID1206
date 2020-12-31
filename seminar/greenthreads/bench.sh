printf "pthread exec time\tgreen exec time\tpthread counter\tgreen counter\n"
for i in {1000..100000..1000}
do
./bench.out $i
done
