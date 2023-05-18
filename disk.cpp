/*
 * DISK.CPP: program to generate a disk galaxy model composed of concentric rings
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979 // value of pi
#define NUM_RINGS 16 // number of rings in the target galaxy
#define NUM_PARTICLES_PER_RING 16 // number of particles in the target galaxy
#define G 1.0 // gravitational constant

// Define the geometry of the rings in the target galaxy
double ring_radii[NUM_RINGS] = {1.0,2.0,3.0, 4.0, 5.0, 6.0, 7.0, 8.5, 10.0, 11.5, 13.0, 15.0, 17.0, 19.0, 22.0, 25.0}; // radii of each ring
double ring_widths[NUM_RINGS] = {0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.2,0.2,0.2,0.2,0.3,0.3,0.3,0.4,0.4}; // widths of each ring

// Define the mass density function of the target galaxy
double mass_density(double r, double a) {
    return exp(-r*r/(2*a*a));
}

int main() {

    // Define the scale length of the mass density function
    double a = 10;
    int NUM_PARTICLES = NUM_RINGS*NUM_PARTICLES_PER_RING;

    // Define arrays to hold the x, y, z positions and velocities of each particle
    double xpos[NUM_PARTICLES], ypos[NUM_PARTICLES], zpos[NUM_PARTICLES];
    double xvel[NUM_PARTICLES], yvel[NUM_PARTICLES], zvel[NUM_PARTICLES];
    double mass[NUM_PARTICLES];

    // Loop over each ring
    int i, j, np;
    np = 0;
    for (i=0; i<NUM_RINGS; i++) {
        // Determine the number of particles in this ring based on its mass
        double ring_mass = 2*PI*ring_radii[i]*ring_widths[i]*mass_density(ring_radii[i], a);
        double particle_mass = ring_mass/NUM_PARTICLES_PER_RING;
        // Loop over each particle in the ring
        double dtheta = 2*PI/NUM_PARTICLES_PER_RING;
        for (j=0; j<NUM_PARTICLES_PER_RING; j++) {
            // Generate a position within the ring at equal angle increments
            double theta = j*dtheta;
            double r = ring_radii[i] + ring_widths[i]*0.5;
            xpos[np] = r*cos(theta);
            ypos[np] = r*sin(theta);
            zpos[np] = 0.0;

            // Generate a velocity within the ring
            double v_mag = sqrt(G*particle_mass/r);
            double v_theta = theta + PI/2;
            xvel[np] = -v_mag*sin(v_theta);
            yvel[np] = v_mag*cos(v_theta);
            zvel[np] = 0.0;

            // Assign the mass of the particle
            mass[np] = particle_mass;

            np++;
        }

    }

    // Output the particle positions and velocities to a file
    FILE *outfile;
    outfile = fopen("disk_initial_conditions.dat", "w");
    for (i=0; i<NUM_PARTICLES; i++) {
        fprintf(outfile, "%lf %lf %lf %lf %lf %lf %lf\n", xpos[i], ypos[i], zpos[i], xvel[i], yvel[i], zvel[i], mass[i]);
    }
    fclose(outfile);

    return 0;
}
