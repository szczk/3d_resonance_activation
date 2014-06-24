#include "Settings.hh"




Settings& Settings::getInstance ( const char * sFile )
{

     static Settings instance ( sFile );

     return instance;
}



void Settings::readCommandLineParameters ( int argc, char** argv )
{
     for ( int i = 1; i < argc; i++ ) {
          if ( strcmp ( "--alpha",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new alpha: " << atof ( argv[i+1] ) <<endl;
               this->setJumpsParameter ( atof ( argv[i+1] ) ) ;
          }
          if ( strcmp ( "--beta",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new beta: " << atof ( argv[i+1] ) <<endl;
               this->setWaitingTimesParameter ( atof ( argv[i+1] ) ) ;
          }
          if ( strcmp ( "--freq",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new frequency: " << atof ( argv[i+1] ) <<endl;
               this->setFrequency ( atof ( argv[i+1] ) ) ;
          }
          if ( strcmp ( "--A0",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new A0: " << atof ( argv[i+1] ) <<endl;
               this->setA0 ( atof ( argv[i+1] ) ) ;
          }
          if ( strcmp ( "--noise",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new D: " << atof ( argv[i+1] ) <<endl;
               this->setNoiseIntensity ( atof ( argv[i+1] ) ) ;
          }

          if ( strcmp ( "--tmp",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new temp path:: '" << argv[i+1] << "'" << endl;
               this->setTmpPath ( argv[i+1] ) ;
          }

          if ( strcmp ( "--storage",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new storage path: '" << argv[i+1] << "'" << endl;
               this->setStoragePath ( argv[i+1] ) ;
          }

          if ( strcmp ( "--data",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new data path: '" << argv[i+1] << "'" << endl;
               this->setDataPath ( argv[i+1] ) ;
          }

          if ( strcmp ( "--prefix",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new files prefix: '" << argv[i+1] << "'" << endl;
               this->setFilesPrefix ( argv[i+1] ) ;
          }


          if ( strcmp ( "--n",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " new ntrajectories: '" << argv[i+1] << "'" << endl;
               this->setNtrajectories ( atoi ( argv[i+1] ) );
          }

          if ( strcmp ( "--threads",argv[i] ) == 0  && ( i+1 ) < argc ) {
               cout << " number of threads: '" << argv[i+1] << "'" << endl;
               this->setNumThreads ( atoi ( argv[i+1] ) );
          }



          if ( strcmp ( "--pt",argv[i] ) == 0  && ( i+1 ) < argc ) {
               int type = atoi ( argv[i+1] ) ;
               if ( type!=2 && type!=4 ) type = 1;
               cout << " new potential type: '" << type << "'" << endl;
               this->set ( "POTENTIAL_TYPE", type );
          }

          if ( strcmp ( "--nt",argv[i] ) == 0  && ( i+1 ) < argc ) {
               int type = atoi ( argv[i+1] ) ;
               if ( type!=1 && type!=2 ) type = 1;
               cout << " new noise type: '" << type << "'" << endl;
               this->set ( "NOISE_TYPE", type ) ;
          }

          if ( strcmp ( "--gamma_p",argv[i] ) == 0 && ( i+1 ) < argc ) {
               //potega gammy
               double p = atof ( argv[i+1] );
               cout << "new gamma power: " << p << endl;
               this->set ( "GAMMA_P", p );
          }

          // usunac kiedy nie bedzie juz potrzebne
          if ( strcmp ( "--data_file_num",argv[i] ) == 0  && ( i+1 ) < argc ) {
               this->setMultipleOutputFilenum ( atoi ( argv[i+1] ) );
          }

          
          if ( strcmp ( "--eplus",argv[i] ) == 0  && ( i+1 ) < argc ) {
	        cout << " new eplus: '" << atof ( argv[i+1] ) << "'" << endl;
               this->set("eplus", atof ( argv[i+1] ) );
          }
          
          if ( strcmp ( "--eminus",argv[i] ) == 0  && ( i+1 ) < argc ) {
	       cout << " new minus: '" << atof ( argv[i+1] ) << "'" << endl;
               this->set("eminus", atof ( argv[i+1] ) );
          }
          
     }
}


Settings:: Settings ( const char * sFile )
{
     this->settingsFile = sFile;
     this->init();
}

Settings:: ~Settings()
{
     this->close();
}


void Settings:: init()
{
     this->setStoragePath ( "./" );
     this->setTmpPath ( "/tmp/" );
     this->setDataPath ( "./" );
     this->setFilesPrefix ( "" );
     this->loadFromFile();

     this->loadDumpTimes();
}

void Settings:: close()
{
     cout << "removing Settings..."<<flush;

     // nothing here...

     cout << "ok." <<endl;
}

void Settings:: loadFromFile()
{
     cout << " Reading file:" << this->settingsFile << ""<<endl;
     ifstream settings ( this->settingsFile,ios_base::in );

     //read file
     string line;

     string paramName;
     paramsType paramValue;

     if ( settings.is_open() ) {
          while ( settings.good() ) {
               getline ( settings,line );
               if ( line[0] =='#' || line.size() <1 ) continue;

               stringstream sline ( line );

               string a;
               if ( getline ( sline,a,'=' ) ) {
                    paramName = a;
                    paramName.erase ( 0, paramName.find_first_not_of ( ' ' ) ); //trim prefixing spaces
                    paramName.erase ( paramName.find_last_not_of ( ' ' ) +1 );  //trim surfixing spaces

// 	cout <<"'" << paramName << "'"<<endl;
                    if ( paramName.size() ==0 ) {
                         //string trimmed to 0
                         continue;
                    }
                    sline >> paramValue;

                    this->parameters.insert ( pair<string,paramsType> ( paramName,paramValue ) );
               }

          }
          settings.close();
     }

//   settings.close();
}


void Settings:: printParameters()
{
     map<string,paramsType>::iterator it;
     for ( it=this->parameters.begin() ; it != this->parameters.end(); it++ ) {
          cout << ( *it ).first << " = " << ( *it ).second << endl;
     }

     cout << "\n\n" << flush ;
}


// dump parameters to file
void Settings:: dumpToFile ( const char * sFile )
{
     ofstream fout ( sFile,ios_base::trunc );


     fout << "trajectory length: " << this->parameters["num_of_periods"] << " * 2^(" << this->parameters["length_power"] << ")";
     fout << setprecision ( 30 ) << " = " << this->calculateNpoints() << setprecision ( 3 ) << endl;

     fout << "jumps parameter: \n alpha = " << this->parameters["alpha"] <<endl << endl;
     fout << "waiting time parameter:\n beta = " << this->parameters["beta"] <<endl << endl;


     fout << "time step: \n dt = " << this->parameters["dt"] <<endl << endl;
     fout << "start time: \n t0 = " << this->parameters["starttime"] <<endl << endl;

     fout << "------------------------------------------" << endl;

     map<string,paramsType>::iterator it;
     for ( it=this->parameters.begin() ; it != this->parameters.end(); it++ ) {
          fout << ( *it ).first << " = " << ( *it ).second << endl;
     }
     fout << endl;

     fout.close();
}

unsigned long int Settings:: calculateNpoints()
{
     return ( ( unsigned long int ) ( this->parameters["num_of_periods"] * pow ( 2.0, this->parameters["length_power"] ) ) );
}

unsigned long int Settings:: calculatePeriodNpoints()
{
     return ( ( unsigned long int ) ( pow ( 2.0, this->parameters["length_power"] ) ) );
}

string  Settings::normalizePath ( const char* str )
{
     string cc ( str );
//     if ( cc.at( cc.length() -1 ) != '/' )
//     {
//         cc.append("/");
//         cerr << "Settings:: fixing path '" << str << "' to '" << cc <<"'" << endl;
//     }
     return cc;
}

void Settings::loadDumpTimes()
{

     double dump_every = this->get ( "dump_every" );
     double maxT = this->get ( "maxT" );
     double dt = this->get ( "dt" );


     if ( dump_every > 0.0 && dump_every < maxT ) {
          cout << "dump iterations: ";

          this->dumpTimes = new unordered_set<int>();

          for ( double t = dump_every ; t < maxT; t+=dump_every ) {
               int iter = t/dt;
               cout << iter << ",";

               this->dumpTimes->insert ( iter );
          }


          cout << endl;
     }
}

bool Settings::isDumpTime ( int iteration )
{
     if ( this->dumpTimes!=nullptr ) {
//             cout << setprecision(30) << t<<endl;
//             for ( auto it = myset.begin(); it != myset.end(); ++it )
//     std::cout << " " << *it;
          return this->dumpTimes->count ( iteration );
     } else {
          cout <<"nullptr"<<endl;
          return false;
     }
}

