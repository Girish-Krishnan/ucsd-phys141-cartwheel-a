# Imports
import matplotlib.pyplot as plt
import numpy as np
# 3d plotting
from mpl_toolkits.mplot3d import Axes3D

# Read data from targetgalaxy.dat and store in arrays
data = np.loadtxt('targetgalaxy.dat')
x = data[:,0]
y = data[:,1]
z = data[:,2]
vx = data[:,3]
vy = data[:,4]
vz = data[:,5]
m = data[:,6]

# Plot the x,y,z positions of the particles in matplotlib, and plot the points in different colors based on their mass, and include a legend
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x, y, z, c=m, marker='o')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.title('3D Plot of Target Galaxy')
plt.show()


