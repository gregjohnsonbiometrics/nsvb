// National Scale Volume and Biomass estimators (NSVB) 
//
// Westfall, James A.; Coulston, John W.; Gray, Andrew N.; Shaw, John D.; Radtke, Philip J.; Walker, David M.; 
// Weiskittel, Aaron R.; MacFarlane, David W.; Affleck, David L.R.; Zhao, Dehai; Temesgen, Hailemariam; 
// Poudel, Krishna P.; Frank, Jereme M.; Prisley, Stephen P.; Wang, Yingfang; Sánchez Meador, Andrew J.; 
// Auty, David; Domke, Grant M. 2024. A national-scale tree volume, biomass, and carbon modeling system for the
// United States. Gen. Tech. Rep. WO-104. Washington, DC: U.S. Department of Agriculture, Forest Service. 37 p.
// https://doi.org/10.2737/WO-GTR-104. 
//
// Converted to C++ 
// Greg Johnson Biometrics LLC
// 10-30-2023

#ifndef NSVB
#define NSVB

#include "nsvb_coef.hpp"

// biomass components (lbs)
struct BIOMASS_COMP {
    double wood = 0.0;
    double bark = 0.0;
    double branch = 0.0;
    double foliage = 0.0;
    double total = 0.0;
    double above_ground_biomass = 0.0;
};

// compute a biomass component (pounds)
//  FIA species code
//  Coefficients
//  Wood specific gravity
//  dbh (inches)
//  height (feet)
double biomass( int fia_spp, const COEFS &coefs, double wood_sg, double dbh, double height );

// compute biomass components in pounds given:
//  FIA species code
//  FIA ecological division. Recognized divisions:
//      130  210  220  230  240  260  340  M210 M220 M230 M240 M260 M310 M330
//  total inside bark volume of tree (top and stump) (cubic feet)
//  dbh (inches)
//  height (feet)
BIOMASS_COMP biomass_components( int fia_spp, std::string division, double vtotib, double dbh, double height );

// compute green tons outside bark of log given:
// FIA species code
// total outside bark volume of merchantable stem (cubic feet)
// total inside bark volume of merchantable stem (cubic feet)
double compute_green_tons( int fia_spp, double cfvolob, double cfvolib );

// compute_volib() : compute total cubic volume inside bark (cubic feet) (FIA equations)
// inputs:
//      fia_spp : FIA numeric species code
//      division : FIA eco-region code or blank
//      dbh : diameter inside bark (inches)
//      height : total height (feet)
double compute_volib( int fia_spp, std::string division, double dbh, double height );

// compute_volob() : get total cubic volume outside bark (cubic feet) (FIA equations)
// inputs:
//      fia_spp : FIA numeric species code
//      division : FIA eco-region code or blank
//      dbh : diameter inside bark (inches)
//      height : total height (feet)
double compute_volob( int fia_spp, std::string division, double dbh, double height );

#endif