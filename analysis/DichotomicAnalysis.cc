#include "DichotomicAnalysis.hh"




DichotomicAnalysis:: DichotomicAnalysis() : measures ( nullptr )
{
     clog<<"creating DichotomicAnalysis"<<endl;
     this->init();
}


DichotomicAnalysis:: ~DichotomicAnalysis()
{

     clog<<"destroy DichotomicAnalysis"<<endl;
     this->destroy();

}

void DichotomicAnalysis:: init()
{

     this->measures = new vector<Measure*>();

     this->measures->push_back ( new AverageTimeMeasure() );
     this->measures->push_back ( new Quantiles() );
     //this->measures->push_back ( new ExtremeStatsMeasure ( 10 ) );

}

void DichotomicAnalysis:: destroy()
{

     if ( this->measures!=nullptr ) {
          vector<Measure*>::iterator it;

          for ( it = this->measures->begin(); it != this->measures->end(); ++it ) {
               Measure * m = *it;
               delete m;
          }

          delete this->measures;
     }

}

     //set terminal post eps size 12,7 enhanced color font 'Helvetica,20' linewidth 2;
     // beta => mu !!
     
     

void DichotomicAnalysis:: addValue ( double gamma, double value )
{

     vector<Measure*>::iterator it;
     for ( it = this->measures->begin(); it != this->measures->end(); ++it ) {
          Measure * m = *it;
          m->addValue ( gamma,value );
     }


}

void DichotomicAnalysis:: save()
{

     vector<Measure*>::iterator it;
     for ( it = this->measures->begin(); it != this->measures->end(); ++it ) {
          Measure * m = *it;
          m->save();
     }

}

