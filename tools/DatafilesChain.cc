#include "DatafilesChain.hh"


DatafilesChain::DatafilesChain ( const char * filename_template, int minNum, int maxNum ) : files ( nullptr ), alreadyClosed(0), totalCount ( 0 ), currentFile(0)
{

     namesTemplate = filename_template;
     this->startNum = minNum;
     this->endNum = maxNum;
     this->init();


}


DatafilesChain::~DatafilesChain()
{
     this->close();
}


void DatafilesChain::init()
{
     this->files = new vector<Datafile *>();
     this->discoverFiles();
}

void DatafilesChain::close()
{
     if(alreadyClosed) return;
     
     clog << "closing datafiles chain"<<endl;
     if ( this->files!=nullptr ) {
       clog<<"deleting"<<endl;
          vector<Datafile *>::iterator it;
          for ( it = this->files->begin(); it != this->files->end(); ++it ) {
               Datafile * datafile = ( *it );

               if(datafile!=nullptr) {
		  datafile->close();
//                   delete datafile;
	       }
	       else {
		//cout << "datafile null!"<<endl; 
	       }
          }
          clog << "delete files"<<endl;
          delete this->files;
	  this->files=nullptr;
     }
     clog << "done closing datafiles chain"<<endl;
     this->alreadyClosed=true;
}


void DatafilesChain::discoverFiles()
{
     clog << "Discovering files..."<<endl;



     for ( int N = this->startNum; N <= this->endNum; N++ ) {
          // replace all occurences
       
          size_t index = 0;

          char buf[5];

          sprintf ( buf, "%d",N );

          string filename ( this->namesTemplate );

          while ( true ) {
               /* Locate the substring to replace. */
               index = filename.find ( "{NUM}", index );
               if ( index == string::npos ) break;
               /* Make the replacement. */


               filename.replace ( index, 5, buf );
               /* Advance index forward so the next iteration doesn't pick it up as well. */
               index += 5;
          }

          cout << "file:" <<  filename << endl;

          Datafile * datfile = Datafile::open( filename.c_str());

          if ( datfile->ok() ) {
               this->files->push_back ( datfile );
               this->totalCount += datfile->getCount();
          }

     }

     clog << "Opened " << this->files->size() << " files. Total count: " << this->totalCount << endl;
}


int DatafilesChain::getCount()
{
  cout << "getCount from chain"<<endl;
     return this->totalCount;
}


bool DatafilesChain::ok() {
 return true; 
}

double DatafilesChain::read() {
  Datafile * file = this->files->at(this->currentFile);
  if(file->hasNext()) {
    return file->read(); 
  }
}

bool DatafilesChain::hasNext() {
  
  if(this->files->size()==0) return false;

  Datafile * file = this->files->at(this->currentFile);
  if(file->hasNext()) {
    return true; 
  }
  else {
    if(this->currentFile + 1 < this->files->size()) {
      // end of current file, but maybe next one
       this->currentFile++;
       return this->hasNext();
    }
    else {
      //end of files
     return false; 
    }
  }
}


void DatafilesChain::write ( double value ) {
  throw "only reading is supported";
}
