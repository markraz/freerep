//This code is an adaptation of Atul Narkhede's implementation
//of the Seidel algorithm for fast polygon tesselation. 
//It is believed to be in the public domain.
//Any modifications from the original work are
//Copyright (C) 2009 The FreeREP Foundation
//See License.txt for terms and conditions

#include "triangulate.h"
#ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
#else
#  include <time.h>
#endif

#include <math.h>

static int choose_idx;
static int permute[SEGSIZE];


/* Generate a random permutation of the segments 1..n */
int generate_random_ordering(
     int n)
{
  register int i;
  int m, st[SEGSIZE], *p;
#ifdef _MSC_VER
  time_t ltime;

  time( &ltime );
  srand( ltime );
#else
//  struct timeval tval;

  choose_idx = 1;
//  gettimeofday(&tval, &tzone);
//  srand48(tval.tv_sec);
#endif

  for (i = 0; i <= n; i++)
    st[i] = i;

  p = st;
  for (i = 1; i <= n; i++, p++)
    {
#ifdef _MSC_VER
	m = rand() % ( n + 1 - i ) + 1;
#else
	m = 100 % ( n + 1 - i ) + 1; //used to be lrand48 instead of 100
	//makes the tesselations random, and over time seems to just find one that
	// takes a long time. 	
#endif
      permute[i] = p[m];
      if (m != 1)
	p[m] = p[1];
    }
  return 0;
}


/* Return the next segment in the generated random ordering of all the */
/* segments in S */
int choose_segment()
{
#ifdef DEBUG
  fprintf(stderr, "choose_segment: %d\n", permute[choose_idx]);
#endif
  return permute[choose_idx++];
}


#ifdef STANDALONE

/* Read in the list of vertices from infile */
int read_segments(filename, genus)
     char *filename;
     int *genus;
{
  FILE *infile;
  int ccount;
  register int i;
  int ncontours, npoints, first, last;

  if ((infile = fopen(filename, "r")) == NULL)
    {
      perror(filename);
      return -1;
    }

  fscanf(infile, "%d", &ncontours);
  if (ncontours <= 0)
    return -1;

  /* For every contour, read in all the points for the contour. The */
  /* outer-most contour is read in first (points specified in */
  /* anti-clockwise order). Next, the inner contours are input in */
  /* clockwise order */

  ccount = 0;
  i = 1;

  while (ccount < ncontours)
    {
      int j;

      fscanf(infile, "%d", &npoints);
      first = i;
      last = first + npoints - 1;
      for (j = 0; j < npoints; j++, i++)
	{
	  fscanf(infile, "%lf%lf", &seg[i].v0.x, &seg[i].v0.y);
	  if (i == last)
	    {
	      seg[i].next = first;
	      seg[i].prev = i-1;
	      seg[i-1].v1 = seg[i].v0;
	    }
	  else if (i == first)
	    {
	      seg[i].next = i+1;
	      seg[i].prev = last;
	      seg[last].v1 = seg[i].v0;
	    }
	  else
	    {
	      seg[i].prev = i-1;
	      seg[i].next = i+1;
	      seg[i-1].v1 = seg[i].v0;
	    }

	  seg[i].is_inserted = FALSE;
	}

      ccount++;
    }

  *genus = ncontours - 1;
  return i-1;
}

#endif


/* Get log*n for given n */
int math_logstar_n(
     int n)
{
  register int i;
  double v;

  for (i = 0, v = (double) n; v >= 1; i++) {
    /* v = log2(v); */
    v = log(v) / log(2.0);
  }

  return (i - 1);
}


int math_N(
     int n,
     int h)
{
  register int i;
  double v;

  for (i = 0, v = (int) n; i < h; i++) {
    /* v = log2(v); */
    v = log(v) / log(2.0);
  }

  return (int) ceil((double) 1.0*n/v);
}