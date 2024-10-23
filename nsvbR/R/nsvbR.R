#' nsvbR
#'
#' Provides access to C++ implementation of the National Scale Volume and Biomass estimators (NSVB).
#' 
#' Developed by:
#' Greg Johnson
#' Greg Johnson Biometrics LLC
#' (c) October 2024
#'
"_PACKAGE"

#' @importFrom Rcpp evalCpp
#' @useDynLib nsvbR


#' 
#' @title biomass_components() : compute biomass components in pounds.
#' @name biomass_components 
#'
#' @param plot     : integer | plot number
#' @param tree     : integer | tree number
#' @param fia_spp  : integer | FIA species code
#' @param division : string  | FIA ecological division
#' @param vtotib   : double  | total inside bark volume of tree (top and stump) (cubic feet)
#' @param dbh      : double  | diameter inside bark (inches)
#' @param height   : double  | total height (feet)
#'
#' @description
#' Compute biomass components in pounds using the National Scale Volume and Biomass estimators (NSVB).
#' 
#' @note
#' \itemize{
#'     \item Returns 0.0 pounds for those species defaulting to the Jenkins equation that are woodland species (code 10).
#'     \item Recognized ecological divisions: "blank", 130, 210, 220, 230, 240, 260, 340, M210, M220, M230, M240, M260, M310, M330.
#' }
#'
#' @return
#' Returns a data.frame with the biomass components for each tree in following variables:
#' \itemize{
#'     \item plot : plot number
#'     \item tree : tree number
#'     \item wood : weight of biomass in wood 
#'     \item bark : weight of biomass in bark
#'     \item branch : weight of biomass in branches
#'     \item foliage : weight of biomass in foliage
#'     \item total : total weight of all components except foliage
#'     \item above_ground_biomass : total weight of above ground biomass including foliage
#' }
#'
#' @examples
#' # compute biomass components for a Douglas-fir tree with 10 inch dbh and 60 foot height in Ecological division M240
#' biomass_components( 1, 1, 201, "M240", 12.21, 10, 60 )
#' 
#' @export

biomass_components <- function( plot, tree, fia_spp, division, vtotib, dbh, height )
{
   bc <- compute_biomass_components( as.integer(plot), as.integer(tree), as.integer(fia_spp), division, vtotib, dbh, height ) 
   bc
}
