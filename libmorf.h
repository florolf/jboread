/***************************************
  $Header: /cvs/src/jbofihe/morf.h,v 1.7 2001/06/22 22:16:51 richard Exp $

  Header file for morphology functions module.
  
  ***************************************/

/* Copyright (C) Richard P. Curnow  2000-2001 */
/*
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 * 
*/

#ifndef MORF_H
#define MORF_H

/* Return type.  Note that gismu/lujvo/fu'ivla are compressed together.  This
 * could be exposed later if the rest of the main program has a use for the
 * information. */
typedef enum {
  MT_BOGUS,
  MT_GISMU,
  MT_LUJVO,
  MT_FUIVLA3, /* stage 3 */
  MT_FUIVLA3_CVC, /* stage 3 starting with CVC rafsi */
  MT_FUIVLA3X, /* stage-3-like with >1 rafsi before hyphen */
  MT_FUIVLA3X_CVC, /* stage-3-like starting with CVC rafsi */
  MT_FUIVLA4, /* stage 4 */
  MT_CMAVOS,
  MT_CMENE,
  MT_BAD_UPPERCASE
} MorfType;

/* Structure for returning extra information about some of the
 * word types */

struct morf_xtra {
  union {
    struct {
      int is_bad;
      int can_split;
      char *ladoi;
      char *tail;
    } cmene;
    struct {
      char *hyph;
    } stage_3;
  } u;
};

/* Prototypes for fns */
extern MorfType morf_scan(char *s, char ***buf_end, struct morf_xtra *);
char *canon_lujvo(char*);

#endif /* MORF_H */

