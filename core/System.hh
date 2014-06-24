#ifndef SYSTEM_H
#define SYSTEM_H

#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <sys/sysinfo.h>
#include "unistd.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
// #include <limits>
#include <cstdio>
#include <set>

#include "Settings.hh"



using namespace std;


class System
{
private:
    long vMemSafetyLimit; // memory limit - if virtualMemSize > vMemSafetyLimit , System will call abort() and shut down entire program
   
  
    long pagesize;

    long virtualMemSize; // program virtual memory size
    long residentMemSize; // program residend memory size
    
    System()  ; 
    ~System();
    
    time_t start_time;
    time_t finish_time;

    void readMemoryUsage() ;
    void checkMemoryLimit() ;
    
    set<string> * filesToDelete;
    
public:
    static System& getInstance();
    
    static const char* getCompilationDate()
    {
        return  __DATE__;
    }


    long getMemoryUsage();
    unsigned long getSystemMemory();
    long int getProcessUserTime();
    long int getProcessSystemTime();
    
    int getNumberOfCPUs();
    
    void start();
    void finish();
    void printTimeSummary();
    void printInfo();
    
    void printMemoryUsage();
    
    long getPageSize();
    long getPageSizeKB();
    
    long getVmUsage();
    long getRmUsage();
    
    long getVmUsageKB() { return this->getVmUsage()/1024; }
    long getRmUsageKB() { return this->getRmUsage()/1024; }
    
    long getVmUsageMB() { return this->getVmUsageKB()/1024; }
    long getRmUsageMB() { return this->getRmUsageKB()/1024; }
    
    void setMemoryLimit(long memLimit) { this->vMemSafetyLimit = memLimit; }
    
    static void waitForKey();
    
    static string calculateTime(time_t time_started, double percent_done);
    static string formatTime( double seconds );
    
    static double bytesToMB(long bytes);
    
    /**
     * tool: save array of values into file
     */
    static void saveArray( const char * fileName, Settings::resultType * array, size_t arraySize  );
    
    /**
     * tool: save 2 arrays of values into file
     */
    static void saveArrays( const char * fileName, Settings::resultType * array1, Settings::resultType * array2, size_t arraysSize  );
    
    
    // register file to delete later
    void tmpFile(const char *);
    void tmpFile(string);
    // delete all registered temp files
    void deleteTmpFiles();
	
	
	// signal handler, assures that temporary files will be deleted, etc.
	static void signalHandler(int signum);
	
};


#endif // SYSTEM_H
