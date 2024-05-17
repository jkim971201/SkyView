#include <cstdio>
#include <iostream>
#include <filesystem>

#include "lef/lefrReader.hpp"
#include "lef/lefwWriter.hpp"
#include "lef/lefiDebug.hpp"
#include "lef/lefiEncryptInt.hpp"
#include "lef/lefiUtil.hpp"

#include "db/dbDatabase.h"

int parse65nm = 0;
int parseLef58Type = 0;

void dataError() 
{
  fprintf(stdout, "ERROR: returned user data is not correct!\n");
}

void checkType(lefrCallbackType_e c) 
{
  if(c >= 0 && c <= lefrLibraryEndCbkType) 
  {
  // OK
  } 
  else 
    fprintf(stdout, "ERROR: callback type is out of bounds!\n");
}

char* orientStr(int orient) 
{
  switch (orient) 
  {
    case 0: return ((char*)"N");
    case 1: return ((char*)"W");
    case 2: return ((char*)"S");
    case 3: return ((char*)"E");
    case 4: return ((char*)"FN");
    case 5: return ((char*)"FW");
    case 6: return ((char*)"FS");
    case 7: return ((char*)"FE");
  };
  return ((char*)"BOGUS");
}

void lefVia(lefiVia *via) 
{
}

void lefSpacing(lefiSpacing* spacing) 
{
}

void lefViaRuleLayer(lefiViaRuleLayer* vLayer) 
{
}

int antennaCB(lefrCallbackType_e c, double value, lefiUserData) 
{
  checkType(c);
  return 0;
}

int arrayBeginCB(lefrCallbackType_e c, const char* name, lefiUserData) 
{
  checkType(c);
  return 0;
}

int arrayCB(lefrCallbackType_e c, lefiArray* a, lefiUserData) 
{
  checkType(c);
  return 0;
}

int arrayEndCB(lefrCallbackType_e c, const char* name, lefiUserData) 
{
  checkType(c);
  return 0;
}

int busBitCharsCB(lefrCallbackType_e c, const char* busBit, lefiUserData)
{
  checkType(c);
  int status = lefwBusBitChars(busBit);
  if(status != LEFW_OK)
    dataError();
  return 0;
}

int caseSensCB(lefrCallbackType_e c, int caseSense, lefiUserData) 
{
  checkType(c);
  return 0;
}

int fixedMaskCB(lefrCallbackType_e c, int fixedMask, lefiUserData) 
{
  checkType(c);
  return 0;
}

int clearanceCB(lefrCallbackType_e c, const char* name, lefiUserData) 
{
  checkType(c);
  return 0;
}

int dividerCB(lefrCallbackType_e c, const char* name, lefiUserData) 
{
  checkType(c);
  return 0;
}

int noWireExtCB(lefrCallbackType_e c, const char* name, lefiUserData) 
{
  checkType(c);
  return 0;
}

int noiseMarCB(lefrCallbackType_e c, lefiNoiseMargin *, lefiUserData) 
{
  checkType(c);
  return 0;
}

int edge1CB(lefrCallbackType_e c, double name, lefiUserData) 
{
  checkType(c);
  return 0;
}

int edge2CB(lefrCallbackType_e c, double name, lefiUserData) 
{
  checkType(c);
  return 0;
}

int edgeScaleCB(lefrCallbackType_e c, double name, lefiUserData) 
{
  checkType(c);
  return 0;
}

int noiseTableCB(lefrCallbackType_e c, lefiNoiseTable *, lefiUserData) 
{
  checkType(c);
  return 0;
}

int correctionCB(lefrCallbackType_e c, lefiCorrectionTable *, lefiUserData) 
{
  checkType(c);
  return 0;
}

int dielectricCB(lefrCallbackType_e c, double dielectric, lefiUserData) 
{
  checkType(c);
  return 0;
}

int irdropBeginCB(lefrCallbackType_e c, void*, lefiUserData)
{
  checkType(c);
  return 0;
}

int irdropCB(lefrCallbackType_e c, lefiIRDrop* irdrop, lefiUserData) 
{
  int i;
  checkType(c);
  //for (i = 0; i < irdrop->lefiIRDrop::numValues(); i++) 
  //  fprintf(stdout, "%g %g ", irdrop->lefiIRDrop::value1(i), 
	//  irdrop->lefiIRDrop::value2(i));
  return 0;
}

int irdropEndCB(lefrCallbackType_e c, void*, lefiUserData)
{
  checkType(c);
  return 0;
}

int layerCB(lefrCallbackType_e c, lefiLayer* layer, lefiUserData) 
{
  int i, j, k;
  int numPoints, propNum;
  double *widths, *current;
  lefiLayerDensity* density;
  lefiAntennaPWL* pwl;
  lefiSpacingTable* spTable;
  lefiInfluence* influence;
  lefiParallel* parallel;
  lefiTwoWidths* twoWidths;
  char pType;
  int numMinCut, numMinenclosed;
  lefiAntennaModel* aModel;
  lefiOrthogonal*   ortho;

  checkType(c);

  lefrSetCaseSensitivity(0);

  // Call parse65nmRules for 5.7 syntax in 5.6
  if(parse65nm)
    layer->lefiLayer::parse65nmRules();

  // Call parseLef58Type for 5.8 syntax in 5.7
  if (parseLef58Type)
    layer->lefiLayer::parseLEF58Layer();

  fprintf(stdout, "LAYER %s\n", layer->lefiLayer::name());
  if(layer->lefiLayer::hasType())
    fprintf(stdout, "  TYPE %s ;\n", layer->lefiLayer::type());
  if(layer->lefiLayer::hasLayerType())
    fprintf(stdout, "  LAYER TYPE %s ;\n", layer->lefiLayer::layerType());
  if(layer->lefiLayer::hasMask())
    fprintf(stdout, "  MASK %d ;\n", layer->lefiLayer::mask());

  if(layer->lefiLayer::hasPitch())
    fprintf(stdout, "  PITCH %g ;\n", layer->lefiLayer::pitch());
  else if(layer->lefiLayer::hasXYPitch())
    fprintf(stdout, "  PITCH %g %g ;\n", layer->lefiLayer::pitchX(), layer->lefiLayer::pitchY());

  if(layer->lefiLayer::hasOffset())
    fprintf(stdout, "  OFFSET %g ;\n", layer->lefiLayer::offset());
  else if(layer->lefiLayer::hasXYOffset())
    fprintf(stdout, "  OFFSET %g %g ;\n", layer->lefiLayer::offsetX(), layer->lefiLayer::offsetY());

  if(layer->lefiLayer::hasDiagPitch())
    fprintf(stdout, "  DIAGPITCH %g ;\n", layer->lefiLayer::diagPitch());
  else if(layer->lefiLayer::hasXYDiagPitch())
    fprintf(stdout, "  DIAGPITCH %g %g ;\n", layer->lefiLayer::diagPitchX(), layer->lefiLayer::diagPitchY());

  if(layer->lefiLayer::hasDiagWidth())
    fprintf(stdout, "  DIAGWIDTH %g ;\n", layer->lefiLayer::diagWidth());
  if(layer->lefiLayer::hasDiagSpacing())
    fprintf(stdout, "  DIAGSPACING %g ;\n", layer->lefiLayer::diagSpacing());
  if(layer->lefiLayer::hasWidth())
    fprintf(stdout, "  WIDTH %g ;\n", layer->lefiLayer::width());
  if(layer->lefiLayer::hasArea())
    fprintf(stdout, "  AREA %g ;\n", layer->lefiLayer::area());
  if(layer->lefiLayer::hasSlotWireWidth())
    fprintf(stdout, "  SLOTWIREWIDTH %g ;\n", layer->lefiLayer::slotWireWidth());
  if(layer->lefiLayer::hasSlotWireLength())
    fprintf(stdout, "  SLOTWIRELENGTH %g ;\n", layer->lefiLayer::slotWireLength());
  if(layer->lefiLayer::hasSlotWidth())
    fprintf(stdout, "  SLOTWIDTH %g ;\n", layer->lefiLayer::slotWidth());
  if(layer->lefiLayer::hasSlotLength())
    fprintf(stdout, "  SLOTLENGTH %g ;\n", layer->lefiLayer::slotLength());
  if(layer->lefiLayer::hasMaxAdjacentSlotSpacing())
    fprintf(stdout, "  MAXADJACENTSLOTSPACING %g ;\n", layer->lefiLayer::maxAdjacentSlotSpacing());
  if(layer->lefiLayer::hasMaxCoaxialSlotSpacing())
    fprintf(stdout, "  MAXCOAXIALSLOTSPACING %g ;\n", layer->lefiLayer::maxCoaxialSlotSpacing());
  if(layer->lefiLayer::hasMaxEdgeSlotSpacing())
    fprintf(stdout, "  MAXEDGESLOTSPACING %g ;\n", layer->lefiLayer::maxEdgeSlotSpacing());
  if(layer->lefiLayer::hasMaxFloatingArea()) // 5.7
    fprintf(stdout, "  MAXFLOATINGAREA %g ;\n", layer->lefiLayer::maxFloatingArea());
  if(layer->lefiLayer::hasArraySpacing()) 
	{       
		// 5.7
    fprintf(stdout, "  ARRAYSPACING ");
    if(layer->lefiLayer::hasLongArray())
      fprintf(stdout, "LONGARRAY ");
    if(layer->lefiLayer::hasViaWidth())
      fprintf(stdout, "WIDTH %g ", layer->lefiLayer::viaWidth());
    fprintf(stdout, "CUTSPACING %g", layer->lefiLayer::cutSpacing());
    for(i = 0; i < layer->lefiLayer::numArrayCuts(); i++) 
      fprintf(stdout, "\n\tARRAYCUTS %d SPACING %g",
          layer->lefiLayer::arrayCuts(i),
          layer->lefiLayer::arraySpacing(i));
    fprintf(stdout, " ;\n");
  }
  if (layer->lefiLayer::hasSplitWireWidth())
    fprintf(stdout, "  SPLITWIREWIDTH %g ;\n",
        layer->lefiLayer::splitWireWidth());
  if (layer->lefiLayer::hasMinimumDensity())
    fprintf(stdout, "  MINIMUMDENSITY %g ;\n",
        layer->lefiLayer::minimumDensity());
  if (layer->lefiLayer::hasMaximumDensity())
    fprintf(stdout, "  MAXIMUMDENSITY %g ;\n",
        layer->lefiLayer::maximumDensity());
  if (layer->lefiLayer::hasDensityCheckWindow())
    fprintf(stdout, "  DENSITYCHECKWINDOW %g %g ;\n",
        layer->lefiLayer::densityCheckWindowLength(),
        layer->lefiLayer::densityCheckWindowWidth());
  if (layer->lefiLayer::hasDensityCheckStep())
    fprintf(stdout, "  DENSITYCHECKSTEP %g ;\n",
        layer->lefiLayer::densityCheckStep());
  if (layer->lefiLayer::hasFillActiveSpacing())
    fprintf(stdout, "  FILLACTIVESPACING %g ;\n",
        layer->lefiLayer::fillActiveSpacing());
  // 5.4.1
  numMinCut = layer->lefiLayer::numMinimumcut();
  if (numMinCut > 0) {
    for (i = 0; i < numMinCut; i++) {
      fprintf(stdout, "  MINIMUMCUT %d WIDTH %g ",
          layer->lefiLayer::minimumcut(i),
          layer->lefiLayer::minimumcutWidth(i));
      if (layer->lefiLayer::hasMinimumcutWithin(i))
        fprintf(stdout, "WITHIN %g ", layer->lefiLayer::minimumcutWithin(i));
      if (layer->lefiLayer::hasMinimumcutConnection(i))
        fprintf(stdout, "%s ", layer->lefiLayer::minimumcutConnection(i));
      if (layer->lefiLayer::hasMinimumcutNumCuts(i))
        fprintf(stdout, "LENGTH %g WITHIN %g ",
            layer->lefiLayer::minimumcutLength(i),
            layer->lefiLayer::minimumcutDistance(i));
      fprintf(stdout, ";\n");
    }
  }
  // 5.4.1
  if (layer->lefiLayer::hasMaxwidth()) {
    fprintf(stdout, "  MAXWIDTH %g ;\n", layer->lefiLayer::maxwidth());
  }
  // 5.5
  if (layer->lefiLayer::hasMinwidth()) {
    fprintf(stdout, "  MINWIDTH %g ;\n", layer->lefiLayer::minwidth());
  }
  // 5.5
  numMinenclosed = layer->lefiLayer::numMinenclosedarea();
  if (numMinenclosed > 0) {
    for (i = 0; i < numMinenclosed; i++) {
      fprintf(stdout, "  MINENCLOSEDAREA %g ",
          layer->lefiLayer::minenclosedarea(i));
      if (layer->lefiLayer::hasMinenclosedareaWidth(i))
        fprintf(stdout, "MINENCLOSEDAREAWIDTH %g ",
            layer->lefiLayer::minenclosedareaWidth(i));
      fprintf (stdout, ";\n"); 
    }
  }
  // 5.4.1 & 5.6
  if (layer->lefiLayer::hasMinstep()) {
    for (i = 0; i < layer->lefiLayer::numMinstep(); i++) {
      fprintf(stdout, "  MINSTEP %g ", layer->lefiLayer::minstep(i));
      if (layer->lefiLayer::hasMinstepType(i))
        fprintf(stdout, "%s ", layer->lefiLayer::minstepType(i));
      if (layer->lefiLayer::hasMinstepLengthsum(i))
        fprintf(stdout, "LENGTHSUM %g ",
            layer->lefiLayer::minstepLengthsum(i));
      if (layer->lefiLayer::hasMinstepMaxedges(i))
        fprintf(stdout, "MAXEDGES %d ", layer->lefiLayer::minstepMaxedges(i));
      if (layer->lefiLayer::hasMinstepMinAdjLength(i))
        fprintf(stdout, "MINADJLENGTH %g ", layer->lefiLayer::minstepMinAdjLength(i));
      if (layer->lefiLayer::hasMinstepMinBetLength(i))
        fprintf(stdout, "MINBETLENGTH %g ", layer->lefiLayer::minstepMinBetLength(i));
      if (layer->lefiLayer::hasMinstepXSameCorners(i))
        fprintf(stdout, "XSAMECORNERS");
      fprintf(stdout, ";\n");
    }
  }
  // 5.4.1
  if (layer->lefiLayer::hasProtrusion()) {
    fprintf(stdout, "  PROTRUSIONWIDTH %g LENGTH %g WIDTH %g ;\n",
        layer->lefiLayer::protrusionWidth1(),
        layer->lefiLayer::protrusionLength(),
        layer->lefiLayer::protrusionWidth2());
  } 
  if (layer->lefiLayer::hasSpacingNumber()) {
    for (i = 0; i < layer->lefiLayer::numSpacing(); i++) {
      fprintf(stdout, "  SPACING %g ", layer->lefiLayer::spacing(i));
      if (layer->lefiLayer::hasSpacingName(i))
        fprintf(stdout, "LAYER %s ", layer->lefiLayer::spacingName(i));
      if (layer->lefiLayer::hasSpacingLayerStack(i))
        fprintf(stdout, "STACK ");               // 5.7
      if (layer->lefiLayer::hasSpacingAdjacent(i))
        fprintf(stdout, "ADJACENTCUTS %d WITHIN %g ",
            layer->lefiLayer::spacingAdjacentCuts(i),
            layer->lefiLayer::spacingAdjacentWithin(i));
      if (layer->lefiLayer::hasSpacingAdjacentExcept(i))  // 5.7
        fprintf(stdout, "EXCEPTSAMEPGNET "); 
      if (layer->lefiLayer::hasSpacingCenterToCenter(i))
        fprintf(stdout, "CENTERTOCENTER ");
      if (layer->lefiLayer::hasSpacingSamenet(i))       // 5.7
        fprintf(stdout, "SAMENET ");
      if (layer->lefiLayer::hasSpacingSamenetPGonly(i)) // 5.7
        fprintf(stdout, "PGONLY ");
      if (layer->lefiLayer::hasSpacingArea(i))        // 5.7
        fprintf(stdout, "AREA %g ", layer->lefiLayer::spacingArea(i));
      if (layer->lefiLayer::hasSpacingRange(i)) {
        fprintf(stdout, "RANGE %g %g ", layer->lefiLayer::spacingRangeMin(i),
            layer->lefiLayer::spacingRangeMax(i));
        if (layer->lefiLayer::hasSpacingRangeUseLengthThreshold(i))
          fprintf(stdout, "USELENGTHTHRESHOLD "); 
        else if (layer->lefiLayer::hasSpacingRangeInfluence(i)) {
          fprintf(stdout, "INFLUENCE %g ",
              layer->lefiLayer::spacingRangeInfluence(i));
          if (layer->lefiLayer::hasSpacingRangeInfluenceRange(i))
            fprintf(stdout, "RANGE %g %g ",
                layer->lefiLayer::spacingRangeInfluenceMin(i),
                layer->lefiLayer::spacingRangeInfluenceMax(i));
        } else if (layer->lefiLayer::hasSpacingRangeRange(i))
          fprintf(stdout, "RANGE %g %g ",
              layer->lefiLayer::spacingRangeRangeMin(i),
              layer->lefiLayer::spacingRangeRangeMax(i));
      } else if (layer->lefiLayer::hasSpacingLengthThreshold(i)) {
        fprintf(stdout, "LENGTHTHRESHOLD %g ",
            layer->lefiLayer::spacingLengthThreshold(i));
        if (layer->lefiLayer::hasSpacingLengthThresholdRange(i))
          fprintf(stdout, "RANGE %g %g",
              layer->lefiLayer::spacingLengthThresholdRangeMin(i),
              layer->lefiLayer::spacingLengthThresholdRangeMax(i));
      } else if (layer->lefiLayer::hasSpacingNotchLength(i)) {// 5.7
        fprintf(stdout, "NOTCHLENGTH %g",
            layer->lefiLayer::spacingNotchLength(i));
      } else if (layer->lefiLayer::hasSpacingEndOfNotchWidth(i)) // 5.7
        fprintf(stdout, "ENDOFNOTCHWIDTH %g NOTCHSPACING %g, NOTCHLENGTH %g",
            layer->lefiLayer::spacingEndOfNotchWidth(i),
            layer->lefiLayer::spacingEndOfNotchSpacing(i),
            layer->lefiLayer::spacingEndOfNotchLength(i));

      if (layer->lefiLayer::hasSpacingParallelOverlap(i))   // 5.7
        fprintf(stdout, "PARALLELOVERLAP "); 
      if (layer->lefiLayer::hasSpacingEndOfLine(i)) {     // 5.7
        fprintf(stdout, "ENDOFLINE %g WITHIN %g ",
            layer->lefiLayer::spacingEolWidth(i),
            layer->lefiLayer::spacingEolWithin(i));
        if (layer->lefiLayer::hasSpacingParellelEdge(i)) {
          fprintf(stdout, "PARALLELEDGE %g WITHIN %g ",
              layer->lefiLayer::spacingParSpace(i),
              layer->lefiLayer::spacingParWithin(i));
          if (layer->lefiLayer::hasSpacingTwoEdges(i)) {
            fprintf(stdout, "TWOEDGES ");
          }
        }
      }
      fprintf(stdout, ";\n");
    }
  }
  if (layer->lefiLayer::hasSpacingTableOrtho()) {      // 5.7
    fprintf(stdout, "SPACINGTABLE ORTHOGONAL"); 
    ortho = layer->lefiLayer::orthogonal();
    for (i = 0; i < ortho->lefiOrthogonal::numOrthogonal(); i++) {
      fprintf(stdout, "\n   WITHIN %g SPACING %g",
          ortho->lefiOrthogonal::cutWithin(i),
          ortho->lefiOrthogonal::orthoSpacing(i));
    }
    fprintf(stdout, ";\n");
  }
  for (i = 0; i < layer->lefiLayer::numEnclosure(); i++) {
    fprintf(stdout, "ENCLOSURE ");
    if (layer->lefiLayer::hasEnclosureRule(i))
      fprintf(stdout, "%s ", layer->lefiLayer::enclosureRule(i));
    fprintf(stdout, "%g %g ", layer->lefiLayer::enclosureOverhang1(i),
        layer->lefiLayer::enclosureOverhang2(i));
    if (layer->lefiLayer::hasEnclosureWidth(i))
      fprintf(stdout, "WIDTH %g ", layer->lefiLayer::enclosureMinWidth(i));
    if (layer->lefiLayer::hasEnclosureExceptExtraCut(i))
      fprintf(stdout, "EXCEPTEXTRACUT %g ",
          layer->lefiLayer::enclosureExceptExtraCut(i));
    if (layer->lefiLayer::hasEnclosureMinLength(i))
      fprintf(stdout, "LENGTH %g ", layer->lefiLayer::enclosureMinLength(i));
    fprintf(stdout, ";\n");
  }
  for (i = 0; i < layer->lefiLayer::numPreferEnclosure(); i++) {
    fprintf(stdout, "PREFERENCLOSURE ");
    if (layer->lefiLayer::hasPreferEnclosureRule(i))
      fprintf(stdout, "%s ", layer->lefiLayer::preferEnclosureRule(i));
    fprintf(stdout, "%g %g ", layer->lefiLayer::preferEnclosureOverhang1(i),
        layer->lefiLayer::preferEnclosureOverhang2(i));
    if (layer->lefiLayer::hasPreferEnclosureWidth(i))
      fprintf(stdout, "WIDTH %g ",layer->lefiLayer::preferEnclosureMinWidth(i));
    fprintf(stdout, ";\n");
  }
  if (layer->lefiLayer::hasResistancePerCut())
    fprintf(stdout, "  RESISTANCE %g ;\n",
        layer->lefiLayer::resistancePerCut());
  if (layer->lefiLayer::hasCurrentDensityPoint())
    fprintf(stdout, "  CURRENTDEN %g ;\n",
        layer->lefiLayer::currentDensityPoint());
  if (layer->lefiLayer::hasCurrentDensityArray()) { 
    layer->lefiLayer::currentDensityArray(&numPoints, &widths, &current);
    for (i = 0; i < numPoints; i++)
      fprintf(stdout, "  CURRENTDEN ( %g %g ) ;\n", widths[i], current[i]);
  }
  if (layer->lefiLayer::hasDirection())
    fprintf(stdout, "  DIRECTION %s ;\n", layer->lefiLayer::direction());
  if (layer->lefiLayer::hasResistance())
    fprintf(stdout, "  RESISTANCE RPERSQ %g ;\n",
        layer->lefiLayer::resistance());
  if (layer->lefiLayer::hasCapacitance())
    fprintf(stdout, "  CAPACITANCE CPERSQDIST %g ;\n",
        layer->lefiLayer::capacitance());
  if (layer->lefiLayer::hasEdgeCap())
    fprintf(stdout, "  EDGECAPACITANCE %g ;\n", layer->lefiLayer::edgeCap());
  if (layer->lefiLayer::hasHeight())
    fprintf(stdout, "  TYPE %g ;\n", layer->lefiLayer::height());
  if (layer->lefiLayer::hasThickness())
    fprintf(stdout, "  THICKNESS %g ;\n", layer->lefiLayer::thickness());
  if (layer->lefiLayer::hasWireExtension())
    fprintf(stdout, "  WIREEXTENSION %g ;\n", layer->lefiLayer::wireExtension());
  if (layer->lefiLayer::hasShrinkage())
    fprintf(stdout, "  SHRINKAGE %g ;\n", layer->lefiLayer::shrinkage());
  if (layer->lefiLayer::hasCapMultiplier())
    fprintf(stdout, "  CAPMULTIPLIER %g ;\n", layer->lefiLayer::capMultiplier());
  if (layer->lefiLayer::hasAntennaArea())
    fprintf(stdout, "  ANTENNAAREAFACTOR %g ;\n",
        layer->lefiLayer::antennaArea());
  if (layer->lefiLayer::hasAntennaLength())
    fprintf(stdout, "  ANTENNALENGTHFACTOR %g ;\n",
        layer->lefiLayer::antennaLength());

  // 5.5 AntennaModel
  for (i = 0; i < layer->lefiLayer::numAntennaModel(); i++) {
    aModel = layer->lefiLayer::antennaModel(i);

    fprintf(stdout, "  ANTENNAMODEL %s ;\n",
        aModel->lefiAntennaModel::antennaOxide());

    if (aModel->lefiAntennaModel::hasAntennaAreaRatio())
      fprintf(stdout, "  ANTENNAAREARATIO %g ;\n",
          aModel->lefiAntennaModel::antennaAreaRatio());
    if (aModel->lefiAntennaModel::hasAntennaDiffAreaRatio())
      fprintf(stdout, "  ANTENNADIFFAREARATIO %g ;\n",
          aModel->lefiAntennaModel::antennaDiffAreaRatio());
    else if (aModel->lefiAntennaModel::hasAntennaDiffAreaRatioPWL()) {
      pwl = aModel->lefiAntennaModel::antennaDiffAreaRatioPWL();
      fprintf(stdout, "  ANTENNADIFFAREARATIO PWL ( ");
      for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
        fprintf(stdout, "( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
            pwl->lefiAntennaPWL::PWLratio(j));
      fprintf(stdout, ") ;\n");
    }
    if (aModel->lefiAntennaModel::hasAntennaCumAreaRatio())
      fprintf(stdout, "  ANTENNACUMAREARATIO %g ;\n",
          aModel->lefiAntennaModel::antennaCumAreaRatio());
    if (aModel->lefiAntennaModel::hasAntennaCumDiffAreaRatio())
      fprintf(stdout, "  ANTENNACUMDIFFAREARATIO %g\n",
          aModel->lefiAntennaModel::antennaCumDiffAreaRatio());
    if (aModel->lefiAntennaModel::hasAntennaCumDiffAreaRatioPWL()) {
      pwl = aModel->lefiAntennaModel::antennaCumDiffAreaRatioPWL();
      fprintf(stdout, "  ANTENNACUMDIFFAREARATIO PWL ( ");
      for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
        fprintf(stdout, "( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
            pwl->lefiAntennaPWL::PWLratio(j));
      fprintf(stdout, ") ;\n");
    }
    if (aModel->lefiAntennaModel::hasAntennaAreaFactor()) {
      fprintf(stdout, "  ANTENNAAREAFACTOR %g ",
          aModel->lefiAntennaModel::antennaAreaFactor());
      if (aModel->lefiAntennaModel::hasAntennaAreaFactorDUO())
        fprintf(stdout, "  DIFFUSEONLY ");
      fprintf(stdout, ";\n");
    }
    if (aModel->lefiAntennaModel::hasAntennaSideAreaRatio())
      fprintf(stdout, "  ANTENNASIDEAREARATIO %g ;\n",
          aModel->lefiAntennaModel::antennaSideAreaRatio());
    if (aModel->lefiAntennaModel::hasAntennaDiffSideAreaRatio())
      fprintf(stdout, "  ANTENNADIFFSIDEAREARATIO %g\n",
          aModel->lefiAntennaModel::antennaDiffSideAreaRatio());
    else if (aModel->lefiAntennaModel::hasAntennaDiffSideAreaRatioPWL()) {
      pwl = aModel->lefiAntennaModel::antennaDiffSideAreaRatioPWL();
      fprintf(stdout, "  ANTENNADIFFSIDEAREARATIO PWL ( ");
      for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
        fprintf(stdout, "( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
            pwl->lefiAntennaPWL::PWLratio(j));
      fprintf(stdout, ") ;\n");
    }
    if (aModel->lefiAntennaModel::hasAntennaCumSideAreaRatio())
      fprintf(stdout, "  ANTENNACUMSIDEAREARATIO %g ;\n",
          aModel->lefiAntennaModel::antennaCumSideAreaRatio());
    if (aModel->lefiAntennaModel::hasAntennaCumDiffSideAreaRatio())
      fprintf(stdout, "  ANTENNACUMDIFFSIDEAREARATIO %g\n",
          aModel->lefiAntennaModel::antennaCumDiffSideAreaRatio());
    else if (aModel->lefiAntennaModel::hasAntennaCumDiffSideAreaRatioPWL()) {
      pwl = aModel->lefiAntennaModel::antennaCumDiffSideAreaRatioPWL();
      fprintf(stdout, "  ANTENNACUMDIFFSIDEAREARATIO PWL ( ");
      for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
        fprintf(stdout, "( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
            pwl->lefiAntennaPWL::PWLratio(j));
      fprintf(stdout, ") ;\n");
    }
    if (aModel->lefiAntennaModel::hasAntennaSideAreaFactor()) {
      fprintf(stdout, "  ANTENNASIDEAREAFACTOR %g ",
          aModel->lefiAntennaModel::antennaSideAreaFactor());
      if (aModel->lefiAntennaModel::hasAntennaSideAreaFactorDUO())
        fprintf(stdout, "  DIFFUSEONLY ");
      fprintf(stdout, ";\n");
    }
    if (aModel->lefiAntennaModel::hasAntennaCumRoutingPlusCut())
      fprintf(stdout, "  ANTENNACUMROUTINGPLUSCUT ;\n");
    if (aModel->lefiAntennaModel::hasAntennaGatePlusDiff())
      fprintf(stdout, "  ANTENNAGATEPLUSDIFF %g ;\n",
          aModel->lefiAntennaModel::antennaGatePlusDiff());
    if (aModel->lefiAntennaModel::hasAntennaAreaMinusDiff())
      fprintf(stdout, "  ANTENNAAREAMINUSDIFF %g ;\n",
          aModel->lefiAntennaModel::antennaAreaMinusDiff());
    if (aModel->lefiAntennaModel::hasAntennaAreaDiffReducePWL()) {
      pwl = aModel->lefiAntennaModel::antennaAreaDiffReducePWL();
      fprintf(stdout, "  ANTENNAAREADIFFREDUCEPWL ( ");
      for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
        fprintf(stdout, "( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
            pwl->lefiAntennaPWL::PWLratio(j));
      fprintf(stdout, ") ;\n");
    }
  }

  if (layer->lefiLayer::numAccurrentDensity()) {
    for (i = 0; i < layer->lefiLayer::numAccurrentDensity(); i++) {
      density = layer->lefiLayer::accurrent(i);
      fprintf(stdout, "  ACCURRENTDENSITY %s", density->type()); 
      if (density->hasOneEntry())
        fprintf(stdout, " %g ;\n", density->oneEntry()); 
      else {
        fprintf(stdout, "\n");
        if (density->numFrequency()) {
          fprintf(stdout, "  FREQUENCY");
          for (j = 0; j < density->numFrequency(); j++)
            fprintf(stdout, " %g", density->frequency(j));
          fprintf(stdout, " ;\n");  
        }
        if (density->numCutareas()) {
          fprintf(stdout, "  CUTAREA");
          for (j = 0; j < density->numCutareas(); j++)
            fprintf(stdout, " %g", density->cutArea(j));
          fprintf(stdout, " ;\n");  
        }
        if (density->numWidths()) {
          fprintf(stdout, "  WIDTH");
          for (j = 0; j < density->numWidths(); j++)
            fprintf(stdout, " %g", density->width(j));
          fprintf(stdout, " ;\n");  
        }
        if (density->numTableEntries()) {
          k = 5;
          fprintf(stdout, "  TABLEENTRIES");
          for (j = 0; j < density->numTableEntries(); j++)
            if (k > 4) {
              fprintf(stdout, "\n   %g", density->tableEntry(j));
              k = 1;
            } else {
              fprintf(stdout, " %g", density->tableEntry(j));
              k++;
            }
          fprintf(stdout, " ;\n");  
        }
      }
    }
  }
  if (layer->lefiLayer::numDccurrentDensity()) {
    for (i = 0; i < layer->lefiLayer::numDccurrentDensity(); i++) {
      density = layer->lefiLayer::dccurrent(i);
      fprintf(stdout, "  DCCURRENTDENSITY %s", density->type()); 
      if (density->hasOneEntry())
        fprintf(stdout, " %g ;\n", density->oneEntry()); 
      else {
        fprintf(stdout, "\n");
        if (density->numCutareas()) {
          fprintf(stdout, "  CUTAREA");
          for (j = 0; j < density->numCutareas(); j++)
            fprintf(stdout, " %g", density->cutArea(j));
          fprintf(stdout, " ;\n");  
        }
        if (density->numWidths()) {
          fprintf(stdout, "  WIDTH");
          for (j = 0; j < density->numWidths(); j++)
            fprintf(stdout, " %g", density->width(j));
          fprintf(stdout, " ;\n");  
        }
        if (density->numTableEntries()) {
          fprintf(stdout, "  TABLEENTRIES");
          for (j = 0; j < density->numTableEntries(); j++)
            fprintf(stdout, " %g", density->tableEntry(j));
          fprintf(stdout, " ;\n");  
        }
      }
    }
  }

  for (i = 0; i < layer->lefiLayer::numSpacingTable(); i++) {
    spTable = layer->lefiLayer::spacingTable(i);
    fprintf(stdout, "   SPACINGTABLE\n");
    if (spTable->lefiSpacingTable::isInfluence()) {
      influence = spTable->lefiSpacingTable::influence();
      fprintf(stdout, "    INFLUENCE");
      for (j = 0; j < influence->lefiInfluence::numInfluenceEntry(); j++) {
        fprintf(stdout, "\n      WIDTH %g WITHIN %g SPACING %g",
            influence->lefiInfluence::width(j),
            influence->lefiInfluence::distance(j),
            influence->lefiInfluence::spacing(j));
      }   
      fprintf(stdout, " ;\n");
    } else if (spTable->lefiSpacingTable::isParallel()){
      parallel = spTable->lefiSpacingTable::parallel();
      fprintf(stdout, "    PARALLELRUNLENGTH");
      for (j = 0; j < parallel->lefiParallel::numLength(); j++) {
        fprintf(stdout, " %g", parallel->lefiParallel::length(j));
      }
      for (j = 0; j < parallel->lefiParallel::numWidth(); j++) {
        fprintf(stdout, "\n      WIDTH %g",
            parallel->lefiParallel::width(j));
        for (k = 0; k < parallel->lefiParallel::numLength(); k++) {
          fprintf(stdout, " %g", parallel->lefiParallel::widthSpacing(j, k));
        }
      }
      fprintf(stdout, " ;\n");
    } else {  // 5.7 TWOWIDTHS
      twoWidths = spTable->lefiSpacingTable::twoWidths();
      fprintf(stdout, "    TWOWIDTHS"); 
      for (j = 0; j < twoWidths->lefiTwoWidths::numWidth(); j++) {
        fprintf(stdout, "\n      WIDTH %g ",
            twoWidths->lefiTwoWidths::width(j));
        if (twoWidths->lefiTwoWidths::hasWidthPRL(j))
          fprintf(stdout, "PRL %g ", twoWidths->lefiTwoWidths::widthPRL(j));
        for (k = 0; k < twoWidths->lefiTwoWidths::numWidthSpacing(j); k++)
          fprintf(stdout, "%g ",twoWidths->lefiTwoWidths::widthSpacing(j, k));
      }
      fprintf(stdout, " ;\n");
    }
  }

  propNum = layer->lefiLayer::numProps();
  if (propNum > 0) {
    fprintf(stdout, "  PROPERTY ");
    for (i = 0; i < propNum; i++) {
      // value can either be a string or number
      fprintf(stdout, "%s ", layer->lefiLayer::propName(i));
      if (layer->lefiLayer::propIsNumber(i))
        fprintf(stdout, "%g ", layer->lefiLayer::propNumber(i));
      if (layer->lefiLayer::propIsString(i)) 
        fprintf(stdout, "%s ", layer->lefiLayer::propValue(i));
      pType = layer->lefiLayer::propType(i);
      switch (pType) {
        case 'R': fprintf(stdout, "REAL ");
              break;
        case 'I': fprintf(stdout, "INTEGER ");
              break;
        case 'S': fprintf(stdout, "STRING ");
              break;
        case 'Q': fprintf(stdout, "QUOTESTRING ");
              break;
        case 'N': fprintf(stdout, "NUMBER ");
              break;
      } 
    }
    fprintf(stdout, ";\n");
  }
  if (layer->lefiLayer::hasDiagMinEdgeLength())
    fprintf(stdout, "  DIAGMINEDGELENGTH %g ;\n",
        layer->lefiLayer::diagMinEdgeLength());
  if (layer->lefiLayer::numMinSize()) {
    fprintf(stdout, "  MINSIZE ");
    for (i = 0; i < layer->lefiLayer::numMinSize(); i++) {
      fprintf(stdout, "%g %g ", layer->lefiLayer::minSizeWidth(i),
          layer->lefiLayer::minSizeLength(i)); 
    }
    fprintf(stdout, ";\n");
  }

  fprintf(stdout, "END %s\n", layer->lefiLayer::name()); 

  // Set it to case sensitive from here on
  lefrSetCaseSensitivity(1);

  return 0;
}

int macroBeginCB(lefrCallbackType_e c, const char* macroName, lefiUserData) 
{
  checkType(c);
  return 0;
}

int macroFixedMaskCB(lefrCallbackType_e c, int,  lefiUserData) 
{
  checkType(c);
  return 0;
}

int macroClassTypeCB(lefrCallbackType_e c, const char* macroClassType, lefiUserData) 
{
  checkType(c);
  return 0;
}

int macroOriginCB(lefrCallbackType_e c, lefiNum, lefiUserData) 
{
  checkType(c);
  return 0;
}

int macroSizeCB(lefrCallbackType_e c, lefiNum, lefiUserData) 
{
  checkType(c);
  return 0;
}

int macroCB(lefrCallbackType_e c, lefiMacro* macro, lefiUserData) 
{
  lefiSitePattern* pattern;
	int i;
  int propNum;
	int hasPrtSym = 0;

  checkType(c);
  if(macro->lefiMacro::hasClass())
    fprintf(stdout, "  CLASS %s ;\n", macro->lefiMacro::macroClass());
  if(macro->lefiMacro::isFixedMask())
    fprintf(stdout, "  FIXEDMASK ;\n");
  if(macro->lefiMacro::hasEEQ())
    fprintf(stdout, "  EEQ %s ;\n", macro->lefiMacro::EEQ());
  if(macro->lefiMacro::hasLEQ())
    fprintf(stdout, "  LEQ %s ;\n", macro->lefiMacro::LEQ());
  if(macro->lefiMacro::hasSource())
    fprintf(stdout, "  SOURCE %s ;\n", macro->lefiMacro::source());
  if(macro->lefiMacro::hasXSymmetry()) 
	{
    fprintf(stdout, "  SYMMETRY X ");
    hasPrtSym = 1;
  }
  if (macro->lefiMacro::hasYSymmetry()) 
	{   
		// print X Y & R90 in one line
    if(!hasPrtSym) 
		{
      fprintf(stdout, "  SYMMETRY Y ");
      hasPrtSym = 1;
    }
    else
      fprintf(stdout, "Y ");
  }
  if(macro->lefiMacro::has90Symmetry()) 
	{
    if(!hasPrtSym) 
		{
      fprintf(stdout, "  SYMMETRY R90 ");
      hasPrtSym = 1;
    }
    else
      fprintf(stdout, "R90 ");
  }
  if(hasPrtSym) 
	{
    fprintf (stdout, ";\n");
    hasPrtSym = 0;
  }
  if(macro->lefiMacro::hasSiteName())
    fprintf(stdout, "  SITE %s ;\n", macro->lefiMacro::siteName());
  if (macro->lefiMacro::hasSitePattern()) 
	{
    for(i = 0; i < macro->lefiMacro::numSitePattern(); i++ ) 
		{
      pattern = macro->lefiMacro::sitePattern(i);
      if(pattern->lefiSitePattern::hasStepPattern()) 
			{
        fprintf(stdout, "  SITE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
            pattern->lefiSitePattern::name(), pattern->lefiSitePattern::x(),
            pattern->lefiSitePattern::y(),
            orientStr(pattern->lefiSitePattern::orient()),
            pattern->lefiSitePattern::xStart(),
            pattern->lefiSitePattern::yStart(),
            pattern->lefiSitePattern::xStep(),
            pattern->lefiSitePattern::yStep());
      } 
			else
			{
        fprintf(stdout, "  SITE %s %g %g %s ;\n",
            pattern->lefiSitePattern::name(), pattern->lefiSitePattern::x(),
            pattern->lefiSitePattern::y(),
            orientStr(pattern->lefiSitePattern::orient()));
      }
    }
  }
  if(macro->lefiMacro::hasSize())
    fprintf(stdout, "  SIZE %g BY %g ;\n", macro->lefiMacro::sizeX(), macro->lefiMacro::sizeY());

  if(macro->lefiMacro::hasForeign()) 
	{
    for(i = 0; i < macro->lefiMacro::numForeigns(); i++) 
		{
      fprintf(stdout, "  FOREIGN %s ", macro->lefiMacro::foreignName(i));
      if(macro->lefiMacro::hasForeignPoint(i)) 
			{
        fprintf(stdout, "( %g %g ) ", macro->lefiMacro::foreignX(i), macro->lefiMacro::foreignY(i));
        if(macro->lefiMacro::hasForeignOrient(i))
          fprintf(stdout, "%s ", macro->lefiMacro::foreignOrientStr(i));
      }
      fprintf(stdout, ";\n");
    }
  }
  if(macro->lefiMacro::hasOrigin())
    fprintf(stdout, "  ORIGIN ( %g %g ) ;\n", macro->lefiMacro::originX(), macro->lefiMacro::originY());
  if(macro->lefiMacro::hasPower())
    fprintf(stdout, "  POWER %g ;\n", macro->lefiMacro::power());
  propNum = macro->lefiMacro::numProperties();
  if(propNum > 0) 
	{
    fprintf(stdout, "  PROPERTY ");
    for(i = 0; i < propNum; i++) 
		{
      // value can either be a string or number
      if(macro->lefiMacro::propValue(i)) 
        fprintf(stdout, "%s %s ", macro->lefiMacro::propName(i), macro->lefiMacro::propValue(i));
      else
        fprintf(stdout, "%s %g ", macro->lefiMacro::propName(i), macro->lefiMacro::propNum(i));

      switch(macro->lefiMacro::propType(i)) 
			{
        case 'R': fprintf(stdout, "REAL ");
              break;
        case 'I': fprintf(stdout, "INTEGER ");
              break;
        case 'S': fprintf(stdout, "STRING ");
              break;
        case 'Q': fprintf(stdout, "QUOTESTRING ");
              break;
        case 'N': fprintf(stdout, "NUMBER ");
              break;
      } 
    }
    fprintf(stdout, ";\n");
  }
  return 0;
}

int macroEndCB(lefrCallbackType_e c, const char* macroName, lefiUserData) 
{
  checkType(c);
  return 0;
}

int manufacturingCB(lefrCallbackType_e c, double num, lefiUserData) 
{
  checkType(c);
  return 0;
}

int maxStackViaCB(lefrCallbackType_e c, lefiMaxStackVia* maxStack, lefiUserData) 
{
  checkType(c);
  fprintf(stdout, "MAXVIASTACK %d ", maxStack->lefiMaxStackVia::maxStackVia());
  if(maxStack->lefiMaxStackVia::hasMaxStackViaRange())
	{
    fprintf(stdout, "RANGE %s %s ",
        maxStack->lefiMaxStackVia::maxStackViaBottomLayer(),
        maxStack->lefiMaxStackVia::maxStackViaTopLayer());
	}
  fprintf(stdout, ";\n");
  return 0;
}

int minFeatureCB(lefrCallbackType_e c, lefiMinFeature* min, lefiUserData) 
{
  checkType(c);
  // if ((long)ud != userData) dataError();
  fprintf(stdout, "MINFEATURE %g %g ;\n", min->lefiMinFeature::one(),
      min->lefiMinFeature::two());
  return 0;
}

int nonDefaultCB(lefrCallbackType_e c, lefiNonDefault* def, lefiUserData) 
{
  int i;
  lefiVia* via;
  lefiSpacing* spacing;

  checkType(c);
  fprintf(stdout, "NONDEFAULTRULE %s\n", def->lefiNonDefault::name());
  if(def->lefiNonDefault::hasHardspacing())
    fprintf(stdout, "  HARDSPACING ;\n");
  for(i = 0; i < def->lefiNonDefault::numLayers(); i++) 
	{
    fprintf(stdout, "  LAYER %s\n", def->lefiNonDefault::layerName(i));
    if (def->lefiNonDefault::hasLayerWidth(i))
      fprintf(stdout, "  WIDTH %g ;\n", def->lefiNonDefault::layerWidth(i));
    if (def->lefiNonDefault::hasLayerSpacing(i))
      fprintf(stdout, "  SPACING %g ;\n",
          def->lefiNonDefault::layerSpacing(i));
    if (def->lefiNonDefault::hasLayerDiagWidth(i))
      fprintf(stdout, "  DIAGWIDTH %g ;\n",
          def->lefiNonDefault::layerDiagWidth(i));
    if (def->lefiNonDefault::hasLayerWireExtension(i))
      fprintf(stdout, "  WIREEXTENSION %g ;\n",
          def->lefiNonDefault::layerWireExtension(i));
    if (def->lefiNonDefault::hasLayerResistance(i))
      fprintf(stdout, "  RESISTANCE RPERSQ %g ;\n",
          def->lefiNonDefault::layerResistance(i));
    if (def->lefiNonDefault::hasLayerCapacitance(i))
      fprintf(stdout, "  CAPACITANCE CPERSQDIST %g ;\n",
          def->lefiNonDefault::layerCapacitance(i));
    if (def->lefiNonDefault::hasLayerEdgeCap(i))
      fprintf(stdout, "  EDGECAPACITANCE %g ;\n",
          def->lefiNonDefault::layerEdgeCap(i));
    fprintf(stdout, "  END %s\n", def->lefiNonDefault::layerName(i));
  }

  // handle via in nondefaultrule
  for(i = 0; i < def->lefiNonDefault::numVias(); i++) 
	{
    via = def->lefiNonDefault::viaRule(i);
    lefVia(via);
  }

  // handle spacing in nondefaultrule
  for(i = 0; i < def->lefiNonDefault::numSpacingRules(); i++) 
	{
    spacing = def->lefiNonDefault::spacingRule(i);
    lefSpacing(spacing);
  }

  // handle usevia
  for (i = 0; i < def->lefiNonDefault::numUseVia(); i++)
    fprintf(stdout, "  USEVIA %s ;\n", def->lefiNonDefault::viaName(i));

  // handle useviarule
  for (i = 0; i < def->lefiNonDefault::numUseViaRule(); i++)
    fprintf(stdout, "  USEVIARULE %s ;\n",
        def->lefiNonDefault::viaRuleName(i));

  // handle mincuts
  for (i = 0; i < def->lefiNonDefault::numMinCuts(); i++) {
    fprintf(stdout, "   MINCUTS %s %d ;\n", def->lefiNonDefault::cutLayerName(i),
        def->lefiNonDefault::numCuts(i));
  }

  // handle property in nondefaultrule
  if(def->lefiNonDefault::numProps() > 0) 
	{
    fprintf(stdout, "   PROPERTY ");
    for (i = 0; i < def->lefiNonDefault::numProps(); i++) 
		{
      fprintf(stdout, "%s ", def->lefiNonDefault::propName(i));
      if(def->lefiNonDefault::propIsNumber(i))
        fprintf(stdout, "%g ", def->lefiNonDefault::propNumber(i));   
      if(def->lefiNonDefault::propIsString(i))
        fprintf(stdout, "%s ", def->lefiNonDefault::propValue(i));   
      switch(def->lefiNonDefault::propType(i)) {
        case 'R': fprintf(stdout, "REAL ");
              break;
        case 'I': fprintf(stdout, "INTEGER ");
              break;
        case 'S': fprintf(stdout, "STRING ");
              break;
        case 'Q': fprintf(stdout, "QUOTESTRING ");
              break;
        case 'N': fprintf(stdout, "NUMBER ");
              break;
      }
    }
    fprintf(stdout, ";\n");
  }
  fprintf(stdout, "END %s ;\n", def->lefiNonDefault::name());

  return 0;
}

int obstructionCB(lefrCallbackType_e c, lefiObstruction* obs, lefiUserData) 
{
  checkType(c);
  lefiGeometries* geometry = obs->lefiObstruction::geometries();
  return 0;
}

int pinCB(lefrCallbackType_e c, lefiPin* pin, lefiUserData) 
{
  int numPorts, i, j;
  lefiGeometries*    geometry;
  lefiPinAntennaModel* aModel;

  checkType(c);
  fprintf(stdout, "  PIN %s\n", pin->lefiPin::name());
  if(pin->lefiPin::hasForeign()) 
	{
    for(i = 0; i < pin->lefiPin::numForeigns(); i++) 
		{
      if(pin->lefiPin::hasForeignOrient(i))
			{
        fprintf(stdout, "  FOREIGN %s STRUCTURE ( %g %g ) %s ;\n",
            pin->lefiPin::foreignName(i), pin->lefiPin::foreignX(i),
            pin->lefiPin::foreignY(i),
            pin->lefiPin::foreignOrientStr(i));
			}
      else if (pin->lefiPin::hasForeignPoint(i))
			{
        fprintf(stdout, "  FOREIGN %s STRUCTURE ( %g %g ) ;\n",
            pin->lefiPin::foreignName(i), pin->lefiPin::foreignX(i),
            pin->lefiPin::foreignY(i));
			}
      else
        fprintf(stdout, "  FOREIGN %s ;\n", pin->lefiPin::foreignName(i));
    }
  }

  if(pin->lefiPin::hasLEQ())
    fprintf(stdout, "  LEQ %s ;\n", pin->lefiPin::LEQ());
  if(pin->lefiPin::hasDirection())
    fprintf(stdout, "  DIRECTION %s ;\n", pin->lefiPin::direction());
  if(pin->lefiPin::hasUse())
    fprintf(stdout, "  USE %s ;\n", pin->lefiPin::use());
  if(pin->lefiPin::hasShape())
    fprintf(stdout, "  SHAPE %s ;\n", pin->lefiPin::shape());

//  if(pin->lefiPin::hasMustjoin())
//    fprintf(stdout, "  MUSTJOIN %s ;\n", pin->lefiPin::mustjoin());
//  if(pin->lefiPin::hasOutMargin())
//    fprintf(stdout, "  OUTPUTNOISEMARGIN %g %g ;\n", pin->lefiPin::outMarginHigh(), pin->lefiPin::outMarginLow());
//  if(pin->lefiPin::hasOutResistance())
//    fprintf(stdout, "  OUTPUTRESISTANCE %g %g ;\n", pin->lefiPin::outResistanceHigh(), pin->lefiPin::outResistanceLow());
//  if(pin->lefiPin::hasInMargin())
//    fprintf(stdout, "  INPUTNOISEMARGIN %g %g ;\n", pin->lefiPin::inMarginHigh(), pin->lefiPin::inMarginLow());
//  if(pin->lefiPin::hasPower())
//    fprintf(stdout, "  POWER %g ;\n", pin->lefiPin::power());
//  if(pin->lefiPin::hasLeakage())
//    fprintf(stdout, "  LEAKAGE %g ;\n", pin->lefiPin::leakage());
//  if(pin->lefiPin::hasMaxload())
//    fprintf(stdout, "  MAXLOAD %g ;\n", pin->lefiPin::maxload());
//  if(pin->lefiPin::hasCapacitance())
//    fprintf(stdout, "  CAPACITANCE %g ;\n", pin->lefiPin::capacitance());
//  if(pin->lefiPin::hasResistance())
//    fprintf(stdout, "  RESISTANCE %g ;\n", pin->lefiPin::resistance());
//  if(pin->lefiPin::hasPulldownres())
//    fprintf(stdout, "  PULLDOWNRES %g ;\n", pin->lefiPin::pulldownres());
//  if(pin->lefiPin::hasTieoffr())
//    fprintf(stdout, "  TIEOFFR %g ;\n", pin->lefiPin::tieoffr());
//  if(pin->lefiPin::hasVHI())
//    fprintf(stdout, "  VHI %g ;\n", pin->lefiPin::VHI());
//  if(pin->lefiPin::hasVLO())
//    fprintf(stdout, "  VLO %g ;\n", pin->lefiPin::VLO());
//  if(pin->lefiPin::hasRiseVoltage())
//    fprintf(stdout, "  RISEVOLTAGETHRESHOLD %g ;\n", pin->lefiPin::riseVoltage());
//  if(pin->lefiPin::hasFallVoltage())
//    fprintf(stdout, "  FALLVOLTAGETHRESHOLD %g ;\n", pin->lefiPin::fallVoltage());
//  if(pin->lefiPin::hasRiseThresh())
//    fprintf(stdout, "  RISETHRESH %g ;\n", pin->lefiPin::riseThresh());
//  if(pin->lefiPin::hasFallThresh())
//    fprintf(stdout, "  FALLTHRESH %g ;\n", pin->lefiPin::fallThresh());
//  if(pin->lefiPin::hasRiseSatcur())
//    fprintf(stdout, "  RISESATCUR %g ;\n", pin->lefiPin::riseSatcur());
//  if(pin->lefiPin::hasFallSatcur())
//    fprintf(stdout, "  FALLSATCUR %g ;\n", pin->lefiPin::fallSatcur());
//  if(pin->lefiPin::hasRiseSlewLimit())
//    fprintf(stdout, "  RISESLEWLIMIT %g ;\n", pin->lefiPin::riseSlewLimit());
//  if(pin->lefiPin::hasFallSlewLimit())
//    fprintf(stdout, "  FALLSLEWLIMIT %g ;\n", pin->lefiPin::fallSlewLimit());
//  if(pin->lefiPin::hasCurrentSource())
//    fprintf(stdout, "  CURRENTSOURCE %s ;\n", pin->lefiPin::currentSource());
//  if(pin->lefiPin::hasTables())
//    fprintf(stdout, "  IV_TABLES %s %s ;\n", pin->lefiPin::tableHighName(), pin->lefiPin::tableLowName());
//  if(pin->lefiPin::hasTaperRule())
//    fprintf(stdout, "  TAPERRULE %s ;\n", pin->lefiPin::taperRule());
//  if(pin->lefiPin::hasNetExpr())
//    fprintf(stdout, "  NETEXPR \"%s\" ;\n", pin->lefiPin::netExpr());
//  if(pin->lefiPin::hasSupplySensitivity())
//    fprintf(stdout, "  SUPPLYSENSITIVITY %s ;\n", pin->lefiPin::supplySensitivity());
//  if(pin->lefiPin::hasGroundSensitivity())
//    fprintf(stdout, "  GROUNDSENSITIVITY %s ;\n", pin->lefiPin::groundSensitivity());
//  if(pin->lefiPin::hasAntennaSize()) 
//	{
//    for(i = 0; i < pin->lefiPin::numAntennaSize(); i++) 
//		{
//      fprintf(stdout, "  ANTENNASIZE %g ", pin->lefiPin::antennaSize(i));
//      if(pin->lefiPin::antennaSizeLayer(i))
//        fprintf(stdout, "LAYER %s ", pin->lefiPin::antennaSizeLayer(i));
//      fprintf(stdout, ";\n");
//    }
//  }
//  if(pin->lefiPin::hasAntennaMetalArea()) 
//	{
//    for(i = 0; i < pin->lefiPin::numAntennaMetalArea(); i++) 
//		{
//      fprintf(stdout, "  ANTENNAMETALAREA %g ", pin->lefiPin::antennaMetalArea(i));
//      if(pin->lefiPin::antennaMetalAreaLayer(i))
//        fprintf(stdout, "LAYER %s ", pin->lefiPin::antennaMetalAreaLayer(i));
//      fprintf(stdout, ";\n");
//    }
//  }
//  if (pin->lefiPin::hasAntennaMetalLength()) {
//    for (i = 0; i < pin->lefiPin::numAntennaMetalLength(); i++) {
//      fprintf(stdout, "  ANTENNAMETALLENGTH %g ",
//          pin->lefiPin::antennaMetalLength(i));
//      if (pin->lefiPin::antennaMetalLengthLayer(i))
//        fprintf(stdout, "LAYER %s ", pin->lefiPin::antennaMetalLengthLayer(i));
//      fprintf(stdout, ";\n");
//    }
//  }
//
//  if (pin->lefiPin::hasAntennaPartialMetalArea()) {
//    for (i = 0; i < pin->lefiPin::numAntennaPartialMetalArea(); i++) {
//      fprintf(stdout, "  ANTENNAPARTIALMETALAREA %g ",
//          pin->lefiPin::antennaPartialMetalArea(i));
//      if (pin->lefiPin::antennaPartialMetalAreaLayer(i))
//        fprintf(stdout, "LAYER %s ",
//            pin->lefiPin::antennaPartialMetalAreaLayer(i));
//      fprintf(stdout, ";\n");
//    }
//  }
//
//  if (pin->lefiPin::hasAntennaPartialMetalSideArea()) {
//    for (i = 0; i < pin->lefiPin::numAntennaPartialMetalSideArea(); i++) {
//      fprintf(stdout, "  ANTENNAPARTIALMETALSIDEAREA %g ",
//          pin->lefiPin::antennaPartialMetalSideArea(i));
//      if (pin->lefiPin::antennaPartialMetalSideAreaLayer(i))
//        fprintf(stdout, "LAYER %s ",
//            pin->lefiPin::antennaPartialMetalSideAreaLayer(i));
//      fprintf(stdout, ";\n");
//    }
//  }
//
//  if (pin->lefiPin::hasAntennaPartialCutArea()) {
//    for (i = 0; i < pin->lefiPin::numAntennaPartialCutArea(); i++) {
//      fprintf(stdout, "  ANTENNAPARTIALCUTAREA %g ",
//          pin->lefiPin::antennaPartialCutArea(i));
//      if (pin->lefiPin::antennaPartialCutAreaLayer(i))
//        fprintf(stdout, "LAYER %s ",
//            pin->lefiPin::antennaPartialCutAreaLayer(i));
//      fprintf(stdout, ";\n");
//    }
//  }
//
//  if (pin->lefiPin::hasAntennaDiffArea()) {
//    for (i = 0; i < pin->lefiPin::numAntennaDiffArea(); i++) {
//      fprintf(stdout, "  ANTENNADIFFAREA %g ",
//          pin->lefiPin::antennaDiffArea(i));
//      if (pin->lefiPin::antennaDiffAreaLayer(i))
//        fprintf(stdout, "LAYER %s ", pin->lefiPin::antennaDiffAreaLayer(i));
//      fprintf(stdout, ";\n");
//    }
//  }
//
//  for (j = 0; j < pin->lefiPin::numAntennaModel(); j++) {
//    aModel = pin->lefiPin::antennaModel(j); 
//
//    fprintf(stdout, "  ANTENNAMODEL %s ;\n",
//        aModel->lefiPinAntennaModel::antennaOxide());
//
//    if (aModel->lefiPinAntennaModel::hasAntennaGateArea()) {
//      for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaGateArea(); i++)
//      {
//        fprintf(stdout, "  ANTENNAGATEAREA %g ",
//            aModel->lefiPinAntennaModel::antennaGateArea(i));
//        if (aModel->lefiPinAntennaModel::antennaGateAreaLayer(i))
//          fprintf(stdout, "LAYER %s ",
//              aModel->lefiPinAntennaModel::antennaGateAreaLayer(i));
//        fprintf(stdout, ";\n");
//      }
//    }
//
//    if (aModel->lefiPinAntennaModel::hasAntennaMaxAreaCar()) {
//      for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxAreaCar();
//          i++) {
//        fprintf(stdout, "  ANTENNAMAXAREACAR %g ",
//            aModel->lefiPinAntennaModel::antennaMaxAreaCar(i));
//        if (aModel->lefiPinAntennaModel::antennaMaxAreaCarLayer(i))
//          fprintf(stdout, "LAYER %s ",
//              aModel->lefiPinAntennaModel::antennaMaxAreaCarLayer(i));
//        fprintf(stdout, ";\n");
//      }
//    }
//
//    if (aModel->lefiPinAntennaModel::hasAntennaMaxSideAreaCar()) {
//      for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxSideAreaCar();
//          i++) {
//        fprintf(stdout, "  ANTENNAMAXSIDEAREACAR %g ",
//            aModel->lefiPinAntennaModel::antennaMaxSideAreaCar(i));
//        if (aModel->lefiPinAntennaModel::antennaMaxSideAreaCarLayer(i))
//          fprintf(stdout, "LAYER %s ",
//              aModel->lefiPinAntennaModel::antennaMaxSideAreaCarLayer(i));
//        fprintf(stdout, ";\n");
//      }
//    }
//
//    if (aModel->lefiPinAntennaModel::hasAntennaMaxCutCar()) {
//      for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxCutCar(); i++)
//      {
//        fprintf(stdout, "  ANTENNAMAXCUTCAR %g ",
//            aModel->lefiPinAntennaModel::antennaMaxCutCar(i));
//        if (aModel->lefiPinAntennaModel::antennaMaxCutCarLayer(i))
//          fprintf(stdout, "LAYER %s ",
//              aModel->lefiPinAntennaModel::antennaMaxCutCarLayer(i));
//        fprintf(stdout, ";\n");
//      }
//    }
//  }
//
//  if (pin->lefiPin::numProperties() > 0) {
//    fprintf(stdout, "  PROPERTY ");
//    for (i = 0; i < pin->lefiPin::numProperties(); i++) {
//      // value can either be a string or number
//      if (pin->lefiPin::propValue(i)) {
//        fprintf(stdout, "%s %s ", pin->lefiPin::propName(i),
//            pin->lefiPin::propValue(i));
//      }
//      else
//        fprintf(stdout, "%s %g ", pin->lefiPin::propName(i),
//            pin->lefiPin::propNum(i));
//      switch (pin->lefiPin::propType(i)) {
//        case 'R': fprintf(stdout, "REAL ");
//              break;
//        case 'I': fprintf(stdout, "INTEGER ");
//              break;
//        case 'S': fprintf(stdout, "STRING ");
//              break;
//        case 'Q': fprintf(stdout, "QUOTESTRING ");
//              break;
//        case 'N': fprintf(stdout, "NUMBER ");
//              break;
//      } 
//    }
//    fprintf(stdout, ";\n");
//  }

  numPorts = pin->lefiPin::numPorts();
  for(i = 0; i < numPorts; i++) 
	{
    fprintf(stdout,"  PORT\n");
    geometry = pin->lefiPin::port(i);
    fprintf(stdout, "  END\n");
  }
  fprintf(stdout, "  END %s\n", pin->lefiPin::name());
  return 0;  
}

int densityCB(lefrCallbackType_e c, lefiDensity* density, lefiUserData) 
{
  struct lefiGeomRect rect;

  checkType(c);
  printf("  DENSITY\n");
  for(int i = 0; i < density->lefiDensity::numLayer(); i++) 
	{
    printf("  LAYER %s ;\n", density->lefiDensity::layerName(i));
    for(int j = 0; j < density->lefiDensity::numRects(i); j++) 
		{
      rect = density->lefiDensity::getRect(i,j);
      printf("    RECT %g %g %g %g ", rect.xl, rect.yl, rect.xh, rect.yh);
      printf("%g ;\n", density->lefiDensity::densityValue(i,j));
    }
  }
  printf("  END\n");
  return 0;
}

int propDefBeginCB(lefrCallbackType_e c, void*, lefiUserData) 
{
  checkType(c);
  return 0;
}

int propDefCB(lefrCallbackType_e c, lefiProp* prop, lefiUserData) 
{
  checkType(c);
  printf(" %s %s", prop->lefiProp::propType(), prop->lefiProp::propName());
  switch(prop->lefiProp::dataType()) 
	{
    case 'I':
      printf(" INTEGER"); 
      break;
    case 'R':
      printf(" REAL"); 
      break;
    case 'S':
      printf(" STRING"); 
      break;
  }
  if(prop->lefiProp::hasNumber())
    printf(" %g", prop->lefiProp::number());
  if(prop->lefiProp::hasRange())
    printf(" RANGE %g %g", prop->lefiProp::left(), prop->lefiProp::right());
  if(prop->lefiProp::hasString())
    printf(" %s", prop->lefiProp::string());
  printf("\n");
  return 0;
}

int propDefEndCB(lefrCallbackType_e c, void*, lefiUserData) 
{
  checkType(c);
  return 0;
}

int siteCB(lefrCallbackType_e c, lefiSite* site, lefiUserData) 
{
  int hasPrtSym = 0;
  int i;
  checkType(c);
  fprintf(stdout, "SITE %s\n", site->lefiSite::name());
  if(site->lefiSite::hasClass())
    fprintf(stdout, "  CLASS %s ;\n", site->lefiSite::siteClass());
  if(site->lefiSite::hasXSymmetry()) 
	{
    fprintf(stdout, "  SYMMETRY X ");
    hasPrtSym = 1;
  }
  if(site->lefiSite::hasYSymmetry()) 
	{
    if (hasPrtSym)
      fprintf(stdout, "Y ");
    else 
		{
      fprintf(stdout, "  SYMMETRY Y ");
      hasPrtSym = 1;
    }
  }
  if(site->lefiSite::has90Symmetry()) 
	{
    if(hasPrtSym)
      fprintf(stdout, "R90 ");
    else 
		{
      fprintf(stdout, "  SYMMETRY R90 ");
      hasPrtSym = 1;
    }
  }
  if(hasPrtSym)
    fprintf(stdout, ";\n");
  if(site->lefiSite::hasSize())
    fprintf(stdout, "  SIZE %g BY %g ;\n", site->lefiSite::sizeX(), site->lefiSite::sizeY());

  if(site->hasRowPattern()) 
	{
    fprintf(stdout, "  ROWPATTERN ");
    for(i = 0; i < site->lefiSite::numSites(); i++)
      fprintf(stdout, "  %s %s ", site->lefiSite::siteName(i), site->lefiSite::siteOrientStr(i));
    fprintf(stdout, ";\n");
  }

  fprintf(stdout, "END %s\n", site->lefiSite::name());
  return 0;
}

int spacingBeginCB(lefrCallbackType_e c, void*, lefiUserData)
{
  checkType(c);
  return 0;
}

int spacingCB(lefrCallbackType_e c, lefiSpacing* spacing, lefiUserData) 
{
  checkType(c);
  lefSpacing(spacing);
  return 0;
}

int spacingEndCB(lefrCallbackType_e c, void*, lefiUserData)
{
  checkType(c);
  return 0;
}

int timingCB(lefrCallbackType_e c, lefiTiming* timing, lefiUserData) 
{
  int i;
  checkType(c);
  fprintf(stdout, "TIMING\n");
  for(i = 0; i < timing->numFromPins(); i++)
    fprintf(stdout, " FROMPIN %s ;\n", timing->fromPin(i));
  for(i = 0; i < timing->numToPins(); i++)
    fprintf(stdout, " TOPIN %s ;\n", timing->toPin(i));

  fprintf(stdout, " RISE SLEW1 %g %g %g %g ;\n", timing->riseSlewOne(),
      timing->riseSlewTwo(), timing->riseSlewThree(),
      timing->riseSlewFour());

  if(timing->hasRiseSlew2())
    fprintf(stdout, " RISE SLEW2 %g %g %g ;\n", timing->riseSlewFive(),
        timing->riseSlewSix(), timing->riseSlewSeven());
  if(timing->hasFallSlew())
    fprintf(stdout, " FALL SLEW1 %g %g %g %g ;\n", timing->fallSlewOne(),
        timing->fallSlewTwo(), timing->fallSlewThree(),
        timing->fallSlewFour());

  if(timing->hasFallSlew2())
    fprintf(stdout, " FALL SLEW2 %g %g %g ;\n", timing->fallSlewFive(), timing->fallSlewSix(), timing->riseSlewSeven());

  if(timing->hasRiseIntrinsic()) 
	{
    fprintf(stdout, "TIMING RISE INTRINSIC %g %g ;\n",
        timing->riseIntrinsicOne(), timing->riseIntrinsicTwo());
    fprintf(stdout, "TIMING RISE VARIABLE %g %g ;\n",
        timing->riseIntrinsicThree(), timing->riseIntrinsicFour());
  }

  if(timing->hasFallIntrinsic()) 
	{
    fprintf(stdout, "TIMING FALL INTRINSIC %g %g ;\n",
        timing->fallIntrinsicOne(), timing->fallIntrinsicTwo());
    fprintf(stdout, "TIMING RISE VARIABLE %g %g ;\n",
        timing->fallIntrinsicThree(), timing->fallIntrinsicFour());
  }

  if(timing->hasRiseRS())
    fprintf(stdout, "TIMING RISERS %g %g ;\n", timing->riseRSOne(), timing->riseRSTwo());
  if (timing->hasRiseCS())
    fprintf(stdout, "TIMING RISECS %g %g ;\n", timing->riseCSOne(), timing->riseCSTwo());
  if (timing->hasFallRS())
    fprintf(stdout, "TIMING FALLRS %g %g ;\n", timing->fallRSOne(), timing->fallRSTwo());
  if (timing->hasFallCS())
    fprintf(stdout, "TIMING FALLCS %g %g ;\n", timing->fallCSOne(), timing->fallCSTwo());
  if (timing->hasUnateness())
    fprintf(stdout, "TIMING UNATENESS %s ;\n", timing->unateness());
  if (timing->hasRiseAtt1())
    fprintf(stdout, "TIMING RISESATT1 %g %g ;\n", timing->riseAtt1One(), timing->riseAtt1Two());
  if (timing->hasFallAtt1())
    fprintf(stdout, "TIMING FALLSATT1 %g %g ;\n", timing->fallAtt1One(), timing->fallAtt1Two());
  if (timing->hasRiseTo())
    fprintf(stdout, "TIMING RISET0 %g %g ;\n", timing->riseToOne(), timing->riseToTwo());
  if (timing->hasFallTo())
    fprintf(stdout, "TIMING FALLT0 %g %g ;\n", timing->fallToOne(), timing->fallToTwo());
  if (timing->hasSDFonePinTrigger())
    fprintf(stdout, " %s TABLEDIMENSION %g %g %g ;\n",
        timing->SDFonePinTriggerType(), timing->SDFtriggerOne(),
        timing->SDFtriggerTwo(), timing->SDFtriggerThree());
  if (timing->hasSDFtwoPinTrigger())
    fprintf(stdout, " %s %s %s TABLEDIMENSION %g %g %g ;\n",
        timing->SDFtwoPinTriggerType(), timing->SDFfromTrigger(),
        timing->SDFtoTrigger(), timing->SDFtriggerOne(),
        timing->SDFtriggerTwo(), timing->SDFtriggerThree());
  fprintf(stdout, "END TIMING\n");
  return 0;
}

int unitsCB(lefrCallbackType_e c, lefiUnits* unit, lefiUserData) 
{
  checkType(c);
  fprintf(stdout, "UNITS\n");
  if (unit->lefiUnits::hasDatabase())
    fprintf(stdout, "  DATABASE %s %g ;\n", unit->lefiUnits::databaseName(),
        unit->lefiUnits::databaseNumber());
  if (unit->lefiUnits::hasCapacitance())
    fprintf(stdout, "  CAPACITANCE PICOFARADS %g ;\n",
        unit->lefiUnits::capacitance());
  if (unit->lefiUnits::hasResistance())
    fprintf(stdout, "  RESISTANCE OHMS %g ;\n", unit->lefiUnits::resistance());
  if (unit->lefiUnits::hasPower())
    fprintf(stdout, "  POWER MILLIWATTS %g ;\n", unit->lefiUnits::power());
  if (unit->lefiUnits::hasCurrent())
    fprintf(stdout, "  CURRENT MILLIAMPS %g ;\n", unit->lefiUnits::current());
  if (unit->lefiUnits::hasVoltage())
    fprintf(stdout, "  VOLTAGE VOLTS %g ;\n", unit->lefiUnits::voltage());
  if (unit->lefiUnits::hasFrequency())
    fprintf(stdout, "  FREQUENCY MEGAHERTZ %g ;\n", unit->lefiUnits::frequency());
  //fprintf(stdout, "END UNITS\n");
  return 0;
}

int useMinSpacingCB(lefrCallbackType_e c, lefiUseMinSpacing* spacing, lefiUserData) 
{
  checkType(c);
  fprintf(stdout, "USEMINSPACING %s ", spacing->lefiUseMinSpacing::name());
  if(spacing->lefiUseMinSpacing::value())
    fprintf(stdout, "ON ;\n");
  else
    fprintf(stdout, "OFF ;\n");
  return 0;
}

int versionCB(lefrCallbackType_e c, double num, lefiUserData) 
{
  checkType(c);
  return 0;
}

int viaCB(lefrCallbackType_e c, lefiVia* via, lefiUserData) 
{
  checkType(c);
  lefVia(via);
  return 0;
}

int viaRuleCB(lefrCallbackType_e c, lefiViaRule* viaRule, lefiUserData) 
{
  int numLayers, numVias, i;
  lefiViaRuleLayer* vLayer;

  checkType(c);
  numLayers = viaRule->lefiViaRule::numLayers();
  // if numLayers == 2, it is VIARULE without GENERATE and has via name
  // if numLayers == 3, it is VIARULE with GENERATE, and the 3rd layer is cut
  for (i = 0; i < numLayers; i++) 
	{
    vLayer = viaRule->lefiViaRule::layer(i); 
    lefViaRuleLayer(vLayer);
  }

  if(numLayers == 2 && !(viaRule->lefiViaRule::hasGenerate())) 
	{
    // should have vianames
    numVias = viaRule->lefiViaRule::numVias();
    if(numVias == 0)
		{
      //fprintf(stderr, "Should have via names in VIARULE.\n");
		}
    else 
		{
      //for(i = 0; i < numVias; i++)
      //  fprintf(stdout, "  VIA %s ;\n", viaRule->lefiViaRule::viaName(i));
    }
  }
  if(viaRule->lefiViaRule::numProps() > 0) 
	{
    for(i = 0; i < viaRule->lefiViaRule::numProps(); i++) 
		{
      //fprintf(stdout, "%s ", viaRule->lefiViaRule::propName(i));
      //if(viaRule->lefiViaRule::propValue(i))
        //fprintf(stdout, "%s ", viaRule->lefiViaRule::propValue(i));
				
      switch(viaRule->lefiViaRule::propType(i)) 
			{
        case 'R': fprintf(stdout, "REAL ");
              break;
        case 'I': fprintf(stdout, "INTEGER ");
              break;
        case 'S': fprintf(stdout, "STRING ");
              break;
        case 'Q': fprintf(stdout, "QUOTESTRING ");
              break;
        case 'N': fprintf(stdout, "NUMBER ");
              break;
      } 
    }
    //fprintf(stdout, ";\n");
  }
  //fprintf(stdout, "END %s\n", viaRule->lefiViaRule::name());
  return 0;
}

int extensionCB(lefrCallbackType_e c, const char* extsn, lefiUserData) 
{
  checkType(c);
  return 0;
}

int doneCB(lefrCallbackType_e c, void*, lefiUserData) 
{
  checkType(c);
  return 0;
}

void errorCB(const char* msg) 
{
  printf ("%s : %s\n", lefrGetUserData(), msg);
}

void warningCB(const char* msg) 
{
  printf ("%s : %s\n", lefrGetUserData(), msg);
}

void* mallocCB(int size) 
{
  return malloc(size);
}

void* reallocCB(void* name, int size) 
{
  return realloc(name, size);
}

void freeCB(void* name) 
{
  free(name);
  return;
}

void lineNumberCB(int lineNo) 
{
  printf("Parsed %d number of lines!!\n", lineNo);
  return;
}

void printWarning(const char *str)
{
  printf("%s\n", str);
  //fprintf(stderr, "%s\n", str);
}

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    std::cout << "Please give input file" << std::endl;
    exit(0);
  }

  printf("Read lef %s \n", argv[1]);

  lefrInitSession(1);

  // sets the parser to be case sensitive...
  // default was supposed to be the case but false...
  // lefrSetCaseSensitivity(true);
  lefrSetWarningLogFunction(printWarning);
  lefrSetAntennaInputCbk(antennaCB);
  lefrSetAntennaInoutCbk(antennaCB);
  lefrSetAntennaOutputCbk(antennaCB);
  lefrSetArrayBeginCbk(arrayBeginCB);
  lefrSetArrayCbk(arrayCB);
  lefrSetArrayEndCbk(arrayEndCB);
  lefrSetBusBitCharsCbk(busBitCharsCB);
  lefrSetCaseSensitiveCbk(caseSensCB);
  lefrSetFixedMaskCbk(fixedMaskCB);
  lefrSetClearanceMeasureCbk(clearanceCB);
  lefrSetDensityCbk(densityCB);
  lefrSetDividerCharCbk(dividerCB);
  lefrSetNoWireExtensionCbk(noWireExtCB);
  lefrSetNoiseMarginCbk(noiseMarCB);
  lefrSetEdgeRateThreshold1Cbk(edge1CB);
  lefrSetEdgeRateThreshold2Cbk(edge2CB);
  lefrSetEdgeRateScaleFactorCbk(edgeScaleCB);
  lefrSetExtensionCbk(extensionCB);
  lefrSetNoiseTableCbk(noiseTableCB);
  lefrSetCorrectionTableCbk(correctionCB);
  lefrSetDielectricCbk(dielectricCB);
  lefrSetIRDropBeginCbk(irdropBeginCB);
  lefrSetIRDropCbk(irdropCB);
  lefrSetIRDropEndCbk(irdropEndCB);
  lefrSetLayerCbk(layerCB);
  lefrSetLibraryEndCbk(doneCB); 
  lefrSetMacroBeginCbk(macroBeginCB);
  lefrSetMacroCbk(macroCB);
  lefrSetMacroClassTypeCbk(macroClassTypeCB);
  lefrSetMacroOriginCbk(macroOriginCB);
  lefrSetMacroSizeCbk(macroSizeCB);
  lefrSetMacroFixedMaskCbk(macroFixedMaskCB);
  lefrSetMacroEndCbk(macroEndCB);
  lefrSetManufacturingCbk(manufacturingCB);
  lefrSetMaxStackViaCbk(maxStackViaCB);
  lefrSetMinFeatureCbk(minFeatureCB);
  lefrSetNonDefaultCbk(nonDefaultCB);
  lefrSetObstructionCbk(obstructionCB);
  lefrSetPinCbk(pinCB);
  lefrSetPropBeginCbk(propDefBeginCB);
  lefrSetPropCbk(propDefCB);
  lefrSetPropEndCbk(propDefEndCB);
  lefrSetSiteCbk(siteCB);
  lefrSetSpacingBeginCbk(spacingBeginCB);
  lefrSetSpacingCbk(spacingCB);
  lefrSetSpacingEndCbk(spacingEndCB);
  lefrSetTimingCbk(timingCB);
  lefrSetUnitsCbk(unitsCB);
  lefrSetUseMinSpacingCbk(useMinSpacingCB);
  lefrSetVersionCbk(versionCB);
  // lefrSetVersionStrCbk(versionStrCB); // when should I use this?
  lefrSetViaCbk(viaCB);
  lefrSetViaRuleCbk(viaRuleCB);
  lefrSetInputAntennaCbk(antennaCB);
  lefrSetOutputAntennaCbk(antennaCB);
  lefrSetInoutAntennaCbk(antennaCB);

  lefrSetLogFunction(errorCB);
  lefrSetWarningLogFunction(warningCB);
  lefrSetLineNumberFunction(lineNumberCB);
  lefrSetMallocFunction(mallocCB);
  lefrSetReallocFunction(reallocCB);
  lefrSetFreeFunction(freeCB);
  lefrSetRegisterUnusedCallbacks();

  lefrSetDeltaNumberLines(1000000);

  lefrSetAntennaInoutWarnings(30);
  lefrSetAntennaInputWarnings(30);
  lefrSetAntennaOutputWarnings(30);
  lefrSetArrayWarnings(30);
  lefrSetCaseSensitiveWarnings(30);
  lefrSetCorrectionTableWarnings(30);
  lefrSetDielectricWarnings(30);
  lefrSetEdgeRateThreshold1Warnings(30);
  lefrSetEdgeRateThreshold2Warnings(30);
  lefrSetEdgeRateScaleFactorWarnings(30);
  lefrSetInoutAntennaWarnings(30);
  lefrSetInputAntennaWarnings(30);
  lefrSetIRDropWarnings(30);
  lefrSetLayerWarnings(30);
  lefrSetMacroWarnings(30);
  lefrSetMaxStackViaWarnings(30);
  lefrSetMinFeatureWarnings(30);
  lefrSetNoiseMarginWarnings(30);
  lefrSetNoiseTableWarnings(30);
  lefrSetNonDefaultWarnings(30);
  lefrSetNoWireExtensionWarnings(30);
  lefrSetOutputAntennaWarnings(30);
  lefrSetPinWarnings(30);
  lefrSetSiteWarnings(30);
  lefrSetSpacingWarnings(30);
  lefrSetTimingWarnings(30);
  lefrSetUnitsWarnings(30);
  lefrSetUseMinSpacingWarnings(30);
  lefrSetViaRuleWarnings(30);
  lefrSetViaWarnings(30);

  lefrSetDeltaNumberLines(1000000);

//	// For each .lef, we should call this
//  lefrReset();
//  (void)lefrEnableReadEncrypted();
//
    char* file_name = argv[1];
//	char* userData;
//  FILE* file = fopen(file_name, "r");
//  int status = lefwInit(stdout); // initialize the lef writer,
//  int res    = lefrRead(file, file_name, (void*)userData);
//  fclose(file);
//
//  lefrClear();    

  db::dbDatabase _db;

	_db.readLef(std::filesystem::path(std::string(file_name)));

	return 0;
}
