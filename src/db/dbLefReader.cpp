#include <cassert>
#include <cstdio>

#include "dbLefReader.h"

namespace db
{

dbLefReader::dbLefReader(std::shared_ptr<dbTypes> types, 
                         std::shared_ptr<dbTech> tech)
  : types_          (types),
    tech_           ( tech),
    parse65nm_      (    0),
    parseLef58Type_ (    0)
{
}

void
dbLefReader::init()
{
  lefrInitSession(0);
  lefrReset();
  
  lefrSetUserData(tech_.get());

  lefrSetUnitsCbk(this->lefUnitsCbk);
  lefrSetLayerCbk(this->lefLayerCbk);
  lefrSetSiteCbk(this->lefSiteCbk);
  lefrSetMacroBeginCbk(this->lefStartCbk);
  lefrSetMacroCbk(this->lefMacroCbk);
  lefrSetPinCbk(this->lefMacroPinCbk);
  lefrSetObstructionCbk(this->lefMacroObsCbk);
  lefrSetMacroEndCbk(this->lefEndCbk);
}

void
dbLefReader::parseLef(const std::string& filename)
{
  init();

  FILE* file = fopen(filename.c_str(), "r");

  if(file == nullptr)
  {
    printf("Cannot open %s\n", filename.c_str());
    exit(1);
  }

  int res = lefrRead(file, filename.c_str(), (void*) tech_.get());
  fclose(file);

  lefrClear();

  if(res)
  {
    printf("Fail to parse %s\n", filename.c_str());
    exit(1);
  }
}

int
dbLefReader::lefUnitsCbk(lefrCallbackType_e c, lefiUnits* unit, lefiUserData ud)
{
  dbTech* tech = (dbTech*) ud;
  tech->setUnits(unit);
  return 0;
}

int 
dbLefReader::lefLayerCbk(lefrCallbackType_e c, lefiLayer* la, lefiUserData ud) 
{
  dbTech* tech = (dbTech*) ud;
  tech->createNewLayer(la);
  return 0;
}

int
dbLefReader::lefSiteCbk(lefrCallbackType_e c, lefiSite* si, lefiUserData ud ) 
{
  dbTech* tech = (dbTech*) ud;
  tech->createNewSite(si);
  return 0;
}

int 
dbLefReader::lefStartCbk(lefrCallbackType_e c, const char* name,  lefiUserData ud ) 
{
//  dbTech* tech = (dbTech*) ud;
//  switch(c) 
//  {
//    case lefrMacroBeginCbkType:
//      topMacro_ = tech->locateOrCreateMacro(name);
//      break;
//    default:
//      break;
//  }
  return 0;
}

int
dbLefReader::lefMacroCbk(lefrCallbackType_e c, lefiMacro* ma,  lefiUserData ud ) 
{
//  dbTech* tech = (dbTech*) ud;
//  if( ma->numProperties() > 0 ) 
//  {
//    for(int i=0; i<ma->numProperties(); i++) 
//    {
//      if( ma->propValue(i) ) 
//      {
////        cout << ma->propName(i) << " val: " << ma->propValue(i) << endl;
////        printf("%s val: %s\n", ma->propName(i),  ma->propValue(i));
//      }
//      else 
//      {
////        cout << ma->propName(i) << " num: " << ma->propNum(i) << endl;
//      }
//    }
//  }
//
//  if( ma->hasClass() ) 
//    topMacro_->type = ma->macroClass();
//
//  if( ma->hasOrigin() ) 
//  {
//    topMacro_->xOrig = ma -> originX();
//    topMacro_->yOrig = ma -> originY();
//  }
//
//  if( ma->hasSize() ) 
//  {
//    topMacro_->width = ma->sizeX();
//    topMacro_->height = ma->sizeY();
//  }
//
//  if( ma->hasSiteName() ) 
//  {
//    site* mySite = tech->locateOrCreateSite(ma->siteName());
//    topMacro_->sites.push_back( tech->site2id.find(mySite->name)->second );
//  }
  
  return 0;
}

int 
dbLefReader::lefMacroPinCbk(lefrCallbackType_e c, lefiPin* pi, lefiUserData ud ) 
{
//  dbTech* tech = (dbTech*) ud;
//
//  macro_pin myPin;
//
//  string pinName = pi->name();
//
//  if( pi->hasDirection() ) 
//    myPin.direction = pi->direction();
//
//  if( pi->hasShape() ) 
//    myPin.shape = pi->shape(); 
//
//  layer* curLayer = NULL;
//  for(int i=0; i<pi->numPorts(); i++) 
//  {
//    lefiGeometries* geom = pi->port(i);
//    lefiGeomRect* lrect = NULL;
//    lefiGeomPolygon* lpoly = NULL;
//    double polyLx = DBL_MAX, polyLy = DBL_MAX;
//    double polyUx = DBL_MIN, polyUy = DBL_MAX;
//
//    opendp::rect tmpRect;
//
//    for(int j=0; j<geom->numItems(); j++) 
//    {
//      switch(geom->itemType(j)) 
//      {
//        // when meets Layer .
//        case lefiGeomLayerE:
//          curLayer = tech->locateOrCreateLayer( geom->getLayer(j) );
//          break;
//        
//        // when meets Rect
//        case lefiGeomRectE:
//          lrect = geom->getRect(j);
//          tmpRect.xLL = lrect->xl;
//          tmpRect.yLL = lrect->yl;
//          tmpRect.xUR = lrect->xh;
//          tmpRect.yUR = lrect->yh;
//          myPin.port.push_back(tmpRect);
//          break;
//
//        // when meets Polygon 
//        case lefiGeomPolygonE:
//          lpoly = geom->getPolygon(j);
//
//          polyLx = DBL_MAX;
//          polyLy = DBL_MAX;
//          polyUx = DBL_MIN;
//          polyUy = DBL_MIN;
//
//          for(int k=0; k<lpoly->numPoints; k++) 
//          {
//            polyLx = min(polyLx, lpoly->x[k]); 
//            polyLy = min(polyLy, lpoly->y[k]);
//            polyUx = max(polyUx, lpoly->x[k]);
//            polyUy = max(polyUy, lpoly->y[k]);
//          }
//         
//          tmpRect.xLL = polyLx;
//          tmpRect.yLL = polyLy;
//          tmpRect.xUR = polyUx;
//          tmpRect.yUR = polyUy; 
//          myPin.port.push_back(tmpRect);
//
//        default:
//          break;
//      }
//    }
//  }
// 
//  topMacro_->pins[pinName] = myPin;
  return 0; 
}

// Set macro's Obs
int 
dbLefReader::lefMacroObsCbk(lefrCallbackType_e c, lefiObstruction* obs, lefiUserData ud ) 
{
//  dbTech* tech = (dbTech*) ud;
//  lefiGeometries* geom = obs->geometries();
//
//  bool isMeetMetalLayer1 = false;
//  for(int i=0; i<geom->numItems(); i++) 
//  {
//    lefiGeomRect* lrect = NULL;
//    opendp::rect tmpRect;
//
//    switch(geom->itemType(i)) 
//    {
//      // when meets metal1 segments.
//      case lefiGeomLayerE:
//        // HARD CODE
//        // Need to be replaced layer. (metal1 name)
//        isMeetMetalLayer1 =  (strcmp(geom->getLayer(i), "metal1") == 0)? true : false;
//      break;
//      // only metal1's obs should be pushed.
//      case lefiGeomRectE:
//        if(!isMeetMetalLayer1)
//          break;
//
//        lrect = geom->getRect(i);
//        tmpRect.xLL = lrect->xl;
//        tmpRect.yLL = lrect->yl;
//        tmpRect.xUR = lrect->xh;
//        tmpRect.yUR = lrect->yh;
//  
//        topMacro_->obses.push_back(tmpRect);
//        break;
//      default: 
//        break;    
//    }
//  }
//
////  cout << "obs: " << topMacro_->obses.size() << endl;
  return 0;
}

int 
dbLefReader::lefEndCbk(lefrCallbackType_e c, const char* name, lefiUserData ud ) 
{
//  dbTech* tech = (dbTech*) ud;
//  switch(c) 
//  {
//    case lefrMacroEndCbkType:
////      cout << "Macro: " << topMacro_->name << " is undergoing test" << endl;
//      tech->read_lef_macro_define_top_power(topMacro_);
//      // reset topMacro_'s pointer
//      topMacro_ = 0;
//      break;
//    default:
//      break;
//  }
  return 0;
}

}
