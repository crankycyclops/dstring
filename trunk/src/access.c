
/* ************************************************************************* *\
   * File: access.c                                                        *
   * Purpose:                                                              *
   *    Provides accessor functions for dstring_t objects                  *
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

#include "static.h"
#include "dstring.h"

/* ************************************************************************* */

const char * const dstrview(const dstring_t str) {

   /* uninitialized dstring_t object */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return NULL;
   }

   _setdstrerrno(DSTR_SUCCESS);
   return (const char * const)DSTRBUF(str);
}

/* ************************************************************************* */

size_t dstrallocsize(const dstring_t str) {

   /* uninitialized dstring_t object */
   if (NULL == str) {
      _setdstrerrno(DSTR_UNINITIALIZED);
      return 0;
   }

   _setdstrerrno(DSTR_SUCCESS);
   return DSTRBUFLEN(str);
}
