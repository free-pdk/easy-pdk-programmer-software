#ifndef __PDK_SFR_DEFS_H__
#define __PDK_SFR_DEFS_H__

#if !defined(__PDK_IO_H__)
#  error "You must #include "pdk/io.h" instead of "pdk/sfr_defs.h" by itself."
#endif

// CPU
#if defined(FLAG_ADDR)
  __sfr __at(FLAG_ADDR)         _flag;
  #define FLAG                  _flag
#endif
#if defined(SP_ADDR)
  __sfr __at(SP_ADDR)           _sp;
  #define SP                    _sp
#endif

// Interrupts
#if defined(INTEN_ADDR)
  __sfr __at(INTEN_ADDR)        _inten;
  #define INTEN                 _inten
#endif
#if defined(INTRQ_ADDR)
  __sfr __at(INTRQ_ADDR)        _intrq;
  #define INTRQ                 _intrq
#endif
#if defined(INTEGS_ADDR)
  __sfr __at(INTEGS_ADDR)       _integs;
  #define INTEGS                _integs
#endif

// Clock / Oscillator
#if defined(CLKMD_ADDR)
  __sfr __at(CLKMD_ADDR)        _clkmd;
  #define CLKMD                 _clkmd
#endif
#if defined(IHRCR_ADDR)
  __sfr __at(IHRCR_ADDR)        _ihrcr;
  #define IHRCR                 _ihrcr
#endif
#if defined(ILRCR_ADDR)
  __sfr __at(ILRCR_ADDR)        _ilrcr;
  #define ILRCR                 _ilrcr
#endif
#if defined(EOSCR_ADDR)
  __sfr __at(EOSCR_ADDR)        _eoscr;
  #define EOSCR                 _eoscr
#endif

// BandGap / Comparator
#if defined(BGTR_ADDR)
  __sfr __at(BGTR_ADDR)         _bgtr;
  #define BGTR                  _bgtr
#endif
#if defined(GPCC_ADDR)
  __sfr __at(GPCC_ADDR)         _gpcc;
  #define GPCC                  _gpcc
#endif
#if defined(GPCS_ADDR)
  __sfr __at(GPCS_ADDR)         _gpcs;
  #define GPCS                  _gpcs
#endif

// IO Port A
#if defined(PADIER_ADDR)
  __sfr __at(PADIER_ADDR)      _padier;
  #define PADIER               _padier
#endif
#if defined(PA_ADDR)
  __sfr __at(PA_ADDR)          _pa;
  #define PA                   _pa
#endif
#if defined(PAC_ADDR)
  __sfr __at(PAC_ADDR)         _pac;
  #define PAC                  _pac
#endif
#if defined(PAPH_ADDR)
  __sfr __at(PAPH_ADDR)        _paph;
  #define PAPH                 _paph
#endif

// IO Port B
#if defined(PBDIER_ADDR)
  __sfr __at(PBDIER_ADDR)      _pbdier;
  #define PBDIER               _pbdier
#endif
#if defined(PB_ADDR)
  __sfr __at(PB_ADDR)          _pb;
  #define PB                   _pb
#endif
#if defined(PBC_ADDR)
  __sfr __at(PBC_ADDR)         _pbc;
  #define PBC                  _pbc
#endif
#if defined(PBPH_ADDR)
  __sfr __at(PBPH_ADDR)        _pbph;
  #define PBPH                 _pbph
#endif
#if defined(PBPL_ADDR)
  __sfr __at(PBPL_ADDR)        _pbpl;
  #define PBPL                 _pbpl
#endif

// IO Port C
#if defined(PCDIER_ADDR)
  __sfr __at(PCDIER_ADDR)      _pcdier;
  #define PCDIER               _pcdier
#endif
#if defined(PC_ADDR)
  __sfr __at(PC_ADDR)          _pc;
  #define PC                   _pc
#endif
#if defined(PCC_ADDR)
  __sfr __at(PCC_ADDR)         _pcc;
  #define PCC                  _pcc
#endif
#if defined(PCPH_ADDR)
  __sfr __at(PCPH_ADDR)        _pcph;
  #define PCPH                 _pcph
#endif
#if defined(PCPL_ADDR)
  __sfr __at(PCPL_ADDR)        _pcpl;
  #define PCPL                 _pcpl
#endif

// Timer16
#if defined(T16M_ADDR)
  __sfr __at(T16M_ADDR)         _t16m;
  #define T16M                  _t16m

  __sfr16                       _t16c;
  #define T16C                  _t16c
#endif

// Timer2
#if defined(TM2C_ADDR)
  __sfr __at(TM2C_ADDR)        _tm2c;
  #define TM2C                 _tm2c
#endif
#if defined(TM2CT_ADDR)
  __sfr __at(TM2CT_ADDR)       _tm2ct;
  #define TM2CT                _tm2ct
#endif
#if defined(TM2S_ADDR)
  __sfr __at(TM2S_ADDR)        _tm2s;
  #define TM2S                 _tm2s
#endif
#if defined(TM2B_ADDR)
  __sfr __at(TM2B_ADDR)        _tm2b;
  #define TM2B                 _tm2b
#endif

// Timer3
#if defined(TM3C_ADDR)
  __sfr __at(TM3C_ADDR)        _tm3c;
  #define TM3C                 _tm3c
#endif
#if defined(TM3CT_ADDR)
  __sfr __at(TM3CT_ADDR)       _tm3ct;
  #define TM3CT                _tm3ct
#endif
#if defined(TM3S_ADDR)
  __sfr __at(TM3S_ADDR)        _tm3s;
  #define TM3S                 _tm3s
#endif
#if defined(TM3B_ADDR)
  __sfr __at(TM3B_ADDR)         _tm3b;
  #define TM3B                  _tm3b
#endif

// ADC
#if defined(ADCC_ADDR)
  __sfr __at(ADCC_ADDR)         _adcc;
  #define ADCC                  _adcc
#endif
#if defined(ADCM_ADDR)
  __sfr __at(ADCM_ADDR)         _adcm;
  #define ADCM                  _adcm
#endif
#if defined(ADCR_ADDR)
  __sfr __at(ADCR_ADDR)         _adcr;
  #define ADCR                  _adcr
#endif
#if defined(ADCRGC_ADDR)
  __sfr __at(ADCRGC_ADDR)       _adcrgc;
  #define ADCRGC                _adcrgc
#endif


// PWM
#if defined(PWMGCLK_ADDR)
  __sfr __at(PWMGCLK_ADDR)      _pwmgclk;
  #define PWMGCLK               _pwmgclk
#endif
#if defined(PWMGCUBH_ADDR)
  __sfr __at(PWMGCUBH_ADDR)     _pwmgcubh;
  #define PWMGCUBH              _pwmgcubh
#endif
#if defined(PWMGCUBL_ADDR)
  __sfr __at(PWMGCUBL_ADDR)     _pwmgcubl;
  #define PWMGCUBL              _pwmgcubl
#endif

// PWM 0
#if defined(PWMG0C_ADDR)
  __sfr __at(PWMG0C_ADDR)       _pwmg0c;
  #define PWMG0C                _pwmg0c
#endif
#if defined(PWMG0S_ADDR)
  __sfr __at(PWMG0S_ADDR)       _pwmg0s;
  #define PWMG0S                _pwmg0s
#endif
#if defined(PWMG0DTH_ADDR)
  __sfr __at(PWMG0DTH_ADDR)     _pwmg0dth;
  #define PWMG0DTH              _pwmg0dth
#endif
#if defined(PWMG0DTL_ADDR)
  __sfr __at(PWMG0DTL_ADDR)     _pwmg0dtl;
  #define PWMG0DTL              _pwmg0dtl
#endif
#if defined(PWMG0CUBH_ADDR)
  __sfr __at(PWMG0CUBH_ADDR)    _pwmg0cubh;
  #define PWMG0CUBH             _pwmg0cubh
#endif
#if defined(PWMG0CUBL_ADDR)
  __sfr __at(PWMG0CUBL_ADDR)    _pwmg0cubl;
  #define PWMG0CUBL             _pwmg0cubl
#endif

// PWM 1
#if defined(PWMG1C_ADDR)
  __sfr __at(PWMG1C_ADDR)       _pwmg1c;
  #define PWMG1C                _pwmg1c
#endif
#if defined(PWMG1S_ADDR)
  __sfr __at(PWMG1S_ADDR)       _pwmg1s;
  #define PWMG1S                _pwmg1s
#endif
#if defined(PWMG1DTH_ADDR)
  __sfr __at(PWMG1DTH_ADDR)     _pwmg1dth;
  #define PWMG1DTH              _pwmg1dth
#endif
#if defined(PWMG1DTL_ADDR)
  __sfr __at(PWMG1DTL_ADDR)     _pwmg1dtl;
  #define PWMG1DTL              _pwmg1dtl
#endif
#if defined(PWMG1CUBH_ADDR)
  __sfr __at(PWMG1CUBH_ADDR)    _pwmg1cubh;
  #define PWMG1CUBH             _pwmg1cubh
#endif
#if defined(PWMG1CUBL_ADDR)
  __sfr __at(PWMG1CUBL_ADDR)    _pwmg1cubl;
  #define PWMG1CUBL             _pwmg1cubl
#endif

// PWM 2
#if defined(PWMG2C_ADDR)
  __sfr __at(PWMG2C_ADDR)       _pwmg2c;
  #define PWMG2C                _pwmg2c
#endif
#if defined(PWMG2S_ADDR)
  __sfr __at(PWMG2S_ADDR)       _pwmg2s;
  #define PWMG2S                _pwmg2s
#endif
#if defined(PWMG2DTH_ADDR)
  __sfr __at(PWMG2DTH_ADDR)     _pwmg2dth;
  #define PWMG2DTH              _pwmg2dth
#endif
#if defined(PWMG2DTL_ADDR)
  __sfr __at(PWMG2DTL_ADDR)     _pwmg2dtl;
  #define PWMG2DTL              _pwmg2dtl
#endif
#if defined(PWMG2CUBH_ADDR)
  __sfr __at(PWMG2CUBH_ADDR)    _pwmg2cubh;
  #define PWMG2CUBH             _pwmg2cubh
#endif
#if defined(PWMG2CUBL_ADDR)
  __sfr __at(PWMG2CUBL_ADDR)    _pwmg2cubl;
  #define PWMG2CUBL             _pwmg2cubl
#endif

// Misc.
#if defined(MISC_ADDR)
  __sfr __at(MISC_ADDR)         _misc;
  #define MISC                  _misc
#endif
#if defined(MISC2_ADDR)
  __sfr __at(MISC2_ADDR)        _misc2;
  #define MISC2                 _misc2
#endif
#if defined(MISCLVR_ADDR)
  __sfr __at(MISCLVR_ADDR)      _misclvr;
  #define MISCLVR               _misclvr
#endif
#if defined(ROP_ADDR)
  __sfr __at(ROP_ADDR)          _rop;
  #define ROP                   _rop
#endif

#endif //__PDK_SFR_DEFS_H__
