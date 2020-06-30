#ifndef __PDK_SFR_DEFS_H__
#define __PDK_SFR_DEFS_H__

#if !defined(__PDK_IO_H__)
#  error "You must #include "pdk/io.h" instead of "pdk/sfr_defs.h" by itself."
#endif

// CPU
#ifdef FLAG_ADDR
  __sfr __at(FLAG_ADDR)         _flag;
  #define FLAG                  _flag
#endif
#ifdef SP_ADDR
  __sfr __at(SP_ADDR)           _sp;
  #define SP                    _sp
#endif

// Interrupts
#ifdef INTEN_ADDR
  __sfr __at(INTEN_ADDR)        _inten;
  #define INTEN                 _inten
#endif
#ifdef INTRQ_ADDR
  __sfr __at(INTRQ_ADDR)        _intrq;
  #define INTRQ                 _intrq
#endif
#ifdef INTEGS_ADDR
  __sfr __at(INTEGS_ADDR)       _integs;
  #define INTEGS                _integs
#endif

// Clock / Oscillator
#ifdef CLKMD_ADDR
  __sfr __at(CLKMD_ADDR)        _clkmd;
  #define CLKMD                 _clkmd
#endif
#ifdef IHRCR_ADDR
  __sfr __at(IHRCR_ADDR)        _ihrcr;
  #define IHRCR                 _ihrcr
#endif
#ifdef ILRCR_ADDR
  __sfr __at(ILRCR_ADDR)        _ilrcr;
  #define ILRCR                 _ilrcr
#endif
#ifdef EOSCR_ADDR
  __sfr __at(EOSCR_ADDR)        _eoscr;
  #define EOSCR                 _eoscr
#endif

// BandGap / Comparator
#ifdef BGTR_ADDR
  __sfr __at(BGTR_ADDR)         _bgtr;
  #define BGTR                  _bgtr
#endif
#ifdef GPCC_ADDR
  __sfr __at(GPCC_ADDR)         _gpcc;
  #define GPCC                  _gpcc
#endif
#ifdef GPCS_ADDR
  __sfr __at(GPCS_ADDR)         _gpcs;
  #define GPCS                  _gpcs
#endif

// IO Port A
#ifdef PADIER_ADDR
  __sfr __at(PADIER_ADDR)      _padier;
  #define PADIER               _padier
#endif
#ifdef PA_ADDR
  __sfr __at(PA_ADDR)          _pa;
  #define PA                   _pa
#endif
#ifdef PAC_ADDR
  __sfr __at(PAC_ADDR)         _pac;
  #define PAC                  _pac
#endif
#ifdef PAPH_ADDR
  __sfr __at(PAPH_ADDR)        _paph;
  #define PAPH                 _paph
#endif

// IO Port B
#ifdef PBDIER_ADDR
  __sfr __at(PBDIER_ADDR)      _pbdier;
  #define PBDIER               _pbdier
#endif
#ifdef PB_ADDR
  __sfr __at(PB_ADDR)          _pb;
  #define PB                   _pb
#endif
#ifdef PBC_ADDR
  __sfr __at(PBC_ADDR)         _pbc;
  #define PBC                  _pbc
#endif
#ifdef PBPH_ADDR
  __sfr __at(PBPH_ADDR)        _pbph;
  #define PBPH                 _pbph
#endif
#ifdef PBPL_ADDR
  __sfr __at(PBPL_ADDR)        _pbpl;
  #define PBPL                 _pbpl
#endif

// IO Port C
#ifdef PCDIER_ADDR
  __sfr __at(PCDIER_ADDR)      _pcdier;
  #define PCDIER               _pcdier
#endif
#ifdef PC_ADDR
  __sfr __at(PC_ADDR)          _pc;
  #define PC                   _pc
#endif
#ifdef PCC_ADDR
  __sfr __at(PCC_ADDR)         _pcc;
  #define PCC                  _pcc
#endif
#ifdef PCPH_ADDR
  __sfr __at(PCPH_ADDR)        _pcph;
  #define PCPH                 _pcph
#endif
#ifdef PCPL_ADDR
  __sfr __at(PCPL_ADDR)        _pcpl;
  #define PCPL                 _pcpl
#endif

// Timer16
#ifdef T16M_ADDR
  __sfr __at(T16M_ADDR)         _t16m;
  #define T16M                  _t16m

  __sfr16                       _t16c;
  #define T16C                  _t16c
#endif

// Timer2
#ifdef TM2C_ADDR
  __sfr __at(TM2C_ADDR)        _tm2c;
  #define TM2C                 _tm2c
#endif
#ifdef TM2CT_ADDR
  __sfr __at(TM2CT_ADDR)       _tm2ct;
  #define TM2CT                _tm2ct
#endif
#ifdef TM2S_ADDR
  __sfr __at(TM2S_ADDR)        _tm2s;
  #define TM2S                 _tm2s
#endif
#ifdef TM2B_ADDR
  __sfr __at(TM2B_ADDR)        _tm2b;
  #define TM2B                 _tm2b
#endif

// Timer3
#ifdef TM3C_ADDR
  __sfr __at(TM3C_ADDR)        _tm3c;
  #define TM3C                 _tm3c
#endif
#ifdef TM3CT_ADDR
  __sfr __at(TM3CT_ADDR)       _tm3ct;
  #define TM3CT                _tm3ct
#endif
#ifdef TM3S_ADDR
  __sfr __at(TM3S_ADDR)        _tm3s;
  #define TM3S                 _tm3s
#endif
#ifdef TM3B_ADDR
  __sfr __at(TM3B_ADDR)         _tm3b;
  #define TM3B                  _tm3b
#endif

// ADC
#ifdef ADCC_ADDR
  __sfr __at(ADCC_ADDR)         _adcc;
  #define ADCC                  _adcc
#endif
#ifdef ADCM_ADDR
  __sfr __at(ADCM_ADDR)         _adcm;
  #define ADCM                  _adcm
#endif
#ifdef ADCR_ADDR
  __sfr __at(ADCR_ADDR)         _adcr;
  #define ADCR                  _adcr
#endif
#ifdef ADCRGC_ADDR
  __sfr __at(ADCRGC_ADDR)       _adcrgc;
  #define ADCRGC                _adcrgc
#endif


// PWM
#ifdef PWMGCLK_ADDR
  __sfr __at(PWMGCLK_ADDR)      _pwmgclk;
  #define PWMGCLK               _pwmgclk
#endif
#ifdef PWMGCUBH_ADDR
  __sfr __at(PWMGCUBH_ADDR)     _pwmgcubh;
  #define PWMGCUBH              _pwmgcubh
#endif
#ifdef PWMGCUBL_ADDR
  __sfr __at(PWMGCUBL_ADDR)     _pwmgcubl;
  #define PWMGCUBL              _pwmgcubl
#endif

// PWM 0
#ifdef PWMG0C_ADDR
  __sfr __at(PWMG0C_ADDR)       _pwmg0c;
  #define PWMG0C                _pwmg0c
#endif
#ifdef PWMG0S_ADDR
  __sfr __at(PWMG0S_ADDR)       _pwmg0s;
  #define PWMG0S                _pwmg0s
#endif
#ifdef PWMG0DTH_ADDR
  __sfr __at(PWMG0DTH_ADDR)     _pwmg0dth;
  #define PWMG0DTH              _pwmg0dth
#endif
#ifdef PWMG0DTL_ADDR
  __sfr __at(PWMG0DTL_ADDR)     _pwmg0dtl;
  #define PWMG0DTL              _pwmg0dtl
#endif
#ifdef PWMG0CUBH_ADDR
  __sfr __at(PWMG0CUBH_ADDR)    _pwmg0cubh;
  #define PWMG0CUBH             _pwmg0cubh
#endif
#ifdef PWMG0CUBL_ADDR
  __sfr __at(PWMG0CUBL_ADDR)    _pwmg0cubl;
  #define PWMG0CUBL             _pwmg0cubl
#endif

// PWM 1
#ifdef PWMG1C_ADDR
  __sfr __at(PWMG1C_ADDR)       _pwmg1c;
  #define PWMG1C                _pwmg1c
#endif
#ifdef PWMG1S_ADDR
  __sfr __at(PWMG1S_ADDR)       _pwmg1s;
  #define PWMG1S                _pwmg1s
#endif
#ifdef PWMG1DTH_ADDR
  __sfr __at(PWMG1DTH_ADDR)     _pwmg1dth;
  #define PWMG1DTH              _pwmg1dth
#endif
#ifdef PWMG1DTL_ADDR
  __sfr __at(PWMG1DTL_ADDR)     _pwmg1dtl;
  #define PWMG1DTL              _pwmg1dtl
#endif
#ifdef PWMG1CUBH_ADDR
  __sfr __at(PWMG1CUBH_ADDR)    _pwmg1cubh;
  #define PWMG1CUBH             _pwmg1cubh
#endif
#ifdef PWMG1CUBL_ADDR
  __sfr __at(PWMG1CUBL_ADDR)    _pwmg1cubl;
  #define PWMG1CUBL             _pwmg1cubl
#endif

// PWM 2
#ifdef PWMG2C_ADDR
  __sfr __at(PWMG2C_ADDR)       _pwmg2c;
  #define PWMG2C                _pwmg2c
#endif
#ifdef PWMG2S_ADDR
  __sfr __at(PWMG2S_ADDR)       _pwmg2s;
  #define PWMG2S                _pwmg2s
#endif
#ifdef PWMG2DTH_ADDR
  __sfr __at(PWMG2DTH_ADDR)     _pwmg2dth;
  #define PWMG2DTH              _pwmg2dth
#endif
#ifdef PWMG2DTL_ADDR
  __sfr __at(PWMG2DTL_ADDR)     _pwmg2dtl;
  #define PWMG2DTL              _pwmg2dtl
#endif
#ifdef PWMG2CUBH_ADDR
  __sfr __at(PWMG2CUBH_ADDR)    _pwmg2cubh;
  #define PWMG2CUBH             _pwmg2cubh
#endif
#ifdef PWMG2CUBL_ADDR
  __sfr __at(PWMG2CUBL_ADDR)    _pwmg2cubl;
  #define PWMG2CUBL             _pwmg2cubl
#endif

// Misc.
#ifdef MISC_ADDR
  __sfr __at(MISC_ADDR)         _misc;
  #define MISC                  _misc
#endif
#ifdef MISC2_ADDR
  __sfr __at(MISC2_ADDR)        _misc2;
  #define MISC2                 _misc2
#endif
#ifdef MISCLVR_ADDR
  __sfr __at(MISCLVR_ADDR)      _misclvr;
  #define MISCLVR               _misclvr
#endif
#ifdef ROP_ADDR
  __sfr __at(ROP_ADDR)          _rop;
  #define ROP                   _rop
#endif

#endif //__PDK_SFR_DEFS_H__
