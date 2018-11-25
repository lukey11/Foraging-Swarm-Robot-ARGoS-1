import pdb
import matplotlib.pyplot as plt
import glob
import os.path
import numpy as np
from pylab import *

def ordered_data_from_file(filename):
    ''' read data from file into a list'''
    f = open(filename)
    coord_info =open("ordered_nest_coord.xml", "w")
    filecontents = f.readlines()
    table = []
    count = 0
    for line in filecontents:
        elems = line.replace('_', '=').split('=')
        print elems
        if len(elems) == 3:
            coord_info.write(elems[0]+"_"+str(count)+'='+elems[2])  
            count+=1
        else:
            coord_info.write(line)  
             
    coord_info.close()        
    f.close()
    
    return table


if __name__ == "__main__":

    #dataFile = raw_input('Please select a data file: ')
    dataFile = "nest_coords.xml"
    data = ordered_data_from_file(dataFile)
