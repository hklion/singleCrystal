#ifndef singCrysReadFile_h
#define singCrysReadFile_h 1

#include "globals.hh"

class singCrysReadFile
{
  public:
    singCrysReadFile(const G4String);
    ~singCrysReadFile();
    G4int GetNEntries() {return nEntries;}
    G4double* GetEnergies() {return energies;}
    G4double* GetVals() {return values;}

  private:
    G4int nEntries;
    G4double* energies;
    G4double* values;
};

#endif
