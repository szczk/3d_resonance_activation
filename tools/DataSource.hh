#ifndef __DATASOURCE__HH__
#define __DATASOURCE__HH__

using namespace std;

/**
 * interface for datafiles and datafilechains
 * 
 * 
 */ 

class DataSource {
    
  
public:
    
    virtual ~DataSource() {};
  
    virtual bool ok() = 0;
    
    
    /*
     * Close source
     */
    virtual void close() = 0;

    /**
     * Read next value 
     */
    virtual double read() = 0;
    
    /**
     * True if still has values to read, false otherwise
     */
    virtual bool hasNext() = 0;

    
    /**
     * Data count
     */
    virtual int getCount() = 0;
    
    
    /**
     * Save next value
     */
    virtual void write(double value) = 0;
    
    
};


#endif




