/*
 * DISK.CPP: program to generate a disk galaxy model composed of concentric rings
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define G = 1.0

void leapstep(double m_nucleus, double x_nucleus, double y_nucleus, double z_nucleus, double vx_nucleus, double vy_nucleus, double vz_nucleus,
              double m_intruder, double x_intruder, double y_intruder, double z_intruder, double vx_intruder, double vy_intruder, double vz_intruder,
              double m[], double x[], double y[], double z[], double vx[], double vy[], double vz[],
              int n, double dt);
void accel(double m_nucleus, double ax_nucleus, double ay_nucleus, double az_nucleus, double x_nucleus, double y_nucleus, double z_nucleus,
           double m_intruder, double ax_intruder, double ay_intruder, double az_intruder, double x_intruder, double y_intruder, double z_intruder,
           double m[], double ax[], double ay[], double az[], double x[], double y[], double z[],
           int n);
double distance(double x1, double y1, double z1, double x2, double y2, double z2);

int main(){
    // host nucleus data
    double m_nucleus = ;    // read or pass in value somehow
    double x_nucleus = ;    // read or pass in value somehow
    double y_nucleus = ;
    double z_nucleus = ;
    double vx_nucleus = ;
    double vy_nucleus = ;
    double vz_nucleus = ;

    // intruding nucleus data
    double m_intruder = ;    // read or pass in value somehow
    double x_intruder = ;    // read or pass in value somehow
    double y_intruder = ;
    double z_intruder = ;
    double vx_intruder = ;
    double vy_intruder = ;
    double vz_intruder = ;

    int n = ;   // number of points

    // disk particle data
    // read or pass in values somehow
    double m[n];
    double x[n], y[n], z[n];
    double vx[n], vy[n], vz[n];

    // log positions to plot later
    double xlog_nucleus[], ylog_nucleus[], zlog_nucleus[];
    double xlog_intruder[], ylog_intruder[], zlog_intruder[];
    double xlog[n][], ylog[n][], zlog[n][];

    double tnow = 0.0;  // set initial time

    // next, set integration parameters
    int nout = 10;  // steps between outputs
    double dt = 0.01;  // timestep for integration (something, idk this is just a placeholder)
    int mstep = 100;    // idk what this should be

    // now, loop performing integration
    for(int nstep=0; nstep<mstep; nstep++){
        if(nstep%nout == 0){    // if time to log state
            // then record log
            // write log code later
        }

        // take integration step
        leapstep(m_nucleus, x_nucleus, y_nucleus, z_nucleus, vx_nucleus, vy_nucleus, vz_nucleus,
                 m_intruder, x_intruder, y_intruder, z_intruder, vx_intruder, vy_intruder, vz_intruder,
                 m, x, y, z, vx, vy, vz,
                 n, dt);
        tnow += dt; // update time value
    }

    if(nstep%nout == 0){    // if time to log state
        // then record log
        // write log code later
    }
}

void leapstep(double m_nucleus, double x_nucleus, double y_nucleus, double z_nucleus, double vx_nucleus, double vy_nucleus, double vz_nucleus,
              double m_intruder, double x_intruder, double y_intruder, double z_intruder, double vx_intruder, double vy_intruder, double vz_intruder,
              double m[], double x[], double y[], double z[], double vx[], double vy[], double vz[],
              int n, double dt)
{
    // initialize accelerations
    double ax_nucleus, ay_nucleus, az_nucleus = 0;
    double ax_intruder, ay_intruder, az_intruder = 0;
    double ax[n], ay[n], az[n];

    // kick
    accel(m_nucleus, ax_nucleus, ay_nucleus, ax_nucleus, x_nucleus, y_nucleus, z_nucleus,
          m_intruder, ax_intruder, ay_intruder, az_intruder, x_intruder, y_intruder, z_intruder,
          m, ax, ay, az, x, y, z, n);

    // drift host nucleus
    vx_nucleus = vx_nucleus + 0.5 * dt * ax_nucleus;
    vy_nucleus = vy_nucleus + 0.5 * dt * ay_nucleus;
    vz_nucleus = vz_nucleus + 0.5 * dt * az_nucleus;
    x_nucleus = x_nucleus + dt * vx_nucleus
    y_nucleus = y_nucleus + dt * vy_nucleus
    z_nucleus = z_nucleus + dt * vz_nucleus
    // drift intruder nucleus
    vx_intruder = vx_intruder + 0.5 * dt * ax_intruder;
    vy_intruder = vy_intruder + 0.5 * dt * ay_intruder;
    vz_intruder = vz_intruder + 0.5 * dt * az_intruder;
    x_intruder = x_intruder + dt * vx_intruder
    y_intruder = y_intruder + dt * vy_intruder
    z_intruder = z_intruder + dt * vz_intruder
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
    accel(m_nucleus, ax_nucleus, ay_nucleus, ax_nucleus, x_nucleus, y_nucleus, z_nucleus,
          m_intruder, ax_intruder, ay_intruder, az_intruder, x_intruder, y_intruder, z_intruder,
          m, ax, ay, az, x, y, z, n);

    // complete vel step
    vx_nucleus = vx_nucleus + 0.5 * dt * ax_nucleus;
    vy_nucleus = vy_nucleus + 0.5 * dt * ay_nucleus;
    vz_nucleus = vz_nucleus + 0.5 * dt * az_nucleus;
    vx_intruder = vx_intruder + 0.5 * dt * ax_intruder;
    vy_intruder = vy_intruder + 0.5 * dt * ay_intruder;
    vz_intruder = vz_intruder + 0.5 * dt * az_intruder;
    for(int i=0; i<n; i++){
        vx[i] = vx[i] + 0.5 * dt * ax[i];
        vy[i] = vy[i] + 0.5 * dt * ay[i];
        vz[i] = vz[i] + 0.5 * dt * az[i];
    }
}

void accel(double m_nucleus, double ax_nucleus, double ay_nucleus, double az_nucleus, double x_nucleus, double y_nucleus, double z_nucleus,
           double m_intruder, double ax_intruder, double ay_intruder, double az_intruder, double x_intruder, double y_intruder, double z_intruder,
           double m[], double ax[], double ay[], double az[], double x[], double y[], double z[],
           int n)
{
    // calc acceleration of host nucleus (only calculates force between it and the intruder nucleus, not the points in the disk)
    double r = distance(x_nucleus, y_nucleus, z_nucleus, x_intruder, y_intruder, z_intruder);
    ax_nucleus = -G * m_intruder * (x_nucleus - x_intruder)/pow(r, 3)
    ay_nucleus = -G * m_intruder * (y_nucleus - y_intruder)/pow(r, 3)
    az_nucleus = -G * m_intruder * (z_nucleus - z_intruder)/pow(r, 3)

    // calc acceleration of intruder nucleus (only calculates force between it and the host nucleus, not the points in the disk)
    ax_intruder = -G * m_nucleus * (x_intruder - x_nucleus)/pow(r, 3)
    ay_nucleus = -G * m_nucleus * (y_intruder - y_nucleus)/pow(r, 3)
    az_nucleus = -G * m_nucleus * (z_intruder - z_nucleus)/pow(r, 3)

    // calc acceleration of disk particles
    for(int i=0; i<n; i++){
        double r_to_nucleus = distance(x[i], y[i], z[i], x_nucleus, y_nucleus, z_nucleus)
        double r_to_intruder = distance(x[i], y[i], z[i], x_intruder, y_intruder, z_intruder)
        ax[i] = (-G * m_nucleus * (x[i] - x_nucleus)/pow(r_to_nucleus, 3)) + (-G * m_intruder * (x[i] - x_intruder)/pow(r_to_intruder, 3))
        ay[i] = (-G * m_nucleus * (y[i] - y_nucleus)/pow(r_to_nucleus, 3)) + (-G * m_intruder * (y[i] - y_intruder)/pow(r_to_intruder, 3))
        az[i] = (-G * m_nucleus * (z[i] - z_nucleus)/pow(r_to_nucleus, 3)) + (-G * m_intruder * (z[i] - z_intruder)/pow(r_to_intruder, 3))
    }
}

double distance(double x1, double y1, double z1, double x2, double y2, double z2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2))
}