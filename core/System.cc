#include "System.hh"


System& System::getInstance()
{
    static System instance;
    return instance;
}


System::System()
{
    this-> vMemSafetyLimit = 7516192768; // 7gb memory limit

    this->start_time = 0;
    this->finish_time = 0;

    this->pagesize = sysconf(_SC_PAGESIZE);

    this->virtualMemSize = 0;
    this->residentMemSize = 0;

    this->filesToDelete = NULL;
	
	// Register signal and signal handler
	signal(SIGINT, signalHandler);
}

System::~System()
{
    this->deleteTmpFiles();
}

void System:: checkMemoryLimit()
{
    if ( this->vMemSafetyLimit>0 && this->virtualMemSize > this->vMemSafetyLimit )
    {
        cerr << " System:: checkMemoryLimit(): memory size exceeding memory limit! \n" << flush;
        cerr << " VM size:"<< bytesToMB(this->virtualMemSize) << ",  limit: " <<  bytesToMB(this->vMemSafetyLimit) << " MB" << flush;
        cerr << " calling terminate() \n" << flush;
        terminate();
    }
}

long System:: getVmUsage()
{
    this->readMemoryUsage();
    return this->virtualMemSize;
}

long System:: getRmUsage()
{
    this->readMemoryUsage();
    return this->residentMemSize;
}

void System:: readMemoryUsage()
{
    this->virtualMemSize = 0;
    this->residentMemSize = 0;


    long size; 	  //total program size (same as VmSize in /proc/[pid]/status)
    long resident;  //resident set size (same as VmRSS in /proc/[pid]/status)
    long share;     //shared pages (from shared mappings)
    long text;      //text (code)
    long lib;       //library (unused in Linux 2.6)
    long data;      //data + stack
    long dt;        //dirty pages (unused in Linux 2.6)

    ifstream statm("/proc/self/statm");
    statm >> size >> resident >> share >> text >> lib >> data >> dt;
    statm.close();

    this->virtualMemSize = size * pagesize;
    this->residentMemSize = resident * pagesize;


    checkMemoryLimit();
}

void System:: printInfo()
{
    cout << endl;
    cout << " Stochastics v0.2" << endl;
    cout << " Compilation date: " << this->getCompilationDate() << endl;
    cout << " Available cores: " << this->getNumberOfCPUs() << endl;
//     cout << "System memory: " << sys.getSystemMemory() <<endl;
//     cout << "Program memory: " << sys.getMemoryUsage() <<endl;
    cout << " Memory page size: " << this->getPageSizeKB() << " kB"<<endl;

    this->printMemoryUsage();
    cout << "----------------------------------------"<<endl;
}

void System:: printMemoryUsage()
{
    cout << " Program memory size: " << this->getVmUsageMB() << " MB"<<endl;
}

int System::getNumberOfCPUs()
{
    int numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    return numCPU;
}


long unsigned int System::getSystemMemory()
{
    long unsigned int mem = 0;
//     struct sysinfo info;
//     int i = sysinfo(&info);
//     if (i==0) info.totalram;
//     else std::cout << "error "<<std::endl;
    return mem;
}


long int System::getMemoryUsage()
{
    long int mem = 0;
//     struct rusage usage;
//     int i = getrusage(RUSAGE_SELF, &usage);
//     if (i==0)  usage.ru_ixrss;
//     else std::cout << "error "<<std::endl;
    return mem;
}

long int System::getProcessUserTime()
{
    long int time = 0;
    struct rusage usage_data;
    if ( !getrusage ( RUSAGE_SELF, &usage_data ) )
    {
        time = usage_data.ru_utime.tv_sec;
    }
    return time;
}

long int System::getProcessSystemTime()
{
    long int time = 0;
    struct rusage usage_data;
    if ( !getrusage ( RUSAGE_SELF, &usage_data ) )
    {
        time = usage_data.ru_stime.tv_sec;
    }
    return time;
}


void System::waitForKey()
{
    std::cout << "Press enter to close."<<std::endl;
    std::cin.get();
}

void System:: start()
{
    this->start_time = ::time(0);
}

void System:: finish()
{
    this->finish_time = ::time(0);
}

void System:: printTimeSummary()
{
    cout << endl << endl << endl;
    cout << "----------------------------------------"<<endl;
    cout << " Time taken: " <<endl;
    cout << " Real time: " << ::difftime (this->finish_time, this->start_time) << " s" << endl;
    cout << " User time: " << this->getProcessUserTime() << " s" << endl;
    cout << " System time: " << this->getProcessSystemTime() << " s" << endl;
    cout << endl << endl;
}

long System:: getPageSize()
{
    return this->pagesize;
}

long System:: getPageSizeKB()
{
    return (this->pagesize/1024);
}


string System::calculateTime(time_t time_started, double percent_done)
{
    time_t current_time = ::time(0);
    double elapsed_time = difftime(current_time,time_started);
    double eta =  ((100.0/percent_done) * elapsed_time) - elapsed_time;

    ostringstream str;
    str <<  "elapsed: " << formatTime(elapsed_time) << "\t ETA: " << formatTime(eta);
    return str.str();
}

string System::formatTime( double seconds )
{
    int time = (int) seconds;
    ostringstream str;

    int s = time % 60;
    time = time - s;
    time = ( time/60 );

    int m = time % 60;

    time = time - m;

    int h = (time/60);
    if ( h < 0) h = 0;
    if (m<0) m = 0;
    if (s<0) s = 0;

    str << h <<"h ";
    str << m <<"m ";
    str << s <<"s ";

    return str.str();
}

double System:: bytesToMB(long bytes)
{
    double kBytes = (bytes/1024.0);
    double mBytes = (kBytes/1024.0);

    return mBytes;
}


void System:: saveArray( const char * fileName, Settings::resultType * array, size_t arraySize  )
{
    ofstream fout (fileName,ios_base::trunc);
    fout << setprecision(16);
    for (unsigned int j=0; j < arraySize; j++)
    {
        fout << array[j] <<"\n"; //<<flush;
    }
    fout << flush;
    fout.close();
}

void System:: saveArrays( const char * fileName, Settings::resultType * array1, Settings::resultType * array2, size_t arraysSize  )
{
    ofstream fout (fileName,ios_base::trunc);
    fout << setprecision(16);
    for (unsigned int j=0; j < arraysSize; j++)
    {
        fout << array1[j] << " " << array2[j] <<"\n"; //<<flush;
    }
    fout << flush;
    fout.close();
}


// register file to delete later
void System:: tmpFile(const char * f)
{
    this->tmpFile( string(f) );
}
void System:: tmpFile(string f) {
    if (this->filesToDelete==NULL) this->filesToDelete = new set<string>();
    this->filesToDelete->insert(f);
}
// delete all registered temp files
void System:: deleteTmpFiles() {
    cout << "System:: Deleting temporary files...";

    if (this->filesToDelete!=NULL)
    {
        set<string>::iterator it ;
        size_t setSize = this->filesToDelete->size();
        size_t deletedFiles = 0;
        for ( it=this->filesToDelete->begin() ; it != this->filesToDelete->end(); it++ )
        {
            if (remove( (*it).c_str() ) ==0) deletedFiles++;
        }
        cout << "ok (" << deletedFiles << "/" << setSize << ")";
    }
    else
    {
        cout << "nothing to delete.";
    }
    cout << endl;

}


void System:: signalHandler(int signum)
{
 cout << endl;
 //cout << "Caught signal " << signum << endl;
 //System& sys = System::getInstance();

 exit(signum);
}

/*
    if(bytes > 1024.0) {
    double kbytes = bytes/1024.0;
    cout << " (" << kbytes << " KB)";
    if(kbytes > 1024.0)
    {
    double mbytes = kbytes/1024.0;
    cout << " (" << mbytes << " MB)";
    if(mbytes > 1024.0)
      {
	double gbytes = mbytes/1024.0;
	cout << " (" << gbytes << " GB)";
	if(gbytes > 1024.0)
	  {
	    // i hope it wont get here :)
	    double tbytes = gbytes/1024.0;
	    cout << " (" << tbytes << " TB)";
	  }
      }
    }
  }*/
