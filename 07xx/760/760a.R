library(oce)
library(testthat)
d <- read.oce("example_ct1.csv")
if (!"flags" %in% names(d@metadata))
    stop("should be storing 'flags' into the metadata' slot")
expect_equal(sort(names(d@data)), c("oxygen", "pressure", "salinity", "sigmaTheta", "temperature"))
expect_equal(sort(names(d@metadata$flags)), c("oxygen", "pressure", "salinity", "temperature"))

