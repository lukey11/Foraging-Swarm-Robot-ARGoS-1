import pdb
import numpy as np
import matplotlib.pyplot as plt
from pylab import *
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
        forage = compute_overall_forage_data(data)
        datas.append(forage)
    return datas
 


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
    #mean = np.mean(forage)
    #std = np.std(forage)
    #return mean, std

def compute_mean_std(fileNames):
    means, stds=[], []
    for fileName in fileNames:
        datas = get_data_from_file(fileName)
        forage = compute_overall_forage_data(datas)
        mean, std = np.mean(forage), np.std(forage)
        means.append(mean)
        stds.append(std)
    return means, stds

def plot_bars(handle, means, stds, Color, counter, width, ind):
    rects= handle.bar(ind+counter*width,np.array(means),width=width, color=Color, yerr=np.array(stds), ecolor='k')
    return rects

def autolabel(rects):
    # attach some text labels
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x()+rect.get_width()/2., 0.6*height, '%0.1f'%float(height),
                ha='center', va='bottom', color='white')

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
  


datas =[]
#CPFA

fileNames = ["random_CPFA_r24_tag384_10by10_iAntTagData.txt", "powerlaw_CPFA_r24_tag384_10by10_iAntTagData.txt", "cluster_CPFA_r24_tag384_10by10_iAntTagData.txt"]
#CPFA_means, CPFA_stds = compute_mean_std(fileNames)
datas.append(get_multiple_data(fileNames))

#static MPFA

fileNames = ["random_static_MPFA_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_static_MPFA_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_static_MPFA_n4_r24_tag384_10by10_iAntTagData.txt"]
#static_MPFA_means, static_MPFA_stds = compute_mean_std(fileNames)
datas.append(get_multiple_data(fileNames))

#dynamic MPFA

fileNames = ["random_dynamic_MPFA_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_MPFTD_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_dynamic_MPFA_n4_r24_tag384_10by10_iAntTagData.txt"]
#dynamic_MPFA_means, dynamic_MPFA_stds = compute_mean_std(fileNames)
datas.append(get_multiple_data(fileNames))

#static K-means++

fileNames = ["random_static_K-means_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_static_K-means_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_static_K-means_n4_r24_tag384_10by10_iAntTagData.txt"]
#static_K_means_means, static_K_means_stds = compute_mean_std(fileNames)
datas.append(get_multiple_data(fileNames))


#dynamic K-means++

fileNames = ["random_dynamic_K-means_n4_r24_tag384_10by10_iAntTagData.txt", "powerlaw_K-means_dynamical_n4_r24_tag384_10by10_iAntTagData.txt", "cluster_dynamic_K-means_n4_r24_tag384_10by10_iAntTagData.txt"]
#dynamic_K_means_means, dynamic_K_means_stds=compute_mean_std(fileNames)
datas.append(get_multiple_data(fileNames))

outputFile = open('linearReg_foraging.txt', 'w+')
outputFile.write('Slope\t\t Intercept\t\t r-squared-value\t\t p-value\t\t stderr\r')
X= [0,1,2,3,4]
#data_matrix =np.array([CPFA_means, static_MPFA_means, dynamic_MPFA_means, static_K_means_means, dynamic_K_means_means])
#pdb.set_trace()
#Y1 = [CPFA_mean[0], nest2_mean[0], nest4_mean[0], nest8_mean[0]]
#Y2= [CPFA_mean[1], nest2_mean[1], nest4_mean[1], nest8_mean[1]]
#Y3= [CPFA_mean[2], nest2_mean[2], nest4_mean[2], nest8_mean[2]]
#YList = [data_matrix[:,i] for i in range(len(CPFA_means))]

#for Y in YList:
#    slope, intercept, r_value, p_value, stderr = stats.linregress(X, Y)
#    outputFile.write(str(slope)+'\t\t'+str(intercept)+'\t\t\t'+str(r_value**2)+'\t\t\t'+str(p_value)+'\t\t'+str(stderr)+'\r')
#outputFile.close()

width =0.17
N=3
ind = np.arange(N)  # the x locations for the groups

#colors =['#e41a1c', '#377eb8', '#4daf4a', '#984ea3', '#ff7f00']
#colors =['#323232', '#666666', '#999999', '#b2b2b2', '#cccccc'] #grayscale
colors =['#c8fdb3', '#a4fc81', '#41e002', '#2b9501',  '#1d6301'] #green scale


fig, axarr = plt.subplots()
Y= datas

counter=1
for y, color in zip(Y, colors):
    rect= axarr.boxplot(y,1, positions =ind+counter*width, patch_artist=True, widths=[0.1]*len(Y[0]))
    plt.setp(rect['boxes'], color=color)
    counter+=1
    print [np.median(ele) for ele in y]  


# the stars, shows the statistical significance
#pdb.set_trace()
plot_stars(axarr, datas, ind, width)


#plot legend
plt.figtext(0.14, 0.84, '____',
            backgroundcolor=colors[0], color=colors[0], weight='roman',
            size='large') #size='x-small''large'
plt.figtext(0.2, 0.835, 'CPFA',
             color='black', weight='roman',
            size='x-large') #size='x-small''large'

plt.figtext(0.14, 0.80, '____',
            backgroundcolor=colors[1], color=colors[1], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.2, 0.795, 'MPFA$_{static}$',
            color='black', weight='roman',
            size='x-large') #size='x-small'


plt.figtext(0.14, 0.76, '____',
            backgroundcolor=colors[2], color=colors[2], weight='roman',
            size='large') #size='x-small'

plt.figtext(0.2, 0.755, 'MPFA$_{dynamic}$',
             color='black', weight='roman',
            size='x-large') #size='x-small'


plt.figtext(0.14, 0.72, '____',
            backgroundcolor=colors[3], color=colors[3], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.2, 0.715, 'MPFA$_{global\_static}$',
            color='black', weight='roman',
            size='x-large') #size='x-small'


plt.figtext(0.14, 0.68, '____',
            backgroundcolor=colors[4], color=colors[4], weight='roman',
            size='large') #size='x-small'
plt.figtext(0.2, 0.675, 'MPFA$_{global\_dynamic}$',
            color='black', weight='roman',
            size='x-large') #size='x-small'



#ax.plot([0, 0.18], [15.5, 15.5], 'k', linewidth ='2')
#ax.plot([1, 1.18], [20.5, 20.5], 'k', linewidth ='2')
#ax.plot([2, 2.18], [21.5, 21.5], 'k', linewidth ='2')


# add some text for labels, title and axes ticks
axarr.set_ylabel('Foraging time (minutes)', fontsize=20)
#ax.set_title('Foraging rate in each model', fontsize=20)
axarr.set_xlim(0, 3.1)
axarr.set_xticks(ind+3*width)
axarr.set_xticklabels( ('Random', 'Partially clustered', 'Clustered'), fontsize=18)
axarr.set_yticks(np.arange(5, 45, 5))
#plt.legend( (rect_CPFA[0], rect_static_MPFA[0], rect_dynamic_MPFA[0], rect_static_K_means[0], rect_dynamic_K_means[0]), ('CPFA', 'Static MPFA', 'Dynamic MPFA', 'Static K-means++', 'Dynamic K-means++'), loc=2, fontsize='12' )

#autolabel(rect_CPFA)
#autolabel(rect_static_MPFA)
#autolabel(rect_dynamic_MPFA)
#autolabel(rect_static_K_means)
#autolabel(rect_dynamic_K_means)


savefig('1_boxplot_overall_forage_rate')
plt.show()
