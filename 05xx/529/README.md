## proj4 style

Insert + when done; see list for mapproj below, which is easier for checking
progress on coding.

+ "aea"
"aeqd"
"airy"
+ "aitoff"
"alsk"
"apian"
"august"
"bacon"
"bipc"
"boggs"
+ "bonne"
"cass"
"cc"
"cea"
"chamb"
"collg"
"crast"
"denoy"
"eck1"
"eck2"
"eck3"
"eck4"
"eck5"
"eck6"
"eqc"
"eqdc"
"euler"
"etmerc"
"fahey"
"fouc"
"fouc_s"
"gall"
"geos"
"gins8"
"gn_sinu"
"gnom"
"goode"
"gs48"
"gs50"
"hammer"
"hatano"
"healpix"
"rhealpix"
"igh"
"imw_p"
"isea"
"kav5"
"kav7"
"krovak"
"labrd"
"laea"
"lagrng"
"larr"
"lask"
"lonlat"
"latlon"
"lcc"
"lcca"
"leac"
"lee_os"
"loxim"
"lsat"
"mbt_s"
"mbt_fps"
"mbtfpp"
"mbtfpq"
"mbtfps"
+ "merc"
"mil_os"
"mill"
# "moll"
"murd1"
"murd2"
"murd3"
"natearth"
"nell"
"nell_h"
"nicol"
"nsper"
"nzmg"
"ob_tran"
"ocea"
"oea"
"omerc"
"ortel"
"ortho"
"pconic"
"poly"
"putp1"
"putp2"
"putp3"
"putp3p"
"putp4p"
"putp5"
"putp5p"
"putp6"
"putp6p"
"qua_aut"
"robin"
"rouss"
"rpoly"
"sinu"
"somerc"
# "stere"
"sterea"
"gstmerc"
"tcc"
"tcea"
"tissot"
"tmerc"
"tpeqd"
"tpers"
"ups"
"urm5"
"urmfps"
"utm"
"vandg"
"vandg2"
"vandg3"
"vandg4"
"vitk1"
"wag1"
"wag2"
"wag3"
"wag4"
"wag5"
"wag6"
"wag7"
"weren"
"wink1"
"wink2"
"wintri"








## mapproj style

Add equivalent proj4 when done in code.

Base website http://www.remotesensing.org/geotiff/proj_list

"aitoff" aitoff()
"aea" albers(lat0,lat1) # albers_equal_area_conic.html
azequalarea()
azequidistant()
bicentric(lon0)
"bonne" bonne(lat0)
conic(lat0)
cylequalarea(lat0)
cylindrical()
eisenlohr()
elliptic(lon0)
fisheye(n)
gall(lat0)
gilbert()
globular()
gnomonic()
guyou
harrison(dist,angle)
hex
homing(lat0)
lagrange()
## or laea for lambert? lambert_azimuthal_equal_area.html
"lcc" lambert(lat0,lat1) # lambert_conic_conformal_1sp.html
laue()
lune(lat,angle)
mecca(lat0)
"merc" mercator()
"moll" mollweide()
newyorker(r)
orthographic()
perspective(dist)
polyconic()
rectangular(lat0)
simpleconic(lat0,lat1)
sinusoidal()
sp_albers(lat0,lat1)
sp_mercator()
square
"stere" stereographic()
tetra
trapezoidal(lat0,lat1)
vandergrinten()
