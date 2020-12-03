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

def get_multiple_data(files):
    datas=[]
    for f in files:
        data = get_data_from_file(f)
        result = compute_overall_collision_data(data)
        datas.append(result)
    return datas


def compute_overall_collision_data(datas):
    words=datas[0].replace(",","").split()
    if words[0]!='tags_collected':
        print "the data may not correct!"
        return
    collision=[]
    forage = [] 
    for line in datas[1:]:
        words =line.replace(",","").split()
        collision.append(float(words[1]))
        #forage.append(float(words[2]))
    #return np.array(collision), np.array(forage)
    return np.array(collision)

def compute_mean_std(fileNames):
    means, stds=[], []
    for fileName in fileNames:
        datas = get_data_from_file(fileName)
        collision, forage = compute_overall_collision_data(datas)
        #values = collision/(forage*60)
        values = collision
        mean, std = np.mean(values), np.std(values)
        means.append(mean)
        stds.append(std)
    return means, stds

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

datas=[]
#CPFA
fileNames = ["random_CPFA_r24_tag384_10by10_iAntTagData.txt", "powerlaw_CPFA_r24_tag384_10by10_iAntTagData.txt", "cluster_CPFA_r24_tag384_10by10_iAntTagData.txt"]
#CPFA_means, CPFA_stds = compute_mean_std(fileNames)
#pdb.set_trace()
datas.append(get_multiple_data(fileNames))

#Static MPFA
fileNames = ["random_static_MPFA_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_static_MPFA_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_static_MPFA_n4_r24_tag384_10by10_iAntTagData.txt"]
#MPFA_means, MPFA_stds = compute_mean_std(fileNames)
datas.append(get_multiple_data(fileNames))

#Dynamic MPFA
fileNames = ["random_dynamic_MPFA_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_MPFTD_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_dynamic_MPFA_n4_r24_tag384_10by10_iAntTagData.txt"]
#dynamic_MPFA_means, dynamic_MPFA_stds=compute_mean_std(fileNames)
datas.append(get_multiple_data(fileNames))

#static K-means++
fileNames = ["random_static_K-means_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_static_K-means_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_static_K-means_n4_r24_tag384_10by10_iAntTagData.txt"]
#K_means_static_means, K_means_static_stds = compute_mean_std(fileNames)
datas.append(get_multiple_data(fileNames))

#K-means dynamic
fileNames = ["random_dynamic_K-means_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_K-means_dynamical_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_dynamic_K-means_n4_r24_tag384_10by10_iAntTagData.txt"]
#K_means_dynamic_means, K_means_dynamic_stds = compute_mean_std(fileNames)
datas.append(get_multiple_data(fileNames))

outputFile = open('linearReg_collision.txt', 'w+')
outputFile.write('Slope\t\t\t Intercept\t\t r-squared-value\t\t p-value\t\t stderr\r')
X= [0,1,2,3]


#Y1 = [CPFA_mean[0], nest2_mean[0], nest4_mean[0], nest8_mean[0]]
#Y2= [CPFA_mean[1], nest2_mean[1], nest4_mean[1], nest8_mean[1]]
#Y3= [CPFA_mean[2], nest2_mean[2], nest4_mean[2], nest8_mean[2]]
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
colors =['#fdcfcc', '#fc8781', '#fa3e35', '#950901', '#4a0400']


fig, axarr = plt.subplots()
Y=datas

counter=1
for y, color in zip(Y, colors):
    rect= axarr.boxplot(y,1, positions =ind+counter*width, patch_artist=True, widths=[0.05]*len(Y[0]))
    plt.setp(rect['boxes'], color=color)
    counter+=1
    print [np.median(ele) for ele in y]  

plot_stars(axarr, datas, ind, width)

plt.figtext(0.14, 0.85, '____',
            backgroundcolor=colors[0], color=colors[0], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.18, 0.845, 'CPFA',
            color='black', weight='roman',
            size='x-large') #size='x-small'


plt.figtext(0.14, 0.82, '____',
            backgroundcolor=colors[1], color=colors[1], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.18, 0.815, 'MPFA$_{static}$',
            color='black', weight='roman',
            size='x-large') #size='x-small'


plt.figtext(0.14, 0.79, '____',
            backgroundcolor=colors[2], color=colors[2], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.18, 0.785, 'MPFA$_{dynamic}$',
            color='black', weight='roman',
            size='x-large') #size='x-small'


plt.figtext(0.14, 0.76, '____',
            backgroundcolor=colors[3], color=colors[3], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.18, 0.755, 'MPFA$_{global\_static}$',
            color='black', weight='roman',
            size='x-large') #size='x-small'


plt.figtext(0.14, 0.73, '____',
            backgroundcolor=colors[4], color=colors[4], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.18, 0.725, 'MPFA$_{global\_dynamic}$',
            color='black', weight='roman',
            size='x-large') #size='x-small'


# add some text for labels, title and axes ticks
axarr.set_ylabel('Total collision time (seconds)', fontsize=20)
#ax.set_title('Collision in each model', fontsize=20)
axarr.set_xlim(0, 3.1)
axarr.set_xticks(ind+3*width)
axarr.set_xticklabels( ('Random','Partially clustered', 'Clustered'), fontsize=18)
axarr.set_ylim([0, 1000])
axarr.set_yticks(np.arange(0, 1000, 100))
#plt.legend( (rect_CPFA[0], rect_MPFA[0], rect_dynamic_MPFA[0], rect_K_means_static[0], rect_K_means_dynamic[0]), ('CPFA', 'Static MPFA', 'Dynamic MPFA', 'Static K-means++', 'Dynamic K-means++'), loc=2, fontsize='12' )

def autolabel(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x()+rect.get_width()/2., 0.6*height, '%d'%int(height),
                ha='center', va='bottom', color='white')


#autolabel(rect_CPFA)
#autolabel(rect_MPFA)
#autolabel(rect_dynamic_MPFA)
#autolabel(rect_K_means_static)
#autolabel(rect_K_means_dynamic)

savefig('2_boxplot_total_collision')
plt.show()
