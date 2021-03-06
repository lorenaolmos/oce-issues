## IMU accelerations
## use dolfyn-supplied test data, one file with IMU, the other without it.

rm(list=ls())
library(oce)

message("\n* ~/src/dolfyn/example_data/vector_data_imu01.VEC")
dimu <- read.oce("~/src/dolfyn/example_data/vector_data_imu01.VEC")
summary(dimu)
if (!interactive()) png("887d.png")
par(mfcol=c(3,1), mar=c(3, 3, 1, 1), mgp=c(2, 0.7, 0))
plot(dimu[['IMUtime']], dimu[['IMUaccelX']], type='l')
plot(dimu[['IMUtime']], dimu[['IMUaccelY']], type='l')
plot(dimu[['IMUtime']], dimu[['IMUaccelZ']], type='l')
if (!interactive()) dev.off()

