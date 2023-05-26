#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

double pdf(double r, double a) {
return 1 / (M_PI * std::pow(a, 2)) * std::exp(-std::pow(r, 2) / (2 * std::pow(a, 2)));
}

double cdf(double r, double a) {
return std::exp(-std::pow(r, 2) / (2 * std::pow(a, 2)));
}

double inverse_cdf(double u, double a) {
return std::sqrt(-2 * std::pow(a, 2) * std::log(u));
}

int main(int argc, char *argv[]) {

if (argc != 8) {
        printf("Usage: ./disk_point_distribution NUM_RINGS NUM_PARTICLES_PER_RING VALUE_OF_G SCALE_LENGTH DISK_RADIUS HOST_MASS <data file name>\n");
        return 1;
}

int n = atoi(argv[1]);
double R = atof(argv[5]);
double a = atof(argv[4]);
char *data_file_name = argv[7];

// Generate uniform samples
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dis(0.0, 1.0);
std::vector<double> u(n);
for (int i = 0; i < n; i++) {
    u[i] = dis(gen);
}

// Transform samples using inverse CDF
std::vector<double> r(n);
for (int i = 0; i < n; i++) {
    r[i] = inverse_cdf(u[i], a);
}

// Round each value in r to 2 decimal places
std::sort(r.begin(), r.end());
for (int i = 0; i < n; i++) {
    r[i] = std::round(r[i] * 100) / 100.0;
}

// Print the rounded values

std::cout << "Radii = ";

for (int i = 0; i < n; i++) {
    std::cout << r[i] << " ";
}

std::cout << std::endl;


// Open disk_settings.dat and write values like this:
/*
NUM_RINGS NUM_PARTICLES_PER_RING G Scale_length
16 16 1.0 10.0
Ring radii
4.44 5.51 5.8 6.28 6.46 7.41 8.12 9.66 12.19 12.37 13.96 14.14 15.37 16.21 22.41 29.85
*/

// Open disk_settings.dat
FILE *fp;
fp = fopen(data_file_name, "w");
if (fp == NULL) {
    printf("Error opening file!\n");
    return 1;
}

// Write NUM_RINGS, NUM_PARTICLES_PER_RING, G, and Scale_length
fprintf(fp, "NUM_RINGS NUM_PARTICLES_PER_RING G Scale_length\n");
fprintf(fp, "%d %d %s %s\n", n, atoi(argv[2]), argv[3], argv[4]);
fprintf(fp, "Ring radii\n");
// Lastly, write the rounded values of r
for (int i = 0; i < n; i++) {
    fprintf(fp, "%s ", std::to_string(r[i]).c_str());
}
fprintf(fp, "\n");

fprintf(fp, "Host mass\n");
fprintf(fp, "%s\n", argv[6]);

// Close disk_settings.dat
fclose(fp);

return 0;
}