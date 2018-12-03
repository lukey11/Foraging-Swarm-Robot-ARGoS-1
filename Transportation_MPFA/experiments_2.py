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
    files = ['Random_MPFA_r2_d1_tag64_5by5.xml', 'Random_MPFA_r16_d4_tag256_10by10.xml', 'Random_MPFA_r72_d16_tag1024_20by20.xml', 'Random_MPFA_r296_d64_tag4096_40by40.xml', 'Random_MPFA_r1192_d256_tag16384_80by80.xml', 'Random_MPFA_backtracking_r16_d4_tag256_10by10.xml', 'Random_MPFA_r72_d16_tag1024_20by20.xml', 'Random_MPFA_r296_d64_tag4096_40by40.xml', 'Random_MPFA_r1192_d256_tag16384_80by80.xml']
    run_count = 10
    
    for file in files:
        print file
        this_run = Random_Argos("./experiments/"+file)
        count =1
        startTime =time.time()
        #result =[]
        for _ in range(run_count):
            print "Run "+str(count)
            count = count+1
            output = subprocess.check_output(['argos3 -n -c ' + this_run.argos_xml], shell=True, stderr=subprocess.STDOUT)
            #elemnts = output.replace('\n', ',').split(',')
            #result.append(elemnts[-5])
        #for ele in result:
        #    outputFile.write(ele+' ')
        #outputFile.close()
        endTime = time.time()
        print 'The total running time is '+str((endTime-startTime)/60.0)+' minutes...'
