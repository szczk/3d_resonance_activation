#include "main.hh"
#include <gsl/gsl_histogram2d.h>
#include <gsl/gsl_histogram.h>
#include <fstream>



// tmp: /mnt/lustre/scratch/people/ufszczep/";
// storage: /storage/ufszczep/
char filePrefix[200] = "";

int main ( int argc, char **argv )
{
     cout << endl << endl;

     System& sys = System::getInstance();
     sys.start();

     sys.printInfo();

     Settings& settings = Settings:: getInstance ( "settings.dat" );
     settings.readCommandLineParameters ( argc,argv );
     settings.printParameters();


     double num = settings.getNtrajectories();
     int tenPerc = ( num>10 ) ? ( int ) ( num*0.1 ) : 1;


     int noiseType = settings.get ( "NOISE_TYPE" );
     double eplus = settings.get ( "eplus" );
     double eminus = settings.get ( "eminus" );
     double alpha = settings.getJumpsParameter();


//     double p = settings.get("GAMMA_P");

     Simulation * sim = new Simulation ( &settings );


     cout << "entering loop"<<endl;

     for ( double p = -5.0; p< 5.0; p += 0.1 ) 
     {
//       double p = 0.0;

          double gamma = pow ( 10.0,p );


          cout << " p = " << p << " gamma = " << gamma << endl;

          sim->setGamma ( gamma );



          char dataFile[200];
          if ( settings.multipleOutputs() ) {
               cout << " multiple outputs! generating file #" << settings.getMultipleOutputFilenum() <<endl;
               //wielokrotny output, wyjsciowy plik dat musi miec numerek przyslany z zewnatrz

               sprintf ( dataFile,"%s/%s_alpha_%1.2f_n%d_Ep_%1.1f_Em_%1.1f_g_%1.1f_xy_output_%d.dat",settings.getStoragePath(), settings.getFilesPrefix(), alpha,  noiseType, eplus,eminus, p , settings.getMultipleOutputFilenum() );
          } else {
               // wszystko do jednego pliku
               sprintf ( dataFile,"%s/%s_alpha_%1.2f_n%d_Ep_%1.1f_Em_%1.1f_g_%1.1f_xy_output.dat",settings.getStoragePath(), settings.getFilesPrefix(), alpha ,  noiseType ,eplus,eminus, p );
          }



          Datafile * data = Datafile::create ( dataFile );





          for ( int i =0; i < num ; i++ ) {
               if ( i%tenPerc==0 ) {
                    cout << i<<"/"<<num<<endl;
               }


               double t = sim->run();

               data->write ( t );

          }

//      output.close();
          data->close();


     }


     sys.finish();
     sys.printTimeSummary();

     return 0;
}


