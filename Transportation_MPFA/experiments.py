from xml.dom.minidom import parse, parseString
import random
import subprocess
import sys, platform
import pdb, time

class Random_Argos:

    def __init__(self, argos_xml = None):
        self.argos_xml = argos_xml

if __name__ == "__main__":
    #files = ['Random_MPFA_r2_d1_tag64_5by5.xml', 'Random_MPFA_r16_d4_tag256_10by10.xml', 'Random_MPFA_r72_d16_tag1024_20by20.xml', 'Random_MPFA_r296_d64_tag4096_40by40.xml']
    files = ['Random_MPFA_r1192_d256_tag16384_80by80.xml']
    #allFiles = [files1 , files2, files3]
    #runs = [160,40,10]
    #runs = [1]
    run_count = 10
    #for (run_count, files) in zip(runs, allFiles):
    #    print "Total runs=",run_count
    for file in files:
	print file
	this_run = Random_Argos("./experiments/"+file)
	count =1
	startTime =time.time()
	
	for _ in range(run_count):
	    print "Run "+str(count)
	    singleRun_StartTime =  time.time()
	    count = count+1
	    output = subprocess.check_output(['argos3 -c ' + this_run.argos_xml], shell=True, stderr=subprocess.STDOUT)
	    singleRun_EndTime = time.time()
	    print 'This run takes '+str((singleRun_EndTime-singleRun_StartTime)/60.0)+' minutes...' 
	endTime = time.time()
	print 'The total running time is '+str((endTime-startTime)/60.0)+' minutes...'
	
