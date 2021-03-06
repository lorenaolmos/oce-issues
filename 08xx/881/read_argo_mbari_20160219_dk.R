read.argo.mbari <- function(file)
{
  filename <- ""
  if (is.character(file)) {
    filename <- fullFilename(file)
    file <- file(file, "r")
    on.exit(close(file))
  }
  header <- readLines(file, encoding="latin1", n=100)
  isHeader <- grepl("^//", header, useBytes=TRUE)
  header <- header[isHeader]
  skip <- length(header)
  seek(file, 0, "start")
  data <- read.delim(file, sep="\t", skip=skip, encoding="latin1")
  names <- names(data)
  
  ## all possible fields:
  ## Nitrate[µM]
  ## Depth[m]
  ## mon/day/yr
  ## Salinity
  ## Temperature[°C]
  ## Density
  ## Oxygen[µM]
  ## OxygenSat[%]
  ## Chlorophyll[µg/l]
  ## BackScatter[/m/sr]
  ## CDOM[PPB]
  ## pHinsitu[Total]
  ## pH25C[Total]
  ## Lon [°E]
  ## Lat [°N]
  names <- gsub("Bot..Depth..m.", "waterDepth", names)
  names <- gsub("Cruise", "cruise", names)
  names <- gsub("Density", "density", names)
  names <- gsub("Depth.m.", "pressure", names)
  names <- gsub("Lat.*", "latitude", names)
  names <- gsub("Lon.*", "longitude", names)
  names <- gsub("Nitrate.*", "nitrate", names)
  names <- gsub("Oxygen\\..*", "oxygen", names)
  names <- gsub("OxygenSat.*", "oxygenSaturation", names)
  names <- gsub("Salinity", "salinity", names)
  names <- gsub("Station", "station", names)
  names <- gsub("Temperature.*", "temperature", names)
  names <- gsub("Type", "type", names)
  names <- gsub("Chlorophyll\\..*", "chlorophyll", names)
  names <- gsub("pHinsitu.Total.", "pHinsitu", names)
  names <- gsub("pH25C.Total.", "pH25C", names)
  names <- gsub("BackScatter\\..*", "backscatter", names)
  ## print(names)
  time <- as.POSIXct(paste(data$mon.day.yr, " ", data$hh.mm, ":00", sep=""), format="%m/%d/%Y %H:%M:%S", tz="UTC")
  
  isQF <- grepl("^QF", names)
  flags <- data[,isQF]
  names(flags) <- names[which(isQF)-1]
  data <- data[,!isQF]
  names(data) <- names[!isQF]
  ## Trim two cols that held time; add in a time column
##>  data <- data[, -which(names=="mon.day.yr"|names=="hh.mm")] # CR had no ","
  data$time <- time
  if (FALSE) { # DK tests
      threeBad <- which(is.na(time))[1:3]
      seek(file,0,"start")
      l <- readLines(file, 1000)
      l[skip+threeBad[1]+seq(-2,2)]
  }
  badLines <- data$cruise == ""
  if (sum(badLines) > 0) {
      data <- data[!badLines, ]
      flags <- data[!badLines, ]
      warning("deleted ", sum(badLines), " bad data lines")
  }
  D <- split(data, factor(data$station))
  F <- split(flags, factor(data$station))
  nprofiles <- length(D)
  nlevels <- length(D[[1]]$pressure)
  
  ## try making the data fields into matrices, much like how an argo
  ## object would be structured
  
  time <- unique(data$time)
  ## First try -- assume that each of the split profiles has the same
  ## number of pressure levels (even though the pressures are not
  ## exactly the same
  ## longitude <- matrix(data$longitude, ncol=nprofiles, nrow=nlevels)[1,]
  ## latitude <- matrix(data$latitude, ncol=nprofiles, nrow=nlevels)[1,]
  ## 
  ## FIXME: for the file 8486Hawaiiqc.txt this isn't the case! So,
  ## need to be smarter about how to build the matrices ... probably
  ## look for the profile with the most number of levels, create an
  ## empty matrix of that size, and then fill in the profiles so the
  ## last levels are missing
  maxPlevels <- max(unlist(lapply(D, function(x) length(x$pressure))))
  longitude <- unlist(lapply(D, function(x) x$longitude[1]), use.names = FALSE)
  latitude <- unlist(lapply(D, function(x) x$latitude[1]), use.names = FALSE)
  
  makeMatrix <- function(x, field) {
    n1 <- length(x)
    n2 <- max(unlist(lapply(x, function(x) length(x$pressure))))
    res <- matrix(NA, nrow=n1, ncol=n2)
    for (i in 1:length(x)) {
      n <- length(x[[i]][[field]])
      res[i, 1:n] <- rev(x[[i]][[field]])
    }
    return(t(res))
  }
  
  pressure <- makeMatrix(D, 'pressure')
  pressureFlag <- makeMatrix(F, 'pressure')
  temperature <- makeMatrix(D, 'temperature')
  temperatureFlag <- makeMatrix(F, 'temperature')
  salinity <- makeMatrix(D, 'salinity')
  salinityFlag <- makeMatrix(F, 'salinity')
  oxygen <- makeMatrix(D, 'oxygen')
  oxygenFlag <- makeMatrix(F, 'oxygen')
  oxygenSaturation <- makeMatrix(D, 'oxygenSaturation')
  oxygenSaturationFlag <- makeMatrix(F, 'oxygenSaturation')
  if ("nitrate" %in% names) {
    nitrate <- makeMatrix(D, 'nitrate')
    nitrateFlag <- makeMatrix(F, 'nitrate')
  }
  if ("chlorophyll" %in% names) {
    chlorophyll <- makeMatrix(D, 'chlorophyll')
    chlorophyllFlag <- makeMatrix(F, 'chlorophyll')
  }
  if ("backscatter" %in% names) {
      backscatter <- makeMatrix(D, 'backscatter')
      backscatterFlag <- makeMatrix(F, 'backscatter')
  }
  if ("pHinsitu" %in% names) {
     pHinsitu <- makeMatrix(D, 'pHinsitu')
     pHinsituFlag <- makeMatrix(F, 'pHinsitu')
  }
  if ("pH25C" %in% names) {
  pH25C <- makeMatrix(D, 'pH25C')
  pH25CFlag <- makeMatrix(F, 'pH25C')
  }

  d <- as.argo(time=time, longitude=ifelse(longitude>180, longitude-360, longitude),
               latitude=latitude,
               pressure=pressure, temperature=temperature, salinity=salinity,
               filename=filename, id="?")
  d <- oceSetData(d, 'oxygen', oxygen, units=list(unit=expression(mu*M), scale=""))
  d <- oceSetData(d, 'oxygenSaturation', oxygenSaturation, units=list(unit=expression(), scale=""))
  d <- oceSetMetadata(d, 'flags', list(pressure=pressureFlag, temperature=temperatureFlag,
                                       salinity=salinityFlag,oxygen=oxygenFlag,oxygenSaturation=oxygenSaturationFlag))
  if ("nitrate" %in% names)  {
    d <- oceSetData(d, 'nitrate', nitrate, units=list(unit=expression(mu*M), scale=""))
    d@metadata$flags$nitrate <- nitrateFlag
  }
  if ("chlorophyll" %in% names)  {
    d <- oceSetData(d, 'chlorophyll', chlorophyll, units=list(unit=expression(mu*g/l), scale=""))
    d@metadata$flags$chlorophyll <- chlorophyllFlag
  }
  if ("backscatter" %in% names) {
    d <- oceSetData(d, 'backscatter', backscatter, units=list(unit=expression(1/(m*sr)), scale=""))
    d@metadata$flags$backscatter <- backscatterFlag
  }
  if ("pHinsitu" %in% names)  {
    d <- oceSetData(d, 'pHinsitu', pHinsitu, units=list(unit=expression(Total), scale=""))
    d@metadata$flags$pHinsitu <- pHinsituFlag
  }
  if ("pH25C" %in% names)  {
    d <- oceSetData(d, 'pH25C', pH25C, units=list(unit=expression(Total), scale=""))
    d@metadata$flags$pH25C <- pH25CFlag
  }
  
  d
}


