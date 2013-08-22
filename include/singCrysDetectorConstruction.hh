//! Detector construction class

/*!
 * Mandatory user detector construction class. Builds a LYSO crystal with a
 * simplified silicon APD on one end. Hits on silicon chip in APD are recorded
 * using a SensitiveDetector in the APD epoxy layer.
 */

#ifndef singCrysDetectorConstruction_h
#define singCrysDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"

class G4VPhysicalVolume;
class G4UserLimits;

class singCrysDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    //! Constructor.
    singCrysDetectorConstruction();
    //! Destructor
    virtual ~singCrysDetectorConstruction();
    //! Constructs geometry
    /*!
      Function called by GEANT4 to construct the geometry. All geometry
      construction goes here. Defines and places crystal, APD, and wrapping
      geometry. Material properties are also defined. Detection is also set
      up with a SensitiveDetector.
      \return A pointer to the world physical volume
    */
    virtual G4VPhysicalVolume* Construct();
  
  private:
    //! Defines materials
    /*!
     * Defines materials that are not standard in GEANT4.
     */
    void DefineMaterials();
    //! Wrapper function that generates a properties table for any material
    /*!
     * Calls the appropriate properties table generator for the material
     * string passed to it. Returns an error and uses the table for vacuum
     * if the material string is not known.
     * \param material The material the table is generated for
     * \return The G4MaterialPropertiesTable for the material
     */
    G4MaterialPropertiesTable* generateTable(G4String material);
    //! Generates a material properties table for the crystal material
    /*!
     * Reads in and processes refractive index, scintillation spectrum and 
     * time constant (for both fast and slow components), Rayleigh scattering
     * length, absorption length, scintillation yield, resolution scale, and
     * yield ratio between fast and total scintillation intensity.
     * \return The G4MaterialPropertiesTable for the material
     */
    G4MaterialPropertiesTable* generateCrysTable();
    //! Generates a material properties table for an arbitrary material
    /*!
     * The table generated only has refractive index, which is constant with
     * respect to photon energy.
     * \param rindex Refractive index of the material
     * \return The G4MaterialPropertiesTable for the crystal material
     */
    G4MaterialPropertiesTable* generateRIndexTable(G4double rindex);
    //! Generates a material properties table for the silicon APD surface
    /*!
     * Returns a table with efficiency equal to the quantum efficiency of
     * the detector, and a reflectivity of zero for all photon energies.
     * \return The G4MaterialPropertiesTable for the silicon surface
     */
    G4MaterialPropertiesTable* generateSiSurfaceTable();
    //! Generates a material properties table for the ceramic APD casing
    /*!
     * Returns a table with efficiency of zero and constant reflectivity
     * with respect to photon energy.
     * \return The G4MaterialPropertiesTable for the ceramic
     */
    G4MaterialPropertiesTable* generateCeramicTable();
    //! Generates a material properties table for aluminum
    /*!
     * Returns a table with real and imaginary components of refractive index
     * \return The G4MaterialPropertiesTable for the aluminum
     */
    G4MaterialPropertiesTable* generateAlTable();
    //! Chooses a surface type for a skin surface
    /*!
     * If the material is silicon, aluminum, or ceramic, the surface
     * is dielectric_metal. Otherwise, it is dielectric_dielectric.
     * \param material The material string of the volume surrounded by the
     * skin surface
     * \return The G4SurfaceType appropriate for the surface
     * \sa surfaceType()
     */
    G4SurfaceType surfaceType(G4String material);
    //! Chooses a surface type for a border surface
    /*!
     * If one of the materials is silicon, aluminum, or ceramic, the surface
     * is dielectric_metal. Otherwise, it is dielectric_dielectric.
     * \param material1 The material string for the first volume on the border
     * \param materail2 The material string for the second volume on the border
     * \return The G4SurfaceType appropriate for the surface
     */
    G4SurfaceType surfaceType(G4String material1, G4String material2);
};

#endif
