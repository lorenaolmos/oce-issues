if (!interactive()) png("404A.png")
set.seed(404)
library(oce)
library(ocedata)
data(coastlineWorld)
par(mar=rep(1/2, 4), mfrow=c(1,1))
mapPlot(coastlineWorld)
d <- array(rnorm(10*5), dim=c(10,5))
lon <- seq(-90, 90, length.out=10)
lat <- seq(45, -45, length.out=5)
mapContour(lon, rev(lat), d, levels=0, lwd=8, col='lightgray')
mapContour(lon, lat, d, levels=0, col='red')
mtext("EXPECT: @richardsc -- can you fill in this sentence?")
if (!interactive()) dev.off()

