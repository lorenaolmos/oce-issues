library(oce)
library(testthat)
data(cm)
if (!interactive()) png("740a.png")
plot(cm)
if (!interactive()) dev.off()
