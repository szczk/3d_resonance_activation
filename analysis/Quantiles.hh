#ifndef __QUANTILES_HH__
#define __QUANTILES_HH__

#include <iostream>
#include <vector>
#include "Measure.hh"
#include "../core/Settings.hh"


using namespace std;

/**
 * Miara - linie kwantylowe
 * i szerokosc q9 - q1
 *
 *
 */

class SingleGammaQuantiles;

class Quantiles : public Measure {

private:

    map<double,SingleGammaQuantiles *> * quantiles;

    void init();
    void destroy();

    SingleGammaQuantiles * getSingleGammaQuantiles(double gamma);


public:

    Quantiles();
    ~Quantiles();


    void addValue(double gamma ,double value);
    void save();


};






class SingleGammaQuantiles {
    //trzeba wszystkie wartosci zebrac
    vector<double> * values;

public:
    SingleGammaQuantiles();
    ~SingleGammaQuantiles();
    void addValue(double v);
    void getQuantiles(double *array, int size);

};

#endif
