
/* ************************************************************************* *\
   * File: dstring.c                                                       *
   * Purpose:                                                              *
   *    Provides misc functions for the DString Library                    *
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

#include "dstring.h"


int dstrerrno;


/* error messages to accompany return codes */
const char *errormsgs[] = {
   "success",
   "out of memory",
   "invalid buffer length",
   "dstring_t uninitialized",
   "unopened file",
   "eof has been reached",
   "error reading file",
   "out of bounds",
   "invalid argument",
   "unknown error"
};

/* ************************************************************************* */

const char * const dstrerrormsg(int code) {

   if (abs(code) > 8) {
      return errormsgs[9];
   } else {
      return errormsgs[abs(code)];
   }
}
