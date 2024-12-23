
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


// [[Rcpp::export]]
Rcpp::DataFrame compute_green_tons( 
        IntegerVector plot,
        IntegerVector tree,
        IntegerVector fia_spp, 
        NumericVector vtotob,
        NumericVector vtotib ) 
{
    size_t n = fia_spp.size();
    
    std::vector<double> green_tons( n, 0.0 );

    for( size_t i = 0; i < n; i++ )
        green_tons[i] = compute_green_tons( fia_spp[i], vtotob[i], vtotib[i] );

    Rcpp::DataFrame tonsDF = Rcpp::DataFrame::create(
        Rcpp::Named("plot") = plot,
        Rcpp::Named("tree") = tree,
        Rcpp::Named("green_tons") = green_tons );
    
    return Rcpp::wrap(tonsDF);
}


// [[Rcpp::export]]
Rcpp::DataFrame compute_volib( 
        IntegerVector plot,
        IntegerVector tree,
        IntegerVector fia_spp, 
        StringVector  division,        
        NumericVector dbh,
        NumericVector height ) 
{
    size_t n = fia_spp.size();
    
    std::vector<double> volib( n, 0.0 );
    
    for( size_t i = 0; i < n; i++ )
    {
        auto d = std::string( division[i] );
        volib[i] = compute_volib( fia_spp[i], d, dbh[i], height[i] );
        
    }
    
    Rcpp::DataFrame volibDF = Rcpp::DataFrame::create(
        Rcpp::Named("plot") = plot,
        Rcpp::Named("tree") = tree,
        Rcpp::Named("volib") = volib );

    
    return Rcpp::wrap(volibDF);
}


// [[Rcpp::export]]
Rcpp::DataFrame compute_volob( 
        IntegerVector plot,
        IntegerVector tree,
        IntegerVector fia_spp, 
        StringVector  division,        
        NumericVector dbh,
        NumericVector height ) 
{
    size_t n = fia_spp.size();
    
    std::vector<double> volob( n, 0.0 );
    
    for( size_t i = 0; i < n; i++ )
    {
        auto d = std::string( division[i] );
        volob[i] = compute_volob( fia_spp[i], d, dbh[i], height[i] );
        
    }
    
    Rcpp::DataFrame volobDF = Rcpp::DataFrame::create(
        Rcpp::Named("plot") = plot,
        Rcpp::Named("tree") = tree,
        Rcpp::Named("volob") = volob );
        
        
    return Rcpp::wrap(volobDF);
}
