#ifndef singCrysUIsession_h
#define singCrysUIsession_h 1

#include "G4UIsession.hh"
#include <fstream>

using namespace std;

class singCrysUIsession : public G4UIsession
{
  public:
    singCrysUIsession();
    ~singCrysUIsession();
    virtual G4int ReceiveG4cout(const G4String& coutString);
    virtual G4int ReceiveG4cerr(const G4String& cerrString);
  private:
    ofstream logfile;
    ofstream errfile;
};
#endif
