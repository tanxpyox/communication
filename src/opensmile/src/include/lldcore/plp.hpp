/*F***************************************************************************
 * 
 * openSMILE - the Munich open source Multimedia Interpretation by 
 * Large-scale Extraction toolkit
 * 
 * This file is part of openSMILE.
 * 
 * openSMILE is copyright (c) by audEERING GmbH. All rights reserved.
 * 
 * See file "COPYING" for details on usage rights and licensing terms.
 * By using, copying, editing, compiling, modifying, reading, etc. this
 * file, you agree to the licensing terms in the file COPYING.
 * If you do not agree to the licensing terms,
 * you must immediately destroy all copies of this file.
 * 
 * THIS SOFTWARE COMES "AS IS", WITH NO WARRANTIES. THIS MEANS NO EXPRESS,
 * IMPLIED OR STATUTORY WARRANTY, INCLUDING WITHOUT LIMITATION, WARRANTIES OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ANY WARRANTY AGAINST
 * INTERFERENCE WITH YOUR ENJOYMENT OF THE SOFTWARE OR ANY WARRANTY OF TITLE
 * OR NON-INFRINGEMENT. THERE IS NO WARRANTY THAT THIS SOFTWARE WILL FULFILL
 * ANY OF YOUR PARTICULAR PURPOSES OR NEEDS. ALSO, YOU MUST PASS THIS
 * DISCLAIMER ON WHENEVER YOU DISTRIBUTE THE SOFTWARE OR DERIVATIVE WORKS.
 * NEITHER TUM NOR ANY CONTRIBUTOR TO THE SOFTWARE WILL BE LIABLE FOR ANY
 * DAMAGES RELATED TO THE SOFTWARE OR THIS LICENSE AGREEMENT, INCLUDING
 * DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL OR INCIDENTAL DAMAGES, TO THE
 * MAXIMUM EXTENT THE LAW PERMITS, NO MATTER WHAT LEGAL THEORY IT IS BASED ON.
 * ALSO, YOU MUST PASS THIS LIMITATION OF LIABILITY ON WHENEVER YOU DISTRIBUTE
 * THE SOFTWARE OR DERIVATIVE WORKS.
 * 
 * Main authors: Florian Eyben, Felix Weninger, 
 * 	      Martin Woellmer, Bjoern Schuller
 * 
 * Copyright (c) 2008-2013, 
 *   Institute for Human-Machine Communication,
 *   Technische Universitaet Muenchen, Germany
 * 
 * Copyright (c) 2013-2015, 
 *   audEERING UG (haftungsbeschraenkt),
 *   Gilching, Germany
 * 
 * Copyright (c) 2016,	 
 *   audEERING GmbH,
 *   Gilching Germany
 ***************************************************************************E*/


/*  openSMILE component: cPlp

This component computes PLP and RASTA-PLP cepstral coefficients from a critical band spectrum (generated by the cMelspec component, for example).

The component is capable of performing the following processing steps:
   1) Take the natural logarithm of the critical band powers (doLog)
   2) RASTA filtering
   3) Computation of auditory spectrum (equal loudness curve and loudness compression)
   4) Inverse of the natural logarithm
   5) Inverse DFT to obtain autocorrelation coefficients
   6) Linear prediction analysis on autocorr. coeff.
   7) Computation of cepstral coefficients from lp coefficients
   8) Cepstral 'liftering'

*/



#ifndef __CPLP_HPP
#define __CPLP_HPP

#include <core/smileCommon.hpp>
#include <core/vectorProcessor.hpp>
#include <math.h>

#define COMPONENT_DESCRIPTION_CPLP "This component computes PLP and RASTA-PLP (currently the RASTA filter is not yet implemented) cepstral coefficients from a critical band spectrum (generated by the cMelspec component, for example).\n   The component is capable of performing the following processing steps: \n   1) Take the natural logarithm of the critical band powers (doLog)\n   2) RASTA filtering\n   3) Computation of auditory spectrum (equal loudness curve and loudness compression)\n   4) Inverse of the natural logarithm\n   5) Inverse DFT to obtain autocorrelation coefficients\n   6) Linear prediction analysis on autocorr. coeff.\n   7) Computation of cepstral coefficients from lp coefficients\n   8) Cepstral 'liftering'";

#define COMPONENT_NAME_CPLP "cPlp"

#undef class
class  cPlp : public cVectorProcessor {
  private:
    int htkcompatible;
    int nAuto, nFreq, nScale;
    int lpOrder;
    int nCeps, firstCC, lastCC;

    int doLog, doAud, doInvLog, doIDFT, RASTA, newRASTA, doLP, doLpToCeps;
    FLOAT_DMEM rastaUpperCutoff, rastaLowerCutoff;

    FLOAT_DMEM rasta_iir;
    FLOAT_DMEM rasta_fir[5];
    // rasta filter history:
    FLOAT_DMEM **rasta_buf_iir; /* size: n bands */
    FLOAT_DMEM **rasta_buf_fir; /* size: n bands * 5 (nCoeff) */
    int rasta_buf_fir_ptr; // buffer pointer (cyclic shifting)
    int rasta_init;

    FLOAT_DMEM cepLifter;
    
    FLOAT_DMEM **acf;
    FLOAT_DMEM **lpc;
    FLOAT_DMEM **ceps;

    FLOAT_DMEM compression;
    FLOAT_DMEM **eqlCurve; // equal loudness curve
    FLOAT_DMEM **costable; // IDFT
    FLOAT_DMEM **sintable; // cepstral liftering

    FLOAT_DMEM melfloor;

    int initTables( long blocksize, int idxc, int fidx );
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR

    virtual void fetchConfig();
    virtual int dataProcessorCustomFinalise();
    virtual int setupNamesForField(int i, const char*name, long nEl);

    //virtual int processVectorInt(const INT_DMEM *src, INT_DMEM *dst, long Nsrc, long Ndst, int idxi);
    virtual int processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cPlp(const char *_name);

    virtual ~cPlp();
};




#endif // __CPLP_HPP
