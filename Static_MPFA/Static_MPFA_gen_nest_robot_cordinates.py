import pdb
import matplotlib.pyplot as plt
import glob
import os.path
import numpy as np
from pylab import *

def sub_gen_coord(max_x,min_x, k):
    x1 = max_x - k/2.0
    y1 = max_y - k/2.0
    coordinates =[]
    for i in range(int(max_x - min_x)/k):
        for j in range(int(max_x - min_x)/k):
            x = x1 -k*i  
            y = y1 -k*j
            coordinates.append([x, y])
    #print coordinates
    return coordinates

arena_width = 128

max_x, max_y = arena_width/2.0,  arena_width/2.0;
min_x, min_y = -max_x, -max_y;

gaps = [8]
results=[]
for k in gaps:
  print k
  results.append(sub_gen_coord(max_x, min_x, k))
  

coord_info =open("coord_for_worldFile.xml", "w")
coord_info_text = open("coord.xml", "w")
count =1;
for coords in results:
    for xy in coords:
        coord_info.write("NestPosition_" + str(count) +"=\"" + str(xy[0]-32) + ", " + str(xy[1]-32)+"\"\n") 
        coord_info_text.write(str(xy[0]) + " " + str(xy[1])+"\n") 
        count += 1  

coord_info_text.close()


# there is only delivering robots
count =0;

total_robot=0
speed = 0.16
num_Drobot = 1
forageRate = 110/1800.0
print "forageRate=", forageRate
#unit = np.sqrt(2*((gaps[-1]/2.0)**2))
#unit = np.sqrt(2*((1/2.0)**2))


#for coords in results[:-1]:
#    for xy in coords:
#        distance = np.sqrt(xy[0]**2 + xy[1]**2)
#        print "distance=", distance
#        #print "unit=", unit
#        print round(2*distance/0.16)
#        quantity = num_Drobot*round(distance/unit)
#        print "quantity", quantity
#        if quantity > 30:
#            quantity = 30
#        total_robot += quantity
	    
#        coord_info.write("<distribute>\n")
#        coord_info.write("<position max=\"" + str(xy[0]+gaps[-1]/4.0)+ ", " + str(xy[1]+gaps[-1]/4.0) + ", 0.0\" method=\"uniform\" min=\"" + str(xy[0]-gaps[-1]/4.0)+ ", " + str(xy[1]-gaps[-1]/4.0) + ", 0.0\"/>\n")
#        coord_info.write("<orientation mean=\"0, 0, 0\" method=\"gaussian\" std_dev=\"360, 0, 0\"/>\n")
#        coord_info.write("<entity max_trials=\"100\" quantity=\""  + str(int(quantity))  +  "\">\n")
#        coord_info.write("<foot-bot id=\"D" + str(count)+ "-" + "\"><controller config=\"MPFA\"/></foot-bot>\n")
#        coord_info.write("</entity>\n")
#        coord_info.write("</distribute>\n\n")
#        count += 1  
        
# there are foraging and delivering robots in each region
for xy in results[-1]:
    coord_info.write("<distribute>\n")
    coord_info.write("<position max=\"" + str(xy[0]+gaps[-1]/4.0)+ ", " + str(xy[1]+gaps[-1]/4.0) + ", 0.0\" method=\"uniform\" min=\"" + str(xy[0]-gaps[-1]/4.0)+ ", " + str(xy[1]-gaps[-1]/4.0) + ", 0.0\"/>\n")
    coord_info.write("<orientation mean=\"0, 0, 0\" method=\"gaussian\" std_dev=\"360, 0, 0\"/>\n")
    coord_info.write("<entity max_trials=\"100\" quantity=\"4\">\n")
    coord_info.write("<foot-bot id=\"F" + str(count)+ "-" + "\"><controller config=\"MPFA\"/></foot-bot>\n")
    coord_info.write("</entity>\n")
    coord_info.write("</distribute>\n\n")
 
    #total_robot += 4
    
    print "nest location =[",xy[0],", ",xy[1], "]"
    distance = np.sqrt(xy[0]**2 + xy[1]**2)
    print "distance=", distance
    #print "unit=", unit
    print (forageRate*2*distance)/speed
    quantity = math.ceil((forageRate*2*distance)/speed)
    
    print "quantity", quantity
    total_robot += quantity
    
    coord_info.write("<distribute>\n")
    #coord_info.write("<position max=\"" + str(xy[0]-32+gaps[-1]/4.0)+ ", " + str(xy[1]-32+gaps[-1]/4.0) + ", 0.0\" method=\"uniform\" min=\"" + str(xy[0]-32-gaps[-1]/4.0)+ ", " + str(xy[1]-32-gaps[-1]/4.0) + ", 0.0\"/>\n")
    coord_info.write("<position max=\"" + str(xy[0]+gaps[-1]/4.0)+ ", " + str(xy[1]+gaps[-1]/4.0) + ", 0.0\" method=\"uniform\" min=\"" + str(xy[0]-gaps[-1]/4.0)+ ", " + str(xy[1]-gaps[-1]/4.0) + ", 0.0\"/>\n")
    coord_info.write("<orientation mean=\"0, 0, 0\" method=\"gaussian\" std_dev=\"360, 0, 0\"/>\n")
    coord_info.write("<entity max_trials=\"100\" quantity=\""  + str(int(quantity))  +  "\">\n")
    coord_info.write("<foot-bot id=\"D" + str(count)+ "-" + "\"><controller config=\"MPFA\"/></foot-bot>\n")
    coord_info.write("</entity>\n")
    coord_info.write("</distribute>\n\n")
    count += 1  
    
coord_info.close() 

print 'total delivering robot=', total_robot


