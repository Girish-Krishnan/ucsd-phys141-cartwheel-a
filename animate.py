import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

f = open('somefilename')
content = f.readlines()
f.close()

x_nucleus, y_nucleus, z_nucleus = []    # position of host nucleus
x_intruder, y_intruder, z_intruder = [] # position of intruding nucleus
x, y, z = []    # positions of points in disk

'''
read in the positional values from the file Somehow:tm:
'''

# Function to update the plot for each animation frame
def update(frame):
    ax.clear()
    ax.set_xlim(-20, 20)  # Adjust the limits according to your data
    ax.set_ylim(-20, 20)
    ax.set_zlim(-20, 20)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.scatter(x_nucleus[frame], y_nucleus[frame], z_nucleus[frame], color='red', s=20)  # Plot the host galaxy nucleus at the current frame
    ax.scatter(x_intruder[frame], y_intruder[frame], z_intruder[frame], color='blue', s=20)  # Plot the host galaxy nucleus at the current frame
    # plot the disk points at the current frame
    for xpoint, ypoint, zpoint in x, y, z:
        ax.scatter(xpoint[frame], ypoint[frame], zpoint[frame], color='red', s=5)

# Create the figure and 3D axes
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Create the animation
animation = FuncAnimation(fig, update, frames=len(x), interval=200)

# Save the animation as a video
animation.save('galaxies_animation.mp4', writer='ffmpeg')

# Show the animation
plt.show()