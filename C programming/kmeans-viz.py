#Importing required modules
import matplotlib.pyplot as plt
import numpy as np
 
#Load Data
data = np.loadtxt("kmeans_outputMon Jan 30 221421 2023.txt", usecols=(0,1), dtype=np.float32)
#print(data) 

k = 16 # number of clusters
#Load the computed cluster numbers
cluster = np.loadtxt("kmeans_outputMon Jan 30 221421 2023.txt", usecols=(2), dtype=np.float32)

#Plot the results:
for i in range(k):
    plt.scatter(data[cluster == i , 0] , data[cluster == i , 1] , label = i)
plt.show()