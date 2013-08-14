// Class implementation for the singCrysConfig class. This class reads and
// stores the contents of a configuration file.

singCrysConfig* singCrysConfig::pInstance = NULL;

singCrysConfig::singCrysConfig()
{
  return singCrysConfig("config.txt");
}

singCrysConfig::singCrysConfig(const G4String filename)
{

}

singCrysconfig::singCrysConfig(const singCrysConfig& right)
{
}

singCrysConfig::singCrysConfig& operator=(const singCrysConfig& right)
{
}

singCrysConfig* singCrysConfig::GetInstance()
{
}

singCrysConfig* singCrysConfig::GetInstance(const G4String filename)
{

}
