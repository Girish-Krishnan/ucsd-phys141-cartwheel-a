import matplotlib
matplotlib.use('Agg')  # Use the Agg backend to save animations without having a window appear
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation
import sys

if len(sys.argv) != 3:
    print('Usage: python animate.py <data_log_filename> <output video filename>')
    sys.exit(1)

filename = sys.argv[1]
output_filename = sys.argv[2]

# Read the .dat file
data = np.genfromtxt(filename)

# Transpose the data array to access columns as separate lists
columns = (data.T)[:7,:].tolist()
point_positions = (data.T)[7:,:].tolist()

# Assign the columns to their respective variables
time = columns[0]
x_nucleus = columns[1]
y_nucleus = columns[2]
z_nucleus = columns[3]    # position of host nucleus
x_intruder = columns[4]
y_intruder = columns[5]
z_intruder = columns[6] # position of intruding nucleus

# Let x be a list of lists containing the x positions of the points in the disk at each time step
x = point_positions[0::3]
y = point_positions[1::3]
z = point_positions[2::3]
    

'''
read in the positional values from the file
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
    # Set title
    ax.set_title('Time: {:.2f}'.format(time[frame]))
    
    # Plot the points in the disk at the current frame
    for i in range(len(x)):
        ax.scatter(x[i][frame], y[i][frame], z[i][frame], color='black', s=1)

# Create the figure and 3D axes
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
# ax.view_init(elev=30, azim=45)  # Set elevation to 30 degrees and azimuth to 45 degrees

# Create the animation
animation = FuncAnimation(fig, update, frames=len(x[0]), interval=50)

# Save the animation as a video
animation.save(output_filename, writer='ffmpeg')

# Show the animation
#plt.show()