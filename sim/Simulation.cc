#include "Simulation.hh"

inline ostream& operator<< ( ostream &out, point3d &cPoint )
{
     // Since operator<< is a friend of the Point class, we can access
     // Point's members directly.
     out << "(" << cPoint.x << ", " <<  cPoint.y << "," <<  cPoint.z << ")";
     return out;
}



Simulation::Simulation ( Settings * set )  : rand ( nullptr ), potential(nullptr),  x0 ( 0.0 ),y0 ( 0.0 ), z0 ( 0.0 ),  r ( 1.0 ) , verbose ( false ) 
{

     this->settings = set;

     this->init();
}


Simulation::~Simulation()
{
     this->destroy();
}



void Simulation::destroy()
{
     cout << "destroying simulation"<<endl;

     if ( rand!=nullptr ) delete rand;
     if ( potential!=nullptr ) delete potential;

}


void Simulation::init()
{
     cout << "initializing simulation"<<endl;
     this->rand = new Randoms();
     
     
    //double beta = settings.getWaitingTimesParameter();
   
     double r = settings->get("radius");
     
     this->r_squared = r*r;
     
     
     //this->reset();
}


void Simulation::reset()
{
     //this->rand->reset();
      
     if ( potential!=nullptr ) delete potential;

     double eplus = this->settings->get("eplus");
     double eminus = this->settings->get("eminus");
     double T = this->settings->get("T");

     this->potential = new Potential3D(this->rand, this->gamma, this->gamma, eplus * T, eminus*T );
 
//      cout << this->potential->toString() <<endl;

}




double Simulation::run ()
{

     this->reset();

//      bool dump_gnuplot = false;

     double dt = this->settings->getDt();
     long double t = this->settings->getStartTime(); //t0


     double alpha = this->settings->getJumpsParameter();
     double dL = exp ( log ( dt ) /alpha );

     double sigma = this->settings->getNoiseIntensity();
     int noiseType = settings->get ( "NOISE_TYPE" );

//   double endTime = 10.0;

    
     point3d current_point3d;

     current_point3d.x = this->settings->getX0();
     current_point3d.y = this->settings->getX0();
     current_point3d.z = this->settings->getX0();


     

     
     while ( inCircle ( current_point3d ) ) 
     {


//        cout << "t="<<t<<endl;
          double * v ;


          switch ( noiseType ) {
          case 1:
               v = rand->getAlphaStableVector3d( alpha, sigma  );
               break;
          case 2:
          default:
	     throw -1;
	     //
               v = rand->getDicreteAlphaStableVector ( alpha, sigma );
               break;
          }
     

          // grad V(x,y)
          vec3d potential = ( * ( this->potential ) ) ( current_point3d.x, current_point3d.y,  current_point3d.z, t );

//           cout << "v[0] = " << v[0] << "\tv[1] = " << v[1] <<endl;

          current_point3d.x += -potential.x*dt  + v[0]*dL;
          current_point3d.y += -potential.y*dt  + v[1]*dL;
	  current_point3d.z += -potential.z*dt  + v[2]*dL;
 
          delete[] v;
          t+= dt;
         
     }


     return t;
}


bool Simulation::inCircle ( point3d& landing_point3d )
{
     return ( landing_point3d.x*landing_point3d.x + landing_point3d.y*landing_point3d.y + landing_point3d.z*landing_point3d.z  < r_squared );
}


double Simulation::dotProduct ( const vec3d & v1, const vec3d & v2 )
{
     return v1.x*v2.x + v1.y*v2.y+ v1.z*v2.z;

}


double Simulation::getDistance ( const point3d & p1, const point3d & p2 )
{
     return sqrt ( pow ( p2.x-p1.x,2.0 ) + pow ( p2.y-p1.y,2.0 )  + pow ( p2.z-p1.z,2.0 ) );
}



void Simulation::norm ( vec3d & v )
{
     double d = sqrt ( v.x*v.x + v.y*v.y + v.z * v.z );
     v.x = v.x/d;
     v.y = v.y/d;
     v.z = v.z/d;
}


