#include "Randoms.hh"



Randoms:: Randoms()
{
     cout << "creating Randoms " <<endl;
     //srand(time(0));
     this->init();
}

Randoms:: ~Randoms()
{
     cout << "destroying Randoms...";
     this->destroy();
//     cout << "ok" <<endl;
}

void Randoms:: init()
{
     this->generator = gsl_rng_alloc ( gsl_rng_mt19937 ); //gsl_rng_ranlux
     //this->reset();
// gsl_rng_set (generator, time(0) );

}

void Randoms::reset ( long unsigned int newseed )
{
//  cout << "Seeding random number generator with '" << newseed << "'"<<endl;
     gsl_rng_set ( generator, newseed );
}


void Randoms:: reset()
{
//     cout << "reset()"<<endl;
     srand ( time ( 0 ) );
     reset ( rand() );

     //gsl_rng_set (generator, r );
}

void Randoms:: destroy()
{
     gsl_rng_free ( this->generator );
}


// Randoms* Randoms::getInstance()
//   {
//   //Inicjalizacja statycznego obiektu.
//   //Obiekt zostanie utworzony przy pierwszym wywołaniu tej metody
//   //i tylko wtedy nastąpi inicjalizacja przy pomocy konstruktora.
//   //Każde następne wywołanie jedynie zwróci referencję tego obiektu.
//   static Randoms instance;
//   return &instance;
//   }


double Randoms:: getUniform ( double a, double b )
{
     return gsl_ran_flat ( this->generator, a, b );
}


double Randoms:: getGauss ( double sigma )
{
     double xi = gsl_ran_gaussian ( this->generator, sigma );
     return xi ;
}


double Randoms:: getLevyStable ( double alpha,double beta, double sigma, double mu )
{
     double xi = mu + gsl_ran_levy_skew ( this->generator, sigma, alpha, beta );

     return xi;
}



double Randoms:: getSkewLevy ( double alpha )
{
     int maxTries = 10;
     double ret = 0.0;
     int cnt=1;
     do {
          ret = this->m_drawSkewLevy ( alpha );
          cnt++;

          if ( ret == numeric_limits<double>::infinity() ) {
               cerr << "Randoms:: WARNING: getSkewLevy() returned infinity, drawing again..."<<endl;
          }

     } while ( ret == numeric_limits<double>::infinity() && cnt < maxTries );

     if ( cnt==maxTries ) {
          cerr << "Randoms:: WARNING: getSkewLevy() returned infinity after " << cnt << " tries. returning 0 instead. "<<endl;
          ret = 0.0;
     }

     return ret;
}


double Randoms::  m_drawSkewLevy ( double alpha )
{
     double v = this->getUniform ( -M_PI_2,M_PI_2 );
     double w = gsl_ran_exponential ( this->generator, 1.0 );

     double c_1 = 1.0; //pow( cos( M_PI_2 * alpha), -1.0/alpha );
     double c_2 = M_PI_2;

     double f1 = sin ( alpha* ( v + c_2 ) ) /pow ( cos ( v ),1.0/alpha );
     double f2 = pow ( ( cos ( v - alpha* ( v+c_2 ) ) /w ), ( 1-alpha ) /alpha );

     double xi = c_1 * f1 * f2;
//   this->gslSkewLevyHist->Fill(xi);
     return xi;
}



double Randoms:: getLevy2 ( double mu )
{
// magdziarz/weron
     double v = this->getUniform ( -M_PI_2,M_PI_2 );
     double w = gsl_ran_exponential ( this->generator, 1.0 );

     double f1 = sin ( mu * v ) /pow ( cos ( v ), 1.0/mu );
     double f2 = pow ( ( cos ( v - mu*v ) /w ), ( 1.0-mu ) /mu );

     double xi = f1 * f2;
     return xi;
}

double Randoms:: getMittagLeffler ( double beta )
{
     double u = this->getUniform ( 0,1.0 );
     double v = this->getUniform ( 0,1.0 );
     double m1 = sin ( beta * M_PI ) /tan ( beta*M_PI*v );
     double m2 = cos ( beta * M_PI );

     double m = pow ( ( m1 - m2 ), ( 1.0/beta ) );
     double tau = - log ( u ) * m;

//   cout << tau<<endl;
//   this->gslSkewLevyHist->Fill(tau);

     return tau;
}


double* Randoms::getAlphaStableVector ( double alpha , double sigma )
{
     if(alpha==2.0) return this->getGaussVector(sigma);
  
     double * v = new double[2];

     // to generate alpha-stable random vector

//   double ONE_OVER_SQRT2 = 0.70710678118;

     double s = exp ( log ( cos ( M_PI*alpha/4.0 ) ) *2.0/alpha );

     //first, generate vector of independent gaussian randoms
     // then generate 1 alpha stable variable, totaly skewed to right (beta=1)
     double S = 0.0;
     do {
          S = this->getLevyStable ( alpha/2.0 , 1.0, s , 0.0 ) ;
          //if(S<0.0)   cout << "S="<<S<<endl;
     } while ( S<0.0 );

     double A = sqrt ( S );


     v[0] = A * this->getGauss ( sigma );
     v[1] = A * this->getGauss ( sigma );


     return v;
}

double* Randoms::getAlphaStableVector3d ( double alpha , double sigma )
{
     if(alpha==2.0) return this->getGaussVector3d(sigma);
  
     double * v = new double[3];

     // to generate alpha-stable random vector

//   double ONE_OVER_SQRT2 = 0.70710678118;

     double s = exp ( log ( cos ( M_PI*alpha/4.0 ) ) *2.0/alpha );

     //first, generate vector of independent gaussian randoms
     // then generate 1 alpha stable variable, totaly skewed to right (beta=1)
     double S = 0.0;
     do {
          S = this->getLevyStable ( alpha/2.0 , 1.0, s , 0.0 ) ;
          //if(S<0.0)   cout << "S="<<S<<endl;
     } while ( S<0.0 );

     double A = sqrt ( S );


     v[0] = A * this->getGauss ( sigma );
     v[1] = A * this->getGauss ( sigma );
     v[2] = A * this->getGauss ( sigma );

     return v;
}

double* Randoms::getGaussVector3d ( double sigma )
{
     double * v = new double[3];

     v[0] =  this->getGauss ( sigma );
     v[1] =  this->getGauss ( sigma );
     v[2] =  this->getGauss ( sigma );

     return v;
}




double* Randoms::getGaussVector ( double sigma )
{
     double * v = new double[2];

     // to generate alpha-stable random vector

//   double ONE_OVER_SQRT2 = 0.70710678118;

//      double sigma =  cos ( M_PI/2.0 )  ;

     //first, generate vector of independent gaussian randoms
     // then generate 1 alpha stable variable, totaly skewed to right (beta=1)
//      double S = 0.0;
//      do {
//           S = this->getLevyStable ( 2.0 , 1.0, sigma , 0.0 ) ;
//           //if(S<0.0)   cout << "S="<<S<<endl;
//      } while ( S<0.0 );
//
//      double A = sqrt ( S );

     v[0] =  this->getGauss ( sigma );
     v[1] =  this->getGauss ( sigma );


     return v;
}


double* Randoms::getDicreteAlphaStableVector ( double alpha, double sigma )
{
     //norlan-muiltivariate.pdf

     double * v = new double[2];

     //double sigma = exp ( log ( cos ( M_PI*alpha/4.0 ) ) *2.0/alpha );


     double Z1 =  this->getLevyStable ( alpha , 1.0, sigma , 0.0 ) ;
     double Z2 =  this->getLevyStable ( alpha , 1.0, sigma , 0.0 ) ;
     double Z3 =  this->getLevyStable ( alpha , 1.0, sigma , 0.0 ) ;
     double Z4 =  this->getLevyStable ( alpha , 1.0, sigma , 0.0 ) ;

     // wagi: gamma_i = 1

     // Z1*[1,0] + Z2*[-1,0] + Z3*[0,1] + Z4*[0,-1]

     v[0] = Z1 - Z2;
     v[1] = Z3 - Z4;


     return v;
}


double Randoms:: getSpecialDist1(double lambda)
{
    if(lambda==0.0) {
      cerr << "dzielenie przez 0!" <<endl;
      throw -1;
    }
    double u = this->getUniform(0,1.0);
    
    double x =(-1.0/lambda)  * log( u );
  
    return x;
}

