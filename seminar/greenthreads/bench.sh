printf "g_time\tg_counter\n"
for i in {1000..100000..1000}
do
./g_bench.out $i
done
printf "p_time\tp_counter\n"
for i in {1000..100000..1000}
do
./p_bench.out $i
done
