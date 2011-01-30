   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
  /*               CLIPS Version 6.10  04/09/97          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Donnell                                     */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_immthpsr
#define _H_immthpsr

#if DEFGENERIC_CONSTRUCT && (! BLOAD_ONLY) && (! RUN_TIME)

#include "genrcfun.h"

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _IMMTHPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE void AddImplicitMethods(DEFGENERIC *);

#ifndef _IMMTHPSR_SOURCE_
#endif

#endif

#endif




