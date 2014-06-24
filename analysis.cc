#include "main.hh"

#include <cstdlib>
#include <cmath>
#include <sys/stat.h>


#include "tools/Datafile.hh"

#include "analysis/DichotomicAnalysis.hh"
#include "tools/DatafilesChain.hh"

int main ( int argc , char ** argv )
{

     cout << endl << endl;

     System& sys = System::getInstance();
     sys.start();


     Settings& settings = Settings:: getInstance ( );


// "settings.dat"



     // override parameters from command line
     settings.readCommandLineParameters ( argc,argv );

     settings.printParameters();
     settings.dumpToFile ( "params.txt" );

     int noiseType = settings.get ( "NOISE_TYPE" );
     double eplus = settings.get ( "eplus" );
     double eminus = settings.get ( "eminus" );
     double alpha = settings.getJumpsParameter();


     cout << "entering loop"<<endl;

//     ResultAnalysis * result = new ResultAnalysis(&settings);

     DichotomicAnalysis * dichotomicAnalysis = new DichotomicAnalysis();

    //for ( double p = -5.0; p< 5.0; p += 0.1 ) 
     {
       double p = 0.0;

          double gamma = pow ( 10.0,p );
          double averageTime = 0.0;
          double averageNum = 0.0;

//         ExtremeStats * es = new ExtremeStats(10);

          char dataFile[200];

          RunningStat * runningStat = new RunningStat();


          DataSource * data = nullptr ;

          if ( settings.multipleOutputs() ) {
               sprintf ( dataFile,"%s/%s_alpha_%1.2f_n%d_Ep_%1.1f_Em_%1.1f_g_%1.1f_xy_output_{NUM}.dat",settings.getDataPath(), settings.getFilesPrefix(), alpha,  noiseType, eplus,eminus, p );

               // multiple inputs, we will use chained input files
               int filesNum = settings.getMultipleOutputFilenum();  // use this number as max-number indicator

               cout << "opening "<< filesNum << "chain files "<< dataFile << endl;
               data = new DatafilesChain ( dataFile ,1, filesNum );
          } else {
               cout << "opening single file"<<endl;
               sprintf ( dataFile,"%s/%s_alpha_%1.2f_n%d_Ep_%1.1f_Em_%1.1f_g_%1.1f_xy_output.dat",settings.getDataPath(), settings.getFilesPrefix(), alpha ,  noiseType ,eplus,eminus, p );
               data = Datafile::open ( dataFile );
          }

          int count = data->getCount();
          cout << "datafile contains " << count << " values"<<endl;
          int tenPerc = ( int ) ( count/10.0 );

          int c = 0;
          while ( data->hasNext() ) {
               if ( c%tenPerc==0 ) cout<< c << "/" << count <<endl;

               double time = data->read();


               runningStat->Push ( time );

// 	       cout << " time = " << time <<endl;
	       
               averageTime = averageTime + ( ( time - averageTime ) / ( averageNum + 1.0 ) );
               averageNum += 1.0;

               ++c;

               dichotomicAnalysis->addValue ( gamma,time );
          }

          data->close();

          double averageNew = dichotomicAnalysis->getAverage ( gamma );

          cout << "gamma=10^"<<p<<"\t MFPT: " << averageTime << " | " << averageNew << endl;;

          double mfpt = runningStat->Mean();
          double mfpt_err = runningStat->StandardDeviationOfMean();
          cout << "mfpt2 = " << mfpt <<  " \t +- " << mfpt_err << endl;


	  sys.printMemoryUsage();
	  
          delete data;
          delete runningStat;
     }
     dichotomicAnalysis->save();

     delete dichotomicAnalysis;



     // alpha beta f(gamma=-\infty) f(gammma=+\infty) f_{min} gamma_{min}

     sys.finish();
     sys.printTimeSummary();


     return 0;
}

