#ifndef CRAVATREND_H
#define CRAVATREND_H

#include <stdio.h>

#include "src/modelsettings.h"

class Simbox;
class InputFiles;
class FFTGrid;

class CravaTrend
{
public:

  CravaTrend();

  CravaTrend(Simbox                       * timeSimbox,
             Simbox                       * timeCutSimbox,
             ModelSettings                * modelSettings,
             bool                         & failed,
             std::string                  & errTxt,
             const InputFiles             * inputFiles);

  ~CravaTrend();

  std::vector<int>                             GetSizeTrendCubes() const;

  std::vector<double>                          GetTrendPosition(const int & i,
                                                                const int & j,
                                                                const int & k) const;

  const std::vector<std::vector<double> >    & GetTrendCubeSampling()          const   { return trend_cube_sampling_;}

private:
  void                                         writeToFile(const Simbox        * timeSimbox,
                                                           FFTGrid             * grid,
                                                           const std::string   & fileName,
                                                           const std::string   & sgriLabel);

  std::vector<NRLib::Grid<double> >    trend_cubes_;               ///< Trend cubes used in rock phyiscs prior model
  std::vector<std::vector<double> >    trend_cube_sampling_;       ///< Common sampling for all trends, corresponding to min/max-values of the trend cubes
  int                                  n_samples_;                 ///< Use default value 1000 for number of samples for trends
  int                                  n_trend_cubes_;

};

#endif
