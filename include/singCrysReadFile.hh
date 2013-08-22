/*!
 * \file singCrysReadFile.hh
 * \brief Header file for singCrysReadFile class. Reads in data files.
 */

#ifndef singCrysReadFile_h
#define singCrysReadFile_h 1

#include "globals.hh"

/*!
 * \class singCrysReadFile
 * \brief Reads in data files
 *
 * Reads in data files that are used for energy-dependent (often optical)
 * material and surface properties. The first line of each file is assumed
 * to be an integer denoting the number of data lines in the file. The
 * subsequent lines are all assumed to contain data. The left column
 * is assumed to be wavelength in nm, and the right column is the value at
 * that wavelength. The class contains arrays of the energies (converted
 * from the wavelengths), the values (in standard GEANT4 units), and the number
 * of entries.
 */

class singCrysReadFile
{
  public:
    //! Constructor
    /*!
     * Constructs an instance of the class and reads in the given file. The
     * first line should be an integer denoting the number of lines of data
     * contained in the file. The function will then read in twice that many
     * whtiespace-delimited entries from the file, assuming that the entries
     * alternate between wavelengths (in nm) and values (in GEANT4 units). The
     * data from the file are put in to the appropriate class fields.
     * \param filename File to be read
     */
    singCrysReadFile(const G4String filename);
    //! Destructor
    /*!
     * Deletes dynamically allocated arrays of energies and values.
     */
    ~singCrysReadFile();
    //! Accessor for number of entries in the file
    /*!
     * \return Number of entries
     */
    G4int GetNEntries() {return nEntries;}
    //! Accessor for the energies of the entries
    /*!
     * \return Array of energies
     */
    G4double* GetEnergies() {return energies;}
    //! Accessor for the values of the entries
    /*!
     * \return Array of values
     */
    G4double* GetVals() {return values;}

  private:
    //! Number of entries
    G4int nEntries;
    //! Energies of entries
    G4double* energies;
    //! Values of entries
    G4double* values;
};

#endif
