
#ifndef __MEASURE__
#define __MEASURE__

using namespace std;


class Measure {

protected:
   const char * extended_data_header;

public:

    Measure();
    virtual ~Measure() = 0;

    virtual void addValue(double gamma, double value) = 0;

    virtual void save() {} 
    
    
//     // wartosc f(gamma=-infinity)
//     virtual double getValueForMinimalGamma() = 0;
//     
//      // wartosc f(gamma=+infinity)
//     virtual double getValueForMaximalGamma() = 0;
//     
//     // wartosc minimalna charakterystyki f(gamma)
//     virtual double getMinimalValue() = 0;
//     
//     // wartosc gamma dla ktorej charakterystyka ma minimum
//     virtual double getMinimalValueGamma() = 0;
//     
    
};

#endif
