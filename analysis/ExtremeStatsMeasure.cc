
#include "ExtremeStatsMeasure.hh"



ExtremeStatsMeasure:: ExtremeStatsMeasure ( int c )
{

     this->setGroupCount ( c );
     this->init();
}

ExtremeStatsMeasure:: ~ExtremeStatsMeasure()
{
     this->destroy();
}


void ExtremeStatsMeasure:: init()
{
     this->extremeStats = new map<double,SingleGammaExtremeStats *>();
}


void ExtremeStatsMeasure:: destroy()
{
     if ( this->extremeStats!=nullptr ) {

          map<double,SingleGammaExtremeStats *>::iterator it;

          for ( it = this->extremeStats->begin(); it != this->extremeStats->end(); ++it ) {
               SingleGammaExtremeStats * m = ( *it ).second;
               delete m;
          }

          delete this->extremeStats;

     }
}


SingleGammaExtremeStats * ExtremeStatsMeasure:: getSingleGammaExtremeStats ( double gamma )
{

     SingleGammaExtremeStats * sgqt = nullptr;

     if ( this->extremeStats->count ( gamma ) > 0 ) {
          sgqt = this->extremeStats->at ( gamma );
     } else {
          sgqt = new SingleGammaExtremeStats ( this->groupCount );
          sgqt->setGamma ( gamma );
          this->extremeStats->insert ( pair<double, SingleGammaExtremeStats*> ( gamma, sgqt ) ) ;
     }

     return sgqt;
}


void ExtremeStatsMeasure:: addValue ( double gamma, double value )
{
     this->getSingleGammaExtremeStats ( gamma )->fill ( value );
}


void ExtremeStatsMeasure:: save()
{

     clog << "Saving extreme stats " << endl;

     map<double,SingleGammaExtremeStats *>::iterator it;

     for ( it = this->extremeStats->begin(); it != this->extremeStats->end(); ++it ) {
          SingleGammaExtremeStats * m = ( *it ).second;
          m->saveMinimumsEDF();
          m->saveMaximumsEDF();
     }




     clog << "Saving extreme stats quantiles " << endl;

     Settings& settings = Settings:: getInstance ( );

     double eplus = settings.get ( "eplus" );
     double eminus = settings.get ( "eminus" );
     double alpha = settings.getJumpsParameter();
     double beta = settings.getWaitingTimesParameter();

     char quantiles_min_filename[200];
     char quantiles_max_filename[200];



     sprintf ( quantiles_min_filename,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_min.txt",settings.getStoragePath(), alpha,eplus,eminus );
     sprintf ( quantiles_max_filename,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_max.txt",settings.getStoragePath(), alpha,eplus,eminus );

     ofstream quantiles_min_file ( quantiles_min_filename,ios_base::out );
     ofstream quantiles_max_file ( quantiles_max_filename,ios_base::out );

     //to samo bez sciezki, do uzycia w skrypcie gnuplota
     sprintf ( quantiles_min_filename,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_min.txt",alpha,eplus,eminus );
     sprintf ( quantiles_max_filename,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_max.txt",alpha,eplus,eminus );


     quantiles_min_file << "#log10(gamma)\tq1\tq2\tq3\tq4\tq5\tq6\tq7\tq8\tq9\t(q9-q1)\tlog10(q9-q1)\n";
     quantiles_max_file << "#log10(gamma)\tq1\tq2\tq3\tq4\tq5\tq6\tq7\tq8\tq9\t(q9-q1)\tlog10(q9-q1)\n";


     // zapisywanie kwantyli
     double quantiles_mins[9];
     double quantiles_maxs[9];

     double mins_smallestWidth = 100.0;
     double mins_positionOfSmallestWidth = 0.0;
     double mins_smallestMedian = 100.0;
     double mins_positionOfSmallestMedian = 0.0;

     double maxs_smallestWidth = 100.0;
     double maxs_positionOfSmallestWidth = 0.0;
     double maxs_smallestMedian = 100.0;
     double maxs_positionOfSmallestMedian = 0.0;


     // wartosci skrajne, extended data
     double maxFPT_f_minus_infinity = 0.0;
     double maxFPT_f_plus_infinity = 0.0;
     double maxFPT_f_w_minimum = 0.0;
     double maxFPT_gamma_for_f_minimum = 0.0;

     double maxFPT_q9q1_f_minus_infinity = 0.0;//
     double maxFPT_q9q1_f_plus_infinity = 0.0; //
     double maxFPT_q9q1_f_w_minimum = 0.0;   //
     double maxFPT_q9q1_gamma_for_f_minimum = 0.0;   //

     double maxFPT_median_f_minus_infinity = 0.0;//
     double maxFPT_median_f_plus_infinity = 0.0;  //
     double maxFPT_median_f_w_minimum = 0.0;//
     double maxFPT_median_gamma_for_f_minimum = 0.0;     //



     double minFPT_f_minus_infinity = 0.0;
     double minFPT_f_plus_infinity = 0.0;
     double minFPT_f_w_minimum = 0.0;
     double minFPT_gamma_for_f_minimum = 0.0;

     double minFPT_q9q1_f_minus_infinity = 0.0;//
     double minFPT_q9q1_f_plus_infinity = 0.0;//
     double minFPT_q9q1_f_w_minimum = 0.0;   //
     double minFPT_q9q1_gamma_for_f_minimum = 0.0;//

     double minFPT_median_f_minus_infinity = 0.0;//
     double minFPT_median_f_plus_infinity = 0.0;//
     double minFPT_median_f_w_minimum = 0.0; //
     double minFPT_median_gamma_for_f_minimum = 0.0;  //




     //map<double,SingleGammaExtremeStats *>::iterator it;
cout << "mark1"<<endl;
     //iteracja po gamma
     for ( it = this->extremeStats->begin(); it != this->extremeStats->end(); ++it ) {
          double gamma = ( *it ).first;
          SingleGammaExtremeStats * sgex = ( *it ).second;

          quantiles_min_file << log10 ( gamma );
          quantiles_max_file << log10 ( gamma );
cout << "mark12"<<endl;
          sgex->getMinumumsQuantiles ( quantiles_mins,9 );
cout << "mark13"<<endl;
          sgex->getMaximumsQuantiles ( quantiles_maxs,9 );
cout << "mark14"<<endl;

          for ( int i=0; i<9; i++ ) {
               quantiles_min_file <<"\t" << quantiles_mins[i];
               quantiles_max_file <<"\t" << quantiles_maxs[i];
          }

          // 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9
          // mediana - kwantyl rzedu 0.5 czyli quantiles[4]
          //median = quantiles[4];

          if ( mins_smallestMedian >= quantiles_mins[4] ) {
               mins_smallestMedian = quantiles_mins[4];
               mins_positionOfSmallestMedian = log10 ( gamma );

               minFPT_median_f_w_minimum = quantiles_mins[4];
               minFPT_median_gamma_for_f_minimum = gamma;
          }

          if ( maxs_smallestMedian >= quantiles_maxs[4] ) {
               maxs_smallestMedian = quantiles_maxs[4];
               maxs_positionOfSmallestMedian = log10 ( gamma );

               maxFPT_median_f_w_minimum = quantiles_maxs[4];
               maxFPT_median_gamma_for_f_minimum = gamma;
          }


          double qWidth_min = ( quantiles_mins[8]-quantiles_mins[0] );
          if ( mins_smallestWidth >= qWidth_min ) {
               mins_smallestWidth = qWidth_min;
               mins_positionOfSmallestWidth = log10 ( gamma );

               minFPT_q9q1_f_w_minimum = qWidth_min;
               minFPT_q9q1_gamma_for_f_minimum = gamma;
          }

          double qWidth_max = ( quantiles_maxs[8]-quantiles_maxs[0] );
          if ( maxs_smallestWidth >= qWidth_max ) {
               maxs_smallestWidth = qWidth_max;
               maxs_positionOfSmallestWidth = log10 ( gamma );

               maxFPT_q9q1_f_w_minimum = qWidth_max;
               maxFPT_q9q1_gamma_for_f_minimum = gamma;
          }

          quantiles_min_file << "\t" << qWidth_min <<"\t" << log10 ( qWidth_min ) << "\n";
          quantiles_max_file << "\t" << qWidth_max <<"\t" << log10 ( qWidth_max ) << "\n";

          //wartosci skrajne
          if ( it == this->extremeStats->begin() ) {
               maxFPT_q9q1_f_minus_infinity = qWidth_max;
               maxFPT_median_f_minus_infinity = quantiles_maxs[4];
               minFPT_q9q1_f_minus_infinity = qWidth_min;
               minFPT_median_f_minus_infinity = quantiles_mins[4];
          }

          maxFPT_q9q1_f_plus_infinity = qWidth_max;
          maxFPT_median_f_plus_infinity = quantiles_maxs[4];
          minFPT_q9q1_f_plus_infinity = qWidth_min;
          minFPT_median_f_plus_infinity = quantiles_mins[4];

     }


cout << "mark2"<<endl;
     quantiles_min_file.close();
     quantiles_max_file.close();


     // gnuplot

     char resultsPlotscriptfile[200];
     sprintf ( resultsPlotscriptfile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_plot.gnu",settings.getStoragePath(), alpha,eplus,eminus );
     ofstream gnuplot_file ( resultsPlotscriptfile,ios_base::out );

     char mins_quantile_plotname[200];
     char maxs_quantile_plotname[200];
     sprintf ( mins_quantile_plotname,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_mins_plot_log.eps", alpha,eplus,eminus );
     sprintf ( maxs_quantile_plotname,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_maxs_plot_log.eps", alpha,eplus,eminus );

     gnuplot_file<< "reset\n";
//     gnuplot_file<< "set terminal pngcairo enhanced size 1200,700;\n";
     gnuplot_file<< "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";
     gnuplot_file<< "set output \""<< mins_quantile_plotname << "\"\n";
     gnuplot_file<< "set xlabel \"log_{10}({/Symbol g})\"\n";
     gnuplot_file<< "set ylabel \"log_{10}(q_{9} - q_{1})\"\n";
//     gnuplot_file<< "set title \"F^{min}, {/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";

     //strzalka
//     double arrowX1 = mins_positionOfSmallestWidth;
//     double arrowX2 = arrowX1;
//     double arrowY1 = log10(mins_smallestWidth) + abs( log10(mins_smallestWidth) )*0.8;
//     double arrowY2 = log10(mins_smallestWidth) + abs( log10(mins_smallestWidth) )*0.4;
//     gnuplot_file << "set style arrow 1 head filled size screen 0.02,6,1 ls 1\n";
//     gnuplot_file << "set arrow from "<<arrowX1<<","<<arrowY1<<" to "<<arrowX2<<","<<arrowY2<<" as 1 \n";
//
//     double labelX= arrowX1 - abs(arrowX1)*0.2;
//     double labelY= arrowY1 ;
//
//     gnuplot_file << "set label 'log_{10} ({/Symbol g})="<< mins_positionOfSmallestWidth <<"' at "<<labelX<<","<< labelY <<" left\n";

     gnuplot_file<< "plot '"<<quantiles_min_filename<<"' using 1:12 with lp notitle lc rgbcolor '#000000';\n";


     // mediana
     sprintf ( mins_quantile_plotname,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_mins_plot_median.eps", alpha,eplus,eminus );

     gnuplot_file<< "reset\n";
//     gnuplot_file<< "set terminal pngcairo enhanced size 1200,700;\n";
     gnuplot_file<< "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";
     gnuplot_file<< "set output \""<< mins_quantile_plotname << "\"\n";
     gnuplot_file<< "set xlabel \"log_{10}({/Symbol g})\"\n";
     gnuplot_file<< "set ylabel \"log_{10}(q_{0.5})\"\n";

//     gnuplot_file<< "set title \"F^{min}, {/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";

     //strzalka
//     arrowX1 = mins_positionOfSmallestMedian;
//     arrowX2 = arrowX1;
//     arrowY1 = (mins_smallestMedian) + abs( (mins_smallestMedian) )*0.05;
//     arrowY2 = (mins_smallestMedian) + abs( (mins_smallestMedian) )*0.02;
//     gnuplot_file << "set style arrow 1 head filled size screen 0.02,6,1 ls 1\n";
//     gnuplot_file << "set arrow from "<<arrowX1<<","<<arrowY1<<" to "<<arrowX2<<","<<arrowY2<<" as 1 \n";
//
//     labelX= arrowX1 - abs(arrowX1)*0.2;
//     labelY= arrowY1 ;
//
//     gnuplot_file << "set label 'log_{10} ({/Symbol g})="<< mins_positionOfSmallestMedian <<"' at "<<labelX<<","<< labelY <<" left\n";

     gnuplot_file<< "plot '"<<quantiles_min_filename<<"' using 1:6 with lp notitle lc rgbcolor '#000000';\n";


     // wszystkie kwantyle


     gnuplot_file<< "reset\n";
     sprintf ( mins_quantile_plotname,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_mins_plot.eps", alpha,eplus,eminus );

//     gnuplot_file<< "set terminal pngcairo enhanced size 1200,700;\n";
     gnuplot_file<< "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";
     gnuplot_file<< "set output \""<< mins_quantile_plotname << "\"\n";
     gnuplot_file<< "set xlabel \"log_{10}({/Symbol g})\"\n";
     gnuplot_file<< "set ylabel \"q\"\n";
     gnuplot_file<< "set logscale y\n";
//     gnuplot_file << "set title \"F^{min} {/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";
     gnuplot_file<< "plot '"<<quantiles_min_filename<<"' using 1:2 with lp title \"q_{0.1}\",\\\n";
     gnuplot_file<< "'"<<quantiles_min_filename<<"' using 1:3 with lp title \"q_{0.2}\",\\\n";
     gnuplot_file<< "'"<<quantiles_min_filename<<"' using 1:4 with lp title \"q_{0.3}\",\\\n";
     gnuplot_file<< "'"<<quantiles_min_filename<<"' using 1:5 with lp title \"q_{0.4}\",\\\n";
     gnuplot_file<< "'"<<quantiles_min_filename<<"' using 1:6 with lp title \"q_{0.5}\",\\\n";
     gnuplot_file<< "'"<<quantiles_min_filename<<"' using 1:7 with lp title \"q_{0.6}\",\\\n";
     gnuplot_file<< "'"<<quantiles_min_filename<<"' using 1:8 with lp title \"q_{0.7}\",\\\n";
     gnuplot_file<< "'"<<quantiles_min_filename<<"' using 1:9 with lp title \"q_{0.8}\",\\\n";
     gnuplot_file<< "'"<<quantiles_min_filename<<"' using 1:10 with lp title \"q_{0.9}\";\n";


     // ============ maximas

     gnuplot_file<< "reset\n";
//     gnuplot_file<< "set terminal pngcairo enhanced size 1200,700;\n";
     gnuplot_file<< "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";
     gnuplot_file<< "set output \""<< maxs_quantile_plotname << "\"\n";
     gnuplot_file<< "set xlabel \"log_{10}({/Symbol g})\"\n";
     gnuplot_file<< "set ylabel \"log_{10}(q_{9} - q_{1})\"\n";
//     gnuplot_file<< "set title \"F^{max}, {/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";

     //strzalka
//     arrowX1 = maxs_positionOfSmallestWidth;
//     arrowX2 = arrowX1;
//     arrowY1 = log10(maxs_smallestWidth) + abs( log10(maxs_smallestWidth) )*0.8;
//     arrowY2 = log10(maxs_smallestWidth) + abs( log10(maxs_smallestWidth) )*0.4;
//     gnuplot_file << "set style arrow 1 head filled size screen 0.02,6,1 ls 1\n";
//     gnuplot_file << "set arrow from "<<arrowX1<<","<<arrowY1<<" to "<<arrowX2<<","<<arrowY2<<" as 1 \n";
//
//     labelX= arrowX1 - abs(arrowX1)*0.2;
//     labelY= arrowY1 ;
//
//     gnuplot_file << "set label 'log_{10} ({/Symbol g})="<< maxs_positionOfSmallestWidth <<"' at "<<labelX<<","<< labelY <<" left\n";

     gnuplot_file<< "plot '"<<quantiles_max_filename<<"' using 1:12 with lp notitle lc rgbcolor '#000000';\n";


     // mediana
     sprintf ( maxs_quantile_plotname,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_maxs_plot_median.eps", alpha,eplus,eminus );

     gnuplot_file<< "reset\n";
//     gnuplot_file<< "set terminal pngcairo enhanced size 1200,700;\n";
     gnuplot_file<< "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";
     gnuplot_file<< "set output \""<< maxs_quantile_plotname << "\"\n";
     gnuplot_file<< "set xlabel \"log_{10}({/Symbol g})\"\n";
     gnuplot_file<< "set ylabel \"log_{10}(q_{0.5})\"\n";

//     gnuplot_file<< "set title \"F^{max}, {/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";

     //strzalka
//     arrowX1 = maxs_positionOfSmallestMedian;
//     arrowX2 = arrowX1;
//     arrowY1 = (maxs_smallestMedian) + abs( (maxs_smallestMedian) )*0.05;
//     arrowY2 = (maxs_smallestMedian) + abs( (maxs_smallestMedian) )*0.02;
//     gnuplot_file << "set style arrow 1 head filled size screen 0.02,6,1 ls 1\n";
//     gnuplot_file << "set arrow from "<<arrowX1<<","<<arrowY1<<" to "<<arrowX2<<","<<arrowY2<<" as 1 \n";
//
//     labelX= arrowX1 - abs(arrowX1)*0.2;
//     labelY= arrowY1 ;
//
//     gnuplot_file << "set label 'log_{10} ({/Symbol g})="<< maxs_positionOfSmallestMedian <<"' at "<<labelX<<","<< labelY <<" left\n";

     gnuplot_file<< "plot '"<<quantiles_max_filename<<"' using 1:6 with lp notitle lc rgbcolor '#000000';\n";


     // wszystkie kwantyle


     gnuplot_file<< "reset\n";
     sprintf ( maxs_quantile_plotname,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extreme_stats_quantiles_maxs_plot.eps", alpha,eplus,eminus );

//     gnuplot_file<< "set terminal pngcairo enhanced size 1200,700;\n";
     gnuplot_file<< "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";
     gnuplot_file<< "set output \""<< maxs_quantile_plotname << "\"\n";
     gnuplot_file<< "set xlabel \"log_{10}({/Symbol g})\"\n";
     gnuplot_file<< "set ylabel \"q\"\n";
     gnuplot_file<< "set logscale y\n";
//     gnuplot_file<< "set title \"F^{max}, {/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";
     gnuplot_file<< "plot '"<<quantiles_max_filename<<"' using 1:2 with lp title \"q_{0.1}\",\\\n";
     gnuplot_file<< "'"<<quantiles_max_filename<<"' using 1:3 with lp title \"q_{0.2}\",\\\n";
     gnuplot_file<< "'"<<quantiles_max_filename<<"' using 1:4 with lp title \"q_{0.3}\",\\\n";
     gnuplot_file<< "'"<<quantiles_max_filename<<"' using 1:5 with lp title \"q_{0.4}\",\\\n";
     gnuplot_file<< "'"<<quantiles_max_filename<<"' using 1:6 with lp title \"q_{0.5}\",\\\n";
     gnuplot_file<< "'"<<quantiles_max_filename<<"' using 1:7 with lp title \"q_{0.6}\",\\\n";
     gnuplot_file<< "'"<<quantiles_max_filename<<"' using 1:8 with lp title \"q_{0.7}\",\\\n";
     gnuplot_file<< "'"<<quantiles_max_filename<<"' using 1:9 with lp title \"q_{0.8}\",\\\n";
     gnuplot_file<< "'"<<quantiles_max_filename<<"' using 1:10 with lp title \"q_{0.9}\";\n";



     gnuplot_file.close();

     ///=============================
     // save extended_data
     char extendedDataFile[200];
     sprintf ( extendedDataFile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extdata_minFPT_q9q1.txt",settings.getStoragePath(), alpha,eplus,eminus );
     ofstream extended ( extendedDataFile );
     extended << extended_data_header << endl;
     extended << alpha << "\t"<< beta;
     // wartosc f(gamma=-infinity)
     extended << "\t" << minFPT_q9q1_f_minus_infinity;
     // wartosc f(gamma=+infinity)
     extended << "\t" << minFPT_q9q1_f_plus_infinity;
     // wartosc minimalna charakterystyki f(gamma)
     extended << "\t" << minFPT_q9q1_f_w_minimum;
     // wartosc gamma dla ktorej charakterystyka ma minimum
     extended << "\t" << minFPT_q9q1_gamma_for_f_minimum;
     extended.close();

     sprintf ( extendedDataFile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extdata_minFPT_median.txt",settings.getStoragePath(),alpha,eplus,eminus );
     extended.open ( extendedDataFile );
     extended << extended_data_header << endl;
     extended << alpha << "\t"<< beta;
     // wartosc f(gamma=-infinity)
     extended << "\t" << minFPT_median_f_minus_infinity;
     // wartosc f(gamma=+infinity)
     extended << "\t" << minFPT_median_f_plus_infinity;
     // wartosc minimalna charakterystyki f(gamma)
     extended << "\t" << minFPT_median_f_w_minimum;
     // wartosc gamma dla ktorej charakterystyka ma minimum
     extended << "\t" << minFPT_median_gamma_for_f_minimum;
     extended.close();


     sprintf ( extendedDataFile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extdata_maxFPT_q9q1.txt",settings.getStoragePath(), alpha,eplus,eminus );
     extended.open ( extendedDataFile );
     extended << extended_data_header << endl;
     extended << alpha << "\t"<< beta;
     // wartosc f(gamma=-infinity)
     extended << "\t" << maxFPT_q9q1_f_minus_infinity;
     // wartosc f(gamma=+infinity)
     extended << "\t" << maxFPT_q9q1_f_plus_infinity;
     // wartosc minimalna charakterystyki f(gamma)
     extended << "\t" << maxFPT_q9q1_f_w_minimum;
     // wartosc gamma dla ktorej charakterystyka ma minimum
     extended << "\t" << maxFPT_q9q1_gamma_for_f_minimum;
     extended.close();



     sprintf ( extendedDataFile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extdata_maxFPT_median.txt",settings.getStoragePath(),alpha,eplus,eminus );
     extended.open ( extendedDataFile );
     extended << extended_data_header << endl;
     extended << alpha << "\t"<< beta;
     // wartosc f(gamma=-infinity)
     extended << "\t" << maxFPT_median_f_minus_infinity;
     // wartosc f(gamma=+infinity)
     extended << "\t" << maxFPT_median_f_plus_infinity;
     // wartosc minimalna charakterystyki f(gamma)
     extended << "\t" << maxFPT_median_f_w_minimum;
     // wartosc gamma dla ktorej charakterystyka ma minimum
     extended << "\t" << maxFPT_median_gamma_for_f_minimum;
     extended.close();


cout << "mark3"<<endl;
}


/// =====================================================




SingleGammaExtremeStats:: SingleGammaExtremeStats ( int count ) : groupCount ( 0 )
{
     this->init();
     this->setCount ( count );

}

SingleGammaExtremeStats::~SingleGammaExtremeStats()
{
     this->destroy();
}




void SingleGammaExtremeStats::init()
{
//     TRI_LOG_STR("Extrem stats init");
     this->elements = new vector< SingleGammaExtremeStatsElement* >();

     this->values = new vector<double>();
}

void SingleGammaExtremeStats::destroy()
{
//     TRI_LOG_STR("Extreme stats destroy");

     std::vector<SingleGammaExtremeStatsElement *>::iterator it;

     for ( it=elements->begin(); it!=elements->end(); ++it ) {
          SingleGammaExtremeStatsElement * ese = *it;
          delete ese;
     }


     if ( this->values!=nullptr ) {
          delete this->values;
     }
}



void SingleGammaExtremeStats::fill ( double value )
{
     this->values->push_back ( value ); //zapisujemy wartosc do liczenia EDF pozniej


     if ( ( elements->size() > 0 ) && ( !elements->back()->full() ) ) {
//         TRI_LOG_STR("Add to existing");
          elements->back()->fill ( value );
     } else {
//         TRI_LOG_STR("New extreme stats element");
          SingleGammaExtremeStatsElement * ese = new SingleGammaExtremeStatsElement ( this->groupCount );
          ese->fill ( value );
          this->elements->push_back ( ese );
     }
}

void SingleGammaExtremeStats::saveRaw ( const char * datafileName )
{
     std::vector<SingleGammaExtremeStatsElement *>::iterator it;
     ofstream results ( datafileName );
     for ( it=elements->begin(); it!=elements->end(); ++it ) {
//         cout <<"group #" << (c++) << ": min = " << (*it)->getMin() << ": max = " << (*it)->getMax() << endl;
          results << ( *it )->getMin() << "\n";
     }
     results.close();
}


double SingleGammaExtremeStats::getLowestMin()
{
     std::vector<SingleGammaExtremeStatsElement *>::iterator it;

     double globalMin = 1000.0;
     for ( it=elements->begin(); it!=elements->end(); ++it ) {
          if ( globalMin > ( *it )->getMin() ) {
               globalMin = ( *it )->getMin();
          }
     }
     return globalMin;
}

double SingleGammaExtremeStats::getHighestMin()
{
     std::vector<SingleGammaExtremeStatsElement *>::iterator it;

     double globalMax = 0.0;
     for ( it=elements->begin(); it!=elements->end(); ++it ) {
          if ( globalMax < ( *it )->getMin() ) {
               globalMax = ( *it )->getMin();
          }
     }
     return globalMax;
}

double SingleGammaExtremeStats::getLowestMax()
{
     std::vector<SingleGammaExtremeStatsElement *>::iterator it;

     double globalMin = 1000.0;
     for ( it=elements->begin(); it!=elements->end(); ++it ) {
          if ( globalMin > ( *it )->getMax() ) {
               globalMin = ( *it )->getMax();
          }
     }
     return globalMin;
}

double SingleGammaExtremeStats::getHighestMax()
{
     std::vector<SingleGammaExtremeStatsElement *>::iterator it;

     double globalMax = 0.0;
     for ( it=elements->begin(); it!=elements->end(); ++it ) {
          if ( globalMax < ( *it )->getMax() ) {
               globalMax = ( *it )->getMax();
          }
     }
     return globalMax;
}


void SingleGammaExtremeStats::saveMinimumsEDF()
{
     // spodziewamy sie ze
     // F_min_n  = 1 - [1 - F(t)]^n
     //trzeba zapisac dystrybuante z minimow (F_min_n) i z calosci (F(t))

     Settings& settings = Settings:: getInstance ( );
//
     double eplus = settings.get ( "eplus" );
     double eminus = settings.get ( "eminus" );
     double alpha = settings.getJumpsParameter();
     double beta = settings.getWaitingTimesParameter();



     char edf_values_filename[200];
     sprintf ( edf_values_filename,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_g%2.8f_edf.txt",settings.getStoragePath(), alpha,eplus,eminus,gamma );


     ofstream edf_values_file ( edf_values_filename,ios_base::out );

     // liczymy 'normalna' dystrybuante
     sort ( this->values->begin(), this->values->end() );
     int vecSize = this->values->size();

     edf_values_file << "#t" <<"\t" << "F(t)"<<"\t" << "[F(t)]^"<< this->groupCount << "\t" <<  "1 - [1-F(t)]^"<<this->groupCount<<endl;

     for ( int i = 0; i < vecSize ; i++ ) {
          double val = this->values->at ( i );
          double EDFval = ( ( double ) i ) / ( ( double ) vecSize );

          edf_values_file << val << "\t" << EDFval <<"\t" <<  pow ( EDFval,this->groupCount ) <<"\t" << 1.0 - pow ( 1.0 - EDFval,this->groupCount )  << "\n";
     }
     edf_values_file.close();

     //dystrybuanta minimow

     char minimas_edf_values_filename[200];
     sprintf ( minimas_edf_values_filename,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_g%2.8f_min_edf.txt",settings.getStoragePath(), alpha,eplus,eminus,gamma );

     // liczymy dystrybuante minimow
     vector<double> * minimas = new vector<double>();

     vector<SingleGammaExtremeStatsElement *>::iterator it;
     for ( it=elements->begin(); it!=elements->end(); ++it ) {
          minimas->push_back ( ( *it )->getMin() );
     }

     sort ( minimas->begin(), minimas->end() );
     int minimasSize = minimas->size();

     //zapisujemy wartosci edf_min

     ofstream edf_minvalues_file ( minimas_edf_values_filename,ios_base::out );
     edf_minvalues_file << "#t" <<"\t" << "F(t)_min_"<< this->groupCount <<endl;

     for ( int i = 0; i < minimasSize ; i++ ) {
          double val = minimas->at ( i );
          double EDFval = ( ( double ) i ) / ( ( double ) minimasSize );

          edf_minvalues_file << val << "\t" << EDFval <<"\t" <<  "\n";
     }

     edf_minvalues_file.close();
     delete minimas;

}

void SingleGammaExtremeStats::saveMaximumsEDF()
{

     // spodziewamy sie ze
     // F_max_n  = [F(t)]^n

     //trzeba zapisac EDF (aka F(t)) z maksimow i edf z calosci

     Settings& settings = Settings:: getInstance ( );
//
     double eplus = settings.get ( "eplus" );
     double eminus = settings.get ( "eminus" );
     double alpha = settings.getJumpsParameter();
     double beta = settings.getWaitingTimesParameter();



// 	char edf_values_filename[200];
// 	sprintf(edf_values_filename,"%s/alpha_%1.2f_beta_%1.3f_Ep_%1.1f_Em_%1.1f_%2.1f_edf.txt",settings.getStoragePath(), alpha,beta,eplus,eminus,gamma);
//
//
// 	ofstream edf_values_file(edf_values_filename,ios_base::out);
//
//     // liczymy 'normalna' dystrybuante
//     sort( this->values->begin(), this->values->end() );
//     int vecSize = this->values->size();
//
// 	edf_values_file << "#t" <<"\t" << "F(t)"<<"\t" << "[F(t)]^"<< this->groupCount <<endl;
//
// 	for(int i = 0; i < vecSize ; i++) {
// 	  double val = this->values->at(i);
// 	  double EDFval = ((double) i)/((double) vecSize);
//
// 	  edf_values_file << val << "\t" << EDFval <<"\t" <<  pow(EDFval,this->groupCount) <<  "\n";
//       }
// 	edf_values_file.close();

     //dystrybuanta maximow

     char maximas_edf_values_filename[200];
     sprintf ( maximas_edf_values_filename,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_g%2.8f_max_edf.txt",settings.getStoragePath(), alpha,eplus,eminus,gamma );

     // liczymy dystrybuante maximow
     vector<double> * maximas = new vector<double>();

     vector<SingleGammaExtremeStatsElement *>::iterator it;
     for ( it=elements->begin(); it!=elements->end(); ++it ) {
          maximas->push_back ( ( *it )->getMax() );
     }

     sort ( maximas->begin(), maximas->end() );
     int maximasSize = maximas->size();

     //zapisujemy wartosci edf_max

     ofstream edf_maxvalues_file ( maximas_edf_values_filename,ios_base::out );
     edf_maxvalues_file << "#t" <<"\t" << "F(t)_max_"<< this->groupCount <<endl;
     for ( int i = 0; i < maximasSize ; i++ ) {
          double val = maximas->at ( i );
          double EDFval = ( ( double ) i ) / ( ( double ) maximasSize );

          edf_maxvalues_file << val << "\t" << EDFval <<"\t" <<  "\n";
     }

     edf_maxvalues_file.close();
     delete maximas;
}

// array to tablica 9 elementowac ggdzie beda zapisane wartosci
// narazie size musi byc == 9, zwracamy kwantyle q1 - q9
void SingleGammaExtremeStats::getMinumumsQuantiles ( double *valsArray, int size )
{
     if ( size!=9 ) {
          cerr << " array size must be 9!" <<endl;
          return ;
     }

     // liczymy dystrybuante minimow
     vector<double> * minimas = new vector<double>();

     vector<SingleGammaExtremeStatsElement *>::iterator it;
     for ( it=elements->begin(); it!=elements->end(); ++it ) {
          minimas->push_back ( ( *it )->getMin() );
	  cout << ( *it )->getMin() << ",";
     }

     sort ( minimas->begin(), minimas->end() );
     int minimasSize = minimas->size();

     //kwantyle

     for ( int q=1; q <= 9; q ++ ) {
          double p = 0.1 * q;
          int k = ceil ( p * minimasSize );

          //kwantyl rzedu p
	  cout << "mark555, k = " << k <<endl;
          double qVal = minimas->at ( k );
          valsArray[q-1] = qVal;
	  cout << "mark5556"<<endl;
     }
cout << "mark5557"<<endl;
     delete minimas;
}


void SingleGammaExtremeStats::getMaximumsQuantiles ( double *valsArray, int size )
{

     if ( size!=9 ) {
          cerr << " array size must be 9!" <<endl;
          return ;
     }

// liczymy dystrybuante maximow
     vector<double> * maximas = new vector<double>();

     vector<SingleGammaExtremeStatsElement *>::iterator it;
     for ( it=elements->begin(); it!=elements->end(); ++it ) {
          maximas->push_back ( ( *it )->getMax() );
     }

     sort ( maximas->begin(), maximas->end() );
     int maximasSize = maximas->size();


     //kwantyle

     for ( int q=1; q <= 9; q ++ ) {
          double p = 0.1 * q;
          int k = ceil ( p * maximasSize );

          //kwantyl rzedu p
          double qVal = maximas->at ( k );
          valsArray[q-1] = qVal;
     }
     delete maximas;

}



/// =========================================================

// SingleGammaExtremeStatsElement

SingleGammaExtremeStatsElement::SingleGammaExtremeStatsElement ( int count ) : values ( nullptr )
{
     this->maxCount = count;
     this->minElement = 100.0;
     this->maxElement = 0.0;

     this->values = new vector<double>();
}


SingleGammaExtremeStatsElement::~SingleGammaExtremeStatsElement()
{
     if ( this->values!=nullptr ) {
          delete values;
     }
}


void SingleGammaExtremeStatsElement::fill ( double v )
{
//   cout << "fill ( " << v <<"), current min=" << minElement << ", currnet max=" << maxElement <<endl;
     this->values->push_back ( v );
     if ( v < minElement )  {
          this->minElement = v;
     }
     if ( v > maxElement ) {
          this->maxElement = v;
     }
}


bool SingleGammaExtremeStatsElement::full()
{
     return ! ( this->values->size() < this->maxCount );
}

