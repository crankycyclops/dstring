
/* ************************************************************************* *\
   * File: test.c                                                          *
   * Purpose:                                                              *
   *    A self test designed to validate the DString library and to detect *
   *    bugs in the implementation.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dstring.h"

#define ALLOCSIZE 1

/* a flag type to determine whether a test passes or fails */
typedef enum {
   PASS, FAIL
} STAT;

const char *teststr = "How many lines could a hacker hack if a hacker could \
hack code?";

/* ************************************************************************* */

int main(int argc, char *argv[]) {

   int  retval;
   char *retvalstr;

   STAT status = PASS;

   dstring_t buildinfo;
   dstring_t testStr = NULL;
   dstring_t uninitializedStr = NULL;

   printf("\nDString Library Validation\n");
   printf("--------------------------\n\n");

   if (DSTR_SUCCESS != dstrnalloc(&buildinfo, 10)) {
      printf("(Failed to retrieve build information)\n\n");
   } else {
      if (DSTR_SUCCESS != dstrbuildinfo(buildinfo)) {
         printf("(Failed to retrieve build information)\n\n");
      } else {
         printf("%s\n\n", dstrview(buildinfo));
      }
   }

   /**************************************************************************\
    * TIER 1: accessor functions                                             *
   \**************************************************************************/

   printf("TIER 1: Accessor Functions\n\n");

   printf("dstrview():\n");
   putchar('\n');
   printf("\tTest 1: Check for correct handling of uninitialized string\n");
   printf("\tPassing NULL to dstrview()\n");
   printf("\tExpected return value: NULL\n");
   printf("\tExpected error message: %s\n", dstrerrormsg(DSTR_UNINITIALIZED));
   printf("\tActual return value: ");

   /* make sure NULL is the return value */
   if (NULL == (retvalstr = dstrview(NULL))) {
      printf("NULL\n");
      printf("\tActual error message: %s\n", dstrerrormsg(dstrerrno));
      /* make sure dstrerrno is set to DSTR_UNINITIALIZED */
      if (DSTR_UNINITIALIZED != dstrerrno) {
         status = FAIL;
         printf("FAIL\n");
      } else {
         printf("\tPASS\n");
      }
   } else {
      printf("%p\n", retvalstr);
      printf("\tFAIL\n");
      status = FAIL;
   }

   putchar('\n');
   printf("\tTest 2: Check for correct return of initialized string\n");
   printf("\tExpected return value should be greater than 0\n");

   retval = dstralloc(&testStr);
   if (DSTR_SUCCESS != retval) {
      printf("\terror: dstralloc() could not allocate space for a test string.");
      putchar('\n');
      printf("\tEither there was not enough memory or dstralloc() is failing.\n");
      printf("\tFree some memory and try again.  If that does nothing, check\n");
      printf("\tto see if dstralloc() fails in Tier 2.\n");
   }

   if (NULL == (retvalstr = dstrview(testStr))) {
      printf("\tReturn value equals 0\n");
      printf("\tFAIL\n");
      printf("\tPossible reason for failure: dstralloc() failed.\n");
      printf("\tCheck for this in Tier 2.\n");
      putchar('\n');
      status = FAIL;
   } else {
      printf("\tReturn value is greater than 0\n");
      printf("\tPASS\n");
      putchar('\n');
   }

   if (PASS == status) {
      printf("dstrview(): PASS\n");
   } else {
      printf("dstrview(): FAIL\n");
   }

   putchar('\n');
   status = PASS;

   printf("dstrallocsize():\n");
   putchar('\n');
   printf("\tTest 1: Check for correct handling of uninitialized string\n");
   printf("\tPassing NULL to dstrallocsize()\n");
   printf("\tExpected return value: 0\n");
   printf("\tActual error: ");

   retval = dstrallocsize(NULL);
   printf("%d\n", retval);

   if (0 == retval) {
      printf("\tPASS\n");
   } else {
      printf("\tFAIL\n");
      status = FAIL;
   }

   putchar('\n');

   printf("\tTest 2: Check for correct reporting of allocation size\n");
   printf("\tPassing testStr to dstrallocsize()\n");
   printf("\tExpected return value: %d\n", ALLOCSIZE);
   printf("\tActual return value: ");

   retval = dstrallocsize(testStr);
   printf("%d\n", retval);

   if (retval != 1) {
      printf("\tFAIL\n");
      status = FAIL;
   } else {
      printf("\tPASS\n");
   }

   putchar('\n');

   if (PASS == status) {
      printf("dstrallocsize(): PASS\n");
   } else {
      printf("dstrallocsize(): FAIL\n");
   }

   putchar('\n');

   /**************************************************************************\
    * TIER 2: Allocation Functions                                           *
   \**************************************************************************/

   printf("TIER 2: Allocation Functions\n\n");

   return EXIT_SUCCESS;
}
