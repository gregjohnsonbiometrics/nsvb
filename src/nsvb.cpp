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

#include <map>
#include <math.h>
#include <iostream>
#include "nsvb.hpp"

//////////////////////////////////////////////////////////////////////////////////

// NSVB equation forms
double equation3( double dbh, double total_height, const COEFS &coefs ) 
{ 
    double x = coefs.a * std::pow(dbh,coefs.b) * std::pow(total_height,coefs.c); 
    return x;
}

double equation31( double dbh, double total_height, double wood_sg, const COEFS &coefs ) 
{ 
    double x = coefs.a * std::pow(dbh,coefs.b) * std::pow(total_height,coefs.c) * wood_sg; 
    return x;
}

double equation4( double dbh, double total_height, double k, const COEFS &coefs ) 
{ 
    double x = dbh < k ? coefs.a0 * std::pow(dbh,coefs.b0) * std::pow(total_height,coefs.c) : 
                         coefs.a0 * std::pow(k,(coefs.b0 - coefs.b1)) * std::pow(dbh,coefs.b1) * std::pow(total_height,coefs.c);
    return x;
}

// eq.5 <- function(DBH,THT) { a * DBH^(a1 * (1 - exp(-b1 * (DBH)))^c1) * THT^c }
// double equation5( double dbh, double total_height, const COEFS &coefs )
// { 
//     double x = coefs.a * std::pow(dbh, coefs.a1 * std::pow(1.0 - std::exp(-coefs.b1 * (dbh)),coefs.c1)) * std::pow(total_height,coefs.c); 
//     return x;
// }

double equation6( double X, double alpha, double beta ) 
{ 
    double x = std::pow(1.0 - std::pow(1.0 - X,alpha),beta);
    return x;
}

double equation50( double dbh, double total_height, const COEFS &coefs )
{ 
    double x = coefs.a * std::pow(dbh,coefs.b) * std::pow(total_height,coefs.c) * std::exp(-(coefs.b2 * dbh));
    return x;
}

//////////////////////////////////////////////////////////////////////////////////

// compute a biomass component (pounds)
//   FIA species code
//   Coefficients
//   Wood specific gravity
//   dbh (inches)
//   height (feet)
double biomass( int fia_spp, const COEFS &coefs, double wood_sg, double dbh, double height )
{
    double biomass = 0.0;

    try {
        switch( coefs.equation ) {
            case 3:
                biomass = equation3( dbh, height, coefs );
                break;
            case 31:
                biomass = equation31( dbh, height, wood_sg, coefs );
                break;
            case 4: {
                // k=9 for softwood trees and k=11 for hardwoods
                double k = fia_spp < 300 ? 9.0 : 11.0;

                biomass = equation4( dbh, height, k, coefs ); 
                break; }
            case 50:
                biomass = equation50( dbh, height, coefs );
                break;
        }
    } catch( const std::exception &e ) {
        throw( e );
    }

    return biomass;
}

// compute biomass components in pounds given:
//   FIA species code
//   FIA ecological division. Recognized divisions:
//       130  210  220  230  240  260  340  M210 M220 M230 M240 M260 M310 M330
//   total inside bark volume of tree (top and stump) (cubic feet)
//   dbh (inches)
//   height (feet)
// NOTE: those species defaulting to Jenkins what are woodland species (code 10) return 0.0 pounds
// 
BIOMASS_COMP biomass_components( int fia_spp, std::string division, double vtotib, double dbh, double height )
{
    try {
        BIOMASS_COMP bc;

        // use other live tree species code if species not found
        if( refs.find( fia_spp ) == refs.end() )
            fia_spp = 999;

        const REFS &r = refs.at( fia_spp );
        int jspp = r.Jenkins_spcd;

        bc.wood = vtotib * r.wood_sg * 62.4;

        bool found = false;
        if( division_bark_coefs.find( division ) != division_bark_coefs.end() )
        {
            if( division_bark_coefs.at(division).find(fia_spp) != division_bark_coefs.at(division).end() )
            {
                bc.bark = biomass( fia_spp, division_bark_coefs.at(division).at(fia_spp), r.wood_sg, dbh, height );
                found = true;
            }
        }

        if( !found )
        {
            if( bark_coefs.find(fia_spp) != bark_coefs.end() )
                bc.bark = biomass( fia_spp, bark_coefs.at(fia_spp), r.wood_sg, dbh, height );
            else 
                bc.bark = jspp < 10 ? biomass( jspp, jenkins_bark_coefs.at(jspp), r.wood_sg, dbh, height ) : 0.0;
        }

        ///////////////

        found = false;
        if( division_branch_coefs.find( division ) != division_branch_coefs.end() )
        {
            if( division_branch_coefs.at(division).find(fia_spp) != division_branch_coefs.at(division).end() )
            {
                bc.branch = biomass( fia_spp, division_branch_coefs.at(division).at(fia_spp), r.wood_sg, dbh, height );
                found = true;
            }
        }

        if( !found )
        {
            if( branch_coefs.find(fia_spp) != branch_coefs.end() )
                bc.branch = biomass( fia_spp, branch_coefs.at(fia_spp), r.wood_sg, dbh, height );
            else 
                bc.branch = jspp < 10 ? biomass( jspp, jenkins_branch_coefs.at(jspp), r.wood_sg, dbh, height ) : 0.0;
        }

        ///////////////
        
        found = false;
        if( division_foliage_coefs.find( division ) != division_foliage_coefs.end() )
        {
            if( division_foliage_coefs.at(division).find(fia_spp) != division_foliage_coefs.at(division).end() )
            {
                bc.foliage = biomass( fia_spp, division_foliage_coefs.at(division).at(fia_spp), r.wood_sg, dbh, height );
                found = true;
            }
        } 
        
        if( !found )
        {
            if( foliage_coefs.find(fia_spp) != foliage_coefs.end() )
                bc.foliage = biomass( fia_spp, foliage_coefs.at(fia_spp), r.wood_sg, dbh, height );
            else
                bc.foliage = jspp < 10 ? biomass( jspp, jenkins_foliage_coefs.at(jspp), r.wood_sg, dbh, height ) : 0.0;
        }

        ///////////////
        
        found = false;
        if( division_total_coefs.find( division ) != division_total_coefs.end() )
        {
            if( division_total_coefs.at(division).find(fia_spp) != division_total_coefs.at(division).end() )
            {
                bc.total = biomass( fia_spp, division_total_coefs.at(division).at(fia_spp), r.wood_sg, dbh, height );
                found = true;
            }    
        }
        
        if( !found )
        {
            if( total_coefs.find(fia_spp) != total_coefs.end() )
                bc.total = biomass( fia_spp, total_coefs.at(fia_spp), r.wood_sg, dbh, height );
            else {
                bc.total = jspp < 10 ? biomass( jspp, jenkins_total_coefs.at(jspp), r.wood_sg, dbh, height ) : 0.0;
                std::cout << "used jenkins total: " << bc.total << "\n";
            }
        }

        //////////////////////////////////////////////
        double TotalC = bc.wood + bc.bark + bc.branch;
        
        double Diff = bc.total - TotalC;
        double WoodR = bc.wood / TotalC;
        double BarkR = bc.bark / TotalC;
        double BranchR = bc.branch / TotalC;

        double WoodAdd = Diff * WoodR;
        double BarkAdd = Diff * BarkR;
        double BranchAdd = Diff * BranchR;

        bc.wood += WoodAdd;
        bc.bark += BarkAdd;
        bc.branch += BranchAdd;

        bc.above_ground_biomass = bc.total + bc.foliage;

        return bc;
    } catch( const std::exception &e ) {
        throw;
    }
}

// Compute green tons outside bark of log
// From: Matt Russell July 27, 2022 
// (https://arbor-analytics.com/post/2022-07-27-simple-volume-to-weight-conversion-for-us-tree-species/index.html)
//
// inputs:
//    FIA species code
//    total outside bark volume of merchantable stem (cubic feet)
//    total inside bark volume of merchantable stem (cubic feet)
double compute_green_tons( int fia_spp, double cfvolob, double cfvolib )
{
    double green_tons = -1.0;

    try {
        // use other live tree species code if species not found
        if( refs.find( fia_spp ) == refs.end() )
            fia_spp = 999;

        const REFS &r = refs.at( fia_spp );

        green_tons = (cfvolib * ((r.wood_sg*1000.0) * (1.0 + (r.mc_pct_green_wood/100.0))) * 2.2046 / 35.3145 +
                        (cfvolob-cfvolib) * ((r.bark_sg*1000.0) * (1.0 + (r.mc_pct_green_bark/100.0))) * 2.2046 / 35.3145) / 2000.0;
    } catch( const std::exception &e ) {
        throw;
    }

    return green_tons;
}

// compute_volib() : get total cubic volume inside bark (cubic feet) (FIA equations)
// inputs:
//      tree_id : tree identifier
//      fia_spp : FIA numeric species code
//      division : FIA eco-region code or blank
//      dbh : diameter inside bark (inches)
//      height : total height (feet)
// NOTE: returns 0.0 cubic feet for woodland (juniper) species 58 - 69
double compute_volib( int fia_spp, std::string division, double dbh, double height )
{
    try {
        // use other live tree species code if species not found
        if( refs.find( fia_spp ) == refs.end() )
            fia_spp = 999;

        const REFS &r = refs.at( fia_spp );
    
        int jspp = r.Jenkins_spcd;

        if( division_volib_coefs.find( division ) != division_volib_coefs.end() )
        {
            if( division_volib_coefs.at(division).find(fia_spp) != division_volib_coefs.at(division).end() )
                return biomass( fia_spp, division_volib_coefs.at(division).at(fia_spp), r.wood_sg, dbh, height );
        }
        
        if( volib_coefs.find(fia_spp) != volib_coefs.end() )
            return biomass( fia_spp, volib_coefs.at(fia_spp), r.wood_sg, dbh, height );
        else 
            return jspp < 10 ? biomass( jspp, jenkins_volib_coefs.at(jspp), r.wood_sg, dbh, height ) : 0.0;

    } catch( const std::exception &e ) {
        throw;
    }
}


// compute_volob() : get total cubic volume outside bark (cubic feet) (FIA equations)
// inputs:
//      tree_id : tree identifier
//      fia_spp : FIA numeric species code
//      division : FIA eco-region code or blank
//      dbh : diameter inside bark (inches)
//      height : total height (feet)
// NOTE: returns 0.0 cubic feet for woodland (juniper) species 58 - 69
double compute_volob( int fia_spp, std::string division, double dbh, double height )
{
    try {
        // use other live tree species code if species not found
        if( refs.find( fia_spp ) == refs.end() )
            fia_spp = 999;
            
        const REFS &r = refs.at( fia_spp );
        int jspp = r.Jenkins_spcd;

        if( division_volob_coefs.find( division ) != division_volob_coefs.end() )
        {
            if( division_volob_coefs.at(division).find(fia_spp) != division_volob_coefs.at(division).end() )
                return biomass( fia_spp, division_volob_coefs.at(division).at(fia_spp), r.wood_sg, dbh, height );
        }
        
        if( volob_coefs.find(fia_spp) != volob_coefs.end() )
            return biomass( fia_spp, volob_coefs.at(fia_spp), r.wood_sg, dbh, height );
        else
            return jspp < 10 ? biomass( jspp, jenkins_volob_coefs.at(jspp), r.wood_sg, dbh, height ) : 0.0;
    } catch( const std::exception &e ) {
        throw;
    }        
}
