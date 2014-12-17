#ifndef CONVECTIVE_H
#define CONVECTIVE_H

double DUU(double LL, double L, double M, double R, double RR, 
                    double DD, double D, double DP, double PP, double alpha, bool border);
                       
double DUV(double LL, double L, double M, double R, double RR, 
					double KL, double KR, double DD, double DM, double D, double DP, double PP, double alpha, bool border);

#endif
