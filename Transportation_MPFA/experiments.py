from xml.dom.minidom import parse, parseString
import random
import subprocess
import sys, platform
import pdb, time

class Random_Argos:

    def __init__(self, argos_xml = None):
        self.argos_xml = argos_xml

if __name__ == "__main__":
    #files1 = ['Test_Random_MPFA_r1_d1_tag64_5by5.xml', 'Test_Random_MPFA_r2_d1_tag64_5by5.xml', 'Test_Random_MPFA_r3_d1_tag64_5by5.xml']
    #files2 = ['Test_Random_MPFA_r4_d4_tag256_10by10.xml', 'Test_Random_MPFA_r8_d4_tag256_10by10.xml', 'Test_Random_MPFA_r12_d4_tag256_10by10.xml'] 
    #files3 = ['Test_Random_MPFA_r16_d16_tag1024_20by20.xml', 'Test_Random_MPFA_r32_d16_tag1024_20by20.xml', 'Test_Random_MPFA_r48_d16_tag1024_20by20.xml']
    files1 = ['Random_MPFA_r8_d4_tag256_10by10.xml']
    #files1 = ['Random_MPFA_r32_d16_tag1024_20by20.xml']

    #allFiles = [files1 , files2, files3]
    #runs = [160,40,10]
    allFiles = [files1]
    runs = [10]
    for (run_count, files) in zip(runs, allFiles):
        print "Total runs=",run_count
        for file in files:
            print file
            this_run = Random_Argos("./experiments/"+file)
            count =1
            startTime =time.time()
            #result =[]
            for _ in range(run_count):
                print "Run "+str(count)
                count = count+1
                output = subprocess.check_output(['argos3 -c ' + this_run.argos_xml], shell=True)
               #elemnts = output.replace('\n', ',').split(',')
               #result.append(elemnts[-5])
            #for ele in result:
            #    outputFile.write(ele+' ')
            #outputFile.close()
            endTime = time.time()
            print 'The total running time is '+str((endTime-startTime)/60.0)+' minutes...'
