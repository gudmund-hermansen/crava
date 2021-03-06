#ifndef RPLIB_DISTRIBUTIONS_SOLID_H
#define RPLIB_DISTRIBUTIONS_SOLID_H

#include "rplib/solid.h"

// Abstract class for holding all t = 0 distribution functions for solid parameters.
// One derived class for each solid model, the latter specified in a parallel, derived Solid class.
// The class must be able to produce an object of the specific Solid class.
class DistributionsSolid {
public:

                                DistributionsSolid(){}

  virtual                       ~DistributionsSolid(){}

  virtual DistributionsSolid  * Clone()                                                      const = 0;

  virtual Solid *               GenerateSample(const std::vector<double> & /*trend_params*/)       = 0;

  std::vector< Solid* >         GenerateWellSample(const  std::vector<double> & trend_params,
                                                   double                       corr);

  virtual bool                  HasDistribution()                                            const = 0;

  virtual std::vector<bool>     HasTrend()                                                   const = 0;

  Solid *                       EvolveSample(double         time,
                                             const Solid &  solid);

  virtual Solid *               UpdateSample(double                      corr_param,
                                             bool                        param_is_time,
                                             const std::vector<double> & trend,
                                             const Solid               * sample)                  = 0;

protected:
  std::vector< double >         alpha_;
};

#endif
