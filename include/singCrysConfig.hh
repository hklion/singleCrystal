#ifndef singCrysConfig_h
#define singCrysConfig_h 1

class singCrysConfig {

  public:
    static singCrysConfig* GetInstance();
    static singCrysConfig* GetInstance(const G4String);

  protected:
    singCyrsConfig();
    singCrysConfig(const G4String);
    singCrysConfig(const singCrysConfig&);
    singCrysConfig& operator=(const singCrysConfig&);
    static singCrysConfig* pInstance;

};

#endif
