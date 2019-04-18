#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "pin.H"

typedef struct {
    bool Taken;
    ADDRINT predTarget;
} BP_Info;

class BranchPredictor {
    map<ADDRINT, ADDRINT> TargetTable;
    map<ADDRINT, bool> TakenTable;
    

public:
    BP_Info GetPrediction(ADDRINT PC) {
        BP_Info tmp_info;

	if ( TargetTable.find(PC) == TargetTable.end() ) {
	    //not found
	    tmp_info.Taken = false;
	    tmp_info.predTarget = -1;
	    // not taken default
	} else {
	    tmp_info.Taken = TakenTable[PC];
	    tmp_info.predTarget = TargetTable[PC];
	}

	return tmp_info;
    }
    
    void Update(ADDRINT PC, bool BrTaken, ADDRINT targetPC) {
        // if not equal & exist before
//      if ( TargetTable[PC] != targetPC && TargetTable.find(PC) != TargetTable.end() )
//	    cout << "Target Changed" << endl;
	    TargetTable[PC] = targetPC;
	    TakenTable[PC] = BrTaken;
    }
    
    int GetSizeOfBP() {
        return TargetTable.size();
    }
};

BranchPredictor myBPU;

long long int DirectionMissCount = 0;
long long int TargetMissCount = 0;
long long int BranchCount = 0;

VOID ProcessBranch(ADDRINT PC, ADDRINT targetPC, bool BrTaken) {
    // cout << PC << "\t" << targetPC << "\t" << BrTaken << endl;
    
    BranchCount++;
    BP_Info pred = myBPU.GetPrediction(PC);
    if ( pred.Taken != BrTaken) {
        DirectionMissCount++;
    }
    if ( pred.predTarget != targetPC ) {
        TargetMissCount++;
    }
    myBPU.Update(PC, BrTaken, targetPC);
}
    
VOID Instruction(INS ins, VOID *v)
{
    if (INS_IsBranch(ins) && INS_HasFallThrough(ins))
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR) ProcessBranch, 
	IARG_ADDRINT, INS_Address(ins), 
	IARG_ADDRINT, INS_DirectBranchOrCallTargetAddress(ins),
	IARG_BRANCH_TAKEN, IARG_END);
}


VOID Fini(INT32 code, VOID *v)
{
    cout << "################################################" << endl;
    cout << "DirectionMissCount: " << DirectionMissCount << endl;
    cout << "TargetMissCount: " << TargetMissCount << endl;
    cout << "BranchCount: " << BranchCount << endl;
    cout << "Branch Direction Miss rate: " << (DirectionMissCount / (float)BranchCount) * 100 << "%" << endl;
    cout << "Branch Target Miss rate: " << (TargetMissCount / (float)BranchCount) * 100 << "%" << endl;
    cout << "################################################" << endl;
}

int main(int argc, char * argv[])
{
    PIN_Init(argc, argv);
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();
    
    return 0;
}
