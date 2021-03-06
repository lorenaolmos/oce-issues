/* vim: set noexpandtab shiftwidth=2 softtabstop=2 tw=70: */
#define DEBUG 0
#define DOLEFT
#define DORIGHT
#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
/*

   system("R CMD SHLIB polygon_subdivide.c") 
   dyn.load("polygon_subdivide.so")

*/

#define SAVE(x,y) {\
  if ((*no) >= (*nomax)) error("Ran out of space (limit %d pairs); contact developer.\n", (*nomax));\
  xo[(*no)]=(x);\
  yo[(*no)]=(y);\
  ++(*no);\
  if (DEBUG) Rprintf(" [ %7.2f %7.2f @ %3d ]\n", (x), (y), (*no));\
}

void polygon_subdivide_vertically1(int *n, double *x, double *y, double *x0,
    int *nomax, int *no, double *xo, double *yo)
{
  Rprintf("polygon_subdivide_vertically1(*n=%d, ..., *x0=%f, *nomax=%d, ...)\n", *n, *x0, *nomax);
  unsigned int *poly_start = (unsigned int*)R_alloc(*(nomax), sizeof(unsigned int));
  unsigned int *poly_end = (unsigned int*)R_alloc(*(nomax), sizeof(unsigned int));
  unsigned int ipoly=0, npoly = 0;
  (*no) = 0; // set to 0 by R anyway, but this protects against R changes

  // Separate steps to make it easier to write/debug/read.

  // 1. Find polygons.
  // Skip any NA at start
  int start;
  for (start = 0; start < (*n)-1; start++) {
    if (!ISNA(x[start]))
      break;
  }
  poly_start[npoly] = start;
  int i = start;
  while (i < (*n)) {
    // Find first non-NA
    while (ISNA(x[i]) && (i < (*n))) {
      i++;
    }
    poly_start[npoly] = i;
    // Find last non-NA
    while (!ISNA(x[i]) && (i < (*n))) {
      i++;
    }
    poly_end[npoly] = i; // FIXME: what is right here??
    npoly++;
    i++;
    if (npoly > 3) error("checking");
  }
  if (npoly == 0) 
    error("no polygons\n");
  //
  // 2. Process each polygon individually.
  for (ipoly = 0; ipoly < npoly; ipoly++) {
    Rprintf("\npoly %d @ %d to %d\n", ipoly, poly_start[ipoly], poly_end[ipoly]);
    // Find intersection segments
    int j;
    unsigned int *intersection = (unsigned int*)R_alloc(*(n), sizeof(unsigned int));
    int nintersection;
    //int nseg = 1 + poly_end[ipoly] - poly_start[ipoly];
    //Rprintf("  nseg=%d\n", nseg);
    nintersection = 0;
    for (i = poly_start[ipoly], j=poly_end[ipoly]; i < poly_end[ipoly]; j=i++) {
      //Rprintf("   x[%d]=%6.2f x[%d]=%6.2f & y[%d]=%6.2f y[%d]=%6.2f\n", i, x[i], j, x[j], i, y[i], j, y[j]);
      if ((x[i] <= (*x0) && (*x0) <= x[j]) || (x[j] <= (*x0) && (*x0) <= x[i])) {
	//Rprintf("     the previous point intersects x0=%f\n", (*x0));
	intersection[nintersection++] = i;
      }
    }
    if (nintersection == 0) {
      // This polygon does not intersect x0, so just emit it.
      for (i = poly_start[ipoly]; i < poly_end[ipoly]; i++) {
	SAVE(x[i], y[i]);
      }
      SAVE(NA_REAL, NA_REAL);
    } else {
      // This polygon intersects x0, so subdivide and emit the two portions. 
      // Do this by tracing along the path, starting at the intersection
      // point that with the highest y value.
      double ymax = y[poly_start[ipoly]];
      int imax = -1;
      for (i = 1 + poly_start[ipoly]; i <= poly_end[ipoly]; i++) {
	if (y[i] > ymax) {
	  ymax = y[i];
	  imax = i;
	}
      }
      Rprintf("       top y[%d] is %f\n", imax, y[imax]);
      int k, kk;
      k = imax - 1;
      if (k < 0)
	k = poly_end[ipoly];
      kk = imax;
      if (kk > poly_end[ipoly]) // FIXME: probably this is wrong
	kk = poly_start[ipoly]; // FIXME: probably this is wrong
      double epsilon=0.25;
      //int hidden_points = 0;
#ifdef DOLEFT
      // Left of x0
      for (i = 0; i <= poly_end[ipoly]-poly_start[ipoly]; i++) {
	Rprintf("   i=%d k=%d kk=%d x[k]=%7.3f y[k]=%7.3f x[kk]=%7.3f y[kk]=%7.3f\n", i, k, kk, x[k], y[k], x[kk], y[kk]);
	if (x[k] <= (*x0) && (*x0) <= x[kk]) {
	  // passing cut point, going right
	  double Y = y[k] + (y[kk]-y[k])*((*x0)-epsilon-x[k])/(x[kk]-x[k]);
	  Rprintf("CASE left.A (crossing, going right): x[k=%d]=%7.3f x[kk=%d]=%7.3f Y=%7.3f\n", k, x[k], kk, x[kk], Y);
	  SAVE(x[k], y[k]);
	  SAVE((*x0)-epsilon, Y);
	} else if (x[kk] <= (*x0) && (*x0) <= x[k]) {
	  double Y = y[k] + (y[kk]-y[k])*((*x0)-epsilon-x[k])/(x[kk]-x[k]);
	  Rprintf("CASE left.B (crossing, going left): x[k=%d]=%7.3f x[kk=%d]=%7.3f Y=%7.3f\n", k, x[k], kk, x[kk], Y);
	  SAVE((*x0)-epsilon, Y);
	  SAVE(x[kk], y[kk]);
	} else {
	  if (x[k] <= (*x0)) {
	    Rprintf("CASE left.C (save point) x[k=%d]=%7.3f y[k=%d]=%7.3f\n", k, x[k], k, y[k]);
	    SAVE(x[k], y[k]);
	  } else {
	    Rprintf("CASE left.D (ignore point) x[k=%d]=%7.3f y[k=%d]=%7.3f\n", k, x[k], k, y[k]);
	    //hidden_points++;
	    //SAVE((*x0)-epsilon, y[k]);
	  }
	}
	if (++k > poly_end[ipoly])
	  k = poly_start[ipoly];
	if (++kk > poly_end[ipoly])
	  kk = poly_start[ipoly];
      }
      SAVE(NA_REAL, NA_REAL);
#endif
#ifdef DORIGHT
      // Right of x0
      k = imax;
      kk = imax + 1;
      if (kk > poly_end[ipoly]) // FIXME: probably this is wrong
	kk = poly_start[ipoly]; // FIXME: probably this is wrong
      //hidden_points = 0;
      //double hidden_y_start = 0;
      for (i = 0; i <= poly_end[ipoly]-poly_start[ipoly]; i++) {
	Rprintf("  i=%d k=%d kk=%d  x[k]=%7.3f y[k]=%7.3f x[kk]=%7.3f y[kk]=%7.3f\n", i, k, kk, x[k], y[k], x[kk], y[kk]);
	if (x[k] <= (*x0) && (*x0) <= x[kk]) {
	  double Y = y[k] + (y[kk]-y[k])*((*x0)+epsilon-x[k])/(x[kk]-x[k]);
	  Rprintf("CASE right.A (crossing, going right): x[k=%d]=%7.3f x[kk=%d]=%7.3f Y=%7.3f\n", k, x[k], kk, x[kk], Y);
	  //if (hidden_points > 0) {
	  //  double YY = hidden_y_start, dy = (Y-YY)/hidden_points;
	  //  for (int p = 0; p < hidden_points; p++) {
	  //    SAVE((*x0)+epsilon, YY);
	  //    YY += dy;
	  //  }
	  //}
	  SAVE((*x0)+epsilon, Y);
	  SAVE(x[kk], y[kk]);
	  //hidden_points = 0;
	  //hidden_y_start = Y;
	} else if (x[kk] <= (*x0) && (*x0) <= x[k]) {
	  double Y = y[k] + (y[kk]-y[k])*((*x0)+epsilon-x[k])/(x[kk]-x[k]);
	  Rprintf("CASE left.B (crossing, going left): x[k=%d]=%7.3f x[kk=%d]=%7.3f Y=%7.3f\n", k, x[k], kk, x[kk], Y);
	  //if (hidden_points > 0) {
	  //  double YY = hidden_y_start, dy = (Y-YY)/hidden_points;
	  //  for (int p = 0; p < hidden_points; p++) {
	  //    SAVE((*x0)+epsilon, YY);
	  //    YY += dy;
	  //  }
	  //}
	  SAVE(x[k], y[k]);
	  SAVE((*x0)+epsilon, Y);
	  //hidden_points = 0;
	  //hidden_y_start = Y;
	} else {
	  if (x[k] >= (*x0)) {
	    Rprintf("CASE right.C (save point) x[k=%d]=%7.3f y[k=%d]=%7.3f\n", k, x[k], k, y[k]);
	    SAVE(x[k], y[k]);
	  } else {
	    Rprintf("CASE right.D (ignore point) x[k=%d]=%7.3f y[k=%d]=%7.3f\n", k, x[k], k, y[k]);
	    //SAVE((*x0)+epsilon, y[k]);
	    //Rprintf("  hidden_points=%d\n", hidden_points);
	    //hidden_points++;
	  }
	}
	if (++k > poly_end[ipoly])
	  k = poly_start[ipoly];
	if (++kk > poly_end[ipoly])
	  kk = poly_start[ipoly];
      }
      SAVE(NA_REAL, NA_REAL);
#endif
    }
  }
}

// smash the opposite side (crude, but possibly not ineffective)
void polygon_subdivide_vertically2(int *n, double *x, double *y, double *x0,
    int *nomax, int *no, double *xo, double *yo)
{
  Rprintf("polygon_subdivide_vertically2(*n=%d, ..., *x0=%f, *nomax=%d, ...)\n", *n, *x0, *nomax);
  unsigned int *poly_start = (unsigned int*)R_alloc(*(nomax), sizeof(unsigned int));
  unsigned int *poly_end = (unsigned int*)R_alloc(*(nomax), sizeof(unsigned int));
  unsigned int ipoly=0, npoly = 0;
  (*no) = 0; // set to 0 by R anyway, but this protects against R changes

  // Separate steps to make it easier to write/debug/read.

  // 1. Find polygons.
  // Skip any NA at start
  int start;
  for (start = 0; start < (*n)-1; start++) {
    if (!ISNA(x[start]))
      break;
  }
  poly_start[npoly] = start;
  int i = start;
  while (i < (*n)) {
    // Find first non-NA
    while (ISNA(x[i]) && (i < (*n))) {
      i++;
    }
    poly_start[npoly] = i;
    // Find last non-NA
    while (!ISNA(x[i]) && (i < (*n))) {
      i++;
    }
    poly_end[npoly] = i-1;
    npoly++;
    i++;
  }
  if (npoly == 0) 
    error("no polygons\n");
  Rprintf("found %d polygons\n", npoly);
  //
  // 2. Process each polygon individually.
  int j;
  double epsilon = 0.2;
  for (ipoly = 0; ipoly < npoly; ipoly++) {
    Rprintf("\npoly %d @ %d to %d\n", ipoly, poly_start[ipoly], poly_end[ipoly]);
    for (i = poly_start[ipoly], j=poly_end[ipoly]; i < poly_end[ipoly]; j=i++) {
      //Rprintf("POLY LHS i=%d j=%d\n", i, j);
      if (x[i] > (*x0) - epsilon) {
	SAVE((*x0) - epsilon, y[i])
      } else {
	SAVE(x[i], y[i])
      }
    }
    SAVE(NA_REAL, NA_REAL);
    for (i = poly_start[ipoly], j=poly_end[ipoly]; i < poly_end[ipoly]; j=i++) {
      //Rprintf("POLY RHS i=%d j=%d\n", i, j);
      if (x[i] < (*x0) + epsilon) {
	SAVE((*x0) + epsilon, y[i])
      } else {
	SAVE(x[i], y[i])
      }
    }
  }
}

// cut the opposite side
void polygon_subdivide_vertically3(int *n, double *x, double *y, double *x0,
    int *nomax, int *no, double *xo, double *yo)
{
  Rprintf("polygon_subdivide_vertically3(*n=%d, ..., *x0=%f, *nomax=%d, ...)\n", *n, *x0, *nomax);
  unsigned int *poly_start = (unsigned int*)R_alloc(*(nomax), sizeof(unsigned int));
  unsigned int *poly_end = (unsigned int*)R_alloc(*(nomax), sizeof(unsigned int));
  unsigned int ipoly=0, npoly = 0;
  (*no) = 0; // set to 0 by R anyway, but this protects against R changes

  // Separate steps to make it easier to write/debug/read.

  // 1. Find polygons.
  // Skip any NA at start
  int start;
  for (start = 0; start < (*n)-1; start++) {
    if (!ISNA(x[start]))
      break;
  }
  poly_start[npoly] = start + 1;
  int i = start;
  while (i < (*n)) {
    // Find first non-NA
    while (ISNA(x[i]) && (i < (*n))) {
      i++;
    }
    poly_start[npoly] = i + 1;
    // Find last non-NA
    while (!ISNA(x[i]) && (i < (*n))) {
      i++;
    }
    poly_end[npoly] = i-1;
    npoly++;
    i++;
  }
  if (npoly == 0) 
    error("no polygons\n");
  Rprintf("found %d polygons\n", npoly);
  //
  // 2. Process each polygon individually.
  int j;
  double epsilon = 2;
  // FIXME: might help to interpolate in an additional point near the boundary
  // FIXME: the opposite side is ugly but very thin so maybe OK
  for (ipoly = 0; ipoly < npoly; ipoly++) {
    int crossing = 0;
    double delta0 = x[poly_start[ipoly]] - (*x0);
    for (i = poly_start[ipoly]; i <= poly_end[ipoly]; i++) {
      double delta = x[i] - (*x0);
      if (delta * delta0 < 0.0) {
	crossing = 1;
	break;
      }
    }
    if (crossing) {
      Rprintf("poly %4d @ %d:%d CROSSES\n", ipoly, poly_start[ipoly], poly_end[ipoly]);
      for (i = poly_start[ipoly], j=poly_end[ipoly]; i <= poly_end[ipoly]; j=i++) {
	//Rprintf("POLY LHS i=%d j=%d\n", i, j);
	if (x[i] > ((*x0) - epsilon)) {
	  SAVE((*x0) - epsilon, y[i])
	    //SAVE(x[i], y[i])
	} else {
	  SAVE(x[i], y[i])
	}
      }
      SAVE(NA_REAL, NA_REAL);
      for (i = poly_start[ipoly], j=poly_end[ipoly]; i <= poly_end[ipoly]; j=i++) {
	//Rprintf("POLY RHS i=%d j=%d\n", i, j);
	if (x[i] < ((*x0) + epsilon)) {
	  SAVE((*x0) + epsilon, y[i])
	    //SAVE(x[i], y[i])
	} else {
	  SAVE(x[i], y[i])
	}
      }
    } else {
      Rprintf("poly %4d @ %d:%d DOES NOT CROSS\n", ipoly, poly_start[ipoly], poly_end[ipoly]);
      for (i = poly_start[ipoly], j=poly_end[ipoly]; i <= poly_end[ipoly]; j=i++) {
	  SAVE(x[i], y[i])
      }
      SAVE(NA_REAL, NA_REAL);
    }
  }
}

