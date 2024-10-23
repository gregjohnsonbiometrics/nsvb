
#include <Rcpp.h>
#include "../../src/nsvb.hpp"
#include <string>

using namespace Rcpp;

// [[Rcpp::export]]
Rcpp::DataFrame compute_biomass_components( 
        IntegerVector plot,
        IntegerVector tree,
        IntegerVector fia_spp, 
        StringVector  division,
        NumericVector vtotib,
        NumericVector dbh, 
        NumericVector height ) 
{
    size_t n = fia_spp.size();
    
    std::vector<double> wood( n, 0.0 );
    std::vector<double> bark( n, 0.0 );
    std::vector<double> branch( n, 0.0 );
    std::vector<double> foliage( n, 0.0 );
    std::vector<double> total( n, 0.0 );
    std::vector<double> above_ground_biomass( n, 0.0 );
    
    for( size_t i = 0; i < n; i++ )
    {
        auto d = std::string( division[i] );
        auto bc = biomass_components( fia_spp[i], d, vtotib[i], dbh[i], height[i] );
        
        wood[i] = bc.wood;
        bark[i] = bc.bark;
        branch[i] = bc.branch;
        foliage[i] = bc.foliage;
        total[i] = bc.total;
        above_ground_biomass[i] = bc.above_ground_biomass;
    }
    
    Rcpp::DataFrame bcDF = Rcpp::DataFrame::create(
        Rcpp::Named("plot") = plot,
        Rcpp::Named("tree") = tree,
        Rcpp::Named("wood") = wood,
        Rcpp::Named("bark") = bark,
        Rcpp::Named("branch") = branch,
        Rcpp::Named("foliage") = foliage,
        Rcpp::Named("total") = total,
        Rcpp::Named("above_ground_biomass") = above_ground_biomass );
    
    return Rcpp::wrap(bcDF);
}

