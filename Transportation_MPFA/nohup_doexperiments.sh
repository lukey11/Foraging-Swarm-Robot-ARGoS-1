mkdir -p launch
logfilename="${1##*/}_$(date +%F_%H:%M:%S,%N)"

nohup mpirun -n $1 -machinefile $2 experiments.py>>launch/${logfilename}_stdout.log 2>> launch/${logfilename}_stderr.log &

#the first parameter is the number of process; the second is the machine file, e.g. moses_cluster 
echo "DONE"

 




