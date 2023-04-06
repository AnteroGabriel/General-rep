/*
 * three_phase_circuit__1_sm_master_data.c
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

/* Block parameters (default storage) */
P_three_phase_circuit__1_sm_master_T three_phase_circuit__1_sm_master_P = {
  /* Mask Parameter: ThreePhaseSource_Frequency
   * Referenced by:
   *   '<S30>/Sine Wave A' (Parameter: Frequency)
   *   '<S30>/Sine Wave B' (Parameter: Frequency)
   *   '<S30>/Sine Wave C' (Parameter: Frequency)
   */
  60.0,

  /* Mask Parameter: ThreePhaseSource1_Frequency
   * Referenced by:
   *   '<S48>/Sine Wave A' (Parameter: Frequency)
   *   '<S48>/Sine Wave B' (Parameter: Frequency)
   *   '<S48>/Sine Wave C' (Parameter: Frequency)
   */
  60.0,

  /* Expression: 0
   * Referenced by: '<S1>/S-Function1' (Parameter: Value)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S1>/S-Function' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 2
   * Referenced by: '<S2>/timeout' (Parameter: Value)
   */
  2.0,

  /* Computed Parameter: SFunction1_P1_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: ctl_id
   * Referenced by: '<S5>/S-Function1' (Parameter: P1)
   */
  1.0,

  /* Computed Parameter: SFunction1_P2_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: recv_id
   * Referenced by: '<S5>/S-Function1' (Parameter: P2)
   */
  1.0,

  /* Computed Parameter: SFunction1_P3_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp1
   * Referenced by: '<S5>/S-Function1' (Parameter: P3)
   */
  1.0,

  /* Computed Parameter: SFunction1_P4_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp2
   * Referenced by: '<S5>/S-Function1' (Parameter: P4)
   */
  2.0,

  /* Computed Parameter: SFunction1_P5_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp3
   * Referenced by: '<S5>/S-Function1' (Parameter: P5)
   */
  3.0,

  /* Computed Parameter: SFunction1_P6_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P6Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp4
   * Referenced by: '<S5>/S-Function1' (Parameter: P6)
   */
  4.0,

  /* Computed Parameter: SFunction1_P7_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P7Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp5
   * Referenced by: '<S5>/S-Function1' (Parameter: P7)
   */
  5.0,

  /* Computed Parameter: SFunction1_P8_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P8Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P8
   * Referenced by: '<S5>/S-Function1' (Parameter: P8)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 49.0 },

  /* Computed Parameter: SFunction1_P9_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P9Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P9
   * Referenced by: '<S5>/S-Function1' (Parameter: P9)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 50.0 },

  /* Computed Parameter: SFunction1_P10_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P10Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P10
   * Referenced by: '<S5>/S-Function1' (Parameter: P10)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 51.0 },

  /* Computed Parameter: SFunction1_P11_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P11Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P11
   * Referenced by: '<S5>/S-Function1' (Parameter: P11)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 52.0 },

  /* Computed Parameter: SFunction1_P12_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P12Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P12
   * Referenced by: '<S5>/S-Function1' (Parameter: P12)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 53.0 },

  /* Computed Parameter: SFunction1_P13_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P13Size)
   */
  { 1.0, 1.0 },

  /* Computed Parameter: SFunction1_P13
   * Referenced by: '<S5>/S-Function1' (Parameter: P13)
   */
  54.0,

  /* Computed Parameter: SFunction1_P14_Size
   * Referenced by: '<S5>/S-Function1' (Parameter: P14Size)
   */
  { 1.0, 1.0 },

  /* Computed Parameter: SFunction1_P14
   * Referenced by: '<S5>/S-Function1' (Parameter: P14)
   */
  55.0,

  /* Expression: Aa
   * Referenced by: '<S30>/Sine Wave A' (Parameter: Amplitude)
   */
  179.62924780409975,

  /* Expression: 0
   * Referenced by: '<S30>/Sine Wave A' (Parameter: Bias)
   */
  0.0,

  /* Computed Parameter: SineWaveA_Hsin
   * Referenced by: '<S30>/Sine Wave A' (Parameter: SinH)
   */
  0.028270566770273248,

  /* Computed Parameter: SineWaveA_HCos
   * Referenced by: '<S30>/Sine Wave A' (Parameter: CosH)
   */
  0.99960030765025654,

  /* Computed Parameter: SineWaveA_PSin
   * Referenced by: '<S30>/Sine Wave A' (Parameter: SinPhi)
   */
  -0.028270566770273248,

  /* Computed Parameter: SineWaveA_PCos
   * Referenced by: '<S30>/Sine Wave A' (Parameter: CosPhi)
   */
  0.99960030765025654,

  /* Expression: Ab
   * Referenced by: '<S30>/Sine Wave B' (Parameter: Amplitude)
   */
  179.62924780409975,

  /* Expression: 0
   * Referenced by: '<S30>/Sine Wave B' (Parameter: Bias)
   */
  0.0,

  /* Computed Parameter: SineWaveB_Hsin
   * Referenced by: '<S30>/Sine Wave B' (Parameter: SinH)
   */
  0.028270566770273248,

  /* Computed Parameter: SineWaveB_HCos
   * Referenced by: '<S30>/Sine Wave B' (Parameter: CosH)
   */
  0.99960030765025654,

  /* Computed Parameter: SineWaveB_PSin
   * Referenced by: '<S30>/Sine Wave B' (Parameter: SinPhi)
   */
  -0.851543976670726,

  /* Computed Parameter: SineWaveB_PCos
   * Referenced by: '<S30>/Sine Wave B' (Parameter: CosPhi)
   */
  -0.52428318282756892,

  /* Expression: Ac
   * Referenced by: '<S30>/Sine Wave C' (Parameter: Amplitude)
   */
  179.62924780409975,

  /* Expression: 0
   * Referenced by: '<S30>/Sine Wave C' (Parameter: Bias)
   */
  0.0,

  /* Computed Parameter: SineWaveC_Hsin
   * Referenced by: '<S30>/Sine Wave C' (Parameter: SinH)
   */
  0.028270566770273248,

  /* Computed Parameter: SineWaveC_HCos
   * Referenced by: '<S30>/Sine Wave C' (Parameter: CosH)
   */
  0.99960030765025654,

  /* Computed Parameter: SineWaveC_PSin
   * Referenced by: '<S30>/Sine Wave C' (Parameter: SinPhi)
   */
  0.8798145434409993,

  /* Computed Parameter: SineWaveC_PCos
   * Referenced by: '<S30>/Sine Wave C' (Parameter: CosPhi)
   */
  -0.47531712482268723,

  /* Expression: S.D
   * Referenced by: '<S51>/State-Space' (Parameter: DS_param)
   */
  { -2.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -2.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -2.0,
    0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0, 0.0 },

  /* Expression: 1
   * Referenced by: '<S18>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S19>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S20>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: Kv
   * Referenced by: '<S9>/Kv1' (Parameter: Gain)
   */
  1.0,

  /* Expression: Aa
   * Referenced by: '<S48>/Sine Wave A' (Parameter: Amplitude)
   */
  179.62924780409975,

  /* Expression: 0
   * Referenced by: '<S48>/Sine Wave A' (Parameter: Bias)
   */
  0.0,

  /* Computed Parameter: SineWaveA_Hsin_l
   * Referenced by: '<S48>/Sine Wave A' (Parameter: SinH)
   */
  0.028270566770273248,

  /* Computed Parameter: SineWaveA_HCos_c
   * Referenced by: '<S48>/Sine Wave A' (Parameter: CosH)
   */
  0.99960030765025654,

  /* Computed Parameter: SineWaveA_PSin_f
   * Referenced by: '<S48>/Sine Wave A' (Parameter: SinPhi)
   */
  -0.028270566770273248,

  /* Computed Parameter: SineWaveA_PCos_a
   * Referenced by: '<S48>/Sine Wave A' (Parameter: CosPhi)
   */
  0.99960030765025654,

  /* Expression: Ab
   * Referenced by: '<S48>/Sine Wave B' (Parameter: Amplitude)
   */
  179.62924780409975,

  /* Expression: 0
   * Referenced by: '<S48>/Sine Wave B' (Parameter: Bias)
   */
  0.0,

  /* Computed Parameter: SineWaveB_Hsin_g
   * Referenced by: '<S48>/Sine Wave B' (Parameter: SinH)
   */
  0.028270566770273248,

  /* Computed Parameter: SineWaveB_HCos_l
   * Referenced by: '<S48>/Sine Wave B' (Parameter: CosH)
   */
  0.99960030765025654,

  /* Computed Parameter: SineWaveB_PSin_i
   * Referenced by: '<S48>/Sine Wave B' (Parameter: SinPhi)
   */
  -0.851543976670726,

  /* Computed Parameter: SineWaveB_PCos_f
   * Referenced by: '<S48>/Sine Wave B' (Parameter: CosPhi)
   */
  -0.52428318282756892,

  /* Expression: Ac
   * Referenced by: '<S48>/Sine Wave C' (Parameter: Amplitude)
   */
  179.62924780409975,

  /* Expression: 0
   * Referenced by: '<S48>/Sine Wave C' (Parameter: Bias)
   */
  0.0,

  /* Computed Parameter: SineWaveC_Hsin_e
   * Referenced by: '<S48>/Sine Wave C' (Parameter: SinH)
   */
  0.028270566770273248,

  /* Computed Parameter: SineWaveC_HCos_j
   * Referenced by: '<S48>/Sine Wave C' (Parameter: CosH)
   */
  0.99960030765025654,

  /* Computed Parameter: SineWaveC_PSin_b
   * Referenced by: '<S48>/Sine Wave C' (Parameter: SinPhi)
   */
  0.8798145434409993,

  /* Computed Parameter: SineWaveC_PCos_f
   * Referenced by: '<S48>/Sine Wave C' (Parameter: CosPhi)
   */
  -0.47531712482268723,

  /* Expression: S.A
   * Referenced by: '<S52>/State-Space' (Parameter: AS_param)
   */
  { -0.99726206662157546, 3.6287457415854746E-8, 0.0, 0.0, 0.0, 0.0,
    -0.030965296996410596, 0.9999995895985252, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    -0.99726206662157546, 3.6287457415854746E-8, 0.0, 0.0, 0.0, 0.0,
    -0.030965296996410596, 0.9999995895985252, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    -0.99726206662157546, 3.6287457415854746E-8, 0.0, 0.0, 0.0, 0.0,
    -0.030965296996410596, 0.9999995895985252 },

  /* Expression: S.B
   * Referenced by: '<S52>/State-Space' (Parameter: BS_param)
   */
  { 412.87062661880805, 0.0054720196626216171, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    412.87062661880805, 0.0054720196626216171, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    412.87062661880805, 0.0054720196626216171 },

  /* Expression: S.C
   * Referenced by: '<S52>/State-Space' (Parameter: CS_param)
   */
  { -2.0534500338184004E-7, 0.0, 0.0, 1.0267250169092002E-7, 0.0, 0.0,
    2.3223972747307947E-6, 0.0, 0.0, -1.1611986373653973E-6, 0.0, 0.0, 0.0,
    -2.0534500338184004E-7, 0.0, 0.0, 1.0267250169092002E-7, 0.0, 0.0,
    2.3223972747307947E-6, 0.0, 0.0, -1.1611986373653973E-6, 0.0, 0.0, 0.0,
    -2.0534500338184004E-7, 0.0, 0.0, 1.0267250169092002E-7, 0.0, 0.0,
    2.3223972747307947E-6, 0.0, 0.0, -1.1611986373653973E-6 },

  /* Expression: S.D
   * Referenced by: '<S52>/State-Space' (Parameter: DS_param)
   */
  { 0.9690347030035894, 0.0, 0.0, 0.0154826484982053, 0.0, 0.0, 0.0,
    0.9690347030035894, 0.0, 0.0, 0.0154826484982053, 0.0, 0.0, 0.0,
    0.9690347030035894, 0.0, 0.0, 0.0154826484982053 },

  /* Expression: S.x0
   * Referenced by: '<S52>/State-Space' (Parameter: X0_param)
   */
  { -525.13053446318031, -34.811867344227451, -31895.260305061936,
    17.83228653130432, 32420.390839528292, 16.979580812923114 },

  /* Expression: 1
   * Referenced by: '<S39>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S40>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S41>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: Kv
   * Referenced by: '<S31>/Kv1' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S36>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S37>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S38>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: Ki
   * Referenced by: '<S31>/Kv' (Parameter: Gain)
   */
  1.0,

  /* Computed Parameter: SFunction_P1_Size
   * Referenced by: '<S50>/S-Function' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: Acqu_group
   * Referenced by: '<S50>/S-Function' (Parameter: P1)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S15>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S16>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S17>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: Ki
   * Referenced by: '<S9>/Kv' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S2>/data ready 2.5 kHz' (Parameter: Amplitude)
   */
  1.0,

  /* Expression: 2
   * Referenced by: '<S2>/data ready 2.5 kHz' (Parameter: Period)
   */
  2.0,

  /* Expression: 2
   * Referenced by: '<S2>/data ready 2.5 kHz' (Parameter: PulseWidth)
   */
  2.0,

  /* Expression: 0
   * Referenced by: '<S2>/data ready 2.5 kHz' (Parameter: PhaseDelay)
   */
  0.0,

  /* Computed Parameter: SFunction2_P1_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: ctl_id
   * Referenced by: '<S8>/S-Function2' (Parameter: P1)
   */
  1.0,

  /* Computed Parameter: SFunction2_P2_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: send_id
   * Referenced by: '<S8>/S-Function2' (Parameter: P2)
   */
  1.0,

  /* Computed Parameter: SFunction2_P3_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: mode
   * Referenced by: '<S8>/S-Function2' (Parameter: P3)
   */
  3.0,

  /* Computed Parameter: SFunction2_P4_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp1
   * Referenced by: '<S8>/S-Function2' (Parameter: P4)
   */
  1.0,

  /* Computed Parameter: SFunction2_P5_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp2
   * Referenced by: '<S8>/S-Function2' (Parameter: P5)
   */
  2.0,

  /* Computed Parameter: SFunction2_P6_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P6Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp3
   * Referenced by: '<S8>/S-Function2' (Parameter: P6)
   */
  3.0,

  /* Computed Parameter: SFunction2_P7_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P7Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp4
   * Referenced by: '<S8>/S-Function2' (Parameter: P7)
   */
  4.0,

  /* Computed Parameter: SFunction2_P8_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P8Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp5
   * Referenced by: '<S8>/S-Function2' (Parameter: P8)
   */
  5.0,

  /* Computed Parameter: SFunction2_P9_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P9Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P9
   * Referenced by: '<S8>/S-Function2' (Parameter: P9)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 49.0 },

  /* Computed Parameter: SFunction2_P10_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P10Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P10
   * Referenced by: '<S8>/S-Function2' (Parameter: P10)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 50.0 },

  /* Computed Parameter: SFunction2_P11_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P11Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P11
   * Referenced by: '<S8>/S-Function2' (Parameter: P11)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 51.0 },

  /* Computed Parameter: SFunction2_P12_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P12Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P12
   * Referenced by: '<S8>/S-Function2' (Parameter: P12)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 52.0 },

  /* Computed Parameter: SFunction2_P13_Size
   * Referenced by: '<S8>/S-Function2' (Parameter: P13Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P13
   * Referenced by: '<S8>/S-Function2' (Parameter: P13)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 53.0 },

  /* Computed Parameter: OpIPSocketCtrl_P1_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: ctl_id
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P1)
   */
  1.0,

  /* Computed Parameter: OpIPSocketCtrl_P2_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: proto
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P2)
   */
  1.0,

  /* Computed Parameter: OpIPSocketCtrl_P3_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: ip_port_remote
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P3)
   */
  25000.0,

  /* Computed Parameter: OpIPSocketCtrl_P4_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: ip_port_local
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P4)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P5_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P5)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P6_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P6Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P6)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P7_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P7Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P7)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P8_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P8Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P8)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P9_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P9Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P9)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P10_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P10Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P10)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P11_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P11Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P11)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P12_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P12Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P12)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P13_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P13Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P13)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl_P14_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P14Size)
   */
  { 1.0, 13.0 },

  /* Computed Parameter: OpIPSocketCtrl_P14
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P14)
   */
  { 49.0, 57.0, 50.0, 46.0, 49.0, 54.0, 56.0, 46.0, 48.0, 46.0, 50.0, 51.0, 50.0
  },

  /* Computed Parameter: OpIPSocketCtrl_P15_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P15Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: OpIPSocketCtrl_P15
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P15)
   */
  { 48.0, 46.0, 48.0, 46.0, 48.0, 46.0, 48.0 },

  /* Computed Parameter: OpIPSocketCtrl_P16_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P16Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P17_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P17Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P18_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P18Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P19_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P19Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P20_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P20Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P21_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P21Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P22_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P22Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P23_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P23Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P24_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P24Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P25_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P25Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl_P26_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P26Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: OpIPSocketCtrl_P26
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P26)
   */
  { 65.0, 115.0, 121.0, 110.0, 99.0, 73.0, 80.0 },

  /* Computed Parameter: OpIPSocketCtrl_P27_Size
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P27Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P27)
   */
  0.0
};
