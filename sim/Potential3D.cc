#include "Potential3D.hh"





Potential3D:: Potential3D(Randoms * rand, double upToDownRate, double downToUpRate, double topValue, double bottomValue) :  timeOfNextSwitch(0.0) {
    this->top_value = topValue;
    this->bottom_value = bottomValue;
    this->gamma_up = downToUpRate;
    this->gamma_down = upToDownRate;

    this->randoms = rand;
//     cout << "creating potential"<<endl;
    this->init();
}



Potential3D:: ~Potential3D() {

    this->cleanUp();
}



void Potential3D::init()
{
//     cout << " draw initial barrier state"<<endl;

    double f = randoms->getUniform(0.0, 1.0);
    double gamma = 0.0;
// 	cout << "f= " << f <<endl;
    if (f <=0.5)
    {
        this->state = barrier_state::up_state;
        gamma = gamma_down;
    }
    else
    {
        this->state = barrier_state::down_state;
        gamma = gamma_up;
    }
    
//     cout << "gamma = " <<gamma <<endl;
    timeOfNextSwitch = randoms->getSpecialDist1(gamma); //getExponential( 1.0/gamma );
    
    
//     cout << " init at t= 0";
//     cout <<"\t\t new state: "<< (( state == barrier_state::up_state) ? " up" : "down ");
//     cout << "\t\t new time to switch = " << timeOfNextSwitch <<endl;
}


void Potential3D::cleanUp()
{
    //if (this->randoms!=NULL) delete this->randoms;
}

void Potential3D::switchBarrier(long double &currentTime)
{
    double gamma = 0.0;
    if ( state == barrier_state::up_state)
    {
        // barrier is in up state, switch do down state
        state = barrier_state:: down_state;
        // get rate of switching to up state
        gamma = gamma_up;
    }
    else
    {
        // is in down state, switch to up state
        state = barrier_state:: up_state;
        // get rate of switching to down state
        gamma = gamma_down;
    }

    
    timeOfNextSwitch = currentTime + randoms->getSpecialDist1(gamma); //getExponential( 1.0/gamma );


//     cout << " barier switch at t=" << currentTime ;
//     cout <<"\t\t new state: "<< (( state == barrier_state::up_state) ? " up" : "down ");
//     cout <<"\t\t new time to switch = " << timeOfNextSwitch <<endl;
}


vec3d Potential3D::getValue(double &x, double &y, double &z, long double &t)
{
    double value = 0.0;

   
    vec3d V;
    /*
     * simple V(x,t) = H r = H sqrt( x^2 + y^2 + r^2)
     * 
     * this actually returns V'(x,t)  
     * so  (d/dx) V(x,y,z, t) =  H  * x/sqrt( x^2 + y^2 + z^2)
     * (d/dy) V(x,y,z, t) =  H  * y/sqrt( x^2 + y^2 + z^2)
     * (d/dz) V(x,y,z, t) =  H  * z/sqrt( x^2 + y^2 + z^2)
     */
    
    double sqr = sqrt( x*x + y*y + z*z );
    
    if( x==0.0 && y==0.0) { 
      V.x = 0.0;
      V.y = 0.0;
      V.z = 0.0;
    }
    else {
    
      if(top_value!=bottom_value) { // if values are the same, no need for this
	if ( t >= timeOfNextSwitch )
	{
	    this->switchBarrier(t);
	}

	if ( this->state == barrier_state:: up_state )
	{
	    value = top_value;
	}
	else // if (this->state = barrier_state :: down_state )
	{
	    value = bottom_value;
	}
      }
      else {
	value = top_value;
      }
      
      V.x = (value * x)/(sqr);
      V.y = (value * y)/(sqr);
      V.z = (value * z)/(sqr);
      
    }
    
//     cout << "get V( x = " << x << ",y = " << y << " t = " << t << ") =  "<<  "("  << V.x  << ","<<V.y << ")"  <<endl;
    return V;
}



