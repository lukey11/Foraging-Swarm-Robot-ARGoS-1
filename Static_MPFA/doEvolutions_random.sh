mkdir -p launch
echo "Create the folder -launch-"
logfilename="${1##*/}_$(date +%F_%H:%M:%S,%N)"

#nohup mpirun -n $1 -machinefile $2 run_2_3types_ga.sh Powerlaw_4nests.xml Random_4nests.xml Cluster_4nests.xml>>launch/${logfilename}_stdout.log 2>> launch/${logfilename}_stderr.log &

nohup mpirun -n $1 -machinefile $2 run_5_1type_ga.sh Random_static_MPFA_r96_50by50.xml>>launch/${logfilename}_stdout.log 2>> launch/${logfilename}_stderr.log &

echo "DONE"

