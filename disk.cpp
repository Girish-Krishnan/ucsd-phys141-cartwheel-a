/*
 * DISK.CPP: program to generate a disk galaxy model composed of concentric rings
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1000 // maximum number of characters in a line of the input file

// Define the mass density function of the target galaxy
double mass_density(double r, double a) {
    return exp(-r*r/(2*a*a));
}

int main(int argc, char *argv[]) {

    printf("Generating disk galaxy model...\n");

    // Read the setup file
    if (argc != 3) {
        printf("Usage: ./disk <setup_file> <output_file>\n");
        return 1;
    }

    char *setup_file = argv[1];
    char *output_file = argv[2];

    FILE *file;
    char buffer[MAX_BUFFER_SIZE];
    int numRings, numParticlesPerRing;
    double G, scaleLength;
    double *ringRadii, *ringWidths;

    file = fopen(setup_file, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read NUM_RINGS, NUM_PARTICLES_PER_RING, G, Scale_length
    fgets(buffer, MAX_BUFFER_SIZE, file);
    // Clear buffer and run fgets again
    memset(buffer, 0, MAX_BUFFER_SIZE);
    fgets(buffer, MAX_BUFFER_SIZE, file);
    sscanf(buffer, "%d %d %lf %lf", &numRings, &numParticlesPerRing, &G, &scaleLength);

    // print the parameters to the screen
    printf("numRings = %d\n", numRings);
    printf("numParticlesPerRing = %d\n", numParticlesPerRing);
    printf("G = %lf\n", G);
    printf("scaleLength = %lf\n", scaleLength);

    // Allocate memory for ringRadii and ringWidths arrays
    ringRadii = (double *)malloc(numRings * sizeof(double));
    ringWidths = (double *)malloc(numRings * sizeof(double));

    // Read ring radii
    fgets(buffer, MAX_BUFFER_SIZE, file);
    memset(buffer, 0, MAX_BUFFER_SIZE);
    fgets(buffer, MAX_BUFFER_SIZE, file);
    char *token = strtok(buffer, " ");
    for (int i = 0; i < numRings; i++) {
        ringRadii[i] = atof(token);
        token = strtok(NULL, " ");
    }

    printf("ringRadii = ");
    for (int i = 0; i < numRings; i++) {
        printf("%lf ", ringRadii[i]);
    }

    // Read ring widths
    fgets(buffer, MAX_BUFFER_SIZE, file);
    memset(buffer, 0, MAX_BUFFER_SIZE);
    fgets(buffer, MAX_BUFFER_SIZE, file);
    token = strtok(buffer, " ");
    for (int i = 0; i < numRings; i++) {
        ringWidths[i] = atof(token);
        token = strtok(NULL, " ");
    }

    printf("\nringWidths = ");
    for (int i = 0; i < numRings; i++) {
        printf("%lf ", ringWidths[i]);
    }

    // Close the file
    fclose(file);

    // Define the scale length of the mass density function
    int NUM_PARTICLES = numRings * numParticlesPerRing;

    // Define arrays to hold the x, y, z positions and velocities of each particle
    double xpos[NUM_PARTICLES], ypos[NUM_PARTICLES], zpos[NUM_PARTICLES];
    double xvel[NUM_PARTICLES], yvel[NUM_PARTICLES], zvel[NUM_PARTICLES];
    double mass[NUM_PARTICLES];

    // Loop over each ring
    int i, j, np;
    np = 0;
    for (i=0; i<numRings; i++) {
        // Determine the number of particles in this ring based on its mass
        double ring_mass = 2*M_PI*ringRadii[i]*ringWidths[i]*mass_density(ringRadii[i], scaleLength);
        double particle_mass = ring_mass/numParticlesPerRing;
        // Loop over each particle in the ring
        double dtheta = 2*M_PI/numParticlesPerRing;
        for (j=0; j<numParticlesPerRing; j++) {
            // Generate a position within the ring at equal angle increments
            double theta = j*dtheta;
            double r = ringRadii[i] + ringWidths[i]*0.5;
            xpos[np] = r*cos(theta);
            ypos[np] = r*sin(theta);
            zpos[np] = 0.0;

            // Generate a velocity within the ring
            double v_mag = sqrt(G*particle_mass/r);
            xvel[np] = -v_mag*sin(theta);
            yvel[np] = v_mag*cos(theta);
            zvel[np] = 0.0;

            // Assign the mass of the particle
            mass[np] = particle_mass;

            np++;
        }

    }

    // Output the particle positions and velocities to a file
    FILE *outfile;
    outfile = fopen(output_file, "w");
    for (i=0; i<NUM_PARTICLES; i++) {
        fprintf(outfile, "%lf %lf %lf %lf %lf %lf %lf\n", xpos[i], ypos[i], zpos[i], xvel[i], yvel[i], zvel[i], mass[i]);
    }
    fclose(outfile);

    return 0;
}
