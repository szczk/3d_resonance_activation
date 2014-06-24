#ifndef _SETTINGS_HH_
#define _SETTINGS_HH_


#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <unordered_set>
#include <iomanip>

using namespace std;



struct vec3d {
     double x;
     double y;
     double z;
};



// global settings class (singleton)

// reads settings file
// and provides access to parameters



class Settings {

public:
     /// GLOBAL PRECISION SETTINGS
     typedef double paramsType; /// these types are used in entire program
     typedef double resultType; /// using floats = 1/2 less memory needed
     ///*************************************************************************************

private:
     const char * settingsFile;

     map<string,paramsType> parameters;

     Settings ( const char * sFile );
     ~Settings();

     void loadFromFile();
     
     void init();
     void close();

     unsigned long int calculateNpoints();
     unsigned long int calculatePeriodNpoints();

     // path for temporary files
     string tmpPath;

     // path for result files
     string storagePath;
     // path for input .dat files
     string dataPath ;

     // prefix for output files
     string filesPrefix;

     string normalizePath ( const char * );
     
     
     

     // wskazuje na wielokrotne pliki wyjsciowe, uzywane w generator.cc

     /**
      * funkcja umozliwia szybsze generowanie wiekszej ilosci danych
      * zamiast jednego procesu , ktory generuje np 10^6 liczb w jednym pliku .dat
      * uruchamiamy 10 procesow generujacych 10^5 liczb, w 10 niezaleznych plikach dat
      * poniewaz pliki sa domyslnie nazywane alpha_XX_beta_XX_Ep_XX_Em_XX_gXX.dat
      * wygenerowanie wiecej niz jednego w tym samym katalogu powodowaloby nadpisywanie
      * wiec dodajemy numeracje, kazdy proces dostaje osobny numerek, przekazywany
      * commandline'owym parametrem "--data_file_num Y" gdzie Y to num,
      * dzieki czemu moze stworzyc plik dat z nazwa alpha_XX_beta_XX_Ep_XX_Em_XX_gXX_Y.dat


      * W analysis.cc mozna bezposrednio wczytywac takie wielokrotne pliki i traktowac jako jeden
      * data input, dzieki DatafilesChain ktory wlasnie do tego sluzy.
      * wywolanie analysis.x wlasnie z tym parametrem "--data_file_num Y" spowoduje
      * szukanie wielokrotnych plikow, a Y w tym przypadku powinien byc max wartoscia numerka w numeracji
      *
      * tzn jesli wygenerowalismy 10 plikow, ktore maja numery kolejno 1, 2, ... 9, 10
      * to nalezy wywolac analysis.x --data_file_num 10
      * DatafilesChain wczyta w ten sposob wszystkie 10, zaczynajac od 1
      */
     bool multipleOutputFile;
     int multipleOutputFileNumber;
     
     
     // czasy posrednie kiedy nalezy zapisywac 
     // stan symulacji
     // a dokladniej numer iteracji, a nei wartosc t!!
     unordered_set<int> * dumpTimes;
     void loadDumpTimes();
     

public:

     void setMultipleOutputFilenum ( int num ) {
          this->multipleOutputFileNumber = num;
          multipleOutputFile = true;
     }
     int  getMultipleOutputFilenum() {
          return this->multipleOutputFileNumber;
     }
     bool multipleOutputs() {
          return this->multipleOutputFile;
     }

     void setFile ( const char * sFile= "settings.dat" );
//     static Settings& getInstance();
     static Settings& getInstance ( const char * sFile= "settings.dat" );
     void printParameters();

     void dumpToFile ( const char * sFile = "params.txt" );

     paramsType get ( const char * key ) {
          return this->parameters[key];
     }

     void set ( const char *key, paramsType value ) {
          this->parameters[key] = value;
     }

     unsigned int getNtrajectories() {
          return ( ( unsigned int ) this->get ( "ntrajectories" ) );
     }

     void setNtrajectories ( unsigned int nt ) {
          this->parameters["ntrajectories"] = nt;
     }

     paramsType getStartTime() {
          return this->parameters["starttime"];
     }

     unsigned long int getNpoints() {
          return this->calculateNpoints();
     }
     unsigned long int getPeriodNpoints() {
          return this->calculatePeriodNpoints();
     }

     paramsType getDt() {
          return this->parameters["dt"];
     }

     /**
      * get superdiffusion parameter (usually named alpha)
      */

     paramsType getJumpsParameter() {
          return this->parameters["alpha"];
     }

     /**
      * get subdiffusion parameter (usually named beta)
      */

     paramsType getWaitingTimesParameter() {
          return this->parameters["beta"];
     }

     void setJumpsParameter ( paramsType alpha ) {
          this->parameters["alpha"] = alpha;
     }
     void setWaitingTimesParameter ( paramsType beta ) {
          this->parameters["beta"] = beta;
     }


     paramsType getA() {
          return this->parameters["PARAM_A"];
     }
     paramsType getB() {
          return this->parameters["PARAM_B"];
     }
     paramsType getA0() {
          return this->parameters["PARAM_A0"];
     }

     void setA0 ( paramsType a0 ) {
          this->parameters["PARAM_A0"] = a0;
     }

     paramsType getPhase() {
          return this->parameters["PHASE"];
     }

     void setPhase ( paramsType phase ) {
          this->parameters["PHASE"] = phase;
     }

     paramsType getFrequency() {
          return this->parameters["FREQ_Q"];
     }

     void setFrequency ( paramsType freq ) {
          this->parameters["FREQ_Q"] = freq;
     }


     paramsType getPeriod() {
          return 1.0/ ( this->getFrequency() );
     }

     paramsType getNoiseIntensity() {
          return this->parameters["NOISE_D"];
     }

     void setNoiseIntensity ( paramsType d ) {
          this->parameters["NOISE_D"] = d;
     }


     paramsType getX0() {
          return this->parameters["X0"];
     }

     void setX0 ( paramsType x0 ) {
          this->parameters["X0"] = x0;
     }

     /**
      * Read command line parameters, can be used to overwrite loaded params
      */
     void readCommandLineParameters ( int argc, char **argv );


     void setTmpPath ( const char * temp ) {
          this->tmpPath = normalizePath ( temp );
     }
     const char* getTmpPath() {
          return this->tmpPath.c_str();
     }

     void setStoragePath ( const char * st ) {
          this->storagePath = normalizePath ( st );

     }
     const char* getStoragePath() {
          return this->storagePath.c_str();
     }

     void setDataPath ( const char * st ) {
          this->dataPath = normalizePath ( st );

     }
     const char* getDataPath() {
          return this->dataPath.c_str();
     }


     void setNumThreads ( unsigned int threads ) {
          this->parameters["NUM_THREADS"] = threads;
     }

     unsigned int getNumThreads() {
          return this->parameters["NUM_THREADS"];
     }


     void setFilesPrefix ( const char * sufix ) {
          this->filesPrefix = normalizePath ( sufix );
     }

     const char * getFilesPrefix() {
          return this->filesPrefix.c_str();
     }

     bool isDumpTime(int iterationNum);
     
};



#endif
