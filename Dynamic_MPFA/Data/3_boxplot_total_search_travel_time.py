import pdb
import numpy as np
import matplotlib.pyplot as plt
from pylab import *
#from scipy import stats
import scipy.stats

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


def autolabel_relate(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x()+rect.get_width()/2., -0.55*rect.get_height(), '%d'%round(rect.get_height()),
                ha='center', va='bottom', color='white', fontsize=13)


def compute_time(timefileNames):
    #num_robots = 24
    #travel_mean, travel_std, search_mean, search_std = [], [], [], []
    travel, search =[], []
    for timeFileName in timeFileNames:
        timeDatas = get_data_from_file(timeFileName)
        
        TravelTime, SearchTime = compute_overall_time_data(timeDatas)

        
        travel.append(TravelTime)
        #travel_std.append(np.std(TravelTime))
        
        search.append(SearchTime)
        #search_std.append(np.std(SearchTime))
        
    #return np.array(travel_mean), np.array(travel_std), np.array(search_mean), np.array(search_std)
    return travel, search

def stars(p):
    #if p < 0.0001:
    #    return "****"
    #elif (p < 0.001):
    if (p < 0.001):
        return "***"
    elif (p < 0.01):
        return "**"
    elif (p < 0.05):
        return "*"
    else:
        return "-"

def plot_stars(handle, datas, ind, width):
    Y = np.array(datas).transpose()
    i = 0
    for y in Y:
        j = 1
        for idx in range(len(y)-1):
            y1 = y[idx]
            y2 = y[idx+1]
            z, p = scipy.stats.mannwhitneyu(y1, y2)
            p_value = p * 2
            s = stars(p)
    
            y_max = np.max(np.concatenate((y1, y2)))
            y_min = np.min(np.concatenate((y1, y2)))
    
            handle.annotate("", xy=(ind[i]+j*width, y_max), xycoords='data',
                xytext=(ind[i]+(j+1)*width, y_max), textcoords='data',
                arrowprops=dict(arrowstyle="-", ec='#aaaaaa',
                            connectionstyle="bar,fraction=0.2"))
            #handle.text(ind[i]+(j+0.5)*width, y_max + abs(y_max - y_min)*0.1, stars(p_value),
            handle.text(ind[i]+(j+0.5)*width, y_max, stars(p_value),
                horizontalalignment='center',
                verticalalignment='center') 
            j+=1
        i+=1


#CPFA
travel_data, search_data =[] , []
timeFileNames =["random_CPFA_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_CPFA_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_CPFA_r24_tag384_10by10_TravelSearchTimeData.txt"] 

CPFA_travel, CPFA_search= compute_time(timeFileNames)
travel_data.append(CPFA_travel)
search_data.append(CPFA_search)

#Static MPFA
timeFileNames = ["random_static_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_static_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_static_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt"]

MPFA_travel, MPFA_search = compute_time(timeFileNames)
travel_data.append(MPFA_travel)
search_data.append(MPFA_search)


#Dynamic MPFA

timeFileNames = ["random_dynamic_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_MPFTD_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_dynamic_MPFA_n4_r24_tag384_10by10_TravelSearchTimeData.txt"]

MPFTD_travel, MPFTD_search =  compute_time(timeFileNames)
travel_data.append(MPFTD_travel)
search_data.append(MPFTD_search)


#Static K-means++
timeFileNames = ["random_static_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_static_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_static_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt"]

k_means_static_travel, k_means_static_search = compute_time(timeFileNames)
travel_data.append(k_means_static_travel)
search_data.append(k_means_static_search)


#Dynamic K-means++
timeFileNames = ["random_dynamic_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "powerlaw_K-means_dynamical_n4_r24_tag384_10by10_TravelSearchTimeData.txt", "cluster_dynamic_K-means_n4_r24_tag384_10by10_TravelSearchTimeData.txt"]

k_means_dynamic_travel, k_means_dynamic_search= compute_time(timeFileNames)
travel_data.append(k_means_dynamic_travel)
search_data.append(k_means_dynamic_search)


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


width=0.17
N=3
ind = np.arange(N)  # the x locations for the groups
#colors =['#e41a1c', '#377eb8', '#4daf4a', '#984ea3', '#ff7f00']
#colors =['#323232', '#666666', '#999999', '#b2b2b2', '#cccccc'] #gray scale
colors =['#ccd4fd', '#8195fc', '#3556fa', '#0223c7', '#01167c']

fig1, axarr = plt.subplots(2, sharex=True)

Y=travel_data

print "travel time"
counter=1
for y, color in zip(Y, colors):
    rect= axarr[0].boxplot(y,1, positions =ind+counter*width, patch_artist=True, widths=[0.05]*len(Y[0]))
    plt.setp(rect['boxes'], color=color)
    counter+=1
    print [np.median(ele) for ele in y]

plot_stars(axarr[0], travel_data, ind, width)

# add some text for labels, title and axes ticks
axarr[0].set_ylabel('Travel time (minutes)', fontsize=18)
#ax.set_title('Average time by collecting each seed', fontsize=20)




Y=search_data
print "search time"
    
counter=1
for y, color in zip(Y, colors):
    rect= axarr[1].boxplot(y,1, positions =ind+counter*width, patch_artist=True, widths=[0.05]*len(Y[0]))
    plt.setp(rect['boxes'], color=color)
    counter+=1
    print [np.median(ele) for ele in y]

plot_stars(axarr[1], search_data, ind, width)

plt.figtext(0.14, 0.86, '____',
            backgroundcolor=colors[0], color=colors[0], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.185, 0.86, 'CPFA',
            color='black', weight='roman',
            size='large') #size='x-small'


plt.figtext(0.24, 0.86, '____',
            backgroundcolor=colors[1], color=colors[1], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.285, 0.86, 'MPFA$_{static}$',
            color='black', weight='roman',
            size='large') #size='x-small'

plt.figtext(0.38, 0.86, '____',
            backgroundcolor=colors[2], color=colors[2], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.425, 0.86, 'MPFA$_{dynamic}$',
            color='black', weight='roman',
            size='large') #size='x-small'


plt.figtext(0.535, 0.86, '____',
            backgroundcolor=colors[3], color=colors[3], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.58, 0.86, 'MPFA$_{global\_static}$',
            color='black', weight='roman',
            size='large') #size='x-small'

plt.figtext(0.71, 0.86, '____',
            backgroundcolor=colors[4], color=colors[4], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.755, 0.86, 'MPFA$_{global\_dynamic}$',
            color='black', weight='roman',
            size='large') #size='x-small'


axarr[0].set_ylim([50, 450])
axarr[0].set_yticks(np.arange(50, 450, 100))

axarr[1].set_ylabel('Search time (minutes)', fontsize=18)
axarr[1].set_ylim([120, 420])
axarr[1].set_yticks(np.arange(120, 420, 50))
#ax.set_ylim([-1.2, 1.2])
#ax.set_yticks(np.arange(-1.2, 1.2, 0.2))
axarr[1].set_xlim(0, 3.1)
axarr[1].set_xticks(ind+3*width)
axarr[1].set_xticklabels( ('Random', 'Partially clustered', 'Clustered'), fontsize=18)


savefig('3_boxplot_total_search_travel_time')
plt.show()
