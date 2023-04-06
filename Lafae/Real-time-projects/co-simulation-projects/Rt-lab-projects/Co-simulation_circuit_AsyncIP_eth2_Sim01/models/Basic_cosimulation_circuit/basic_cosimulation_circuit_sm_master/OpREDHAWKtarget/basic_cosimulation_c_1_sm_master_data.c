/*
 * basic_cosimulation_c_1_sm_master_data.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "basic_cosimulation_c_1_sm_master".
 *
 * Model version              : 13.1
 * Simulink Coder version : 9.6 (R2021b) 14-May-2021
 * C source code generated on : Thu Mar 16 12:54:23 2023
 *
 * Target selection: rtlab_rtmodel.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "basic_cosimulation_c_1_sm_master.h"
#include "basic_cosimulation_c_1_sm_master_private.h"

/* Block parameters (default storage) */
P_basic_cosimulation_c_1_sm_master_T basic_cosimulation_c_1_sm_master_P = {
  /* Variable: Inom_peak
   * Referenced by:
   *   '<S45>/Saturation' (Parameter: UpperLimit)
   *   '<S45>/Saturation' (Parameter: LowerLimit)
   *   '<S45>/Saturation1' (Parameter: UpperLimit)
   *   '<S45>/Saturation1' (Parameter: LowerLimit)
   *   '<S124>/Saturation' (Parameter: UpperLimit)
   *   '<S124>/Saturation' (Parameter: LowerLimit)
   *   '<S124>/Saturation1' (Parameter: UpperLimit)
   *   '<S124>/Saturation1' (Parameter: LowerLimit)
   */
  481.12522432468819,

  /* Variable: Ts
   * Referenced by:
   *   '<S67>/delta' (Parameter: Gain)
   *   '<S67>/delta'' (Parameter: Gain)
   *   '<S68>/delta' (Parameter: Gain)
   *   '<S68>/delta'' (Parameter: Gain)
   *   '<S69>/delta' (Parameter: Gain)
   *   '<S69>/delta'' (Parameter: Gain)
   *   '<S70>/delta' (Parameter: Gain)
   *   '<S70>/delta'' (Parameter: Gain)
   *   '<S71>/delta' (Parameter: Gain)
   *   '<S71>/delta'' (Parameter: Gain)
   *   '<S72>/delta' (Parameter: Gain)
   *   '<S72>/delta'' (Parameter: Gain)
   */
  5.0E-5,

  /* Variable: Vdc
   * Referenced by:
   *   '<S45>/Constant2' (Parameter: Value)
   *   '<S86>/DC' (Parameter: Value)
   *   '<S124>/Constant2' (Parameter: Value)
   *   '<S152>/DC' (Parameter: Value)
   */
  400.0,

  /* Variable: Vnom_peak
   * Referenced by:
   *   '<S49>/Constant' (Parameter: Value)
   *   '<S51>/Gain' (Parameter: Gain)
   *   '<S128>/Constant' (Parameter: Value)
   *   '<S130>/Gain' (Parameter: Gain)
   */
  120.0,

  /* Variable: decouple
   * Referenced by:
   *   '<S45>/Gain5' (Parameter: Gain)
   *   '<S45>/Gain6' (Parameter: Gain)
   *   '<S124>/Gain5' (Parameter: Gain)
   *   '<S124>/Gain6' (Parameter: Gain)
   */
  0.36275987284684352,

  /* Variable: fsw_inv
   * Referenced by:
   *   '<S26>/Constant11' (Parameter: Value)
   *   '<S107>/Constant11' (Parameter: Value)
   */
  5760.0,

  /* Variable: ki_current
   * Referenced by:
   *   '<S45>/Gain2' (Parameter: Gain)
   *   '<S45>/Gain4' (Parameter: Gain)
   *   '<S124>/Gain2' (Parameter: Gain)
   *   '<S124>/Gain4' (Parameter: Gain)
   */
  1.0,

  /* Variable: kp_current
   * Referenced by:
   *   '<S45>/Gain1' (Parameter: Gain)
   *   '<S45>/Gain3' (Parameter: Gain)
   *   '<S124>/Gain1' (Parameter: Gain)
   *   '<S124>/Gain3' (Parameter: Gain)
   */
  0.96225044864937614,

  /* Mask Parameter: PowerMeasurementThreePhase_F
   * Referenced by:
   *   '<S56>/Gain' (Parameter: Gain)
   *   '<S57>/Gain' (Parameter: Gain)
   *   '<S58>/Gain' (Parameter: Gain)
   *   '<S59>/Gain' (Parameter: Gain)
   *   '<S60>/Gain' (Parameter: Gain)
   *   '<S61>/Gain' (Parameter: Gain)
   *   '<S64>/Radial frequencies' (Parameter: Value)
   *   '<S67>/delta' (Parameter: Gain)
   *   '<S67>/delta'' (Parameter: Gain)
   *   '<S68>/delta' (Parameter: Gain)
   *   '<S68>/delta'' (Parameter: Gain)
   *   '<S69>/delta' (Parameter: Gain)
   *   '<S69>/delta'' (Parameter: Gain)
   *   '<S70>/delta' (Parameter: Gain)
   *   '<S70>/delta'' (Parameter: Gain)
   *   '<S71>/delta' (Parameter: Gain)
   *   '<S71>/delta'' (Parameter: Gain)
   *   '<S72>/delta' (Parameter: Gain)
   *   '<S72>/delta'' (Parameter: Gain)
   */
  60.0,

  /* Mask Parameter: PowerMeasurementThreePhase_F_f
   * Referenced by:
   *   '<S135>/Gain' (Parameter: Gain)
   *   '<S135>/Transport Delay' (Parameter: DelayTime)
   *   '<S136>/Gain' (Parameter: Gain)
   *   '<S136>/Transport Delay' (Parameter: DelayTime)
   *   '<S137>/Gain' (Parameter: Gain)
   *   '<S137>/Transport Delay' (Parameter: DelayTime)
   *   '<S138>/Gain' (Parameter: Gain)
   *   '<S138>/Transport Delay' (Parameter: DelayTime)
   *   '<S139>/Gain' (Parameter: Gain)
   *   '<S139>/Transport Delay' (Parameter: DelayTime)
   *   '<S140>/Gain' (Parameter: Gain)
   *   '<S140>/Transport Delay' (Parameter: DelayTime)
   *   '<S143>/Radial frequencies' (Parameter: Value)
   */
  60.0,

  /* Mask Parameter: ThreePhaseSource_Frequency
   * Referenced by:
   *   '<S157>/Sine Wave A' (Parameter: Frequency)
   *   '<S157>/Sine Wave B' (Parameter: Frequency)
   *   '<S157>/Sine Wave C' (Parameter: Frequency)
   */
  60.0,

  /* Mask Parameter: PowerMeasurementThreePhase_K
   * Referenced by: '<S64>/Radial frequencies' (Parameter: Value)
   */
  1.0,

  /* Mask Parameter: PowerMeasurementThreePhase_K_j
   * Referenced by: '<S143>/Radial frequencies' (Parameter: Value)
   */
  { 0.0, 1.0, 2.0, 3.0 },

  /* Mask Parameter: Tail_Tf
   * Referenced by: '<S37>/Tail' (Parameter: System parameter #0)
   */
  0.0,

  /* Mask Parameter: Tail_Tf_a
   * Referenced by: '<S116>/Tail' (Parameter: System parameter #2)
   */
  0.0,

  /* Mask Parameter: Tail_Tt
   * Referenced by: '<S37>/Tail' (Parameter: System parameter #1)
   */
  0.0,

  /* Mask Parameter: Tail_Tt_m
   * Referenced by: '<S116>/Tail' (Parameter: System parameter #3)
   */
  0.0,

  /* Mask Parameter: IntegratorwithWrappedStateDiscr
   * Referenced by:
   *   '<S76>/Constant' (Parameter: Value)
   *   '<S78>/Bias' (Parameter: Bias)
   *   '<S78>/Gain' (Parameter: Gain)
   *   '<S78>/Gain1' (Parameter: Gain)
   */
  0.0,

  /* Mask Parameter: IntegratorwithWrappedStateDis_j
   * Referenced by:
   *   '<S77>/Constant' (Parameter: Value)
   *   '<S78>/Gain' (Parameter: Gain)
   *   '<S78>/Gain1' (Parameter: Gain)
   */
  6.2831853071795862,

  /* Mask Parameter: Phasor_to_Time_Domain_SFA_offse
   * Referenced by: '<S27>/Phase// Frequency Offset' (Parameter: Freq)
   */
  60.0,

  /* Mask Parameter: Time_Domain_To_Phasor_SFA_offse
   * Referenced by: '<S30>/Phase// Frequency Offset' (Parameter: Freq)
   */
  60.0,

  /* Mask Parameter: IntegratorwithWrappedStateDis_a
   * Referenced by: '<S75>/Initial' (Parameter: Value)
   */
  0.0,

  /* Mask Parameter: IntegratorwithWrappedStateDis_h
   * Referenced by: '<S147>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 1/3
   * Referenced by: '<S62>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 1/3
   * Referenced by: '<S63>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 3/2
   * Referenced by: '<S53>/Gain2' (Parameter: Gain)
   */
  1.5,

  /* Expression: 1/3
   * Referenced by: '<S54>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 1/3
   * Referenced by: '<S55>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 3/2
   * Referenced by: '<S53>/Gain1' (Parameter: Gain)
   */
  1.5,

  /* Expression: 1/3
   * Referenced by: '<S65>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 1/3
   * Referenced by: '<S66>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 3/2
   * Referenced by: '<S53>/Gain3' (Parameter: Gain)
   */
  1.5,

  /* Expression: 1/3
   * Referenced by: '<S141>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 1/3
   * Referenced by: '<S142>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 3/2
   * Referenced by: '<S132>/Gain2' (Parameter: Gain)
   */
  1.5,

  /* Expression: 1/3
   * Referenced by: '<S133>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 1/3
   * Referenced by: '<S134>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 3/2
   * Referenced by: '<S132>/Gain1' (Parameter: Gain)
   */
  1.5,

  /* Expression: 1/3
   * Referenced by: '<S144>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 1/3
   * Referenced by: '<S145>/Gain1' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 3/2
   * Referenced by: '<S132>/Gain3' (Parameter: Gain)
   */
  1.5,

  /* Expression: 0
   * Referenced by: '<S2>/S-Function1' (Parameter: Value)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S2>/S-Function' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 2
   * Referenced by: '<S3>/timeout' (Parameter: Value)
   */
  2.0,

  /* Computed Parameter: SFunction1_P1_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: ctl_id
   * Referenced by: '<S6>/S-Function1' (Parameter: P1)
   */
  1.0,

  /* Computed Parameter: SFunction1_P2_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: recv_id
   * Referenced by: '<S6>/S-Function1' (Parameter: P2)
   */
  1.0,

  /* Computed Parameter: SFunction1_P3_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp1
   * Referenced by: '<S6>/S-Function1' (Parameter: P3)
   */
  1.0,

  /* Computed Parameter: SFunction1_P4_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp2
   * Referenced by: '<S6>/S-Function1' (Parameter: P4)
   */
  2.0,

  /* Computed Parameter: SFunction1_P5_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp3
   * Referenced by: '<S6>/S-Function1' (Parameter: P5)
   */
  3.0,

  /* Computed Parameter: SFunction1_P6_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P6Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp4
   * Referenced by: '<S6>/S-Function1' (Parameter: P6)
   */
  4.0,

  /* Computed Parameter: SFunction1_P7_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P7Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp5
   * Referenced by: '<S6>/S-Function1' (Parameter: P7)
   */
  5.0,

  /* Computed Parameter: SFunction1_P8_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P8Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P8
   * Referenced by: '<S6>/S-Function1' (Parameter: P8)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 49.0 },

  /* Computed Parameter: SFunction1_P9_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P9Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P9
   * Referenced by: '<S6>/S-Function1' (Parameter: P9)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 50.0 },

  /* Computed Parameter: SFunction1_P10_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P10Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P10
   * Referenced by: '<S6>/S-Function1' (Parameter: P10)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 51.0 },

  /* Computed Parameter: SFunction1_P11_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P11Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P11
   * Referenced by: '<S6>/S-Function1' (Parameter: P11)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 52.0 },

  /* Computed Parameter: SFunction1_P12_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P12Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction1_P12
   * Referenced by: '<S6>/S-Function1' (Parameter: P12)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 53.0 },

  /* Computed Parameter: SFunction1_P13_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P13Size)
   */
  { 1.0, 1.0 },

  /* Computed Parameter: SFunction1_P13
   * Referenced by: '<S6>/S-Function1' (Parameter: P13)
   */
  54.0,

  /* Computed Parameter: SFunction1_P14_Size
   * Referenced by: '<S6>/S-Function1' (Parameter: P14Size)
   */
  { 1.0, 1.0 },

  /* Computed Parameter: SFunction1_P14
   * Referenced by: '<S6>/S-Function1' (Parameter: P14)
   */
  55.0,

  /* Expression: 0
   * Referenced by: '<S37>/Unit Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S45>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 2*pi
   * Referenced by: '<S51>/Constant2' (Parameter: Value)
   */
  6.2831853071795862,

  /* Expression: inf
   * Referenced by: '<S51>/Integrator1' (Parameter: UpperSaturationLimit)
   */
  0.0,

  /* Expression: -inf
   * Referenced by: '<S51>/Integrator1' (Parameter: LowerSaturationLimit)
   */
  0.0,

  /* Expression: -1
   * Referenced by: '<S52>/Gain' (Parameter: Gain)
   */
  -1.0,

  /* Expression: 2*pi/3
   * Referenced by: '<S52>/Constant3' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S52>/Constant1' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: -1
   * Referenced by: '<S52>/Gain1' (Parameter: Gain)
   */
  -1.0,

  /* Expression: 2*pi/3
   * Referenced by: '<S52>/Constant4' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S52>/Constant2' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: -1
   * Referenced by: '<S52>/Gain2' (Parameter: Gain)
   */
  -1.0,

  /* Computed Parameter: SFunction_P1_Size
   * Referenced by: '<S96>/S-Function' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: Data_width
   * Referenced by: '<S96>/S-Function' (Parameter: P1)
   */
  3.0,

  /* Computed Parameter: SFunction_P2_Size
   * Referenced by: '<S96>/S-Function' (Parameter: P2Size)
   */
  { 3.0, 1.0 },

  /* Expression: InitialConditions
   * Referenced by: '<S96>/S-Function' (Parameter: P2)
   */
  { 0.0, 0.0, 0.0 },

  /* Expression: 1.1
   * Referenced by: '<S17>/Saturation' (Parameter: UpperLimit)
   */
  1.1,

  /* Expression: -1.1
   * Referenced by: '<S17>/Saturation' (Parameter: LowerLimit)
   */
  -1.1,

  /* Expression: 1
   * Referenced by: '<S26>/Constant9' (Parameter: Value)
   */
  1.0,

  /* Expression: 2
   * Referenced by: '<S26>/Constant10' (Parameter: Value)
   */
  2.0,

  /* Computed Parameter: RTESPWM1_P1_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: MaxEvents
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P1)
   */
  4.0,

  /* Computed Parameter: RTESPWM1_P2_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: SampleTime
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P2)
   */
  5.0E-5,

  /* Computed Parameter: RTESPWM1_P3_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: MaxFreq
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P3)
   */
  0.0,

  /* Computed Parameter: RTESPWM1_P4_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: MinFreq
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P4)
   */
  0.0,

  /* Computed Parameter: RTESPWM1_P5_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: EnablingPort
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P5)
   */
  0.0,

  /* Computed Parameter: RTESPWM1_P6_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P6Size)
   */
  { 1.0, 1.0 },

  /* Expression: NumberPhases
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P6)
   */
  3.0,

  /* Computed Parameter: RTESPWM1_P7_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P7Size)
   */
  { 1.0, 1.0 },

  /* Expression: ComplementaryMode
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P7)
   */
  1.0,

  /* Computed Parameter: RTESPWM1_P8_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P8Size)
   */
  { 1.0, 1.0 },

  /* Expression: RiseTimeDelay
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P8)
   */
  0.0,

  /* Computed Parameter: RTESPWM1_P9_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P9Size)
   */
  { 1.0, 1.0 },

  /* Expression: CenterAlignmentMode
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P9)
   */
  1.0,

  /* Computed Parameter: RTESPWM1_P10_Size
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P10Size)
   */
  { 1.0, 1.0 },

  /* Expression: SpaceVector
   * Referenced by: '<S26>/RTE SPWM1' (Parameter: P10)
   */
  0.0,

  /* Computed Parameter: RTEConversion1_P1_Size
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: nbMaxEvents
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P1)
   */
  4.0,

  /* Computed Parameter: RTEConversion1_P2_Size
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: inputdatatype
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P2)
   */
  2.0,

  /* Computed Parameter: RTEConversion1_P3_Size
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: outputdatatype
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P3)
   */
  1.0,

  /* Computed Parameter: RTEConversion1_P4_Size
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: compensation
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P4)
   */
  2.0,

  /* Computed Parameter: RTEConversion1_P5_Size
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: sampleTime
   * Referenced by: '<S26>/RTE Conversion1' (Parameter: P5)
   */
  5.0E-5,

  /* Expression: 0
   * Referenced by: '<S37>/Not in ARTEMIS' (Parameter: Value)
   */
  0.0,

  /* Expression: pi/180
   * Referenced by: '<S27>/Gain' (Parameter: Gain)
   */
  0.017453292519943295,

  /* Computed Parameter: PhaseFrequencyOffset_Phase
   * Referenced by: '<S27>/Phase// Frequency Offset' (Parameter: Phase)
   */
  0.0,

  /* Expression: Vf_SwitchOn
   * Referenced by: '<S39>/Vf Devices & Clamping Diodes' (Parameter: Value)
   */
  { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001 },

  /* Expression: Vf_DiodeOn
   * Referenced by: '<S39>/Vf Diodes' (Parameter: Value)
   */
  { -0.001, -0.001, -0.001, -0.001, -0.001, -0.001 },

  /* Computed Parameter: StateSpace_P1_Size
   * Referenced by: '<S162>/State-Space' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S162>/State-Space' (Parameter: P1)
   */
  0.0,

  /* Computed Parameter: StateSpace_P2_Size
   * Referenced by: '<S162>/State-Space' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S162>/State-Space' (Parameter: P2)
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<S21>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S22>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S23>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Computed Parameter: PhaseFrequencyOffset_Phase_p
   * Referenced by: '<S30>/Phase// Frequency Offset' (Parameter: Phase)
   */
  0.0,

  /* Expression: 180/pi
   * Referenced by: '<S30>/Gain1' (Parameter: Gain)
   */
  57.295779513082323,

  /* Expression: 0.0
   * Referenced by: '<S5>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 2
   * Referenced by: '<S46>/Gain6' (Parameter: Gain)
   */
  2.0,

  /* Expression: 0.0
   * Referenced by: '<S5>/Delay1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 1/3
   * Referenced by: '<S46>/Gain7' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 1/3
   * Referenced by: '<S46>/Gain8' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 2
   * Referenced by: '<S46>/Gain10' (Parameter: Gain)
   */
  2.0,

  /* Expression: 1/3
   * Referenced by: '<S46>/Gain9' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 0.0
   * Referenced by: '<S8>/Delay2' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S8>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 2
   * Referenced by: '<S125>/Gain6' (Parameter: Gain)
   */
  2.0,

  /* Expression: 0.0
   * Referenced by: '<S8>/Delay1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 1/3
   * Referenced by: '<S125>/Gain7' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 1/3
   * Referenced by: '<S125>/Gain8' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 2
   * Referenced by: '<S125>/Gain10' (Parameter: Gain)
   */
  2.0,

  /* Expression: 1/3
   * Referenced by: '<S125>/Gain9' (Parameter: Gain)
   */
  0.33333333333333331,

  /* Expression: 0
   * Referenced by: '<S10>/In1' (Parameter: Value)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S9>/In1' (Parameter: Value)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S12>/In1' (Parameter: Value)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S11>/In1' (Parameter: Value)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S15>/In1' (Parameter: Value)
   */
  0.0,

  /* Computed Parameter: SFunction_P1_Size_j
   * Referenced by: '<S161>/S-Function' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: Acqu_group
   * Referenced by: '<S161>/S-Function' (Parameter: P1)
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S37>/0 4' (Parameter: Value)
   */
  0.0,

  /* Expression: 1./Ron
   * Referenced by: '<S37>/1//Ron' (Parameter: Gain)
   */
  1000.0,

  /* Expression: 0.5
   * Referenced by: '<S37>/Switch' (Parameter: Threshold)
   */
  0.5,

  /* Expression: inf
   * Referenced by: '<S37>/Saturation' (Parameter: UpperLimit)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S37>/Saturation' (Parameter: LowerLimit)
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<S34>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S35>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S36>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 0.0
   * Referenced by: '<S5>/Delay2' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S47>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Computed Parameter: DiscreteTimeIntegrator_gainval
   * Referenced by: '<S45>/Discrete-Time Integrator' (Parameter: gainval)
   */
  5.0E-5,

  /* Expression: 0
   * Referenced by: '<S45>/Discrete-Time Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Computed Parameter: DiscreteTimeIntegrator1_gainval
   * Referenced by: '<S45>/Discrete-Time Integrator1' (Parameter: gainval)
   */
  5.0E-5,

  /* Expression: 0
   * Referenced by: '<S45>/Discrete-Time Integrator1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S48>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 2
   * Referenced by: '<S49>/Gain1' (Parameter: Gain)
   */
  2.0,

  /* Expression: 0
   * Referenced by: '<S49>/Switch' (Parameter: Threshold)
   */
  0.0,

  /* Expression: 3
   * Referenced by: '<S49>/Gain' (Parameter: Gain)
   */
  3.0,

  /* Expression: -2
   * Referenced by: '<S49>/Gain3' (Parameter: Gain)
   */
  -2.0,

  /* Expression: 3
   * Referenced by: '<S49>/Gain2' (Parameter: Gain)
   */
  3.0,

  /* Expression: 2*pi/3
   * Referenced by: '<S47>/Constant' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S47>/Constant1' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S47>/Constant2' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S47>/Constant3' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 0.666
   * Referenced by: '<S47>/Gain' (Parameter: Gain)
   */
  0.666,

  /* Expression: -0.666
   * Referenced by: '<S47>/Gain1' (Parameter: Gain)
   */
  -0.666,

  /* Expression: 2*pi/3
   * Referenced by: '<S48>/Constant' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S48>/Constant1' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S48>/Constant2' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S48>/Constant3' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 0.666
   * Referenced by: '<S48>/Gain' (Parameter: Gain)
   */
  0.666,

  /* Expression: -0.666
   * Referenced by: '<S48>/Gain1' (Parameter: Gain)
   */
  -0.666,

  /* Expression: S
   * Referenced by: '<S53>/Sequence' (Parameter: Value)
   */
  1.0,

  /* Computed Parameter: Integrator_gainval
   * Referenced by: '<S75>/Integrator' (Parameter: gainval)
   */
  5.0E-5,

  /* Computed Parameter: Integrator_gainval_l
   * Referenced by: '<S57>/Integrator' (Parameter: gainval)
   */
  2.5E-5,

  /* Expression: 0
   * Referenced by: '<S57>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S68>/Unit Delay1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S68>/Unit Delay2' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S68>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.5
   * Referenced by: '<S68>/Gain' (Parameter: Gain)
   */
  0.5,

  /* Computed Parameter: Integrator_gainval_b
   * Referenced by: '<S59>/Integrator' (Parameter: gainval)
   */
  2.5E-5,

  /* Expression: 0
   * Referenced by: '<S59>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S70>/Unit Delay1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S70>/Unit Delay2' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S70>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.5
   * Referenced by: '<S70>/Gain' (Parameter: Gain)
   */
  0.5,

  /* Computed Parameter: Integrator_gainval_d
   * Referenced by: '<S61>/Integrator' (Parameter: gainval)
   */
  2.5E-5,

  /* Expression: 0
   * Referenced by: '<S61>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S72>/Unit Delay1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S72>/Unit Delay2' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S72>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.5
   * Referenced by: '<S72>/Gain' (Parameter: Gain)
   */
  0.5,

  /* Computed Parameter: Integrator_gainval_c
   * Referenced by: '<S56>/Integrator' (Parameter: gainval)
   */
  2.5E-5,

  /* Expression: 0
   * Referenced by: '<S56>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S67>/Unit Delay1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S67>/Unit Delay2' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S67>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.5
   * Referenced by: '<S67>/Gain' (Parameter: Gain)
   */
  0.5,

  /* Computed Parameter: Integrator_gainval_h
   * Referenced by: '<S58>/Integrator' (Parameter: gainval)
   */
  2.5E-5,

  /* Expression: 0
   * Referenced by: '<S58>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S69>/Unit Delay1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S69>/Unit Delay2' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S69>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.5
   * Referenced by: '<S69>/Gain' (Parameter: Gain)
   */
  0.5,

  /* Computed Parameter: Integrator_gainval_i
   * Referenced by: '<S60>/Integrator' (Parameter: gainval)
   */
  2.5E-5,

  /* Expression: 0
   * Referenced by: '<S60>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S71>/Unit Delay1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S71>/Unit Delay2' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S71>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.5
   * Referenced by: '<S71>/Gain' (Parameter: Gain)
   */
  0.5,

  /* Expression: 0.866
   * Referenced by: '<S51>/Gain5' (Parameter: Gain)
   */
  0.866,

  /* Expression: -0.866
   * Referenced by: '<S51>/Gain6' (Parameter: Gain)
   */
  -0.866,

  /* Expression: 0.666
   * Referenced by: '<S51>/Gain4' (Parameter: Gain)
   */
  0.666,

  /* Expression: -0.5
   * Referenced by: '<S51>/Gain2' (Parameter: Gain)
   */
  -0.5,

  /* Expression: -0.5
   * Referenced by: '<S51>/Gain3' (Parameter: Gain)
   */
  -0.5,

  /* Expression: 0.666
   * Referenced by: '<S51>/Gain1' (Parameter: Gain)
   */
  0.666,

  /* Expression: 2*pi*60
   * Referenced by: '<S51>/Constant' (Parameter: Value)
   */
  376.99111843077515,

  /* Expression: 92
   * Referenced by: '<S51>/Gain7' (Parameter: Gain)
   */
  92.0,

  /* Expression: 0
   * Referenced by: '<S51>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

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

  /* Expression: 1
   * Referenced by: '<S31>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S32>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S24>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S116>/0 4' (Parameter: Value)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S116>/Unit Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S124>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 2*pi
   * Referenced by: '<S130>/Constant2' (Parameter: Value)
   */
  6.2831853071795862,

  /* Expression: inf
   * Referenced by: '<S130>/Integrator2' (Parameter: UpperSaturationLimit)
   */
  0.0,

  /* Expression: -inf
   * Referenced by: '<S130>/Integrator2' (Parameter: LowerSaturationLimit)
   */
  0.0,

  /* Expression: -1
   * Referenced by: '<S131>/Gain' (Parameter: Gain)
   */
  -1.0,

  /* Expression: 2*pi/3
   * Referenced by: '<S131>/Constant3' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S131>/Constant1' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: -1
   * Referenced by: '<S131>/Gain1' (Parameter: Gain)
   */
  -1.0,

  /* Expression: 2*pi/3
   * Referenced by: '<S131>/Constant4' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S131>/Constant2' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: -1
   * Referenced by: '<S131>/Gain2' (Parameter: Gain)
   */
  -1.0,

  /* Expression: 1.1
   * Referenced by: '<S101>/Saturation' (Parameter: UpperLimit)
   */
  1.1,

  /* Expression: -1.1
   * Referenced by: '<S101>/Saturation' (Parameter: LowerLimit)
   */
  -1.1,

  /* Expression: 1
   * Referenced by: '<S107>/Constant9' (Parameter: Value)
   */
  1.0,

  /* Expression: 2
   * Referenced by: '<S107>/Constant10' (Parameter: Value)
   */
  2.0,

  /* Computed Parameter: RTESPWM1_P1_Size_f
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: MaxEvents
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P1)
   */
  4.0,

  /* Computed Parameter: RTESPWM1_P2_Size_n
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: SampleTime
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P2)
   */
  5.0E-5,

  /* Computed Parameter: RTESPWM1_P3_Size_e
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: MaxFreq
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P3)
   */
  0.0,

  /* Computed Parameter: RTESPWM1_P4_Size_a
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: MinFreq
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P4)
   */
  0.0,

  /* Computed Parameter: RTESPWM1_P5_Size_f
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: EnablingPort
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P5)
   */
  0.0,

  /* Computed Parameter: RTESPWM1_P6_Size_a
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P6Size)
   */
  { 1.0, 1.0 },

  /* Expression: NumberPhases
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P6)
   */
  3.0,

  /* Computed Parameter: RTESPWM1_P7_Size_l
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P7Size)
   */
  { 1.0, 1.0 },

  /* Expression: ComplementaryMode
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P7)
   */
  1.0,

  /* Computed Parameter: RTESPWM1_P8_Size_k
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P8Size)
   */
  { 1.0, 1.0 },

  /* Expression: RiseTimeDelay
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P8)
   */
  0.0,

  /* Computed Parameter: RTESPWM1_P9_Size_k
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P9Size)
   */
  { 1.0, 1.0 },

  /* Expression: CenterAlignmentMode
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P9)
   */
  1.0,

  /* Computed Parameter: RTESPWM1_P10_Size_b
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P10Size)
   */
  { 1.0, 1.0 },

  /* Expression: SpaceVector
   * Referenced by: '<S107>/RTE SPWM1' (Parameter: P10)
   */
  0.0,

  /* Computed Parameter: RTEConversion1_P1_Size_a
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: nbMaxEvents
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P1)
   */
  4.0,

  /* Computed Parameter: RTEConversion1_P2_Size_g
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: inputdatatype
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P2)
   */
  2.0,

  /* Computed Parameter: RTEConversion1_P3_Size_h
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: outputdatatype
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P3)
   */
  1.0,

  /* Computed Parameter: RTEConversion1_P4_Size_j
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: compensation
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P4)
   */
  2.0,

  /* Computed Parameter: RTEConversion1_P5_Size_c
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: sampleTime
   * Referenced by: '<S107>/RTE Conversion1' (Parameter: P5)
   */
  5.0E-5,

  /* Expression: 0
   * Referenced by: '<S116>/Not in ARTEMIS' (Parameter: Value)
   */
  0.0,

  /* Expression: Aa
   * Referenced by: '<S157>/Sine Wave A' (Parameter: Amplitude)
   */
  359.2584956081995,

  /* Expression: 0
   * Referenced by: '<S157>/Sine Wave A' (Parameter: Bias)
   */
  0.0,

  /* Computed Parameter: SineWaveA_Hsin
   * Referenced by: '<S157>/Sine Wave A' (Parameter: SinH)
   */
  0.018848439715408175,

  /* Computed Parameter: SineWaveA_HCos
   * Referenced by: '<S157>/Sine Wave A' (Parameter: CosH)
   */
  0.999822352380809,

  /* Computed Parameter: SineWaveA_PSin
   * Referenced by: '<S157>/Sine Wave A' (Parameter: SinPhi)
   */
  -0.018848439715408175,

  /* Computed Parameter: SineWaveA_PCos
   * Referenced by: '<S157>/Sine Wave A' (Parameter: CosPhi)
   */
  0.999822352380809,

  /* Expression: Ab
   * Referenced by: '<S157>/Sine Wave B' (Parameter: Amplitude)
   */
  359.2584956081995,

  /* Expression: 0
   * Referenced by: '<S157>/Sine Wave B' (Parameter: Bias)
   */
  0.0,

  /* Computed Parameter: SineWaveB_Hsin
   * Referenced by: '<S157>/Sine Wave B' (Parameter: SinH)
   */
  0.018848439715408175,

  /* Computed Parameter: SineWaveB_HCos
   * Referenced by: '<S157>/Sine Wave B' (Parameter: CosH)
   */
  0.999822352380809,

  /* Computed Parameter: SineWaveB_PSin
   * Referenced by: '<S157>/Sine Wave B' (Parameter: SinPhi)
   */
  -0.85644733657559335,

  /* Computed Parameter: SineWaveB_PCos
   * Referenced by: '<S157>/Sine Wave B' (Parameter: CosPhi)
   */
  -0.51623440380564745,

  /* Expression: Ac
   * Referenced by: '<S157>/Sine Wave C' (Parameter: Amplitude)
   */
  359.2584956081995,

  /* Expression: 0
   * Referenced by: '<S157>/Sine Wave C' (Parameter: Bias)
   */
  0.0,

  /* Computed Parameter: SineWaveC_Hsin
   * Referenced by: '<S157>/Sine Wave C' (Parameter: SinH)
   */
  0.018848439715408175,

  /* Computed Parameter: SineWaveC_HCos
   * Referenced by: '<S157>/Sine Wave C' (Parameter: CosH)
   */
  0.999822352380809,

  /* Computed Parameter: SineWaveC_PSin
   * Referenced by: '<S157>/Sine Wave C' (Parameter: SinPhi)
   */
  0.87529577629100175,

  /* Computed Parameter: SineWaveC_PCos
   * Referenced by: '<S157>/Sine Wave C' (Parameter: CosPhi)
   */
  -0.48358794857516113,

  /* Expression: Vf_SwitchOn
   * Referenced by: '<S118>/Vf Devices & Clamping Diodes' (Parameter: Value)
   */
  { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001 },

  /* Expression: Vf_DiodeOn
   * Referenced by: '<S118>/Vf Diodes' (Parameter: Value)
   */
  { -0.001, -0.001, -0.001, -0.001, -0.001, -0.001 },

  /* Computed Parameter: StateSpace_P1_Size_n
   * Referenced by: '<S163>/State-Space' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S163>/State-Space' (Parameter: P1)
   */
  0.0,

  /* Computed Parameter: StateSpace_P2_Size_h
   * Referenced by: '<S163>/State-Space' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S163>/State-Space' (Parameter: P2)
   */
  0.0,

  /* Expression: 1./Ron
   * Referenced by: '<S116>/1//Ron' (Parameter: Gain)
   */
  1000.0,

  /* Expression: 0.5
   * Referenced by: '<S116>/Switch' (Parameter: Threshold)
   */
  0.5,

  /* Expression: inf
   * Referenced by: '<S116>/Saturation' (Parameter: UpperLimit)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S116>/Saturation' (Parameter: LowerLimit)
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<S113>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S114>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S115>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 0.0
   * Referenced by: '<S126>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.0
   * Referenced by: '<S127>/Delay' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: S
   * Referenced by: '<S132>/Sequence' (Parameter: Value)
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S136>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S136>/Transport Delay' (Parameter: InitialOutput)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S138>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S138>/Transport Delay' (Parameter: InitialOutput)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S140>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S140>/Transport Delay' (Parameter: InitialOutput)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S135>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S135>/Transport Delay' (Parameter: InitialOutput)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S137>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S137>/Transport Delay' (Parameter: InitialOutput)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S139>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S139>/Transport Delay' (Parameter: InitialOutput)
   */
  0.0,

  /* Computed Parameter: DiscreteTimeIntegrator_gainva_m
   * Referenced by: '<S124>/Discrete-Time Integrator' (Parameter: gainval)
   */
  5.0E-5,

  /* Expression: 0
   * Referenced by: '<S124>/Discrete-Time Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Computed Parameter: DiscreteTimeIntegrator1_gainv_i
   * Referenced by: '<S124>/Discrete-Time Integrator1' (Parameter: gainval)
   */
  5.0E-5,

  /* Expression: 0
   * Referenced by: '<S124>/Discrete-Time Integrator1' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 2
   * Referenced by: '<S128>/Gain1' (Parameter: Gain)
   */
  2.0,

  /* Expression: 0
   * Referenced by: '<S128>/Switch' (Parameter: Threshold)
   */
  0.0,

  /* Expression: 3
   * Referenced by: '<S128>/Gain' (Parameter: Gain)
   */
  3.0,

  /* Expression: -2
   * Referenced by: '<S128>/Gain3' (Parameter: Gain)
   */
  -2.0,

  /* Expression: 3
   * Referenced by: '<S128>/Gain2' (Parameter: Gain)
   */
  3.0,

  /* Expression: 2*pi/3
   * Referenced by: '<S126>/Constant' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S126>/Constant1' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S126>/Constant2' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S126>/Constant3' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 0.666
   * Referenced by: '<S126>/Gain' (Parameter: Gain)
   */
  0.666,

  /* Expression: -0.666
   * Referenced by: '<S126>/Gain1' (Parameter: Gain)
   */
  -0.666,

  /* Expression: 2*pi/3
   * Referenced by: '<S127>/Constant' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S127>/Constant1' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S127>/Constant2' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 2*pi/3
   * Referenced by: '<S127>/Constant3' (Parameter: Value)
   */
  2.0943951023931953,

  /* Expression: 0.666
   * Referenced by: '<S127>/Gain' (Parameter: Gain)
   */
  0.666,

  /* Expression: -0.666
   * Referenced by: '<S127>/Gain1' (Parameter: Gain)
   */
  -0.666,

  /* Expression: WrappedStateUpperValue
   * Referenced by: '<S147>/Integrator' (Parameter: WrappedStateUpperValue)
   */
  6.2831853071795862,

  /* Expression: WrappedStateLowerValue
   * Referenced by: '<S147>/Integrator' (Parameter: WrappedStateLowerValue)
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S130>/Integrator' (Parameter: InitialCondition)
   */
  0.0,

  /* Expression: 0.866
   * Referenced by: '<S130>/Gain5' (Parameter: Gain)
   */
  0.866,

  /* Expression: -0.866
   * Referenced by: '<S130>/Gain6' (Parameter: Gain)
   */
  -0.866,

  /* Expression: 0.666
   * Referenced by: '<S130>/Gain4' (Parameter: Gain)
   */
  0.666,

  /* Expression: -0.5
   * Referenced by: '<S130>/Gain2' (Parameter: Gain)
   */
  -0.5,

  /* Expression: -0.5
   * Referenced by: '<S130>/Gain3' (Parameter: Gain)
   */
  -0.5,

  /* Expression: 0.666
   * Referenced by: '<S130>/Gain1' (Parameter: Gain)
   */
  0.666,

  /* Expression: 92
   * Referenced by: '<S130>/Gain7' (Parameter: Gain)
   */
  92.0,

  /* Expression: 2*pi*60
   * Referenced by: '<S130>/Constant' (Parameter: Value)
   */
  376.99111843077515,

  /* Expression: 1
   * Referenced by: '<S102>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S103>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S104>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S110>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S111>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S105>/do not delete this gain' (Parameter: Gain)
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<S3>/data ready 2.5 kHz' (Parameter: Amplitude)
   */
  1.0,

  /* Expression: 2
   * Referenced by: '<S3>/data ready 2.5 kHz' (Parameter: Period)
   */
  2.0,

  /* Expression: 2
   * Referenced by: '<S3>/data ready 2.5 kHz' (Parameter: PulseWidth)
   */
  2.0,

  /* Expression: 0
   * Referenced by: '<S3>/data ready 2.5 kHz' (Parameter: PhaseDelay)
   */
  0.0,

  /* Computed Parameter: SFunction2_P1_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: ctl_id
   * Referenced by: '<S14>/S-Function2' (Parameter: P1)
   */
  1.0,

  /* Computed Parameter: SFunction2_P2_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: send_id
   * Referenced by: '<S14>/S-Function2' (Parameter: P2)
   */
  1.0,

  /* Computed Parameter: SFunction2_P3_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: mode
   * Referenced by: '<S14>/S-Function2' (Parameter: P3)
   */
  3.0,

  /* Computed Parameter: SFunction2_P4_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp1
   * Referenced by: '<S14>/S-Function2' (Parameter: P4)
   */
  1.0,

  /* Computed Parameter: SFunction2_P5_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp2
   * Referenced by: '<S14>/S-Function2' (Parameter: P5)
   */
  2.0,

  /* Computed Parameter: SFunction2_P6_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P6Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp3
   * Referenced by: '<S14>/S-Function2' (Parameter: P6)
   */
  3.0,

  /* Computed Parameter: SFunction2_P7_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P7Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp4
   * Referenced by: '<S14>/S-Function2' (Parameter: P7)
   */
  4.0,

  /* Computed Parameter: SFunction2_P8_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P8Size)
   */
  { 1.0, 1.0 },

  /* Expression: fp5
   * Referenced by: '<S14>/S-Function2' (Parameter: P8)
   */
  5.0,

  /* Computed Parameter: SFunction2_P9_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P9Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P9
   * Referenced by: '<S14>/S-Function2' (Parameter: P9)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 49.0 },

  /* Computed Parameter: SFunction2_P10_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P10Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P10
   * Referenced by: '<S14>/S-Function2' (Parameter: P10)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 50.0 },

  /* Computed Parameter: SFunction2_P11_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P11Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P11
   * Referenced by: '<S14>/S-Function2' (Parameter: P11)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 51.0 },

  /* Computed Parameter: SFunction2_P12_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P12Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P12
   * Referenced by: '<S14>/S-Function2' (Parameter: P12)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 52.0 },

  /* Computed Parameter: SFunction2_P13_Size
   * Referenced by: '<S14>/S-Function2' (Parameter: P13Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: SFunction2_P13
   * Referenced by: '<S14>/S-Function2' (Parameter: P13)
   */
  { 115.0, 116.0, 114.0, 105.0, 110.0, 103.0, 53.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P1_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P1Size)
   */
  { 1.0, 1.0 },

  /* Expression: ctl_id
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P1)
   */
  1.0,

  /* Computed Parameter: OpIPSocketCtrl1_P2_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P2Size)
   */
  { 1.0, 1.0 },

  /* Expression: proto
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P2)
   */
  1.0,

  /* Computed Parameter: OpIPSocketCtrl1_P3_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P3Size)
   */
  { 1.0, 1.0 },

  /* Expression: ip_port_remote
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P3)
   */
  23002.0,

  /* Computed Parameter: OpIPSocketCtrl1_P4_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P4Size)
   */
  { 1.0, 1.0 },

  /* Expression: ip_port_local
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P4)
   */
  23001.0,

  /* Computed Parameter: OpIPSocketCtrl1_P5_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P5Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P5)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl1_P6_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P6Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P6)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl1_P7_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P7Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P7)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl1_P8_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P8Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P8)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl1_P9_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P9Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P9)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl1_P10_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P10Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P10)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl1_P11_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P11Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P11)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl1_P12_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P12Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P12)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl1_P13_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P13Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P13)
   */
  0.0,

  /* Computed Parameter: OpIPSocketCtrl1_P14_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P14Size)
   */
  { 1.0, 13.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P14
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P14)
   */
  { 49.0, 57.0, 50.0, 46.0, 49.0, 54.0, 56.0, 46.0, 49.0, 48.0, 46.0, 49.0, 54.0
  },

  /* Computed Parameter: OpIPSocketCtrl1_P15_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P15Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P15
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P15)
   */
  { 48.0, 46.0, 48.0, 46.0, 48.0, 46.0, 48.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P16_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P16Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P17_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P17Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P18_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P18Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P19_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P19Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P20_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P20Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P21_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P21Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P22_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P22Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P23_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P23Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P24_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P24Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P25_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P25Size)
   */
  { 0.0, 0.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P26_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P26Size)
   */
  { 1.0, 7.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P26
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P26)
   */
  { 65.0, 115.0, 121.0, 110.0, 99.0, 73.0, 80.0 },

  /* Computed Parameter: OpIPSocketCtrl1_P27_Size
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P27Size)
   */
  { 1.0, 1.0 },

  /* Expression: 0
   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P27)
   */
  0.0,

  /* Expression: -0.5*(1+1j*sqrt(3))
   * Referenced by: '<S62>/alpha2' (Parameter: Gain)
   */
  { -0.5, -0.8660254037844386 },

  /* Expression: -0.5*(1-1j*sqrt(3))
   * Referenced by: '<S62>/alpha' (Parameter: Gain)
   */
  { -0.5, 0.8660254037844386 },

  /* Expression: -0.5*(1+1j*sqrt(3))
   * Referenced by: '<S63>/alpha2' (Parameter: Gain)
   */
  { -0.5, -0.8660254037844386 },

  /* Expression: -0.5*(1-1j*sqrt(3))
   * Referenced by: '<S63>/alpha' (Parameter: Gain)
   */
  { -0.5, 0.8660254037844386 },

  /* Expression: -0.5*(1-1j*sqrt(3))
   * Referenced by: '<S54>/alpha' (Parameter: Gain)
   */
  { -0.5, 0.8660254037844386 },

  /* Expression: -0.5*(1+1j*sqrt(3))
   * Referenced by: '<S54>/alpha2' (Parameter: Gain)
   */
  { -0.5, -0.8660254037844386 },

  /* Expression: -0.5*(1-1j*sqrt(3))
   * Referenced by: '<S55>/alpha' (Parameter: Gain)
   */
  { -0.5, 0.8660254037844386 },

  /* Expression: -0.5*(1+1j*sqrt(3))
   * Referenced by: '<S55>/alpha2' (Parameter: Gain)
   */
  { -0.5, -0.8660254037844386 },

  /* Expression: -0.5*(1+1j*sqrt(3))
   * Referenced by: '<S141>/alpha2' (Parameter: Gain)
   */
  { -0.5, -0.8660254037844386 },

  /* Expression: -0.5*(1-1j*sqrt(3))
   * Referenced by: '<S141>/alpha' (Parameter: Gain)
   */
  { -0.5, 0.8660254037844386 },

  /* Expression: -0.5*(1+1j*sqrt(3))
   * Referenced by: '<S142>/alpha2' (Parameter: Gain)
   */
  { -0.5, -0.8660254037844386 },

  /* Expression: -0.5*(1-1j*sqrt(3))
   * Referenced by: '<S142>/alpha' (Parameter: Gain)
   */
  { -0.5, 0.8660254037844386 },

  /* Expression: -0.5*(1-1j*sqrt(3))
   * Referenced by: '<S133>/alpha' (Parameter: Gain)
   */
  { -0.5, 0.8660254037844386 },

  /* Expression: -0.5*(1+1j*sqrt(3))
   * Referenced by: '<S133>/alpha2' (Parameter: Gain)
   */
  { -0.5, -0.8660254037844386 },

  /* Expression: -0.5*(1-1j*sqrt(3))
   * Referenced by: '<S134>/alpha' (Parameter: Gain)
   */
  { -0.5, 0.8660254037844386 },

  /* Expression: -0.5*(1+1j*sqrt(3))
   * Referenced by: '<S134>/alpha2' (Parameter: Gain)
   */
  { -0.5, -0.8660254037844386 },

  /* Expression: j*(1/sqrt(3))*[0 1 -1; -1 0 1; 1 -1 0]
   * Referenced by: '<S30>/Gain' (Parameter: Gain)
   */
  { { 0.0, 0.0 }, { -0.0, -0.57735026918962584 }, { 0.0, 0.57735026918962584 },
      { 0.0, 0.57735026918962584 }, { 0.0, 0.0 }, { -0.0, -0.57735026918962584 },
    { -0.0, -0.57735026918962584 }, { 0.0, 0.57735026918962584 }, { 0.0, 0.0 } },

  /* Start of '<S116>/Tail' */
  {
    /* Expression: 0
     * Referenced by: '<S117>/itail' (Parameter: InitialOutput)
     */
    0.0,

    /* Expression: 1
     * Referenced by: '<S117>/1' (Parameter: Value)
     */
    1.0,

    /* Computed Parameter: u_Value_f
     * Referenced by: '<S117>/2' (Parameter: Value)
     */
    0.0,

    /* Computed Parameter: DiscreteTimeIntegrator_gainval
     * Referenced by: '<S117>/Discrete-Time Integrator' (Parameter: gainval)
     */
    5.0E-5,

    /* Expression: 0
     * Referenced by: '<S117>/Discrete-Time Integrator' (Parameter: InitialCondition)
     */
    0.0,

    /* Expression: 0.9
     * Referenced by: '<S120>/Constant' (Parameter: Value)
     */
    0.9,

    /* Expression: 0.9
     * Referenced by: '<S120>/Saturation1' (Parameter: UpperLimit)
     */
    0.9,

    /* Expression: 0
     * Referenced by: '<S120>/Saturation1' (Parameter: LowerLimit)
     */
    0.0,

    /* Expression: 0.1
     * Referenced by: '<S120>/Saturation2' (Parameter: UpperLimit)
     */
    0.1,

    /* Expression: 0
     * Referenced by: '<S120>/Saturation2' (Parameter: LowerLimit)
     */
    0.0,

    /* Expression: 0
     * Referenced by: '<S117>/Unit Delay' (Parameter: InitialCondition)
     */
    0.0,

    /* Expression: 0.5
     * Referenced by: '<S117>/Switch' (Parameter: Threshold)
     */
    0.5
  }
  ,

  /* End of '<S116>/Tail' */

  /* Start of '<S37>/Tail' */
  {
    /* Expression: 0
     * Referenced by: '<S38>/itail' (Parameter: InitialOutput)
     */
    0.0,

    /* Expression: 1
     * Referenced by: '<S38>/1' (Parameter: Value)
     */
    1.0,

    /* Computed Parameter: u_Value_f
     * Referenced by: '<S38>/2' (Parameter: Value)
     */
    0.0,

    /* Computed Parameter: DiscreteTimeIntegrator_gainval
     * Referenced by: '<S38>/Discrete-Time Integrator' (Parameter: gainval)
     */
    5.0E-5,

    /* Expression: 0
     * Referenced by: '<S38>/Discrete-Time Integrator' (Parameter: InitialCondition)
     */
    0.0,

    /* Expression: 0.9
     * Referenced by: '<S41>/Constant' (Parameter: Value)
     */
    0.9,

    /* Expression: 0.9
     * Referenced by: '<S41>/Saturation1' (Parameter: UpperLimit)
     */
    0.9,

    /* Expression: 0
     * Referenced by: '<S41>/Saturation1' (Parameter: LowerLimit)
     */
    0.0,

    /* Expression: 0.1
     * Referenced by: '<S41>/Saturation2' (Parameter: UpperLimit)
     */
    0.1,

    /* Expression: 0
     * Referenced by: '<S41>/Saturation2' (Parameter: LowerLimit)
     */
    0.0,

    /* Expression: 0
     * Referenced by: '<S38>/Unit Delay' (Parameter: InitialCondition)
     */
    0.0,

    /* Expression: 0.5
     * Referenced by: '<S38>/Switch' (Parameter: Threshold)
     */
    0.5
  }
  /* End of '<S37>/Tail' */
};
