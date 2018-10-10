mkdir -p launch
echo "Create the folder -launch-"
logfilename="${1##*/}_$(date +%F_%H:%M:%S,%N)"

nohup ./$1 $2 $3 $4 >>launch/${logfilename}_stdout.log 2>> launch/${logfilename}_stderr.log &

echo "DONE"

#Command example: ./currentscript.sh run_2_3types_ga.sh Cluster_4nests.xml Powerlaw_4nests.xml Random_4nests.xml

