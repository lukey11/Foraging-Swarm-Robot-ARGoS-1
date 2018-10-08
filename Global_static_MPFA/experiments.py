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
    #file = raw_input('please input an argos file: ')
    #files = ['Cluster_K-means_static_12by12.xml', 'Powerlaw_K-means_static_12by12.xml', 'Random_K-means_static_12by12.xml','Cluster_K-means_static_14by14.xml', 'Powerlaw_K-means_static_14by14.xml', 'Random_K-means_static_14by14.xml', 'Cluster_K-means_static_16by16.xml', 'Powerlaw_K-means_static_16by16.xml', 'Random_K-means_static_16by16.xml']
    #files = ['Cluster_K-means_static_r48_50by50.xml', 'Powerlaw_K-means_static_r48_50by50.xml','Random_K-means_static_r48_50by50.xml', 'Cluster_K-means_static_r72_50by50.xml', 'Powerlaw_K-means_static_r72_50by50.xml','Random_K-means_static_r72_50by50.xml', 'Cluster_K-means_static_r96_50by50.xml', 'Powerlaw_K-means_static_r96_50by50.xml','Random_K-means_static_r96_50by50.xml']
    #files = ['Cluster_static_K-means_10by10.xml', 'Powerlaw_static_K-means_10by10.xml','Random_static_K-means_10by10.xml']
    files = ['Powerlaw_static_K-means_r24_50by50.xml']
    for file in files:
        print file
        this_run = Random_Argos("./experiments/"+file)
        count =1
        startTime =time.time()
        #result =[]
        for _ in range(80):
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
