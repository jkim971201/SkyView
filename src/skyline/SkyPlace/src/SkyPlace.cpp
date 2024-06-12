#include <cstdio>
#include <iostream>
#include <chrono>

#include "SkyPlace.h"
#include "InitialPlacer.h"

namespace skyplace 
{


SkyPlace::SkyPlace()
  : param_             ( ),
    db_                (nullptr),
    painter_           (nullptr),
    initialPlacer_     (nullptr),
    wireLength_        (nullptr),
    ntOptimizer_       (nullptr),
    adOptimizer_       (nullptr),
    density_           (nullptr),
    dbTime_            (0      ),
    localLambdaMode_   (false  ),
    guiMode_           (false  ),
    plotMode_          (false  ),
    outputDir_         (""     ),
    isLegalized_       (false  )
{
  db_ = std::make_shared<SkyPlaceDB>();
}

void
SkyPlace::setDB(std::shared_ptr<dbDatabase> db)
{
  db_->setDB(db);
  makeSubTools();
}

void 
SkyPlace::makeSubTools()
{
  std::clock_t start = std::clock(); // Clock Start
  
  // Make InitialPlacer
  initialPlacer_ = std::make_unique<InitialPlacer>(db_);

  // Make Density Function
  density_ = std::make_shared<DensityGradient>(db_);

  density_->setLocalLambdaMode(localLambdaMode_);

  // Make WireLength Function
  wireLength_ = std::make_shared<WireLengthGradient>(db_);

  // Make Target Function
  targetFunc_ = std::make_shared<TargetFunction>(db_, wireLength_, density_, param_);

  legalizer_ = std::make_shared<Legalizer>(db_);

  std::clock_t end = std::clock(); // Clock End

  dbTime_ = (double)(end - start);
}

void 
SkyPlace::setPainter(std::shared_ptr<Painter> painter) 
{ 
  painter_ = painter; 
  painter_->setDB(db_);
}

void
SkyPlace::setInitMethod(const std::string& init_method)
{
  initialPlacer_->setInitMethod(init_method);
}

void
SkyPlace::setOptimizer(const std::string& opt_type)
{
  if(opt_type == "Adam" || opt_type == "adam")
    opt_ = OptimizerType::Adam;
  else if(opt_type == "Nesterov" || opt_type == "nesterov")
    opt_ = OptimizerType::Nesterov;
  else
  {
    std::cout << "Undefined Optimizer " << opt_type << std::endl;
    std::cout << "Argument is ignored and Nesterov will be used..." << std::endl;
  }
}

void
SkyPlace::setMacroWeight(float macroWeight)
{
  db_->updateMacroDensityWeight(macroWeight);
  targetFunc_->setMacroDensityWeight(true);
}

void
SkyPlace::doPlace()
{
  // Make Optimizer
  if(opt_ == OptimizerType::Adam)
    adOptimizer_ = std::make_unique<AdamOptimizer>(param_, db_, targetFunc_, painter_);
  else if(opt_ == OptimizerType::Nesterov)
    ntOptimizer_ =std::make_unique<NesterovOptimizer>(param_, db_, targetFunc_, painter_);

  printf("[SkyPlace] Local Lambda Mode : %s\n", (localLambdaMode_  ? "ON" : "OFF"));
  printf("[SkyPlace] GIF Plot Mode     : %s\n", (plotMode_         ? "ON" : "OFF"));
  printf("[SkyPlace] GUI Mode          : %s\n", (guiMode_          ? "ON" : "OFF"));
  printf("[SkyPlace] Start Placement!\n");

  // Step#1: Initial Placement
  doInitialPlace();

  auto t1 = std::chrono::high_resolution_clock::now();

  // Step#2: Main Placement Iteration
  Stat finalStat; 
  if(opt_ == OptimizerType::Nesterov)
    finalStat = ntOptimizer_->startOptimize(plotMode_);
  else if(opt_ == OptimizerType::Adam)
    finalStat = adOptimizer_->startOptimize(plotMode_);

  auto t2 = std::chrono::high_resolution_clock::now();

  std::chrono::duration<float> runtime = t2 - t1;

  totalTime_ = runtime.count();

  printf("[SkyPlace] Placement Finished!\n");

  // Finish : Print Statistic
  printStat(finalStat);

  // TODO : Fix mismatch between hpwl_ of Optimizer class and SkyPlaceDB
  db_->updateHpwl();
  //legalizer_->legalize();

//  if( db_->ifLefDef() )
//    db_->writeDef(outputDir_);       // Finish : Write results to .def file
//  else if( db_->ifBookShelf() ) 
//    db_->writeBookShelf(outputDir_); // Finish : Write results to .pl  file

  // GUI
  if(guiMode_)
    showFinalPlace();
}

void
SkyPlace::legalize()
{
  legalizer_->legalize();
  isLegalized_ = true;
}

void
SkyPlace::writeResult()
{
  if( db_->ifLefDef() )
    db_->writeDef(outputDir_, isLegalized_);       // Finish : Write results to .def file
  else if( db_->ifBookShelf() ) 
    db_->writeBookShelf(outputDir_, isLegalized_); // Finish : Write results to .pl  file
}

void
SkyPlace::doInitialPlace()
{
  printf("[SkyPlace] Start Initial Place.\n");
  initialPlacer_->doInitialPlace();
  printf("[SkyPlace] Finish Initial Place.\n");
}

void
SkyPlace::printStat(Stat finalStat)
{
  float dbTime            
    = static_cast<float>(dbTime_) / CLOCKS_PER_SEC;

  float ipTime 
    = static_cast<float>(initialPlacer_->getRuntime()) / CLOCKS_PER_SEC;

  float gpTime = 
    static_cast<float>(finalStat.totalPlaceTime) / CLOCKS_PER_SEC;

  float wlGradTime = 
    static_cast<float>(finalStat.wlGradTime) / CLOCKS_PER_SEC;

  float denGradTime = 
    static_cast<float>(finalStat.denGradTime) / CLOCKS_PER_SEC;

  float poissonTime = 
    static_cast<float>(finalStat.poissonTime) / CLOCKS_PER_SEC;

  float binDensityTime = 
    static_cast<float>(finalStat.binDensityTime) / CLOCKS_PER_SEC;

  float initTime = 
    static_cast<float>(finalStat.initTime) / CLOCKS_PER_SEC;

  //float totalTime
  //  = static_cast<float>(totalTime_) / CLOCKS_PER_SEC;
  
  float totalTime = totalTime_ + dbTime + ipTime;

  printf(" ==================================================\n");
  printf(" Final Statistic\n");
  printf(" ==================================================\n");
  printf("  | Benchmark      | %-10s    \n", db_->designName().c_str());
  if( db_->ifLefDef() )
    printf("  | HPWL           | %-10.1f um\n", finalStat.hpwl / db_->dbUnit());
  else
    printf("  | HPWL           | %-10.3f  \n", finalStat.hpwl / 1e6);
  printf("  | Overflow       | %-10.3f  \n", finalStat.overflow);
  printf("  | # Iteration    | %-10d    \n", finalStat.iter);
  printf("  | Time DB        | %-5.1f   \n", dbTime);
  printf("  | Time IP        | %-5.1f   \n", ipTime);
  printf("  | Time NonLinOpt | %-5.1f   \n", gpTime);
  printf("  | Time WL Grad   | %-5.1f (%3.1f%%) \n", wlGradTime    , wlGradTime    / gpTime * 100);
  printf("  | Time Den Grad  | %-5.1f (%3.1f%%) \n", denGradTime   , denGradTime   / gpTime * 100);
  printf("    - Time Poisson   | %-5.1f (%3.1f%%) \n", poissonTime   , poissonTime   / gpTime * 100);
  printf("    - Time BinDenUp  | %-5.1f (%3.1f%%) \n", binDensityTime, binDensityTime/ gpTime * 100);
  printf("  | Time OptInit   | %-5.1f (%3.1f%%) \n", initTime      , initTime      / gpTime * 100);
  printf("  | Time Total     | %-5.1f   \n", totalTime);
  printf("  | Converge?      | %-5d     \n", finalStat.converge);
  printf(" ==================================================\n");
}

void
SkyPlace::showFinalPlace()
{
  printf("[SkyPlace] Draw from SkyPlaceDB.\n");
  painter_->drawChip();
}

} // namespace SkyPlace
