if (!interactive()) png("501.png")
library(oce)
d <- read.landsat("/data/archive/landsat/LE71910202005194ASN00",band="panchromatic",debug=3)
plot(d)
if (!interactive()) dev.off()

