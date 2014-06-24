#include "Quantiles.hh"


Quantiles:: Quantiles() : quantiles ( nullptr )
{
     clog << "create Quantiles "<<endl;
     this->init();
}




Quantiles:: ~Quantiles()
{
     clog << "destroy Quantiles "<<endl;
     this->destroy();
}


void Quantiles:: init()
{
     this->quantiles = new map<double,SingleGammaQuantiles *>();

}


void Quantiles:: destroy()
{
     if ( this->quantiles!=nullptr ) {

          map<double,SingleGammaQuantiles *>::iterator it;

          for ( it = this->quantiles->begin(); it != this->quantiles->end(); ++it ) {
               SingleGammaQuantiles * m = ( *it ).second;
               delete m;
          }

          delete this->quantiles;

     }
}




SingleGammaQuantiles * Quantiles:: getSingleGammaQuantiles ( double gamma )
{

     SingleGammaQuantiles * sgqt = nullptr;

     if ( this->quantiles->count ( gamma ) > 0 ) {
          sgqt = this->quantiles->at ( gamma );
     } else {
          sgqt = new SingleGammaQuantiles();
          this->quantiles->insert ( pair<double, SingleGammaQuantiles*> ( gamma, sgqt ) ) ;
     }

     return sgqt;
}


void Quantiles:: addValue ( double gamma, double value )
{
     this->getSingleGammaQuantiles ( gamma )->addValue ( value );
}


void Quantiles:: save()
{


     clog << "Saving quantiles " << endl;

     Settings& settings = Settings:: getInstance ( );

     double eplus = settings.get ( "eplus" );
     double eminus = settings.get ( "eminus" );
     double alpha = settings.getJumpsParameter();
     double beta = settings.getWaitingTimesParameter();

     char quantiles_filename[200];
     char quantile_plotname[200];

     sprintf ( quantiles_filename,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_quantiles.txt",settings.getStoragePath(), alpha,eplus,eminus );

     ofstream quantiles_file ( quantiles_filename,ios_base::out );

     //to samo bez sciezki, do uzycia w skrypcie gnuplota
     sprintf ( quantiles_filename,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_quantiles.txt",alpha,eplus,eminus );

     quantiles_file << "#log10(gamma)\tq1\tq2\tq3\tq4\tq5\tq6\tq7\tq8\tq9\t(q9-q1)\tlog10(q9-q1)\n";

     // zapisywanie kwantyli
     double quantiles[9];

     double smallestWidth = 100.0;
     double positionOfSmallestWidth = 0.0;

     //double median = 0.0;
     double smallestMedian = 100.0;
     double positionOfSmallestMedian = 0.0;



     // wartosci skrajne (extended data)
     // szerokosc q9-q1
     double q9q1_f_minus_infinity = 0.0;
     double q9q1_f_plus_infinity = 0.0;
     double q9q1_f_w_minimum = 0.0;
     double q9q1_gamma_for_f_minimum = 0.0;

     // mediana
     double median_f_minus_infinity = 0.0;
     double median_f_plus_infinity = 0.0;
     double median_f_w_minimum = 0.0;
     double median_gamma_for_f_minimum = 0.0;


     map<double,SingleGammaQuantiles *>::iterator it;
     for ( it = this->quantiles->begin(); it != this->quantiles->end(); ++it ) {
          double gamma = ( *it ).first;
          SingleGammaQuantiles * sgq = ( *it ).second;

          quantiles_file << log10 ( gamma );

          sgq->getQuantiles ( quantiles,9 );

          for ( int i=0; i<9; i++ ) {
               quantiles_file <<"\t" << quantiles[i];
          }

          // 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9
          // mediana - kwantyl rzedu 0.5 czyli quantiles[4]
          //median = quantiles[4];

          if ( smallestMedian >= quantiles[4] ) {
               smallestMedian = quantiles[4];
               positionOfSmallestMedian = log10 ( gamma );

               median_f_w_minimum = quantiles[4];
               median_gamma_for_f_minimum = gamma;
          }


          double qWidth = ( quantiles[8]-quantiles[0] );
          if ( smallestWidth >= qWidth ) {
               smallestWidth = qWidth;
               positionOfSmallestWidth = log10 ( gamma );

               q9q1_f_w_minimum = qWidth;
               q9q1_gamma_for_f_minimum = gamma;
          }
          quantiles_file << "\t" << qWidth <<"\t" << log10 ( qWidth ) << "\n";

          //extended data
          if ( it == this->quantiles->begin() ) {
               q9q1_f_minus_infinity = qWidth;
               median_f_minus_infinity = quantiles[4];
          }
          q9q1_f_plus_infinity = qWidth;
          median_f_plus_infinity = quantiles[4];
     }



     quantiles_file.close();


     // gnuplot

     char resultsPlotscriptfile[200];
     sprintf ( resultsPlotscriptfile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_plotquantiles.gnu",settings.getStoragePath(), alpha,eplus,eminus );
     ofstream gnuplot_file ( resultsPlotscriptfile,ios_base::out );


     sprintf ( quantile_plotname,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_quantiles_plot_log.eps", alpha,eplus,eminus );

     gnuplot_file<< "reset\n";
//     gnuplot_file<< "set terminal pngcairo enhanced size 1200,700;\n";
     gnuplot_file<< "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";

     gnuplot_file<< "set output \""<< quantile_plotname << "\"\n";
     gnuplot_file<< "set xlabel \"log_{10}({/Symbol g})\"\n";
     gnuplot_file<< "set ylabel \"log_{10}(q_{9} - q_{1})\"\n";
//     gnuplot_file<< "set title \"E_{+}= "<< eplus <<"T, E_{-}= "<< eminus <<"T,  {/Symbol a}="<<alpha<<", {/Symbol b}="<< beta << "\";\n";

     //strzalka
//     double arrowX1 = positionOfSmallestWidth;
//     double arrowX2 = arrowX1;
//     double arrowY1 = log10(smallestWidth) + abs( log10(smallestWidth) )*0.8;
//     double arrowY2 = log10(smallestWidth) + abs( log10(smallestWidth) )*0.4;
//     gnuplot_file << "set style arrow 1 head filled size screen 0.02,6,1 ls 1\n";
//     gnuplot_file << "set arrow from "<<arrowX1<<","<<arrowY1<<" to "<<arrowX2<<","<<arrowY2<<" as 1 \n";
//
//     double labelX= arrowX1 - abs(arrowX1)*0.2;
//     double labelY= arrowY1 ;
//
//     gnuplot_file << "set label 'log_{10} ({/Symbol g})="<< positionOfSmallestWidth <<"' at "<<labelX<<","<< labelY <<" left\n";



     gnuplot_file<< "plot '"<<quantiles_filename<<"' using 1:12 with lp notitle lc rgbcolor '#000000';\n";


     // mediana
     sprintf ( quantile_plotname,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_quantiles_plot_median.eps", alpha,eplus,eminus );

     gnuplot_file<< "reset\n";
//     gnuplot_file<< "set terminal pngcairo enhanced size 1200,700;\n";
     gnuplot_file<< "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";
     gnuplot_file<< "set output \""<< quantile_plotname << "\"\n";
     gnuplot_file<< "set xlabel \"log_{10}({/Symbol g})\"\n";
     gnuplot_file<< "set ylabel \"log_{10}(q_{0.5})\"\n";

//     gnuplot_file<< "set title \"{/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";
//     gnuplot_file<< "set title \"median E_{+}= "<< eplus <<"T, E_{-}= "<< eminus <<"T,  {/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";

     //strzalka
//     arrowX1 = positionOfSmallestMedian;
//     arrowX2 = arrowX1;
//     arrowY1 = (smallestMedian) + abs( (smallestMedian) )*0.05;
//     arrowY2 = (smallestMedian) + abs( (smallestMedian) )*0.02;
//     gnuplot_file << "set style arrow 1 head filled size screen 0.02,6,1 ls 1\n";
//     gnuplot_file << "set arrow from "<<arrowX1<<","<<arrowY1<<" to "<<arrowX2<<","<<arrowY2<<" as 1 \n";
//
//     labelX= arrowX1 - abs(arrowX1)*0.2;
//     labelY= arrowY1 ;
//
//     gnuplot_file << "set label 'log_{10} ({/Symbol g})="<< positionOfSmallestMedian <<"' at "<<labelX<<","<< labelY <<" left\n";

     gnuplot_file<< "plot '"<<quantiles_filename<<"' using 1:6 with lp notitle lc rgbcolor '#000000';\n";


     // wszystkie kwantyle


     gnuplot_file<< "reset\n";
     sprintf ( quantile_plotname,"alpha_%1.2f_Ep_%1.1f_Em_%1.1f_quantiles_plot.eps", alpha,eplus,eminus );

//     gnuplot_file<< "set terminal pngcairo enhanced size 1200,700;\n";
     gnuplot_file<< "set terminal post eps size 12,7 enhanced color font 'Helvetica,35' linewidth 2;";

     gnuplot_file<< "set output \""<< quantile_plotname << "\"\n";
     gnuplot_file<< "set xlabel \"log_{10}({/Symbol g})\"\n";
     gnuplot_file<< "set ylabel \"q\"\n";
     gnuplot_file<< "set logscale y\n";
//     gnuplot_file << "set title \"{/Symbol a}="<<alpha<<", {/Symbol n}="<< beta << "\";\n";
     gnuplot_file<< "plot '"<<quantiles_filename<<"' using 1:2 with lp title \"q_{0.1}\",\\\n";
     gnuplot_file<< "'"<<quantiles_filename<<"' using 1:3 with lp title \"q_{0.2}\",\\\n";
     gnuplot_file<< "'"<<quantiles_filename<<"' using 1:4 with lp title \"q_{0.3}\",\\\n";
     gnuplot_file<< "'"<<quantiles_filename<<"' using 1:5 with lp title \"q_{0.4}\",\\\n";
     gnuplot_file<< "'"<<quantiles_filename<<"' using 1:6 with lp title \"q_{0.5}\",\\\n";
     gnuplot_file<< "'"<<quantiles_filename<<"' using 1:7 with lp title \"q_{0.6}\",\\\n";
     gnuplot_file<< "'"<<quantiles_filename<<"' using 1:8 with lp title \"q_{0.7}\",\\\n";
     gnuplot_file<< "'"<<quantiles_filename<<"' using 1:9 with lp title \"q_{0.8}\",\\\n";
     gnuplot_file<< "'"<<quantiles_filename<<"' using 1:10 with lp title \"q_{0.9}\";\n";



     gnuplot_file.close();


     //===================================================================================
     // save extended_data
     char extendedDataFile[200];
     sprintf ( extendedDataFile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extdata_q9q1.txt",settings.getStoragePath(), alpha,eplus,eminus );
     ofstream extended ( extendedDataFile );
     extended << extended_data_header << endl;
     extended << alpha << "\t"<< beta;
     // wartosc f(gamma=-infinity)
     extended << "\t" << q9q1_f_minus_infinity;
     // wartosc f(gamma=+infinity)
     extended << "\t" << q9q1_f_plus_infinity;
     // wartosc minimalna charakterystyki f(gamma)
     extended << "\t" << q9q1_f_w_minimum;
     // wartosc gamma dla ktorej charakterystyka ma minimum
     extended << "\t" << q9q1_gamma_for_f_minimum;
     extended.close();

     sprintf ( extendedDataFile,"%s/alpha_%1.2f_Ep_%1.1f_Em_%1.1f_extdata_median.txt",settings.getStoragePath(), alpha,eplus,eminus );
     extended.open ( extendedDataFile );
     extended << extended_data_header << endl;
     extended << alpha << "\t"<< beta;
     // wartosc f(gamma=-infinity)
     extended << "\t" << median_f_minus_infinity;
     // wartosc f(gamma=+infinity)
     extended << "\t" << median_f_plus_infinity;
     // wartosc minimalna charakterystyki f(gamma)
     extended << "\t" << median_f_w_minimum;
     // wartosc gamma dla ktorej charakterystyka ma minimum
     extended << "\t" << median_gamma_for_f_minimum;
     extended.close();
}


//==================================================================

SingleGammaQuantiles:: SingleGammaQuantiles() : values ( nullptr )
{
     this->values = new vector<double>();
}
SingleGammaQuantiles:: ~SingleGammaQuantiles()
{
     if ( this->values!=nullptr ) {
          delete this->values;
     }
}

void SingleGammaQuantiles:: addValue ( double v )
{
     this->values->push_back ( v );
}

// valsArray to tablica 9 elementowac ggdzie beda zapisane wartosci
void SingleGammaQuantiles:: getQuantiles ( double * valsArray, int size )
{

     //TODO: dodac wsparcie dowolnej ilosci kwantyli
     if ( size!=9 ) {
          cerr << " array size must be 9!" <<endl;
          return ;
     }

     //sort values
     sort ( this->values->begin(), this->values->end() );
     int vecSize = this->values->size();

     // tutaj mamy dystrybuante

// 	     for(int i = 0; i < vecSize ; i++) {
// 	double val = this->values->at(i);
// 	double EDFval = ((double) i)/((double) vecSize);
//
// 	edf_file << val << "\t" << EDFval <<"\t" << (1.0 - EDFval) << "\t" << log10(1.0 - EDFval) << "\n";
//       }



     //kwantyle

     for ( int q=1; q <= 9; q ++ ) {
          double p = 0.1 * q;
          int k = ceil ( p * vecSize );

          //kwantyl rzedu p
          double qVal = this->values->at ( k );
          valsArray[q-1] = qVal;
     }

}

