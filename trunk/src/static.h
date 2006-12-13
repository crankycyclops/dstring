
/* ************************************************************************* *\
   * File: static.h                                                        *
   * Purpose:                                                              *
   *    Provides typedefs, macros and prototypes for items that are        *
   *    private to the library.                                            *
   *************************************************************************
   * Project:    DString                                                   *
   * Programmer: James Colannino                                           *
   * Email:      james@colannino.org                                       *
   * Homepage:   http://james.colannino.org/                               *
   *                                                                       *
   * Description:                                                          *
   *     The purpose of this library is to provide facilities for easily   *
   * dealing with dynamically allocated strings.                           *
   ************************************************************************* 
   * DString Library Copyright 2006 by James Colannino                     *
   *                                                                       *
   * This program is free software; you can redistribute it and/or         *
   * modify it under the terms of the GNU Lesser General Public            *
   * License as published by the Free Software Foundation; either          *
   * version 2.1 of the License, or (at your option) any later version.    *
   *                                                                       *
   * This program is distributed in the hope that it will be useful,       *
   * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
   * Lesser General Public License for more details.                       *
   *                                                                       *
   * You should have received a copy of the GNU Lesser General Public      *
   * License along with this library; if not, write to:                    *
   *                                                                       *
   * The Free Software Foundation, Inc.                                    *
   * 51 Franklin St, Fifth Floor                                           *
   * Boston, MA 02110-1301 USA                                             * 
\* ************************************************************************* */

#include <stddef.h>

/* what the opaque datatype dstring_t points to */
typedef struct {
   char *buf;
   size_t buflen;
} dstr;

typedef dstr * dstrptr;

/* macros for typecasting and accessing data members */
#define DSTRREF(X)     ((dstrptr)(X))
#define DSTRBUF(X)     (DSTRREF(X)->buf)
#define DSTRBUFLEN(X)  (DSTRREF(X)->buflen)
