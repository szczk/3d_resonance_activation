#ifndef __datafile_hh__
#define __datafile_hh__

#include <fstream>
#include <string.h>
#include <iostream>

#include "DataSource.hh"

using namespace std;



/*
 * simple data file support
 *
 * structure:
 *
 * [header section]
 * first 4 bytes:  asci text "DAT8" or "DAT4" - 8 or 4 means size of
 * data in data section of file (double of float)
 *
 * [data section]
 * just numbers in 8 or 4 bytes (doule of float)
 *
 */

// only double for now

// file can be created to write or opened to read, mutually exclusive operations

class Datafile : public DataSource {

private:
//     constexpr char * fileExtension = "dat"; //not used
    fstream * file;
    const char * filename;
    long fileSizeBytes;
    long headerSizeBytes;
    long singleValueSizeBytes; 

    bool okToRead;
    bool okToWrite;
    
    bool alreadyClosed;
   
    
    int valuesCount; // how many values in file
    int valuesRead; // values already read;
    

    /**
     * Read header. Return true if all is ok
     */
    bool readHeader();
    void writeHeader();
    
    void setFilename(const char * filename);
    
    void createNew(const char * filename);
    void closeFile();

    
    void openExisting(const char * filename);
    
    
    
public:
    Datafile();
    virtual ~Datafile();
    
    
    
    /**
     * Open existing file to read from
     */
    static Datafile* open(const char * filename);
    /**
     * Create new file to write into
     */
    static Datafile* create(const char * filename);

    virtual bool ok();
    virtual void close();

    /**
     * Read next value from file
     */
    virtual double read();
    
    /**
     * True if still has values to read, false otherwise
     */
    virtual bool hasNext();

    virtual int getCount() { return this->valuesCount; }
    
    /**
     * Save next value to file
     */
    virtual void write(double value);
};
#endif
