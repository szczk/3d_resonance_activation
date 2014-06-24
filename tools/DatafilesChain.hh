#ifndef __DATAFILES_CHAIN__
#define __DATAFILES_CHAIN__

#include <vector>
#include <iostream>
#include <string>
#include "DataSource.hh"
#include "Datafile.hh"

using namespace std;

/**
 * chain of Datafile-s - makes it possible to open multiple files
 * (generated with 'multiple output' option in generator.cc)
 * and read them as one big file (just like TChain in ROOT)
 *
 * simplest implementation - all files are named the same, only with _N at the end
 * of filename, where N is the file number (number is not used for order, only
 * to differentiate between files)
 *
 *
 * obviously, supports only reading, not writing
 *
 *
 * provides methods from Datafile interface:
 *
 *
 * bool hasNext()
 * double read()
 * int getCount()
 */

class DatafilesChain : public DataSource {

private:
     vector<Datafile *> * files;
     bool alreadyClosed;
     const char * namesTemplate;
     int startNum;
     int endNum;

     long totalCount;
     void discoverFiles();
     void init();
     
     int currentFile;

public:

     // constructor, requires datafiles name template, where "{NUM}" indicates
     // number part of every filename
     // minNum and maxNum are lower and higher boundary numbers
     // when searching for files, iteration will go from minNum to maxNum, incrementing by 1
     // and replacing "{NUM}" string with current number in filename
     DatafilesChain ( const char * filename_template, int minNum, int maxNum );
     ~DatafilesChain();


     virtual bool ok();

     virtual void close();

     virtual double read();

     virtual bool hasNext();

     virtual int getCount();

     virtual void write ( double value );
};

#endif
