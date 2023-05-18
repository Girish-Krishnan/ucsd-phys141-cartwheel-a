/*
 * DISK.CPP: program to generate a disk galaxy model composed of concentric rings
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define G 1.0

void leapstep(double m_nucleus, double *x_nucleus, double *y_nucleus, double *z_nucleus, double *vx_nucleus, double *vy_nucleus, double *vz_nucleus,
              double m_intruder, double *x_intruder, double *y_intruder, double *z_intruder, double *vx_intruder, double *vy_intruder, double *vz_intruder,
              double m[], double x[], double y[], double z[], double vx[], double vy[], double vz[],
              int n, double dt);
void accel(double m_nucleus, double *ax_nucleus, double *ay_nucleus, double *az_nucleus, double x_nucleus, double y_nucleus, double z_nucleus,
           double m_intruder, double *ax_intruder, double *ay_intruder, double *az_intruder, double x_intruder, double y_intruder, double z_intruder,
           double m[], double ax[], double ay[], double az[], double x[], double y[], double z[],
           int n);
double distance(double x1, double y1, double z1, double x2, double y2, double z2);

int main(){

    // host nucleus data
    double m_nucleus = 10.0;    // read or pass in value somehow
    double x_nucleus = 0.0;    // read or pass in value somehow
    double y_nucleus = 0.0;
    double z_nucleus = 0.0;
    double vx_nucleus = 0.0;
    double vy_nucleus = 0.0;
    double vz_nucleus = 0.0;

    // intruding nucleus data
    double m_intruder = 10.0;    // read or pass in value somehow
    double x_intruder = 0.0;    // read or pass in value somehow
    double y_intruder = 0.0;
    double z_intruder = 10.0;
    double vx_intruder = 0.0;
    double vy_intruder = 0.0;
    double vz_intruder = -10.0;

    // next, set integration parameters
    int nout = 2;  // steps between outputs
    double dt = 0.01;  // timestep for integration
    int mstep = 500;    // number of steps to take

    // disk particle data
    int n = 256; // number of points in disk
    double m[n];
    double x[n], y[n], z[n];
    double vx[n], vy[n], vz[n];

    FILE *disk_file = fopen("disk_initial_conditions.dat", "r");
    
    int count = 0;
    while (count < n &&
           fscanf(disk_file, "%lf %lf %lf %lf %lf %lf %lf",
                  &x[count], &y[count], &z[count],
                  &vx[count], &vy[count], &vz[count], &m[count]) == 7) {
        count++;
    }

    fclose(disk_file);

    // log positions to plot later
    double xlog_nucleus[mstep/nout], ylog_nucleus[mstep/nout], zlog_nucleus[mstep/nout];
    double xlog_intruder[mstep/nout], ylog_intruder[mstep/nout], zlog_intruder[mstep/nout];
    double xlog[n][mstep/nout], ylog[n][mstep/nout], zlog[n][mstep/nout];
    double tlog[mstep/nout];

    double tnow = 0.0;  // set initial time

    // now, loop performing integration
    for(int nstep=0; nstep<mstep; nstep++){
        if(nstep%nout == 0){    // if time to log state
            // then record log
            // log positions of host nucleus, intruder nucleus, and disk points
            xlog_nucleus[nstep/nout] = x_nucleus;
            ylog_nucleus[nstep/nout] = y_nucleus;
            zlog_nucleus[nstep/nout] = z_nucleus;
            xlog_intruder[nstep/nout] = x_intruder;
            ylog_intruder[nstep/nout] = y_intruder;
            zlog_intruder[nstep/nout] = z_intruder;
            for(int i=0; i<n; i++){
                xlog[i][nstep/nout] = x[i];
                ylog[i][nstep/nout] = y[i];
                zlog[i][nstep/nout] = z[i];
            }

            tlog[nstep/nout] = tnow;

        }

        // take integration step
        leapstep(m_nucleus, &x_nucleus, &y_nucleus, &z_nucleus, &vx_nucleus, &vy_nucleus, &vz_nucleus,
                 m_intruder, &x_intruder, &y_intruder, &z_intruder, &vx_intruder, &vy_intruder, &vz_intruder,
                 m, x, y, z, vx, vy, vz,
                 n, dt);

        tnow += dt; // update time value

    }
    
    // open a .dat file to write log data
    FILE *data_log;
    data_log = fopen("data_log.dat", "w");

    // write log data to file, for the target nucleus, the intruder nucleus, and the disk points, as well as timestamp
    for(int i=0; i<mstep/nout; i++){
        fprintf(data_log, "%f %f %f %f %f %f %f", tlog[i], xlog_nucleus[i], ylog_nucleus[i], zlog_nucleus[i],
                xlog_intruder[i], ylog_intruder[i], zlog_intruder[i]);

        for(int j=0; j<n; j++){
            fprintf(data_log, " %f %f %f", xlog[j][i], ylog[j][i], zlog[j][i]);
        }

        fprintf(data_log, "\n");
    }

    // close log file
    fclose(data_log);

    return 0;
}

void leapstep(double m_nucleus, double *x_nucleus, double *y_nucleus, double *z_nucleus, double *vx_nucleus, double *vy_nucleus, double *vz_nucleus,
              double m_intruder, double *x_intruder, double *y_intruder, double *z_intruder, double *vx_intruder, double *vy_intruder, double *vz_intruder,
              double m[], double x[], double y[], double z[], double vx[], double vy[], double vz[],
              int n, double dt)
{
    // initialize accelerations
    double ax_nucleus, ay_nucleus, az_nucleus = 0;
    double ax_intruder, ay_intruder, az_intruder = 0;
    double ax[n], ay[n], az[n];

    // kick
    accel(m_nucleus, &ax_nucleus, &ay_nucleus, &az_nucleus, *x_nucleus, *y_nucleus, *z_nucleus,
          m_intruder, &ax_intruder, &ay_intruder, &az_intruder, *x_intruder, *y_intruder, *z_intruder,
          m, ax, ay, az, x, y, z, n);

    // drift host nucleus
    *vx_nucleus = *vx_nucleus + 0.5 * dt * ax_nucleus;
    *vy_nucleus = *vy_nucleus + 0.5 * dt * ay_nucleus;
    *vz_nucleus = *vz_nucleus + 0.5 * dt * az_nucleus;
    *x_nucleus = *x_nucleus + dt * (*vx_nucleus);
    *y_nucleus = *y_nucleus + dt * (*vy_nucleus);
    *z_nucleus = *z_nucleus + dt * (*vz_nucleus);
    // drift intruder nucleus
    *vx_intruder = *vx_intruder + 0.5 * dt * ax_intruder;
    *vy_intruder = *vy_intruder + 0.5 * dt * ay_intruder;
    *vz_intruder = *vz_intruder + 0.5 * dt * az_intruder;
    *x_intruder = *x_intruder + dt * *vx_intruder;
    *y_intruder = *y_intruder + dt * *vy_intruder;
    *z_intruder = *z_intruder + dt * *vz_intruder;
    // drift disk points
    for(int i=0; i<n; i++){
        vx[i] = vx[i] + 0.5 * dt * ax[i];
        vy[i] = vy[i] + 0.5 * dt * ay[i];
        vz[i] = vz[i] + 0.5 * dt * az[i];
        x[i] = x[i] + dt * vx[i];
        y[i] = y[i] + dt * vy[i];
        z[i] = z[i] + dt * vz[i];
    }

    // kick
    accel(m_nucleus, &ax_nucleus, &ay_nucleus, &ax_nucleus, *x_nucleus, *y_nucleus, *z_nucleus,
          m_intruder, &ax_intruder, &ay_intruder, &az_intruder, *x_intruder, *y_intruder, *z_intruder,
          m, ax, ay, az, x, y, z, n);

    

    // complete vel step
    // drift host nucleus
    *vx_nucleus = *vx_nucleus + 0.5 * dt * ax_nucleus;
    *vy_nucleus = *vy_nucleus + 0.5 * dt * ay_nucleus;
    *vz_nucleus = *vz_nucleus + 0.5 * dt * az_nucleus;
    // drift intruder nucleus
    *vx_intruder = *vx_intruder + 0.5 * dt * ax_intruder;
    *vy_intruder = *vy_intruder + 0.5 * dt * ay_intruder;
    *vz_intruder = *vz_intruder + 0.5 * dt * az_intruder;

    for(int i=0; i<n; i++){
        vx[i] = vx[i] + 0.5 * dt * ax[i];
        vy[i] = vy[i] + 0.5 * dt * ay[i];
        vz[i] = vz[i] + 0.5 * dt * az[i];
    }

}

void accel(double m_nucleus, double *ax_nucleus, double *ay_nucleus, double *az_nucleus, double x_nucleus, double y_nucleus, double z_nucleus,
           double m_intruder, double *ax_intruder, double *ay_intruder, double *az_intruder, double x_intruder, double y_intruder, double z_intruder,
           double m[], double ax[], double ay[], double az[], double x[], double y[], double z[],
           int n)
{
    // calc acceleration of host nucleus (only calculates force between it and the intruder nucleus, not the points in the disk)
    double r = distance(x_nucleus, y_nucleus, z_nucleus, x_intruder, y_intruder, z_intruder);
    *ax_nucleus = -G * m_intruder * (x_nucleus - x_intruder)/pow(r, 3);
    *ay_nucleus = -G * m_intruder * (y_nucleus - y_intruder)/pow(r, 3);
    *az_nucleus = -G * m_intruder * (z_nucleus - z_intruder)/pow(r, 3);

    // calc acceleration of intruder nucleus (only calculates force between it and the host nucleus, not the points in the disk)
    *ax_intruder = -G * m_nucleus * (x_intruder - x_nucleus)/pow(r, 3);
    *ay_intruder = -G * m_nucleus * (y_intruder - y_nucleus)/pow(r, 3);
    *az_intruder = -G * m_nucleus * (z_intruder - z_nucleus)/pow(r, 3);

    // calc acceleration of disk particles
    for(int i = 0; i < n; i++){
        double r_to_nucleus = distance(x[i], y[i], z[i], x_nucleus, y_nucleus, z_nucleus);
        double r_to_intruder = distance(x[i], y[i], z[i], x_intruder, y_intruder, z_intruder);
        ax[i] = (-G * m_nucleus * (x[i] - x_nucleus)/pow(r_to_nucleus, 3)) + (-G * m_intruder * (x[i] - x_intruder)/pow(r_to_intruder, 3));
        ay[i] = (-G * m_nucleus * (y[i] - y_nucleus)/pow(r_to_nucleus, 3)) + (-G * m_intruder * (y[i] - y_intruder)/pow(r_to_intruder, 3));
        az[i] = (-G * m_nucleus * (z[i] - z_nucleus)/pow(r_to_nucleus, 3)) + (-G * m_intruder * (z[i] - z_intruder)/pow(r_to_intruder, 3));
    }
}




double distance(double x1, double y1, double z1, double x2, double y2, double z2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}