import pdb
import numpy as np
import matplotlib.pyplot as plt
from pylab import *
from scipy import stats

def get_data_from_file(filename):
    ''' read data from file into a list'''
    f = open(filename)
    filecontents = f.readlines()
    table = [line.strip('\n') for line in filecontents]
    f.close()
    return table

def compute_overall_forage_data(datas):
    words=datas[0].replace(",","").split()
    if words[0]!='tags_collected':
        print "the data may not correct!"
        return
    forage=[]
    for line in datas[1:]:
        words =line.replace(",","").split()
        forage.append(float(words[2]))
    return forage

def compute_overall_time_data(datas):
    words=datas[0].replace(",","").split()
    searchTime=[]
    travelTime=[]
    travelTimeList=[]
    searchTimeList=[]
    for line in datas:
        words =line.replace(",","").split()
        travelTimeList.append(float(words[0])/60.0)
        searchTimeList.append(float(words[1])/60.0)
      
    return travelTimeList, searchTimeList
def autolabel(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x()+rect.get_width()/2., 0.55*height, '%d'%round(height),
                ha='center', va='bottom', color='white', fontsize=13)

#def autolabel_relate(rects, rect_bases):
#    # attach some text labels
#    for rect, rect_base in zip(rects, rect_bases):
#        height = rect.get_height() + rect_base.get_height()
#        ax.text(rect.get_x()+rect.get_width()/2., 0.50*rect.get_height() + rect_base.get_height(), '%d'%float(rect.get_height()),
#                ha='center', va='bottom', color='white', fontsize=14)

def autolabel_relate(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x()+rect.get_width()/2., -0.55*rect.get_height(), '%d'%round(rect.get_height()),
                ha='center', va='bottom', color='white', fontsize=13)


def compute_time(timefileNames, forageFileNames):
    #num_robots = 24
    travel_mean_per_seed, travel_std_per_seed, search_mean_per_seed, search_std_per_seed = [], [], [], []
    travel_mean, travel_std, search_mean, search_std = [], [], [], []
    for timeFileName, forageFileName in zip (timeFileNames, forageFileNames):
        timeDatas = get_data_from_file(timeFileName)
        forageDatas = get_data_from_file(forageFileName)
        
        TravelTime, SearchTime = compute_overall_time_data(timeDatas)

        forage = compute_overall_forage_data(forageDatas)
        #travelTimePerSeed = [a/(b*num_robots) for a, b in zip(TravelTime ,forage)]
         
        #travel_mean_per_seed.append(np.mean(travelTimePerSeed))
        #travel_std_per_seed.append(np.std(travelTimePerSeed))
        travel_mean.append(np.mean(TravelTime))
        travel_std.append(np.std(TravelTime))
        #searchTimePerSeed = [a/(b*num_robots) for a, b in zip(SearchTime ,forage)]
        #search_mean_per_seed.append(np.mean(searchTimePerSeed))
        #search_std_per_seed.append(np.std(searchTimePerSeed))
        search_mean.append(np.mean(SearchTime))
        search_std.append(np.std(SearchTime))
        
    #return np.array(travel_mean_per_seed), np.array(travel_std_per_seed), np.array(search_mean_per_seed), np.array(search_std_per_seed) 
    
    return np.array(travel_mean), np.array(travel_std), np.array(search_mean), np.array(search_std)

#CPFA

timeFileNames =["random_CPFA_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_CPFA_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_CPFA_r24_tag384_10by10_TravelSearchTimeData.txt"] 

forageFileNames = ["random_CPFA_r24_tag384_10by10_iAntTagData.txt", "powerlaw_CPFA_r24_tag384_10by10_iAntTagData.txt","cluster_CPFA_r24_tag384_10by10_iAntTagData.txt"]

CPFA_travel_mean_per_seed, CPFA_travel_std_per_seed, CPFA_search_mean_per_seed, CPFA_search_std_per_seed = compute_time(timeFileNames, forageFileNames)

#Static MPFA
timeFileNames = ["random_static_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_static_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_static_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt"]

forageFileNames = ["random_static_MPFA_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_static_MPFA_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_static_MPFA_n4_r24_tag384_10by10_iAntTagData.txt"]

MPFA_travel_mean_per_seed, MPFA_travel_std_per_seed, MPFA_search_mean_per_seed, MPFA_search_std_per_seed = compute_time(timeFileNames, forageFileNames)


#Dynamic MPFA

timeFileNames = ["random_dynamic_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_MPFTD_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_dynamic_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt"]

forageFileNames = ["random_dynamic_MPFA_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_MPFTD_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_dynamic_MPFA_n4_r24_tag384_10by10_iAntTagData.txt"]


MPFTD_travel_mean_per_seed, MPFTD_travel_std_per_seed, MPFTD_search_mean_per_seed, MPFTD_search_std_per_seed =  compute_time(timeFileNames, forageFileNames)


#Static K-means++
timeFileNames = ["random_static_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_static_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_static_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt"]

forageFileNames =["random_static_K-means_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_static_K-means_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_static_K-means_n4_r24_tag384_10by10_iAntTagData.txt"]

k_means_static_travel_mean_per_seed, k_means_static_travel_std_per_seed, k_means_static_search_mean_per_seed, k_means_static_search_std_per_seed = compute_time(timeFileNames, forageFileNames)

#Dynamic K-means++
timeFileNames = ["random_dynamic_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_K-means_dynamical_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_dynamic_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt"]

forageFileNames =["random_dynamic_K-means_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_K-means_dynamical_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_dynamic_K-means_n4_r24_tag384_10by10_iAntTagData.txt"]

k_means_dynamic_travel_mean_per_seed, k_means_dynamic_travel_std_per_seed, k_means_dynamic_search_mean_per_seed, k_means_dynamic_search_std_per_seed = compute_time(timeFileNames, forageFileNames)



#X= [0,1,2,3]

#outputFile = open('linearReg_searchTime.txt', 'w+')
#outputFile.write('Slope\t\t\t Intercept\t\t r-squared-value\t\t p-value\t\t stderr\r')
#Y1 = [CPFA_search_mean_per_seed[0], MPFA_search_mean_per_seed[0], MPFTD_search_mean_per_seed[0], Optimum_search_mean_per_seed[0]]
#Y2= [CPFA_search_mean_per_seed[1], MPFA_search_mean_per_seed[1], MPFTD_search_mean_per_seed[1], Optimum_search_mean_per_seed[1]]
#Y3= [CPFA_search_mean_per_seed[2], MPFA_search_mean_per_seed[2], MPFTD_search_mean_per_seed[2], Optimum_search_mean_per_seed[2]]
#YList = [Y1, Y2, Y3]
#for Y in YList:
#    slope, intercept, r_value, p_value, stderr = stats.linregress(X, Y)
#    outputFile.write(str(slope)+'\t\t'+str(intercept)+'\t\t'+str(r_value**2)+'\t\t'+str(p_value)+'\t\t'+str(stderr)+'\r')
#outputFile.close()


#outputFile = open('linearReg_travelTime.txt', 'w+')
#outputFile.write('Slope\t\t\t Intercept\t\t r-squared-value\t\t p-value\t\t stderr\r')
#Y1 = [CPFA_travel_mean_per_seed[0], MPFA_travel_mean_per_seed[0], MPFTD_travel_mean_per_seed[0], Optimum_travel_mean_per_seed[0]]
#Y2= [CPFA_travel_mean_per_seed[1], MPFA_travel_mean_per_seed[1], MPFTD_travel_mean_per_seed[1], Optimum_travel_mean_per_seed[1]]
#Y3= [CPFA_travel_mean_per_seed[2], MPFA_travel_mean_per_seed[2], MPFTD_travel_mean_per_seed[2], Optimum_travel_mean_per_seed[2]]
#YList = [Y1, Y2, Y3]
#for Y in YList:
#    slope, intercept, r_value, p_value, stderr = stats.linregress(X, Y)
#    outputFile.write(str(slope)+'\t\t'+str(intercept)+'\t\t'+str(r_value**2)+'\t\t'+str(p_value)+'\t\t'+str(stderr)+'\r')
#outputFile.close()


width=0.18
N=3
ind = np.arange(N)  # the x locations for the groups
fig1, ax = plt.subplots()

rect_CPFA_search = ax.bar(ind, CPFA_search_mean_per_seed,width=width, color='#e41a1c', edgecolor='black', yerr=CPFA_search_std_per_seed, ecolor='k')
#rect_CPFA_travel = ax.bar(ind, CPFA_travel_mean_per_seed,width=width, bottom=CPFA_search_mean_per_seed, color='#e41a1c', edgecolor='black', hatch="X", yerr=CPFA_travel_std_per_seed, ecolor='k')
rect_CPFA_travel = ax.bar(ind, CPFA_travel_mean_per_seed,width=width, bottom=(-1)*CPFA_travel_mean_per_seed,  color='#e41a1c', edgecolor='black', hatch="X", yerr=CPFA_travel_std_per_seed, ecolor='k')


rect_MPFA_search = ax.bar(ind+width, MPFA_search_mean_per_seed,width=width, color='#377eb8', edgecolor='black', yerr=MPFA_search_std_per_seed, ecolor='k')
#rect_MPFA_travel = ax.bar(ind+width, MPFA_travel_mean_per_seed,width=width, bottom=MPFA_search_mean_per_seed, color='#377eb8', edgecolor='black', hatch="X", yerr=MPFA_travel_std_per_seed, ecolor='k')
rect_MPFA_travel = ax.bar(ind+width, MPFA_travel_mean_per_seed,width=width, bottom =(-1)*MPFA_travel_mean_per_seed, color='#377eb8', edgecolor='black', hatch="X", yerr=MPFA_travel_std_per_seed, ecolor='k')


rect_MPFTD_search = ax.bar(ind+2.0*width, MPFTD_search_mean_per_seed,width=width, color='#4daf4a', edgecolor='black', yerr=MPFTD_search_std_per_seed, ecolor='k')
#rect_MPFTD_travel= ax.bar(ind+2*width, MPFTD_travel_mean_per_seed,width=width, bottom=MPFTD_search_mean_per_seed, color='#4daf4a', edgecolor='black', hatch="X", yerr=MPFTD_travel_std_per_seed, ecolor='k')
rect_MPFTD_travel= ax.bar(ind+2.0*width, MPFTD_travel_mean_per_seed,width=width, bottom=(-1)*MPFTD_travel_mean_per_seed, color='#4daf4a', edgecolor='black', hatch="X", yerr=MPFTD_travel_std_per_seed, ecolor='k')


rect_K_means_static_search = ax.bar(ind+3.0*width, k_means_static_search_mean_per_seed,width=width, color='#984ea3', edgecolor='black', yerr=k_means_static_search_std_per_seed, ecolor='k')
#rect_K_means_static_travel = ax.bar(ind+3*width, k_means_static_travel_mean_per_seed, width=width, bottom=k_means_static_search_mean_per_seed, color='#984ea3', edgecolor='black', hatch="X", yerr=k_means_static_travel_std_per_seed, ecolor='k')

rect_K_means_static_travel = ax.bar(ind+3.0*width, k_means_static_travel_mean_per_seed, width=width, bottom=(-1)*k_means_static_travel_mean_per_seed, color='#984ea3', edgecolor='black', hatch="X", yerr=k_means_static_travel_std_per_seed, ecolor='k')


rect_K_means_dynamic_search = ax.bar(ind+4.0*width, k_means_dynamic_search_mean_per_seed,width=width, color='#ff7f00', edgecolor='black', yerr=k_means_dynamic_search_std_per_seed, ecolor='k')
#rect_K_means_dynamic_travel = ax.bar(ind+4*width, k_means_dynamic_travel_mean_per_seed, width=width, bottom=k_means_dynamic_search_mean_per_seed, color='#ff7f00', edgecolor='black', hatch="X", yerr=k_means_dynamic_travel_std_per_seed, ecolor='k')
rect_K_means_dynamic_travel = ax.bar(ind+4.0*width, k_means_dynamic_travel_mean_per_seed, width=width, bottom = (-1)*k_means_dynamic_travel_mean_per_seed, color='#ff7f00', edgecolor='black', hatch="X", yerr=k_means_dynamic_travel_std_per_seed, ecolor='k')

# add some text for labels, title and axes ticks
ax.set_ylabel('Foraging time (minutes)', fontsize=15)
#ax.set_title('Average time by collecting each seed', fontsize=20)
ax.set_xticks(ind+3*width)

ax.set_ylim([-550, 750])
ax.set_yticks(np.arange(-550, 750, 100))
#ax.set_ylim([-1.2, 1.2])
#ax.set_yticks(np.arange(-1.2, 1.2, 0.2))

ax.set_yticklabels(str(abs(x)) for x in ax.get_yticks())

#ax.set_title('Random             Partially clustered      Clustered                         ', fontsize=16)
#ax.set_xlabel('Number of nests', fontsize=15)
#ax.set_xticklabels( ('CPFA      MPFA      MPFTD      K_means_static', 'CPFA      MPFA      MPFTD      K_means_static', 'CPFA      MPFA      MPFTD      K_means_static'), fontsize=12)

#ax.set_title('Random             Partially clustered            Clustered',fontsize=15)
ax.set_xticklabels( ('Random', 'Partially clustered', 'Clustered'), fontsize=18)
#draw legend manually
#plt.legend( (rect_CPFA_search[0], rect_CPFA_travel[0]), ('Search time', 'Travel time'), ncol=2, loc=2)
plt.legend( (rect_CPFA_search[0], rect_MPFA_search[0], rect_MPFTD_search[0], rect_K_means_static_search[0], rect_K_means_dynamic_search[0]), ('CPFA', 'Static MPFA', 'Dynamic MPFA', 'Static K-means++', 'Dynamic K-means++'), ncol=2, loc=2)

autolabel(rect_CPFA_search)
autolabel(rect_MPFA_search)
autolabel(rect_MPFTD_search)
autolabel(rect_K_means_static_search)
autolabel(rect_K_means_dynamic_search)

autolabel_relate(rect_CPFA_travel)
autolabel_relate(rect_MPFA_travel)
autolabel_relate(rect_MPFTD_travel)
autolabel_relate(rect_K_means_static_travel)
autolabel_relate(rect_K_means_dynamic_travel)

savefig('3_total_search_travel_time')
plt.show()
