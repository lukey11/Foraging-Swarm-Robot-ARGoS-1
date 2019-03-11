from xml.dom.minidom import parse, parseString
import random
import subprocess
import sys, platform
import pdb, time

class Random_Argos:

    def __init__(self, argos_xml = None):
        self.argos_xml = argos_xml

if __name__ == "__main__":
    #system = 'linux' if platform.system() == 'Linux' else 'mac'
    #files = ['02_05_regular_arena_scale_by_4/Random_MPFA_backtracking_r24_d5_tag100_10by10.xml', '02_05_regular_arena_scale_by_4/Random_MPFA_backtracking_r104_d17_tag400_20by20.xml', '02_05_regular_arena_scale_by_4/Random_MPFA_backtracking_r424_d65_tag1600_40by40.xml', '02_05_regular_arena_scale_by_4/Random_MPFA_backtracking_r1704_d257_tag6400_80by80.xml']
    #files = ['02_04_19_vary_speed_constant_capacity/Random_MPFA_16by16_mapTo_r106_d65_tag1024_32by32.xml', '02_04_19_vary_speed_constant_capacity/Random_MPFA_r20_d5_tag4_2by2.xml', '02_04_19_vary_speed_constant_capacity/Random_MPFA_r112_d21_tag64_8by8.xml']
    files = ['02_15_19_regular_arena_scale_by_4/Random_MPFA_no_backtracking_r32_d5_tag100_10by10.xml', '02_15_19_regular_arena_scale_by_4/Random_MPFA_no_backtracking_r144_d21_tag400_20by20.xml', '02_15_19_regular_arena_scale_by_4/Random_MPFA_no_backtracking_r592_d85_tag1600_40by40_mapTo_r148_d22_tag400_20by20.xml', '02_15_19_regular_arena_scale_by_4/Random_MPFA_no_backtracking_r2384_d257_tag6400_80by80_mapTo_r596_d86_tag1600_40by40.xml']
    run_count = 20
    for file in files:
        print file
        this_run = Random_Argos("./experiments/"+file)
        count =1
        startTime =time.time()
        #result =[]
        for _ in range(run_count):
            print "Run "+str(count)
	    singleRun_StartTime =  time.time()
            count = count+1
            output = subprocess.check_output(['argos3 -n -c ' + this_run.argos_xml], shell=True, stderr=subprocess.STDOUT)
	    singleRun_EndTime = time.time()
	    print 'This run takes '+str((singleRun_EndTime-singleRun_StartTime)/60.0)+' minutes...' 
        endTime = time.time()
        print 'The total running time is '+str((endTime-startTime)/60.0)+' minutes...'
