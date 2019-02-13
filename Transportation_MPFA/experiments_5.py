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
    #files = ['Random_MPFA_r1192_d256_tag16384_80by80.xml']
    #files = ['Random_MPFA_backtracking_r16_d4_tag256_10by10.xml', 'Random_MPFA_backtracking_r72_d16_tag1024_20by20.xml', 'Random_MPFA_backtracking_r296_d64_tag4096_40by40.xml', 'Random_MPFA_backtracking_r1192_d256_tag16384_80by80.xml']
    #files = ['Random_MPFA_r296_d64_tag64_40by40.xml', 'Random_MPFA_r296_d64_tag256_40by40.xml', 'Random_MPFA_r296_d64_tag1024_40by40.xml', 'Random_MPFA_r296_d64_tag16384_40by40.xml']
    #files = ['Random_MPFA_backtracking_r24_d4_tag256_10by10.xml', 'Random_MPFA_backtracking_r104_d16_tag1024_20by20.xml', 'Random_MPFA_backtracking_r424_d64_tag4096_40by40.xml', 'Random_MPFA_backtracking_r1704_d256_tag16384_80by80.xml']
    #files = ['02_04_19_constant_speed_constant_capacity/Random_MPFA_16by16_mapTo_rXXX_d65_tag1024_32by32.xml', '02_04_19_constant_speed_constant_capacity/Random_MPFA_64by64_mapTo_rXXX_d257_tag16384_128by128.xml', '02_04_19_constant_speed_constant_capacity/Random_MPFA_r20_d5_tag4_2by2.xml', '02_04_19_constant_speed_constant_capacity/Random_MPFA_rXXX_d17_tag64_8by8.xml']
    files = ['02_05_regular_arena_scale_by_4/Random_MPFA_mapTo_40by40_backtracking_r1704_d257_tag6400_80by80.xml']
    run_count = 5
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
	
