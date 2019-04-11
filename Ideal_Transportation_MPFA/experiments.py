from xml.dom.minidom import parse, parseString
import random
import subprocess
import sys, platform
import pdb, time

class Random_Argos:

    def __init__(self, argos_xml = None):
        self.argos_xml = argos_xml

if __name__ == "__main__":
    #files = ['02_15_19_constant_speed_constant_capacity/Random_MPFA_r724_d85_tag4096_64by64_mapTo_r181_d22_tag1024_32by32.xml', '02_15_19_constant_speed_constant_capacity/Random_MPFA_r112_d21_tag256_16by16.xml', '02_15_19_constant_speed_constant_capacity/Random_MPFA_r24_d5_tag16_4by4.xml', '02_15_19_constant_speed_constant_capacity/Random_MPFA_r4_d1_tag1_1by1.xml', '02_20_19_vary_speed_constant_capacity/Random_MPFA_r2004_d341_tag4096_64by64_mapTo_r501_d86_tag1024_32by32.xml','02_20_19_vary_speed_constant_capacity/Random_MPFA_r112_d21_tag64_8by8.xml','02_20_19_vary_speed_constant_capacity/Random_MPFA_r4_d1_tag1_1by1.xml']
    #files = ['03_10_19_constant_speed_constant_capacity/Random_MPFA_r4_d1_tag1_1by1.xml', '03_10_19_constant_speed_constant_capacity/Random_MPFA_r24_d5_tag16_4by4.xml', '03_10_19_constant_speed_constant_capacity/Random_MPFA_r136_d21_tag256_16by16.xml', '03_10_19_constant_speed_constant_capacity/Random_MPFA_r840_d85_tag4096_64by64_mapTo_r210_d22_tag1024_32by32.xml']
    files = ['03_10_19_constant_speed_constant_capacity/Random_MPFA_r136_d21_tag256_16by16.xml', '03_10_19_constant_speed_constant_capacity/Random_MPFA_r840_d85_tag4096_64by64_mapTo_r210_d22_tag1024_32by32.xml']
    run_count = 20
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
	
