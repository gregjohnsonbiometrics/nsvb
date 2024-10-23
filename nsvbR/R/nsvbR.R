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
#' v_ib <- volib( 1, 1, 201, "M240", 10, 60 )
#' v_ob <- volob( 1, 1, 201, "M240", 10, 60 )
#' biomass_components( 1, 1, 201, "M240", v_ib$volib, 10, 60 )
#' 
#' @export

biomass_components <- function( plot, tree, fia_spp, division, vtotib, dbh, height )
{
   bc <- compute_biomass_components( as.integer(plot), as.integer(tree), as.integer(fia_spp), division, vtotib, dbh, height ) 
   bc
}



#' 
#' @title green_tons() : computes green tons of a log segment or tree.
#' @name green_tons 
#'
#' @param plot     : integer | plot number
#' @param tree     : integer | tree number
#' @param fia_spp  : integer | FIA species code
#' @param division : string  | FIA ecological division
#' @param vtotob   : double  | total outside bark volume of tree or log (cubic feet)
#' @param vtotib   : double  | total inside bark volume of tree or log (cubic feet)
#'
#' @description
#' Compute green tons using  Miles, et al. 2009.
#' 
#' @return
#' Returns a data.frame with the green tons for each tree or log in following variables:
#' \itemize{
#'     \item plot : plot number
#'     \item tree : tree number
#'     \item green_tons : green tons in the tree or log
#' }
#'
#' @examples
#' # compute green tons for a Douglas-fir tree with 10 inch dbh and 60 foot height 
#' v_ib <- volib( 1, 1, 201, "", 10, 60 )
#' v_ob <- volob( 1, 1, 201, "", 10, 60 )
#' green_tons( 1, 1, 201, v_ob$volob, v_ib$volib )
#' 
#' @export

green_tons <- function( plot, tree, fia_spp, vtotob, vtotib )
{
    tons <- compute_green_tons( as.integer(plot), as.integer(tree), as.integer(fia_spp), vtotob, vtotib ) 
    tons
}



#' 
#' @title volib() : computes cubic foot volume inside bark of a tree
#' @name volib 
#'
#' @param plot     : integer | plot number
#' @param tree     : integer | tree number
#' @param fia_spp  : integer | FIA species code
#' @param division : string  | FIA ecological division
#' @param dbh      : double  | diameter inside bark (inches)
#' @param height   : double  | total height (feet)
#'
#' @description
#' Compute cubic foot volume inside bark using the National Scale Volume and Biomass estimators (NSVB).
#' 
#' @return
#' Returns a data.frame with the cubic foot volume for each tree in following variables:
#' \itemize{
#'     \item plot : plot number
#'     \item tree : tree number
#'     \item volib : cubic foot volume inside bark
#' }
#'
#' @examples
#' # compute cubic foot volume inside bark for a Douglas-fir tree with 10 inch dbh and 60 foot height 
#' volib( 1, 1, 201, "M240", 10, 60 )
#' 
#' @export

volib <- function( plot, tree, fia_spp, division, dbh, height )
{
    vib <- compute_volib( as.integer(plot), as.integer(tree), as.integer(fia_spp), division, dbh, height ) 
    vib
}


#' 
#' @title volob() : computes cubic foot volume outside bark of a tree
#' @name volob 
#'
#' @param plot     : integer | plot number
#' @param tree     : integer | tree number
#' @param fia_spp  : integer | FIA species code
#' @param division : string  | FIA ecological division
#' @param dbh      : double  | diameter inside bark (inches)
#' @param height   : double  | total height (feet)
#'
#' @description
#' Compute cubic foot volume outside bark using the National Scale Volume and Biomass estimators (NSVB).
#' 
#' @return
#' Returns a data.frame with the cubic foot volume for each tree in following variables:
#' \itemize{
#'     \item plot : plot number
#'     \item tree : tree number
#'     \item volob : cubic foot volume outside bark
#' }
#'
#' @examples
#' # compute cubic foot volume outside bark for a Douglas-fir tree with 10 inch dbh and 60 foot height 
#' volob( 1, 1, 201, "M240", 10, 60 )
#' 
#' @export

volob <- function( plot, tree, fia_spp, division, dbh, height )
{
    vob <- compute_volob( as.integer(plot), as.integer(tree), as.integer(fia_spp), division, dbh, height ) 
    vob
}
