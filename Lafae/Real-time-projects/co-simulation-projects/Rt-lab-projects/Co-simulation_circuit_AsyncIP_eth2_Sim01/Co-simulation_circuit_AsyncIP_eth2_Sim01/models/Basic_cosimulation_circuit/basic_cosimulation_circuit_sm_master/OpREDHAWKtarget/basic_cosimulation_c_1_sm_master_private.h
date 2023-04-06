/*
 * basic_cosimulation_c_1_sm_master_private.h
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "basic_cosimulation_c_1_sm_master".
 *
 * Model version              : 13.7
 * Simulink Coder version : 9.6 (R2021b) 14-May-2021
 * C source code generated on : Wed Mar 22 14:38:59 2023
 *
 * Target selection: rtlab_rtmodel.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_basic_cosimulation_c_1_sm_master_private_h_
#define RTW_HEADER_basic_cosimulation_c_1_sm_master_private_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"
#include "zero_crossing_types.h"
#include "basic_cosimulation_c_1_sm_master.h"

extern real_T rt_hypotd_snf(real_T u0, real_T u1);
extern real_T rt_atan2d_snf(real_T u0, real_T u1);
real_T rt_TDelayInterpolate(
  real_T tMinusDelay,                 /* tMinusDelay = currentSimTime - delay */
  real_T tStart,
  real_T *uBuf,
  int_T bufSz,
  int_T *lastIdx,
  int_T oldestIdx,
  int_T newIdx,
  real_T initOutput,
  boolean_T discrete,
  boolean_T minorStepAndTAtLastMajorOutput)
  ;
extern int32_T div_s32(int32_T numerator, int32_T denominator);
extern void sfun_recv_async(SimStruct *rts);
extern void RECV_Param(SimStruct *rts);
extern void rte_svpwm(SimStruct *rts);
extern void rte_conversion(SimStruct *rts);
extern void ssn_sfun_v1(SimStruct *rts);
extern void OP_SEND(SimStruct *rts);
extern void opwritefile(SimStruct *rts);
extern void sfun_send_async(SimStruct *rts);
extern void sfun_gen_async_ctrl(SimStruct *rts);
extern void basic_cosimulation_c__Tail_Init(B_Tail_basic_cosimulation_c_1_T
  *localB, DW_Tail_basic_cosimulation_c__T *localDW,
  P_Tail_basic_cosimulation_c_1_T *localP);
extern void basic_cosimulation_c_Tail_Reset(DW_Tail_basic_cosimulation_c__T
  *localDW, P_Tail_basic_cosimulation_c_1_T *localP);
extern void basic_cosimulation_Tail_Disable(DW_Tail_basic_cosimulation_c__T
  *localDW);
extern void basic_cosimulation__Tail_Update(const real_T rtu_g[6],
  B_Tail_basic_cosimulation_c_1_T *localB, DW_Tail_basic_cosimulation_c__T
  *localDW, P_Tail_basic_cosimulation_c_1_T *localP);
extern void basic_cosimulation_c_1_sm__Tail
  (RT_MODEL_basic_cosimulation_c_1_sm_master_T * const
   basic_cosimulation_c_1_sm_master_M, real_T rtu_Enable, const real_T rtu_Iak[6],
   const real_T rtu_g[6], real_T rtp_Tf, real_T rtp_Tt,
   B_Tail_basic_cosimulation_c_1_T *localB, DW_Tail_basic_cosimulation_c__T
   *localDW, P_Tail_basic_cosimulation_c_1_T *localP);

/* private model entry point functions */
extern void basic_cosimulation_c_1_sm_master_derivatives(void);

#endif              /* RTW_HEADER_basic_cosimulation_c_1_sm_master_private_h_ */
