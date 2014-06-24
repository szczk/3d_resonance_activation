#ifndef __DICHOTOMIC_ANALYSIS_HH__
#define __DICHOTOMIC_ANALYSIS_HH__

#include <iostream>
#include <vector>
#include "Settings.hh"

#include "Measure.hh"
#include "AverageTimeMeasure.hh"
#include "Quantiles.hh"
#include "ExtremeStatsMeasure.hh"


using namespace std;

/**
 *
 *
 */
class DichotomicAnalysis {

private:
     vector<Measure*> * measures; // wektor miar

     void init();
     void destroy();

public:

     DichotomicAnalysis();
     ~DichotomicAnalysis();

     double getAverage ( double gamma ) {
          AverageTimeMeasure* avt = ( AverageTimeMeasure* ) this->measures->at ( 0 );
          return avt->getAverage ( gamma );
     }


     void addValue ( double gamma, double value );

     void save();
};


#endif
