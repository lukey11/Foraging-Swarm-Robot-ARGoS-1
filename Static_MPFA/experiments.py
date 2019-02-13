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
    #files =['Random_MPFA_r1192_d256_tag16384_80by80.xml']
    #files = ['Random_MPFA_r1192_d257_tag16384_80by80.xml', 'Random_MPFA_r296_d65_tag4096_40by40.xml', 'Random_MPFA_r16_d5_tag256_10by10.xml', 'Random_MPFA_r72_d17_tag1024_20by20.xml']
    #files = ['Random_MPFA_r296_d65_tag64_40by40.xml', 'Random_MPFA_r296_d65_tag256_40by40.xml', 'Random_MPFA_r296_d65_tag1024_40by40.xml', 'Random_MPFA_r296_d65_tag16384_40by40.xml']
    #files = ['Widths_32_48_64_96/Test_no_deliver_Random_MPFA_r16_d5_tag82_8by8.xml', 'Widths_32_48_64_96/Test_no_deliver_Random_MPFA_r16_d5_tag328_16by16.xml', 'Widths_32_48_64_96/Test_no_deliver_Random_MPFA_r16_d5_tag737_24by24.xml']
    #files = ['Widths_32_48_64_96/Test_Random_MPFA_r24_d5_tag82_8by8.xml', 'Widths_32_48_64_96/Test_Random_MPFA_r16_d5_tag328_16by16.xml', 'Widths_32_48_64_96/Test_Random_MPFA_r16_d5_tag737_24by24.xml']
    #files = ['1_8_19_constant_speed/Test_no_deliver_Random_MPFA_r16_d5_tag82_8by8.xml', '1_8_19_constant_speed/Test_no_deliver_Random_MPFA_r16_d5_tag328_16by16.xml', '1_8_19_constant_speed/Test_no_deliver_Random_MPFA_r16_d5_tag415_18by18.xml']
    #files = ['02_04_19_vary_speed/Random_MPFA_rXXX_d17_tag64_8by8.xml', '02_04_19_vary_speed/Random_MPFA_r20_d5_tag4_2by2.xml', '02_04_19_vary_speed/Random_MPFA_64by64_mapTo_rXXX_d257_tag16384_128by128.xml', '02_04_19_vary_speed/Random_MPFA_16by16_mapTo_rXXX_d65_tag1024_32by32.xml']
    #files = ['02_04_19_constant_speed/no_delivery_Random_MPFA_16by16_mapTo_r256_d65_tag1024_32by32.xml', '02_04_19_constant_speed/no_delivery_Random_MPFA_64by64_mapTo_r1024_d257_tag16384_128by128.xml', '02_04_19_constant_speed/no_delivery_Random_MPFA_r64_d17_tag64_8by8.xml', '02_04_19_constant_speed/Random_MPFA_16by16_mapTo_r219_d65_tag1024_32by32.xml', '02_04_19_constant_speed/Random_MPFA_64by64_mapTo_r2672_d257_tag16384_128by128.xml','02_04_19_constant_speed/Random_MPFA_r112_d17_tag64_8by8.xml']
<<<<<<< HEAD
    files = ['02_04_19_constant_speed/Random_MPFA_64by64_mapTo_r2672_d257_tag16384_128by128.xml','02_04_19_constant_speed/Random_MPFA_r112_d17_tag64_8by8.xml']
    run_count = 10
=======
    files = ['02_05_regular_arena_scale_by_4/Random_MPFA_r24_d5_tag100_10by10.xml', '02_05_regular_arena_scale_by_4/Random_MPFA_r104_d17_tag400_20by20.xml', '02_05_regular_arena_scale_by_4/Random_MPFA_mapTo_20by20_r424_d65_tag1600_40by40.xml', '02_05_regular_arena_scale_by_4/Random_MPFA_mapTo_40by40_r1704_d257_tag6400_80by80.xml']
    run_count = 5
>>>>>>> 06ca410941bc5c5f6f6b150c4b3c548d9648c182
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
