#ifndef __AVERAGE_TIME_HH__
#define __AVERAGE_TIME_HH__

#include <iostream>
#include <map>
#include "Measure.hh"
#include "../core/Settings.hh"
#include <cmath>
#include "RunningStat.hh"

using namespace std;

class SingleGammaAverageTime;

/**
 * Miara - sredni czas ucieczki (MFPT)
 *
 */
class AverageTimeMeasure : public Measure {

private:

    map<double,SingleGammaAverageTime *> * averages;

    void init();
    void destroy();

    SingleGammaAverageTime * getSingleGammaAverage(double gamma);

public:

    AverageTimeMeasure();
    ~AverageTimeMeasure();


    void addValue(double gamma ,double value);
    double getAverage(double gamma);
    void save();
};





class SingleGammaAverageTime {

private:
//     double average;
//     unsigned int count;
    RunningStat * stat;

public:
    SingleGammaAverageTime() : stat(nullptr) { stat = new RunningStat(); }
    ~SingleGammaAverageTime() { delete stat; } 

    void addValue(double v) {
        //moving average
//         average = average + ((v - average)/(count + 1.0));
//         ++count;
// 	  clog << average << endl;
      this->stat->Push(v);
    }

    double getAverage() {
//         return average;
	return this->stat->Mean();
    }
    double getError() { 
	return this->stat->StandardDeviationOfMean();
    }
    
    unsigned int getCount() {
//         return count;
        return this->stat->NumDataValues();
    }
    
    

};

#endif
