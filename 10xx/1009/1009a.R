library(oce)
data(ctd)
ctd <- oceSetData(ctd, 'junk', rnorm(ctd[['pressure']]), list(unit=expression(JNK), scale=''))
if (!interactive()) png("1009a.png")
par(mfrow=c(2,2))
plotProfile(ctd, xtype='junk')
plotProfile(ctd, xtype='junk', xlim=c(-5, 5))
plotProfile(ctd, xtype='temperature')
plotProfile(ctd, xtype='temperature', Tlim=c(0, 30))
if (!interactive()) dev.off()
