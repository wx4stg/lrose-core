// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// RCS info
//   $Author: dave $
//   $Locker:  $
//   $Date: 2017/07/28 22:38:28 $
//   $Id: Main.cc,v 1.3 2017/07/28 22:38:28 dave Exp $
//   $Revision: 1.3 $
//   $State: Exp $
//
 
/**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**/
/*********************************************************************
 * Main.cc: RefractCompute main routine
 *
 * RAP, NCAR, Boulder CO
 *
 * January 2008
 *
 * Nancy Rehak
 *
 *********************************************************************/

#include <stdio.h>

#include <toolsa/port.h>
#include <toolsa/umisc.h>

#include "RefractCompute.hh"


// Prototypes for static functions

static void tidy_and_exit(int sig);


// Global variables

RefractCompute *Prog = (RefractCompute *)NULL;


/*********************************************************************
 * main()
 */

int main(int argc, char **argv)
{
  // Create program object.

  Prog = RefractCompute::Inst(argc, argv);
  if (!Prog->okay)
    return -1;

  if (!Prog->init())
    return -1;
  
  // Register function to trap termination and interrupts.

  PORTsignal(SIGQUIT, tidy_and_exit);
  PORTsignal(SIGTERM, tidy_and_exit);
  PORTsignal(SIGINT, tidy_and_exit);

  // Run the program.

  Prog->run();

  // clean up

  tidy_and_exit(0);
  return 0;
}

/*********************************************************************
 * tidy_and_exit()
 */

static void tidy_and_exit(int sig)
{
  // Delete the program object.

  if (Prog != (RefractCompute *)NULL)
    delete Prog;

  // Now exit the program.

  exit(sig);
}
