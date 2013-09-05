/*!
 * \file singCrysConfig.hh
 * \brief Header file for the singCrysConfig class. Read and process
 * configuration file.
 */

#ifndef singCrysConfig_h
#define singCrysConfig_h 1

#include "globals.hh"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

/*!
 * \class singCrysConfig
 * \brief Singleton class for reading and processing configuration file options
 *
 * This class is a singleton. It should first be called with the LoadFile
 * method. It can then subsequently be called with the GetInstance method.
 * Calling LoadFile again with a different file name will do nothing. The
 * variables map is stored in vm. The map is accessed by the GetMap method.
 *
 * <H3>How to add a new option</H3>
 *
 * In the constructor for singCrysConfig, there is a list of all of the allowed
 * configuration file options. The information for each option is contained
 * within parentheses. There are generally three fields to fill in: option
 * name, option type and default value, and a descritpion of the option. The
 * option name is what the option is referred to in the configuration file. The
 * type can be any of the GEANT4 types, except for G4String. Instead of
 * G4String, use std::string and typecast the option value to a G4String when
 * retrieving the value. The specification of a default value is optional.
 *
 * To retrieve an option value in another class, first get the pointer to the
 * singleton instance of singCrysConfig, and then use the
 * singCrysConfig::GetMap() method to get the pointer to the variables_map
 * storing the option names and values. An example of how to retrieve the
 * values of a few options is shown below.
 *
 * \code
 * namespace po = boost::program_options;
 * po::variables_map config = *(singCrysConfig::GetInstance()->GetMap());
 * G4int crysNumSides = config["crysNumSides"].as<G4int>();
 * G4String crysMat = (G4String) config["crysMat"].as<std::string>();
 * \endcode
 */ 

class singCrysConfig {

  public:
    //! Returns a pointer to the singleton instance of the class.
    /*!
     * If the pointer to the class is not yet initialized, the class is
     * constructed. Otherwise, the pointer to the class is returned. However,
     * this function should only be called if LoadFile has already been called.
     * \return A pointer to this class
     */
    static singCrysConfig* GetInstance();
    //! Takes the input file name and returns the instance of the class
    /*!
     * Takes the input file name and updates the filename field. Then the
     * instance of the class is constructed and returned. This must be the
     * first class method called, and must be called exacly once.
     * \param inFilename The name of the configuration file to read
     * \return A pointer to this class
     */ 
    static singCrysConfig* LoadFile(const G4String inFilename);
    //! Accessor method for the map of configuration options
    /*!
     * Returns the pointer to the variables_map that the configuration options
     * are stored in.
     * \return A pointer to the variable map
     */
    po::variables_map* GetMap();

  protected:
    //! Constructor
    /*!
     * Builds the class. Reads in the file from filename and parses the options
     * using the boost::program_options library. The options are stored in a
     * variables_map options.  All possible configuration file options and
     * default values must be specified here.
     */
    singCrysConfig();
    singCrysConfig(const singCrysConfig&);
    singCrysConfig& operator=(const singCrysConfig&);
    //! Map that stores all configuration options
    po::variables_map vm;
    //! Name of the file from which the configuration options are read
    static G4String filename;
    //! Whether the class has been constructed
    static G4bool constructed;
};

#endif
