# nsvb
A C++ implementation of National Scale Volume and Biomass estimators (`NSVB`) 

## Introduction

The National Scale Volume and Biomass Estimators (`NSVB`)^[Westfall, James A.; Coulston, John W.; Gray, Andrew N.; Shaw, John D.; Radtke, Philip J.; Walker, David M.; Weiskittel, Aaron R.; MacFarlane, David W.; Affleck, David L.R.; Zhao, Dehai; Temesgen, Hailemariam; Poudel, Krishna P.; Frank, Jereme M.; Prisley, Stephen P.; Wang, Yingfang; Sánchez Meador, Andrew J.; Auty, David; Domke, Grant M. 2024. A national-scale tree volume, biomass, and carbon modeling system for the United States. Gen. Tech. Rep. WO-104. Washington, DC: U.S. Department of Agriculture, Forest Service. 37 p. https://doi.org/10.2737/WO-GTR-104.] were implemented in R^[R Core Team (2024). _R: A Language and Environment for Statistical Computing_. R Foundation for Statistical Computing, Vienna, Austria. <https://www.R-project.org/>] by Virginia Tech. The performance of the code is hampered by the interpretive nature of R. To provide a higher performance API the Virginia Tech code is implemented in C++23.

## Biomass Components Estimated

The dry weight in pounds is estimated for the following tree components:

* Wood
* Bark
* Branches
* Foliage
* Total

Additionally, aboveground biomass (AGB) is computed.

## Additional Estimators

Functions are included to compute total cubic foot volume inside and outside bark using the `NSCB` system.

## Application Programming Interface

The following functions are exposed:

* `biomass_components`: Computes biomass components in pounds given: 
   * FIA numeric species code (`fia_spp`), 
   * FIA ecological division^[Ecological divisions specifically recognized are: 130  210  220  230  240  260  340  M210 M220 M230 M240 M260 M310 M330] (`division`), 
   * total inside bark volume of tree (top and stump) (`vtotib`), 
   * diameter at breast height (`dbh`) in inches, and 
   * total height (`tht`) in feet.
* `compute_green_tons`: Compute green tons outside bark of a log given:
   * `fia_spp`, 
   * total outside bark volume of merchantable stem (cubic feet) (`cfvolob`),
   * total outside bark volume of merchantable stem (cubic feet) (`cfvolib`).
* `compute_volib`: Computes total cubic volume inside bark (`vtotib`) (cubic feet) given:
   * `fia_spp`,
   * `division`,
   * `dbh`,
   * `tht`.
* `compute_volob`: Computes total cubic volume outside bark (`vtotob`) (cubic feet) given:
   * `fia_spp`,
   * `division`,
   * `dbh`,
   * `tht`.

`biomass_components` returns a `BIOMASS_COMP` structure defined in `carbon.hpp`. 

## Compilation

A simple program is available to test your compilation. It accepts `fia_spp`, `dbh`, `tht`, and optionally `division` on the command line. The `makefile` can compile the test program using the target `test`.

A dynamic link library can be created using the target `dll`.

## Notes

1. The `NSVB` makes a direct estimate of total biomass. A separate estimate of total biomass is computed as a sum of the biomass components of the tree. The component estimates are rebalanced to scale to the direct estimate.
2. Green tons are estimated using specific gravity and moisture content values from Miles and Smith (2009)^[Miles, Patrick D.; Smith, W. Brad. 2009. Specific gravity and other properties of wood and bark for 156 tree species found in North America. Res. Note NRS-38. Newtown Square, PA: U.S. Department of Agriculture, Forest Service, Northern Research Station. 35 p. https://doi.org/10.2737/NRS-RN-38]