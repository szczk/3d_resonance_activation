#include "Datafile.hh"


Datafile::Datafile() : file(0), filename(0), singleValueSizeBytes(sizeof(double)), alreadyClosed(false), valuesCount(0), valuesRead(0)
{

}


Datafile::~Datafile()
{
    if(!alreadyClosed) this->close();
}



Datafile * Datafile::open(const char* filename)
{
    Datafile * d = new Datafile();
    d->openExisting(filename);
    return d;
}

Datafile * Datafile::create(const char* filename)
{
    Datafile * d = new Datafile() ;
    d->createNew(filename);
    return d;
}

void Datafile::setFilename(const char* filename)
{
    this->filename = strdup(filename);
}


void Datafile::createNew(const char* filename)
{
    this->setFilename(filename);
//     cout << "creating new file '" << filename << "'\n";
    this->file = new fstream(filename, ios_base::out | ios_base::trunc | ios_base::binary);

    

    this->okToRead = false;
    this->okToWrite = this->file->good();
    
    this->writeHeader();
}

void Datafile::openExisting(const char* filename)
{

    this->setFilename(filename);
//     cout << "opening existing file '" << filename << "'\n";
    this->file = new fstream(filename, ios_base::in | ios_base::binary);



    if( this->file->good() ) {
        this->okToWrite = false;
        this->okToRead =  this->readHeader();
    }
    else {
     cerr << "unable to open file '" << filename << "'\n"; 
    }
}



void Datafile::close()
{
//     cout << "closing file '" << this->filename <<"'\n";
    this->closeFile();
}

void Datafile::closeFile()
{
    if(!alreadyClosed && file!=0) {
        if(file->is_open()) {
	    file->flush();
            file->close();
            delete this->file;
	    alreadyClosed = true;
        }
    }
}


bool Datafile::ok()
{
    if(file!=0) {
        return ( file->is_open() && (!file->bad()));
    }
    else {
        return false;
    }
}


bool Datafile::readHeader()
{
   this->headerSizeBytes = 4;
      //get file sizeof
    this->file->seekg (0, ios::end);
    this->fileSizeBytes = this->file->tellg() - headerSizeBytes ;
    
    
    this->valuesCount = fileSizeBytes/singleValueSizeBytes;
    this->valuesRead = 0;

//     cout << "file size: " << fileSizeBytes <<  " b."<< " values stored: " << valuesCount <<endl;

    
    // przewin do poczatku,
    this->file->seekg (0, ios::beg);

    char head[4];
    this->file->read(head,4);

//     cout << "header: '" << head[0]<< head[1]<<head[2]<<head[3]<<"'... ";


    if(head[0]=='D' && head[0]=='D'  && head[0]=='D'  && head[0]=='D'  )
    {
// 	cout << " ok.\n";
        return true;
    }
    else {
	cerr << " not recognized. unable to read file!\n";
        return false;
    }
    
    

}

void Datafile::writeHeader()
{
    this->headerSizeBytes = 4;
    
    if(this->okToWrite) {
        // przewin do poczatku, tak na wszelki wypadek
        this->file->seekg (0, ios::beg);
        char head[] = {'D','A','T','8'};
        this->file->write(head,4);
	
	this->file->flush();
    }
}


double Datafile::read()
{
    double value = 0.0;
    if(this->okToRead) {
      this->file->read((char*) &value, sizeof(value));
      ++(this->valuesRead);
    }
    return value;
}


void Datafile::write(double value)
{
    if(this->okToWrite) {
	
	this->file->write( (char*) (&value), sizeof(value) );
    }
}


bool Datafile::hasNext()
{
  return ( valuesRead < valuesCount  );
}

