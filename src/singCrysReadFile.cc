/*!
 * \file singCrysReadFile.cc
 * \brief Implementation file for the singCrysReadFile class. Reads in data
 * files.
 */

#include "singCrysReadFile.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>

// Constructor: reads in file and puts data in appropriate fields
singCrysReadFile::singCrysReadFile(const G4String filename)
{
  // Open file
  std::ifstream inf(filename);
  // If cannot be opened, return error.
  if (!inf)
  {
    G4cerr << "Error: " << filename << " cannot be read. Aborting." << G4endl;
    exit(1);
  }
  // Otherwise, read in the number of entries.
  inf >> nEntries;
  // Declare the arrays 'energies' and 'values'
  energies = new G4double[nEntries];
  values = new G4double[nEntries];
  // Go through file, reading in wavelengths, converting them to energies,
  // and reading in the corresponding values.
  G4double wavelength;
  for (G4int i = 0; i < nEntries; i++)
  {
    inf >> wavelength;
    energies[i] = hbarc * twopi / (wavelength * nm);
    inf >> values[i];
  }
  inf.close();
}

// Destructor: deletes dynamically allocated arrays
singCrysReadFile::~singCrysReadFile()
{
  delete[] energies;
  delete[] values;
}
