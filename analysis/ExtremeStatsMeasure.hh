#ifndef __EXTREME_STATS_MEASURE__
#define __EXTREME_STATS_MEASURE__


#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Measure.hh"
#include "../core/Settings.hh"

using namespace std;



class SingleGammaExtremeStats;
class SingleGammaExtremeStatsElement;

class ExtremeStatsMeasure : public Measure {
  
  
 private:
	int groupCount; // ile w grupie
    map<double,SingleGammaExtremeStats *> * extremeStats;

    void init();
    void destroy();

    SingleGammaExtremeStats * getSingleGammaExtremeStats(double gamma);


public:

    ExtremeStatsMeasure(int groupCount);
    ~ExtremeStatsMeasure();


    void addValue(double gamma ,double value);
    void save(); 
	void setGroupCount(int gc) { if(gc>0) this->groupCount = gc;}

};




///=======================================================

class SingleGammaExtremeStats {
  
private:
  
  /* ile w grupie */
  int groupCount;
  
  vector<SingleGammaExtremeStatsElement*> * elements;
  
  void init();
  void destroy();	
  
  // :)
  double getLowestMax(); //global min(max) 
  double getHighestMax(); // global max(max)
  double getLowestMin(); //global min(min)  
  double getHighestMin(); //global max(min)
  
  
  double gamma;
  //ogolnie wszystkie wartosci - zeby zrobic dystrybuante
  vector<double> * values;

public:
  SingleGammaExtremeStats(int count);
  ~SingleGammaExtremeStats();
  
  void setGamma(double g) { this->gamma = g; }
  void setCount(int c) { if(c>0) this->groupCount = c; }
  
  void fill(double value);
  void saveRaw(const char * datafileName);
/*  
  double saveMinHistogram(const char * datafileName);
  double saveMaxHistogram(const char * datafileName);*/


  
  void saveMinimumsEDF();
  void saveMaximumsEDF();
  
  // array to tablica 9 elementowac ggdzie beda zapisane wartosci
  // narazie size musi byc == 9, zwracamy kwantyle q1 - q9
  void getMinumumsQuantiles(double *array, int size);
  void getMaximumsQuantiles(double *array, int size);
  
};


///==================================================================

class SingleGammaExtremeStatsElement {
private:
  unsigned int maxCount;
  double minElement;
  double maxElement;
  vector<double> * values;

public:
  SingleGammaExtremeStatsElement(int count) ;
  ~SingleGammaExtremeStatsElement();
  
  void fill(double v);
  double getMin() {return this->minElement; }
  double getMax() {return this->maxElement; }
  
  bool full();
  
};


#endif 
