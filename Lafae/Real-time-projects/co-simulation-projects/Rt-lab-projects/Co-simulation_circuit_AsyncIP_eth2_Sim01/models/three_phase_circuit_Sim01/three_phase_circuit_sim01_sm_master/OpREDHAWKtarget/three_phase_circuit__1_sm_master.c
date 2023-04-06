/*
 * three_phase_circuit__1_sm_master.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "three_phase_circuit__1_sm_master".
 *
 * Model version              : 11.10
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C source code generated on : Tue Jan 17 17:45:37 2023
 *
 * Target selection: rtlab_rtmodel.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "three_phase_circuit__1_sm_master.h"
#include "three_phase_circuit__1_sm_master_private.h"

/* Block signals (default storage) */
B_three_phase_circuit__1_sm_master_T three_phase_circuit__1_sm_master_B;

/* Block states (default storage) */
DW_three_phase_circuit__1_sm_master_T three_phase_circuit__1_sm_master_DW;

/* Real-time model */
static RT_MODEL_three_phase_circuit__1_sm_master_T
  three_phase_circuit__1_sm_master_M_;
RT_MODEL_three_phase_circuit__1_sm_master_T *const
  three_phase_circuit__1_sm_master_M = &three_phase_circuit__1_sm_master_M_;
int32_T div_s32(int32_T numerator, int32_T denominator)
{
  int32_T quotient;
  uint32_T tempAbsQuotient;
  if (denominator == 0) {
    quotient = numerator >= 0 ? MAX_int32_T : MIN_int32_T;

    /* Divide by zero handler */
  } else {
    tempAbsQuotient = (numerator < 0 ? ~(uint32_T)numerator + 1U : (uint32_T)
                       numerator) / (denominator < 0 ? ~(uint32_T)denominator +
      1U : (uint32_T)denominator);
    quotient = (numerator < 0) != (denominator < 0) ? -(int32_T)tempAbsQuotient :
      (int32_T)tempAbsQuotient;
  }

  return quotient;
}

/* Model output function */
static void three_phase_circuit__1_sm_master_output(void)
{
  real_T tmp;

  /* Memory: '<S1>/S-Function' */
  three_phase_circuit__1_sm_master_B.SFunction =
    three_phase_circuit__1_sm_master_DW.SFunction_PreviousInput;

  /* Sum: '<S1>/Sum' incorporates:
   *  Constant: '<S1>/S-Function1'
   */
  three_phase_circuit__1_sm_master_B.Sum =
    three_phase_circuit__1_sm_master_P.SFunction1_Value +
    three_phase_circuit__1_sm_master_B.SFunction;

  /* Stop: '<S1>/Stop Simulation' */
  if (three_phase_circuit__1_sm_master_B.Sum != 0.0) {
    rtmSetStopRequested(three_phase_circuit__1_sm_master_M, 1);
  }

  /* End of Stop: '<S1>/Stop Simulation' */

  /* Constant: '<S2>/timeout' */
  three_phase_circuit__1_sm_master_B.timeout =
    three_phase_circuit__1_sm_master_P.timeout_Value;

  /* S-Function (sfun_recv_async): '<S5>/S-Function1' */

  /* Level2 S-Function Block: '<S5>/S-Function1' (sfun_recv_async) */
  {
    SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[0];
    sfcnOutputs(rts,0);
  }

  /* Sin: '<S30>/Sine Wave A' */
  if (three_phase_circuit__1_sm_master_DW.systemEnable != 0) {
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource_Frequency;
    three_phase_circuit__1_sm_master_DW.lastSin = sin(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource_Frequency;
    three_phase_circuit__1_sm_master_DW.lastCos = cos(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    three_phase_circuit__1_sm_master_DW.systemEnable = 0;
  }

  /* Sin: '<S30>/Sine Wave A' */
  three_phase_circuit__1_sm_master_B.SineWaveA =
    three_phase_circuit__1_sm_master_P.SineWaveA_Amp *
    ((three_phase_circuit__1_sm_master_DW.lastSin *
      three_phase_circuit__1_sm_master_P.SineWaveA_PCos +
      three_phase_circuit__1_sm_master_DW.lastCos *
      three_phase_circuit__1_sm_master_P.SineWaveA_PSin) *
     three_phase_circuit__1_sm_master_P.SineWaveA_HCos +
     (three_phase_circuit__1_sm_master_DW.lastCos *
      three_phase_circuit__1_sm_master_P.SineWaveA_PCos -
      three_phase_circuit__1_sm_master_DW.lastSin *
      three_phase_circuit__1_sm_master_P.SineWaveA_PSin) *
     three_phase_circuit__1_sm_master_P.SineWaveA_Hsin) +
    three_phase_circuit__1_sm_master_P.SineWaveA_Bias;

  /* Sin: '<S30>/Sine Wave B' */
  if (three_phase_circuit__1_sm_master_DW.systemEnable_a != 0) {
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource_Frequency;
    three_phase_circuit__1_sm_master_DW.lastSin_g = sin(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource_Frequency;
    three_phase_circuit__1_sm_master_DW.lastCos_m = cos(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    three_phase_circuit__1_sm_master_DW.systemEnable_a = 0;
  }

  /* Sin: '<S30>/Sine Wave B' */
  three_phase_circuit__1_sm_master_B.SineWaveB =
    three_phase_circuit__1_sm_master_P.SineWaveB_Amp *
    ((three_phase_circuit__1_sm_master_DW.lastSin_g *
      three_phase_circuit__1_sm_master_P.SineWaveB_PCos +
      three_phase_circuit__1_sm_master_DW.lastCos_m *
      three_phase_circuit__1_sm_master_P.SineWaveB_PSin) *
     three_phase_circuit__1_sm_master_P.SineWaveB_HCos +
     (three_phase_circuit__1_sm_master_DW.lastCos_m *
      three_phase_circuit__1_sm_master_P.SineWaveB_PCos -
      three_phase_circuit__1_sm_master_DW.lastSin_g *
      three_phase_circuit__1_sm_master_P.SineWaveB_PSin) *
     three_phase_circuit__1_sm_master_P.SineWaveB_Hsin) +
    three_phase_circuit__1_sm_master_P.SineWaveB_Bias;

  /* Sin: '<S30>/Sine Wave C' */
  if (three_phase_circuit__1_sm_master_DW.systemEnable_k != 0) {
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource_Frequency;
    three_phase_circuit__1_sm_master_DW.lastSin_m = sin(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource_Frequency;
    three_phase_circuit__1_sm_master_DW.lastCos_i = cos(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    three_phase_circuit__1_sm_master_DW.systemEnable_k = 0;
  }

  /* Sin: '<S30>/Sine Wave C' */
  three_phase_circuit__1_sm_master_B.SineWaveC =
    three_phase_circuit__1_sm_master_P.SineWaveC_Amp *
    ((three_phase_circuit__1_sm_master_DW.lastSin_m *
      three_phase_circuit__1_sm_master_P.SineWaveC_PCos +
      three_phase_circuit__1_sm_master_DW.lastCos_i *
      three_phase_circuit__1_sm_master_P.SineWaveC_PSin) *
     three_phase_circuit__1_sm_master_P.SineWaveC_HCos +
     (three_phase_circuit__1_sm_master_DW.lastCos_i *
      three_phase_circuit__1_sm_master_P.SineWaveC_PCos -
      three_phase_circuit__1_sm_master_DW.lastSin_m *
      three_phase_circuit__1_sm_master_P.SineWaveC_PSin) *
     three_phase_circuit__1_sm_master_P.SineWaveC_Hsin) +
    three_phase_circuit__1_sm_master_P.SineWaveC_Bias;

  /* S-Function (sfun_spssw_discc): '<S51>/State-Space' */

  /* S-Function block: <S51>/State-Space */
  {
    real_T accum;

    /*
     * Compute outputs:
     * ---------------
     */
    real_T *Ds = (real_T*)
      three_phase_circuit__1_sm_master_DW.StateSpace_PWORK.DS;

    {
      int_T i1;
      real_T *y0 = &three_phase_circuit__1_sm_master_B.StateSpace[0];
      for (i1=0; i1 < 6; i1++) {
        accum = 0.0;
        accum += *(Ds++) * three_phase_circuit__1_sm_master_B.SFunction1_o3[0];
        accum += *(Ds++) * three_phase_circuit__1_sm_master_B.SFunction1_o3[1];
        accum += *(Ds++) * three_phase_circuit__1_sm_master_B.SFunction1_o3[2];
        accum += *(Ds++) * three_phase_circuit__1_sm_master_B.SineWaveA;
        accum += *(Ds++) * three_phase_circuit__1_sm_master_B.SineWaveB;
        accum += *(Ds++) * three_phase_circuit__1_sm_master_B.SineWaveC;
        y0[i1] = accum;
      }
    }
  }

  /* Gain: '<S18>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain *
    three_phase_circuit__1_sm_master_B.StateSpace[0];

  /* Gain: '<S19>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_d =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_d *
    three_phase_circuit__1_sm_master_B.StateSpace[1];

  /* Gain: '<S20>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_c =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_b *
    three_phase_circuit__1_sm_master_B.StateSpace[2];

  /* Gain: '<S9>/Kv1' */
  three_phase_circuit__1_sm_master_B.Kv1[0] =
    three_phase_circuit__1_sm_master_P.Kv1_Gain *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain;
  three_phase_circuit__1_sm_master_B.Kv1[1] =
    three_phase_circuit__1_sm_master_P.Kv1_Gain *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_d;
  three_phase_circuit__1_sm_master_B.Kv1[2] =
    three_phase_circuit__1_sm_master_P.Kv1_Gain *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_c;

  /* Sin: '<S48>/Sine Wave A' */
  if (three_phase_circuit__1_sm_master_DW.systemEnable_d != 0) {
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource1_Frequency;
    three_phase_circuit__1_sm_master_DW.lastSin_k = sin(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource1_Frequency;
    three_phase_circuit__1_sm_master_DW.lastCos_a = cos(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    three_phase_circuit__1_sm_master_DW.systemEnable_d = 0;
  }

  /* Sin: '<S48>/Sine Wave A' */
  three_phase_circuit__1_sm_master_B.SineWaveA_p =
    three_phase_circuit__1_sm_master_P.SineWaveA_Amp_j *
    ((three_phase_circuit__1_sm_master_DW.lastSin_k *
      three_phase_circuit__1_sm_master_P.SineWaveA_PCos_a +
      three_phase_circuit__1_sm_master_DW.lastCos_a *
      three_phase_circuit__1_sm_master_P.SineWaveA_PSin_f) *
     three_phase_circuit__1_sm_master_P.SineWaveA_HCos_c +
     (three_phase_circuit__1_sm_master_DW.lastCos_a *
      three_phase_circuit__1_sm_master_P.SineWaveA_PCos_a -
      three_phase_circuit__1_sm_master_DW.lastSin_k *
      three_phase_circuit__1_sm_master_P.SineWaveA_PSin_f) *
     three_phase_circuit__1_sm_master_P.SineWaveA_Hsin_l) +
    three_phase_circuit__1_sm_master_P.SineWaveA_Bias_k;

  /* Sin: '<S48>/Sine Wave B' */
  if (three_phase_circuit__1_sm_master_DW.systemEnable_e != 0) {
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource1_Frequency;
    three_phase_circuit__1_sm_master_DW.lastSin_kg = sin(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource1_Frequency;
    three_phase_circuit__1_sm_master_DW.lastCos_l = cos(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    three_phase_circuit__1_sm_master_DW.systemEnable_e = 0;
  }

  /* Sin: '<S48>/Sine Wave B' */
  three_phase_circuit__1_sm_master_B.SineWaveB_p =
    three_phase_circuit__1_sm_master_P.SineWaveB_Amp_a *
    ((three_phase_circuit__1_sm_master_DW.lastSin_kg *
      three_phase_circuit__1_sm_master_P.SineWaveB_PCos_f +
      three_phase_circuit__1_sm_master_DW.lastCos_l *
      three_phase_circuit__1_sm_master_P.SineWaveB_PSin_i) *
     three_phase_circuit__1_sm_master_P.SineWaveB_HCos_l +
     (three_phase_circuit__1_sm_master_DW.lastCos_l *
      three_phase_circuit__1_sm_master_P.SineWaveB_PCos_f -
      three_phase_circuit__1_sm_master_DW.lastSin_kg *
      three_phase_circuit__1_sm_master_P.SineWaveB_PSin_i) *
     three_phase_circuit__1_sm_master_P.SineWaveB_Hsin_g) +
    three_phase_circuit__1_sm_master_P.SineWaveB_Bias_e;

  /* Sin: '<S48>/Sine Wave C' */
  if (three_phase_circuit__1_sm_master_DW.systemEnable_g != 0) {
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource1_Frequency;
    three_phase_circuit__1_sm_master_DW.lastSin_p = sin(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    tmp = 2.0 * 3.1415926535897931 *
      three_phase_circuit__1_sm_master_P.ThreePhaseSource1_Frequency;
    three_phase_circuit__1_sm_master_DW.lastCos_h = cos(tmp *
      three_phase_circuit__1_sm_master_M->Timing.t[0]);
    three_phase_circuit__1_sm_master_DW.systemEnable_g = 0;
  }

  /* Sin: '<S48>/Sine Wave C' */
  three_phase_circuit__1_sm_master_B.SineWaveC_i =
    three_phase_circuit__1_sm_master_P.SineWaveC_Amp_c *
    ((three_phase_circuit__1_sm_master_DW.lastSin_p *
      three_phase_circuit__1_sm_master_P.SineWaveC_PCos_f +
      three_phase_circuit__1_sm_master_DW.lastCos_h *
      three_phase_circuit__1_sm_master_P.SineWaveC_PSin_b) *
     three_phase_circuit__1_sm_master_P.SineWaveC_HCos_j +
     (three_phase_circuit__1_sm_master_DW.lastCos_h *
      three_phase_circuit__1_sm_master_P.SineWaveC_PCos_f -
      three_phase_circuit__1_sm_master_DW.lastSin_p *
      three_phase_circuit__1_sm_master_P.SineWaveC_PSin_b) *
     three_phase_circuit__1_sm_master_P.SineWaveC_Hsin_e) +
    three_phase_circuit__1_sm_master_P.SineWaveC_Bias_e;

  /* S-Function (sfun_spssw_discc): '<S52>/State-Space' */

  /* S-Function block: <S52>/State-Space */
  {
    real_T accum;

    /*
     * Compute outputs:
     * ---------------
     */
    real_T *Cs = (real_T*)
      three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.CS;
    real_T *Ds = (real_T*)
      three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.DS;

    {
      int_T i1;
      real_T *y0 = &three_phase_circuit__1_sm_master_B.StateSpace_f[0];
      for (i1=0; i1 < 6; i1++) {
        accum = 0.0;

        {
          int_T i2;
          real_T *xd = &three_phase_circuit__1_sm_master_DW.StateSpace_DSTATE[0];
          for (i2=0; i2 < 6; i2++) {
            accum += *(Cs++) * xd[i2];
          }
        }

        accum += *(Ds++) * three_phase_circuit__1_sm_master_B.SineWaveA_p;
        accum += *(Ds++) * three_phase_circuit__1_sm_master_B.SineWaveB_p;
        accum += *(Ds++) * three_phase_circuit__1_sm_master_B.SineWaveC_i;
        y0[i1] = accum;
      }
    }
  }

  /* Gain: '<S39>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_j =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_n *
    three_phase_circuit__1_sm_master_B.StateSpace_f[0];

  /* Gain: '<S40>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_i =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_a *
    three_phase_circuit__1_sm_master_B.StateSpace_f[1];

  /* Gain: '<S41>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_p =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_dr *
    three_phase_circuit__1_sm_master_B.StateSpace_f[2];

  /* Gain: '<S31>/Kv1' */
  three_phase_circuit__1_sm_master_B.Kv1_p[0] =
    three_phase_circuit__1_sm_master_P.Kv1_Gain_d *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_j;
  three_phase_circuit__1_sm_master_B.Kv1_p[1] =
    three_phase_circuit__1_sm_master_P.Kv1_Gain_d *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_i;
  three_phase_circuit__1_sm_master_B.Kv1_p[2] =
    three_phase_circuit__1_sm_master_P.Kv1_Gain_d *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_p;

  /* Gain: '<S36>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_k =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_k *
    three_phase_circuit__1_sm_master_B.StateSpace_f[3];

  /* Gain: '<S37>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_g =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_g *
    three_phase_circuit__1_sm_master_B.StateSpace_f[4];

  /* Gain: '<S38>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_i1 =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_e *
    three_phase_circuit__1_sm_master_B.StateSpace_f[5];

  /* Gain: '<S31>/Kv' */
  three_phase_circuit__1_sm_master_B.Kv[0] =
    three_phase_circuit__1_sm_master_P.Kv_Gain *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_k;
  three_phase_circuit__1_sm_master_B.Kv[1] =
    three_phase_circuit__1_sm_master_P.Kv_Gain *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_g;
  three_phase_circuit__1_sm_master_B.Kv[2] =
    three_phase_circuit__1_sm_master_P.Kv_Gain *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_i1;

  /* S-Function (OP_SEND): '<S50>/S-Function' */

  /* Level2 S-Function Block: '<S50>/S-Function' (OP_SEND) */
  {
    SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[1];
    sfcnOutputs(rts,0);
  }

  /* Gain: '<S15>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_di =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_f *
    three_phase_circuit__1_sm_master_B.StateSpace[3];

  /* Gain: '<S16>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_gn =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_j *
    three_phase_circuit__1_sm_master_B.StateSpace[4];

  /* Gain: '<S17>/do not delete this gain' */
  three_phase_circuit__1_sm_master_B.donotdeletethisgain_g1 =
    three_phase_circuit__1_sm_master_P.donotdeletethisgain_Gain_i *
    three_phase_circuit__1_sm_master_B.StateSpace[5];

  /* Gain: '<S9>/Kv' */
  three_phase_circuit__1_sm_master_B.Kv_f[0] =
    three_phase_circuit__1_sm_master_P.Kv_Gain_h *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_di;
  three_phase_circuit__1_sm_master_B.Kv_f[1] =
    three_phase_circuit__1_sm_master_P.Kv_Gain_h *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_gn;
  three_phase_circuit__1_sm_master_B.Kv_f[2] =
    three_phase_circuit__1_sm_master_P.Kv_Gain_h *
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_g1;

  /* DiscretePulseGenerator: '<S2>/data ready 2.5 kHz' */
  three_phase_circuit__1_sm_master_B.dataready25kHz = ((real_T)
    three_phase_circuit__1_sm_master_DW.clockTickCounter <
    three_phase_circuit__1_sm_master_P.dataready25kHz_Duty) &&
    (three_phase_circuit__1_sm_master_DW.clockTickCounter >= 0) ?
    three_phase_circuit__1_sm_master_P.dataready25kHz_Amp : 0.0;

  /* DiscretePulseGenerator: '<S2>/data ready 2.5 kHz' */
  if ((real_T)three_phase_circuit__1_sm_master_DW.clockTickCounter >=
      three_phase_circuit__1_sm_master_P.dataready25kHz_Period - (real_T)1) {
    three_phase_circuit__1_sm_master_DW.clockTickCounter = 0;
  } else {
    three_phase_circuit__1_sm_master_DW.clockTickCounter =
      three_phase_circuit__1_sm_master_DW.clockTickCounter + 1;
  }

  /* S-Function (sfun_send_async): '<S8>/S-Function2' */

  /* Level2 S-Function Block: '<S8>/S-Function2' (sfun_send_async) */
  {
    SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[2];
    sfcnOutputs(rts,0);
  }

  /* S-Function (sfun_gen_async_ctrl): '<S2>/OpIPSocketCtrl' */

  /* Level2 S-Function Block: '<S2>/OpIPSocketCtrl' (sfun_gen_async_ctrl) */
  {
    SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[3];
    sfcnOutputs(rts,0);
  }
}

/* Model update function */
static void three_phase_circuit__1_sm_master_update(void)
{
  real_T HoldCosine;
  real_T HoldSine;

  /* Update for Memory: '<S1>/S-Function' */
  three_phase_circuit__1_sm_master_DW.SFunction_PreviousInput =
    three_phase_circuit__1_sm_master_B.Sum;

  /* Update for Sin: '<S30>/Sine Wave A' */
  HoldSine = three_phase_circuit__1_sm_master_DW.lastSin;
  HoldCosine = three_phase_circuit__1_sm_master_DW.lastCos;
  three_phase_circuit__1_sm_master_DW.lastSin = HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveA_HCos + HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveA_Hsin;
  three_phase_circuit__1_sm_master_DW.lastCos = HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveA_HCos - HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveA_Hsin;

  /* Update for Sin: '<S30>/Sine Wave B' */
  HoldSine = three_phase_circuit__1_sm_master_DW.lastSin_g;
  HoldCosine = three_phase_circuit__1_sm_master_DW.lastCos_m;
  three_phase_circuit__1_sm_master_DW.lastSin_g = HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveB_HCos + HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveB_Hsin;
  three_phase_circuit__1_sm_master_DW.lastCos_m = HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveB_HCos - HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveB_Hsin;

  /* Update for Sin: '<S30>/Sine Wave C' */
  HoldSine = three_phase_circuit__1_sm_master_DW.lastSin_m;
  HoldCosine = three_phase_circuit__1_sm_master_DW.lastCos_i;
  three_phase_circuit__1_sm_master_DW.lastSin_m = HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveC_HCos + HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveC_Hsin;
  three_phase_circuit__1_sm_master_DW.lastCos_i = HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveC_HCos - HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveC_Hsin;

  /* Update for Sin: '<S48>/Sine Wave A' */
  HoldSine = three_phase_circuit__1_sm_master_DW.lastSin_k;
  HoldCosine = three_phase_circuit__1_sm_master_DW.lastCos_a;
  three_phase_circuit__1_sm_master_DW.lastSin_k = HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveA_HCos_c + HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveA_Hsin_l;
  three_phase_circuit__1_sm_master_DW.lastCos_a = HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveA_HCos_c - HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveA_Hsin_l;

  /* Update for Sin: '<S48>/Sine Wave B' */
  HoldSine = three_phase_circuit__1_sm_master_DW.lastSin_kg;
  HoldCosine = three_phase_circuit__1_sm_master_DW.lastCos_l;
  three_phase_circuit__1_sm_master_DW.lastSin_kg = HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveB_HCos_l + HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveB_Hsin_g;
  three_phase_circuit__1_sm_master_DW.lastCos_l = HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveB_HCos_l - HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveB_Hsin_g;

  /* Update for Sin: '<S48>/Sine Wave C' */
  HoldSine = three_phase_circuit__1_sm_master_DW.lastSin_p;
  HoldCosine = three_phase_circuit__1_sm_master_DW.lastCos_h;
  three_phase_circuit__1_sm_master_DW.lastSin_p = HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveC_HCos_j + HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveC_Hsin_e;
  three_phase_circuit__1_sm_master_DW.lastCos_h = HoldCosine *
    three_phase_circuit__1_sm_master_P.SineWaveC_HCos_j - HoldSine *
    three_phase_circuit__1_sm_master_P.SineWaveC_Hsin_e;

  /* Update for S-Function (sfun_spssw_discc): '<S52>/State-Space' */

  /* S-Function block: <S52>/State-Space */
  {
    const real_T *As = (real_T*)
      three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.AS;
    const real_T *Bs = (real_T*)
      three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.BS;
    real_T *xtmp = (real_T*)
      three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.XTMP;
    real_T accum;

    /* Calculate new states... */
    {
      int_T i1;
      real_T *xd = &three_phase_circuit__1_sm_master_DW.StateSpace_DSTATE[0];
      for (i1=0; i1 < 6; i1++) {
        accum = 0.0;

        {
          int_T i2;
          real_T *xd = &three_phase_circuit__1_sm_master_DW.StateSpace_DSTATE[0];
          for (i2=0; i2 < 6; i2++) {
            accum += *(As++) * xd[i2];
          }
        }

        accum += *(Bs++) * three_phase_circuit__1_sm_master_B.SineWaveA_p;
        accum += *(Bs++) * three_phase_circuit__1_sm_master_B.SineWaveB_p;
        accum += *(Bs++) * three_phase_circuit__1_sm_master_B.SineWaveC_i;
        xtmp[i1] = accum;
      }
    }

    {
      int_T i1;
      real_T *xd = &three_phase_circuit__1_sm_master_DW.StateSpace_DSTATE[0];
      for (i1=0; i1 < 6; i1++) {
        xd[i1] = xtmp[i1];
      }
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++three_phase_circuit__1_sm_master_M->Timing.clockTick0)) {
    ++three_phase_circuit__1_sm_master_M->Timing.clockTickH0;
  }

  three_phase_circuit__1_sm_master_M->Timing.t[0] =
    three_phase_circuit__1_sm_master_M->Timing.clockTick0 *
    three_phase_circuit__1_sm_master_M->Timing.stepSize0 +
    three_phase_circuit__1_sm_master_M->Timing.clockTickH0 *
    three_phase_circuit__1_sm_master_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
static void three_phase_circuit__1_sm_master_initialize(void)
{
  /* Start for Constant: '<S2>/timeout' */
  three_phase_circuit__1_sm_master_B.timeout =
    three_phase_circuit__1_sm_master_P.timeout_Value;

  /* Start for S-Function (sfun_spssw_discc): '<S51>/State-Space' */

  /* S-Function block: <S51>/State-Space */
  {
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK.DS = (real_T*)calloc(6 *
      6, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK.DX_COL = (real_T*)
      calloc(6, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK.TMP2 = (real_T*)calloc
      (6, sizeof(real_T));
  }

  /* Start for S-Function (sfun_spssw_discc): '<S52>/State-Space' */

  /* S-Function block: <S52>/State-Space */
  {
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.AS = (real_T*)calloc
      (6 * 6, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.BS = (real_T*)calloc
      (6 * 3, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.CS = (real_T*)calloc
      (6 * 6, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.DS = (real_T*)calloc
      (6 * 3, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.DX_COL = (real_T*)
      calloc(6, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.TMP2 = (real_T*)
      calloc(3, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.BD_COL = (real_T*)
      calloc(6, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.TMP1 = (real_T*)
      calloc(6, sizeof(real_T));
    three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.XTMP = (real_T*)
      calloc(6, sizeof(real_T));
  }

  {
    int32_T Ns = 0;

    /* user code (Initialize function Body) */

    /* System '<Root>' */

    /* Opal-RT Technologies */
    opalSizeDwork = sizeof(rtDWork);
    if (Opal_rtmGetNumBlockIO(pRtModel) != 0)
      opalSizeBlockIO = sizeof(rtB);
    else
      opalSizeBlockIO = 0;
    if (Opal_rtmGetNumBlockParams(pRtModel) != 0)
      opalSizeRTP = sizeof(rtP);
    else
      opalSizeRTP = 0;

    /* InitializeConditions for Memory: '<S1>/S-Function' */
    three_phase_circuit__1_sm_master_DW.SFunction_PreviousInput =
      three_phase_circuit__1_sm_master_P.SFunction_InitialCondition;

    /* InitializeConditions for S-Function (sfun_recv_async): '<S5>/S-Function1' */
    /* Level2 S-Function Block: '<S5>/S-Function1' (sfun_recv_async) */
    {
      SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[0];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (sfun_spssw_discc): '<S51>/State-Space' */
    {
      int32_T i, j;
      real_T *Ds = (real_T*)
        three_phase_circuit__1_sm_master_DW.StateSpace_PWORK.DS;

      /* Copy and transpose D */
      for (i=0; i<6; i++) {
        for (j=0; j<6; j++)
          Ds[i*6 + j] =
            (three_phase_circuit__1_sm_master_P.StateSpace_DS_param[i + j*6]);
      }
    }

    /* InitializeConditions for S-Function (sfun_spssw_discc): '<S52>/State-Space' */
    {
      int32_T i, j;
      real_T *As = (real_T*)
        three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.AS;
      real_T *Bs = (real_T*)
        three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.BS;
      real_T *Cs = (real_T*)
        three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.CS;
      real_T *Ds = (real_T*)
        three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.DS;
      real_T *X0 = (real_T*)
        &three_phase_circuit__1_sm_master_DW.StateSpace_DSTATE[0];
      for (i = 0; i < 6; i++) {
        X0[i] = (three_phase_circuit__1_sm_master_P.StateSpace_X0_param[i]);
      }

      /* Copy and transpose A and B */
      for (i=0; i<6; i++) {
        for (j=0; j<6; j++)
          As[i*6 + j] =
            (three_phase_circuit__1_sm_master_P.StateSpace_AS_param[i + j*6]);
        for (j=0; j<3; j++)
          Bs[i*3 + j] =
            (three_phase_circuit__1_sm_master_P.StateSpace_BS_param[i + j*6]);
      }

      /* Copy and transpose C */
      for (i=0; i<6; i++) {
        for (j=0; j<6; j++)
          Cs[i*6 + j] =
            (three_phase_circuit__1_sm_master_P.StateSpace_CS_param[i + j*6]);
      }

      /* Copy and transpose D */
      for (i=0; i<6; i++) {
        for (j=0; j<3; j++)
          Ds[i*3 + j] =
            (three_phase_circuit__1_sm_master_P.StateSpace_DS_param_c[i + j*6]);
      }
    }

    /* InitializeConditions for S-Function (OP_SEND): '<S50>/S-Function' */
    /* Level2 S-Function Block: '<S50>/S-Function' (OP_SEND) */
    {
      SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[1];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for DiscretePulseGenerator: '<S2>/data ready 2.5 kHz' */
    if (Ns <= 0) {
      three_phase_circuit__1_sm_master_DW.clockTickCounter = Ns;
    } else {
      three_phase_circuit__1_sm_master_DW.clockTickCounter = Ns - (int32_T)
        (three_phase_circuit__1_sm_master_P.dataready25kHz_Period * (real_T)
         div_s32(Ns, (int32_T)
                 three_phase_circuit__1_sm_master_P.dataready25kHz_Period));
    }

    /* End of InitializeConditions for DiscretePulseGenerator: '<S2>/data ready 2.5 kHz' */

    /* InitializeConditions for S-Function (sfun_send_async): '<S8>/S-Function2' */
    /* Level2 S-Function Block: '<S8>/S-Function2' (sfun_send_async) */
    {
      SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[2];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (sfun_gen_async_ctrl): '<S2>/OpIPSocketCtrl' */
    /* Level2 S-Function Block: '<S2>/OpIPSocketCtrl' (sfun_gen_async_ctrl) */
    {
      SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[3];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }
  }

  /* Enable for Sin: '<S30>/Sine Wave A' */
  three_phase_circuit__1_sm_master_DW.systemEnable = 1;

  /* Enable for Sin: '<S30>/Sine Wave B' */
  three_phase_circuit__1_sm_master_DW.systemEnable_a = 1;

  /* Enable for Sin: '<S30>/Sine Wave C' */
  three_phase_circuit__1_sm_master_DW.systemEnable_k = 1;

  /* Enable for Sin: '<S48>/Sine Wave A' */
  three_phase_circuit__1_sm_master_DW.systemEnable_d = 1;

  /* Enable for Sin: '<S48>/Sine Wave B' */
  three_phase_circuit__1_sm_master_DW.systemEnable_e = 1;

  /* Enable for Sin: '<S48>/Sine Wave C' */
  three_phase_circuit__1_sm_master_DW.systemEnable_g = 1;
}

/* Model terminate function */
static void three_phase_circuit__1_sm_master_terminate(void)
{
  /* Terminate for S-Function (sfun_recv_async): '<S5>/S-Function1' */
  /* Level2 S-Function Block: '<S5>/S-Function1' (sfun_recv_async) */
  {
    SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (sfun_spssw_discc): '<S51>/State-Space' */

  /* S-Function block: <S51>/State-Space */
  {
    /* Free memory */
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK.DS);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK.DX_COL);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK.TMP2);
  }

  /* Terminate for S-Function (sfun_spssw_discc): '<S52>/State-Space' */

  /* S-Function block: <S52>/State-Space */
  {
    /* Free memory */
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.AS);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.BS);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.CS);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.DS);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.DX_COL);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.TMP2);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.BD_COL);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.TMP1);
    free(three_phase_circuit__1_sm_master_DW.StateSpace_PWORK_m.XTMP);
  }

  /* Terminate for S-Function (OP_SEND): '<S50>/S-Function' */
  /* Level2 S-Function Block: '<S50>/S-Function' (OP_SEND) */
  {
    SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[1];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (sfun_send_async): '<S8>/S-Function2' */
  /* Level2 S-Function Block: '<S8>/S-Function2' (sfun_send_async) */
  {
    SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[2];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (sfun_gen_async_ctrl): '<S2>/OpIPSocketCtrl' */
  /* Level2 S-Function Block: '<S2>/OpIPSocketCtrl' (sfun_gen_async_ctrl) */
  {
    SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[3];
    sfcnTerminate(rts);
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  three_phase_circuit__1_sm_master_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  three_phase_circuit__1_sm_master_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  three_phase_circuit__1_sm_master_initialize();
}

void MdlTerminate(void)
{
  three_phase_circuit__1_sm_master_terminate();
}

/* Registration function */
RT_MODEL_three_phase_circuit__1_sm_master_T *three_phase_circuit__1_sm_master
  (void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)three_phase_circuit__1_sm_master_M, 0,
                sizeof(RT_MODEL_three_phase_circuit__1_sm_master_T));
  rtsiSetSolverName(&three_phase_circuit__1_sm_master_M->solverInfo,
                    "FixedStepDiscrete");
  three_phase_circuit__1_sm_master_M->solverInfoPtr =
    (&three_phase_circuit__1_sm_master_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap =
      three_phase_circuit__1_sm_master_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    three_phase_circuit__1_sm_master_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap
      [0]);
    three_phase_circuit__1_sm_master_M->Timing.sampleTimes =
      (&three_phase_circuit__1_sm_master_M->Timing.sampleTimesArray[0]);
    three_phase_circuit__1_sm_master_M->Timing.offsetTimes =
      (&three_phase_circuit__1_sm_master_M->Timing.offsetTimesArray[0]);

    /* task periods */
    three_phase_circuit__1_sm_master_M->Timing.sampleTimes[0] = (7.5E-5);

    /* task offsets */
    three_phase_circuit__1_sm_master_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(three_phase_circuit__1_sm_master_M,
             &three_phase_circuit__1_sm_master_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits =
      three_phase_circuit__1_sm_master_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    three_phase_circuit__1_sm_master_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(three_phase_circuit__1_sm_master_M, -1);
  three_phase_circuit__1_sm_master_M->Timing.stepSize0 = 7.5E-5;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    three_phase_circuit__1_sm_master_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(three_phase_circuit__1_sm_master_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(three_phase_circuit__1_sm_master_M->rtwLogInfo, (NULL));
    rtliSetLogT(three_phase_circuit__1_sm_master_M->rtwLogInfo, "");
    rtliSetLogX(three_phase_circuit__1_sm_master_M->rtwLogInfo, "");
    rtliSetLogXFinal(three_phase_circuit__1_sm_master_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(three_phase_circuit__1_sm_master_M->rtwLogInfo,
      "rt_");
    rtliSetLogFormat(three_phase_circuit__1_sm_master_M->rtwLogInfo, 0);
    rtliSetLogMaxRows(three_phase_circuit__1_sm_master_M->rtwLogInfo, 1000);
    rtliSetLogDecimation(three_phase_circuit__1_sm_master_M->rtwLogInfo, 1);
    rtliSetLogY(three_phase_circuit__1_sm_master_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(three_phase_circuit__1_sm_master_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(three_phase_circuit__1_sm_master_M->rtwLogInfo, (NULL));
  }

  three_phase_circuit__1_sm_master_M->solverInfoPtr =
    (&three_phase_circuit__1_sm_master_M->solverInfo);
  three_phase_circuit__1_sm_master_M->Timing.stepSize = (7.5E-5);
  rtsiSetFixedStepSize(&three_phase_circuit__1_sm_master_M->solverInfo, 7.5E-5);
  rtsiSetSolverMode(&three_phase_circuit__1_sm_master_M->solverInfo,
                    SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  three_phase_circuit__1_sm_master_M->ModelData.blockIO = ((void *)
    &three_phase_circuit__1_sm_master_B);

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      three_phase_circuit__1_sm_master_B.StateSpace[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      three_phase_circuit__1_sm_master_B.StateSpace_f[i] = 0.0;
    }

    three_phase_circuit__1_sm_master_B.SFunction = 0.0;
    three_phase_circuit__1_sm_master_B.Sum = 0.0;
    three_phase_circuit__1_sm_master_B.timeout = 0.0;
    three_phase_circuit__1_sm_master_B.SFunction1_o1 = 0.0;
    three_phase_circuit__1_sm_master_B.SFunction1_o2 = 0.0;
    three_phase_circuit__1_sm_master_B.SFunction1_o3[0] = 0.0;
    three_phase_circuit__1_sm_master_B.SFunction1_o3[1] = 0.0;
    three_phase_circuit__1_sm_master_B.SFunction1_o3[2] = 0.0;
    three_phase_circuit__1_sm_master_B.SineWaveA = 0.0;
    three_phase_circuit__1_sm_master_B.SineWaveB = 0.0;
    three_phase_circuit__1_sm_master_B.SineWaveC = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_d = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_c = 0.0;
    three_phase_circuit__1_sm_master_B.Kv1[0] = 0.0;
    three_phase_circuit__1_sm_master_B.Kv1[1] = 0.0;
    three_phase_circuit__1_sm_master_B.Kv1[2] = 0.0;
    three_phase_circuit__1_sm_master_B.SineWaveA_p = 0.0;
    three_phase_circuit__1_sm_master_B.SineWaveB_p = 0.0;
    three_phase_circuit__1_sm_master_B.SineWaveC_i = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_j = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_i = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_p = 0.0;
    three_phase_circuit__1_sm_master_B.Kv1_p[0] = 0.0;
    three_phase_circuit__1_sm_master_B.Kv1_p[1] = 0.0;
    three_phase_circuit__1_sm_master_B.Kv1_p[2] = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_k = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_g = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_i1 = 0.0;
    three_phase_circuit__1_sm_master_B.Kv[0] = 0.0;
    three_phase_circuit__1_sm_master_B.Kv[1] = 0.0;
    three_phase_circuit__1_sm_master_B.Kv[2] = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_di = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_gn = 0.0;
    three_phase_circuit__1_sm_master_B.donotdeletethisgain_g1 = 0.0;
    three_phase_circuit__1_sm_master_B.Kv_f[0] = 0.0;
    three_phase_circuit__1_sm_master_B.Kv_f[1] = 0.0;
    three_phase_circuit__1_sm_master_B.Kv_f[2] = 0.0;
    three_phase_circuit__1_sm_master_B.dataready25kHz = 0.0;
    three_phase_circuit__1_sm_master_B.SFunction2 = 0.0;
  }

  /* parameters */
  three_phase_circuit__1_sm_master_M->ModelData.defaultParam = ((real_T *)
    &three_phase_circuit__1_sm_master_P);

  /* states (dwork) */
  three_phase_circuit__1_sm_master_M->ModelData.dwork = ((void *)
    &three_phase_circuit__1_sm_master_DW);
  (void) memset((void *)&three_phase_circuit__1_sm_master_DW, 0,
                sizeof(DW_three_phase_circuit__1_sm_master_T));

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      three_phase_circuit__1_sm_master_DW.StateSpace_DSTATE[i] = 0.0;
    }
  }

  three_phase_circuit__1_sm_master_DW.SFunction_PreviousInput = 0.0;
  three_phase_circuit__1_sm_master_DW.lastSin = 0.0;
  three_phase_circuit__1_sm_master_DW.lastCos = 0.0;
  three_phase_circuit__1_sm_master_DW.lastSin_g = 0.0;
  three_phase_circuit__1_sm_master_DW.lastCos_m = 0.0;
  three_phase_circuit__1_sm_master_DW.lastSin_m = 0.0;
  three_phase_circuit__1_sm_master_DW.lastCos_i = 0.0;
  three_phase_circuit__1_sm_master_DW.lastSin_k = 0.0;
  three_phase_circuit__1_sm_master_DW.lastCos_a = 0.0;
  three_phase_circuit__1_sm_master_DW.lastSin_kg = 0.0;
  three_phase_circuit__1_sm_master_DW.lastCos_l = 0.0;
  three_phase_circuit__1_sm_master_DW.lastSin_p = 0.0;
  three_phase_circuit__1_sm_master_DW.lastCos_h = 0.0;

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo =
      &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.sfcnInfo;
    three_phase_circuit__1_sm_master_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus
      (three_phase_circuit__1_sm_master_M)));
    rtssSetNumRootSampTimesPtr(sfcnInfo,
      &three_phase_circuit__1_sm_master_M->Sizes.numSampTimes);
    three_phase_circuit__1_sm_master_M->NonInlinedSFcns.taskTimePtrs[0] =
      &(rtmGetTPtr(three_phase_circuit__1_sm_master_M)[0]);
    rtssSetTPtrPtr(sfcnInfo,
                   three_phase_circuit__1_sm_master_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(three_phase_circuit__1_sm_master_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(three_phase_circuit__1_sm_master_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput
      (three_phase_circuit__1_sm_master_M));
    rtssSetStepSizePtr(sfcnInfo,
                       &three_phase_circuit__1_sm_master_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested
      (three_phase_circuit__1_sm_master_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo,
      &three_phase_circuit__1_sm_master_M->ModelData.derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo,
      &three_phase_circuit__1_sm_master_M->ModelData.zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &three_phase_circuit__1_sm_master_M->ModelData.CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo,
                         &three_phase_circuit__1_sm_master_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo,
      &three_phase_circuit__1_sm_master_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &three_phase_circuit__1_sm_master_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo,
                         &three_phase_circuit__1_sm_master_M->solverInfoPtr);
  }

  three_phase_circuit__1_sm_master_M->Sizes.numSFcns = (4);

  /* register each child */
  {
    (void) memset((void *)
                  &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.childSFunctions
                  [0], 0,
                  4*sizeof(SimStruct));
    three_phase_circuit__1_sm_master_M->childSfunctions =
      (&three_phase_circuit__1_sm_master_M->NonInlinedSFcns.childSFunctionPtrs[0]);
    three_phase_circuit__1_sm_master_M->childSfunctions[0] =
      (&three_phase_circuit__1_sm_master_M->NonInlinedSFcns.childSFunctions[0]);
    three_phase_circuit__1_sm_master_M->childSfunctions[1] =
      (&three_phase_circuit__1_sm_master_M->NonInlinedSFcns.childSFunctions[1]);
    three_phase_circuit__1_sm_master_M->childSfunctions[2] =
      (&three_phase_circuit__1_sm_master_M->NonInlinedSFcns.childSFunctions[2]);
    three_phase_circuit__1_sm_master_M->childSfunctions[3] =
      (&three_phase_circuit__1_sm_master_M->NonInlinedSFcns.childSFunctions[3]);

    /* Level2 S-Function Block: three_phase_circuit__1_sm_master/<S5>/S-Function1 (sfun_recv_async) */
    {
      SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, three_phase_circuit__1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods2
                           [0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods3
                           [0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods4
                           [0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [0]);
        ssSetPeriodicStatesInfo(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [0]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.UPtrs0;
          sfcnUPtrs[0] = &three_phase_circuit__1_sm_master_B.timeout;
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 1);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 3);
        _ssSetPortInfo2ForOutputUnits(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            &three_phase_circuit__1_sm_master_B.SFunction1_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            &three_phase_circuit__1_sm_master_B.SFunction1_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidth(rts, 2, 3);
          ssSetOutputPortSignal(rts, 2, ((real_T *)
            three_phase_circuit__1_sm_master_B.SFunction1_o3));
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function1");
      ssSetPath(rts,
                "three_phase_circuit__1_sm_master/SM_master/OpAsyncRecv/S-Function1");
      ssSetRTModel(rts,three_phase_circuit__1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 14);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P12_Size);
        ssSetSFcnParam(rts, 12, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P13_Size);
        ssSetSFcnParam(rts, 13, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction1_P14_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &three_phase_circuit__1_sm_master_DW.SFunction1_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn0.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0, &three_phase_circuit__1_sm_master_DW.SFunction1_PWORK);
      }

      /* registration */
      sfun_recv_async(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 7.5E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetOutputPortWidth(rts, 2, 3);
      ssSetOutputPortDataType(rts, 2, SS_DOUBLE);
      ssSetOutputPortComplexSignal(rts, 2, 0);
      ssSetOutputPortFrameData(rts, 2, 0);
      ssSetOutputPortUnit(rts, 2, 0);
      ssSetOutputPortIsContinuousQuantity(rts, 2, 0);
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 1, 1);
      _ssSetOutputPortConnected(rts, 2, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 2, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
    }

    /* Level2 S-Function Block: three_phase_circuit__1_sm_master/<S50>/S-Function (OP_SEND) */
    {
      SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, three_phase_circuit__1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods2
                           [1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods3
                           [1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods4
                           [1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [1]);
        ssSetPeriodicStatesInfo(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [1]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.UPtrs0;
          sfcnUPtrs[0] = &three_phase_circuit__1_sm_master_B.SFunction1_o3[0];
          sfcnUPtrs[1] = &three_phase_circuit__1_sm_master_B.SFunction1_o3[1];
          sfcnUPtrs[2] = &three_phase_circuit__1_sm_master_B.SFunction1_o3[2];
          sfcnUPtrs[3] = &three_phase_circuit__1_sm_master_B.Kv1[0];
          sfcnUPtrs[4] = &three_phase_circuit__1_sm_master_B.Kv1[1];
          sfcnUPtrs[5] = &three_phase_circuit__1_sm_master_B.Kv1[2];
          sfcnUPtrs[6] = &three_phase_circuit__1_sm_master_B.SFunction1_o3[0];
          sfcnUPtrs[7] = &three_phase_circuit__1_sm_master_B.SFunction1_o3[1];
          sfcnUPtrs[8] = &three_phase_circuit__1_sm_master_B.SFunction1_o3[2];
          sfcnUPtrs[9] = &three_phase_circuit__1_sm_master_B.Kv1_p[0];
          sfcnUPtrs[10] = &three_phase_circuit__1_sm_master_B.Kv1_p[1];
          sfcnUPtrs[11] = &three_phase_circuit__1_sm_master_B.Kv1_p[2];
          sfcnUPtrs[12] = &three_phase_circuit__1_sm_master_B.Kv[0];
          sfcnUPtrs[13] = &three_phase_circuit__1_sm_master_B.Kv[1];
          sfcnUPtrs[14] = &three_phase_circuit__1_sm_master_B.Kv[2];
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 15);
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function");
      ssSetPath(rts,
                "three_phase_circuit__1_sm_master/SM_master/rtlab_send_subsystem/Subsystem1/Send1/S-Function");
      ssSetRTModel(rts,three_phase_circuit__1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 1);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction_P1_Size);
      }

      /* work vectors */
      ssSetIWork(rts, (int_T *)
                 &three_phase_circuit__1_sm_master_DW.SFunction_IWORK[0]);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn1.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* IWORK */
        ssSetDWorkWidth(rts, 0, 5);
        ssSetDWorkDataType(rts, 0,SS_INTEGER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0, &three_phase_circuit__1_sm_master_DW.SFunction_IWORK
                   [0]);
      }

      /* registration */
      OP_SEND(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 7.5E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetInputPortWidth(rts, 0, 15);
      ssSetInputPortDataType(rts, 0, SS_DOUBLE);
      ssSetInputPortComplexSignal(rts, 0, 0);
      ssSetInputPortFrameData(rts, 0, 0);
      ssSetInputPortUnit(rts, 0, 0);
      ssSetInputPortIsContinuousQuantity(rts, 0, 0);
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
    }

    /* Level2 S-Function Block: three_phase_circuit__1_sm_master/<S8>/S-Function2 (sfun_send_async) */
    {
      SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[2];

      /* timing info */
      time_T *sfcnPeriod =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.sfcnPeriod;
      time_T *sfcnOffset =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.sfcnOffset;
      int_T *sfcnTsMap =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [2]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [2]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, three_phase_circuit__1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods2
                           [2]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods3
                           [2]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods4
                           [2]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [2]);
        ssSetPeriodicStatesInfo(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [2]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.UPtrs0;
          sfcnUPtrs[0] = &three_phase_circuit__1_sm_master_B.dataready25kHz;
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 1);
        }

        /* port 1 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.UPtrs1;
          sfcnUPtrs[0] = three_phase_circuit__1_sm_master_B.Kv1;
          sfcnUPtrs[1] = &three_phase_circuit__1_sm_master_B.Kv1[1];
          sfcnUPtrs[2] = &three_phase_circuit__1_sm_master_B.Kv1[2];
          ssSetInputPortSignalPtrs(rts, 1, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidth(rts, 1, 3);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 1);
        _ssSetPortInfo2ForOutputUnits(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            &three_phase_circuit__1_sm_master_B.SFunction2));
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function2");
      ssSetPath(rts,
                "three_phase_circuit__1_sm_master/SM_master/send message 1/S-Function2");
      ssSetRTModel(rts,three_phase_circuit__1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.params;
        ssSetSFcnParamsCount(rts, 13);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P12_Size);
        ssSetSFcnParam(rts, 12, (mxArray*)
                       three_phase_circuit__1_sm_master_P.SFunction2_P13_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &three_phase_circuit__1_sm_master_DW.SFunction2_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn2.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0, &three_phase_circuit__1_sm_master_DW.SFunction2_PWORK);
      }

      /* registration */
      sfun_send_async(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 7.5E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetInputPortWidth(rts, 1, 3);
      ssSetInputPortDataType(rts, 1, SS_DOUBLE);
      ssSetInputPortComplexSignal(rts, 1, 0);
      ssSetInputPortFrameData(rts, 1, 0);
      ssSetInputPortUnit(rts, 1, 0);
      ssSetInputPortIsContinuousQuantity(rts, 1, 0);
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetInputPortConnected(rts, 1, 1);
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
    }

    /* Level2 S-Function Block: three_phase_circuit__1_sm_master/<S2>/OpIPSocketCtrl (sfun_gen_async_ctrl) */
    {
      SimStruct *rts = three_phase_circuit__1_sm_master_M->childSfunctions[3];

      /* timing info */
      time_T *sfcnPeriod =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn3.sfcnPeriod;
      time_T *sfcnOffset =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn3.sfcnOffset;
      int_T *sfcnTsMap =
        three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn3.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [3]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [3]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, three_phase_circuit__1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods2
                           [3]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods3
                           [3]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.methods4
                           [3]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [3]);
        ssSetPeriodicStatesInfo(rts,
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [3]);
      }

      /* path info */
      ssSetModelName(rts, "OpIPSocketCtrl");
      ssSetPath(rts, "three_phase_circuit__1_sm_master/SM_master/OpIPSocketCtrl");
      ssSetRTModel(rts,three_phase_circuit__1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn3.params;
        ssSetSFcnParamsCount(rts, 27);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P12_Size);
        ssSetSFcnParam(rts, 12, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P13_Size);
        ssSetSFcnParam(rts, 13, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P14_Size);
        ssSetSFcnParam(rts, 14, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P15_Size);
        ssSetSFcnParam(rts, 15, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P16_Size);
        ssSetSFcnParam(rts, 16, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P17_Size);
        ssSetSFcnParam(rts, 17, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P18_Size);
        ssSetSFcnParam(rts, 18, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P19_Size);
        ssSetSFcnParam(rts, 19, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P20_Size);
        ssSetSFcnParam(rts, 20, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P21_Size);
        ssSetSFcnParam(rts, 21, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P22_Size);
        ssSetSFcnParam(rts, 22, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P23_Size);
        ssSetSFcnParam(rts, 23, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P24_Size);
        ssSetSFcnParam(rts, 24, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P25_Size);
        ssSetSFcnParam(rts, 25, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P26_Size);
        ssSetSFcnParam(rts, 26, (mxArray*)
                       three_phase_circuit__1_sm_master_P.OpIPSocketCtrl_P27_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &three_phase_circuit__1_sm_master_DW.OpIPSocketCtrl_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn3.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &three_phase_circuit__1_sm_master_M->NonInlinedSFcns.Sfcn3.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0,
                   &three_phase_circuit__1_sm_master_DW.OpIPSocketCtrl_PWORK);
      }

      /* registration */
      sfun_gen_async_ctrl(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 7.5E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 0;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      /* Update the BufferDstPort flags for each input port */
    }
  }

  /* Initialize Sizes */
  three_phase_circuit__1_sm_master_M->Sizes.numContStates = (0);/* Number of continuous states */
  three_phase_circuit__1_sm_master_M->Sizes.numY = (0);/* Number of model outputs */
  three_phase_circuit__1_sm_master_M->Sizes.numU = (0);/* Number of model inputs */
  three_phase_circuit__1_sm_master_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  three_phase_circuit__1_sm_master_M->Sizes.numSampTimes = (1);/* Number of sample times */
  three_phase_circuit__1_sm_master_M->Sizes.numBlocks = (125);/* Number of blocks */
  three_phase_circuit__1_sm_master_M->Sizes.numBlockIO = (32);/* Number of block outputs */
  three_phase_circuit__1_sm_master_M->Sizes.numBlockPrms = (450);/* Sum of parameter "widths" */
  return three_phase_circuit__1_sm_master_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
