library(oce)
data(coastlineWorld)
if (!interactive()) png("546.png")
mapPlot(coastlineWorld, projection="+proj=wintri", fill="gray")
if (!interactive()) dev.off()

