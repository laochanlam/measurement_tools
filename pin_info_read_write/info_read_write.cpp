#include "pin.H"
#include <iostream>
#include <fstream>
#include <sys/time.h>

#define READ "read"
#define WRITE "write"

std::ofstream TraceFile;


KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "output.log", "specify trace file name");

int read_count = 0;
int write_count = 0;
struct timeval start,end;
double total_read_time = 0.0, total_write_time = 0.0;
int total_read = 0, total_write = 0;

VOID Arg1Before(CHAR * name, ADDRINT size)
{
    TraceFile << "[" << name << "]" << endl << std::dec <<  "size: " << size << " bytes" << endl;
    if (strcmp(name, READ) == 0) {
        total_read += size;
        read_count++;
    }
    if (strcmp(name, WRITE) == 0) {
        total_write += size;
        write_count++;
    }
    gettimeofday (&start, NULL);


}

VOID funcAfter(ADDRINT ret, CHAR * name)
{
    // end = clock();
    gettimeofday (&end, NULL);

    double cpu_time_used = (end.tv_sec - start.tv_sec) * 1e6; 
    cpu_time_used = (cpu_time_used + (end.tv_usec -  
                              start.tv_usec)) * 1e-6; 
  
    TraceFile << "time: " <<  cpu_time_used << endl;
    if (strcmp(name, READ) == 0) 
        total_read_time += cpu_time_used;
    if (strcmp(name, WRITE) == 0) 
        total_write_time += cpu_time_used;
}


VOID Image(IMG img, VOID *v)
{

    RTN readRtn = RTN_FindByName(img, READ);
    if (RTN_Valid(readRtn)) {
        RTN_Open(readRtn);
        RTN_InsertCall(readRtn, IPOINT_BEFORE, (AFUNPTR)Arg1Before,
                       IARG_ADDRINT, READ,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
                       IARG_END);
        RTN_InsertCall(readRtn, IPOINT_AFTER, (AFUNPTR)funcAfter,
                       IARG_FUNCRET_EXITPOINT_VALUE, IARG_ADDRINT, READ, IARG_END);

        RTN_Close(readRtn);
    }

    RTN writeRtn = RTN_FindByName(img, WRITE);
    if (RTN_Valid(writeRtn)) {
        RTN_Open(writeRtn);
        RTN_InsertCall(writeRtn, IPOINT_BEFORE, (AFUNPTR)Arg1Before,
                       IARG_ADDRINT, WRITE,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
                       IARG_END);
        RTN_InsertCall(writeRtn, IPOINT_AFTER, (AFUNPTR)funcAfter,
                       IARG_FUNCRET_EXITPOINT_VALUE, IARG_ADDRINT, WRITE, IARG_END);

        RTN_Close(writeRtn);
    }
}

VOID Fini(INT32 code, VOID *v)
{
    TraceFile << "#######################profile#######################" << endl;
    TraceFile << std::dec << "read " << read_count << " times.\n" << endl;
    TraceFile << std::dec << "write " << write_count <<  " times.\n" << endl; 
    TraceFile << "average read size: " <<  total_read/(float)read_count << " byte/time" << endl;
    TraceFile << "average write size: " << total_write/(float)write_count << " byte/time" << endl; 
    TraceFile << "average read time: " <<  total_read_time/(float)read_count << " s/time" << endl;
    TraceFile << "average write time: " <<  total_write_time/(float)write_count << " s/time" << endl; 
    TraceFile.close(); 
}

   
INT32 Usage()
{
    cerr << "This tool produces a trace of calls to malloc." << endl;
    return -1;
}

int main(int argc, char *argv[])
{

    PIN_InitSymbols();
    if( PIN_Init(argc,argv)) {
        return Usage();
    }

    TraceFile.open(KnobOutputFile.Value().c_str());
    TraceFile << hex;
    TraceFile.setf(ios::showbase);
    

    TraceFile << "#######################trace#######################" << endl;
    IMG_AddInstrumentFunction(Image, 0);
    PIN_AddFiniFunction(Fini, 0);

    PIN_StartProgram();
    
    return 0;
}
