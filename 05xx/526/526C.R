if (!interactive()) png("526C.png")
library(oce)
data(coastlineWorld)
par(mfrow=c(2,1), mar=c(3, 3, 1, 1))
mapPlot(coastlineWorld, proj="+proj=moll")
mtext("EXPECT: no axes", font=2, col='purple', adj=0)
mtext("proj4", adj=1)
mapPlot(coastlineWorld, proj="mollweide")
mtext("mapproj", adj=1)
if (!interactive()) dev.off()

