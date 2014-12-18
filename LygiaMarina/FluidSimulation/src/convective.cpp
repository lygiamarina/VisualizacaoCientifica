/*  3D Navier-Stokes Solver */

#include "convective.h"
#include <math.h>
#include <errno.h>
#include <stdlib.h>


//####################################################################
//#                                                                  #
//# Discretization of convective Terms:                              #
//#                                                                  #
//#       e.g. discretization of \partial(uu)/\partial x             #
//#                                                                  #
//#    ------------- -------------- -------------- --------------    #
//#   |             |              |              |              |   #
//#   |             |              |              |              |   #
//#   |             |              |              |              |   #
//#   LL            L       KL     M      KR      R              RR  #
//#   |             |              |              |              |   #
//#   |             |              |              |              |   #
//#   |             |              |              |              |   #
//#    ------------- -------------- -------------- --------------    #
//#    \____  _____/ \_____  _____/ \______  ____/ \_____  _____/    #
//#         \/             \/              \/            \/          #
//#         DD             D               DP            PP          #
//#                                                                  #
//####################################################################

double DUU(double LL, double L, double M, double R, double RR, 
                       double DD, double D, double DP, double PP, double alpha, bool border) 
{
   double d, KL=(L+M)/2, KR=(R+M)/2, UR, UL, KRABS, KLABS, Phi_R, Phi_L; 
   //S.Convective pode assumir um dos valores: DonorCell, QUICK, HLPA, SMART, VONOS
   // Sharp and Monotonic Algorithm for Realistic Transport (SMART) 2nd-Order
   UR=(KR>=0) ? 0:1;
   UL=(KL>=0) ? 0:1;      
   if(border==false)
   {         
		Phi_R=(1-UR)*(M-L)/(((R-L)!=0.0)?(R-L):1E-15)  + UR*(R-RR)/(((M-RR)!=0.0)?(M-RR):1E-15);
		Phi_L=(1-UL)*(L-LL)/(((M-LL)!=0.0)?(M-LL):1E-15) + UL*(M-R)/(((L-R)!=0.0)?(L-R):1E-15);
      
		if ((Phi_R >= 0) && (Phi_R < (3./74)))
		{
            UR=(1-UR)*(10*M-9*L)+UR*(10*R-9*RR);
		}
		else 
		   if((Phi_R>=(3./74)) && (Phi_R<(5./6)))
			  UR=(1-UR)*((3.*R+6.*M-L)/8.) + UR*((3.*M+6.*R-RR)/8.);
			  //UR=(1-UR)*((3./8)*R+(6./8)*M-(1./8)*L)+UR*((3./8)*M+(6./8)*R-(1./8)*RR);
		   else 
			  if((Phi_R>=(5./6)) && (Phi_R<=1))
				 UR=(1-UR)*R+UR*M;
			  else 
				 UR=(1-UR)*M+UR*R;

		if((Phi_L>=0) && (Phi_L<(3./74)))
		   UL=(1-UL)*(10*L-9*LL)+UL*(10*M-9*R);
		else 
		   if((Phi_L>=(3./74)) && (Phi_L<(5./6)))
			  UL=(1-UL)*((3.*M+6.*L-LL)/8.) + UL*((3.*L+6.*M-R)/8.);
			  //UL=(1-UL)*((3./8)*M+(6./8)*L-(1./8)*LL)+UL*((3./8)*L+(6./8)*M-(1./8)*R);
		   else 
			  if((Phi_L>=(5./6)) && (Phi_L<=1))
				 UL=(1-UL)*M+UL*L;
			  else 
				 UL=(1-UL)*L+UL*M;

		d=(KR*UR-KL*UL)/(0.5*(D +DP));
	}
	else
	{// Donor-Cell 
		KRABS=fabs(KR);       
		KLABS=fabs(KL);
		d=(KR*(R+M) -KL*(L+M))/(D +DP); // central differences
		// alpha=0 => central differences,  alpha=1 => full upwind
		d=(1-alpha)*d + alpha*( (KR-KRABS)*R + ( (KR+KRABS)-(KL-KLABS) )*M - (KL+KLABS)*L )/(D+DP); 
	}        
    return (d) ;
}


//#################################################################################
//#                                                                               #
//#             e.g. discretization of \partial(uv)/\partial x                    #                 
//#                                                                               #
//#  ------- ------------- -------------- -------------- -------------- -------   #
//#  .      |      .      |      .       |      .       |      .       |      .   #
//#  .      |      .      |      .       |      .       |      .       |      .   #
//#  .      |      .      |      .       |      .       |      .       |      .   #
//#  .      LL     .      L      KL      M      KR      R      .       RR     .   #
//#  .      |      .      |      .       |      .       |      .       |      .   #
//#  .      |      .      |      .       |      .       |      .       |      .   #
//#  .      |      .      |      .       |      .       |      .       |      .   #
//#  --------------------- -------------- -------------- -------------- -------   #
//#   \_____  ____/ \____  _____/ \_____  _____/ \______  ____/ \_____  _____/    #
//#         \/           \/             \/              \/            \/          #
//#         DD           DM             D               DP            PP          #
//#                                                                               #
//#################################################################################

//##########################################################################
//## The discretization for 'mixed' convective term is somewhat different ##
//##########################################################################

double DUV(double LL, double L, double M, double R, double RR, 
			double KL, double KR, double DD, double DM, double D, double DP, double PP, double alpha, bool border) {
    double d, KRABS,KLABS, Phi_L, Phi_R;
    double UR=(KR>=0) ? 0:1;
    double UL=(KL>=0) ? 0:1;
      if(border==false)
      {// Sharp and Monotonic Algorithm for Realistic Transport (SMART) 2nd-Order
         Phi_R=(1-UR)*(M-L)/(((R-L)!=0.0)?(R-L):1E-15)  + UR*(R-RR)/(((M-RR)!=0.0)?(M-RR):1E-15);
         Phi_L=(1-UL)*(L-LL)/(((M-LL)!=0.0)?(M-LL):1E-15) + UL*(M-R)/(((L-R)!=0.0)?(L-R):1E-15);
	
         if((Phi_R>=0) && (Phi_R<(3./74)))
            UR=(1-UR)*(10*M-9*L)+UR*(10*R-9*RR);
         else 
            if((Phi_R>=(3./74)) && (Phi_R<(5./6)))
               UR=(1-UR)*((3.*R+6.*M-L)/8.) + UR*((3.*M+6.*R-RR)/8.);
               //UR=(1-UR)*((3./8)*R+(6./8)*M-(1./8)*L)+UR*((3./8)*M+(6./8)*R-(1./8)*RR);
            else 
               if((Phi_R>=(5./6)) && (Phi_R<=1))
                  UR=(1-UR)*R+UR*M;
               else 
                  UR=(1-UR)*M+UR*R;
	
         if((Phi_L>=0) && (Phi_L<(3./74)))
            UL=(1-UL)*(10*L-9*LL)+UL*(10*M-9*R);
         else 
            if((Phi_L>=(3./74)) && (Phi_L<(5./6)))
               UL=(1-UL)*((3.*M+6.*L-LL)/8.) + UL*((3.*L+6.*M-R)/8.);
               // UL=(1-UL)*((3./8)*M+(6./8)*L-(1./8)*LL)+UL*((3./8)*L+(6./8)*M-(1./8)*R);
            else 
               if((Phi_L>=(5./6)) && (Phi_L<=1))
                  UL=(1-UL)*M+UL*L;
               else 
                  UL=(1-UL)*L+UL*M;
	
         d=(KR*UR-KL*UL)/D;
      }
      else
      {// Donor-Cell full upwind
         KRABS=fabs(KR);       
         KLABS=fabs(KL);
         UR=(D*R +DP*M)/(D +DP);// weighted values at cross points of stagg. gr.
         UL=(DM*M +D*L)/(DM +D);
         d=(KR*UR -KL*UL)/D; 
         // alpha=0 => central differences,  alpha=1 => full upwind
         d=(1-alpha)*d+alpha*( (KR-KRABS)*R + ( (KR+KRABS)-(KL-KLABS) )*M - (KL+KLABS)*L )/(2*D); 
      }
    return (d) ;
}
