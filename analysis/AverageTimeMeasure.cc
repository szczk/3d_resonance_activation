#include "AverageTimeMeasure.hh"


AverageTimeMeasure:: AverageTimeMeasure() : averages ( nullptr )
{
     clog << "create AverageTimeMeasure "<<endl;
     this->init();
}




AverageTimeMeasure:: ~AverageTimeMeasure()
{
     clog << "destroy AverageTimeMeasure "<<endl;
     this->destroy();
}


void AverageTimeMeasure:: init()
{
     this->averages = new map<double,SingleGammaAverageTime *>();

}


void AverageTimeMeasure:: destroy()
{
     if ( this->averages!=nullptr ) {

          map<double,SingleGammaAverageTime *>::iterator it;

          for ( it = this->averages->begin(); it != this->averages->end(); ++it ) {
               SingleGammaAverageTime * m = ( *it ).second;
               delete m;
          }

          delete this->averages;

     }
}




SingleGammaAverageTime * AverageTimeMeasure:: getSingleGammaAverage ( double gamma )
{

     SingleGammaAverageTime * sgat = nullptr;

     if ( this->averages->count ( gamma ) > 0 ) {
          sgat = this->averages->at ( gamma );
     } else {
          sgat = new SingleGammaAverageTime();
          this->averages->insert ( pair<double, SingleGammaAverageTime*> ( gamma, sgat ) ) ;
     }

     return sgat;
}



void AverageTimeMeasure:: addValue ( double gamma, double value )
{
     this->getSingleGammaAverage ( gamma )->addValue ( value );
}

double AverageTimeMeasure:: getAverage ( double gamma )
{
     return this->getSingleGammaAverage ( gamma )->getAverage();
}

void AverageTimeMeasure:: save()
{
     clog << "Saving average time " << endl;

     Settings& settings = Settings:: getInstance ( );

     double eplus = settings.get ( "eplus" );
     double eminus = settings.get ( "eminus" );
     double alpha = settings.getJumpsParameter();
     double beta = settings.getWaitingTimesParameter();
     double T = settings.get ( "T" );
     double L = 1.0; //settings.get ( "L" );


     char resultsFilename[200];
     sprintf ( resultsFilename,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_results.txt",settings.getStoragePath(), alpha,eplus,eminus );
     ofstream results ( resultsFilename );
     sprintf ( resultsFilename,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_results.txt", alpha,eplus,eminus );

     results << "#log10( (gamma)/T )\tlog10( (MFPT*T) )\terror\n";

     map<double,SingleGammaAverageTime *>::iterator it;

     double smallestMFPT = 100.0;
     double biggestMFPT = -100.0;
     double positionOfSmallestMFPT = 0.0; // for plot arrow

     // wartosci skrajne
     double f_minus_infinity = 0.0;
     double f_plus_infinity = 0.0;

     double f_w_minimum = 0.0;
     double gamma_for_f_minimum = 0.0;


     for ( it = this->averages->begin(); it != this->averages->end(); ++it ) {

          double gamma = ( *it ).first;
          double averageTime = ( ( *it ).second )->getAverage();
          double averageTime_err = ( ( *it ).second )->getError();

          // do extended data
          if ( it==this->averages->begin() )  {
               f_minus_infinity = averageTime;
          }
          f_plus_infinity = averageTime;


          double X = log10 ( ( gamma * L*L ) /T );
          double Y = log10 ( ( averageTime * T ) / ( L*L ) );
          double Y_err = abs ( averageTime_err/ ( Y * log ( 10.0 ) ) ) ;


          results << X << "\t" << Y <<"\t"<< Y_err <<endl;

          if ( smallestMFPT > Y ) {
               smallestMFPT = Y;
               positionOfSmallestMFPT = X;

               f_w_minimum = averageTime;
               gamma_for_f_minimum = gamma;
          }
          if ( biggestMFPT < Y ) biggestMFPT = Y;

     }
     results.close();
     char resultsPlotscriptfile[200];
     sprintf ( resultsPlotscriptfile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_plot.gnu",settings.getStoragePath(), alpha,eplus,eminus );


     ofstream resultsPlot ( resultsPlotscriptfile );


     char resultsPlotfile[200];
     sprintf ( resultsPlotfile,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_result.eps",alpha,eplus,eminus );


     resultsPlot << "reset\n";
//      resultsPlot << "set terminal pngcairo enhanced size 1200,700;\n";
     resultsPlot << "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";
     resultsPlot << "set out \""<< resultsPlotfile << "\";\n";




     //strzalka
//      resultsPlot << "set style arrow 1 head filled size screen 0.03,5,1 ls 1\n";
//      resultsPlot << "set arrow from "<<positionOfSmallestMFPT<<","<<smallestMFPT + abs ( smallestMFPT*0.2 ) <<" to "<<positionOfSmallestMFPT<<","<<smallestMFPT + abs ( smallestMFPT*0.05 ) <<" as 1 \n";
//
//      double labelX= positionOfSmallestMFPT - abs ( positionOfSmallestMFPT*0.05 );
//      double labelY= smallestMFPT + abs ( smallestMFPT*0.26 );
//      resultsPlot << "set label 'log_{10} ({/Symbol g})="<< positionOfSmallestMFPT <<"' at "<<labelX<<","<< labelY <<" left\n";
//

     resultsPlot << "set ylabel \"log_{10} ( <{/Symbol t}> )\"\n";
     resultsPlot << "set xlabel \"log_{10} ({/Symbol g})\"\n";
//      resultsPlot << "set title \"E_{+}= "<< eplus <<"T, E_{-}= "<< eminus <<"T,  {/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";
//      resultsPlot << "set title \"{/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";
     resultsPlot << "plot '"<< resultsFilename <<"' using 1:2 with linespoints notitle lc rgbcolor '#000000';\n";



     resultsPlot.close();



     // save extended_data
     char extendedDataFile[200];
     sprintf ( extendedDataFile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extdata_mfpt.txt",settings.getStoragePath(), alpha,eplus,eminus );

     ofstream extended ( extendedDataFile );

     extended << extended_data_header << endl;

     extended << alpha << "\t"<< beta;

     // wartosc f(gamma=-infinity)
     extended << "\t" << f_minus_infinity;

     // wartosc f(gamma=+infinity)
     extended << "\t" << f_plus_infinity;

     // wartosc minimalna charakterystyki f(gamma)
     extended << "\t" << f_w_minimum;

     // wartosc gamma dla ktorej charakterystyka ma minimum
     extended << "\t" << gamma_for_f_minimum;

     extended.close();
}
