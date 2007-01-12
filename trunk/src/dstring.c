
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

/* error messages to accompany return codes */
const char *errormsgs[] = {
   "success",
   "out of memory",
   "invalid buffer length",
   "dstring_t uninitialized",
   "unopened file",
   "eof has been reached",
   "unknown error"
};


const char * const dstrerrormsg(int code) {

   /* return the proper error message */
   switch(code) {
      case DSTR_SUCCESS:
         return errormsgs[0];
      case DSTR_NOMEM:
         return errormsgs[1];
      case DSTR_INVALID_BUFLEN:
         return errormsgs[2];
      case DSTR_UNINITIALIZED:
         return errormsgs[3];
      case DSTR_UNOPENED_FILE:
         return errormsgs[4];
      case DSTR_EOF:
         return errormsgs[5];
      default:
         return errormsgs[6];
   }

   /* we shouldn't get here, but whatever... */
   return errormsgs[6];
}
