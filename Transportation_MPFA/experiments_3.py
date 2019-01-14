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
    files = ['1_8_19_constant_speed/Random_MPFA_r16_d5_tag20_4by4.xml', '1_8_19_constant_speed/Random_MPFA_rXXX_d17_tag328_16by16.xml', '1_8_19_constant_speed/Random_MPFA_rXXX_d82_tag8398_81by81.xml']
    run_count = 5
    
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
