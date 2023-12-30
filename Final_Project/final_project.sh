# SETUP
source $HOME/opt/nemo/nemo_start.sh
PATH=/Applications/glnemo2.app/Contents/MacOS/:$PATH
rm -f ./*.ic ./*.out

# Key points in the paper

# Barred vs. non-barred
# Properties of target disc: mass dist, velocity dispersion
# Properties of companion: mass, orbit
# Properties of rings: amplitude, width, shape, expansion velocity, lifetime

# Formation and evolution of spokes
# companions of relatively small mass

# Oblique impacts - transient displacement of the bar, changes in pattern speed and size, formation of an asymmetric pseudo ring

# INTRO AND PROBLEM STATEMENT

# What are ring galaxies?
# Pronounced ring structure surrounding an apparently empty region - an offcentered nucleus can be seen
# Low-density environments
# Have a companion that lies preferentially near the minor axis of the ring
# Collision between companion and target led to ring formation

# As intruder approaches disc, the extra inwards gravitational force on disc particles increases
# causes orbits to contract
# When companion leaves...
# there is a strong rebound
# orbits crowd together and
# a high-amplitude, transient density wave is formed - propagates outward
# It is possible for second or third rebounds to happen -- leading to more rings

# PREVIOUS WORK
# Huang & Stewart (1988), Appleton and Hames (1990), Hernquist and Weil (1993), Horelou and Comes (1993)
# Relatively high-mass companions - 1x, 0.4x, 0.333x the target galaxy mass

# THIS PAPER
# infall of a small companion galaxy on a non-barred or barred target galaxy
# Use companions of relatively low mass
# 0.02 to 0.2x
# To ensure that the disk survives the collision and the ring can be considered as a perturbation
# comparable to that of resonant rings of ringed galaxies
# Another goal: study the morphology and kinematics of the rings formed by impacts and compare them with the corresponding ones for ringed galaxies
# Consider the evolution in both non-barred and barred target discs

# NUMERICAL TECHNIQUES

# Create the companion galaxy -- Plummer sphere
# cs = standard companion
# csd = double the mass of cs
# csh = half the mass of cs

# For R, C runs
# The ratio of the number of particles equals the mass ratio b/w different components

# for S runs
# masses of disc, halo, bulge are
# 4e-5, 3.57e-5, 5e-5
# Ratio: 1: 3.34: 0.31

# Mass of particles in companion cs: 4.9657e-5
# in companion c1: 5e-5

# UNITS
# G = 1
# Length = 3 kpc
# Time = 1e7 year

# Mass = 6e10 solar masses
# velocity = 293 km/s
# volume denisty = 2.22 solar masses per pc^3

# SIMULATIONS

# Init positions and velocities

# Decide the impact point and velocity for model s1 and companion cs
# backcalculate the orbit in time for a time interval long enough so that the distance between the center of the target galaxy and companion is longer than 1.4R_H
# while keep target galaxy particles fixed

# Due to evolution of disc, and due to differences between discs,
# the companion never hits the target at EXACTLY at the impact point

# stored every 2 time units for C55-99
# but everywhere else every 1 time unit

# vectorized version of treecode, tolerance of 1 for S runs
# quadruple terms in force calc

# Softening: 0.066666 for R runs
# 0.05 for S runs
# timestep = 0.025 for R and 0.05 for S
# Run for 80 computer units == 8e8 years

# Parallelized over 40 chips , speed of 20 Gflops, one time step only 3 s

# IMAGES in paper
# C59
# C58, C55, C57 to compare companion mass
# C55, 85, 88
# C57, 86, 89
# Spokes: C61
# Diff Q value: C57 C91
# Amplitude vs time
# R7 - barred target galaxy
# R9
# S1 - spokes

# The effects on the vertical structure of the disk are not explained here - likely in a future paper
# Galaxy = HALO + DISK + BULGE

# Models m1, m2, m3, s1: 
# DISK: Kuzmin/Toomre (KT) projected radial density profile
# Vertical dist: sech^2(z/z0)
# BULGE and HALO: truncated Plummer (PL) profile

# Radial velocity dispersions are chosen so that Qtoomre doesn't depend on radius and equals 1, 1.1, 1.2, or 1.5
# m1 = standard
# m2 = extended
# m3 = very extended
# mb = no bulge; bar unstable
# mh = barred galaxy + redistributed the disc particles randomly in azimuth

# Setting up
# First construct HALO and BULGE components separately and superpose them and allow them to relax


# Choose positions for disc particles, using prescribed denisty profiles
# Tabulate the forces from this distribution of particles on a grid

# S1 simulation

mkplummer out=companion.ic nbody=4000
TODO: requires characteristic and cutoff radius
mass = 0.2 to 0.02 of target

magalie target_halo_bulge.ic ndisk=0 nbulge=8000 nhalo=10000
gyrfalcON target_halo_bulge.ic target_halo_bulge.out tstop=50 step=0.1 kmax=6 eps=0.066

###
TODO: use magalie or mkgalaxy to set positions of disc particles
Then let the initial galaxy relax
###

# Create the target galaxy - Disk, Halo, Bulge
magalie target.ic ndisk=2000 nbulge=8000 nhalo=10000
# Prepare simulation
snapstack target.ic companion.ic collision.ic deltar=0,0,10 deltav=0,0,-1
# View initial state
glnemo2 collision.ic 0:9999,20000:29999
# Run the collision
gyrfalcON colision.ic collision.out tstop=50 step=0.1 kmax=6 eps=0.066
# View simulation
glnemo2 collision.out 0:9999,20000:29999

# RESULTS
# The amplitude, width, lifetime, and expansion velocity of first ring increases w/companion mass
# and so does the expansion velocity of particles within it and the total extent of the disc

# Create target galaxy with magalie for just testing purposes
rm -f target_magalie.ic
magalie out=target_magalie.ic ndisk=8000 nbulge=2000 nhalo=30000
glnemo2 target_magalie.ic 0:9999

# Companion plummer has already been created

# Combining
rm -f combined_magalie.ic
snapstack target_magalie.ic companion_plummer.snp combined_magalie.ic deltar=0,0,10 deltav=0,0,-5
glnemo2 combined_magalie.ic 0:9999,40000:43999

# Running the sim and visualizing
rm -f output_magalie.out
gyrfalcON combined_magalie.ic output_magalie.out tstop=50 step=0.1 kmax=6 eps=0.066
glnemo2 output_magalie.out 0:9999,40000:43999

# Create the s1_2 model
#mkgalaxy name=s1_2 Nh=1200000 Nd=320000 Nb=80000 Md=1 Rd=1 Zd=8 Rdmax=5.0 Mh=1.0733 Rh=5 Rth=10 Mb=0.0997 Rtb=10
