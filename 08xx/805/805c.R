fin <- par('fin')
## small fractions yield figure margin errors
width <- fin[1]*0.5 # EXPERIMENT: adjust this

m <- 1 # 1 inch margin
left <- m + (fin[1] - width)
right <- m
par(mai=c(m, left, m, right))
plot(1:3)
message("par('fin')=c(", paste(round(par('fin'),1), collapse=','),")")
message("par('mai')=c(", paste(round(par('mai'),1), collapse=','),")")
message("par('fin')[1]-left-right=", round(par('fin')[1]-left-right, 1))

message("Even if last-printed number is < about 3.3, figure-margin error ensues")
message("But why do we need 3.3 inches for this?")
message("par('cin'): ", paste(par('cin'), collapse=","))
