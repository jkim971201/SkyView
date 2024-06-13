#pragma once

#include <memory>
#include <ctime>

#include "HyperParam.h"
#include "Painter.h"
#include "SkyPlaceDB.h"
#include "InitialPlacer.h"
#include "NesterovOptimizer.h"
#include "AdamOptimizer.h"

namespace skyplace 
{

enum class OptimizerType 
{
  Nesterov,
  Adam
};

using namespace db;

typedef struct Stat
{
  bool    converge;
  float   hpwl;
  float   overflow;
  int     iter;

  double  totalPlaceTime;
  double  wlGradTime;
  double  denGradTime;
  double  poissonTime;
  double  binDensityTime;
  double  initTime;
} Stat;

class NesterovOptimizer;
class AdamOptimizer;

class SkyPlace 
{
  public:

    SkyPlace();

    // APIs
    void doPlace();     // global_place

    // 1. Link Parser and Convert Parsed Data to SkyPlaceDB
    // 2. Make Initial Placer
    // 3. Construct TargetFunction f (WireLength & Density) 
    void setDB(std::shared_ptr<dbDatabase> db);   

    // Link Painter and Placer
    void setPainter(std::shared_ptr<Painter> painter);   

    void setGUIMode()         { guiMode_         = true; }
    void setPlotMode()        { plotMode_        = true; }
    void setLocalLambdaMode() { localLambdaMode_ = true; }
    void setInitMethod(const std::string& init_method);
    void setOptimizer(const std::string& opt_type);

    // TODO: Fix mismatch between HyperParam
    // Hyper-Parameter Setting command
    void setTargetOverflow  (float val) { param_.targetOverflow = val;       }
    void setInitLambda      (float val) { targetFunc_->setInitLambda(val);   }
    void setMaxPhiCoef      (float val) { targetFunc_->setMaxPhiCoef(val);   }
    void setRefHpwl         (float val) { targetFunc_->setRefHpwl(val);      }
    void setInitGammaInv    (float val) { targetFunc_->setInitGammaInv(val); }
    void setAdamAlpha       (float val) { param_.adam_alpha = val;           }
    void setAdamBeta1       (float val) { param_.adam_beta1 = val;           }
    void setAdamBeta2       (float val) { param_.adam_beta2 = val;           }
    void setMacroWeight     (float val);
    void setOutputDir       (const std::string& outputDir) { outputDir_ = outputDir; }

    // Though targetDensity is also a hyper-parameter,
    // this has to be initialized before SkyPlaceDB is initialized.
    void setTargetDensity (float density)  
    { 
      if(density > 1.00 || density <= 0.0 || density <= db_->util() )
      {
        std::cout << "Warning - Invalid target density." << std::endl;
        std::cout << "Given value will be ignored" << std::endl;
      }
      else
        db_->setTargetDensity(density);
    }

  private:

    // Hyper-Parameters
    HyperParam param_;

    void makeSubTools();
    // Make InitialPlacer / Density / WireLength / TargetFunction

    // Sub-Tools
    std::shared_ptr<SkyPlaceDB> db_;
    std::shared_ptr<Painter>    painter_;

    // For main Placement Stage
    std::unique_ptr<InitialPlacer>        initialPlacer_;
    std::shared_ptr<DensityGradient>      density_;
    std::shared_ptr<WireLengthGradient>   wireLength_;
    std::shared_ptr<TargetFunction>       targetFunc_;
    std::unique_ptr<NesterovOptimizer>    ntOptimizer_;
    std::unique_ptr<AdamOptimizer>        adOptimizer_;

    void doInitialPlace();

    void showFinalPlace();

    // Configuration
    OptimizerType opt_;

    // ETC
    void printStat(Stat finalStat);
    bool guiMode_;
    bool plotMode_;
    bool localLambdaMode_;

    std::string outputDir_;

    // RunTime
    double dbTime_;
    double totalTime_;
};

} // namespace skyplace 
