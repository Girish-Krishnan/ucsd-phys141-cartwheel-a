/*
 * DISK.CPP: program to generate a disk galaxy model composed of concentric rings
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NPARTICLES 100    // number of particles
#define PI 3.14159265358979 // value of pi

// Define the geometry of the rings in the target galaxy
double ring_radii[4] = {0.0, 0.3, 0.6, 0.9}; // radii of each ring
double ring_widths[4] = {0.1, 0.2, 0.2, 0.2}; // width of each ring

// Define the mass density function of the target galaxy
double mass_density(double r, double a) {
    return exp(-r*r/(2*a*a));
}

int main() {
    // Seed the random number generator
    srand(1234);

    // Define the mass of each particle
    double particle_mass = 1.0;

    // Define the scale length of the mass density function
    double a = 0.2;

    // Define arrays to hold the x, y, z positions and velocities of each particle
    double xpos[NPARTICLES], ypos[NPARTICLES], zpos[NPARTICLES];
    double xvel[NPARTICLES], yvel[NPARTICLES], zvel[NPARTICLES];
    double mass[NPARTICLES];

    // Loop over each ring
    int i, j, np;
    np = 0;
    for (i=0; i<4; i++) {
        // Determine the number of particles in this ring based on its mass
        double ring_mass = 2*PI*ring_radii[i]*ring_widths[i]*mass_density(ring_radii[i], a);
        int nring = (int) round(ring_mass/particle_mass);

        // Loop over each particle in the ring
        double dtheta = 2*PI/nring;
        for (j=0; j<nring; j++) {
            // Generate a position within the ring at equal angle increments
            double theta = j*dtheta;
            double r = ring_radii[i] + ring_widths[i]*0.5;
            xpos[np] = r*cos(theta);
            ypos[np] = r*sin(theta);
            zpos[np] = 0.0;

            // Generate a velocity within the ring
            double v_mag = sqrt(mass_density(r, a)*r);
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
    outfile = fopen("targetgalaxy.dat", "w");
    for (i=0; i<NPARTICLES; i++) {
        fprintf(outfile, "%lf %lf %lf %lf %lf %lf %lf\n", xpos[i], ypos[i], zpos[i], xvel[i], yvel[i], zvel[i], mass[i]);
    }
    fclose(outfile);

    return 0;
}
