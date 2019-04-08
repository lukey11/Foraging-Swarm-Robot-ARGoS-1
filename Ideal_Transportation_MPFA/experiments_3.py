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
    #files = ['03_10_19_vary_speed_constant_capacity/Random_MPFA_r4_d1_tag1_1by1.xml', '03_10_19_vary_speed_constant_capacity/Random_MPFA_r84_d21_tag64_8by8.xml', '03_10_19_vary_speed_constant_capacity/Random_MPFA_r1392_d341_tag4096_64by64_mapTo_r348_d86_tag1024_32by32.xml']
    files = ['03_10_19_vary_speed_constant_capacity/Random_MPFA_r84_d21_tag64_8by8.xml', '03_10_19_vary_speed_constant_capacity/Random_MPFA_r1392_d341_tag4096_64by64_mapTo_r348_d86_tag1024_32by32.xml']
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
