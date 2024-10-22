
#include "nsvb.hpp"
#include <vector>
#include <iostream>
 #include <iomanip>

int main( int argc, char **argv )
{
    // test biomass equations for an array of FIA species
    std::vector<int> species = {10, 14, 17, 20, 40, 43, 52, 55, 58, 61, 64, 67, 70, 73, 91, 94, 97, 101, 104, 107, 110, 113, 116, 
                               119, 122, 125, 128, 131, 134, 137, 140, 143, 201, 212, 222, 231, 241, 251, 261, 264, 303, 311, 314, 
                               317, 320, 323, 332, 336, 345, 352, 356, 360, 363, 371, 374, 378, 391, 402, 405, 408, 411, 420, 423, 
                               450, 460, 463, 481, 492, 502, 505, 508, 511, 514, 522, 540, 543, 546, 549, 552, 571, 582, 600, 603, 
                               606, 631, 651, 654, 658, 662, 680, 683, 691, 694, 712, 722, 731, 741, 744, 747, 752, 756, 760, 763, 
                               766, 770, 773, 801, 804, 807, 810, 813, 816, 819, 822, 825, 828, 831, 834, 837, 840, 843, 846, 852, 
                               855, 858, 863, 866, 869, 874, 882, 885, 888, 895, 901, 907, 911, 914, 920, 923, 926, 929, 935, 940, 
                               952, 971, 974, 977, 986, 989, 992, 995, 998 };
    
    // and for an array of ecological division
    std::vector<std::string> divisions = { "", "130", "210", "220", "230", "240", "260", "340", "M210", "M220", "M230", "M240", "M260", "M310", "M330"};

    // total volume inside bark
    int fia_spp = 202;
    double dbh = 10.0;       // inches
    double height = 60.0;    // feet
    std::string d = "M240";  // division

    try {   
        if( argc > 3 )
        {
            // get species, dbh, and height
            
                fia_spp = atof( argv[1] );
                dbh = std::atof( argv[2] );
                height = std::atof( argv[3] );

        } else if( argc > 4 ) {
            d = std::string( argv[4] );
        }
    } catch( std::exception &e ) {
        return -1;
    }

    std::cout << "species = " << fia_spp << ", division = " << d << ", dbh = " << dbh << ", height = " << height << "\n";

    auto vib = compute_volib( fia_spp, d, dbh, height );
    auto vob = compute_volob( fia_spp, d, dbh, height );  
    std::cout << "Total Volume Inside Bark (cubic feet) = " << std::setprecision(4) << vib << "\n";
    std::cout << "Total Volume Outside Bark (cubic feet) = " << std::setprecision(4) << vob << "\n";
  
    std::cout << "Green Lbs = " << std::setprecision(4) << compute_green_tons( fia_spp,  vob, vib )*2000.0 << "\n";

    std::cout << "Biomass\n";
    auto bm = biomass_components( fia_spp, d, vib, dbh, height );
    std::cout << "\tAGB     = \t" << std::setprecision(4) << bm.above_ground_biomass << "\n";
    std::cout << "\tBark    = \t" << std::setprecision(4) << bm.bark << "\n";
    std::cout << "\tBranch  = \t" << std::setprecision(4) << bm.branch << "\n";
    std::cout << "\tfoliage = \t" << std::setprecision(4) << bm.foliage << "\n";
    std::cout << "\twood    = \t" << std::setprecision(4) << bm.wood << "\n";
    std::cout << "\ttotal   = \t" << std::setprecision(4) << bm.total << "\n";

    return 0;
}