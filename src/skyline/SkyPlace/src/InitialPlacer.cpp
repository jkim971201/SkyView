#include <stdio.h>
#include <vector>
#include <map>
#include <random>
#include <memory>

#include "InitialPlacer.h"
#include "PlacerDB.h"

namespace SkyPlace
{

InitialPlacer::InitialPlacer()
  : db_          (nullptr),
    numCluster_  (0),
    runtime_     (0),
    randomInit_  (true)
{}

InitialPlacer::InitialPlacer(std::shared_ptr<PlacerDB> db)
{
  db_ = db;

  // Hyper-parameters
  maxFanout_      =         200;
  maxBiCGIter_    =         150; // 150
  maxIter_        =          30; // 30 or 15
  minIter_        =           5; 
  netWeightScale_ =           1; // 800?
  minError_       =      1.0e-5;
  if(db_->ifLefDef() )
    minLength_    =        1500;
  else
    minLength_    =          25;
}

void
InitialPlacer::doInitialPlace()
{
  std::clock_t start = std::clock();

  if(randomInit_)
    doRandomInit();
  else
    doClusterQuadratic();

  std::clock_t end = std::clock();

  runtime_ = (double)(end - start);
}

void
InitialPlacer::setInitMethod(const std::string& init_method)
{
  using namespace std;
  if(init_method == "Random" || init_method == "random")
  {
    randomInit_ = true; 
    qpInit_ = false;
  }
  else if(init_method == "qp" || init_method == "quadratic")
  {
    randomInit_ = false; 
    qpInit_ = true;
  }
  else
  {
    cout << "[WARN] Undefined Initialization method " << init_method << endl;
    cout << "[WARN] Argument is ignored and Random init will be used...\n";
  }
}

void
InitialPlacer::doRandomInit()
{
  printf("[InitialPlacer] Place all movable cells in the center with Gaussian Noise.\n");

  // See DREAMPlace TCAD21 Paper for details
  // The authors argue that random center initiailization
  // does not degrade the placement quality
  float dieCx = db_->die()->cx();
  float dieCy = db_->die()->cy();

  float dieDx = db_->die()->dx();
  float dieDy = db_->die()->dy();

  float meanX = dieCx;  
  float meanY = dieCy;  

  float devX = dieDx * 0.05;
  float devY = dieDy * 0.05;

  std::default_random_engine gen;
  std::normal_distribution<float> gaussianNoiseX(meanX, devX);
  std::normal_distribution<float> gaussianNoiseY(meanY, devY);

  for(auto& cell : db_->movableCells())
  {
    float locX = gaussianNoiseX(gen);
    float locY = gaussianNoiseY(gen);

    //if(!cell->isFiller())
    cell->setCenterLocation(locX, locY);
  }

  for(auto& cell : db_->movableCells())
    db_->moveCellInsideLayout(cell);

  db_->updateHpwl();

  printf("[InitialPlacer] Initial HPWL: %6.1f\n",
                          db_->getHPWL() / 1e6);

  printf("[InitialPlacer] Finish Random Center Initialization.\n");
}

void                      // Default = false
InitialPlacer::placeCenter(bool include_filler)
{
  float dieCx = db_->die()->cx();
  float dieCy = db_->die()->cy();

  for(auto& cell : db_->movableCells())
  {
    if(!cell->isFiller() || include_filler)
      cell->setCenterLocation(dieCx, dieCy);
  }
}

void
InitialPlacer::updateLaplacian()
{
  db_->updatePinBound();

  int numMovable = db_->numMovable() - db_->numFiller();

  xM_.resize(numMovable);
  yM_.resize(numMovable);

  LMFxF_ = Eigen::MatrixXf::Constant(numMovable, 1, 0.0);
  LMFyF_ = Eigen::MatrixXf::Constant(numMovable, 1, 0.0);

  LxMM_.resize(numMovable, numMovable);
  LyMM_.resize(numMovable, numMovable);

  // setFromTripets() automatically
  // sum up duplicated triplets
  std::vector<Triplet> tripletVectorX;
  std::vector<Triplet> tripletVectorY;

  for(auto& net : db_->nets())
  {  
    if(net->deg() < 2 || net->deg() > maxFanout_) 
      continue;

    float wB2B = netWeightScale_ / (net->deg() - 1);

    // Select two pins of each net
    for(int p1 = 0; p1 < net->deg() - 1; p1++)
    {
      Pin* pin1 = net->pins()[p1];
      for(int p2 = p1 + 1; p2 < net->deg(); p2++)
      {
        Pin* pin2 = net->pins()[p2];

        int c1 = pin1->cell()->id();
        int c2 = pin2->cell()->id();

        if(pin1->cell() == pin2->cell())
          continue;
  
        // B2B Net Model X Coordinate
        if(pin1->isMinPinX() || pin1->isMaxPinX() ||
           pin2->isMinPinX() || pin2->isMaxPinX() ) 
        {
          float lengthX = std::abs(pin1->cx() - pin2->cx());
          float weightX = 0.0;

          if(lengthX > minLength_)
            weightX = wB2B / lengthX; 
          else 
            weightX = wB2B / minLength_;

          // Case 1 : p1: Movable <-> p2: Movable
          if(!pin1->cell()->isFixed() && !pin2->cell()->isFixed())
          {
            tripletVectorX.push_back(Triplet(c1, c1, weightX));
            tripletVectorX.push_back(Triplet(c2, c2, weightX));
            tripletVectorX.push_back(Triplet(c1, c2, -weightX));
            tripletVectorX.push_back(Triplet(c2, c1, -weightX));
          }

          // Case 2 : p1: Movable <-> p2: Fixed
          else if(!pin1->cell()->isFixed() && pin2->cell()->isFixed())
          {
            tripletVectorX.push_back(Triplet(c1, c1, weightX));
            LMFxF_(c1) += weightX * pin2->cell()->cx();
          }

          // Case 3 : p1: Fixed <-> p2: Movable
          else if(pin1->cell()->isFixed() && !pin2->cell()->isFixed())
          {
            tripletVectorX.push_back(Triplet(c2, c2, weightX));
            LMFxF_(c2) += weightX * pin1->cell()->cx();
          }
        }

        // B2B Net Model Y Coordinate
        if(pin1->isMinPinY() || pin1->isMaxPinY() ||
           pin2->isMinPinY() || pin2->isMaxPinY() ) 
        {
          float lengthY = std::abs(pin1->cy() - pin2->cy());
          float weightY = 0.0; 

          if(lengthY > minLength_)
            weightY = wB2B / lengthY; 
          else 
            weightY = wB2B / minLength_;

          // Case 1 : Movable <-> Movable
          if(!pin1->cell()->isFixed() && !pin2->cell()->isFixed())
          {
            tripletVectorY.push_back(Triplet(c1, c1, weightY));
            tripletVectorY.push_back(Triplet(c2, c2, weightY));
            tripletVectorY.push_back(Triplet(c1, c2, -weightY));
            tripletVectorY.push_back(Triplet(c2, c1, -weightY));
          }

          // Case 2 : Movable <-> Fixed
          else if(!pin1->cell()->isFixed() && pin2->cell()->isFixed())
          {
            tripletVectorY.push_back(Triplet(c1, c1, weightY));
            LMFyF_(c1) += weightY * pin2->cell()->cy();
          }

          // Case 3 : Fixed <-> Movable
          else if(pin1->cell()->isFixed() && !pin2->cell()->isFixed())
          {
            tripletVectorY.push_back(Triplet(c2, c2, weightY));
            LMFyF_(c2) += weightY * pin1->cell()->cy();
          }
        }
      }
    }
  }
  LxMM_.setFromTriplets(tripletVectorX.begin(), tripletVectorX.end());
  LyMM_.setFromTriplets(tripletVectorY.begin(), tripletVectorY.end());
}

void
InitialPlacer::updateCoordinates()
{
  for(auto& cell : db_->movableCells())
  {
    // only update movable cells
    if(!cell->isFiller())
    {
      cell->setCenterLocation(static_cast<int>(xM_(cell->id())), 
                              static_cast<int>(yM_(cell->id())));

      //std::cout << xM_(cell->id()) << std::endl;
      //std::cout << yM_(cell->id()) << std::endl;
    }
  }
}

}; // namespace SkyPlace
