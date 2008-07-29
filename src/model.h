#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>

#include "nrlib/surface/regularsurface.hpp"

#include "lib/global_def.h"
#include "src/definitions.h"
#include "src/background.h" //or move getAlpha & co to cpp-file.
#include "src/modelsettings.h"

struct irapgrid;
class Corr;
class ModelFile;
class Wavelet;
class Vario;
class Simbox;
class WellData;
class FFTGrid;
class RandomGen;

class Model{
public:
  Model(char * fileName);
  ~Model();

  ModelSettings  * getModelSettings()         const { return modelSettings_          ;}
  Simbox         * getTimeSimbox()            const { return timeSimbox_             ;}
  Simbox         * getDepthSimbox()           const { return depthSimbox_            ;}
  WellData      ** getWells()                 const { return wells_                  ;}
  FFTGrid        * getBackAlpha()             const { return background_->getAlpha() ;}
  FFTGrid        * getBackBeta()              const { return background_->getBeta()  ;}
  FFTGrid        * getBackRho()               const { return background_->getRho()   ;}
  Corr           * getPriorCorrelations()     const { return priorCorrelations_      ;}
  float          * getPriorFacies()           const { return priorFacies_            ;}  
  FFTGrid       ** getSeisCubes()             const { return seisCube_               ;}
  Wavelet       ** getWavelets()              const { return wavelet_                ;}
  float         ** getAMatrix()               const { return reflectionMatrix_       ;}
  RandomGen      * getRandomGen()             const { return randomGen_              ;} 
  bool             hasSignalToNoiseRatio()    const { return hasSignalToNoiseRatio_  ;}
  bool             getFailed()                const { return failed_                 ;}
  void             releaseWells();                                        // Deallocates well data.
  void             releaseGrids();                                        // Cuts connection to SeisCube_ and  backModel_
  Surface        * getCorrXYGrid();

private:
  void             makeTimeSimbox(Simbox        *& timeSimbox,
                                  ModelSettings *& modelSettings, 
                                  ModelFile      * modelFile,
                                  char           * errText,
                                  bool           & failed);
  void             makeDepthSimbox(Simbox       *& depthSimbox,
                                   ModelSettings * modelSettings, 
                                   ModelFile     * modelFile,
                                   char          * errText,
                                   bool          & failed);
  void             processSeismic(FFTGrid      **& seisCube,
                                  Simbox        *& timeSimbox,
                                  ModelSettings *& modelSettings, 
                                  ModelFile      * modelFile,
                                  char           * errText);
  void             processWells(WellData     **& wells,
                                Simbox         * timeSimbox,
                                RandomGen      * randomGen,
                                ModelSettings *& modelSettings, 
                                ModelFile      * modelFile,
                                char           * errText);
  void             processBackground(Background   *& background, 
                                     WellData     ** wells,
                                     Simbox        * timeSimbox,
                                     ModelSettings * modelSettings, 
                                     ModelFile     * modelFile, 
                                     char          * errText);
  void             processPriorCorrelations(Corr         *& priorCorrelations,
                                            Background    * background,
                                            WellData     ** wells,
                                            Simbox        * timeSimbox,
                                            ModelSettings * modelSettings, 
                                            ModelFile     * modelFile,
                                            char          * errText);
  void             processReflectionMatrix(float       **& reflectionMatrix,
                                           Background    * background,
                                           ModelSettings * modelSettings, 
                                           ModelFile     * modelFile,                  
                                           char          * errText);
  void             processWavelets(Wavelet     **& wavelet,
                                   FFTGrid      ** seisCube,
                                   WellData     ** wells,
                                   float        ** reflectionMatrix,
                                   Simbox        * timeSimbox,
                                   Surface      ** shiftGrids,
                                   Surface      ** gainGrids,
                                   ModelSettings * modelSettings, 
                                   ModelFile     * modelFile,
                                   bool            hasSignalToNoiseRatio);
  void             processPriorFaciesProb(float        *& priorFacies,
                                          WellData     ** wells,
                                          RandomGen     * randomGen,
                                          int             nz,
                                          ModelSettings * modelSettings);
  void             setSimboxSurfaces(Simbox    *& simbox, 
                                     char      ** surfFile, 
                                     bool         parallelSurfaces, 
                                     double       dTop, 
                                     double       lz, 
                                     double       dz, 
                                     int          nz, 
                                     int        & error);
  void             estimateXYPaddingSizes(Simbox         * timeSimbox,
                                          ModelSettings *& modelSettings);
  void             estimateZPaddingSize(Simbox         * timeSimbox,
                                        ModelSettings *& modelSettings);
  int              readSegyFiles(char          ** fNames, 
                                 int              nFiles, 
                                 FFTGrid       ** target, 
                                 Simbox        *& timeSimbox, 
                                 ModelSettings *& modelSettings,
                                 char           * errText, 
                                 int              gridType, 
                                 int              start = 0);
  int              readStormFile(char           * fName, 
                                 FFTGrid       *& target, 
                                 const char     * parName,
                                 Simbox         * timeSimbox,
                                 ModelSettings *& modelSettings, 
                                 char           * errText);
  void             estimateCorrXYFromSeismic(Surface *& CorrXY,
                                             FFTGrid ** seisCube,
                                             int        nAngles);
  int              setPaddingSize(int   nx, 
                                  float px);
  float         ** readMatrix(char       * fileName, 
                              int          n1, 
                              int          n2, 
                              const char * readReason, 
                              char       * errText);
  void             setupDefaultReflectionMatrix(float       **& reflectionMatrix,
                                                Background    * background,
                                                ModelSettings * modelSettings,
                                                ModelFile     * modelFile);
  int              checkFileOpen(char      ** fNames, 
                                 int          nFiles, 
                                 const char * command, 
                                 char       * errText, 
                                 int          start = 0,
                                 bool         details = true);
  void             checkAvailableMemory(Simbox        * timeSimbox,
                                        ModelSettings * modelSettings);
  void             checkFaciesNames(WellData      ** wells,
                                    ModelSettings *& modelSettings);
  void             printSettings(ModelSettings * modelSettings,
                                 ModelFile     * modelFile,
                                 bool            hasSignalToNoiseRatio);
  int              getWaveletFileFormat(char * fileName);

  ModelSettings  * modelSettings_;
  Simbox         * timeSimbox_;            // Information about simulation area.
  Simbox         * depthSimbox_;           // Simbox with depth surfaces
  WellData      ** wells_;                 // Well data
  Background     * background_;            // Holds the background model.
  Corr           * priorCorrelations_;     //
  FFTGrid       ** seisCube_;              // Seismic data cubes
  Wavelet       ** wavelet_;               // Wavelet for angle
  Surface       ** shiftGrids_;            // Grids containing shift data for wavelets
  Surface       ** gainGrids_;             // Grids containing gain data for wavelets.
  RandomGen      * randomGen_;             // Random generator.
  float          * priorFacies_;
  float         ** reflectionMatrix_;      // May specify own Zoeppritz-approximation. Default NULL,
                                           // indicating that standard approximation will be used.

  bool             hasSignalToNoiseRatio_; // Use SN ratio instead of error variance in model file. 
  bool             failed_;                // Indicates whether errors ocuured during construction. 
};

#endif

