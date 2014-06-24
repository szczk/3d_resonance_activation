#ifndef _RANDOMS_HH_
#define _RANDOMS_HH_

#include <cmath>
#include <iostream>
#include <limits>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

// #include "TH1D.h"


using namespace std;


/**
 * utility random numbers generator
 *
 *
 *
 * **** thread safety: yes, as long as every thread makes his own object :)
 *
 */

class Randoms {

private:
     //static Randoms* INSTANCE;


     gsl_rng * generator;


     void init();
     void destroy();

     double m_drawSkewLevy ( double alpha );

public:
//   static Randoms* getInstance();

     Randoms();
     Randoms ( const Randoms & );
     Randoms& operator= ( const Randoms& );

     ~Randoms();

     /**
      * Reset (reseed) random number generator
      */
     void reset();
     void reset ( long unsigned int seed );

     double getUniform ( double a, double b );
     double getGauss ( double sigma );
     double getMittagLeffler ( double beta ); // w publikacji fulgera, par. beta nazywa sie alpha,  par. gamma = 1.0


     // 0 < alpha <= 2
     // -1 <= beta <= 1
     // beta = 1 => "totaly skewed levy distr'
     // alpha = 2 => gauss with sigma = sqrt(2) * sigma_parameter
     double getLevyStable ( double alpha,double beta, double sigma, double mu );

     // get skew levy
     // checked for infinite value
     double getSkewLevy ( double alpha );

//   double getGslSkewLevy(double alpha, double beta,double sigma);
//
//   double getLevy(double alpha);
     double getLevy2 ( double mu );


     /**
      * Returns a alpha stable random 2D vector
      * REMEMBER TO MANUALY REMOVE THE RETURNED ARRAY
      */
     double * getAlphaStableVector ( double alpha, double sigma );


     // 2d symmetric gaussian distribution
     //REMEMBER TO MANUALY REMOVE THE RETURNED ARRAY
     double * getGaussVector ( double sigma );

     /**
      * get 2D alpha stable dicrete vector
      * Nolan method, with s_i points:
      * (1,0),(-1,0),(0,1),(0,-1)
      * REMEMBER TO MANUALY REMOVE THE RETURNED ARRAY
      */
     double * getDicreteAlphaStableVector ( double alpha, double sigma );
     
     
      /**
      * Returns a alpha stable random 2D vector
      * REMEMBER TO MANUALY REMOVE THE RETURNED ARRAY
      */
     double * getAlphaStableVector3d ( double alpha, double sigma );


     // 2d symmetric gaussian distribution
     //REMEMBER TO MANUALY REMOVE THE RETURNED ARRAY
     double * getGaussVector3d ( double sigma );
     
     
     
         // losowanie z rozkladu
    // f(t) = lamba * e^(-lambda t)
    double getSpecialDist1(double lambda);
};

#endif
