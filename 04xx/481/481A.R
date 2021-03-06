if (!interactive()) png("481A.png")
library(oce)
data(section)
GS <- subset(section, 109<=stationId&stationId<=129)
GS <- sectionSort(GS, by="longitude")
GSG <- sectionGrid(GS, p=seq(0, 1600, 25))
plot(GSG, which=c(1,99), map.xlim=c(-80,-60))
if (!interactive()) dev.off()

