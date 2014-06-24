#ifndef __SIMULATION_HH__
#define __SIMULATION_HH__

#include "../core/Randoms.hh"
#include "../core/Settings.hh"
#include "Potential3D.hh"
// #include "../main.hh"
// #include <map>
// #include <gsl/gsl_histogram2d.h>

using namespace std;

struct  point3d {
  
     double x;
     double y;
     double z;

     point3d ( double x=0.0, double y=0.0, double z=0.0 )  : x ( x ), y ( y ), z(z) {
     }

     point3d operator+ ( const point3d& a ) const {
          return point3d ( a.x+x, a.y+y , a.z+z);
     }
};



class Simulation {


private:
     Settings * settings;
     Randoms * rand;
     Potential3D * potential;


     double x0 ;
     double y0 ;
     double z0 ;
     double r ;
     double r_squared;


     double gamma;
      
    
     bool verbose;


     /**
      * Initialize anything that needs to be initialized
      */
     void init();

     /**
      *  Close anything that needs to be closed.
      */
     void destroy();


     /**
      * Reset all and prepare for new simulations
      */
     void reset();

     /**
      * if the given landing point3d is still in circle
      */
     bool inCircle ( point3d& );


     /**
      *  vectors dot product
      */
     double dotProduct ( const vec3d & v1, const vec3d & v2 );

     /**
      * get distance between point3ds
      */

     double getDistance ( const point3d & p1 ,const point3d & p2 );

     /**
      * normalize vector
      */
     void norm ( vec3d & v );



public:
     Simulation ( Settings * );
     ~Simulation();

     void setGamma(double g) { if(g>0.0) this->gamma = g; }
     void setRadius ( double r ) {
          if ( r>0.0 )  {
            this->r = r;
            this->r_squared = r*r;
          }
          
     }

     /**
      * symulacja az opusci okrag i zwraca t jaki uplynal
      */
     double run ( );


     void setVerbose ( bool v ) {
          this->verbose = v;
     }


};

#endif
