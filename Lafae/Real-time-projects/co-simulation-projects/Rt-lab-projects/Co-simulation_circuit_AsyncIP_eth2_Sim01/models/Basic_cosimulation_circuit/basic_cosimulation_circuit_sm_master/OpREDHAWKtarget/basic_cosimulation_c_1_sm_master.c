/*
 * basic_cosimulation_c_1_sm_master.c
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

/* Block signals (default storage) */
B_basic_cosimulation_c_1_sm_master_T basic_cosimulation_c_1_sm_master_B;

/* Continuous states */
X_basic_cosimulation_c_1_sm_master_T basic_cosimulation_c_1_sm_master_X;

/* Block states (default storage) */
DW_basic_cosimulation_c_1_sm_master_T basic_cosimulation_c_1_sm_master_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_basic_cosimulation_c_1_sm_master_T
  basic_cosimulation_c_1_sm_master_PrevZCX;

/* Periodic continuous states */
PeriodicIndX_basic_cosimulation_c_1_sm_master_T
  basic_cosimulation_c_1_sm_master_PeriodicIndX;
PeriodicRngX_basic_cosimulation_c_1_sm_master_T
  basic_cosimulation_c_1_sm_master_PeriodicRngX;

/* Real-time model */
static RT_MODEL_basic_cosimulation_c_1_sm_master_T
  basic_cosimulation_c_1_sm_master_M_;
RT_MODEL_basic_cosimulation_c_1_sm_master_T *const
  basic_cosimulation_c_1_sm_master_M = &basic_cosimulation_c_1_sm_master_M_;

/*
 * Time delay interpolation routine
 *
 * The linear interpolation is performed using the formula:
 *
 * (t2 - tMinusDelay)         (tMinusDelay - t1)
 * u(t)  =  ----------------- * u1  +  ------------------- * u2
 * (t2 - t1)                  (t2 - t1)
 */
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
{
  int_T i;
  real_T yout, t1, t2, u1, u2;
  real_T* tBuf = uBuf + bufSz;

  /*
   * If there is only one data point in the buffer, this data point must be
   * the t= 0 and tMinusDelay > t0, it ask for something unknown. The best
   * guess if initial output as well
   */
  if ((newIdx == 0) && (oldestIdx ==0 ) && (tMinusDelay > tStart))
    return initOutput;

  /*
   * If tMinusDelay is less than zero, should output initial value
   */
  if (tMinusDelay <= tStart)
    return initOutput;

  /* For fixed buffer extrapolation:
   * if tMinusDelay is small than the time at oldestIdx, if discrete, output
   * tailptr value,  else use tailptr and tailptr+1 value to extrapolate
   * It is also for fixed buffer. Note: The same condition can happen for transport delay block where
   * use tStart and and t[tail] other than using t[tail] and t[tail+1].
   * See below
   */
  if ((tMinusDelay <= tBuf[oldestIdx] ) ) {
    if (discrete) {
      return(uBuf[oldestIdx]);
    } else {
      int_T tempIdx= oldestIdx + 1;
      if (oldestIdx == bufSz-1)
        tempIdx = 0;
      t1= tBuf[oldestIdx];
      t2= tBuf[tempIdx];
      u1= uBuf[oldestIdx];
      u2= uBuf[tempIdx];
      if (t2 == t1) {
        if (tMinusDelay >= t2) {
          yout = u2;
        } else {
          yout = u1;
        }
      } else {
        real_T f1 = (t2-tMinusDelay) / (t2-t1);
        real_T f2 = 1.0 - f1;

        /*
         * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
         */
        yout = f1*u1 + f2*u2;
      }

      return yout;
    }
  }

  /*
   * When block does not have direct feedthrough, we use the table of
   * values to extrapolate off the end of the table for delays that are less
   * than 0 (less then step size).  This is not completely accurate.  The
   * chain of events is as follows for a given time t.  Major output - look
   * in table.  Update - add entry to table.  Now, if we call the output at
   * time t again, there is a new entry in the table. For very small delays,
   * this means that we will have a different answer from the previous call
   * to the output fcn at the same time t.  The following code prevents this
   * from happening.
   */
  if (minorStepAndTAtLastMajorOutput) {
    /* pretend that the new entry has not been added to table */
    if (newIdx != 0) {
      if (*lastIdx == newIdx) {
        (*lastIdx)--;
      }

      newIdx--;
    } else {
      if (*lastIdx == newIdx) {
        *lastIdx = bufSz-1;
      }

      newIdx = bufSz - 1;
    }
  }

  i = *lastIdx;
  if (tBuf[i] < tMinusDelay) {
    /* Look forward starting at last index */
    while (tBuf[i] < tMinusDelay) {
      /* May occur if the delay is less than step-size - extrapolate */
      if (i == newIdx)
        break;
      i = ( i < (bufSz-1) ) ? (i+1) : 0;/* move through buffer */
    }
  } else {
    /*
     * Look backwards starting at last index which can happen when the
     * delay time increases.
     */
    while (tBuf[i] >= tMinusDelay) {
      /*
       * Due to the entry condition at top of function, we
       * should never hit the end.
       */
      i = (i > 0) ? i-1 : (bufSz-1);   /* move through buffer */
    }

    i = ( i < (bufSz-1) ) ? (i+1) : 0;
  }

  *lastIdx = i;
  if (discrete) {
    /*
     * tempEps = 128 * eps;
     * localEps = max(tempEps, tempEps*fabs(tBuf[i]))/2;
     */
    double tempEps = (DBL_EPSILON) * 128.0;
    double localEps = tempEps * fabs(tBuf[i]);
    if (tempEps > localEps) {
      localEps = tempEps;
    }

    localEps = localEps / 2.0;
    if (tMinusDelay >= (tBuf[i] - localEps)) {
      yout = uBuf[i];
    } else {
      if (i == 0) {
        yout = uBuf[bufSz-1];
      } else {
        yout = uBuf[i-1];
      }
    }
  } else {
    if (i == 0) {
      t1 = tBuf[bufSz-1];
      u1 = uBuf[bufSz-1];
    } else {
      t1 = tBuf[i-1];
      u1 = uBuf[i-1];
    }

    t2 = tBuf[i];
    u2 = uBuf[i];
    if (t2 == t1) {
      if (tMinusDelay >= t2) {
        yout = u2;
      } else {
        yout = u1;
      }
    } else {
      real_T f1 = (t2-tMinusDelay) / (t2-t1);
      real_T f2 = 1.0 - f1;

      /*
       * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
       */
      yout = f1*u1 + f2*u2;
    }
  }

  return(yout);
}

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

/* State reduction function */
void local_stateReduction(real_T* x, int_T* p, int_T n, real_T* r)
{
  int_T i, j;
  for (i = 0, j = 0; i < n; ++i, ++j) {
    int_T k = p[i];
    real_T lb = r[j++];
    real_T xk = x[k]-lb;
    real_T rk = r[j]-lb;
    int_T q = (int_T) floor(xk/rk);
    if (q) {
      x[k] = xk-q*rk+lb;
    }
  }
}

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 56;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  basic_cosimulation_c_1_sm_master_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  basic_cosimulation_c_1_sm_master_output();
  basic_cosimulation_c_1_sm_master_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  basic_cosimulation_c_1_sm_master_output();
  basic_cosimulation_c_1_sm_master_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  basic_cosimulation_c_1_sm_master_output();
  basic_cosimulation_c_1_sm_master_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  local_stateReduction(rtsiGetContStates(si), rtsiGetPeriodicContStateIndices(si),
                       4,
                       rtsiGetPeriodicContStateRanges(si));
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/*
 * System initialize for enable system:
 *    '<S37>/Tail'
 *    '<S116>/Tail'
 */
void basic_cosimulation_c__Tail_Init(B_Tail_basic_cosimulation_c_1_T *localB,
  DW_Tail_basic_cosimulation_c__T *localDW, P_Tail_basic_cosimulation_c_1_T
  *localP)
{
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  int32_T i_2;

  /* InitializeConditions for DiscreteIntegrator: '<S38>/Discrete-Time Integrator' */
  for (i = 0; i < 6; i = i + 1) {
    localDW->DiscreteTimeIntegrator_DSTATE[i] =
      localP->DiscreteTimeIntegrator_IC;
  }

  for (i_0 = 0; i_0 < 6; i_0 = i_0 + 1) {
    localDW->DiscreteTimeIntegrator_PrevRese[i_0] = 2;
  }

  /* End of InitializeConditions for DiscreteIntegrator: '<S38>/Discrete-Time Integrator' */

  /* InitializeConditions for UnitDelay: '<S38>/Unit Delay' */
  for (i_2 = 0; i_2 < 6; i_2 = i_2 + 1) {
    localDW->UnitDelay_DSTATE[i_2] = localP->UnitDelay_InitialCondition;
  }

  /* End of InitializeConditions for UnitDelay: '<S38>/Unit Delay' */
  for (i_1 = 0; i_1 < 6; i_1 = i_1 + 1) {
    /* SystemInitialize for Product: '<S38>/Product' incorporates:
     *  Outport: '<S38>/itail'
     */
    localB->Product[i_1] = localP->itail_Y0;
  }
}

/*
 * System reset for enable system:
 *    '<S37>/Tail'
 *    '<S116>/Tail'
 */
void basic_cosimulation_c_Tail_Reset(DW_Tail_basic_cosimulation_c__T *localDW,
  P_Tail_basic_cosimulation_c_1_T *localP)
{
  int32_T i;
  int32_T i_0;
  int32_T i_1;

  /* InitializeConditions for DiscreteIntegrator: '<S38>/Discrete-Time Integrator' */
  for (i = 0; i < 6; i = i + 1) {
    localDW->DiscreteTimeIntegrator_DSTATE[i] =
      localP->DiscreteTimeIntegrator_IC;
  }

  for (i_0 = 0; i_0 < 6; i_0 = i_0 + 1) {
    localDW->DiscreteTimeIntegrator_PrevRese[i_0] = 2;
  }

  /* End of InitializeConditions for DiscreteIntegrator: '<S38>/Discrete-Time Integrator' */

  /* InitializeConditions for UnitDelay: '<S38>/Unit Delay' */
  for (i_1 = 0; i_1 < 6; i_1 = i_1 + 1) {
    localDW->UnitDelay_DSTATE[i_1] = localP->UnitDelay_InitialCondition;
  }

  /* End of InitializeConditions for UnitDelay: '<S38>/Unit Delay' */
}

/*
 * Disable for enable system:
 *    '<S37>/Tail'
 *    '<S116>/Tail'
 */
void basic_cosimulation_Tail_Disable(DW_Tail_basic_cosimulation_c__T *localDW)
{
  localDW->Tail_MODE = false;
}

/*
 * Outputs for enable system:
 *    '<S37>/Tail'
 *    '<S116>/Tail'
 */
void basic_cosimulation_c_1_sm__Tail(RT_MODEL_basic_cosimulation_c_1_sm_master_T
  * const basic_cosimulation_c_1_sm_master_M, real_T rtu_Enable, const real_T
  rtu_Iak[6], const real_T rtu_g[6], real_T rtp_Tf, real_T rtp_Tt,
  B_Tail_basic_cosimulation_c_1_T *localB, DW_Tail_basic_cosimulation_c__T
  *localDW, P_Tail_basic_cosimulation_c_1_T *localP)
{
  real_T tmp;
  real_T u0;
  real_T u0_0;
  real_T y;
  real_T y_0;
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  int32_T i_2;
  int32_T i_3;
  int32_T i_4;
  int32_T i_5;
  int32_T i_6;
  int32_T i_7;
  int32_T i_8;
  int32_T i_9;

  /* Outputs for Enabled SubSystem: '<S37>/Tail' incorporates:
   *  EnablePort: '<S38>/Enable'
   */
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    if (rtu_Enable > 0.0) {
      if (!localDW->Tail_MODE) {
        basic_cosimulation_c_Tail_Reset(localDW, localP);
        localDW->Tail_MODE = true;
      }
    } else if (localDW->Tail_MODE) {
      basic_cosimulation_Tail_Disable(localDW);
    }
  }

  if (localDW->Tail_MODE) {
    /* DiscreteIntegrator: '<S38>/Discrete-Time Integrator' */
    for (i = 0; i < 6; i = i + 1) {
      if ((rtu_g[i] <= 0.0) && (localDW->DiscreteTimeIntegrator_PrevRese[i] == 1))
      {
        localDW->DiscreteTimeIntegrator_DSTATE[i] =
          localP->DiscreteTimeIntegrator_IC;
      }
    }

    for (i_0 = 0; i_0 < 6; i_0 = i_0 + 1) {
      /* DiscreteIntegrator: '<S38>/Discrete-Time Integrator' */
      localB->DiscreteTimeIntegrator[i_0] =
        localDW->DiscreteTimeIntegrator_DSTATE[i_0];
    }

    /* Gain: '<S41>/-0.9//Tf' */
    tmp = -0.9 / (rtp_Tf + 2.2204460492503131E-16);
    for (i_1 = 0; i_1 < 6; i_1 = i_1 + 1) {
      /* Gain: '<S41>/-0.9//Tf' */
      localB->u9Tf[i_1] = tmp * localB->DiscreteTimeIntegrator[i_1];
    }

    for (i_7 = 0; i_7 < 6; i_7 = i_7 + 1) {
      /* Sum: '<S41>/Add' incorporates:
       *  Constant: '<S41>/Constant'
       */
      localB->Add[i_7] = localP->Constant_Value + localB->u9Tf[i_7];
    }

    for (i_2 = 0; i_2 < 6; i_2 = i_2 + 1) {
      /* Saturate: '<S41>/Saturation1' */
      u0 = localB->Add[i_2];
      if (u0 > localP->Saturation1_UpperSat) {
        y = localP->Saturation1_UpperSat;
      } else if (u0 < localP->Saturation1_LowerSat) {
        y = localP->Saturation1_LowerSat;
      } else {
        y = u0;
      }

      /* Saturate: '<S41>/Saturation1' */
      localB->Saturation1[i_2] = y;
    }

    /* Sum: '<S41>/Add1' incorporates:
     *  Constant: '<S41>/Constant2'
     */
    tmp = rtp_Tf + rtp_Tt;
    for (i_8 = 0; i_8 < 6; i_8 = i_8 + 1) {
      /* Sum: '<S41>/Add1' */
      localB->Add1[i_8] = tmp - localB->DiscreteTimeIntegrator[i_8];
    }

    /* Gain: '<S41>/0.1//Tt' */
    tmp = 0.1 / (rtp_Tt + 2.2204460492503131E-16);
    for (i_3 = 0; i_3 < 6; i_3 = i_3 + 1) {
      /* Gain: '<S41>/0.1//Tt' */
      localB->u1Tt[i_3] = tmp * localB->Add1[i_3];
    }

    for (i_4 = 0; i_4 < 6; i_4 = i_4 + 1) {
      /* Saturate: '<S41>/Saturation2' */
      u0_0 = localB->u1Tt[i_4];
      if (u0_0 > localP->Saturation2_UpperSat) {
        y_0 = localP->Saturation2_UpperSat;
      } else if (u0_0 < localP->Saturation2_LowerSat) {
        y_0 = localP->Saturation2_LowerSat;
      } else {
        y_0 = u0_0;
      }

      /* Saturate: '<S41>/Saturation2' */
      localB->Saturation2[i_4] = y_0;
    }

    for (i_9 = 0; i_9 < 6; i_9 = i_9 + 1) {
      /* Sum: '<S41>/Add2' */
      localB->Add2[i_9] = localB->Saturation1[i_9] + localB->Saturation2[i_9];
    }

    for (i_5 = 0; i_5 < 6; i_5 = i_5 + 1) {
      /* UnitDelay: '<S38>/Unit Delay' */
      localB->UnitDelay[i_5] = localDW->UnitDelay_DSTATE[i_5];
    }

    /* Switch: '<S38>/Switch' */
    for (i = 0; i < 6; i = i + 1) {
      if (rtu_g[i] >= localP->Switch_Threshold) {
        /* Switch: '<S38>/Switch' */
        localB->Switch[i] = rtu_Iak[i];
      } else {
        /* Switch: '<S38>/Switch' */
        localB->Switch[i] = localB->UnitDelay[i];
      }
    }

    /* End of Switch: '<S38>/Switch' */
    for (i_6 = 0; i_6 < 6; i_6 = i_6 + 1) {
      /* Product: '<S38>/Product' incorporates:
       *  Constant: '<S38>/2'
       */
      localB->Product[i_6] = localB->Add2[i_6] * localB->Switch[i_6] *
        localP->u_Value_f;
    }
  }

  /* End of Outputs for SubSystem: '<S37>/Tail' */
}

/*
 * Update for enable system:
 *    '<S37>/Tail'
 *    '<S116>/Tail'
 */
void basic_cosimulation__Tail_Update(const real_T rtu_g[6],
  B_Tail_basic_cosimulation_c_1_T *localB, DW_Tail_basic_cosimulation_c__T
  *localDW, P_Tail_basic_cosimulation_c_1_T *localP)
{
  real_T tmp;
  int32_T i;
  int32_T i_0;
  int32_T i_1;

  /* Update for Enabled SubSystem: '<S37>/Tail' incorporates:
   *  EnablePort: '<S38>/Enable'
   */
  if (localDW->Tail_MODE) {
    /* Update for DiscreteIntegrator: '<S38>/Discrete-Time Integrator' incorporates:
     *  Constant: '<S38>/1'
     */
    tmp = localP->DiscreteTimeIntegrator_gainval * localP->u_Value;
    for (i_1 = 0; i_1 < 6; i_1 = i_1 + 1) {
      localDW->DiscreteTimeIntegrator_DSTATE[i_1] =
        localDW->DiscreteTimeIntegrator_DSTATE[i_1] + tmp;
    }

    for (i = 0; i < 6; i = i + 1) {
      if (rtu_g[i] > 0.0) {
        localDW->DiscreteTimeIntegrator_PrevRese[i] = 1;
      } else if (rtu_g[i] < 0.0) {
        localDW->DiscreteTimeIntegrator_PrevRese[i] = -1;
      } else if (rtu_g[i] == 0.0) {
        localDW->DiscreteTimeIntegrator_PrevRese[i] = 0;
      } else {
        localDW->DiscreteTimeIntegrator_PrevRese[i] = 2;
      }
    }

    /* End of Update for DiscreteIntegrator: '<S38>/Discrete-Time Integrator' */

    /* Update for UnitDelay: '<S38>/Unit Delay' */
    for (i_0 = 0; i_0 < 6; i_0 = i_0 + 1) {
      localDW->UnitDelay_DSTATE[i_0] = localB->Switch[i_0];
    }

    /* End of Update for UnitDelay: '<S38>/Unit Delay' */
  }

  /* End of Update for SubSystem: '<S37>/Tail' */
}

real_T rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T a;
  real_T y;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) {
    a = a / y;
    y = y * sqrt(a * a + 1.0);
  } else if (a > y) {
    y = y / a;
    y = a * sqrt(y * y + 1.0);
  } else if (!rtIsNaN(y)) {
    y = a * 1.4142135623730951;
  }

  return y;
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T tmp;
  real_T tmp_0;
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtGetNaN();
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u1 > 0.0) {
      tmp = 1.0;
    } else {
      tmp = -1.0;
    }

    if (u0 > 0.0) {
      tmp_0 = 1.0;
    } else {
      tmp_0 = -1.0;
    }

    y = atan2(tmp_0, tmp);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Model output function */
void basic_cosimulation_c_1_sm_master_output(void)
{
  creal_T tmp_5;
  creal_T tmp_6;
  creal_T tmp_7;
  real_T Bias;
  real_T b;
  real_T b_idx_0;
  real_T b_idx_1;
  real_T b_idx_2;
  real_T b_idx_3;
  real_T cosOut;
  real_T im;
  real_T im_0;
  real_T im_1;
  real_T im_10;
  real_T im_11;
  real_T im_12;
  real_T im_13;
  real_T im_14;
  real_T im_15;
  real_T im_16;
  real_T im_17;
  real_T im_18;
  real_T im_19;
  real_T im_1a;
  real_T im_1b;
  real_T im_1c;
  real_T im_1d;
  real_T im_1e;
  real_T im_1f;
  real_T im_1g;
  real_T im_1h;
  real_T im_1i;
  real_T im_1j;
  real_T im_1k;
  real_T im_1l;
  real_T im_2;
  real_T im_3;
  real_T im_4;
  real_T im_5;
  real_T im_6;
  real_T im_7;
  real_T im_8;
  real_T im_9;
  real_T im_a;
  real_T im_b;
  real_T im_c;
  real_T im_d;
  real_T im_e;
  real_T im_f;
  real_T im_g;
  real_T im_h;
  real_T im_i;
  real_T im_j;
  real_T im_k;
  real_T im_l;
  real_T im_m;
  real_T im_n;
  real_T im_o;
  real_T im_p;
  real_T im_q;
  real_T im_r;
  real_T im_s;
  real_T im_t;
  real_T im_u;
  real_T im_v;
  real_T im_w;
  real_T im_x;
  real_T im_y;
  real_T im_z;
  real_T re;
  real_T re_0;
  real_T re_1;
  real_T re_10;
  real_T re_11;
  real_T re_12;
  real_T re_13;
  real_T re_14;
  real_T re_15;
  real_T re_16;
  real_T re_17;
  real_T re_18;
  real_T re_19;
  real_T re_1a;
  real_T re_1b;
  real_T re_1c;
  real_T re_1d;
  real_T re_1e;
  real_T re_1f;
  real_T re_1g;
  real_T re_1h;
  real_T re_1i;
  real_T re_1j;
  real_T re_1k;
  real_T re_1l;
  real_T re_2;
  real_T re_3;
  real_T re_4;
  real_T re_5;
  real_T re_6;
  real_T re_7;
  real_T re_8;
  real_T re_9;
  real_T re_a;
  real_T re_b;
  real_T re_c;
  real_T re_d;
  real_T re_e;
  real_T re_f;
  real_T re_g;
  real_T re_h;
  real_T re_i;
  real_T re_j;
  real_T re_k;
  real_T re_l;
  real_T re_m;
  real_T re_n;
  real_T re_o;
  real_T re_p;
  real_T re_q;
  real_T re_r;
  real_T re_s;
  real_T re_t;
  real_T re_u;
  real_T re_v;
  real_T re_w;
  real_T re_x;
  real_T re_y;
  real_T re_z;
  real_T u0;
  real_T u0_0;
  real_T u0_1;
  real_T u0_2;
  real_T u0_3;
  real_T u0_4;
  real_T u0_5;
  real_T u0_6;
  real_T u0_7;
  real_T u0_8;
  real_T u0_9;
  real_T u1;
  real_T u1_0;
  real_T u1_1;
  real_T y;
  real_T y_0;
  real_T y_1;
  real_T y_2;
  real_T y_3;
  real_T y_4;
  real_T y_5;
  real_T y_6;
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  int32_T i_2;
  int32_T i_3;
  int32_T i_4;
  int32_T i_5;
  int32_T i_6;
  int32_T i_7;
  int32_T i_8;
  int32_T i_9;
  int32_T i_a;
  int32_T i_b;
  int32_T i_c;
  int32_T i_d;
  int32_T i_e;
  int32_T i_f;
  int32_T i_g;
  int32_T i_h;
  int32_T i_i;
  int_T tmp;
  int_T tmp_0;
  int_T tmp_1;
  int_T tmp_2;
  int_T tmp_3;
  int_T tmp_4;
  boolean_T didZcEventOccur;
  boolean_T resetSolver;
  boolean_T stateChanged;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* set solver stop time */
    if (!(basic_cosimulation_c_1_sm_master_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                            ((basic_cosimulation_c_1_sm_master_M->Timing.clockTickH0
        + 1) * basic_cosimulation_c_1_sm_master_M->Timing.stepSize0 *
        4294967296.0));
    } else {
      rtsiSetSolverStopTime(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                            ((basic_cosimulation_c_1_sm_master_M->Timing.clockTick0
        + 1) * basic_cosimulation_c_1_sm_master_M->Timing.stepSize0 +
        basic_cosimulation_c_1_sm_master_M->Timing.clockTickH0 *
        basic_cosimulation_c_1_sm_master_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    basic_cosimulation_c_1_sm_master_M->Timing.t[0] = rtsiGetT
      (&basic_cosimulation_c_1_sm_master_M->solverInfo);
  }

  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Memory: '<S2>/S-Function' */
    basic_cosimulation_c_1_sm_master_B.SFunction =
      basic_cosimulation_c_1_sm_master_DW.SFunction_PreviousInput;

    /* Sum: '<S2>/Sum' incorporates:
     *  Constant: '<S2>/S-Function1'
     */
    basic_cosimulation_c_1_sm_master_B.Sum =
      basic_cosimulation_c_1_sm_master_P.SFunction1_Value +
      basic_cosimulation_c_1_sm_master_B.SFunction;

    /* Stop: '<S2>/Stop Simulation' */
    if (basic_cosimulation_c_1_sm_master_B.Sum != 0.0) {
      rtmSetStopRequested(basic_cosimulation_c_1_sm_master_M, 1);
    }

    /* End of Stop: '<S2>/Stop Simulation' */

    /* Constant: '<S3>/timeout' */
    basic_cosimulation_c_1_sm_master_B.timeout =
      basic_cosimulation_c_1_sm_master_P.timeout_Value;

    /* S-Function (sfun_recv_async): '<S6>/S-Function1' */

    /* Level2 S-Function Block: '<S6>/S-Function1' (sfun_recv_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[0];
      sfcnOutputs(rts,0);
    }

    for (i_a = 0; i_a < 6; i_a = i_a + 1) {
      /* UnitDelay: '<S37>/Unit Delay' */
      basic_cosimulation_c_1_sm_master_B.UnitDelay[i_a] =
        basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE[i_a];
    }

    /* Constant: '<S26>/Constant11' */
    basic_cosimulation_c_1_sm_master_B.Constant11 =
      basic_cosimulation_c_1_sm_master_P.fsw_inv;

    /* Delay: '<S45>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[0];
    basic_cosimulation_c_1_sm_master_B.Delay[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[1];
  }

  /* RelationalOperator: '<S51>/Relational Operator' incorporates:
   *  Constant: '<S51>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.RelationalOperator =
    (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE >
     basic_cosimulation_c_1_sm_master_P.Constant2_Value);

  /* Sum: '<S51>/Subtract' incorporates:
   *  Constant: '<S51>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Subtract =
    basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value;

  /* Integrator: '<S51>/Integrator1' */
  /* Limited  Integrator  */
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    resetSolver = false;
    stateChanged = false;
    didZcEventOccur = (((int32_T)
                        basic_cosimulation_c_1_sm_master_B.RelationalOperator >
                        (int32_T)0) &&
                       (basic_cosimulation_c_1_sm_master_PrevZCX.Integrator1_Reset_ZCE
                        != POS_ZCSIG));
    basic_cosimulation_c_1_sm_master_PrevZCX.Integrator1_Reset_ZCE = (uint8_T)
      ((int32_T)basic_cosimulation_c_1_sm_master_B.RelationalOperator > (int32_T)
       0);

    /* evaluate zero-crossings */
    if (didZcEventOccur ||
        (basic_cosimulation_c_1_sm_master_DW.Integrator1_IWORK != 0)) {
      resetSolver = true;
      basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE =
        basic_cosimulation_c_1_sm_master_B.Subtract;
      stateChanged = true;
    }

    if (resetSolver) {
      rtsiSetBlockStateForSolverChangedAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
      if (stateChanged) {
        rtsiSetContTimeOutputInconsistentWithStateAtMajorStep
          (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
      }
    }
  }

  if (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE >=
      basic_cosimulation_c_1_sm_master_P.Integrator1_UpperSat) {
    if (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE !=
        basic_cosimulation_c_1_sm_master_P.Integrator1_UpperSat) {
      rtsiSetBlockStateForSolverChangedAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
      rtsiSetContTimeOutputInconsistentWithStateAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
    }

    basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE =
      basic_cosimulation_c_1_sm_master_P.Integrator1_UpperSat;
  } else if (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE <=
             basic_cosimulation_c_1_sm_master_P.Integrator1_LowerSat) {
    if (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE !=
        basic_cosimulation_c_1_sm_master_P.Integrator1_LowerSat) {
      rtsiSetBlockStateForSolverChangedAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
      rtsiSetContTimeOutputInconsistentWithStateAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
    }

    basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE =
      basic_cosimulation_c_1_sm_master_P.Integrator1_LowerSat;
  }

  /* Integrator: '<S51>/Integrator1' */
  basic_cosimulation_c_1_sm_master_B.Integrator1 =
    basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE;

  /* Trigonometry: '<S52>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Product: '<S52>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product =
    basic_cosimulation_c_1_sm_master_B.Delay[0] *
    basic_cosimulation_c_1_sm_master_B.Cos;

  /* Trigonometry: '<S52>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1 = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Gain: '<S52>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_h =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain *
    basic_cosimulation_c_1_sm_master_B.Cos1;

  /* Product: '<S52>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_o =
    basic_cosimulation_c_1_sm_master_B.Delay[1] *
    basic_cosimulation_c_1_sm_master_B.Gain_h;

  /* Sum: '<S52>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_j =
    basic_cosimulation_c_1_sm_master_B.Product +
    basic_cosimulation_c_1_sm_master_B.Product1_o;

  /* Sum: '<S52>/Sum6' incorporates:
   *  Constant: '<S52>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum6 =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant3_Value;

  /* Trigonometry: '<S52>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2 = cos
    (basic_cosimulation_c_1_sm_master_B.Sum6);

  /* Product: '<S52>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_c =
    basic_cosimulation_c_1_sm_master_B.Delay[0] *
    basic_cosimulation_c_1_sm_master_B.Cos2;

  /* Sum: '<S52>/Sum2' incorporates:
   *  Constant: '<S52>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2 =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant1_Value;

  /* Trigonometry: '<S52>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3 = sin
    (basic_cosimulation_c_1_sm_master_B.Sum2);

  /* Gain: '<S52>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_fm =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_d *
    basic_cosimulation_c_1_sm_master_B.Cos3;

  /* Product: '<S52>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_n =
    basic_cosimulation_c_1_sm_master_B.Delay[1] *
    basic_cosimulation_c_1_sm_master_B.Gain1_fm;

  /* Sum: '<S52>/Sum3' */
  basic_cosimulation_c_1_sm_master_B.Sum3 =
    basic_cosimulation_c_1_sm_master_B.Product2_c +
    basic_cosimulation_c_1_sm_master_B.Product3_n;

  /* Sum: '<S52>/Sum7' incorporates:
   *  Constant: '<S52>/Constant4'
   */
  basic_cosimulation_c_1_sm_master_B.Sum7 =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant4_Value;

  /* Trigonometry: '<S52>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4 = cos
    (basic_cosimulation_c_1_sm_master_B.Sum7);

  /* Product: '<S52>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4 =
    basic_cosimulation_c_1_sm_master_B.Delay[0] *
    basic_cosimulation_c_1_sm_master_B.Cos4;

  /* Sum: '<S52>/Sum4' incorporates:
   *  Constant: '<S52>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum4 =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_m;

  /* Trigonometry: '<S52>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5 = sin
    (basic_cosimulation_c_1_sm_master_B.Sum4);

  /* Gain: '<S52>/Gain2' */
  basic_cosimulation_c_1_sm_master_B.Gain2_h =
    basic_cosimulation_c_1_sm_master_P.Gain2_Gain_e *
    basic_cosimulation_c_1_sm_master_B.Cos5;

  /* Product: '<S52>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5 =
    basic_cosimulation_c_1_sm_master_B.Delay[1] *
    basic_cosimulation_c_1_sm_master_B.Gain2_h;

  /* Sum: '<S52>/Sum5' */
  basic_cosimulation_c_1_sm_master_B.Sum5 =
    basic_cosimulation_c_1_sm_master_B.Product4 +
    basic_cosimulation_c_1_sm_master_B.Product5;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* S-Function (RECV_Param): '<S96>/S-Function' */

    /* Level2 S-Function Block: '<S96>/S-Function' (RECV_Param) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[1];
      sfcnOutputs(rts,0);
    }
  }

  /* Product: '<S17>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_m[0] =
    basic_cosimulation_c_1_sm_master_B.Sum1_j *
    basic_cosimulation_c_1_sm_master_B.SFunction_i[0];
  basic_cosimulation_c_1_sm_master_B.Product2_m[1] =
    basic_cosimulation_c_1_sm_master_B.Sum3 *
    basic_cosimulation_c_1_sm_master_B.SFunction_i[0];
  basic_cosimulation_c_1_sm_master_B.Product2_m[2] =
    basic_cosimulation_c_1_sm_master_B.Sum5 *
    basic_cosimulation_c_1_sm_master_B.SFunction_i[0];

  /* Saturate: '<S17>/Saturation' */
  u0 = basic_cosimulation_c_1_sm_master_B.Product2_m[0];
  if (u0 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat) {
    y = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat;
  } else if (u0 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat) {
    y = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat;
  } else {
    y = u0;
  }

  /* Saturate: '<S17>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation[0] = y;

  /* Saturate: '<S17>/Saturation' */
  u0_0 = basic_cosimulation_c_1_sm_master_B.Product2_m[1];
  if (u0_0 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat) {
    y_0 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat;
  } else if (u0_0 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat) {
    y_0 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat;
  } else {
    y_0 = u0_0;
  }

  /* Saturate: '<S17>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation[1] = y_0;

  /* Saturate: '<S17>/Saturation' */
  u0_1 = basic_cosimulation_c_1_sm_master_B.Product2_m[2];
  if (u0_1 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat) {
    y_1 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat;
  } else if (u0_1 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat) {
    y_1 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat;
  } else {
    y_1 = u0_1;
  }

  /* Saturate: '<S17>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation[2] = y_1;

  /* Sum: '<S26>/Sum16' incorporates:
   *  Constant: '<S26>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum16 =
    basic_cosimulation_c_1_sm_master_B.Saturation[0] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value;

  /* Product: '<S26>/Divide7' incorporates:
   *  Constant: '<S26>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide7 =
    basic_cosimulation_c_1_sm_master_B.Sum16 /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value;

  /* Sum: '<S26>/Sum17' incorporates:
   *  Constant: '<S26>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum17 =
    basic_cosimulation_c_1_sm_master_B.Saturation[1] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value;

  /* Product: '<S26>/Divide8' incorporates:
   *  Constant: '<S26>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide8 =
    basic_cosimulation_c_1_sm_master_B.Sum17 /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value;

  /* Sum: '<S26>/Sum18' incorporates:
   *  Constant: '<S26>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum18 =
    basic_cosimulation_c_1_sm_master_B.Saturation[2] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value;

  /* Product: '<S26>/Divide9' incorporates:
   *  Constant: '<S26>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide9 =
    basic_cosimulation_c_1_sm_master_B.Sum18 /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* SignalConversion generated from: '<S26>/RTE SPWM1' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In[0] =
      basic_cosimulation_c_1_sm_master_B.Divide7;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In[1] =
      basic_cosimulation_c_1_sm_master_B.Divide8;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In[2] =
      basic_cosimulation_c_1_sm_master_B.Divide9;

    /* S-Function (rte_svpwm): '<S26>/RTE SPWM1' */

    /* Level2 S-Function Block: '<S26>/RTE SPWM1' (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[2];
      sfcnOutputs(rts,0);
    }

    /* S-Function (rte_conversion): '<S26>/RTE Conversion1' */

    /* Level2 S-Function Block: '<S26>/RTE Conversion1' (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[3];
      sfcnOutputs(rts,0);
    }

    /* DataTypeConversion: '<S40>/Data Type Conversion' */
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion[0] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1[0];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion[1] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1[3];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion[2] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1[1];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion[3] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1[4];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion[4] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1[2];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion[5] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1[5];

    /* Outputs for Enabled SubSystem: '<S37>/Tail' */
    /* Outputs for Enabled SubSystem: '<S37>/Tail' */
    /* Constant: '<S37>/Not in ARTEMIS' */
    basic_cosimulation_c_1_sm__Tail(basic_cosimulation_c_1_sm_master_M,
      basic_cosimulation_c_1_sm_master_P.NotinARTEMIS_Value,
      basic_cosimulation_c_1_sm_master_B.UnitDelay,
      basic_cosimulation_c_1_sm_master_B.DataTypeConversion,
      basic_cosimulation_c_1_sm_master_P.Tail_Tf,
      basic_cosimulation_c_1_sm_master_P.Tail_Tt,
      &basic_cosimulation_c_1_sm_master_B.Tail,
      &basic_cosimulation_c_1_sm_master_DW.Tail,
      &basic_cosimulation_c_1_sm_master_P.Tail);

    /* End of Outputs for SubSystem: '<S37>/Tail' */
    /* End of Outputs for SubSystem: '<S37>/Tail' */

    /* Constant: '<S86>/DC' */
    basic_cosimulation_c_1_sm_master_B.DC =
      basic_cosimulation_c_1_sm_master_P.Vdc;

    /* Gain: '<S27>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_f[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_e *
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[0 + 3];
    basic_cosimulation_c_1_sm_master_B.Gain_f[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_e *
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[1 + 3];
    basic_cosimulation_c_1_sm_master_B.Gain_f[2] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_e *
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[2 + 3];

    /* MagnitudeAngleToComplex: '<S27>/Magnitude-Angle to Complex' */
    u0_2 = basic_cosimulation_c_1_sm_master_B.Gain_f[0];
    u1 = basic_cosimulation_c_1_sm_master_B.SFunction1_o3[0];
    re_1j = cos(u0_2);
    im_1j = sin(u0_2);
    tmp_5.re = u1 * re_1j;
    tmp_5.im = u1 * im_1j;

    /* MagnitudeAngleToComplex: '<S27>/Magnitude-Angle to Complex' */
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[0].re = tmp_5.re;
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[0].im = tmp_5.im;

    /* MagnitudeAngleToComplex: '<S27>/Magnitude-Angle to Complex' */
    u0_3 = basic_cosimulation_c_1_sm_master_B.Gain_f[1];
    u1_0 = basic_cosimulation_c_1_sm_master_B.SFunction1_o3[1];
    re_1k = cos(u0_3);
    im_1k = sin(u0_3);
    tmp_6.re = u1_0 * re_1k;
    tmp_6.im = u1_0 * im_1k;

    /* MagnitudeAngleToComplex: '<S27>/Magnitude-Angle to Complex' */
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[1].re = tmp_6.re;
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[1].im = tmp_6.im;

    /* MagnitudeAngleToComplex: '<S27>/Magnitude-Angle to Complex' */
    u0_4 = basic_cosimulation_c_1_sm_master_B.Gain_f[2];
    u1_1 = basic_cosimulation_c_1_sm_master_B.SFunction1_o3[2];
    re_1l = cos(u0_4);
    im_1l = sin(u0_4);
    tmp_7.re = u1_1 * re_1l;
    tmp_7.im = u1_1 * im_1l;

    /* MagnitudeAngleToComplex: '<S27>/Magnitude-Angle to Complex' */
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[2].re = tmp_7.re;
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[2].im = tmp_7.im;

    /* S-Function (scomfreqoffset): '<S27>/Phase// Frequency Offset' incorporates:
     *  MagnitudeAngleToComplex: '<S27>/Magnitude-Angle to Complex'
     */
    Bias = 1.0;
    if ((basic_cosimulation_c_1_sm_master_DW.freqTnow >= Bias) ||
        (basic_cosimulation_c_1_sm_master_DW.freqTnow <= -Bias)) {
      basic_cosimulation_c_1_sm_master_DW.freqTnow = fmod
        (basic_cosimulation_c_1_sm_master_DW.freqTnow, Bias);
    }

    /* Compute (w*t + phi) argument for calls to cos and sin below */
    Bias = 6.2831853071795862 * basic_cosimulation_c_1_sm_master_DW.freqTnow +
      basic_cosimulation_c_1_sm_master_P.PhaseFrequencyOffset_Phase;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[0].re =
      basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[0].re * cos
      (Bias) - basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[0].im *
      sin(Bias);
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[0].im =
      basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[0].re * sin
      (Bias) + basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[0].im *
      cos(Bias);
    basic_cosimulation_c_1_sm_master_DW.freqTnow =
      basic_cosimulation_c_1_sm_master_DW.freqTnow +
      basic_cosimulation_c_1_sm_master_P.Phasor_to_Time_Domain_SFA_offse *
      1.6666666666666667E-5;
    Bias = 1.0;
    if ((basic_cosimulation_c_1_sm_master_DW.freqTnow >= Bias) ||
        (basic_cosimulation_c_1_sm_master_DW.freqTnow <= -Bias)) {
      basic_cosimulation_c_1_sm_master_DW.freqTnow = fmod
        (basic_cosimulation_c_1_sm_master_DW.freqTnow, Bias);
    }

    /* Compute (w*t + phi) argument for calls to cos and sin below */
    Bias = 6.2831853071795862 * basic_cosimulation_c_1_sm_master_DW.freqTnow +
      basic_cosimulation_c_1_sm_master_P.PhaseFrequencyOffset_Phase;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[1].re =
      basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[1].re * cos
      (Bias) - basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[1].im *
      sin(Bias);
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[1].im =
      basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[1].re * sin
      (Bias) + basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[1].im *
      cos(Bias);
    basic_cosimulation_c_1_sm_master_DW.freqTnow =
      basic_cosimulation_c_1_sm_master_DW.freqTnow +
      basic_cosimulation_c_1_sm_master_P.Phasor_to_Time_Domain_SFA_offse *
      1.6666666666666667E-5;
    Bias = 1.0;
    if ((basic_cosimulation_c_1_sm_master_DW.freqTnow >= Bias) ||
        (basic_cosimulation_c_1_sm_master_DW.freqTnow <= -Bias)) {
      basic_cosimulation_c_1_sm_master_DW.freqTnow = fmod
        (basic_cosimulation_c_1_sm_master_DW.freqTnow, Bias);
    }

    /* Compute (w*t + phi) argument for calls to cos and sin below */
    Bias = 6.2831853071795862 * basic_cosimulation_c_1_sm_master_DW.freqTnow +
      basic_cosimulation_c_1_sm_master_P.PhaseFrequencyOffset_Phase;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[2].re =
      basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[2].re * cos
      (Bias) - basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[2].im *
      sin(Bias);
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[2].im =
      basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[2].re * sin
      (Bias) + basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[2].im *
      cos(Bias);
    basic_cosimulation_c_1_sm_master_DW.freqTnow =
      basic_cosimulation_c_1_sm_master_DW.freqTnow +
      basic_cosimulation_c_1_sm_master_P.Phasor_to_Time_Domain_SFA_offse *
      1.6666666666666667E-5;

    /* End of S-Function (scomfreqoffset): '<S27>/Phase// Frequency Offset' */

    /* ComplexToRealImag: '<S27>/Complex to Real-Imag4' incorporates:
     *  S-Function (scomfreqoffset): '<S27>/Phase// Frequency Offset'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[0] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[0].re;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[1] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[1].re;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[2] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[2].re;

    /* ComplexToRealImag: '<S27>/Complex to Real-Imag4' incorporates:
     *  S-Function (scomfreqoffset): '<S27>/Phase// Frequency Offset'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[0] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[0].im;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[1] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[1].im;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[2] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[2].im;

    /* Switch: '<S39>/Switch' */
    for (i = 0; i < 6; i = i + 1) {
      if (basic_cosimulation_c_1_sm_master_B.UnitDelay[i] != 0.0) {
        /* Switch: '<S39>/Switch' incorporates:
         *  Constant: '<S39>/Vf Devices & Clamping Diodes'
         */
        basic_cosimulation_c_1_sm_master_B.Switch[i] =
          basic_cosimulation_c_1_sm_master_P.VfDevicesClampingDiodes_Value[i];
      } else {
        /* Switch: '<S39>/Switch' incorporates:
         *  Constant: '<S39>/Vf Diodes'
         */
        basic_cosimulation_c_1_sm_master_B.Switch[i] =
          basic_cosimulation_c_1_sm_master_P.VfDiodes_Value[i];
      }
    }

    /* End of Switch: '<S39>/Switch' */

    /* S-Function (ssn_sfun_v1): '<S162>/State-Space' */

    /* Level2 S-Function Block: '<S162>/State-Space' (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[4];
      sfcnOutputs(rts,0);
    }

    /* Gain: '<S21>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[14];

    /* Gain: '<S22>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_g =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_p *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[15];

    /* Gain: '<S23>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_n =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_a *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[16];

    /* SignalConversion generated from: '<S30>/Gain' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[0] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[1] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_g;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[2] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_n;
    for (i_i = 0; i_i < 3; i_i = i_i + 1) {
      /* Gain: '<S30>/Gain' */
      re_1i = 0.0;
      im_1i = 0.0;
      re_1i = re_1i + basic_cosimulation_c_1_sm_master_P.Gain_Gain_mm[i_i + 0].
        re * basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport
        [0];
      im_1i = im_1i + basic_cosimulation_c_1_sm_master_P.Gain_Gain_mm[i_i + 0].
        im * basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport
        [0];
      re_1i = re_1i + basic_cosimulation_c_1_sm_master_P.Gain_Gain_mm[i_i + 3].
        re * basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport
        [1];
      im_1i = im_1i + basic_cosimulation_c_1_sm_master_P.Gain_Gain_mm[i_i + 3].
        im * basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport
        [1];
      re_1i = re_1i + basic_cosimulation_c_1_sm_master_P.Gain_Gain_mm[i_i + (3 <<
        1)].re *
        basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[2];
      im_1i = im_1i + basic_cosimulation_c_1_sm_master_P.Gain_Gain_mm[i_i + (3 <<
        1)].im *
        basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[2];
      basic_cosimulation_c_1_sm_master_B.Gain[i_i].re = re_1i;
      basic_cosimulation_c_1_sm_master_B.Gain[i_i].im = im_1i;
    }

    /* Sum: '<S30>/Sum1' incorporates:
     *  Gain: '<S30>/Gain'
     */
    basic_cosimulation_c_1_sm_master_B.Sum1[0].re =
      basic_cosimulation_c_1_sm_master_B.Gain[0].re +
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[0];
    basic_cosimulation_c_1_sm_master_B.Sum1[0].im =
      basic_cosimulation_c_1_sm_master_B.Gain[0].im + 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1[1].re =
      basic_cosimulation_c_1_sm_master_B.Gain[1].re +
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[1];
    basic_cosimulation_c_1_sm_master_B.Sum1[1].im =
      basic_cosimulation_c_1_sm_master_B.Gain[1].im + 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1[2].re =
      basic_cosimulation_c_1_sm_master_B.Gain[2].re +
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[2];
    basic_cosimulation_c_1_sm_master_B.Sum1[2].im =
      basic_cosimulation_c_1_sm_master_B.Gain[2].im + 0.0;

    /* S-Function (scomfreqoffset): '<S30>/Phase// Frequency Offset' incorporates:
     *  Sum: '<S30>/Sum1'
     */
    Bias = 1.0;
    if ((basic_cosimulation_c_1_sm_master_DW.freqTnow_p >= Bias) ||
        (basic_cosimulation_c_1_sm_master_DW.freqTnow_p <= -Bias)) {
      basic_cosimulation_c_1_sm_master_DW.freqTnow_p = fmod
        (basic_cosimulation_c_1_sm_master_DW.freqTnow_p, Bias);
    }

    /* Compute (w*t + phi) argument for calls to cos and sin below */
    Bias = 6.2831853071795862 * basic_cosimulation_c_1_sm_master_DW.freqTnow_p +
      basic_cosimulation_c_1_sm_master_P.PhaseFrequencyOffset_Phase_p;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0].re =
      basic_cosimulation_c_1_sm_master_B.Sum1[0].re * cos(Bias) -
      basic_cosimulation_c_1_sm_master_B.Sum1[0].im * sin(Bias);
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0].im =
      basic_cosimulation_c_1_sm_master_B.Sum1[0].re * sin(Bias) +
      basic_cosimulation_c_1_sm_master_B.Sum1[0].im * cos(Bias);
    cosOut = -basic_cosimulation_c_1_sm_master_P.Time_Domain_To_Phasor_SFA_offse;
    basic_cosimulation_c_1_sm_master_DW.freqTnow_p =
      basic_cosimulation_c_1_sm_master_DW.freqTnow_p + cosOut *
      1.6666666666666667E-5;
    Bias = 1.0;
    if ((basic_cosimulation_c_1_sm_master_DW.freqTnow_p >= Bias) ||
        (basic_cosimulation_c_1_sm_master_DW.freqTnow_p <= -Bias)) {
      basic_cosimulation_c_1_sm_master_DW.freqTnow_p = fmod
        (basic_cosimulation_c_1_sm_master_DW.freqTnow_p, Bias);
    }

    /* Compute (w*t + phi) argument for calls to cos and sin below */
    Bias = 6.2831853071795862 * basic_cosimulation_c_1_sm_master_DW.freqTnow_p +
      basic_cosimulation_c_1_sm_master_P.PhaseFrequencyOffset_Phase_p;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1].re =
      basic_cosimulation_c_1_sm_master_B.Sum1[1].re * cos(Bias) -
      basic_cosimulation_c_1_sm_master_B.Sum1[1].im * sin(Bias);
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1].im =
      basic_cosimulation_c_1_sm_master_B.Sum1[1].re * sin(Bias) +
      basic_cosimulation_c_1_sm_master_B.Sum1[1].im * cos(Bias);
    cosOut = -basic_cosimulation_c_1_sm_master_P.Time_Domain_To_Phasor_SFA_offse;
    basic_cosimulation_c_1_sm_master_DW.freqTnow_p =
      basic_cosimulation_c_1_sm_master_DW.freqTnow_p + cosOut *
      1.6666666666666667E-5;
    Bias = 1.0;
    if ((basic_cosimulation_c_1_sm_master_DW.freqTnow_p >= Bias) ||
        (basic_cosimulation_c_1_sm_master_DW.freqTnow_p <= -Bias)) {
      basic_cosimulation_c_1_sm_master_DW.freqTnow_p = fmod
        (basic_cosimulation_c_1_sm_master_DW.freqTnow_p, Bias);
    }

    /* Compute (w*t + phi) argument for calls to cos and sin below */
    Bias = 6.2831853071795862 * basic_cosimulation_c_1_sm_master_DW.freqTnow_p +
      basic_cosimulation_c_1_sm_master_P.PhaseFrequencyOffset_Phase_p;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2].re =
      basic_cosimulation_c_1_sm_master_B.Sum1[2].re * cos(Bias) -
      basic_cosimulation_c_1_sm_master_B.Sum1[2].im * sin(Bias);
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2].im =
      basic_cosimulation_c_1_sm_master_B.Sum1[2].re * sin(Bias) +
      basic_cosimulation_c_1_sm_master_B.Sum1[2].im * cos(Bias);
    cosOut = -basic_cosimulation_c_1_sm_master_P.Time_Domain_To_Phasor_SFA_offse;
    basic_cosimulation_c_1_sm_master_DW.freqTnow_p =
      basic_cosimulation_c_1_sm_master_DW.freqTnow_p + cosOut *
      1.6666666666666667E-5;

    /* End of S-Function (scomfreqoffset): '<S30>/Phase// Frequency Offset' */

    /* ComplexToMagnitudeAngle: '<S30>/Complex to Magnitude-Angle' incorporates:
     *  S-Function (scomfreqoffset): '<S30>/Phase// Frequency Offset'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[0] =
      rt_hypotd_snf(basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0]
                    .re,
                    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0]
                    .im);
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[1] =
      rt_hypotd_snf(basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1]
                    .re,
                    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1]
                    .im);
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[2] =
      rt_hypotd_snf(basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2]
                    .re,
                    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2]
                    .im);

    /* ComplexToMagnitudeAngle: '<S30>/Complex to Magnitude-Angle' incorporates:
     *  S-Function (scomfreqoffset): '<S30>/Phase// Frequency Offset'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[0] =
      rt_atan2d_snf(basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0]
                    .im,
                    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0]
                    .re);
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[1] =
      rt_atan2d_snf(basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1]
                    .im,
                    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1]
                    .re);
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[2] =
      rt_atan2d_snf(basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2]
                    .im,
                    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2]
                    .re);

    /* Gain: '<S30>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_i[0] =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_kr *
      basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[0];
    basic_cosimulation_c_1_sm_master_B.Gain1_i[1] =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_kr *
      basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[1];
    basic_cosimulation_c_1_sm_master_B.Gain1_i[2] =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_kr *
      basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[2];

    /* Delay: '<S5>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_l =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_b;

    /* Gain: '<S46>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6 =
      basic_cosimulation_c_1_sm_master_P.Gain6_Gain *
      basic_cosimulation_c_1_sm_master_B.Delay_l;

    /* Delay: '<S5>/Delay1' */
    basic_cosimulation_c_1_sm_master_B.Delay1 =
      basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE;

    /* Sum: '<S46>/Sum4' */
    basic_cosimulation_c_1_sm_master_B.Sum4_o =
      basic_cosimulation_c_1_sm_master_B.Gain6 +
      basic_cosimulation_c_1_sm_master_B.Delay1;

    /* Gain: '<S46>/Gain7' */
    basic_cosimulation_c_1_sm_master_B.Gain7 =
      basic_cosimulation_c_1_sm_master_P.Gain7_Gain *
      basic_cosimulation_c_1_sm_master_B.Sum4_o;

    /* Sum: '<S46>/Sum5' */
    basic_cosimulation_c_1_sm_master_B.Sum5_c =
      basic_cosimulation_c_1_sm_master_B.Delay1 -
      basic_cosimulation_c_1_sm_master_B.Delay_l;

    /* Gain: '<S46>/Gain8' */
    basic_cosimulation_c_1_sm_master_B.Gain8 =
      basic_cosimulation_c_1_sm_master_P.Gain8_Gain *
      basic_cosimulation_c_1_sm_master_B.Sum5_c;

    /* Gain: '<S46>/Gain10' */
    basic_cosimulation_c_1_sm_master_B.Gain10 =
      basic_cosimulation_c_1_sm_master_P.Gain10_Gain *
      basic_cosimulation_c_1_sm_master_B.Delay1;

    /* Sum: '<S46>/Sum6' */
    Bias = 0.0;

    /* Sum: '<S46>/Sum6' */
    basic_cosimulation_c_1_sm_master_B.Sum6_b = (Bias -
      basic_cosimulation_c_1_sm_master_B.Gain10) -
      basic_cosimulation_c_1_sm_master_B.Delay_l;

    /* Gain: '<S46>/Gain9' */
    basic_cosimulation_c_1_sm_master_B.Gain9 =
      basic_cosimulation_c_1_sm_master_P.Gain9_Gain *
      basic_cosimulation_c_1_sm_master_B.Sum6_b;

    /* Delay: '<S8>/Delay2' */
    basic_cosimulation_c_1_sm_master_B.Delay2[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[0];
    basic_cosimulation_c_1_sm_master_B.Delay2[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1];
    basic_cosimulation_c_1_sm_master_B.Delay2[2] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[2];

    /* Delay: '<S8>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_c =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_f;

    /* Gain: '<S125>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_h =
      basic_cosimulation_c_1_sm_master_P.Gain6_Gain_p *
      basic_cosimulation_c_1_sm_master_B.Delay_c;

    /* Delay: '<S8>/Delay1' */
    basic_cosimulation_c_1_sm_master_B.Delay1_i =
      basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE_a;

    /* Sum: '<S125>/Sum4' */
    basic_cosimulation_c_1_sm_master_B.Sum4_f =
      basic_cosimulation_c_1_sm_master_B.Gain6_h +
      basic_cosimulation_c_1_sm_master_B.Delay1_i;

    /* Gain: '<S125>/Gain7' */
    basic_cosimulation_c_1_sm_master_B.Gain7_b =
      basic_cosimulation_c_1_sm_master_P.Gain7_Gain_d *
      basic_cosimulation_c_1_sm_master_B.Sum4_f;

    /* Sum: '<S125>/Sum5' */
    basic_cosimulation_c_1_sm_master_B.Sum5_b =
      basic_cosimulation_c_1_sm_master_B.Delay1_i -
      basic_cosimulation_c_1_sm_master_B.Delay_c;

    /* Gain: '<S125>/Gain8' */
    basic_cosimulation_c_1_sm_master_B.Gain8_i =
      basic_cosimulation_c_1_sm_master_P.Gain8_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Sum5_b;

    /* Gain: '<S125>/Gain10' */
    basic_cosimulation_c_1_sm_master_B.Gain10_i =
      basic_cosimulation_c_1_sm_master_P.Gain10_Gain_a *
      basic_cosimulation_c_1_sm_master_B.Delay1_i;

    /* Sum: '<S125>/Sum6' */
    Bias = 0.0;

    /* Sum: '<S125>/Sum6' */
    basic_cosimulation_c_1_sm_master_B.Sum6_f = (Bias -
      basic_cosimulation_c_1_sm_master_B.Gain10_i) -
      basic_cosimulation_c_1_sm_master_B.Delay_c;

    /* Gain: '<S125>/Gain9' */
    basic_cosimulation_c_1_sm_master_B.Gain9_n =
      basic_cosimulation_c_1_sm_master_P.Gain9_Gain_n *
      basic_cosimulation_c_1_sm_master_B.Sum6_f;

    /* DataTypeConversion: '<S10>/OpInput' incorporates:
     *  Constant: '<S10>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput =
      basic_cosimulation_c_1_sm_master_P.In1_Value;

    /* DataTypeConversion: '<S9>/OpInput' incorporates:
     *  Constant: '<S9>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput_a =
      basic_cosimulation_c_1_sm_master_P.In1_Value_d;

    /* DataTypeConversion: '<S12>/OpInput' incorporates:
     *  Constant: '<S12>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput_p =
      basic_cosimulation_c_1_sm_master_P.In1_Value_a;

    /* DataTypeConversion: '<S11>/OpInput' incorporates:
     *  Constant: '<S11>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput_b =
      basic_cosimulation_c_1_sm_master_P.In1_Value_dr;

    /* DataTypeConversion: '<S15>/OpInput' incorporates:
     *  Constant: '<S15>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput_f =
      basic_cosimulation_c_1_sm_master_P.In1_Value_i;

    /* S-Function (OP_SEND): '<S161>/S-Function' */

    /* Level2 S-Function Block: '<S161>/S-Function' (OP_SEND) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[5];
      sfcnOutputs(rts,0);
    }

    /* ComplexToRealImag: '<S30>/Complex to Real-Imag1' incorporates:
     *  S-Function (scomfreqoffset): '<S30>/Phase// Frequency Offset'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o1[0] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0].re;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o1[1] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1].re;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o1[2] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2].re;

    /* ComplexToRealImag: '<S30>/Complex to Real-Imag1' incorporates:
     *  S-Function (scomfreqoffset): '<S30>/Phase// Frequency Offset'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o2[0] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0].im;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o2[1] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1].im;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o2[2] =
      basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2].im;
    for (i_0 = 0; i_0 < 6; i_0 = i_0 + 1) {
      /* Gain: '<S37>/1//Ron' */
      basic_cosimulation_c_1_sm_master_B.uRon[i_0] =
        basic_cosimulation_c_1_sm_master_P.uRon_Gain *
        basic_cosimulation_c_1_sm_master_B.StateSpace_o1[i_0];
    }

    /* Switch: '<S37>/Switch' */
    for (i = 0; i < 6; i = i + 1) {
      if (basic_cosimulation_c_1_sm_master_B.StateSpace_o2[i] >=
          basic_cosimulation_c_1_sm_master_P.Switch_Threshold) {
        /* Switch: '<S37>/Switch' */
        basic_cosimulation_c_1_sm_master_B.Switch_l[i] =
          basic_cosimulation_c_1_sm_master_B.uRon[i];
      } else {
        /* Switch: '<S37>/Switch' incorporates:
         *  Constant: '<S37>/0 4'
         */
        basic_cosimulation_c_1_sm_master_B.Switch_l[i] =
          basic_cosimulation_c_1_sm_master_P.u4_Value;
      }
    }

    /* End of Switch: '<S37>/Switch' */
    for (i_1 = 0; i_1 < 6; i_1 = i_1 + 1) {
      /* Saturate: '<S37>/Saturation' */
      u0_9 = basic_cosimulation_c_1_sm_master_B.Switch_l[i_1];
      if (u0_9 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_c) {
        y_6 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_c;
      } else if (u0_9 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_l)
      {
        y_6 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_l;
      } else {
        y_6 = u0_9;
      }

      /* Saturate: '<S37>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_b[i_1] = y_6;
    }

    /* Gain: '<S34>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_l =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_o *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[8];

    /* Gain: '<S35>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_m =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_oq *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[9];

    /* Gain: '<S36>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_k =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_n *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[10];

    /* Delay: '<S5>/Delay2' */
    basic_cosimulation_c_1_sm_master_B.Delay2_f[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[0];
    basic_cosimulation_c_1_sm_master_B.Delay2_f[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1];
    basic_cosimulation_c_1_sm_master_B.Delay2_f[2] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[2];

    /* Delay: '<S47>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_b[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[0];
    basic_cosimulation_c_1_sm_master_B.Delay_b[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[1];

    /* DiscreteIntegrator: '<S45>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE;

    /* DiscreteIntegrator: '<S45>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1 =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE;

    /* Delay: '<S48>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_i[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[0];
    basic_cosimulation_c_1_sm_master_B.Delay_i[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[1];

    /* Gain: '<S49>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_o =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_kd *
      basic_cosimulation_c_1_sm_master_B.SFunction_i[1];

    /* Switch: '<S49>/Switch' */
    if (basic_cosimulation_c_1_sm_master_B.SFunction_i[0] >
        basic_cosimulation_c_1_sm_master_P.Switch_Threshold_l) {
      /* Switch: '<S49>/Switch' */
      basic_cosimulation_c_1_sm_master_B.Switch_o =
        basic_cosimulation_c_1_sm_master_B.Delay_i[0];
    } else {
      /* Switch: '<S49>/Switch' incorporates:
       *  Constant: '<S49>/Constant'
       */
      basic_cosimulation_c_1_sm_master_B.Switch_o =
        basic_cosimulation_c_1_sm_master_P.Vnom_peak;
    }

    /* End of Switch: '<S49>/Switch' */

    /* Gain: '<S49>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_k =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Switch_o;

    /* Product: '<S49>/Divide' */
    basic_cosimulation_c_1_sm_master_B.Divide =
      basic_cosimulation_c_1_sm_master_B.Gain1_o /
      basic_cosimulation_c_1_sm_master_B.Gain_k;

    /* Saturate: '<S45>/Saturation' */
    cosOut = -1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    Bias = 1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    if (basic_cosimulation_c_1_sm_master_B.Divide > Bias) {
      /* Saturate: '<S45>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_a = Bias;
    } else if (basic_cosimulation_c_1_sm_master_B.Divide < cosOut) {
      /* Saturate: '<S45>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_a = cosOut;
    } else {
      /* Saturate: '<S45>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_a =
        basic_cosimulation_c_1_sm_master_B.Divide;
    }

    /* End of Saturate: '<S45>/Saturation' */

    /* Sum: '<S45>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_c =
      basic_cosimulation_c_1_sm_master_B.Saturation_a -
      basic_cosimulation_c_1_sm_master_B.Delay_b[0];

    /* Product: '<S45>/Product' */
    basic_cosimulation_c_1_sm_master_B.Product_l =
      basic_cosimulation_c_1_sm_master_B.Subtract_c *
      basic_cosimulation_c_1_sm_master_B.SFunction_i[0];

    /* Gain: '<S45>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_b2 =
      basic_cosimulation_c_1_sm_master_P.kp_current *
      basic_cosimulation_c_1_sm_master_B.Product_l;

    /* Sum: '<S45>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_i =
      basic_cosimulation_c_1_sm_master_B.Gain1_b2 +
      basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator;

    /* Gain: '<S45>/Gain5' */
    basic_cosimulation_c_1_sm_master_B.Gain5 =
      basic_cosimulation_c_1_sm_master_P.decouple *
      basic_cosimulation_c_1_sm_master_B.Delay_b[1];

    /* Sum: '<S45>/Sum3' */
    basic_cosimulation_c_1_sm_master_B.Sum3_n =
      (basic_cosimulation_c_1_sm_master_B.Delay_i[0] +
       basic_cosimulation_c_1_sm_master_B.Sum1_i) -
      basic_cosimulation_c_1_sm_master_B.Gain5;

    /* Product: '<S45>/Divide' incorporates:
     *  Constant: '<S45>/Constant2'
     */
    cosOut = basic_cosimulation_c_1_sm_master_P.Vdc / 2.0;

    /* Product: '<S45>/Divide' */
    basic_cosimulation_c_1_sm_master_B.Divide_j =
      basic_cosimulation_c_1_sm_master_B.Sum3_n / cosOut;

    /* Gain: '<S45>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_a =
      basic_cosimulation_c_1_sm_master_P.decouple *
      basic_cosimulation_c_1_sm_master_B.Delay_b[0];

    /* Gain: '<S49>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_k =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_a *
      basic_cosimulation_c_1_sm_master_B.SFunction_i[2];

    /* Gain: '<S49>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_b =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_n *
      basic_cosimulation_c_1_sm_master_B.Switch_o;

    /* Product: '<S49>/Divide1' */
    basic_cosimulation_c_1_sm_master_B.Divide1 =
      basic_cosimulation_c_1_sm_master_B.Gain3_k /
      basic_cosimulation_c_1_sm_master_B.Gain2_b;

    /* Saturate: '<S45>/Saturation1' */
    cosOut = -1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    Bias = 1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    if (basic_cosimulation_c_1_sm_master_B.Divide1 > Bias) {
      /* Saturate: '<S45>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1 = Bias;
    } else if (basic_cosimulation_c_1_sm_master_B.Divide1 < cosOut) {
      /* Saturate: '<S45>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1 = cosOut;
    } else {
      /* Saturate: '<S45>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1 =
        basic_cosimulation_c_1_sm_master_B.Divide1;
    }

    /* End of Saturate: '<S45>/Saturation1' */

    /* Sum: '<S45>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1 =
      basic_cosimulation_c_1_sm_master_B.Saturation1 -
      basic_cosimulation_c_1_sm_master_B.Delay_b[1];

    /* Product: '<S45>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_d =
      basic_cosimulation_c_1_sm_master_B.SFunction_i[0] *
      basic_cosimulation_c_1_sm_master_B.Subtract1;

    /* Gain: '<S45>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_e =
      basic_cosimulation_c_1_sm_master_P.kp_current *
      basic_cosimulation_c_1_sm_master_B.Product1_d;

    /* Sum: '<S45>/Sum2' */
    basic_cosimulation_c_1_sm_master_B.Sum2_b =
      basic_cosimulation_c_1_sm_master_B.Gain3_e +
      basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1;

    /* Sum: '<S45>/Sum4' */
    basic_cosimulation_c_1_sm_master_B.Sum4_e =
      (basic_cosimulation_c_1_sm_master_B.Gain6_a +
       basic_cosimulation_c_1_sm_master_B.Sum2_b) +
      basic_cosimulation_c_1_sm_master_B.Delay_i[1];

    /* Product: '<S45>/Divide1' incorporates:
     *  Constant: '<S45>/Constant2'
     */
    cosOut = basic_cosimulation_c_1_sm_master_P.Vdc / 2.0;

    /* Product: '<S45>/Divide1' */
    basic_cosimulation_c_1_sm_master_B.Divide1_j =
      basic_cosimulation_c_1_sm_master_B.Sum4_e / cosOut;

    /* Gain: '<S45>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_k =
      basic_cosimulation_c_1_sm_master_P.ki_current *
      basic_cosimulation_c_1_sm_master_B.Product_l;

    /* Gain: '<S45>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4 =
      basic_cosimulation_c_1_sm_master_P.ki_current *
      basic_cosimulation_c_1_sm_master_B.Product1_d;

    /* SignalConversion generated from: '<S45>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[0] =
      basic_cosimulation_c_1_sm_master_B.Divide_j;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[1] =
      basic_cosimulation_c_1_sm_master_B.Divide1_j;
  }

  /* Trigonometry: '<S47>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_g = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Sum: '<S47>/Sum' incorporates:
   *  Constant: '<S47>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.Sum_a =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant_Value;

  /* Trigonometry: '<S47>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_g = cos
    (basic_cosimulation_c_1_sm_master_B.Sum_a);

  /* Sum: '<S47>/Sum2' incorporates:
   *  Constant: '<S47>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_p =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_f;

  /* Trigonometry: '<S47>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_d = cos
    (basic_cosimulation_c_1_sm_master_B.Sum2_p);

  /* Trigonometry: '<S47>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_i = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Sum: '<S47>/Sum3' incorporates:
   *  Constant: '<S47>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum3_c =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_c;

  /* Trigonometry: '<S47>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_a = sin
    (basic_cosimulation_c_1_sm_master_B.Sum3_c);

  /* Sum: '<S47>/Sum5' incorporates:
   *  Constant: '<S47>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum5_d =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_k;

  /* Trigonometry: '<S47>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_j = sin
    (basic_cosimulation_c_1_sm_master_B.Sum5_d);

  /* Product: '<S47>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_o =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[0] *
    basic_cosimulation_c_1_sm_master_B.Cos_g;

  /* Product: '<S47>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_h =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[1] *
    basic_cosimulation_c_1_sm_master_B.Cos1_g;

  /* Product: '<S47>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_j =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[2] *
    basic_cosimulation_c_1_sm_master_B.Cos2_d;

  /* Sum: '<S47>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_d =
    (basic_cosimulation_c_1_sm_master_B.Product_o +
     basic_cosimulation_c_1_sm_master_B.Product1_h) +
    basic_cosimulation_c_1_sm_master_B.Product2_j;

  /* Gain: '<S47>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_g =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_em *
    basic_cosimulation_c_1_sm_master_B.Sum1_d;

  /* Product: '<S47>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_f =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[0] *
    basic_cosimulation_c_1_sm_master_B.Cos3_i;

  /* Product: '<S47>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_m =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[1] *
    basic_cosimulation_c_1_sm_master_B.Cos4_a;

  /* Product: '<S47>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_o =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[2] *
    basic_cosimulation_c_1_sm_master_B.Cos5_j;

  /* Sum: '<S47>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_g =
    (basic_cosimulation_c_1_sm_master_B.Product3_f +
     basic_cosimulation_c_1_sm_master_B.Product4_m) +
    basic_cosimulation_c_1_sm_master_B.Product5_o;

  /* Gain: '<S47>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_n =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_g *
    basic_cosimulation_c_1_sm_master_B.Sum4_g;

  /* Trigonometry: '<S48>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_m = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Sum: '<S48>/Sum' incorporates:
   *  Constant: '<S48>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.Sum_m =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant_Value_k;

  /* Trigonometry: '<S48>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_e = cos
    (basic_cosimulation_c_1_sm_master_B.Sum_m);

  /* Sum: '<S48>/Sum2' incorporates:
   *  Constant: '<S48>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_e =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_l;

  /* Trigonometry: '<S48>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_l = cos
    (basic_cosimulation_c_1_sm_master_B.Sum2_e);

  /* Trigonometry: '<S48>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_e = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Sum: '<S48>/Sum3' incorporates:
   *  Constant: '<S48>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum3_d =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_n;

  /* Trigonometry: '<S48>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_o = sin
    (basic_cosimulation_c_1_sm_master_B.Sum3_d);

  /* Sum: '<S48>/Sum5' incorporates:
   *  Constant: '<S48>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum5_m =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_a;

  /* Trigonometry: '<S48>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_a = sin
    (basic_cosimulation_c_1_sm_master_B.Sum5_m);

  /* Product: '<S48>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_p =
    basic_cosimulation_c_1_sm_master_B.Gain7 *
    basic_cosimulation_c_1_sm_master_B.Cos_m;

  /* Product: '<S48>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_m =
    basic_cosimulation_c_1_sm_master_B.Gain8 *
    basic_cosimulation_c_1_sm_master_B.Cos1_e;

  /* Product: '<S48>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_p =
    basic_cosimulation_c_1_sm_master_B.Gain9 *
    basic_cosimulation_c_1_sm_master_B.Cos2_l;

  /* Sum: '<S48>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_h =
    (basic_cosimulation_c_1_sm_master_B.Product_p +
     basic_cosimulation_c_1_sm_master_B.Product1_m) +
    basic_cosimulation_c_1_sm_master_B.Product2_p;

  /* Gain: '<S48>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_n =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_m *
    basic_cosimulation_c_1_sm_master_B.Sum1_h;

  /* Product: '<S48>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_k =
    basic_cosimulation_c_1_sm_master_B.Gain7 *
    basic_cosimulation_c_1_sm_master_B.Cos3_e;

  /* Product: '<S48>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_e =
    basic_cosimulation_c_1_sm_master_B.Gain8 *
    basic_cosimulation_c_1_sm_master_B.Cos4_o;

  /* Product: '<S48>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_n =
    basic_cosimulation_c_1_sm_master_B.Gain9 *
    basic_cosimulation_c_1_sm_master_B.Cos5_a;

  /* Sum: '<S48>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_n =
    (basic_cosimulation_c_1_sm_master_B.Product3_k +
     basic_cosimulation_c_1_sm_master_B.Product4_e) +
    basic_cosimulation_c_1_sm_master_B.Product5_n;

  /* Gain: '<S48>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_b1 =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_oe *
    basic_cosimulation_c_1_sm_master_B.Sum4_n;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* SignalConversion generated from: '<S47>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[0] =
      basic_cosimulation_c_1_sm_master_B.Gain_g;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[1] =
      basic_cosimulation_c_1_sm_master_B.Gain1_n;

    /* SignalConversion generated from: '<S48>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[0] =
      basic_cosimulation_c_1_sm_master_B.Gain_n;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[1] =
      basic_cosimulation_c_1_sm_master_B.Gain1_b1;

    /* RelationalOperator: '<S76>/Compare' incorporates:
     *  Constant: '<S76>/Constant'
     */
    basic_cosimulation_c_1_sm_master_B.Compare =
      (basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE <
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDiscr);

    /* RelationalOperator: '<S77>/Compare' incorporates:
     *  Constant: '<S77>/Constant'
     */
    basic_cosimulation_c_1_sm_master_B.Compare_c =
      (basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE >=
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_j);

    /* Logic: '<S75>/Logical Operator' */
    basic_cosimulation_c_1_sm_master_B.LogicalOperator =
      (basic_cosimulation_c_1_sm_master_B.Compare ||
       basic_cosimulation_c_1_sm_master_B.Compare_c);

    /* Bias: '<S78>/Bias' */
    cosOut = -basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDiscr;

    /* Bias: '<S78>/Bias' */
    basic_cosimulation_c_1_sm_master_B.Bias =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE + cosOut;

    /* Gain: '<S78>/Gain' */
    cosOut = 1.0 /
      (basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_j -
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDiscr);

    /* Gain: '<S78>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_e = cosOut *
      basic_cosimulation_c_1_sm_master_B.Bias;

    /* Rounding: '<S78>/Rounding Function' */
    basic_cosimulation_c_1_sm_master_B.RoundingFunction = floor
      (basic_cosimulation_c_1_sm_master_B.Gain_e);

    /* Gain: '<S78>/Gain1' */
    cosOut = basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_j
      - basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDiscr;

    /* Gain: '<S78>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_nl = cosOut *
      basic_cosimulation_c_1_sm_master_B.RoundingFunction;

    /* Sum: '<S78>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_e =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE -
      basic_cosimulation_c_1_sm_master_B.Gain1_nl;

    /* InitialCondition: '<S75>/Initial' */
    if (basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime) {
      basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime = false;

      /* InitialCondition: '<S75>/Initial' */
      basic_cosimulation_c_1_sm_master_B.Initial =
        basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_a;
    } else {
      /* InitialCondition: '<S75>/Initial' */
      basic_cosimulation_c_1_sm_master_B.Initial =
        basic_cosimulation_c_1_sm_master_B.Sum1_e;
    }

    /* End of InitialCondition: '<S75>/Initial' */

    /* DiscreteIntegrator: '<S75>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_B.LogicalOperator) {
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE =
        basic_cosimulation_c_1_sm_master_B.Initial;
    }

    /* DiscreteIntegrator: '<S75>/Integrator' */
    basic_cosimulation_c_1_sm_master_B.Integrator =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE;

    /* Trigonometry: '<S64>/Trigonometric Function1' */
    cosOut = basic_cosimulation_c_1_sm_master_B.Integrator;
    Bias = sin(cosOut);
    cosOut = cos(cosOut);

    /* Trigonometry: '<S64>/Trigonometric Function1' */
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1 = Bias;

    /* Trigonometry: '<S64>/Trigonometric Function1' */
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2 = cosOut;

    /* Product: '<S57>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_hb[0] =
      basic_cosimulation_c_1_sm_master_B.Gain7 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1;
    basic_cosimulation_c_1_sm_master_B.Product1_hb[1] =
      basic_cosimulation_c_1_sm_master_B.Gain7 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2;

    /* DiscreteIntegrator: '<S57>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE != 0) {
      /* DiscreteIntegrator: '<S57>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_c[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_c[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[1];
    } else {
      /* DiscreteIntegrator: '<S57>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_c[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_l *
        basic_cosimulation_c_1_sm_master_B.Product1_hb[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_c[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_l *
        basic_cosimulation_c_1_sm_master_B.Product1_hb[1];
    }

    /* End of DiscreteIntegrator: '<S57>/Integrator' */

    /* UnitDelay: '<S68>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[1];

    /* Sum: '<S68>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_m[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_m[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1[1];

    /* Gain: '<S68>/delta' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S68>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_m[0];
    basic_cosimulation_c_1_sm_master_B.delta[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_m[1];

    /* Sum: '<S68>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1[0] =
      basic_cosimulation_c_1_sm_master_B.delta[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1[0];
    basic_cosimulation_c_1_sm_master_B.Add1[1] =
      basic_cosimulation_c_1_sm_master_B.delta[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1[1];

    /* UnitDelay: '<S68>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[1];

    /* Sum: '<S68>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3[0] =
      basic_cosimulation_c_1_sm_master_B.Add1[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3[1] =
      basic_cosimulation_c_1_sm_master_B.Add1[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[1];

    /* Delay: '<S68>/Delay' */
    tmp = (int32_T)0U << 1;

    /* Delay: '<S68>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_d[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[tmp + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_d[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[tmp + 1];

    /* Sum: '<S68>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_d[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_d[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[1];

    /* Gain: '<S68>/delta'' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S68>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_p[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2[0];
    basic_cosimulation_c_1_sm_master_B.delta_p[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2[1];

    /* Sum: '<S68>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2[0] =
      basic_cosimulation_c_1_sm_master_B.delta_p[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[0];
    basic_cosimulation_c_1_sm_master_B.Add2[1] =
      basic_cosimulation_c_1_sm_master_B.delta_p[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[1];

    /* Sum: '<S68>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[0] -
      basic_cosimulation_c_1_sm_master_B.Add2[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[1] -
      basic_cosimulation_c_1_sm_master_B.Add2[1];

    /* Sum: '<S68>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4[0];
    basic_cosimulation_c_1_sm_master_B.Add3[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4[1];

    /* Gain: '<S68>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_ki[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_e0 *
      basic_cosimulation_c_1_sm_master_B.Add3[0];
    basic_cosimulation_c_1_sm_master_B.Gain_ki[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_e0 *
      basic_cosimulation_c_1_sm_master_B.Add3[1];

    /* Gain: '<S57>/Gain' */
    cosOut = 2.0 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;

    /* Gain: '<S57>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_hy[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_ki[0];
    basic_cosimulation_c_1_sm_master_B.Gain_hy[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_ki[1];

    /* RealImagToComplex: '<S57>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.re =
      basic_cosimulation_c_1_sm_master_B.Gain_hy[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.im =
      basic_cosimulation_c_1_sm_master_B.Gain_hy[1];

    /* Product: '<S59>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_ku[0] =
      basic_cosimulation_c_1_sm_master_B.Gain8 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1;
    basic_cosimulation_c_1_sm_master_B.Product1_ku[1] =
      basic_cosimulation_c_1_sm_master_B.Gain8 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2;

    /* DiscreteIntegrator: '<S59>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_p != 0) {
      /* DiscreteIntegrator: '<S59>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[1];
    } else {
      /* DiscreteIntegrator: '<S59>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_b *
        basic_cosimulation_c_1_sm_master_B.Product1_ku[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_b *
        basic_cosimulation_c_1_sm_master_B.Product1_ku[1];
    }

    /* End of DiscreteIntegrator: '<S59>/Integrator' */

    /* UnitDelay: '<S70>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[1];

    /* Sum: '<S70>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_p[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_p[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[1];

    /* Gain: '<S70>/delta' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S70>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_f[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_p[0];
    basic_cosimulation_c_1_sm_master_B.delta_f[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_p[1];

    /* Sum: '<S70>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_l[0] =
      basic_cosimulation_c_1_sm_master_B.delta_f[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[0];
    basic_cosimulation_c_1_sm_master_B.Add1_l[1] =
      basic_cosimulation_c_1_sm_master_B.delta_f[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[1];

    /* UnitDelay: '<S70>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[1];

    /* Sum: '<S70>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_h[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_l[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_h[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_l[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[1];

    /* Delay: '<S70>/Delay' */
    tmp_0 = (int32_T)0U << 1;

    /* Delay: '<S70>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_lg[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[tmp_0 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_lg[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[tmp_0 + 1];

    /* Sum: '<S70>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_n[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_lg[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_n[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_lg[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[1];

    /* Gain: '<S70>/delta'' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S70>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_c[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_n[0];
    basic_cosimulation_c_1_sm_master_B.delta_c[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_n[1];

    /* Sum: '<S70>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_a[0] =
      basic_cosimulation_c_1_sm_master_B.delta_c[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[0];
    basic_cosimulation_c_1_sm_master_B.Add2_a[1] =
      basic_cosimulation_c_1_sm_master_B.delta_c[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[1];

    /* Sum: '<S70>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_n[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_a[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_n[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_a[1];

    /* Sum: '<S70>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_h[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_h[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_n[0];
    basic_cosimulation_c_1_sm_master_B.Add3_h[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_h[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_n[1];

    /* Gain: '<S70>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_a[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add3_h[0];
    basic_cosimulation_c_1_sm_master_B.Gain_a[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add3_h[1];

    /* Gain: '<S59>/Gain' */
    cosOut = 2.0 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;

    /* Gain: '<S59>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_l[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_a[0];
    basic_cosimulation_c_1_sm_master_B.Gain_l[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_a[1];

    /* RealImagToComplex: '<S59>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.re =
      basic_cosimulation_c_1_sm_master_B.Gain_l[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.im =
      basic_cosimulation_c_1_sm_master_B.Gain_l[1];

    /* Product: '<S61>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_e[0] =
      basic_cosimulation_c_1_sm_master_B.Gain9 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1;
    basic_cosimulation_c_1_sm_master_B.Product1_e[1] =
      basic_cosimulation_c_1_sm_master_B.Gain9 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2;

    /* DiscreteIntegrator: '<S61>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_i != 0) {
      /* DiscreteIntegrator: '<S61>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_b[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_b[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[1];
    } else {
      /* DiscreteIntegrator: '<S61>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_b[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_d *
        basic_cosimulation_c_1_sm_master_B.Product1_e[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_b[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_d *
        basic_cosimulation_c_1_sm_master_B.Product1_e[1];
    }

    /* End of DiscreteIntegrator: '<S61>/Integrator' */

    /* UnitDelay: '<S72>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[1];

    /* Sum: '<S72>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_h[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_h[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[1];

    /* Gain: '<S72>/delta' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S72>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_cv[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_h[0];
    basic_cosimulation_c_1_sm_master_B.delta_cv[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_h[1];

    /* Sum: '<S72>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_e[0] =
      basic_cosimulation_c_1_sm_master_B.delta_cv[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[0];
    basic_cosimulation_c_1_sm_master_B.Add1_e[1] =
      basic_cosimulation_c_1_sm_master_B.delta_cv[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[1];

    /* UnitDelay: '<S72>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[1];

    /* Sum: '<S72>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_d[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_e[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_d[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_e[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[1];

    /* Delay: '<S72>/Delay' */
    tmp_1 = (int32_T)0U << 1;

    /* Delay: '<S72>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_k[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[tmp_1 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_k[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[tmp_1 + 1];

    /* Sum: '<S72>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_g[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_k[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_g[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_k[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[1];

    /* Gain: '<S72>/delta'' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S72>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_e[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_g[0];
    basic_cosimulation_c_1_sm_master_B.delta_e[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_g[1];

    /* Sum: '<S72>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_h[0] =
      basic_cosimulation_c_1_sm_master_B.delta_e[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[0];
    basic_cosimulation_c_1_sm_master_B.Add2_h[1] =
      basic_cosimulation_c_1_sm_master_B.delta_e[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[1];

    /* Sum: '<S72>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_m[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_h[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_m[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_h[1];

    /* Sum: '<S72>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_m[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_d[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_m[0];
    basic_cosimulation_c_1_sm_master_B.Add3_m[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_d[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_m[1];

    /* Gain: '<S72>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_m[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_cw *
      basic_cosimulation_c_1_sm_master_B.Add3_m[0];
    basic_cosimulation_c_1_sm_master_B.Gain_m[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_cw *
      basic_cosimulation_c_1_sm_master_B.Add3_m[1];

    /* Gain: '<S61>/Gain' */
    cosOut = 2.0 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;

    /* Gain: '<S61>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_nw[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_m[0];
    basic_cosimulation_c_1_sm_master_B.Gain_nw[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_m[1];

    /* RealImagToComplex: '<S61>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re =
      basic_cosimulation_c_1_sm_master_B.Gain_nw[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im =
      basic_cosimulation_c_1_sm_master_B.Gain_nw[1];

    /* Product: '<S56>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_e1[0] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[0] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1;
    basic_cosimulation_c_1_sm_master_B.Product1_e1[1] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[0] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2;

    /* DiscreteIntegrator: '<S56>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_h != 0) {
      /* DiscreteIntegrator: '<S56>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[1];
    } else {
      /* DiscreteIntegrator: '<S56>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_c *
        basic_cosimulation_c_1_sm_master_B.Product1_e1[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_c *
        basic_cosimulation_c_1_sm_master_B.Product1_e1[1];
    }

    /* End of DiscreteIntegrator: '<S56>/Integrator' */

    /* UnitDelay: '<S67>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[1];

    /* Sum: '<S67>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_a[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_a[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[1];

    /* Gain: '<S67>/delta' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S67>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_c1[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_a[0];
    basic_cosimulation_c_1_sm_master_B.delta_c1[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_a[1];

    /* Sum: '<S67>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_k[0] =
      basic_cosimulation_c_1_sm_master_B.delta_c1[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[0];
    basic_cosimulation_c_1_sm_master_B.Add1_k[1] =
      basic_cosimulation_c_1_sm_master_B.delta_c1[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[1];

    /* UnitDelay: '<S67>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[1];

    /* Sum: '<S67>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_a[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_k[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_a[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_k[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[1];

    /* Delay: '<S67>/Delay' */
    tmp_2 = (int32_T)0U << 1;

    /* Delay: '<S67>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_in[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[tmp_2 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_in[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[tmp_2 + 1];

    /* Sum: '<S67>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_f[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_in[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_f[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_in[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[1];

    /* Gain: '<S67>/delta'' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S67>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_j[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_f[0];
    basic_cosimulation_c_1_sm_master_B.delta_j[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_f[1];

    /* Sum: '<S67>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_i[0] =
      basic_cosimulation_c_1_sm_master_B.delta_j[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[0];
    basic_cosimulation_c_1_sm_master_B.Add2_i[1] =
      basic_cosimulation_c_1_sm_master_B.delta_j[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[1];

    /* Sum: '<S67>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_h[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_i[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_h[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_i[1];

    /* Sum: '<S67>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_g[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_a[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_h[0];
    basic_cosimulation_c_1_sm_master_B.Add3_g[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_a[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_h[1];

    /* Gain: '<S67>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_d[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_a *
      basic_cosimulation_c_1_sm_master_B.Add3_g[0];
    basic_cosimulation_c_1_sm_master_B.Gain_d[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_a *
      basic_cosimulation_c_1_sm_master_B.Add3_g[1];

    /* Gain: '<S56>/Gain' */
    cosOut = 2.0 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;

    /* Gain: '<S56>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_c[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_d[0];
    basic_cosimulation_c_1_sm_master_B.Gain_c[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_d[1];

    /* RealImagToComplex: '<S56>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re =
      basic_cosimulation_c_1_sm_master_B.Gain_c[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im =
      basic_cosimulation_c_1_sm_master_B.Gain_c[1];

    /* Product: '<S58>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_l[0] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[1] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1;
    basic_cosimulation_c_1_sm_master_B.Product1_l[1] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[1] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2;

    /* DiscreteIntegrator: '<S58>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_iy != 0) {
      /* DiscreteIntegrator: '<S58>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_m[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_m[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[1];
    } else {
      /* DiscreteIntegrator: '<S58>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_m[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_h *
        basic_cosimulation_c_1_sm_master_B.Product1_l[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_m[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_h *
        basic_cosimulation_c_1_sm_master_B.Product1_l[1];
    }

    /* End of DiscreteIntegrator: '<S58>/Integrator' */

    /* UnitDelay: '<S69>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[1];

    /* Sum: '<S69>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_n[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_n[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[1];

    /* Gain: '<S69>/delta' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S69>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_n[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_n[0];
    basic_cosimulation_c_1_sm_master_B.delta_n[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_n[1];

    /* Sum: '<S69>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_ej[0] =
      basic_cosimulation_c_1_sm_master_B.delta_n[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[0];
    basic_cosimulation_c_1_sm_master_B.Add1_ej[1] =
      basic_cosimulation_c_1_sm_master_B.delta_n[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[1];

    /* UnitDelay: '<S69>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[1];

    /* Sum: '<S69>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_aq[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_ej[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_aq[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_ej[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[1];

    /* Delay: '<S69>/Delay' */
    tmp_3 = (int32_T)0U << 1;

    /* Delay: '<S69>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_j[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[tmp_3 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_j[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[tmp_3 + 1];

    /* Sum: '<S69>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_m[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_j[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_m[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_j[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[1];

    /* Gain: '<S69>/delta'' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S69>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_h[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_m[0];
    basic_cosimulation_c_1_sm_master_B.delta_h[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_m[1];

    /* Sum: '<S69>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_l[0] =
      basic_cosimulation_c_1_sm_master_B.delta_h[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[0];
    basic_cosimulation_c_1_sm_master_B.Add2_l[1] =
      basic_cosimulation_c_1_sm_master_B.delta_h[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[1];

    /* Sum: '<S69>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_c[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_l[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_c[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_l[1];

    /* Sum: '<S69>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_c[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_aq[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_c[0];
    basic_cosimulation_c_1_sm_master_B.Add3_c[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_aq[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_c[1];

    /* Gain: '<S69>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_kl[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_jx *
      basic_cosimulation_c_1_sm_master_B.Add3_c[0];
    basic_cosimulation_c_1_sm_master_B.Gain_kl[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_jx *
      basic_cosimulation_c_1_sm_master_B.Add3_c[1];

    /* Gain: '<S58>/Gain' */
    cosOut = 2.0 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;

    /* Gain: '<S58>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_o[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_kl[0];
    basic_cosimulation_c_1_sm_master_B.Gain_o[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_kl[1];

    /* RealImagToComplex: '<S58>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.re =
      basic_cosimulation_c_1_sm_master_B.Gain_o[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.im =
      basic_cosimulation_c_1_sm_master_B.Gain_o[1];

    /* Product: '<S60>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_k4[0] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[2] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1;
    basic_cosimulation_c_1_sm_master_B.Product1_k4[1] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[2] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2;

    /* DiscreteIntegrator: '<S60>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_a != 0) {
      /* DiscreteIntegrator: '<S60>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[1];
    } else {
      /* DiscreteIntegrator: '<S60>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_i *
        basic_cosimulation_c_1_sm_master_B.Product1_k4[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_i *
        basic_cosimulation_c_1_sm_master_B.Product1_k4[1];
    }

    /* End of DiscreteIntegrator: '<S60>/Integrator' */

    /* UnitDelay: '<S71>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[1];

    /* Sum: '<S71>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_pk[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_pk[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[1];

    /* Gain: '<S71>/delta' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S71>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_a[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_pk[0];
    basic_cosimulation_c_1_sm_master_B.delta_a[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract1_pk[1];

    /* Sum: '<S71>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_ea[0] =
      basic_cosimulation_c_1_sm_master_B.delta_a[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[0];
    basic_cosimulation_c_1_sm_master_B.Add1_ea[1] =
      basic_cosimulation_c_1_sm_master_B.delta_a[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[1];

    /* UnitDelay: '<S71>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[1];

    /* Sum: '<S71>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_hy[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_ea[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_hy[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_ea[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[1];

    /* Delay: '<S71>/Delay' */
    tmp_4 = (int32_T)0U << 1;

    /* Delay: '<S71>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_jw[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[tmp_4 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_jw[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[tmp_4 + 1];

    /* Sum: '<S71>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_mn[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_jw[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_mn[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_jw[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[1];

    /* Gain: '<S71>/delta'' */
    Bias = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F
      / basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    cosOut = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S71>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_fs[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_mn[0];
    basic_cosimulation_c_1_sm_master_B.delta_fs[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract2_mn[1];

    /* Sum: '<S71>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_a5[0] =
      basic_cosimulation_c_1_sm_master_B.delta_fs[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[0];
    basic_cosimulation_c_1_sm_master_B.Add2_a5[1] =
      basic_cosimulation_c_1_sm_master_B.delta_fs[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[1];

    /* Sum: '<S71>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_me[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_a5[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_me[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_a5[1];

    /* Sum: '<S71>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_n[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_hy[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_me[0];
    basic_cosimulation_c_1_sm_master_B.Add3_n[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_hy[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_me[1];

    /* Gain: '<S71>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_k2[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_n *
      basic_cosimulation_c_1_sm_master_B.Add3_n[0];
    basic_cosimulation_c_1_sm_master_B.Gain_k2[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_n *
      basic_cosimulation_c_1_sm_master_B.Add3_n[1];

    /* Gain: '<S60>/Gain' */
    cosOut = 2.0 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;

    /* Gain: '<S60>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_cs[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_k2[0];
    basic_cosimulation_c_1_sm_master_B.Gain_cs[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain_k2[1];

    /* RealImagToComplex: '<S60>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.re =
      basic_cosimulation_c_1_sm_master_B.Gain_cs[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.im =
      basic_cosimulation_c_1_sm_master_B.Gain_cs[1];

    /* MultiPortSwitch: '<S53>/Multiport Switch' incorporates:
     *  Constant: '<S53>/Sequence'
     */
    switch ((int32_T)basic_cosimulation_c_1_sm_master_P.Sequence_Value) {
     case 1:
      /* Gain: '<S62>/alpha2' incorporates:
       *  RealImagToComplex: '<S60>/Real-Imag to Complex'
       */
      re_17 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.re -
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.im;
      im_17 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.im +
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.re;

      /* Gain: '<S62>/alpha2' */
      basic_cosimulation_c_1_sm_master_B.alpha2_py.re = re_17;
      basic_cosimulation_c_1_sm_master_B.alpha2_py.im = im_17;

      /* Gain: '<S62>/alpha' incorporates:
       *  RealImagToComplex: '<S58>/Real-Imag to Complex'
       */
      re_19 = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.re -
        basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.im;
      im_19 = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.im +
        basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.re;

      /* Gain: '<S62>/alpha' */
      basic_cosimulation_c_1_sm_master_B.alpha_g.re = re_19;
      basic_cosimulation_c_1_sm_master_B.alpha_g.im = im_19;

      /* Sum: '<S62>/Add' incorporates:
       *  Gain: '<S62>/alpha'
       *  Gain: '<S62>/alpha2'
       *  RealImagToComplex: '<S56>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_gd.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re +
         basic_cosimulation_c_1_sm_master_B.alpha_g.re) +
        basic_cosimulation_c_1_sm_master_B.alpha2_py.re;
      basic_cosimulation_c_1_sm_master_B.Add_gd.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im +
         basic_cosimulation_c_1_sm_master_B.alpha_g.im) +
        basic_cosimulation_c_1_sm_master_B.alpha2_py.im;

      /* Gain: '<S62>/Gain1' incorporates:
       *  Sum: '<S62>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_mo.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
        basic_cosimulation_c_1_sm_master_B.Add_gd.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_mo.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
        basic_cosimulation_c_1_sm_master_B.Add_gd.im;

      /* Math: '<S53>/Math Function' incorporates:
       *  Gain: '<S62>/Gain1'
       */
      basic_cosimulation_c_1_sm_master_B.MathFunction_n.re =
        basic_cosimulation_c_1_sm_master_B.Gain1_mo.re;
      basic_cosimulation_c_1_sm_master_B.MathFunction_n.im =
        -basic_cosimulation_c_1_sm_master_B.Gain1_mo.im;

      /* Gain: '<S63>/alpha2' incorporates:
       *  RealImagToComplex: '<S61>/Real-Imag to Complex'
       */
      re_1c = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_p.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re -
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_p.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im;
      im_1c = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_p.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im +
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_p.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re;

      /* Gain: '<S63>/alpha2' */
      basic_cosimulation_c_1_sm_master_B.alpha2_c.re = re_1c;
      basic_cosimulation_c_1_sm_master_B.alpha2_c.im = im_1c;

      /* Gain: '<S63>/alpha' incorporates:
       *  RealImagToComplex: '<S59>/Real-Imag to Complex'
       */
      re_1e = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.re -
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.im;
      im_1e = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.im +
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.re;

      /* Gain: '<S63>/alpha' */
      basic_cosimulation_c_1_sm_master_B.alpha_m.re = re_1e;
      basic_cosimulation_c_1_sm_master_B.alpha_m.im = im_1e;

      /* Sum: '<S63>/Add' incorporates:
       *  Gain: '<S63>/alpha'
       *  Gain: '<S63>/alpha2'
       *  RealImagToComplex: '<S57>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_l.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.re +
         basic_cosimulation_c_1_sm_master_B.alpha_m.re) +
        basic_cosimulation_c_1_sm_master_B.alpha2_c.re;
      basic_cosimulation_c_1_sm_master_B.Add_l.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.im +
         basic_cosimulation_c_1_sm_master_B.alpha_m.im) +
        basic_cosimulation_c_1_sm_master_B.alpha2_c.im;

      /* Gain: '<S63>/Gain1' incorporates:
       *  Sum: '<S63>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_b0.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_p *
        basic_cosimulation_c_1_sm_master_B.Add_l.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_b0.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_p *
        basic_cosimulation_c_1_sm_master_B.Add_l.im;

      /* Product: '<S53>/Product3' incorporates:
       *  Gain: '<S63>/Gain1'
       *  Math: '<S53>/Math Function'
       */
      re_1g = basic_cosimulation_c_1_sm_master_B.Gain1_b0.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction_n.re -
        basic_cosimulation_c_1_sm_master_B.Gain1_b0.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction_n.im;
      im_1g = basic_cosimulation_c_1_sm_master_B.Gain1_b0.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction_n.im +
        basic_cosimulation_c_1_sm_master_B.Gain1_b0.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction_n.re;

      /* Product: '<S53>/Product3' */
      basic_cosimulation_c_1_sm_master_B.Product3_j.re = re_1g;
      basic_cosimulation_c_1_sm_master_B.Product3_j.im = im_1g;

      /* Gain: '<S53>/Gain2' incorporates:
       *  Product: '<S53>/Product3'
       */
      basic_cosimulation_c_1_sm_master_B.Gain2_p.re =
        basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
        basic_cosimulation_c_1_sm_master_B.Product3_j.re;
      basic_cosimulation_c_1_sm_master_B.Gain2_p.im =
        basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
        basic_cosimulation_c_1_sm_master_B.Product3_j.im;

      /* MultiPortSwitch: '<S53>/Multiport Switch' incorporates:
       *  Gain: '<S53>/Gain2'
       */
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch.re =
        basic_cosimulation_c_1_sm_master_B.Gain2_p.re;
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch.im =
        basic_cosimulation_c_1_sm_master_B.Gain2_p.im;
      break;

     case 2:
      /* Gain: '<S54>/alpha' incorporates:
       *  RealImagToComplex: '<S60>/Real-Imag to Complex'
       */
      re_18 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_o.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.re -
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_o.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.im;
      im_18 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_o.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.im +
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_o.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.re;

      /* Gain: '<S54>/alpha' */
      basic_cosimulation_c_1_sm_master_B.alpha_j.re = re_18;
      basic_cosimulation_c_1_sm_master_B.alpha_j.im = im_18;

      /* Gain: '<S54>/alpha2' incorporates:
       *  RealImagToComplex: '<S58>/Real-Imag to Complex'
       */
      re_1a = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_f.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.re -
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_f.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.im;
      im_1a = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_f.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.im +
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_f.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.re;

      /* Gain: '<S54>/alpha2' */
      basic_cosimulation_c_1_sm_master_B.alpha2_o.re = re_1a;
      basic_cosimulation_c_1_sm_master_B.alpha2_o.im = im_1a;

      /* Sum: '<S54>/Add' incorporates:
       *  Gain: '<S54>/alpha'
       *  Gain: '<S54>/alpha2'
       *  RealImagToComplex: '<S56>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_m.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re +
         basic_cosimulation_c_1_sm_master_B.alpha2_o.re) +
        basic_cosimulation_c_1_sm_master_B.alpha_j.re;
      basic_cosimulation_c_1_sm_master_B.Add_m.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im +
         basic_cosimulation_c_1_sm_master_B.alpha2_o.im) +
        basic_cosimulation_c_1_sm_master_B.alpha_j.im;

      /* Gain: '<S54>/Gain1' incorporates:
       *  Sum: '<S54>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_f.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a *
        basic_cosimulation_c_1_sm_master_B.Add_m.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_f.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a *
        basic_cosimulation_c_1_sm_master_B.Add_m.im;

      /* Math: '<S53>/Math Function1' incorporates:
       *  Gain: '<S54>/Gain1'
       */
      basic_cosimulation_c_1_sm_master_B.MathFunction1_n.re =
        basic_cosimulation_c_1_sm_master_B.Gain1_f.re;
      basic_cosimulation_c_1_sm_master_B.MathFunction1_n.im =
        -basic_cosimulation_c_1_sm_master_B.Gain1_f.im;

      /* Gain: '<S55>/alpha' incorporates:
       *  RealImagToComplex: '<S61>/Real-Imag to Complex'
       */
      re_1d = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re -
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im;
      im_1d = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im +
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re;

      /* Gain: '<S55>/alpha' */
      basic_cosimulation_c_1_sm_master_B.alpha_a.re = re_1d;
      basic_cosimulation_c_1_sm_master_B.alpha_a.im = im_1d;

      /* Gain: '<S55>/alpha2' incorporates:
       *  RealImagToComplex: '<S59>/Real-Imag to Complex'
       */
      re_1f = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_h.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.re -
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_h.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.im;
      im_1f = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_h.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.im +
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_h.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.re;

      /* Gain: '<S55>/alpha2' */
      basic_cosimulation_c_1_sm_master_B.alpha2_g.re = re_1f;
      basic_cosimulation_c_1_sm_master_B.alpha2_g.im = im_1f;

      /* Sum: '<S55>/Add' incorporates:
       *  Gain: '<S55>/alpha'
       *  Gain: '<S55>/alpha2'
       *  RealImagToComplex: '<S57>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_o.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.re +
         basic_cosimulation_c_1_sm_master_B.alpha2_g.re) +
        basic_cosimulation_c_1_sm_master_B.alpha_a.re;
      basic_cosimulation_c_1_sm_master_B.Add_o.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.im +
         basic_cosimulation_c_1_sm_master_B.alpha2_g.im) +
        basic_cosimulation_c_1_sm_master_B.alpha_a.im;

      /* Gain: '<S55>/Gain1' incorporates:
       *  Sum: '<S55>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_bz.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_px *
        basic_cosimulation_c_1_sm_master_B.Add_o.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_bz.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_px *
        basic_cosimulation_c_1_sm_master_B.Add_o.im;

      /* Product: '<S53>/Product1' incorporates:
       *  Gain: '<S55>/Gain1'
       *  Math: '<S53>/Math Function1'
       */
      re_1h = basic_cosimulation_c_1_sm_master_B.Gain1_bz.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction1_n.re -
        basic_cosimulation_c_1_sm_master_B.Gain1_bz.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction1_n.im;
      im_1h = basic_cosimulation_c_1_sm_master_B.Gain1_bz.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction1_n.im +
        basic_cosimulation_c_1_sm_master_B.Gain1_bz.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction1_n.re;

      /* Product: '<S53>/Product1' */
      basic_cosimulation_c_1_sm_master_B.Product1_k.re = re_1h;
      basic_cosimulation_c_1_sm_master_B.Product1_k.im = im_1h;

      /* Gain: '<S53>/Gain1' incorporates:
       *  Product: '<S53>/Product1'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_k.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_p3 *
        basic_cosimulation_c_1_sm_master_B.Product1_k.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_k.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_p3 *
        basic_cosimulation_c_1_sm_master_B.Product1_k.im;

      /* MultiPortSwitch: '<S53>/Multiport Switch' incorporates:
       *  Gain: '<S53>/Gain1'
       */
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch.re =
        basic_cosimulation_c_1_sm_master_B.Gain1_k.re;
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch.im =
        basic_cosimulation_c_1_sm_master_B.Gain1_k.im;
      break;

     default:
      /* Sum: '<S65>/Add' incorporates:
       *  RealImagToComplex: '<S56>/Real-Imag to Complex'
       *  RealImagToComplex: '<S58>/Real-Imag to Complex'
       *  RealImagToComplex: '<S60>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_au.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re +
         basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.re) +
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.re;
      basic_cosimulation_c_1_sm_master_B.Add_au.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im +
         basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.im) +
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.im;

      /* Gain: '<S65>/Gain1' incorporates:
       *  Sum: '<S65>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_j.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_f *
        basic_cosimulation_c_1_sm_master_B.Add_au.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_j.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_f *
        basic_cosimulation_c_1_sm_master_B.Add_au.im;

      /* Math: '<S53>/Math Function2' incorporates:
       *  Gain: '<S65>/Gain1'
       */
      basic_cosimulation_c_1_sm_master_B.MathFunction2_b.re =
        basic_cosimulation_c_1_sm_master_B.Gain1_j.re;
      basic_cosimulation_c_1_sm_master_B.MathFunction2_b.im =
        -basic_cosimulation_c_1_sm_master_B.Gain1_j.im;

      /* Sum: '<S66>/Add' incorporates:
       *  RealImagToComplex: '<S57>/Real-Imag to Complex'
       *  RealImagToComplex: '<S59>/Real-Imag to Complex'
       *  RealImagToComplex: '<S61>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_pe.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.re +
         basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.re) +
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re;
      basic_cosimulation_c_1_sm_master_B.Add_pe.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.im +
         basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.im) +
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im;

      /* Gain: '<S66>/Gain1' incorporates:
       *  Sum: '<S66>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_l.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a0 *
        basic_cosimulation_c_1_sm_master_B.Add_pe.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_l.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a0 *
        basic_cosimulation_c_1_sm_master_B.Add_pe.im;

      /* Product: '<S53>/Product2' incorporates:
       *  Gain: '<S66>/Gain1'
       *  Math: '<S53>/Math Function2'
       */
      re_1b = basic_cosimulation_c_1_sm_master_B.Gain1_l.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction2_b.re -
        basic_cosimulation_c_1_sm_master_B.Gain1_l.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction2_b.im;
      im_1b = basic_cosimulation_c_1_sm_master_B.Gain1_l.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction2_b.im +
        basic_cosimulation_c_1_sm_master_B.Gain1_l.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction2_b.re;

      /* Product: '<S53>/Product2' */
      basic_cosimulation_c_1_sm_master_B.Product2_h.re = re_1b;
      basic_cosimulation_c_1_sm_master_B.Product2_h.im = im_1b;

      /* Gain: '<S53>/Gain3' incorporates:
       *  Product: '<S53>/Product2'
       */
      basic_cosimulation_c_1_sm_master_B.Gain3_o.re =
        basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
        basic_cosimulation_c_1_sm_master_B.Product2_h.re;
      basic_cosimulation_c_1_sm_master_B.Gain3_o.im =
        basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
        basic_cosimulation_c_1_sm_master_B.Product2_h.im;

      /* MultiPortSwitch: '<S53>/Multiport Switch' incorporates:
       *  Gain: '<S53>/Gain3'
       */
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch.re =
        basic_cosimulation_c_1_sm_master_B.Gain3_o.re;
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch.im =
        basic_cosimulation_c_1_sm_master_B.Gain3_o.im;
      break;
    }

    /* End of MultiPortSwitch: '<S53>/Multiport Switch' */

    /* ComplexToRealImag: '<S53>/Complex to Real-Imag' incorporates:
     *  MultiPortSwitch: '<S53>/Multiport Switch'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1 =
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch.re;

    /* ComplexToRealImag: '<S53>/Complex to Real-Imag' incorporates:
     *  MultiPortSwitch: '<S53>/Multiport Switch'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2 =
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch.im;

    /* DataTypeConversion: '<S73>/Conversion' incorporates:
     *  Constant: '<S64>/Radial frequencies'
     */
    basic_cosimulation_c_1_sm_master_B.Conversion = 2.0 * 3.1415926535897931 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K;

    /* Gain: '<S51>/Gain' */
    cosOut = 1.0 / basic_cosimulation_c_1_sm_master_P.Vnom_peak;

    /* Gain: '<S51>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_j[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain7;
    basic_cosimulation_c_1_sm_master_B.Gain_j[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain8;
    basic_cosimulation_c_1_sm_master_B.Gain_j[2] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain9;

    /* Gain: '<S51>/Gain5' */
    basic_cosimulation_c_1_sm_master_B.Gain5_e =
      basic_cosimulation_c_1_sm_master_P.Gain5_Gain *
      basic_cosimulation_c_1_sm_master_B.Gain_j[1];

    /* Gain: '<S51>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_p =
      basic_cosimulation_c_1_sm_master_P.Gain6_Gain_p3 *
      basic_cosimulation_c_1_sm_master_B.Gain_j[2];

    /* Sum: '<S51>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_k =
      basic_cosimulation_c_1_sm_master_B.Gain5_e +
      basic_cosimulation_c_1_sm_master_B.Gain6_p;

    /* Gain: '<S51>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4_k =
      basic_cosimulation_c_1_sm_master_P.Gain4_Gain *
      basic_cosimulation_c_1_sm_master_B.Sum1_k;

    /* Gain: '<S51>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_b3 =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Gain_j[1];

    /* Gain: '<S51>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_j =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Gain_j[2];

    /* Sum: '<S51>/Sum' */
    basic_cosimulation_c_1_sm_master_B.Sum_l =
      (basic_cosimulation_c_1_sm_master_B.Gain_j[0] +
       basic_cosimulation_c_1_sm_master_B.Gain2_b3) +
      basic_cosimulation_c_1_sm_master_B.Gain3_j;

    /* Gain: '<S51>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_e =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_kw *
      basic_cosimulation_c_1_sm_master_B.Sum_l;
  }

  /* Trigonometry: '<S51>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_h = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Trigonometry: '<S51>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_f = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
  }

  /* Trigonometry: '<S51>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_l = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Trigonometry: '<S51>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_i = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Product: '<S51>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_le =
    basic_cosimulation_c_1_sm_master_B.Gain4_k *
    basic_cosimulation_c_1_sm_master_B.Cos_l;

  /* Product: '<S51>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_c =
    basic_cosimulation_c_1_sm_master_B.Gain1_e *
    basic_cosimulation_c_1_sm_master_B.Cos1_i;

  /* Sum: '<S51>/Sum3' */
  basic_cosimulation_c_1_sm_master_B.Sum3_dz =
    basic_cosimulation_c_1_sm_master_B.Product_le -
    basic_cosimulation_c_1_sm_master_B.Product1_c;

  /* Gain: '<S51>/Gain7' */
  basic_cosimulation_c_1_sm_master_B.Gain7_i =
    basic_cosimulation_c_1_sm_master_P.Gain7_Gain_o *
    basic_cosimulation_c_1_sm_master_B.Sum3_dz;

  /* Integrator: '<S51>/Integrator' */
  basic_cosimulation_c_1_sm_master_B.Integrator_dd =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE;

  /* Product: '<S51>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_pp =
    basic_cosimulation_c_1_sm_master_B.Gain4_k *
    basic_cosimulation_c_1_sm_master_B.Cos2_h;

  /* Product: '<S51>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_o =
    basic_cosimulation_c_1_sm_master_B.Gain1_e *
    basic_cosimulation_c_1_sm_master_B.Cos3_f;

  /* Sum: '<S51>/Sum2' */
  basic_cosimulation_c_1_sm_master_B.Sum2_b4 =
    basic_cosimulation_c_1_sm_master_B.Product2_pp +
    basic_cosimulation_c_1_sm_master_B.Product3_o;

  /* Sum: '<S51>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_ng =
    basic_cosimulation_c_1_sm_master_B.Integrator_dd +
    basic_cosimulation_c_1_sm_master_B.Gain7_i;

  /* Sum: '<S51>/Sum5' incorporates:
   *  Constant: '<S51>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.W =
    basic_cosimulation_c_1_sm_master_B.Sum4_ng +
    basic_cosimulation_c_1_sm_master_P.Constant_Value_p;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Gain: '<S18>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_o =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_h *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[11];

    /* Gain: '<S19>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_b *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[12];

    /* Gain: '<S20>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_b =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_e *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[13];

    /* SignalConversion generated from: '<S5>/Delay2' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[0] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_o;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[1] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[2] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_b;

    /* Gain: '<S31>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_gz =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_f *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[6];

    /* Gain: '<S32>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_e =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_ow *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[7];

    /* RealImagToComplex: '<S27>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[0].re =
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[0].im =
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[0 + 3];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[1].re =
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[1];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[1].im =
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[1 + 3];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[2].re =
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[2];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[2].im =
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[2 + 3];

    /* Gain: '<S24>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_j =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_ht *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[17];
    for (i_b = 0; i_b < 6; i_b = i_b + 1) {
      /* UnitDelay: '<S116>/Unit Delay' */
      basic_cosimulation_c_1_sm_master_B.UnitDelay_c[i_b] =
        basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE_g[i_b];
    }

    /* Constant: '<S107>/Constant11' */
    basic_cosimulation_c_1_sm_master_B.Constant11_i =
      basic_cosimulation_c_1_sm_master_P.fsw_inv;

    /* Delay: '<S124>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_bh[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[0];
    basic_cosimulation_c_1_sm_master_B.Delay_bh[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[1];
  }

  /* RelationalOperator: '<S130>/Relational Operator' incorporates:
   *  Constant: '<S130>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.RelationalOperator_i =
    (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE >
     basic_cosimulation_c_1_sm_master_P.Constant2_Value_o);

  /* Sum: '<S130>/Subtract' incorporates:
   *  Constant: '<S130>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Subtract_h =
    basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_o;

  /* Integrator: '<S130>/Integrator2' */
  /* Limited  Integrator  */
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    resetSolver = false;
    stateChanged = false;
    didZcEventOccur = (((int32_T)
                        basic_cosimulation_c_1_sm_master_B.RelationalOperator_i >
                        (int32_T)0) &&
                       (basic_cosimulation_c_1_sm_master_PrevZCX.Integrator2_Reset_ZCE
                        != POS_ZCSIG));
    basic_cosimulation_c_1_sm_master_PrevZCX.Integrator2_Reset_ZCE = (uint8_T)
      ((int32_T)basic_cosimulation_c_1_sm_master_B.RelationalOperator_i >
       (int32_T)0);

    /* evaluate zero-crossings */
    if (didZcEventOccur ||
        (basic_cosimulation_c_1_sm_master_DW.Integrator2_IWORK != 0)) {
      resetSolver = true;
      basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE =
        basic_cosimulation_c_1_sm_master_B.Subtract_h;
      stateChanged = true;
    }

    if (resetSolver) {
      rtsiSetBlockStateForSolverChangedAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
      if (stateChanged) {
        rtsiSetContTimeOutputInconsistentWithStateAtMajorStep
          (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
      }
    }
  }

  if (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE >=
      basic_cosimulation_c_1_sm_master_P.Integrator2_UpperSat) {
    if (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE !=
        basic_cosimulation_c_1_sm_master_P.Integrator2_UpperSat) {
      rtsiSetBlockStateForSolverChangedAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
      rtsiSetContTimeOutputInconsistentWithStateAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
    }

    basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE =
      basic_cosimulation_c_1_sm_master_P.Integrator2_UpperSat;
  } else if (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE <=
             basic_cosimulation_c_1_sm_master_P.Integrator2_LowerSat) {
    if (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE !=
        basic_cosimulation_c_1_sm_master_P.Integrator2_LowerSat) {
      rtsiSetBlockStateForSolverChangedAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
      rtsiSetContTimeOutputInconsistentWithStateAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, true);
    }

    basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE =
      basic_cosimulation_c_1_sm_master_P.Integrator2_LowerSat;
  }

  /* Integrator: '<S130>/Integrator2' */
  basic_cosimulation_c_1_sm_master_B.Integrator2 =
    basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE;

  /* Trigonometry: '<S131>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_h = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Product: '<S131>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_k =
    basic_cosimulation_c_1_sm_master_B.Delay_bh[0] *
    basic_cosimulation_c_1_sm_master_B.Cos_h;

  /* Trigonometry: '<S131>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_j = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Gain: '<S131>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_i =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_o *
    basic_cosimulation_c_1_sm_master_B.Cos1_j;

  /* Product: '<S131>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_cr =
    basic_cosimulation_c_1_sm_master_B.Delay_bh[1] *
    basic_cosimulation_c_1_sm_master_B.Gain_i;

  /* Sum: '<S131>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_n =
    basic_cosimulation_c_1_sm_master_B.Product_k +
    basic_cosimulation_c_1_sm_master_B.Product1_cr;

  /* Sum: '<S131>/Sum6' incorporates:
   *  Constant: '<S131>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum6_g =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_h;

  /* Trigonometry: '<S131>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_m = cos
    (basic_cosimulation_c_1_sm_master_B.Sum6_g);

  /* Product: '<S131>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_g =
    basic_cosimulation_c_1_sm_master_B.Delay_bh[0] *
    basic_cosimulation_c_1_sm_master_B.Cos2_m;

  /* Sum: '<S131>/Sum2' incorporates:
   *  Constant: '<S131>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_m =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_g;

  /* Trigonometry: '<S131>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_g = sin
    (basic_cosimulation_c_1_sm_master_B.Sum2_m);

  /* Gain: '<S131>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_ov =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_gz *
    basic_cosimulation_c_1_sm_master_B.Cos3_g;

  /* Product: '<S131>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_h =
    basic_cosimulation_c_1_sm_master_B.Delay_bh[1] *
    basic_cosimulation_c_1_sm_master_B.Gain1_ov;

  /* Sum: '<S131>/Sum3' */
  basic_cosimulation_c_1_sm_master_B.Sum3_m =
    basic_cosimulation_c_1_sm_master_B.Product2_g +
    basic_cosimulation_c_1_sm_master_B.Product3_h;

  /* Sum: '<S131>/Sum7' incorporates:
   *  Constant: '<S131>/Constant4'
   */
  basic_cosimulation_c_1_sm_master_B.Sum7_c =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant4_Value_n;

  /* Trigonometry: '<S131>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_f = cos
    (basic_cosimulation_c_1_sm_master_B.Sum7_c);

  /* Product: '<S131>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_i =
    basic_cosimulation_c_1_sm_master_B.Delay_bh[0] *
    basic_cosimulation_c_1_sm_master_B.Cos4_f;

  /* Sum: '<S131>/Sum4' incorporates:
   *  Constant: '<S131>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum4_d =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_e;

  /* Trigonometry: '<S131>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_c = sin
    (basic_cosimulation_c_1_sm_master_B.Sum4_d);

  /* Gain: '<S131>/Gain2' */
  basic_cosimulation_c_1_sm_master_B.Gain2_e =
    basic_cosimulation_c_1_sm_master_P.Gain2_Gain_o *
    basic_cosimulation_c_1_sm_master_B.Cos5_c;

  /* Product: '<S131>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_m =
    basic_cosimulation_c_1_sm_master_B.Delay_bh[1] *
    basic_cosimulation_c_1_sm_master_B.Gain2_e;

  /* Sum: '<S131>/Sum5' */
  basic_cosimulation_c_1_sm_master_B.Sum5_e =
    basic_cosimulation_c_1_sm_master_B.Product4_i +
    basic_cosimulation_c_1_sm_master_B.Product5_m;

  /* Product: '<S101>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_f[0] =
    basic_cosimulation_c_1_sm_master_B.Sum1_n *
    basic_cosimulation_c_1_sm_master_B.SFunction_i[0];
  basic_cosimulation_c_1_sm_master_B.Product2_f[1] =
    basic_cosimulation_c_1_sm_master_B.Sum3_m *
    basic_cosimulation_c_1_sm_master_B.SFunction_i[0];
  basic_cosimulation_c_1_sm_master_B.Product2_f[2] =
    basic_cosimulation_c_1_sm_master_B.Sum5_e *
    basic_cosimulation_c_1_sm_master_B.SFunction_i[0];

  /* Saturate: '<S101>/Saturation' */
  u0_5 = basic_cosimulation_c_1_sm_master_B.Product2_f[0];
  if (u0_5 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l) {
    y_2 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l;
  } else if (u0_5 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f) {
    y_2 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f;
  } else {
    y_2 = u0_5;
  }

  /* Saturate: '<S101>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation_g[0] = y_2;

  /* Saturate: '<S101>/Saturation' */
  u0_6 = basic_cosimulation_c_1_sm_master_B.Product2_f[1];
  if (u0_6 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l) {
    y_3 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l;
  } else if (u0_6 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f) {
    y_3 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f;
  } else {
    y_3 = u0_6;
  }

  /* Saturate: '<S101>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation_g[1] = y_3;

  /* Saturate: '<S101>/Saturation' */
  u0_7 = basic_cosimulation_c_1_sm_master_B.Product2_f[2];
  if (u0_7 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l) {
    y_4 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l;
  } else if (u0_7 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f) {
    y_4 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f;
  } else {
    y_4 = u0_7;
  }

  /* Saturate: '<S101>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation_g[2] = y_4;

  /* Sum: '<S107>/Sum16' incorporates:
   *  Constant: '<S107>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum16_m =
    basic_cosimulation_c_1_sm_master_B.Saturation_g[0] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value_k;

  /* Product: '<S107>/Divide7' incorporates:
   *  Constant: '<S107>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide7_i =
    basic_cosimulation_c_1_sm_master_B.Sum16_m /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value_l;

  /* Sum: '<S107>/Sum17' incorporates:
   *  Constant: '<S107>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum17_d =
    basic_cosimulation_c_1_sm_master_B.Saturation_g[1] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value_k;

  /* Product: '<S107>/Divide8' incorporates:
   *  Constant: '<S107>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide8_p =
    basic_cosimulation_c_1_sm_master_B.Sum17_d /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value_l;

  /* Sum: '<S107>/Sum18' incorporates:
   *  Constant: '<S107>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum18_b =
    basic_cosimulation_c_1_sm_master_B.Saturation_g[2] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value_k;

  /* Product: '<S107>/Divide9' incorporates:
   *  Constant: '<S107>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide9_d =
    basic_cosimulation_c_1_sm_master_B.Sum18_b /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value_l;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* SignalConversion generated from: '<S107>/RTE SPWM1' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f[0] =
      basic_cosimulation_c_1_sm_master_B.Divide7_i;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f[1] =
      basic_cosimulation_c_1_sm_master_B.Divide8_p;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f[2] =
      basic_cosimulation_c_1_sm_master_B.Divide9_d;

    /* S-Function (rte_svpwm): '<S107>/RTE SPWM1' */

    /* Level2 S-Function Block: '<S107>/RTE SPWM1' (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[6];
      sfcnOutputs(rts,0);
    }

    /* S-Function (rte_conversion): '<S107>/RTE Conversion1' */

    /* Level2 S-Function Block: '<S107>/RTE Conversion1' (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[7];
      sfcnOutputs(rts,0);
    }

    /* DataTypeConversion: '<S119>/Data Type Conversion' */
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o[0] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1_j[0];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o[1] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1_j[3];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o[2] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1_j[1];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o[3] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1_j[4];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o[4] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1_j[2];
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o[5] =
      basic_cosimulation_c_1_sm_master_B.RTEConversion1_j[5];

    /* Outputs for Enabled SubSystem: '<S116>/Tail' */
    /* Outputs for Enabled SubSystem: '<S116>/Tail' */
    /* Constant: '<S116>/Not in ARTEMIS' */
    basic_cosimulation_c_1_sm__Tail(basic_cosimulation_c_1_sm_master_M,
      basic_cosimulation_c_1_sm_master_P.NotinARTEMIS_Value_k,
      basic_cosimulation_c_1_sm_master_B.UnitDelay_c,
      basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o,
      basic_cosimulation_c_1_sm_master_P.Tail_Tf_a,
      basic_cosimulation_c_1_sm_master_P.Tail_Tt_m,
      &basic_cosimulation_c_1_sm_master_B.Tail_e,
      &basic_cosimulation_c_1_sm_master_DW.Tail_e,
      &basic_cosimulation_c_1_sm_master_P.Tail_e);

    /* End of Outputs for SubSystem: '<S116>/Tail' */
    /* End of Outputs for SubSystem: '<S116>/Tail' */

    /* Constant: '<S152>/DC' */
    basic_cosimulation_c_1_sm_master_B.DC_h =
      basic_cosimulation_c_1_sm_master_P.Vdc;

    /* Sin: '<S157>/Sine Wave A' */
    if (basic_cosimulation_c_1_sm_master_DW.systemEnable != 0) {
      cosOut = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastSin = sin(cosOut *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      cosOut = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastCos = cos(cosOut *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      basic_cosimulation_c_1_sm_master_DW.systemEnable = 0;
    }

    /* Sin: '<S157>/Sine Wave A' */
    basic_cosimulation_c_1_sm_master_B.SineWaveA =
      basic_cosimulation_c_1_sm_master_P.SineWaveA_Amp *
      ((basic_cosimulation_c_1_sm_master_DW.lastSin *
        basic_cosimulation_c_1_sm_master_P.SineWaveA_PCos +
        basic_cosimulation_c_1_sm_master_DW.lastCos *
        basic_cosimulation_c_1_sm_master_P.SineWaveA_PSin) *
       basic_cosimulation_c_1_sm_master_P.SineWaveA_HCos +
       (basic_cosimulation_c_1_sm_master_DW.lastCos *
        basic_cosimulation_c_1_sm_master_P.SineWaveA_PCos -
        basic_cosimulation_c_1_sm_master_DW.lastSin *
        basic_cosimulation_c_1_sm_master_P.SineWaveA_PSin) *
       basic_cosimulation_c_1_sm_master_P.SineWaveA_Hsin) +
      basic_cosimulation_c_1_sm_master_P.SineWaveA_Bias;

    /* Sin: '<S157>/Sine Wave B' */
    if (basic_cosimulation_c_1_sm_master_DW.systemEnable_n != 0) {
      cosOut = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastSin_f = sin(cosOut *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      cosOut = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastCos_m = cos(cosOut *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      basic_cosimulation_c_1_sm_master_DW.systemEnable_n = 0;
    }

    /* Sin: '<S157>/Sine Wave B' */
    basic_cosimulation_c_1_sm_master_B.SineWaveB =
      basic_cosimulation_c_1_sm_master_P.SineWaveB_Amp *
      ((basic_cosimulation_c_1_sm_master_DW.lastSin_f *
        basic_cosimulation_c_1_sm_master_P.SineWaveB_PCos +
        basic_cosimulation_c_1_sm_master_DW.lastCos_m *
        basic_cosimulation_c_1_sm_master_P.SineWaveB_PSin) *
       basic_cosimulation_c_1_sm_master_P.SineWaveB_HCos +
       (basic_cosimulation_c_1_sm_master_DW.lastCos_m *
        basic_cosimulation_c_1_sm_master_P.SineWaveB_PCos -
        basic_cosimulation_c_1_sm_master_DW.lastSin_f *
        basic_cosimulation_c_1_sm_master_P.SineWaveB_PSin) *
       basic_cosimulation_c_1_sm_master_P.SineWaveB_Hsin) +
      basic_cosimulation_c_1_sm_master_P.SineWaveB_Bias;

    /* Sin: '<S157>/Sine Wave C' */
    if (basic_cosimulation_c_1_sm_master_DW.systemEnable_j != 0) {
      cosOut = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastSin_k = sin(cosOut *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      cosOut = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastCos_i = cos(cosOut *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      basic_cosimulation_c_1_sm_master_DW.systemEnable_j = 0;
    }

    /* Sin: '<S157>/Sine Wave C' */
    basic_cosimulation_c_1_sm_master_B.SineWaveC =
      basic_cosimulation_c_1_sm_master_P.SineWaveC_Amp *
      ((basic_cosimulation_c_1_sm_master_DW.lastSin_k *
        basic_cosimulation_c_1_sm_master_P.SineWaveC_PCos +
        basic_cosimulation_c_1_sm_master_DW.lastCos_i *
        basic_cosimulation_c_1_sm_master_P.SineWaveC_PSin) *
       basic_cosimulation_c_1_sm_master_P.SineWaveC_HCos +
       (basic_cosimulation_c_1_sm_master_DW.lastCos_i *
        basic_cosimulation_c_1_sm_master_P.SineWaveC_PCos -
        basic_cosimulation_c_1_sm_master_DW.lastSin_k *
        basic_cosimulation_c_1_sm_master_P.SineWaveC_PSin) *
       basic_cosimulation_c_1_sm_master_P.SineWaveC_Hsin) +
      basic_cosimulation_c_1_sm_master_P.SineWaveC_Bias;

    /* Switch: '<S118>/Switch' */
    for (i = 0; i < 6; i = i + 1) {
      if (basic_cosimulation_c_1_sm_master_B.UnitDelay_c[i] != 0.0) {
        /* Switch: '<S118>/Switch' incorporates:
         *  Constant: '<S118>/Vf Devices & Clamping Diodes'
         */
        basic_cosimulation_c_1_sm_master_B.Switch_d[i] =
          basic_cosimulation_c_1_sm_master_P.VfDevicesClampingDiodes_Value_p[i];
      } else {
        /* Switch: '<S118>/Switch' incorporates:
         *  Constant: '<S118>/Vf Diodes'
         */
        basic_cosimulation_c_1_sm_master_B.Switch_d[i] =
          basic_cosimulation_c_1_sm_master_P.VfDiodes_Value_b[i];
      }
    }

    /* End of Switch: '<S118>/Switch' */

    /* S-Function (ssn_sfun_v1): '<S163>/State-Space' */

    /* Level2 S-Function Block: '<S163>/State-Space' (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[8];
      sfcnOutputs(rts,0);
    }

    for (i_2 = 0; i_2 < 6; i_2 = i_2 + 1) {
      /* Gain: '<S116>/1//Ron' */
      basic_cosimulation_c_1_sm_master_B.uRon_o[i_2] =
        basic_cosimulation_c_1_sm_master_P.uRon_Gain_a *
        basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[i_2];
    }

    /* Switch: '<S116>/Switch' */
    for (i = 0; i < 6; i = i + 1) {
      if (basic_cosimulation_c_1_sm_master_B.StateSpace_o2_f[i] >=
          basic_cosimulation_c_1_sm_master_P.Switch_Threshold_lp) {
        /* Switch: '<S116>/Switch' */
        basic_cosimulation_c_1_sm_master_B.Switch_g[i] =
          basic_cosimulation_c_1_sm_master_B.uRon_o[i];
      } else {
        /* Switch: '<S116>/Switch' incorporates:
         *  Constant: '<S116>/0 4'
         */
        basic_cosimulation_c_1_sm_master_B.Switch_g[i] =
          basic_cosimulation_c_1_sm_master_P.u4_Value_f;
      }
    }

    /* End of Switch: '<S116>/Switch' */
    for (i_3 = 0; i_3 < 6; i_3 = i_3 + 1) {
      /* Saturate: '<S116>/Saturation' */
      u0_8 = basic_cosimulation_c_1_sm_master_B.Switch_g[i_3];
      if (u0_8 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_j) {
        y_5 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_j;
      } else if (u0_8 <
                 basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_fu) {
        y_5 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_fu;
      } else {
        y_5 = u0_8;
      }

      /* Saturate: '<S116>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_h[i_3] = y_5;
    }

    /* Gain: '<S113>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c2 =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_hq *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[8];

    /* Gain: '<S114>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_h =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_ha *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[9];

    /* Gain: '<S115>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_f =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_bv *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[10];

    /* Delay: '<S126>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_f[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cr[0];
    basic_cosimulation_c_1_sm_master_B.Delay_f[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cr[1];

    /* Delay: '<S127>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_iu[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ou[0];
    basic_cosimulation_c_1_sm_master_B.Delay_iu[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ou[1];
  }

  /* Integrator: '<S136>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_cl[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_h[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S136>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
    real_T tMinusDelay ;

    {
      int_T i1;
      real_T *y0 = &basic_cosimulation_c_1_sm_master_B.TransportDelay[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        tMinusDelay = (((1.0 /
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f) :
                       0.0);
        tMinusDelay = simTime - tMinusDelay;
        y0[i1] = rt_TDelayInterpolate(
          tMinusDelay,
          0.0,
          *uBuffer,
          iw_CircularBufSize[i1],
          &iw_Last[i1],
          iw_Tail[i1],
          iw_Head[i1],
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput,
          0,
          0);
        uBuffer++;
      }
    }
  }

  for (i_c = 0; i_c < 8; i_c = i_c + 1) {
    /* Sum: '<S136>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_k[i_c] =
      basic_cosimulation_c_1_sm_master_B.Integrator_cl[i_c] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay[i_c];
  }

  /* Gain: '<S136>/Gain' */
  cosOut = 2.0 *
    basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f;
  for (i_4 = 0; i_4 < 8; i_4 = i_4 + 1) {
    /* Gain: '<S136>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_hs[i_4] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract_k[i_4];
  }

  /* RealImagToComplex: '<S136>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[3 + 4];

  /* Integrator: '<S138>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_e[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_a[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S138>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_b.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
    real_T tMinusDelay ;

    {
      int_T i1;
      real_T *y0 = &basic_cosimulation_c_1_sm_master_B.TransportDelay_m[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        tMinusDelay = (((1.0 /
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f) :
                       0.0);
        tMinusDelay = simTime - tMinusDelay;
        y0[i1] = rt_TDelayInterpolate(
          tMinusDelay,
          0.0,
          *uBuffer,
          iw_CircularBufSize[i1],
          &iw_Last[i1],
          iw_Tail[i1],
          iw_Head[i1],
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_g,
          0,
          0);
        uBuffer++;
      }
    }
  }

  for (i_d = 0; i_d < 8; i_d = i_d + 1) {
    /* Sum: '<S138>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_p[i_d] =
      basic_cosimulation_c_1_sm_master_B.Integrator_e[i_d] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_m[i_d];
  }

  /* Gain: '<S138>/Gain' */
  cosOut = 2.0 *
    basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f;
  for (i_5 = 0; i_5 < 8; i_5 = i_5 + 1) {
    /* Gain: '<S138>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_ow[i_5] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract_p[i_5];
  }

  /* RealImagToComplex: '<S138>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_ow[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_ow[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_ow[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_ow[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_ow[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_ow[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_ow[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_ow[3 + 4];

  /* Integrator: '<S140>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_g[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_d[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S140>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_n.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
    real_T tMinusDelay ;

    {
      int_T i1;
      real_T *y0 = &basic_cosimulation_c_1_sm_master_B.TransportDelay_a[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        tMinusDelay = (((1.0 /
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f) :
                       0.0);
        tMinusDelay = simTime - tMinusDelay;
        y0[i1] = rt_TDelayInterpolate(
          tMinusDelay,
          0.0,
          *uBuffer,
          iw_CircularBufSize[i1],
          &iw_Last[i1],
          iw_Tail[i1],
          iw_Head[i1],
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_l,
          0,
          0);
        uBuffer++;
      }
    }
  }

  for (i_e = 0; i_e < 8; i_e = i_e + 1) {
    /* Sum: '<S140>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_b[i_e] =
      basic_cosimulation_c_1_sm_master_B.Integrator_g[i_e] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_a[i_e];
  }

  /* Gain: '<S140>/Gain' */
  cosOut = 2.0 *
    basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f;
  for (i_6 = 0; i_6 < 8; i_6 = i_6 + 1) {
    /* Gain: '<S140>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_hk[i_6] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract_b[i_6];
  }

  /* RealImagToComplex: '<S140>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hk[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hk[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hk[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hk[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hk[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hk[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hk[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hk[3 + 4];

  /* Integrator: '<S135>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_bf[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_g[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S135>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_h.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
    real_T tMinusDelay ;

    {
      int_T i1;
      real_T *y0 = &basic_cosimulation_c_1_sm_master_B.TransportDelay_d[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        tMinusDelay = (((1.0 /
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f) :
                       0.0);
        tMinusDelay = simTime - tMinusDelay;
        y0[i1] = rt_TDelayInterpolate(
          tMinusDelay,
          0.0,
          *uBuffer,
          iw_CircularBufSize[i1],
          &iw_Last[i1],
          iw_Tail[i1],
          iw_Head[i1],
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_h,
          0,
          0);
        uBuffer++;
      }
    }
  }

  for (i_f = 0; i_f < 8; i_f = i_f + 1) {
    /* Sum: '<S135>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_b2[i_f] =
      basic_cosimulation_c_1_sm_master_B.Integrator_bf[i_f] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_d[i_f];
  }

  /* Gain: '<S135>/Gain' */
  cosOut = 2.0 *
    basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f;
  for (i_7 = 0; i_7 < 8; i_7 = i_7 + 1) {
    /* Gain: '<S135>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_b[i_7] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract_b2[i_7];
  }

  /* RealImagToComplex: '<S135>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_b[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_b[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_b[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_b[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_b[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_b[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_b[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_b[3 + 4];

  /* Integrator: '<S137>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_j[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_m[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S137>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_bn.TUbufferPtrs
      [0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
    real_T tMinusDelay ;

    {
      int_T i1;
      real_T *y0 = &basic_cosimulation_c_1_sm_master_B.TransportDelay_k[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        tMinusDelay = (((1.0 /
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f) :
                       0.0);
        tMinusDelay = simTime - tMinusDelay;
        y0[i1] = rt_TDelayInterpolate(
          tMinusDelay,
          0.0,
          *uBuffer,
          iw_CircularBufSize[i1],
          &iw_Last[i1],
          iw_Tail[i1],
          iw_Head[i1],
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_i,
          0,
          0);
        uBuffer++;
      }
    }
  }

  for (i_g = 0; i_g < 8; i_g = i_g + 1) {
    /* Sum: '<S137>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_i[i_g] =
      basic_cosimulation_c_1_sm_master_B.Integrator_j[i_g] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_k[i_g];
  }

  /* Gain: '<S137>/Gain' */
  cosOut = 2.0 *
    basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f;
  for (i_8 = 0; i_8 < 8; i_8 = i_8 + 1) {
    /* Gain: '<S137>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_dj[i_8] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract_i[i_8];
  }

  /* RealImagToComplex: '<S137>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_dj[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_dj[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_dj[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_dj[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_dj[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_dj[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_dj[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_dj[3 + 4];

  /* Integrator: '<S139>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_n[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_ho[0], 8U *
         sizeof(real_T));

  /* TransportDelay: '<S139>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_d.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
    real_T tMinusDelay ;

    {
      int_T i1;
      real_T *y0 = &basic_cosimulation_c_1_sm_master_B.TransportDelay_p[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        tMinusDelay = (((1.0 /
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f) :
                       0.0);
        tMinusDelay = simTime - tMinusDelay;
        y0[i1] = rt_TDelayInterpolate(
          tMinusDelay,
          0.0,
          *uBuffer,
          iw_CircularBufSize[i1],
          &iw_Last[i1],
          iw_Tail[i1],
          iw_Head[i1],
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_j,
          0,
          0);
        uBuffer++;
      }
    }
  }

  for (i_h = 0; i_h < 8; i_h = i_h + 1) {
    /* Sum: '<S139>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_by[i_h] =
      basic_cosimulation_c_1_sm_master_B.Integrator_n[i_h] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_p[i_h];
  }

  /* Gain: '<S139>/Gain' */
  cosOut = 2.0 *
    basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f;
  for (i_9 = 0; i_9 < 8; i_9 = i_9 + 1) {
    /* Gain: '<S139>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_bo[i_9] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Subtract_by[i_9];
  }

  /* RealImagToComplex: '<S139>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[3 + 4];

  /* MultiPortSwitch: '<S132>/Multiport Switch' incorporates:
   *  Constant: '<S132>/Sequence'
   */
  switch ((int32_T)basic_cosimulation_c_1_sm_master_P.Sequence_Value_o) {
   case 1:
    /* Gain: '<S141>/alpha2' incorporates:
     *  RealImagToComplex: '<S139>/Real-Imag to Complex'
     */
    re = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].im;
    im = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].re;

    /* Gain: '<S141>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_j[0].re = re;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[0].im = im;

    /* Gain: '<S141>/alpha2' incorporates:
     *  RealImagToComplex: '<S139>/Real-Imag to Complex'
     */
    re_1 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].im;
    im_1 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].re;

    /* Gain: '<S141>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_j[1].re = re_1;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[1].im = im_1;

    /* Gain: '<S141>/alpha2' incorporates:
     *  RealImagToComplex: '<S139>/Real-Imag to Complex'
     */
    re_3 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].im;
    im_3 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].re;

    /* Gain: '<S141>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_j[2].re = re_3;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[2].im = im_3;

    /* Gain: '<S141>/alpha2' incorporates:
     *  RealImagToComplex: '<S139>/Real-Imag to Complex'
     */
    re_5 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].im;
    im_5 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].re;

    /* Gain: '<S141>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_j[3].re = re_5;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[3].im = im_5;

    /* Gain: '<S141>/alpha' incorporates:
     *  RealImagToComplex: '<S137>/Real-Imag to Complex'
     */
    re_7 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].im;
    im_7 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].re;

    /* Gain: '<S141>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_h[0].re = re_7;
    basic_cosimulation_c_1_sm_master_B.alpha_h[0].im = im_7;

    /* Gain: '<S141>/alpha' incorporates:
     *  RealImagToComplex: '<S137>/Real-Imag to Complex'
     */
    re_9 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].im;
    im_9 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].re;

    /* Gain: '<S141>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_h[1].re = re_9;
    basic_cosimulation_c_1_sm_master_B.alpha_h[1].im = im_9;

    /* Gain: '<S141>/alpha' incorporates:
     *  RealImagToComplex: '<S137>/Real-Imag to Complex'
     */
    re_b = basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].im;
    im_b = basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].re;

    /* Gain: '<S141>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_h[2].re = re_b;
    basic_cosimulation_c_1_sm_master_B.alpha_h[2].im = im_b;

    /* Gain: '<S141>/alpha' incorporates:
     *  RealImagToComplex: '<S137>/Real-Imag to Complex'
     */
    re_d = basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].im;
    im_d = basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_p.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].re;

    /* Gain: '<S141>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_h[3].re = re_d;
    basic_cosimulation_c_1_sm_master_B.alpha_h[3].im = im_d;

    /* Sum: '<S141>/Add' incorporates:
     *  Gain: '<S141>/alpha'
     *  Gain: '<S141>/alpha2'
     *  RealImagToComplex: '<S135>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_p[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].re +
       basic_cosimulation_c_1_sm_master_B.alpha_h[0].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_p[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].im +
       basic_cosimulation_c_1_sm_master_B.alpha_h[0].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_p[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].re +
       basic_cosimulation_c_1_sm_master_B.alpha_h[1].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_p[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].im +
       basic_cosimulation_c_1_sm_master_B.alpha_h[1].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_p[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].re +
       basic_cosimulation_c_1_sm_master_B.alpha_h[2].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_p[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].im +
       basic_cosimulation_c_1_sm_master_B.alpha_h[2].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_p[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].re +
       basic_cosimulation_c_1_sm_master_B.alpha_h[3].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_p[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].im +
       basic_cosimulation_c_1_sm_master_B.alpha_h[3].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[3].im;

    /* Gain: '<S141>/Gain1' incorporates:
     *  Sum: '<S141>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Add_p[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Add_p[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Add_p[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Add_p[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Add_p[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Add_p[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Add_p[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Add_p[3].im;

    /* Math: '<S132>/Math Function' incorporates:
     *  Gain: '<S141>/Gain1'
     */
    basic_cosimulation_c_1_sm_master_B.MathFunction[0].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction[0].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction[1].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction[1].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction[2].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction[2].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction[3].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction[3].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].im;

    /* Gain: '<S142>/alpha2' incorporates:
     *  RealImagToComplex: '<S140>/Real-Imag to Complex'
     */
    re_g = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im;
    im_g = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re;

    /* Gain: '<S142>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].re = re_g;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].im = im_g;

    /* Gain: '<S142>/alpha2' incorporates:
     *  RealImagToComplex: '<S140>/Real-Imag to Complex'
     */
    re_j = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im;
    im_j = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re;

    /* Gain: '<S142>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].re = re_j;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].im = im_j;

    /* Gain: '<S142>/alpha2' incorporates:
     *  RealImagToComplex: '<S140>/Real-Imag to Complex'
     */
    re_m = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im;
    im_m = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re;

    /* Gain: '<S142>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].re = re_m;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].im = im_m;

    /* Gain: '<S142>/alpha2' incorporates:
     *  RealImagToComplex: '<S140>/Real-Imag to Complex'
     */
    re_p = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im;
    im_p = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_mb.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re;

    /* Gain: '<S142>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].re = re_p;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].im = im_p;

    /* Gain: '<S142>/alpha' incorporates:
     *  RealImagToComplex: '<S138>/Real-Imag to Complex'
     */
    re_r = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].im;
    im_r = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].re;

    /* Gain: '<S142>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_i[0].re = re_r;
    basic_cosimulation_c_1_sm_master_B.alpha_i[0].im = im_r;

    /* Gain: '<S142>/alpha' incorporates:
     *  RealImagToComplex: '<S138>/Real-Imag to Complex'
     */
    re_t = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].im;
    im_t = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].re;

    /* Gain: '<S142>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_i[1].re = re_t;
    basic_cosimulation_c_1_sm_master_B.alpha_i[1].im = im_t;

    /* Gain: '<S142>/alpha' incorporates:
     *  RealImagToComplex: '<S138>/Real-Imag to Complex'
     */
    re_v = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].im;
    im_v = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].re;

    /* Gain: '<S142>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_i[2].re = re_v;
    basic_cosimulation_c_1_sm_master_B.alpha_i[2].im = im_v;

    /* Gain: '<S142>/alpha' incorporates:
     *  RealImagToComplex: '<S138>/Real-Imag to Complex'
     */
    re_x = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].im;
    im_x = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_ay.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].re;

    /* Gain: '<S142>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_i[3].re = re_x;
    basic_cosimulation_c_1_sm_master_B.alpha_i[3].im = im_x;

    /* Sum: '<S142>/Add' incorporates:
     *  Gain: '<S142>/alpha'
     *  Gain: '<S142>/alpha2'
     *  RealImagToComplex: '<S136>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_gb[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re +
       basic_cosimulation_c_1_sm_master_B.alpha_i[0].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_gb[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im +
       basic_cosimulation_c_1_sm_master_B.alpha_i[0].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_gb[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re +
       basic_cosimulation_c_1_sm_master_B.alpha_i[1].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_gb[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im +
       basic_cosimulation_c_1_sm_master_B.alpha_i[1].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_gb[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re +
       basic_cosimulation_c_1_sm_master_B.alpha_i[2].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_gb[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im +
       basic_cosimulation_c_1_sm_master_B.alpha_i[2].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_gb[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re +
       basic_cosimulation_c_1_sm_master_B.alpha_i[3].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_gb[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im +
       basic_cosimulation_c_1_sm_master_B.alpha_i[3].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].im;

    /* Gain: '<S142>/Gain1' incorporates:
     *  Sum: '<S142>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_a[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[3].im;

    /* Product: '<S132>/Product3' incorporates:
     *  Gain: '<S142>/Gain1'
     *  Math: '<S132>/Math Function'
     */
    re_z = basic_cosimulation_c_1_sm_master_B.Gain1_a[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction[0].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_a[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction[0].im;
    im_z = basic_cosimulation_c_1_sm_master_B.Gain1_a[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction[0].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_a[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction[0].re;

    /* Product: '<S132>/Product3' */
    basic_cosimulation_c_1_sm_master_B.Product3[0].re = re_z;
    basic_cosimulation_c_1_sm_master_B.Product3[0].im = im_z;

    /* Product: '<S132>/Product3' incorporates:
     *  Gain: '<S142>/Gain1'
     *  Math: '<S132>/Math Function'
     */
    re_11 = basic_cosimulation_c_1_sm_master_B.Gain1_a[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction[1].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_a[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction[1].im;
    im_11 = basic_cosimulation_c_1_sm_master_B.Gain1_a[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction[1].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_a[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction[1].re;

    /* Product: '<S132>/Product3' */
    basic_cosimulation_c_1_sm_master_B.Product3[1].re = re_11;
    basic_cosimulation_c_1_sm_master_B.Product3[1].im = im_11;

    /* Product: '<S132>/Product3' incorporates:
     *  Gain: '<S142>/Gain1'
     *  Math: '<S132>/Math Function'
     */
    re_13 = basic_cosimulation_c_1_sm_master_B.Gain1_a[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction[2].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_a[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction[2].im;
    im_13 = basic_cosimulation_c_1_sm_master_B.Gain1_a[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction[2].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_a[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction[2].re;

    /* Product: '<S132>/Product3' */
    basic_cosimulation_c_1_sm_master_B.Product3[2].re = re_13;
    basic_cosimulation_c_1_sm_master_B.Product3[2].im = im_13;

    /* Product: '<S132>/Product3' incorporates:
     *  Gain: '<S142>/Gain1'
     *  Math: '<S132>/Math Function'
     */
    re_15 = basic_cosimulation_c_1_sm_master_B.Gain1_a[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction[3].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_a[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction[3].im;
    im_15 = basic_cosimulation_c_1_sm_master_B.Gain1_a[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction[3].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_a[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction[3].re;

    /* Product: '<S132>/Product3' */
    basic_cosimulation_c_1_sm_master_B.Product3[3].re = re_15;
    basic_cosimulation_c_1_sm_master_B.Product3[3].im = im_15;

    /* Gain: '<S132>/Gain2' incorporates:
     *  Product: '<S132>/Product3'
     */
    basic_cosimulation_c_1_sm_master_B.Gain2[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_f *
      basic_cosimulation_c_1_sm_master_B.Product3[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain2[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_f *
      basic_cosimulation_c_1_sm_master_B.Product3[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain2[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_f *
      basic_cosimulation_c_1_sm_master_B.Product3[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain2[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_f *
      basic_cosimulation_c_1_sm_master_B.Product3[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain2[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_f *
      basic_cosimulation_c_1_sm_master_B.Product3[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain2[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_f *
      basic_cosimulation_c_1_sm_master_B.Product3[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain2[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_f *
      basic_cosimulation_c_1_sm_master_B.Product3[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain2[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_f *
      basic_cosimulation_c_1_sm_master_B.Product3[3].im;

    /* MultiPortSwitch: '<S132>/Multiport Switch' incorporates:
     *  Gain: '<S132>/Gain2'
     */
    memcpy(&basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[0],
           &basic_cosimulation_c_1_sm_master_B.Gain2[0], 4U * sizeof(creal_T));
    break;

   case 2:
    /* Gain: '<S133>/alpha' incorporates:
     *  RealImagToComplex: '<S139>/Real-Imag to Complex'
     */
    re_0 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].im;
    im_0 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].re;

    /* Gain: '<S133>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha[0].re = re_0;
    basic_cosimulation_c_1_sm_master_B.alpha[0].im = im_0;

    /* Gain: '<S133>/alpha' incorporates:
     *  RealImagToComplex: '<S139>/Real-Imag to Complex'
     */
    re_2 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].im;
    im_2 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].re;

    /* Gain: '<S133>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha[1].re = re_2;
    basic_cosimulation_c_1_sm_master_B.alpha[1].im = im_2;

    /* Gain: '<S133>/alpha' incorporates:
     *  RealImagToComplex: '<S139>/Real-Imag to Complex'
     */
    re_4 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].im;
    im_4 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].re;

    /* Gain: '<S133>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha[2].re = re_4;
    basic_cosimulation_c_1_sm_master_B.alpha[2].im = im_4;

    /* Gain: '<S133>/alpha' incorporates:
     *  RealImagToComplex: '<S139>/Real-Imag to Complex'
     */
    re_6 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].im;
    im_6 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].re;

    /* Gain: '<S133>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha[3].re = re_6;
    basic_cosimulation_c_1_sm_master_B.alpha[3].im = im_6;

    /* Gain: '<S133>/alpha2' incorporates:
     *  RealImagToComplex: '<S137>/Real-Imag to Complex'
     */
    re_8 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].im;
    im_8 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].re;

    /* Gain: '<S133>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2[0].re = re_8;
    basic_cosimulation_c_1_sm_master_B.alpha2[0].im = im_8;

    /* Gain: '<S133>/alpha2' incorporates:
     *  RealImagToComplex: '<S137>/Real-Imag to Complex'
     */
    re_a = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].im;
    im_a = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].re;

    /* Gain: '<S133>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2[1].re = re_a;
    basic_cosimulation_c_1_sm_master_B.alpha2[1].im = im_a;

    /* Gain: '<S133>/alpha2' incorporates:
     *  RealImagToComplex: '<S137>/Real-Imag to Complex'
     */
    re_c = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].im;
    im_c = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].re;

    /* Gain: '<S133>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2[2].re = re_c;
    basic_cosimulation_c_1_sm_master_B.alpha2[2].im = im_c;

    /* Gain: '<S133>/alpha2' incorporates:
     *  RealImagToComplex: '<S137>/Real-Imag to Complex'
     */
    re_e = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].im;
    im_e = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].re;

    /* Gain: '<S133>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2[3].re = re_e;
    basic_cosimulation_c_1_sm_master_B.alpha2[3].im = im_e;

    /* Sum: '<S133>/Add' incorporates:
     *  Gain: '<S133>/alpha'
     *  Gain: '<S133>/alpha2'
     *  RealImagToComplex: '<S135>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_a[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].re +
       basic_cosimulation_c_1_sm_master_B.alpha2[0].re) +
      basic_cosimulation_c_1_sm_master_B.alpha[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_a[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].im +
       basic_cosimulation_c_1_sm_master_B.alpha2[0].im) +
      basic_cosimulation_c_1_sm_master_B.alpha[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_a[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].re +
       basic_cosimulation_c_1_sm_master_B.alpha2[1].re) +
      basic_cosimulation_c_1_sm_master_B.alpha[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_a[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].im +
       basic_cosimulation_c_1_sm_master_B.alpha2[1].im) +
      basic_cosimulation_c_1_sm_master_B.alpha[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_a[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].re +
       basic_cosimulation_c_1_sm_master_B.alpha2[2].re) +
      basic_cosimulation_c_1_sm_master_B.alpha[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_a[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].im +
       basic_cosimulation_c_1_sm_master_B.alpha2[2].im) +
      basic_cosimulation_c_1_sm_master_B.alpha[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_a[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].re +
       basic_cosimulation_c_1_sm_master_B.alpha2[3].re) +
      basic_cosimulation_c_1_sm_master_B.alpha[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_a[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].im +
       basic_cosimulation_c_1_sm_master_B.alpha2[3].im) +
      basic_cosimulation_c_1_sm_master_B.alpha[3].im;

    /* Gain: '<S133>/Gain1' incorporates:
     *  Sum: '<S133>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_h[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Add_a[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Add_a[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Add_a[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Add_a[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Add_a[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Add_a[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Add_a[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Add_a[3].im;

    /* Math: '<S132>/Math Function1' incorporates:
     *  Gain: '<S133>/Gain1'
     */
    basic_cosimulation_c_1_sm_master_B.MathFunction1[0].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_h[0].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[0].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_h[0].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[1].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_h[1].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[1].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_h[1].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[2].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_h[2].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[2].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_h[2].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[3].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_h[3].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[3].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_h[3].im;

    /* Gain: '<S134>/alpha' incorporates:
     *  RealImagToComplex: '<S140>/Real-Imag to Complex'
     */
    re_h = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im;
    im_h = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re;

    /* Gain: '<S134>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_f[0].re = re_h;
    basic_cosimulation_c_1_sm_master_B.alpha_f[0].im = im_h;

    /* Gain: '<S134>/alpha' incorporates:
     *  RealImagToComplex: '<S140>/Real-Imag to Complex'
     */
    re_k = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im;
    im_k = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re;

    /* Gain: '<S134>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_f[1].re = re_k;
    basic_cosimulation_c_1_sm_master_B.alpha_f[1].im = im_k;

    /* Gain: '<S134>/alpha' incorporates:
     *  RealImagToComplex: '<S140>/Real-Imag to Complex'
     */
    re_n = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im;
    im_n = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re;

    /* Gain: '<S134>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_f[2].re = re_n;
    basic_cosimulation_c_1_sm_master_B.alpha_f[2].im = im_n;

    /* Gain: '<S134>/alpha' incorporates:
     *  RealImagToComplex: '<S140>/Real-Imag to Complex'
     */
    re_q = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im;
    im_q = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i1.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re;

    /* Gain: '<S134>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_f[3].re = re_q;
    basic_cosimulation_c_1_sm_master_B.alpha_f[3].im = im_q;

    /* Gain: '<S134>/alpha2' incorporates:
     *  RealImagToComplex: '<S138>/Real-Imag to Complex'
     */
    re_s = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].im;
    im_s = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].re;

    /* Gain: '<S134>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_p[0].re = re_s;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[0].im = im_s;

    /* Gain: '<S134>/alpha2' incorporates:
     *  RealImagToComplex: '<S138>/Real-Imag to Complex'
     */
    re_u = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].im;
    im_u = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].re;

    /* Gain: '<S134>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_p[1].re = re_u;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[1].im = im_u;

    /* Gain: '<S134>/alpha2' incorporates:
     *  RealImagToComplex: '<S138>/Real-Imag to Complex'
     */
    re_w = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].im;
    im_w = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].re;

    /* Gain: '<S134>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_p[2].re = re_w;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[2].im = im_w;

    /* Gain: '<S134>/alpha2' incorporates:
     *  RealImagToComplex: '<S138>/Real-Imag to Complex'
     */
    re_y = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].im;
    im_y = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].re;

    /* Gain: '<S134>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_p[3].re = re_y;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[3].im = im_y;

    /* Sum: '<S134>/Add' incorporates:
     *  Gain: '<S134>/alpha'
     *  Gain: '<S134>/alpha2'
     *  RealImagToComplex: '<S136>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_g[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_p[0].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_g[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_p[0].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_g[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_p[1].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_g[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_p[1].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_g[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_p[2].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_g[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_p[2].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_g[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_p[3].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_g[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_p[3].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[3].im;

    /* Gain: '<S134>/Gain1' incorporates:
     *  Sum: '<S134>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_m[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_g[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_g[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_g[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_g[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_g[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_g[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_g[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_g[3].im;

    /* Product: '<S132>/Product1' incorporates:
     *  Gain: '<S134>/Gain1'
     *  Math: '<S132>/Math Function1'
     */
    re_10 = basic_cosimulation_c_1_sm_master_B.Gain1_m[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[0].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_m[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[0].im;
    im_10 = basic_cosimulation_c_1_sm_master_B.Gain1_m[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[0].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_m[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[0].re;

    /* Product: '<S132>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1[0].re = re_10;
    basic_cosimulation_c_1_sm_master_B.Product1[0].im = im_10;

    /* Product: '<S132>/Product1' incorporates:
     *  Gain: '<S134>/Gain1'
     *  Math: '<S132>/Math Function1'
     */
    re_12 = basic_cosimulation_c_1_sm_master_B.Gain1_m[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[1].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_m[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[1].im;
    im_12 = basic_cosimulation_c_1_sm_master_B.Gain1_m[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[1].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_m[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[1].re;

    /* Product: '<S132>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1[1].re = re_12;
    basic_cosimulation_c_1_sm_master_B.Product1[1].im = im_12;

    /* Product: '<S132>/Product1' incorporates:
     *  Gain: '<S134>/Gain1'
     *  Math: '<S132>/Math Function1'
     */
    re_14 = basic_cosimulation_c_1_sm_master_B.Gain1_m[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[2].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_m[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[2].im;
    im_14 = basic_cosimulation_c_1_sm_master_B.Gain1_m[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[2].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_m[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[2].re;

    /* Product: '<S132>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1[2].re = re_14;
    basic_cosimulation_c_1_sm_master_B.Product1[2].im = im_14;

    /* Product: '<S132>/Product1' incorporates:
     *  Gain: '<S134>/Gain1'
     *  Math: '<S132>/Math Function1'
     */
    re_16 = basic_cosimulation_c_1_sm_master_B.Gain1_m[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[3].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_m[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[3].im;
    im_16 = basic_cosimulation_c_1_sm_master_B.Gain1_m[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[3].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_m[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1[3].re;

    /* Product: '<S132>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1[3].re = re_16;
    basic_cosimulation_c_1_sm_master_B.Product1[3].im = im_16;

    /* Gain: '<S132>/Gain1' incorporates:
     *  Product: '<S132>/Product1'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_md[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1[3].im;

    /* MultiPortSwitch: '<S132>/Multiport Switch' incorporates:
     *  Gain: '<S132>/Gain1'
     */
    memcpy(&basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[0],
           &basic_cosimulation_c_1_sm_master_B.Gain1_md[0], 4U * sizeof(creal_T));
    break;

   default:
    /* Sum: '<S144>/Add' incorporates:
     *  RealImagToComplex: '<S135>/Real-Imag to Complex'
     *  RealImagToComplex: '<S137>/Real-Imag to Complex'
     *  RealImagToComplex: '<S139>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].re;
    basic_cosimulation_c_1_sm_master_B.Add[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].im;
    basic_cosimulation_c_1_sm_master_B.Add[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].re;
    basic_cosimulation_c_1_sm_master_B.Add[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].im;
    basic_cosimulation_c_1_sm_master_B.Add[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].re;
    basic_cosimulation_c_1_sm_master_B.Add[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].im;
    basic_cosimulation_c_1_sm_master_B.Add[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].re;
    basic_cosimulation_c_1_sm_master_B.Add[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].im;

    /* Gain: '<S144>/Gain1' incorporates:
     *  Sum: '<S144>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add[3].im;

    /* Math: '<S132>/Math Function2' incorporates:
     *  Gain: '<S144>/Gain1'
     */
    basic_cosimulation_c_1_sm_master_B.MathFunction2[0].re =
      basic_cosimulation_c_1_sm_master_B.Gain1[0].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[0].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1[0].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[1].re =
      basic_cosimulation_c_1_sm_master_B.Gain1[1].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[1].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1[1].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[2].re =
      basic_cosimulation_c_1_sm_master_B.Gain1[2].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[2].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1[2].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[3].re =
      basic_cosimulation_c_1_sm_master_B.Gain1[3].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[3].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1[3].im;

    /* Sum: '<S145>/Add' incorporates:
     *  RealImagToComplex: '<S136>/Real-Imag to Complex'
     *  RealImagToComplex: '<S138>/Real-Imag to Complex'
     *  RealImagToComplex: '<S140>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_f[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_f[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_f[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_f[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_f[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_f[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_f[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_f[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im;

    /* Gain: '<S145>/Gain1' incorporates:
     *  Sum: '<S145>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_b[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[3].im;

    /* Product: '<S132>/Product2' incorporates:
     *  Gain: '<S145>/Gain1'
     *  Math: '<S132>/Math Function2'
     */
    re_f = basic_cosimulation_c_1_sm_master_B.Gain1_b[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[0].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_b[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[0].im;
    im_f = basic_cosimulation_c_1_sm_master_B.Gain1_b[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[0].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_b[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[0].re;

    /* Product: '<S132>/Product2' */
    basic_cosimulation_c_1_sm_master_B.Product2[0].re = re_f;
    basic_cosimulation_c_1_sm_master_B.Product2[0].im = im_f;

    /* Product: '<S132>/Product2' incorporates:
     *  Gain: '<S145>/Gain1'
     *  Math: '<S132>/Math Function2'
     */
    re_i = basic_cosimulation_c_1_sm_master_B.Gain1_b[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[1].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_b[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[1].im;
    im_i = basic_cosimulation_c_1_sm_master_B.Gain1_b[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[1].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_b[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[1].re;

    /* Product: '<S132>/Product2' */
    basic_cosimulation_c_1_sm_master_B.Product2[1].re = re_i;
    basic_cosimulation_c_1_sm_master_B.Product2[1].im = im_i;

    /* Product: '<S132>/Product2' incorporates:
     *  Gain: '<S145>/Gain1'
     *  Math: '<S132>/Math Function2'
     */
    re_l = basic_cosimulation_c_1_sm_master_B.Gain1_b[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[2].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_b[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[2].im;
    im_l = basic_cosimulation_c_1_sm_master_B.Gain1_b[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[2].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_b[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[2].re;

    /* Product: '<S132>/Product2' */
    basic_cosimulation_c_1_sm_master_B.Product2[2].re = re_l;
    basic_cosimulation_c_1_sm_master_B.Product2[2].im = im_l;

    /* Product: '<S132>/Product2' incorporates:
     *  Gain: '<S145>/Gain1'
     *  Math: '<S132>/Math Function2'
     */
    re_o = basic_cosimulation_c_1_sm_master_B.Gain1_b[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[3].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_b[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[3].im;
    im_o = basic_cosimulation_c_1_sm_master_B.Gain1_b[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[3].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_b[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2[3].re;

    /* Product: '<S132>/Product2' */
    basic_cosimulation_c_1_sm_master_B.Product2[3].re = re_o;
    basic_cosimulation_c_1_sm_master_B.Product2[3].im = im_o;

    /* Gain: '<S132>/Gain3' incorporates:
     *  Product: '<S132>/Product2'
     */
    basic_cosimulation_c_1_sm_master_B.Gain3[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_g *
      basic_cosimulation_c_1_sm_master_B.Product2[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain3[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_g *
      basic_cosimulation_c_1_sm_master_B.Product2[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain3[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_g *
      basic_cosimulation_c_1_sm_master_B.Product2[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain3[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_g *
      basic_cosimulation_c_1_sm_master_B.Product2[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain3[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_g *
      basic_cosimulation_c_1_sm_master_B.Product2[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain3[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_g *
      basic_cosimulation_c_1_sm_master_B.Product2[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain3[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_g *
      basic_cosimulation_c_1_sm_master_B.Product2[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain3[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_g *
      basic_cosimulation_c_1_sm_master_B.Product2[3].im;

    /* MultiPortSwitch: '<S132>/Multiport Switch' incorporates:
     *  Gain: '<S132>/Gain3'
     */
    memcpy(&basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[0],
           &basic_cosimulation_c_1_sm_master_B.Gain3[0], 4U * sizeof(creal_T));
    break;
  }

  /* End of MultiPortSwitch: '<S132>/Multiport Switch' */

  /* ComplexToRealImag: '<S132>/Complex to Real-Imag' incorporates:
   *  MultiPortSwitch: '<S132>/Multiport Switch'
   */
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_b[0] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[0].re;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_b[1] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[1].re;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_b[2] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[2].re;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_b[3] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[3].re;

  /* ComplexToRealImag: '<S132>/Complex to Real-Imag' incorporates:
   *  MultiPortSwitch: '<S132>/Multiport Switch'
   */
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_i[0] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[0].im;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_i[1] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[1].im;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_i[2] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[2].im;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_i[3] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[3].im;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* DiscreteIntegrator: '<S124>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator_b =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_h;

    /* DiscreteIntegrator: '<S124>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1_p =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_m;

    /* Gain: '<S128>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_lp =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_gy *
      basic_cosimulation_c_1_sm_master_B.SFunction_i[1];

    /* Switch: '<S128>/Switch' */
    if (basic_cosimulation_c_1_sm_master_B.SFunction_i[0] >
        basic_cosimulation_c_1_sm_master_P.Switch_Threshold_d) {
      /* Switch: '<S128>/Switch' */
      basic_cosimulation_c_1_sm_master_B.Switch_e =
        basic_cosimulation_c_1_sm_master_B.Delay_iu[0];
    } else {
      /* Switch: '<S128>/Switch' incorporates:
       *  Constant: '<S128>/Constant'
       */
      basic_cosimulation_c_1_sm_master_B.Switch_e =
        basic_cosimulation_c_1_sm_master_P.Vnom_peak;
    }

    /* End of Switch: '<S128>/Switch' */

    /* Gain: '<S128>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_hn =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_o3 *
      basic_cosimulation_c_1_sm_master_B.Switch_e;

    /* Product: '<S128>/Divide' */
    basic_cosimulation_c_1_sm_master_B.Divide_g =
      basic_cosimulation_c_1_sm_master_B.Gain1_lp /
      basic_cosimulation_c_1_sm_master_B.Gain_hn;

    /* Saturate: '<S124>/Saturation' */
    cosOut = -1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    Bias = 1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    if (basic_cosimulation_c_1_sm_master_B.Divide_g > Bias) {
      /* Saturate: '<S124>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_n = Bias;
    } else if (basic_cosimulation_c_1_sm_master_B.Divide_g < cosOut) {
      /* Saturate: '<S124>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_n = cosOut;
    } else {
      /* Saturate: '<S124>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_n =
        basic_cosimulation_c_1_sm_master_B.Divide_g;
    }

    /* End of Saturate: '<S124>/Saturation' */

    /* Sum: '<S124>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_cj =
      basic_cosimulation_c_1_sm_master_B.Saturation_n -
      basic_cosimulation_c_1_sm_master_B.Delay_f[0];

    /* Product: '<S124>/Product' */
    basic_cosimulation_c_1_sm_master_B.Product_o0 =
      basic_cosimulation_c_1_sm_master_B.Subtract_cj *
      basic_cosimulation_c_1_sm_master_B.SFunction_i[0];

    /* Gain: '<S124>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_c =
      basic_cosimulation_c_1_sm_master_P.kp_current *
      basic_cosimulation_c_1_sm_master_B.Product_o0;

    /* Sum: '<S124>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_g =
      basic_cosimulation_c_1_sm_master_B.Gain1_c +
      basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator_b;

    /* Gain: '<S124>/Gain5' */
    basic_cosimulation_c_1_sm_master_B.Gain5_j =
      basic_cosimulation_c_1_sm_master_P.decouple *
      basic_cosimulation_c_1_sm_master_B.Delay_f[1];

    /* Sum: '<S124>/Sum3' */
    basic_cosimulation_c_1_sm_master_B.Sum3_l =
      (basic_cosimulation_c_1_sm_master_B.Delay_iu[0] +
       basic_cosimulation_c_1_sm_master_B.Sum1_g) -
      basic_cosimulation_c_1_sm_master_B.Gain5_j;

    /* Product: '<S124>/Divide' incorporates:
     *  Constant: '<S124>/Constant2'
     */
    cosOut = basic_cosimulation_c_1_sm_master_P.Vdc / 2.0;

    /* Product: '<S124>/Divide' */
    basic_cosimulation_c_1_sm_master_B.Divide_c =
      basic_cosimulation_c_1_sm_master_B.Sum3_l / cosOut;

    /* Gain: '<S124>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_h5 =
      basic_cosimulation_c_1_sm_master_P.decouple *
      basic_cosimulation_c_1_sm_master_B.Delay_f[0];

    /* Gain: '<S128>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_ez =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_az *
      basic_cosimulation_c_1_sm_master_B.SFunction_i[2];

    /* Gain: '<S128>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_et =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_fz *
      basic_cosimulation_c_1_sm_master_B.Switch_e;

    /* Product: '<S128>/Divide1' */
    basic_cosimulation_c_1_sm_master_B.Divide1_d =
      basic_cosimulation_c_1_sm_master_B.Gain3_ez /
      basic_cosimulation_c_1_sm_master_B.Gain2_et;

    /* Saturate: '<S124>/Saturation1' */
    cosOut = -1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    Bias = 1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    if (basic_cosimulation_c_1_sm_master_B.Divide1_d > Bias) {
      /* Saturate: '<S124>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1_k = Bias;
    } else if (basic_cosimulation_c_1_sm_master_B.Divide1_d < cosOut) {
      /* Saturate: '<S124>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1_k = cosOut;
    } else {
      /* Saturate: '<S124>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1_k =
        basic_cosimulation_c_1_sm_master_B.Divide1_d;
    }

    /* End of Saturate: '<S124>/Saturation1' */

    /* Sum: '<S124>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_d =
      basic_cosimulation_c_1_sm_master_B.Saturation1_k -
      basic_cosimulation_c_1_sm_master_B.Delay_f[1];

    /* Product: '<S124>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_f =
      basic_cosimulation_c_1_sm_master_B.SFunction_i[0] *
      basic_cosimulation_c_1_sm_master_B.Subtract1_d;

    /* Gain: '<S124>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_ko =
      basic_cosimulation_c_1_sm_master_P.kp_current *
      basic_cosimulation_c_1_sm_master_B.Product1_f;

    /* Sum: '<S124>/Sum2' */
    basic_cosimulation_c_1_sm_master_B.Sum2_pd =
      basic_cosimulation_c_1_sm_master_B.Gain3_ko +
      basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1_p;

    /* Sum: '<S124>/Sum4' */
    basic_cosimulation_c_1_sm_master_B.Sum4_m =
      (basic_cosimulation_c_1_sm_master_B.Gain6_h5 +
       basic_cosimulation_c_1_sm_master_B.Sum2_pd) +
      basic_cosimulation_c_1_sm_master_B.Delay_iu[1];

    /* Product: '<S124>/Divide1' incorporates:
     *  Constant: '<S124>/Constant2'
     */
    cosOut = basic_cosimulation_c_1_sm_master_P.Vdc / 2.0;

    /* Product: '<S124>/Divide1' */
    basic_cosimulation_c_1_sm_master_B.Divide1_h =
      basic_cosimulation_c_1_sm_master_B.Sum4_m / cosOut;

    /* Gain: '<S124>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_n =
      basic_cosimulation_c_1_sm_master_P.ki_current *
      basic_cosimulation_c_1_sm_master_B.Product_o0;

    /* Gain: '<S124>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4_m =
      basic_cosimulation_c_1_sm_master_P.ki_current *
      basic_cosimulation_c_1_sm_master_B.Product1_f;

    /* SignalConversion generated from: '<S124>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[0] =
      basic_cosimulation_c_1_sm_master_B.Divide_c;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[1] =
      basic_cosimulation_c_1_sm_master_B.Divide1_h;
  }

  /* Trigonometry: '<S126>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_ln = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Sum: '<S126>/Sum' incorporates:
   *  Constant: '<S126>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.Sum_n =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant_Value_i;

  /* Trigonometry: '<S126>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_m = cos
    (basic_cosimulation_c_1_sm_master_B.Sum_n);

  /* Sum: '<S126>/Sum2' incorporates:
   *  Constant: '<S126>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_c =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_i;

  /* Trigonometry: '<S126>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_k = cos
    (basic_cosimulation_c_1_sm_master_B.Sum2_c);

  /* Trigonometry: '<S126>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_iw = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Sum: '<S126>/Sum3' incorporates:
   *  Constant: '<S126>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum3_mo =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_p;

  /* Trigonometry: '<S126>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_l = sin
    (basic_cosimulation_c_1_sm_master_B.Sum3_mo);

  /* Sum: '<S126>/Sum5' incorporates:
   *  Constant: '<S126>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum5_f =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_hr;

  /* Trigonometry: '<S126>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_g = sin
    (basic_cosimulation_c_1_sm_master_B.Sum5_f);

  /* Product: '<S126>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_n =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.Cos_ln;

  /* Product: '<S126>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_j =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.Cos1_m;

  /* Product: '<S126>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_g1 =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.Cos2_k;

  /* Sum: '<S126>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_gm =
    (basic_cosimulation_c_1_sm_master_B.Product_n +
     basic_cosimulation_c_1_sm_master_B.Product1_j) +
    basic_cosimulation_c_1_sm_master_B.Product2_g1;

  /* Gain: '<S126>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_nq =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_h *
    basic_cosimulation_c_1_sm_master_B.Sum1_gm;

  /* Product: '<S126>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_e =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.Cos3_iw;

  /* Product: '<S126>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_b =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.Cos4_l;

  /* Product: '<S126>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_l =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.Cos5_g;

  /* Sum: '<S126>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_c =
    (basic_cosimulation_c_1_sm_master_B.Product3_e +
     basic_cosimulation_c_1_sm_master_B.Product4_b) +
    basic_cosimulation_c_1_sm_master_B.Product5_l;

  /* Gain: '<S126>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_ft =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_oj *
    basic_cosimulation_c_1_sm_master_B.Sum4_c;

  /* Trigonometry: '<S127>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_a = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Sum: '<S127>/Sum' incorporates:
   *  Constant: '<S127>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.Sum_f =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant_Value_d;

  /* Trigonometry: '<S127>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_c = cos
    (basic_cosimulation_c_1_sm_master_B.Sum_f);

  /* Sum: '<S127>/Sum2' incorporates:
   *  Constant: '<S127>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_j =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_gs;

  /* Trigonometry: '<S127>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_j = cos
    (basic_cosimulation_c_1_sm_master_B.Sum2_j);

  /* Trigonometry: '<S127>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_p = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Sum: '<S127>/Sum3' incorporates:
   *  Constant: '<S127>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum3_o =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_d;

  /* Trigonometry: '<S127>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_e = sin
    (basic_cosimulation_c_1_sm_master_B.Sum3_o);

  /* Sum: '<S127>/Sum5' incorporates:
   *  Constant: '<S127>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum5_k =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_e;

  /* Trigonometry: '<S127>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_e = sin
    (basic_cosimulation_c_1_sm_master_B.Sum5_k);

  /* Product: '<S127>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_i =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.Cos_a;

  /* Product: '<S127>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_c2 =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.Cos1_c;

  /* Product: '<S127>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_jv =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.Cos2_j;

  /* Sum: '<S127>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_ju =
    (basic_cosimulation_c_1_sm_master_B.Product_i +
     basic_cosimulation_c_1_sm_master_B.Product1_c2) +
    basic_cosimulation_c_1_sm_master_B.Product2_jv;

  /* Gain: '<S127>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_e4 =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_g *
    basic_cosimulation_c_1_sm_master_B.Sum1_ju;

  /* Product: '<S127>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_g =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.Cos3_p;

  /* Product: '<S127>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_d =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.Cos4_e;

  /* Product: '<S127>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_c =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.Cos5_e;

  /* Sum: '<S127>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_k =
    (basic_cosimulation_c_1_sm_master_B.Product3_g +
     basic_cosimulation_c_1_sm_master_B.Product4_d) +
    basic_cosimulation_c_1_sm_master_B.Product5_c;

  /* Gain: '<S127>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_ka =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a0v *
    basic_cosimulation_c_1_sm_master_B.Sum4_k;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* SignalConversion generated from: '<S126>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[0] =
      basic_cosimulation_c_1_sm_master_B.Gain_nq;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[1] =
      basic_cosimulation_c_1_sm_master_B.Gain1_ft;

    /* SignalConversion generated from: '<S127>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[0] =
      basic_cosimulation_c_1_sm_master_B.Gain_e4;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[1] =
      basic_cosimulation_c_1_sm_master_B.Gain1_ka;
  }

  /* Integrator: '<S147>/Integrator' */
  basic_cosimulation_c_1_sm_master_B.Integrator_i[0] =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[0];
  basic_cosimulation_c_1_sm_master_B.Integrator_i[1] =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[1];
  basic_cosimulation_c_1_sm_master_B.Integrator_i[2] =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[2];
  basic_cosimulation_c_1_sm_master_B.Integrator_i[3] =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[3];

  /* Trigonometry: '<S143>/Trigonometric Function1' */
  for (i = 0; i < 4; i = i + 1) {
    cosOut = basic_cosimulation_c_1_sm_master_B.Integrator_i[i];
    Bias = sin(cosOut);
    cosOut = cos(cosOut);
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[i] = Bias;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[i] = cosOut;
  }

  /* End of Trigonometry: '<S143>/Trigonometric Function1' */

  /* Product: '<S135>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_er[0] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[0];
  basic_cosimulation_c_1_sm_master_B.Product1_er[1] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[1];
  basic_cosimulation_c_1_sm_master_B.Product1_er[2] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[2];
  basic_cosimulation_c_1_sm_master_B.Product1_er[3] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[3];
  basic_cosimulation_c_1_sm_master_B.Product1_er[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[0];
  basic_cosimulation_c_1_sm_master_B.Product1_er[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[1];
  basic_cosimulation_c_1_sm_master_B.Product1_er[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[2];
  basic_cosimulation_c_1_sm_master_B.Product1_er[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[3];

  /* Product: '<S136>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_kj[0] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[0];
  basic_cosimulation_c_1_sm_master_B.Product1_kj[1] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[1];
  basic_cosimulation_c_1_sm_master_B.Product1_kj[2] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[2];
  basic_cosimulation_c_1_sm_master_B.Product1_kj[3] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[3];
  basic_cosimulation_c_1_sm_master_B.Product1_kj[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[0];
  basic_cosimulation_c_1_sm_master_B.Product1_kj[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[1];
  basic_cosimulation_c_1_sm_master_B.Product1_kj[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[2];
  basic_cosimulation_c_1_sm_master_B.Product1_kj[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[3];

  /* Product: '<S137>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_ew[0] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[0];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[1] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[1];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[2] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[2];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[3] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[3];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[0];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[1];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[2];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[3];

  /* Product: '<S138>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_b[0] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[0];
  basic_cosimulation_c_1_sm_master_B.Product1_b[1] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[1];
  basic_cosimulation_c_1_sm_master_B.Product1_b[2] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[2];
  basic_cosimulation_c_1_sm_master_B.Product1_b[3] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[3];
  basic_cosimulation_c_1_sm_master_B.Product1_b[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[0];
  basic_cosimulation_c_1_sm_master_B.Product1_b[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[1];
  basic_cosimulation_c_1_sm_master_B.Product1_b[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[2];
  basic_cosimulation_c_1_sm_master_B.Product1_b[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[3];

  /* Product: '<S139>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_bw[0] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[0];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[1] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[1];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[2] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[2];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[3] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[3];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[0];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[1];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[2];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[3];

  /* Product: '<S140>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_l0[0] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[0];
  basic_cosimulation_c_1_sm_master_B.Product1_l0[1] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[1];
  basic_cosimulation_c_1_sm_master_B.Product1_l0[2] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[2];
  basic_cosimulation_c_1_sm_master_B.Product1_l0[3] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[3];
  basic_cosimulation_c_1_sm_master_B.Product1_l0[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[0];
  basic_cosimulation_c_1_sm_master_B.Product1_l0[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[1];
  basic_cosimulation_c_1_sm_master_B.Product1_l0[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[2];
  basic_cosimulation_c_1_sm_master_B.Product1_l0[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[3];
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* DataTypeConversion: '<S147>/Data Type Conversion' incorporates:
     *  Constant: '<S143>/Radial frequencies'
     */
    Bias = 2.0 * 3.1415926535897931 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_f;
    b_idx_0 = basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K_j
      [0];
    b_idx_1 = basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K_j
      [1];
    b_idx_2 = basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K_j
      [2];
    b_idx_3 = basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K_j
      [3];
    b = b_idx_0;
    b = Bias * b;
    b_idx_0 = b;
    b = b_idx_1;
    b = Bias * b;
    b_idx_1 = b;
    b = b_idx_2;
    b = Bias * b;
    b_idx_2 = b;
    b = b_idx_3;
    b = Bias * b;
    b_idx_3 = b;

    /* DataTypeConversion: '<S147>/Data Type Conversion' incorporates:
     *  Constant: '<S143>/Radial frequencies'
     */
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[0] = b_idx_0;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[1] = b_idx_1;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[2] = b_idx_2;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[3] = b_idx_3;

    /* Gain: '<S130>/Gain' */
    cosOut = 1.0 / basic_cosimulation_c_1_sm_master_P.Vnom_peak;

    /* Gain: '<S130>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_oh[0] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain7_b;
    basic_cosimulation_c_1_sm_master_B.Gain_oh[1] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain8_i;
    basic_cosimulation_c_1_sm_master_B.Gain_oh[2] = cosOut *
      basic_cosimulation_c_1_sm_master_B.Gain9_n;

    /* Gain: '<S130>/Gain5' */
    basic_cosimulation_c_1_sm_master_B.Gain5_o =
      basic_cosimulation_c_1_sm_master_P.Gain5_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Gain_oh[1];

    /* Gain: '<S130>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_k =
      basic_cosimulation_c_1_sm_master_P.Gain6_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Gain_oh[2];

    /* Sum: '<S130>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_f =
      basic_cosimulation_c_1_sm_master_B.Gain5_o +
      basic_cosimulation_c_1_sm_master_B.Gain6_k;

    /* Gain: '<S130>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4_c =
      basic_cosimulation_c_1_sm_master_P.Gain4_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Sum1_f;
  }

  /* Integrator: '<S130>/Integrator' */
  basic_cosimulation_c_1_sm_master_B.Integrator_g5 =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_hp;

  /* Trigonometry: '<S130>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_j = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Product: '<S130>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_c =
    basic_cosimulation_c_1_sm_master_B.Gain4_c *
    basic_cosimulation_c_1_sm_master_B.Cos_j;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Gain: '<S130>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_a =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_p *
      basic_cosimulation_c_1_sm_master_B.Gain_oh[1];

    /* Gain: '<S130>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_m =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Gain_oh[2];

    /* Sum: '<S130>/Sum' */
    basic_cosimulation_c_1_sm_master_B.Sum_mo =
      (basic_cosimulation_c_1_sm_master_B.Gain_oh[0] +
       basic_cosimulation_c_1_sm_master_B.Gain2_a) +
      basic_cosimulation_c_1_sm_master_B.Gain3_m;

    /* Gain: '<S130>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_il =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_jm *
      basic_cosimulation_c_1_sm_master_B.Sum_mo;
  }

  /* Trigonometry: '<S130>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_h = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Product: '<S130>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_g =
    basic_cosimulation_c_1_sm_master_B.Gain1_il *
    basic_cosimulation_c_1_sm_master_B.Cos1_h;

  /* Sum: '<S130>/Sum3' */
  basic_cosimulation_c_1_sm_master_B.Sum3_e =
    basic_cosimulation_c_1_sm_master_B.Product_c -
    basic_cosimulation_c_1_sm_master_B.Product1_g;

  /* Gain: '<S130>/Gain7' */
  basic_cosimulation_c_1_sm_master_B.Gain7_n =
    basic_cosimulation_c_1_sm_master_P.Gain7_Gain_dd *
    basic_cosimulation_c_1_sm_master_B.Sum3_e;

  /* Sum: '<S130>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_b =
    basic_cosimulation_c_1_sm_master_B.Integrator_g5 +
    basic_cosimulation_c_1_sm_master_B.Gain7_n;

  /* Sum: '<S130>/Sum5' incorporates:
   *  Constant: '<S130>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.W_a =
    basic_cosimulation_c_1_sm_master_B.Sum4_b +
    basic_cosimulation_c_1_sm_master_P.Constant_Value_px;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
  }

  /* Trigonometry: '<S130>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_lc = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Trigonometry: '<S130>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_n = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
  }

  /* Product: '<S130>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_i =
    basic_cosimulation_c_1_sm_master_B.Gain4_c *
    basic_cosimulation_c_1_sm_master_B.Cos2_lc;

  /* Product: '<S130>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_fy =
    basic_cosimulation_c_1_sm_master_B.Gain1_il *
    basic_cosimulation_c_1_sm_master_B.Cos3_n;

  /* Sum: '<S130>/Sum2' */
  basic_cosimulation_c_1_sm_master_B.Sum2_g =
    basic_cosimulation_c_1_sm_master_B.Product2_i +
    basic_cosimulation_c_1_sm_master_B.Product3_fy;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Gain: '<S102>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_ge =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_j *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[11];

    /* Gain: '<S103>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c5 =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_hj *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[12];

    /* Gain: '<S104>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_nq =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_l *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[13];

    /* SignalConversion generated from: '<S8>/Delay2' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[0] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_ge;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[1] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c5;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[2] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_nq;

    /* Gain: '<S110>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_he =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_je *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[6];

    /* Gain: '<S111>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_i =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_d *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[7];

    /* Gain: '<S105>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_lx =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_nc *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[14];

    /* DiscretePulseGenerator: '<S3>/data ready 2.5 kHz' */
    basic_cosimulation_c_1_sm_master_B.dataready25kHz = ((real_T)
      basic_cosimulation_c_1_sm_master_DW.clockTickCounter <
      basic_cosimulation_c_1_sm_master_P.dataready25kHz_Duty) &&
      (basic_cosimulation_c_1_sm_master_DW.clockTickCounter >= 0) ?
      basic_cosimulation_c_1_sm_master_P.dataready25kHz_Amp : 0.0;

    /* DiscretePulseGenerator: '<S3>/data ready 2.5 kHz' */
    if ((real_T)basic_cosimulation_c_1_sm_master_DW.clockTickCounter >=
        basic_cosimulation_c_1_sm_master_P.dataready25kHz_Period - (real_T)1) {
      basic_cosimulation_c_1_sm_master_DW.clockTickCounter = 0;
    } else {
      basic_cosimulation_c_1_sm_master_DW.clockTickCounter =
        basic_cosimulation_c_1_sm_master_DW.clockTickCounter + 1;
    }

    /* S-Function (sfun_send_async): '<S14>/S-Function2' */

    /* Level2 S-Function Block: '<S14>/S-Function2' (sfun_send_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[9];
      sfcnOutputs(rts,0);
    }

    /* S-Function (sfun_gen_async_ctrl): '<S3>/OpIPSocketCtrl1' */

    /* Level2 S-Function Block: '<S3>/OpIPSocketCtrl1' (sfun_gen_async_ctrl) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[10];
      sfcnOutputs(rts,0);
    }
  }
}

/* Model update function */
void basic_cosimulation_c_1_sm_master_update(void)
{
  real_T HoldCosine;
  real_T HoldSine;
  int32_T i;
  int32_T i_0;
  int_T idxDelay;

  /* Update for Integrator: '<S51>/Integrator1' */
  basic_cosimulation_c_1_sm_master_DW.Integrator1_IWORK = 0;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Update for Memory: '<S2>/S-Function' */
    basic_cosimulation_c_1_sm_master_DW.SFunction_PreviousInput =
      basic_cosimulation_c_1_sm_master_B.Sum;

    /* Update for UnitDelay: '<S37>/Unit Delay' */
    for (i = 0; i < 6; i = i + 1) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE[i] =
        basic_cosimulation_c_1_sm_master_B.Saturation_b[i];
    }

    /* End of Update for UnitDelay: '<S37>/Unit Delay' */

    /* Update for Delay: '<S45>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[1];

    /* Update for Enabled SubSystem: '<S37>/Tail' */
    basic_cosimulation__Tail_Update
      (basic_cosimulation_c_1_sm_master_B.DataTypeConversion,
       &basic_cosimulation_c_1_sm_master_B.Tail,
       &basic_cosimulation_c_1_sm_master_DW.Tail,
       &basic_cosimulation_c_1_sm_master_P.Tail);

    /* End of Update for SubSystem: '<S37>/Tail' */

    /* Update for Delay: '<S5>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_b =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_e;

    /* Update for Delay: '<S5>/Delay1' */
    basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_gz;

    /* Update for Delay: '<S8>/Delay2' */
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[0];
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[1];
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1 << 1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[2];

    /* Update for Delay: '<S8>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_f =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_i;

    /* Update for Delay: '<S8>/Delay1' */
    basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE_a =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_he;

    /* Update for Delay: '<S5>/Delay2' */
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[0];
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[1];
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1 << 1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[2];

    /* Update for Delay: '<S47>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[1];

    /* Update for DiscreteIntegrator: '<S45>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE +
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator_gainval *
      basic_cosimulation_c_1_sm_master_B.Gain2_k;

    /* Update for DiscreteIntegrator: '<S45>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE +
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator1_gainval *
      basic_cosimulation_c_1_sm_master_B.Gain4;

    /* Update for Delay: '<S48>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[1];

    /* Update for DiscreteIntegrator: '<S75>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval *
      basic_cosimulation_c_1_sm_master_B.Conversion;

    /* Update for DiscreteIntegrator: '<S57>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_l *
      basic_cosimulation_c_1_sm_master_B.Product1_hb[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_l *
      basic_cosimulation_c_1_sm_master_B.Product1_hb[1];

    /* Update for UnitDelay: '<S68>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[1];

    /* Update for UnitDelay: '<S68>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_d[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_d[1];

    /* Update for Delay: '<S68>/Delay' */
    for (idxDelay = 0; idxDelay < 332; idxDelay = idxDelay + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[(idxDelay << 1) + 0] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[((idxDelay + 1) << 1)
        + 0];
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[(idxDelay << 1) + 1] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[((idxDelay + 1) << 1)
        + 1];
    }

    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[(332 << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[(332 << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[1];

    /* End of Update for Delay: '<S68>/Delay' */

    /* Update for DiscreteIntegrator: '<S59>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_p = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_b *
      basic_cosimulation_c_1_sm_master_B.Product1_ku[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_b *
      basic_cosimulation_c_1_sm_master_B.Product1_ku[1];

    /* Update for UnitDelay: '<S70>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1];

    /* Update for UnitDelay: '<S70>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_lg[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_lg[1];

    /* Update for Delay: '<S70>/Delay' */
    for (idxDelay = 0; idxDelay < 332; idxDelay = idxDelay + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[(idxDelay << 1) + 0] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[((idxDelay + 1) << 1)
        + 0];
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[(idxDelay << 1) + 1] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[((idxDelay + 1) << 1)
        + 1];
    }

    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[(332 << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[(332 << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1];

    /* End of Update for Delay: '<S70>/Delay' */

    /* Update for DiscreteIntegrator: '<S61>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_i = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_d *
      basic_cosimulation_c_1_sm_master_B.Product1_e[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_d *
      basic_cosimulation_c_1_sm_master_B.Product1_e[1];

    /* Update for UnitDelay: '<S72>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[1];

    /* Update for UnitDelay: '<S72>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_k[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_k[1];

    /* Update for Delay: '<S72>/Delay' */
    for (idxDelay = 0; idxDelay < 332; idxDelay = idxDelay + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[(idxDelay << 1) + 0] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[((idxDelay + 1) << 1)
        + 0];
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[(idxDelay << 1) + 1] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[((idxDelay + 1) << 1)
        + 1];
    }

    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[(332 << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[(332 << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[1];

    /* End of Update for Delay: '<S72>/Delay' */

    /* Update for DiscreteIntegrator: '<S56>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_h = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_c *
      basic_cosimulation_c_1_sm_master_B.Product1_e1[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_c *
      basic_cosimulation_c_1_sm_master_B.Product1_e1[1];

    /* Update for UnitDelay: '<S67>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1];

    /* Update for UnitDelay: '<S67>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_in[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_in[1];

    /* Update for Delay: '<S67>/Delay' */
    for (idxDelay = 0; idxDelay < 332; idxDelay = idxDelay + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[(idxDelay << 1) + 0] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[((idxDelay + 1) << 1)
        + 0];
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[(idxDelay << 1) + 1] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[((idxDelay + 1) << 1)
        + 1];
    }

    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[(332 << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[(332 << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1];

    /* End of Update for Delay: '<S67>/Delay' */

    /* Update for DiscreteIntegrator: '<S58>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_iy = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_h *
      basic_cosimulation_c_1_sm_master_B.Product1_l[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_h *
      basic_cosimulation_c_1_sm_master_B.Product1_l[1];

    /* Update for UnitDelay: '<S69>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[1];

    /* Update for UnitDelay: '<S69>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_j[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_j[1];

    /* Update for Delay: '<S69>/Delay' */
    for (idxDelay = 0; idxDelay < 332; idxDelay = idxDelay + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[(idxDelay << 1) + 0] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[((idxDelay + 1) << 1)
        + 0];
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[(idxDelay << 1) + 1] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[((idxDelay + 1) << 1)
        + 1];
    }

    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[(332 << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[(332 << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_m[1];

    /* End of Update for Delay: '<S69>/Delay' */

    /* Update for DiscreteIntegrator: '<S60>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_a = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_i *
      basic_cosimulation_c_1_sm_master_B.Product1_k4[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_i *
      basic_cosimulation_c_1_sm_master_B.Product1_k4[1];

    /* Update for UnitDelay: '<S71>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1];

    /* Update for UnitDelay: '<S71>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_jw[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_jw[1];

    /* Update for Delay: '<S71>/Delay' */
    for (idxDelay = 0; idxDelay < 332; idxDelay = idxDelay + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[(idxDelay << 1) + 0] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[((idxDelay + 1) << 1)
        + 0];
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[(idxDelay << 1) + 1] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[((idxDelay + 1) << 1)
        + 1];
    }

    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[(332 << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[(332 << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1];

    /* End of Update for Delay: '<S71>/Delay' */

    /* Update for UnitDelay: '<S116>/Unit Delay' */
    for (i_0 = 0; i_0 < 6; i_0 = i_0 + 1) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE_g[i_0] =
        basic_cosimulation_c_1_sm_master_B.Saturation_h[i_0];
    }

    /* End of Update for UnitDelay: '<S116>/Unit Delay' */

    /* Update for Delay: '<S124>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[1];
  }

  /* Update for Integrator: '<S130>/Integrator2' */
  basic_cosimulation_c_1_sm_master_DW.Integrator2_IWORK = 0;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Update for Enabled SubSystem: '<S116>/Tail' */
    basic_cosimulation__Tail_Update
      (basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o,
       &basic_cosimulation_c_1_sm_master_B.Tail_e,
       &basic_cosimulation_c_1_sm_master_DW.Tail_e,
       &basic_cosimulation_c_1_sm_master_P.Tail_e);

    /* End of Update for SubSystem: '<S116>/Tail' */

    /* Update for Sin: '<S157>/Sine Wave A' */
    HoldSine = basic_cosimulation_c_1_sm_master_DW.lastSin;
    HoldCosine = basic_cosimulation_c_1_sm_master_DW.lastCos;
    basic_cosimulation_c_1_sm_master_DW.lastSin = HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveA_HCos + HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveA_Hsin;
    basic_cosimulation_c_1_sm_master_DW.lastCos = HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveA_HCos - HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveA_Hsin;

    /* Update for Sin: '<S157>/Sine Wave B' */
    HoldSine = basic_cosimulation_c_1_sm_master_DW.lastSin_f;
    HoldCosine = basic_cosimulation_c_1_sm_master_DW.lastCos_m;
    basic_cosimulation_c_1_sm_master_DW.lastSin_f = HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveB_HCos + HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveB_Hsin;
    basic_cosimulation_c_1_sm_master_DW.lastCos_m = HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveB_HCos - HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveB_Hsin;

    /* Update for Sin: '<S157>/Sine Wave C' */
    HoldSine = basic_cosimulation_c_1_sm_master_DW.lastSin_k;
    HoldCosine = basic_cosimulation_c_1_sm_master_DW.lastCos_i;
    basic_cosimulation_c_1_sm_master_DW.lastSin_k = HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveC_HCos + HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveC_Hsin;
    basic_cosimulation_c_1_sm_master_DW.lastCos_i = HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveC_HCos - HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveC_Hsin;

    /* Update for Delay: '<S126>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cr[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cr[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[1];

    /* Update for Delay: '<S127>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ou[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ou[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[1];
  }

  /* Update for TransportDelay: '<S136>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];

    {
      int_T i1;
      const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Integrator_cl[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.Head[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        iw_Head[i1] = ((iw_Head[i1] < (iw_CircularBufSize[i1]-1)) ? (iw_Head[i1]
          +1) : 0);
        if (iw_Head[i1] == iw_Tail[i1]) {
          iw_Tail[i1] = ((iw_Tail[i1] < (iw_CircularBufSize[i1]-1)) ?
                         (iw_Tail[i1]+1) : 0);
        }

        (*uBuffer + iw_CircularBufSize[i1])[iw_Head[i1]] = simTime;
        (*uBuffer++)[iw_Head[i1]] = u0[i1];
      }
    }
  }

  /* Update for TransportDelay: '<S138>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_b.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];

    {
      int_T i1;
      const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Integrator_e[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.Head[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        iw_Head[i1] = ((iw_Head[i1] < (iw_CircularBufSize[i1]-1)) ? (iw_Head[i1]
          +1) : 0);
        if (iw_Head[i1] == iw_Tail[i1]) {
          iw_Tail[i1] = ((iw_Tail[i1] < (iw_CircularBufSize[i1]-1)) ?
                         (iw_Tail[i1]+1) : 0);
        }

        (*uBuffer + iw_CircularBufSize[i1])[iw_Head[i1]] = simTime;
        (*uBuffer++)[iw_Head[i1]] = u0[i1];
      }
    }
  }

  /* Update for TransportDelay: '<S140>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_n.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];

    {
      int_T i1;
      const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Integrator_g[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.Head[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        iw_Head[i1] = ((iw_Head[i1] < (iw_CircularBufSize[i1]-1)) ? (iw_Head[i1]
          +1) : 0);
        if (iw_Head[i1] == iw_Tail[i1]) {
          iw_Tail[i1] = ((iw_Tail[i1] < (iw_CircularBufSize[i1]-1)) ?
                         (iw_Tail[i1]+1) : 0);
        }

        (*uBuffer + iw_CircularBufSize[i1])[iw_Head[i1]] = simTime;
        (*uBuffer++)[iw_Head[i1]] = u0[i1];
      }
    }
  }

  /* Update for TransportDelay: '<S135>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_h.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];

    {
      int_T i1;
      const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Integrator_bf[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.Head[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        iw_Head[i1] = ((iw_Head[i1] < (iw_CircularBufSize[i1]-1)) ? (iw_Head[i1]
          +1) : 0);
        if (iw_Head[i1] == iw_Tail[i1]) {
          iw_Tail[i1] = ((iw_Tail[i1] < (iw_CircularBufSize[i1]-1)) ?
                         (iw_Tail[i1]+1) : 0);
        }

        (*uBuffer + iw_CircularBufSize[i1])[iw_Head[i1]] = simTime;
        (*uBuffer++)[iw_Head[i1]] = u0[i1];
      }
    }
  }

  /* Update for TransportDelay: '<S137>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_bn.TUbufferPtrs
      [0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];

    {
      int_T i1;
      const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Integrator_j[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.Head[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        iw_Head[i1] = ((iw_Head[i1] < (iw_CircularBufSize[i1]-1)) ? (iw_Head[i1]
          +1) : 0);
        if (iw_Head[i1] == iw_Tail[i1]) {
          iw_Tail[i1] = ((iw_Tail[i1] < (iw_CircularBufSize[i1]-1)) ?
                         (iw_Tail[i1]+1) : 0);
        }

        (*uBuffer + iw_CircularBufSize[i1])[iw_Head[i1]] = simTime;
        (*uBuffer++)[iw_Head[i1]] = u0[i1];
      }
    }
  }

  /* Update for TransportDelay: '<S139>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_d.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];

    {
      int_T i1;
      const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Integrator_n[0];
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.Head[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.CircularBufSize
        [0];
      for (i1=0; i1 < 8; i1++) {
        iw_Head[i1] = ((iw_Head[i1] < (iw_CircularBufSize[i1]-1)) ? (iw_Head[i1]
          +1) : 0);
        if (iw_Head[i1] == iw_Tail[i1]) {
          iw_Tail[i1] = ((iw_Tail[i1] < (iw_CircularBufSize[i1]-1)) ?
                         (iw_Tail[i1]+1) : 0);
        }

        (*uBuffer + iw_CircularBufSize[i1])[iw_Head[i1]] = simTime;
        (*uBuffer++)[iw_Head[i1]] = u0[i1];
      }
    }
  }

  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Update for DiscreteIntegrator: '<S124>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_h =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_h +
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator_gainva_m *
      basic_cosimulation_c_1_sm_master_B.Gain2_n;

    /* Update for DiscreteIntegrator: '<S124>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_m =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_m +
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator1_gainv_i *
      basic_cosimulation_c_1_sm_master_B.Gain4_m;
  }

  /* ContTimeOutputInconsistentWithStateAtMajorOutputFlag is set, need to run a minor output */
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    if (rtsiGetContTimeOutputInconsistentWithStateAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo)) {
      rtsiSetSimTimeStep(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                         MINOR_TIME_STEP);
      rtsiSetContTimeOutputInconsistentWithStateAtMajorStep
        (&basic_cosimulation_c_1_sm_master_M->solverInfo, false);
      basic_cosimulation_c_1_sm_master_output();
      rtsiSetSimTimeStep(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                         MAJOR_TIME_STEP);
    }
  }

  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    rt_ertODEUpdateContinuousStates
      (&basic_cosimulation_c_1_sm_master_M->solverInfo);
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
  if (!(++basic_cosimulation_c_1_sm_master_M->Timing.clockTick0)) {
    ++basic_cosimulation_c_1_sm_master_M->Timing.clockTickH0;
  }

  basic_cosimulation_c_1_sm_master_M->Timing.t[0] = rtsiGetSolverStopTime
    (&basic_cosimulation_c_1_sm_master_M->solverInfo);

  {
    /* Update absolute timer for sample time: [5.0E-5s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++basic_cosimulation_c_1_sm_master_M->Timing.clockTick1)) {
      ++basic_cosimulation_c_1_sm_master_M->Timing.clockTickH1;
    }

    basic_cosimulation_c_1_sm_master_M->Timing.t[1] =
      basic_cosimulation_c_1_sm_master_M->Timing.clockTick1 *
      basic_cosimulation_c_1_sm_master_M->Timing.stepSize1 +
      basic_cosimulation_c_1_sm_master_M->Timing.clockTickH1 *
      basic_cosimulation_c_1_sm_master_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void basic_cosimulation_c_1_sm_master_derivatives(void)
{
  XDot_basic_cosimulation_c_1_sm_master_T *_rtXdot;
  boolean_T lsat;
  boolean_T usat;
  _rtXdot = ((XDot_basic_cosimulation_c_1_sm_master_T *)
             basic_cosimulation_c_1_sm_master_M->ModelData.derivs);

  /* Derivatives for Integrator: '<S51>/Integrator1' */
  lsat = (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE <=
          basic_cosimulation_c_1_sm_master_P.Integrator1_LowerSat);
  usat = (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE >=
          basic_cosimulation_c_1_sm_master_P.Integrator1_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (basic_cosimulation_c_1_sm_master_B.W >
        0.0)) || (usat && (basic_cosimulation_c_1_sm_master_B.W < 0.0))) {
    _rtXdot->Integrator1_CSTATE = basic_cosimulation_c_1_sm_master_B.W;
  } else {
    /* in saturation */
    _rtXdot->Integrator1_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S51>/Integrator1' */

  /* Derivatives for Integrator: '<S51>/Integrator' */
  _rtXdot->Integrator_CSTATE = basic_cosimulation_c_1_sm_master_B.Gain7_i;

  /* Derivatives for Integrator: '<S130>/Integrator2' */
  lsat = (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE <=
          basic_cosimulation_c_1_sm_master_P.Integrator2_LowerSat);
  usat = (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE >=
          basic_cosimulation_c_1_sm_master_P.Integrator2_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (basic_cosimulation_c_1_sm_master_B.W_a >
        0.0)) || (usat && (basic_cosimulation_c_1_sm_master_B.W_a < 0.0))) {
    _rtXdot->Integrator2_CSTATE = basic_cosimulation_c_1_sm_master_B.W_a;
  } else {
    /* in saturation */
    _rtXdot->Integrator2_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S130>/Integrator2' */

  /* Derivatives for Integrator: '<S136>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_h[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_kj[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S138>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_a[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_b[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S140>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_d[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_l0[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S135>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_g[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_er[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S137>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_m[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_ew[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S139>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_ho[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_bw[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S147>/Integrator' */
  _rtXdot->Integrator_CSTATE_b[0] =
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[0];
  _rtXdot->Integrator_CSTATE_b[1] =
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[1];
  _rtXdot->Integrator_CSTATE_b[2] =
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[2];
  _rtXdot->Integrator_CSTATE_b[3] =
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[3];

  /* Derivatives for Integrator: '<S130>/Integrator' */
  _rtXdot->Integrator_CSTATE_hp = basic_cosimulation_c_1_sm_master_B.Gain7_n;
}

/* Model initialize function */
void basic_cosimulation_c_1_sm_master_initialize(void)
{
  /* Start for Constant: '<S3>/timeout' */
  basic_cosimulation_c_1_sm_master_B.timeout =
    basic_cosimulation_c_1_sm_master_P.timeout_Value;

  /* Start for Constant: '<S26>/Constant11' */
  basic_cosimulation_c_1_sm_master_B.Constant11 =
    basic_cosimulation_c_1_sm_master_P.fsw_inv;

  /* Start for S-Function (rte_svpwm): '<S26>/RTE SPWM1' */
  /* Level2 S-Function Block: '<S26>/RTE SPWM1' (rte_svpwm) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[2];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for S-Function (rte_conversion): '<S26>/RTE Conversion1' */
  /* Level2 S-Function Block: '<S26>/RTE Conversion1' (rte_conversion) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[3];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for S-Function (ssn_sfun_v1): '<S162>/State-Space' */
  /* Level2 S-Function Block: '<S162>/State-Space' (ssn_sfun_v1) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[4];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for InitialCondition: '<S75>/Initial' */
  basic_cosimulation_c_1_sm_master_B.Initial =
    basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_a;

  /* Start for InitialCondition: '<S75>/Initial' */
  basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime = true;

  /* Start for Constant: '<S107>/Constant11' */
  basic_cosimulation_c_1_sm_master_B.Constant11_i =
    basic_cosimulation_c_1_sm_master_P.fsw_inv;

  /* Start for S-Function (rte_svpwm): '<S107>/RTE SPWM1' */
  /* Level2 S-Function Block: '<S107>/RTE SPWM1' (rte_svpwm) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[6];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for S-Function (rte_conversion): '<S107>/RTE Conversion1' */
  /* Level2 S-Function Block: '<S107>/RTE Conversion1' (rte_conversion) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[7];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for S-Function (ssn_sfun_v1): '<S163>/State-Space' */
  /* Level2 S-Function Block: '<S163>/State-Space' (ssn_sfun_v1) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[8];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for TransportDelay: '<S136>/Transport Delay' */
  {
    real_T *pBuffer =
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK.TUbufferArea[0];

    {
      int_T i1;
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK.CircularBufSize
        [0];
      void **pw_TUbufferPtrs =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK.TUbufferPtrs[0];
      for (i1=0; i1 < 8; i1++) {
        iw_Tail[i1] = 0;
        iw_Head[i1] = 0;
        iw_Last[i1] = 0;
        iw_CircularBufSize[i1] = 4096;
        pBuffer[0] =
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput;
        pBuffer[4096] = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
        pw_TUbufferPtrs[i1] = (void *) &pBuffer[0];
        pBuffer += 8192;
      }
    }
  }

  /* Start for TransportDelay: '<S138>/Transport Delay' */
  {
    real_T *pBuffer =
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_d.TUbufferArea[0];

    {
      int_T i1;
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_j.CircularBufSize
        [0];
      void **pw_TUbufferPtrs =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_b.TUbufferPtrs[
        0];
      for (i1=0; i1 < 8; i1++) {
        iw_Tail[i1] = 0;
        iw_Head[i1] = 0;
        iw_Last[i1] = 0;
        iw_CircularBufSize[i1] = 4096;
        pBuffer[0] =
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_g;
        pBuffer[4096] = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
        pw_TUbufferPtrs[i1] = (void *) &pBuffer[0];
        pBuffer += 8192;
      }
    }
  }

  /* Start for TransportDelay: '<S140>/Transport Delay' */
  {
    real_T *pBuffer =
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_a.TUbufferArea[0];

    {
      int_T i1;
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_b.CircularBufSize
        [0];
      void **pw_TUbufferPtrs =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_n.TUbufferPtrs[
        0];
      for (i1=0; i1 < 8; i1++) {
        iw_Tail[i1] = 0;
        iw_Head[i1] = 0;
        iw_Last[i1] = 0;
        iw_CircularBufSize[i1] = 4096;
        pBuffer[0] =
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_l;
        pBuffer[4096] = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
        pw_TUbufferPtrs[i1] = (void *) &pBuffer[0];
        pBuffer += 8192;
      }
    }
  }

  /* Start for TransportDelay: '<S135>/Transport Delay' */
  {
    real_T *pBuffer =
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_f.TUbufferArea[0];

    {
      int_T i1;
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_k.CircularBufSize
        [0];
      void **pw_TUbufferPtrs =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_h.TUbufferPtrs[
        0];
      for (i1=0; i1 < 8; i1++) {
        iw_Tail[i1] = 0;
        iw_Head[i1] = 0;
        iw_Last[i1] = 0;
        iw_CircularBufSize[i1] = 4096;
        pBuffer[0] =
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_h;
        pBuffer[4096] = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
        pw_TUbufferPtrs[i1] = (void *) &pBuffer[0];
        pBuffer += 8192;
      }
    }
  }

  /* Start for TransportDelay: '<S137>/Transport Delay' */
  {
    real_T *pBuffer =
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_j.TUbufferArea[0];

    {
      int_T i1;
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_l.CircularBufSize
        [0];
      void **pw_TUbufferPtrs =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_bn.TUbufferPtrs
        [0];
      for (i1=0; i1 < 8; i1++) {
        iw_Tail[i1] = 0;
        iw_Head[i1] = 0;
        iw_Last[i1] = 0;
        iw_CircularBufSize[i1] = 4096;
        pBuffer[0] =
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_i;
        pBuffer[4096] = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
        pw_TUbufferPtrs[i1] = (void *) &pBuffer[0];
        pBuffer += 8192;
      }
    }
  }

  /* Start for TransportDelay: '<S139>/Transport Delay' */
  {
    real_T *pBuffer =
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_p.TUbufferArea[0];

    {
      int_T i1;
      int_T *iw_Tail =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.Tail[0];
      int_T *iw_Head =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.Head[0];
      int_T *iw_Last =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.Last[0];
      int_T *iw_CircularBufSize =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_IWORK_a.CircularBufSize
        [0];
      void **pw_TUbufferPtrs =
        &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_d.TUbufferPtrs[
        0];
      for (i1=0; i1 < 8; i1++) {
        iw_Tail[i1] = 0;
        iw_Head[i1] = 0;
        iw_Last[i1] = 0;
        iw_CircularBufSize[i1] = 4096;
        pBuffer[0] =
          basic_cosimulation_c_1_sm_master_P.TransportDelay_InitOutput_j;
        pBuffer[4096] = basic_cosimulation_c_1_sm_master_M->Timing.t[0];
        pw_TUbufferPtrs[i1] = (void *) &pBuffer[0];
        pBuffer += 8192;
      }
    }
  }

  basic_cosimulation_c_1_sm_master_PrevZCX.Integrator1_Reset_ZCE =
    UNINITIALIZED_ZCSIG;
  basic_cosimulation_c_1_sm_master_PrevZCX.Integrator2_Reset_ZCE =
    UNINITIALIZED_ZCSIG;

  {
    int32_T Ns = 0;
    int32_T i;
    int32_T i_0;
    int32_T i_1;
    int32_T i_2;
    int32_T i_3;
    int32_T i_4;
    int32_T i_5;
    int32_T i_6;
    int32_T i_7;
    int32_T i_8;
    int32_T i_9;
    int32_T i_a;
    int32_T i_b;
    int32_T i_c;

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

    /* InitializeConditions for Memory: '<S2>/S-Function' */
    basic_cosimulation_c_1_sm_master_DW.SFunction_PreviousInput =
      basic_cosimulation_c_1_sm_master_P.SFunction_InitialCondition;

    /* InitializeConditions for S-Function (sfun_recv_async): '<S6>/S-Function1' */
    /* Level2 S-Function Block: '<S6>/S-Function1' (sfun_recv_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[0];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for UnitDelay: '<S37>/Unit Delay' */
    for (i = 0; i < 6; i = i + 1) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE[i] =
        basic_cosimulation_c_1_sm_master_P.UnitDelay_InitialCondition;
    }

    /* End of InitializeConditions for UnitDelay: '<S37>/Unit Delay' */

    /* InitializeConditions for Delay: '<S45>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition;

    /* InitializeConditions for Integrator: '<S51>/Integrator1' */
    if (rtmIsFirstInitCond(basic_cosimulation_c_1_sm_master_M)) {
      basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE = 0.0;
    }

    basic_cosimulation_c_1_sm_master_DW.Integrator1_IWORK = 1;

    /* End of InitializeConditions for Integrator: '<S51>/Integrator1' */

    /* InitializeConditions for S-Function (RECV_Param): '<S96>/S-Function' */
    /* Level2 S-Function Block: '<S96>/S-Function' (RECV_Param) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[1];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (rte_svpwm): '<S26>/RTE SPWM1' */
    /* Level2 S-Function Block: '<S26>/RTE SPWM1' (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[2];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (rte_conversion): '<S26>/RTE Conversion1' */
    /* Level2 S-Function Block: '<S26>/RTE Conversion1' (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[3];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (scomfreqoffset): '<S27>/Phase// Frequency Offset' */
    basic_cosimulation_c_1_sm_master_DW.freqTnow = 0.0;

    /* InitializeConditions for S-Function (ssn_sfun_v1): '<S162>/State-Space' */
    /* Level2 S-Function Block: '<S162>/State-Space' (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[4];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (scomfreqoffset): '<S30>/Phase// Frequency Offset' */
    basic_cosimulation_c_1_sm_master_DW.freqTnow_p = 0.0;

    /* InitializeConditions for Delay: '<S5>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_b =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_b;

    /* InitializeConditions for Delay: '<S5>/Delay1' */
    basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE =
      basic_cosimulation_c_1_sm_master_P.Delay1_InitialCondition;

    /* InitializeConditions for Delay: '<S8>/Delay2' */
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[2] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition;

    /* InitializeConditions for Delay: '<S8>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_f =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_l;

    /* InitializeConditions for Delay: '<S8>/Delay1' */
    basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE_a =
      basic_cosimulation_c_1_sm_master_P.Delay1_InitialCondition_c;

    /* InitializeConditions for S-Function (OP_SEND): '<S161>/S-Function' */
    /* Level2 S-Function Block: '<S161>/S-Function' (OP_SEND) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[5];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for Delay: '<S5>/Delay2' */
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition_i;
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition_i;
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[2] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition_i;

    /* InitializeConditions for Delay: '<S47>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_i;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_i;

    /* InitializeConditions for DiscreteIntegrator: '<S45>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE =
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator_IC;

    /* InitializeConditions for DiscreteIntegrator: '<S45>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE =
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator1_IC;

    /* InitializeConditions for Delay: '<S48>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_a;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_a;

    /* InitializeConditions for DiscreteIntegrator: '<S75>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE =
      basic_cosimulation_c_1_sm_master_B.Initial;

    /* InitializeConditions for DiscreteIntegrator: '<S57>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC;

    /* InitializeConditions for UnitDelay: '<S68>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition;

    /* InitializeConditions for UnitDelay: '<S68>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition;

    /* InitializeConditions for Delay: '<S68>/Delay' */
    for (i_0 = 0; i_0 < 666; i_0 = i_0 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[i_0] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_k;
    }

    /* End of InitializeConditions for Delay: '<S68>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S59>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_j;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_j;

    /* InitializeConditions for UnitDelay: '<S70>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_b;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_b;

    /* InitializeConditions for UnitDelay: '<S70>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_a;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_a;

    /* InitializeConditions for Delay: '<S70>/Delay' */
    for (i_1 = 0; i_1 < 666; i_1 = i_1 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[i_1] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_lj;
    }

    /* End of InitializeConditions for Delay: '<S70>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S61>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_a;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_a;

    /* InitializeConditions for UnitDelay: '<S72>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_d;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_d;

    /* InitializeConditions for UnitDelay: '<S72>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_b;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_b;

    /* InitializeConditions for Delay: '<S72>/Delay' */
    for (i_2 = 0; i_2 < 666; i_2 = i_2 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[i_2] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_p;
    }

    /* End of InitializeConditions for Delay: '<S72>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S56>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_m;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_m;

    /* InitializeConditions for UnitDelay: '<S67>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_p;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_p;

    /* InitializeConditions for UnitDelay: '<S67>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_h;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_h;

    /* InitializeConditions for Delay: '<S67>/Delay' */
    for (i_3 = 0; i_3 < 666; i_3 = i_3 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[i_3] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_o;
    }

    /* End of InitializeConditions for Delay: '<S67>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S58>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_c;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_c;

    /* InitializeConditions for UnitDelay: '<S69>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_a;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_a;

    /* InitializeConditions for UnitDelay: '<S69>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_e;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_e;

    /* InitializeConditions for Delay: '<S69>/Delay' */
    for (i_4 = 0; i_4 < 666; i_4 = i_4 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[i_4] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_kv;
    }

    /* End of InitializeConditions for Delay: '<S69>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S60>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_my;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_my;

    /* InitializeConditions for UnitDelay: '<S71>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_po;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_po;

    /* InitializeConditions for UnitDelay: '<S71>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_k;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_k;

    /* InitializeConditions for Delay: '<S71>/Delay' */
    for (i_5 = 0; i_5 < 666; i_5 = i_5 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[i_5] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_c;
    }

    /* End of InitializeConditions for Delay: '<S71>/Delay' */

    /* InitializeConditions for Integrator: '<S51>/Integrator' */
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_i;

    /* InitializeConditions for UnitDelay: '<S116>/Unit Delay' */
    for (i_6 = 0; i_6 < 6; i_6 = i_6 + 1) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE_g[i_6] =
        basic_cosimulation_c_1_sm_master_P.UnitDelay_InitialCondition_a;
    }

    /* End of InitializeConditions for UnitDelay: '<S116>/Unit Delay' */

    /* InitializeConditions for Delay: '<S124>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_f;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_f;

    /* InitializeConditions for Integrator: '<S130>/Integrator2' */
    if (rtmIsFirstInitCond(basic_cosimulation_c_1_sm_master_M)) {
      basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE = 0.0;
    }

    basic_cosimulation_c_1_sm_master_DW.Integrator2_IWORK = 1;

    /* End of InitializeConditions for Integrator: '<S130>/Integrator2' */

    /* InitializeConditions for S-Function (rte_svpwm): '<S107>/RTE SPWM1' */
    /* Level2 S-Function Block: '<S107>/RTE SPWM1' (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[6];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (rte_conversion): '<S107>/RTE Conversion1' */
    /* Level2 S-Function Block: '<S107>/RTE Conversion1' (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[7];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (ssn_sfun_v1): '<S163>/State-Space' */
    /* Level2 S-Function Block: '<S163>/State-Space' (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[8];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for Delay: '<S126>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cr[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_fz;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cr[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_fz;

    /* InitializeConditions for Delay: '<S127>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ou[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_av;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ou[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_av;

    /* InitializeConditions for Integrator: '<S136>/Integrator' */
    for (i_7 = 0; i_7 < 8; i_7 = i_7 + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_h[i_7] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_g;
    }

    /* End of InitializeConditions for Integrator: '<S136>/Integrator' */

    /* InitializeConditions for Integrator: '<S138>/Integrator' */
    for (i_8 = 0; i_8 < 8; i_8 = i_8 + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_a[i_8] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_e;
    }

    /* End of InitializeConditions for Integrator: '<S138>/Integrator' */

    /* InitializeConditions for Integrator: '<S140>/Integrator' */
    for (i_9 = 0; i_9 < 8; i_9 = i_9 + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_d[i_9] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_i3;
    }

    /* End of InitializeConditions for Integrator: '<S140>/Integrator' */

    /* InitializeConditions for Integrator: '<S135>/Integrator' */
    for (i_a = 0; i_a < 8; i_a = i_a + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_g[i_a] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_b;
    }

    /* End of InitializeConditions for Integrator: '<S135>/Integrator' */

    /* InitializeConditions for Integrator: '<S137>/Integrator' */
    for (i_b = 0; i_b < 8; i_b = i_b + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_m[i_b] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_i2;
    }

    /* End of InitializeConditions for Integrator: '<S137>/Integrator' */

    /* InitializeConditions for Integrator: '<S139>/Integrator' */
    for (i_c = 0; i_c < 8; i_c = i_c + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_ho[i_c] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_h;
    }

    /* End of InitializeConditions for Integrator: '<S139>/Integrator' */

    /* InitializeConditions for DiscreteIntegrator: '<S124>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_h =
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator_IC_d;

    /* InitializeConditions for DiscreteIntegrator: '<S124>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_m =
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator1_IC_l;

    /* InitializeConditions for Integrator: '<S147>/Integrator' */
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[0] =
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_h;
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[1] =
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_h;
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[2] =
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_h;
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[3] =
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_h;

    /* InitializeConditions for Integrator: '<S130>/Integrator' */
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_hp =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_n;

    /* InitializeConditions for DiscretePulseGenerator: '<S3>/data ready 2.5 kHz' */
    if (Ns <= 0) {
      basic_cosimulation_c_1_sm_master_DW.clockTickCounter = Ns;
    } else {
      basic_cosimulation_c_1_sm_master_DW.clockTickCounter = Ns - (int32_T)
        (basic_cosimulation_c_1_sm_master_P.dataready25kHz_Period * (real_T)
         div_s32(Ns, (int32_T)
                 basic_cosimulation_c_1_sm_master_P.dataready25kHz_Period));
    }

    /* End of InitializeConditions for DiscretePulseGenerator: '<S3>/data ready 2.5 kHz' */

    /* InitializeConditions for S-Function (sfun_send_async): '<S14>/S-Function2' */
    /* Level2 S-Function Block: '<S14>/S-Function2' (sfun_send_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[9];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (sfun_gen_async_ctrl): '<S3>/OpIPSocketCtrl1' */
    /* Level2 S-Function Block: '<S3>/OpIPSocketCtrl1' (sfun_gen_async_ctrl) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[10];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* SystemInitialize for Enabled SubSystem: '<S37>/Tail' */
    basic_cosimulation_c__Tail_Init(&basic_cosimulation_c_1_sm_master_B.Tail,
      &basic_cosimulation_c_1_sm_master_DW.Tail,
      &basic_cosimulation_c_1_sm_master_P.Tail);

    /* End of SystemInitialize for SubSystem: '<S37>/Tail' */

    /* SystemInitialize for Enabled SubSystem: '<S116>/Tail' */
    basic_cosimulation_c__Tail_Init(&basic_cosimulation_c_1_sm_master_B.Tail_e,
      &basic_cosimulation_c_1_sm_master_DW.Tail_e,
      &basic_cosimulation_c_1_sm_master_P.Tail_e);

    /* End of SystemInitialize for SubSystem: '<S116>/Tail' */

    /* InitializeConditions for root-level periodic continuous states */
    {
      int_T rootPeriodicContStateIndices[4] = { 51, 52, 53, 54 };

      real_T rootPeriodicContStateRanges[8] = { 0.0, 6.2831853071795862, 0.0,
        6.2831853071795862, 0.0, 6.2831853071795862, 0.0, 6.2831853071795862 };

      (void) memcpy((void*)basic_cosimulation_c_1_sm_master_PeriodicIndX,
                    rootPeriodicContStateIndices,
                    4*sizeof(int_T));
      (void) memcpy((void*)basic_cosimulation_c_1_sm_master_PeriodicRngX,
                    rootPeriodicContStateRanges,
                    8*sizeof(real_T));
    }

    /* set "at time zero" to false */
    if (rtmIsFirstInitCond(basic_cosimulation_c_1_sm_master_M)) {
      rtmSetFirstInitCond(basic_cosimulation_c_1_sm_master_M, 0);
    }
  }

  /* Enable for DiscreteIntegrator: '<S57>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE = 1;

  /* Enable for DiscreteIntegrator: '<S59>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_p = 1;

  /* Enable for DiscreteIntegrator: '<S61>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_i = 1;

  /* Enable for DiscreteIntegrator: '<S56>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_h = 1;

  /* Enable for DiscreteIntegrator: '<S58>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_iy = 1;

  /* Enable for DiscreteIntegrator: '<S60>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_a = 1;

  /* Enable for Sin: '<S157>/Sine Wave A' */
  basic_cosimulation_c_1_sm_master_DW.systemEnable = 1;

  /* Enable for Sin: '<S157>/Sine Wave B' */
  basic_cosimulation_c_1_sm_master_DW.systemEnable_n = 1;

  /* Enable for Sin: '<S157>/Sine Wave C' */
  basic_cosimulation_c_1_sm_master_DW.systemEnable_j = 1;
}

/* Model terminate function */
void basic_cosimulation_c_1_sm_master_terminate(void)
{
  /* Terminate for S-Function (sfun_recv_async): '<S6>/S-Function1' */
  /* Level2 S-Function Block: '<S6>/S-Function1' (sfun_recv_async) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (RECV_Param): '<S96>/S-Function' */
  /* Level2 S-Function Block: '<S96>/S-Function' (RECV_Param) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[1];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (rte_svpwm): '<S26>/RTE SPWM1' */
  /* Level2 S-Function Block: '<S26>/RTE SPWM1' (rte_svpwm) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[2];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (rte_conversion): '<S26>/RTE Conversion1' */
  /* Level2 S-Function Block: '<S26>/RTE Conversion1' (rte_conversion) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[3];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (ssn_sfun_v1): '<S162>/State-Space' */
  /* Level2 S-Function Block: '<S162>/State-Space' (ssn_sfun_v1) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[4];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (OP_SEND): '<S161>/S-Function' */
  /* Level2 S-Function Block: '<S161>/S-Function' (OP_SEND) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[5];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (rte_svpwm): '<S107>/RTE SPWM1' */
  /* Level2 S-Function Block: '<S107>/RTE SPWM1' (rte_svpwm) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[6];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (rte_conversion): '<S107>/RTE Conversion1' */
  /* Level2 S-Function Block: '<S107>/RTE Conversion1' (rte_conversion) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[7];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (ssn_sfun_v1): '<S163>/State-Space' */
  /* Level2 S-Function Block: '<S163>/State-Space' (ssn_sfun_v1) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[8];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (sfun_send_async): '<S14>/S-Function2' */
  /* Level2 S-Function Block: '<S14>/S-Function2' (sfun_send_async) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[9];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (sfun_gen_async_ctrl): '<S3>/OpIPSocketCtrl1' */
  /* Level2 S-Function Block: '<S3>/OpIPSocketCtrl1' (sfun_gen_async_ctrl) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[10];
    sfcnTerminate(rts);
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  basic_cosimulation_c_1_sm_master_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  basic_cosimulation_c_1_sm_master_update();
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
  basic_cosimulation_c_1_sm_master_initialize();
}

void MdlTerminate(void)
{
  basic_cosimulation_c_1_sm_master_terminate();
}

/* Registration function */
RT_MODEL_basic_cosimulation_c_1_sm_master_T *basic_cosimulation_c_1_sm_master
  (void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  basic_cosimulation_c_1_sm_master_P.Integrator1_UpperSat = rtInf;
  basic_cosimulation_c_1_sm_master_P.Integrator1_LowerSat = rtMinusInf;
  basic_cosimulation_c_1_sm_master_P.RTESPWM1_P3 = rtInf;
  basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_c = rtInf;
  basic_cosimulation_c_1_sm_master_P.Integrator2_UpperSat = rtInf;
  basic_cosimulation_c_1_sm_master_P.Integrator2_LowerSat = rtMinusInf;
  basic_cosimulation_c_1_sm_master_P.RTESPWM1_P3_h = rtInf;
  basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_j = rtInf;

  /* initialize real-time model */
  (void) memset((void *)basic_cosimulation_c_1_sm_master_M, 0,
                sizeof(RT_MODEL_basic_cosimulation_c_1_sm_master_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                          &basic_cosimulation_c_1_sm_master_M->Timing.simTimeStep);
    rtsiSetTPtr(&basic_cosimulation_c_1_sm_master_M->solverInfo, &rtmGetTPtr
                (basic_cosimulation_c_1_sm_master_M));
    rtsiSetStepSizePtr(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                       &basic_cosimulation_c_1_sm_master_M->Timing.stepSize0);
    rtsiSetdXPtr(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                 &basic_cosimulation_c_1_sm_master_M->ModelData.derivs);
    rtsiSetContStatesPtr(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                         (real_T **)
                         &basic_cosimulation_c_1_sm_master_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&basic_cosimulation_c_1_sm_master_M->solverInfo,
      &basic_cosimulation_c_1_sm_master_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr
      (&basic_cosimulation_c_1_sm_master_M->solverInfo,
       &basic_cosimulation_c_1_sm_master_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr
      (&basic_cosimulation_c_1_sm_master_M->solverInfo,
       &basic_cosimulation_c_1_sm_master_M->ModelData.periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr
      (&basic_cosimulation_c_1_sm_master_M->solverInfo,
       &basic_cosimulation_c_1_sm_master_M->ModelData.periodicContStateRanges);
    rtsiSetErrorStatusPtr(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                          (&rtmGetErrorStatus(basic_cosimulation_c_1_sm_master_M)));
    rtsiSetRTModelPtr(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                      basic_cosimulation_c_1_sm_master_M);
  }

  rtsiSetSimTimeStep(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                     MAJOR_TIME_STEP);
  basic_cosimulation_c_1_sm_master_M->ModelData.intgData.y =
    basic_cosimulation_c_1_sm_master_M->ModelData.odeY;
  basic_cosimulation_c_1_sm_master_M->ModelData.intgData.f[0] =
    basic_cosimulation_c_1_sm_master_M->ModelData.odeF[0];
  basic_cosimulation_c_1_sm_master_M->ModelData.intgData.f[1] =
    basic_cosimulation_c_1_sm_master_M->ModelData.odeF[1];
  basic_cosimulation_c_1_sm_master_M->ModelData.intgData.f[2] =
    basic_cosimulation_c_1_sm_master_M->ModelData.odeF[2];
  basic_cosimulation_c_1_sm_master_M->ModelData.intgData.f[3] =
    basic_cosimulation_c_1_sm_master_M->ModelData.odeF[3];
  basic_cosimulation_c_1_sm_master_M->ModelData.contStates = ((real_T *)
    &basic_cosimulation_c_1_sm_master_X);
  basic_cosimulation_c_1_sm_master_M->ModelData.periodicContStateIndices =
    ((int_T*) basic_cosimulation_c_1_sm_master_PeriodicIndX);
  basic_cosimulation_c_1_sm_master_M->ModelData.periodicContStateRanges =
    ((real_T*) basic_cosimulation_c_1_sm_master_PeriodicRngX);
  rtsiSetSolverData(&basic_cosimulation_c_1_sm_master_M->solverInfo, (void *)
                    &basic_cosimulation_c_1_sm_master_M->ModelData.intgData);
  rtsiSetSolverName(&basic_cosimulation_c_1_sm_master_M->solverInfo,"ode4");
  basic_cosimulation_c_1_sm_master_M->solverInfoPtr =
    (&basic_cosimulation_c_1_sm_master_M->solverInfo);

  /* Initialize timing info */
  {
    int_T *mdlTsMap =
      basic_cosimulation_c_1_sm_master_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;

    /* polyspace +2 MISRA2012:D4.1 [Justified:Low] "basic_cosimulation_c_1_sm_master_M points to
       static memory which is guaranteed to be non-NULL" */
    basic_cosimulation_c_1_sm_master_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap
      [0]);
    basic_cosimulation_c_1_sm_master_M->Timing.sampleTimes =
      (&basic_cosimulation_c_1_sm_master_M->Timing.sampleTimesArray[0]);
    basic_cosimulation_c_1_sm_master_M->Timing.offsetTimes =
      (&basic_cosimulation_c_1_sm_master_M->Timing.offsetTimesArray[0]);

    /* task periods */
    basic_cosimulation_c_1_sm_master_M->Timing.sampleTimes[0] = (0.0);
    basic_cosimulation_c_1_sm_master_M->Timing.sampleTimes[1] = (5.0E-5);

    /* task offsets */
    basic_cosimulation_c_1_sm_master_M->Timing.offsetTimes[0] = (0.0);
    basic_cosimulation_c_1_sm_master_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(basic_cosimulation_c_1_sm_master_M,
             &basic_cosimulation_c_1_sm_master_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits =
      basic_cosimulation_c_1_sm_master_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    basic_cosimulation_c_1_sm_master_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(basic_cosimulation_c_1_sm_master_M, -1);
  basic_cosimulation_c_1_sm_master_M->Timing.stepSize0 = 5.0E-5;
  basic_cosimulation_c_1_sm_master_M->Timing.stepSize1 = 5.0E-5;
  rtmSetFirstInitCond(basic_cosimulation_c_1_sm_master_M, 1);

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    basic_cosimulation_c_1_sm_master_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, (NULL));
    rtliSetLogT(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, "");
    rtliSetLogX(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, "");
    rtliSetLogXFinal(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(basic_cosimulation_c_1_sm_master_M->rtwLogInfo,
      "rt_");
    rtliSetLogFormat(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, 0);
    rtliSetLogMaxRows(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, 1000);
    rtliSetLogDecimation(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, 1);
    rtliSetLogY(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(basic_cosimulation_c_1_sm_master_M->rtwLogInfo, (NULL));
  }

  basic_cosimulation_c_1_sm_master_M->solverInfoPtr =
    (&basic_cosimulation_c_1_sm_master_M->solverInfo);
  basic_cosimulation_c_1_sm_master_M->Timing.stepSize = (5.0E-5);
  rtsiSetFixedStepSize(&basic_cosimulation_c_1_sm_master_M->solverInfo, 5.0E-5);
  rtsiSetSolverMode(&basic_cosimulation_c_1_sm_master_M->solverInfo,
                    SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  basic_cosimulation_c_1_sm_master_M->ModelData.blockIO = ((void *)
    &basic_cosimulation_c_1_sm_master_B);
  (void) memset(((void *) &basic_cosimulation_c_1_sm_master_B), 0,
                sizeof(B_basic_cosimulation_c_1_sm_master_T));

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.UnitDelay[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.RTEConversion1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.DataTypeConversion[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Switch[i] = 0.0;
    }

    for (i = 0; i < 18; i++) {
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.StateSpace_o2[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.uRon[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Switch_l[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Saturation_b[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.UnitDelay_c[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.RTEConversion1_j[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Switch_d[i] = 0.0;
    }

    for (i = 0; i < 15; i++) {
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.StateSpace_o2_f[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.uRon_o[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Switch_g[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Saturation_h[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Integrator_cl[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.TransportDelay[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Subtract_k[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Gain_hs[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Integrator_e[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.TransportDelay_m[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Subtract_p[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Gain_ow[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Integrator_g[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.TransportDelay_a[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Subtract_b[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Gain_hk[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Integrator_bf[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.TransportDelay_d[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Subtract_b2[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Gain_b[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Integrator_j[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.TransportDelay_k[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Subtract_i[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Gain_dj[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Integrator_n[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.TransportDelay_p[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Subtract_by[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Gain_bo[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Product1_er[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Product1_kj[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Product1_ew[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Product1_b[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Product1_bw[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Product1_l0[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.DiscreteTimeIntegrator[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.u9Tf[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.Add[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.Saturation1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.Add1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.u1Tt[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.Saturation2[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.Add2[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.UnitDelay[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.Switch[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_e.Product[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.DiscreteTimeIntegrator[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.u9Tf[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.Add[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.Saturation1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.Add1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.u1Tt[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.Saturation2[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.Add2[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.UnitDelay[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.Switch[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail.Product[i] = 0.0;
    }

    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MagnitudeAngletoComplex[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseFrequencyOffset_f[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_c[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i0[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ak[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_la[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ov[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_b[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_a[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_a[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_a[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_a[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_a[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_a[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_a[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_a[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_h[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_h[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_h[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_h[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_h[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_h[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_h[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_h[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_h[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_i[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_i[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_i[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_i[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_i[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_i[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_i[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_i[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gb[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gb[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gb[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gb[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gb[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gb[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gb[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gb[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_au.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_au.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_j.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_j.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_b.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_b.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_pe.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_pe.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_l.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_l.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_h.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_h.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_o.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_o.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_j.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_j.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_o.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_o.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_m.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_m.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_f.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_f.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_n.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_n.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_a.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_a.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_g.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_g.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_o.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_o.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bz.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bz.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_k.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_k.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_k.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_k.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_py.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_py.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_g.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_g.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gd.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_gd.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mo.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mo.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_n.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_n.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_c.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_c.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_m.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_m.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_l.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_l.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b0.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b0.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_j.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_j.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.SFunction = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum = 0.0;
    basic_cosimulation_c_1_sm_master_B.timeout = 0.0;
    basic_cosimulation_c_1_sm_master_B.SFunction1_o1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.SFunction1_o2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Constant11 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum6 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_fm = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum7 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos4 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.SFunction_i[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.SFunction_i[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.SFunction_i[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_m[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_m[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_m[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum16 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide7 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum17 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide8 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum18 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide9 = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DC = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_f[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_f[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_f[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtGainInport[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_i[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_i[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_i[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain6 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain7 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain8 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain10 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum6_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain9 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay2[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain6_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay1_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain7_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain8_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain10_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum6_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain9_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpInput = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpInput_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpInput_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpInput_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpInput_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o1[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag1_o2[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay2_f[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay2_f[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay2_f[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_b[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_b[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator = 0.0;
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_i[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_i[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Switch_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain6_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide1_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4 = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos4_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos5_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos4_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos5_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Bias = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.RoundingFunction = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_nl = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Initial = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_hb[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_hb[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_c[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_c[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_m[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_m[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_d[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_d[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_p[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_p[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_ki[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_ki[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_hy[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_hy[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_ku[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_ku[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_a[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_a[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_p[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_p[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_f[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_f[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_l[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_l[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_lg[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_lg[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_n[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_n[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_c[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_c[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_a[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_a[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_n[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_n[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_a[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_a[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_l[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_l[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_e[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_e[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_b[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_b[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_cv[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_cv[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_e[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_e[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_d[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_d[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_k[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_k[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_g[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_g[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_e[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_e[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_m[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_m[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_m[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_m[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_m[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_m[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_nw[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_nw[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_e1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_e1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_d[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_d[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_a[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_a[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_c1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_c1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_k[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_k[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_a[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_a[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_in[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_in[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_f[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_f[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_j[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_j[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_i[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_i[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_g[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_g[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_d[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_d[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_c[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_c[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_l[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_l[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_m[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_m[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_n[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_n[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_n[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_n[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_ej[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_ej[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_aq[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_aq[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_j[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_j[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_m[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_m[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_l[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_l[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_c[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_c[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_c[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_c[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_kl[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_kl[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_o[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_o[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_k4[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_k4[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_o[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_o[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_pk[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_pk[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_a[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_a[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_ea[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add1_ea[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_hy[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract3_hy[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_jw[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_jw[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_mn[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract2_mn[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_fs[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.delta_fs[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_a5[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add2_a5[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_me[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract4_me[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_n[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add3_n[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_k2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_k2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_cs[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_cs[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Conversion = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_j[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_j[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_j[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain5_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain6_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_b3 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_le = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_dz = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain7_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_dd = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_pp = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_b4 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_ng = 0.0;
    basic_cosimulation_c_1_sm_master_B.W = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_gz = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Constant11_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_bh[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_bh[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_cr = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum6_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ov = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum7_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos4_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos5_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_f[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_f[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_f[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation_g[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation_g[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation_g[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum16_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide7_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum17_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide8_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum18_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide9_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1_h[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2_b[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2_b[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2_b[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DC_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.SineWaveA = 0.0;
    basic_cosimulation_c_1_sm_master_B.SineWaveB = 0.0;
    basic_cosimulation_c_1_sm_master_B.SineWaveC = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_f[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_f[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_iu[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_iu[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_b[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_b[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_b[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_b[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_i[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_i[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_i[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_i[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_lp = 0.0;
    basic_cosimulation_c_1_sm_master_B.Switch_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_hn = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract_cj = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_o0 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain5_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain6_h5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_ez = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_et = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide1_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation1_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_ko = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_pd = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide1_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_ln = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_iw = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_mo = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos4_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos5_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_g1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_gm = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_nq = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ft = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos4_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos5_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_jv = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_ju = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_e4 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ka = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_i[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_i[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_i[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_i[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_h[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_l[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_g5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_oh[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_oh[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_oh[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain5_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain6_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_mo = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_il = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain7_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.W_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_lc = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_fy = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_ge = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_nq = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_he = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_lx = 0.0;
    basic_cosimulation_c_1_sm_master_B.dataready25kHz = 0.0;
    basic_cosimulation_c_1_sm_master_B.SFunction2 = 0.0;
  }

  /* parameters */
  basic_cosimulation_c_1_sm_master_M->ModelData.defaultParam = ((real_T *)
    &basic_cosimulation_c_1_sm_master_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &basic_cosimulation_c_1_sm_master_X;
    basic_cosimulation_c_1_sm_master_M->ModelData.contStates = (x);
    (void) memset((void *)&basic_cosimulation_c_1_sm_master_X, 0,
                  sizeof(X_basic_cosimulation_c_1_sm_master_T));
  }

  /* Periodic continuous states */
  {
    int_T* rootPeriodicContStateIndices =
      basic_cosimulation_c_1_sm_master_PeriodicIndX;
    real_T* rootPeriodicContStateRanges =
      basic_cosimulation_c_1_sm_master_PeriodicRngX;
    basic_cosimulation_c_1_sm_master_M->ModelData.periodicContStateIndices =
      (rootPeriodicContStateIndices);
    basic_cosimulation_c_1_sm_master_M->ModelData.periodicContStateRanges =
      (rootPeriodicContStateRanges);
    (void) memset((void*) basic_cosimulation_c_1_sm_master_PeriodicIndX, 0,
                  4*sizeof(int_T));
    (void) memset((void*) basic_cosimulation_c_1_sm_master_PeriodicRngX, 0,
                  8*sizeof(real_T));
  }

  /* states (dwork) */
  basic_cosimulation_c_1_sm_master_M->ModelData.dwork = ((void *)
    &basic_cosimulation_c_1_sm_master_DW);
  (void) memset((void *)&basic_cosimulation_c_1_sm_master_DW, 0,
                sizeof(DW_basic_cosimulation_c_1_sm_master_T));

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.freqTnow = 0.0;

  {
    int32_T i;
    for (i = 0; i < 14; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 16; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_o[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_m[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_n[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_e[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_mt[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_i[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 18; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_j[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 18; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_d[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.MOV_segment9_l = 0.0;
  basic_cosimulation_c_1_sm_master_DW.freqTnow_p = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_b = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[2] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_f = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE_a = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[2] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE = 0.0;
  basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 666; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 666; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 666; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 666; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 666; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 666; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE_g[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 14; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_jn[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 16; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_ly[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_c[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_jc[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_k[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_f[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_ib[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 15; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_kr[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 15; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_er[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.MOV_segment9_f0 = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cr[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cr[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ou[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ou[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_h = 0.0;
  basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_m = 0.0;
  basic_cosimulation_c_1_sm_master_DW.SFunction_PreviousInput = 0.0;
  basic_cosimulation_c_1_sm_master_DW.lastSin = 0.0;
  basic_cosimulation_c_1_sm_master_DW.lastCos = 0.0;
  basic_cosimulation_c_1_sm_master_DW.lastSin_f = 0.0;
  basic_cosimulation_c_1_sm_master_DW.lastCos_m = 0.0;
  basic_cosimulation_c_1_sm_master_DW.lastSin_k = 0.0;
  basic_cosimulation_c_1_sm_master_DW.lastCos_i = 0.0;

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.RTEConversion1_RWORK[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.RTEConversion1_RWORK_c[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK.modelTStart = 0.0;

  {
    int32_T i;
    for (i = 0; i < 65536; i++) {
      basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK.TUbufferArea[i] =
        0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_d.modelTStart = 0.0;

  {
    int32_T i;
    for (i = 0; i < 65536; i++) {
      basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_d.TUbufferArea[i]
        = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_a.modelTStart = 0.0;

  {
    int32_T i;
    for (i = 0; i < 65536; i++) {
      basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_a.TUbufferArea[i]
        = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_f.modelTStart = 0.0;

  {
    int32_T i;
    for (i = 0; i < 65536; i++) {
      basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_f.TUbufferArea[i]
        = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_j.modelTStart = 0.0;

  {
    int32_T i;
    for (i = 0; i < 65536; i++) {
      basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_j.TUbufferArea[i]
        = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_p.modelTStart = 0.0;

  {
    int32_T i;
    for (i = 0; i < 65536; i++) {
      basic_cosimulation_c_1_sm_master_DW.TransportDelay_RWORK_p.TUbufferArea[i]
        = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.Tail_e.DiscreteTimeIntegrator_DSTATE[i]
        = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.Tail_e.UnitDelay_DSTATE[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.Tail.DiscreteTimeIntegrator_DSTATE[i] =
        0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.Tail.UnitDelay_DSTATE[i] = 0.0;
    }
  }

  /* child S-Function registration */
  {
    RTWSfcnInfo *sfcnInfo =
      &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.sfcnInfo;
    basic_cosimulation_c_1_sm_master_M->sfcnInfo = (sfcnInfo);
    rtssSetErrorStatusPtr(sfcnInfo, (&rtmGetErrorStatus
      (basic_cosimulation_c_1_sm_master_M)));
    basic_cosimulation_c_1_sm_master_M->Sizes.numSampTimes = (2);
    rtssSetNumRootSampTimesPtr(sfcnInfo,
      &basic_cosimulation_c_1_sm_master_M->Sizes.numSampTimes);
    basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.taskTimePtrs[0] =
      &(rtmGetTPtr(basic_cosimulation_c_1_sm_master_M)[0]);
    basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.taskTimePtrs[1] =
      &(rtmGetTPtr(basic_cosimulation_c_1_sm_master_M)[1]);
    rtssSetTPtrPtr(sfcnInfo,
                   basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.taskTimePtrs);
    rtssSetTStartPtr(sfcnInfo, &rtmGetTStart(basic_cosimulation_c_1_sm_master_M));
    rtssSetTFinalPtr(sfcnInfo, &rtmGetTFinal(basic_cosimulation_c_1_sm_master_M));
    rtssSetTimeOfLastOutputPtr(sfcnInfo, &rtmGetTimeOfLastOutput
      (basic_cosimulation_c_1_sm_master_M));
    rtssSetStepSizePtr(sfcnInfo,
                       &basic_cosimulation_c_1_sm_master_M->Timing.stepSize);
    rtssSetStopRequestedPtr(sfcnInfo, &rtmGetStopRequested
      (basic_cosimulation_c_1_sm_master_M));
    rtssSetDerivCacheNeedsResetPtr(sfcnInfo,
      &basic_cosimulation_c_1_sm_master_M->ModelData.derivCacheNeedsReset);
    rtssSetZCCacheNeedsResetPtr(sfcnInfo,
      &basic_cosimulation_c_1_sm_master_M->ModelData.zCCacheNeedsReset);
    rtssSetContTimeOutputInconsistentWithStateAtMajorStepPtr(sfcnInfo,
      &basic_cosimulation_c_1_sm_master_M->ModelData.CTOutputIncnstWithState);
    rtssSetSampleHitsPtr(sfcnInfo,
                         &basic_cosimulation_c_1_sm_master_M->Timing.sampleHits);
    rtssSetPerTaskSampleHitsPtr(sfcnInfo,
      &basic_cosimulation_c_1_sm_master_M->Timing.perTaskSampleHits);
    rtssSetSimModePtr(sfcnInfo, &basic_cosimulation_c_1_sm_master_M->simMode);
    rtssSetSolverInfoPtr(sfcnInfo,
                         &basic_cosimulation_c_1_sm_master_M->solverInfoPtr);
  }

  basic_cosimulation_c_1_sm_master_M->Sizes.numSFcns = (11);

  /* register each child */
  {
    (void) memset((void *)
                  &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.childSFunctions
                  [0], 0,
                  11*sizeof(SimStruct));
    basic_cosimulation_c_1_sm_master_M->childSfunctions =
      (&basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.childSFunctionPtrs[0]);

    {
      int_T i;
      for (i = 0; i < 11; i++) {
        basic_cosimulation_c_1_sm_master_M->childSfunctions[i] =
          (&basic_cosimulation_c_1_sm_master_M->
           NonInlinedSFcns.childSFunctions[i]);
      }
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S6>/S-Function1 (sfun_recv_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[0];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [0]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [0]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [0]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [0]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [0]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [0]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [0]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.UPtrs0;
          sfcnUPtrs[0] = &basic_cosimulation_c_1_sm_master_B.timeout;
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 1);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 3);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        ssSetOutputPortUnit(rts, 2, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 2, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.SFunction1_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.SFunction1_o2));
        }

        /* port 2 */
        {
          _ssSetOutputPortNumDimensions(rts, 2, 1);
          ssSetOutputPortWidth(rts, 2, 6);
          ssSetOutputPortSignal(rts, 2, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.SFunction1_o3));
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function1");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/OpAsyncRecv/S-Function1");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.params;
        ssSetSFcnParamsCount(rts, 14);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P12_Size);
        ssSetSFcnParam(rts, 12, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P13_Size);
        ssSetSFcnParam(rts, 13, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction1_P14_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.SFunction1_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn0.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0, &basic_cosimulation_c_1_sm_master_DW.SFunction1_PWORK);
      }

      /* registration */
      sfun_recv_async(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetOutputPortWidth(rts, 2, 6);
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S96>/S-Function (RECV_Param) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[1];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn1.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn1.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn1.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [1]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [1]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [1]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [1]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [1]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [1]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [1]);
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn1.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 1);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn1.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn1.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 3);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.SFunction_i));
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/OpComm/Receive/S-Function");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn1.params;
        ssSetSFcnParamsCount(rts, 2);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction_P2_Size);
      }

      /* registration */
      RECV_Param(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);

      /* Update the BufferDstPort flags for each input port */
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S26>/RTE SPWM1 (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[2];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [2]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [2]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [2]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [2]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [2]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [2]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [2]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          ssSetInputPortRequiredContiguous(rts, 0, 1);
          ssSetInputPortSignal(rts, 0,
                               &basic_cosimulation_c_1_sm_master_B.Constant11);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 1);
        }

        /* port 1 */
        {
          ssSetInputPortRequiredContiguous(rts, 1, 1);
          ssSetInputPortSignal(rts, 1,
                               basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidth(rts, 1, 3);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 3);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 3);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "RTE SPWM1");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/PWM/RTE SPWM1");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.params;
        ssSetSFcnParamsCount(rts, 10);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P10_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.RTESPWM1_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn2.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0, &basic_cosimulation_c_1_sm_master_DW.RTESPWM1_PWORK);
      }

      /* registration */
      rte_svpwm(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetInputPortConnected(rts, 1, 1);
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 1, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S26>/RTE Conversion1 (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[3];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [3]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [3]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [3]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [3]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [3]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [3]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [3]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.UPtrs0;
          sfcnUPtrs[0] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1[0];
          sfcnUPtrs[1] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1[1];
          sfcnUPtrs[2] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1[2];
          sfcnUPtrs[3] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2[0];
          sfcnUPtrs[4] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2[1];
          sfcnUPtrs[5] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2[2];
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 6);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 1);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 6);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.RTEConversion1));
        }
      }

      /* path info */
      ssSetModelName(rts, "RTE Conversion1");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/PWM/RTE Conversion1");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.params;
        ssSetSFcnParamsCount(rts, 5);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P5_Size);
      }

      /* work vectors */
      ssSetRWork(rts, (real_T *)
                 &basic_cosimulation_c_1_sm_master_DW.RTEConversion1_RWORK[0]);
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.RTEConversion1_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn3.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 2);

        /* RWORK */
        ssSetDWorkWidth(rts, 0, 6);
        ssSetDWorkDataType(rts, 0,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0,
                   &basic_cosimulation_c_1_sm_master_DW.RTEConversion1_RWORK[0]);

        /* PWORK */
        ssSetDWorkWidth(rts, 1, 1);
        ssSetDWorkDataType(rts, 1,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 1, 0);
        ssSetDWork(rts, 1,
                   &basic_cosimulation_c_1_sm_master_DW.RTEConversion1_PWORK);
      }

      /* registration */
      rte_conversion(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetInputPortWidth(rts, 0, 6);
      ssSetInputPortDataType(rts, 0, SS_DOUBLE);
      ssSetInputPortComplexSignal(rts, 0, 0);
      ssSetInputPortFrameData(rts, 0, 0);
      ssSetInputPortUnit(rts, 0, 0);
      ssSetInputPortIsContinuousQuantity(rts, 0, 0);
      ssSetOutputPortWidth(rts, 0, 6);
      ssSetOutputPortDataType(rts, 0, SS_DOUBLE);
      ssSetOutputPortComplexSignal(rts, 0, 0);
      ssSetOutputPortFrameData(rts, 0, 0);
      ssSetOutputPortUnit(rts, 0, 0);
      ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
      ssSetNumRWork(rts, 6);
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S162>/State-Space (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[4];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [4]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [4]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [4]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [4]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [4]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [4]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [4]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.UPtrs0;

          {
            int_T i1;
            const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Tail.Product
              [0];
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1] = &u0[i1];
            }

            sfcnUPtrs[6] = &basic_cosimulation_c_1_sm_master_B.DC;
            sfcnUPtrs[7] =
              &basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[0];
            sfcnUPtrs[8] =
              &basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[1];
            sfcnUPtrs[9] =
              &basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[2];
            u0 = &basic_cosimulation_c_1_sm_master_B.Switch[0];
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1+ 10] = &u0[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 16);
        }

        /* port 1 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.UPtrs1;

          {
            int_T i1;
            const real_T *u1 =
              basic_cosimulation_c_1_sm_master_B.DataTypeConversion;
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1] = &u1[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 1, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidth(rts, 1, 6);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 18);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.StateSpace_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 6);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.StateSpace_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "State-Space");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/powergui/EquivalentModel1/State-Space");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.params;
        ssSetSFcnParamsCount(rts, 2);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.StateSpace_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.StateSpace_P2_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.StateSpace_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn4.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 11);

        /* xn_g0 */
        ssSetDWorkWidth(rts, 0, 14);
        ssSetDWorkDataType(rts, 0,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWorkUsedAsDState(rts, 0, 1);
        ssSetDWork(rts, 0, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9[0]);

        /* un_g0 */
        ssSetDWorkWidth(rts, 1, 16);
        ssSetDWorkDataType(rts, 1,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 1, 0);
        ssSetDWorkUsedAsDState(rts, 1, 1);
        ssSetDWork(rts, 1, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_o[0]);

        /* sw_g0 */
        ssSetDWorkWidth(rts, 2, 6);
        ssSetDWorkDataType(rts, 2,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 2, 0);
        ssSetDWorkUsedAsDState(rts, 2, 1);
        ssSetDWork(rts, 2, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_m[0]);

        /* sw_volta0 */
        ssSetDWorkWidth(rts, 3, 6);
        ssSetDWorkDataType(rts, 3,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 3, 0);
        ssSetDWorkUsedAsDState(rts, 3, 1);
        ssSetDWork(rts, 3, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_n[0]);

        /* sw_volta_old0 */
        ssSetDWorkWidth(rts, 4, 6);
        ssSetDWorkDataType(rts, 4,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 4, 0);
        ssSetDWorkUsedAsDState(rts, 4, 1);
        ssSetDWork(rts, 4, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_e[0]);

        /* sw_sta_old_g0 */
        ssSetDWorkWidth(rts, 5, 6);
        ssSetDWorkDataType(rts, 5,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 5, 0);
        ssSetDWorkUsedAsDState(rts, 5, 1);
        ssSetDWork(rts, 5, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_mt
                   [0]);

        /* global_sw_g0 */
        ssSetDWorkWidth(rts, 6, 6);
        ssSetDWorkDataType(rts, 6,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 6, 0);
        ssSetDWorkUsedAsDState(rts, 6, 1);
        ssSetDWork(rts, 6, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_i[0]);

        /* yout_g0 */
        ssSetDWorkWidth(rts, 7, 18);
        ssSetDWorkDataType(rts, 7,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 7, 0);
        ssSetDWorkUsedAsDState(rts, 7, 1);
        ssSetDWork(rts, 7, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_j[0]);

        /* youtold_g0 */
        ssSetDWorkWidth(rts, 8, 18);
        ssSetDWorkDataType(rts, 8,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 8, 0);
        ssSetDWorkUsedAsDState(rts, 8, 1);
        ssSetDWork(rts, 8, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_d[0]);

        /* MOV_segment0 */
        ssSetDWorkWidth(rts, 9, 1);
        ssSetDWorkDataType(rts, 9,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 9, 0);
        ssSetDWorkUsedAsDState(rts, 9, 1);
        ssSetDWork(rts, 9, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_l);

        /* PWORK */
        ssSetDWorkWidth(rts, 10, 1);
        ssSetDWorkDataType(rts, 10,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 10, 0);
        ssSetDWork(rts, 10,
                   &basic_cosimulation_c_1_sm_master_DW.StateSpace_PWORK);
      }

      /* registration */
      ssn_sfun_v1(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetInputPortConnected(rts, 1, 1);
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 1, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S161>/S-Function (OP_SEND) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[5];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [5]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [5]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [5]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [5]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [5]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [5]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [5]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.UPtrs0;

          {
            int_T i1;
            const real_T *u0 =
              &basic_cosimulation_c_1_sm_master_B.SFunction1_o3[0];
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1] = &u0[i1];
            }

            sfcnUPtrs[6] =
              &basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[0];
            sfcnUPtrs[7] =
              &basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[1];
            sfcnUPtrs[8] =
              &basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[2];
            sfcnUPtrs[9] = &basic_cosimulation_c_1_sm_master_B.Gain1_i[0];
            sfcnUPtrs[10] = &basic_cosimulation_c_1_sm_master_B.Gain1_i[1];
            sfcnUPtrs[11] = &basic_cosimulation_c_1_sm_master_B.Gain1_i[2];
            sfcnUPtrs[12] = &basic_cosimulation_c_1_sm_master_B.Gain7;
            sfcnUPtrs[13] = &basic_cosimulation_c_1_sm_master_B.Gain8;
            sfcnUPtrs[14] = &basic_cosimulation_c_1_sm_master_B.Gain9;
            sfcnUPtrs[15] = &basic_cosimulation_c_1_sm_master_B.Delay2[0];
            sfcnUPtrs[16] = &basic_cosimulation_c_1_sm_master_B.Delay2[1];
            sfcnUPtrs[17] = &basic_cosimulation_c_1_sm_master_B.Delay2[2];
            sfcnUPtrs[18] = &basic_cosimulation_c_1_sm_master_B.Gain7_b;
            sfcnUPtrs[19] = &basic_cosimulation_c_1_sm_master_B.Gain8_i;
            sfcnUPtrs[20] = &basic_cosimulation_c_1_sm_master_B.Gain9_n;
            sfcnUPtrs[21] = &basic_cosimulation_c_1_sm_master_B.OpInput;
            sfcnUPtrs[22] = &basic_cosimulation_c_1_sm_master_B.OpInput_a;
            sfcnUPtrs[23] = &basic_cosimulation_c_1_sm_master_B.OpInput_p;
            sfcnUPtrs[24] = &basic_cosimulation_c_1_sm_master_B.OpInput_b;
            sfcnUPtrs[25] = &basic_cosimulation_c_1_sm_master_B.OpInput_f;
          }

          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 26);
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/rtlab_send_subsystem/Subsystem1/Send1/S-Function");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.params;
        ssSetSFcnParamsCount(rts, 1);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction_P1_Size_j);
      }

      /* work vectors */
      ssSetIWork(rts, (int_T *)
                 &basic_cosimulation_c_1_sm_master_DW.SFunction_IWORK[0]);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn5.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* IWORK */
        ssSetDWorkWidth(rts, 0, 5);
        ssSetDWorkDataType(rts, 0,SS_INTEGER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0, &basic_cosimulation_c_1_sm_master_DW.SFunction_IWORK
                   [0]);
      }

      /* registration */
      OP_SEND(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetInputPortWidth(rts, 0, 26);
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S107>/RTE SPWM1 (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[6];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [6]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [6]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [6]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [6]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [6]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [6]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [6]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          ssSetInputPortRequiredContiguous(rts, 0, 1);
          ssSetInputPortSignal(rts, 0,
                               &basic_cosimulation_c_1_sm_master_B.Constant11_i);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 1);
        }

        /* port 1 */
        {
          ssSetInputPortRequiredContiguous(rts, 1, 1);
          ssSetInputPortSignal(rts, 1,
                               basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidth(rts, 1, 3);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 3);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1_h));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 3);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2_b));
        }
      }

      /* path info */
      ssSetModelName(rts, "RTE SPWM1");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/PWM/RTE SPWM1");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.params;
        ssSetSFcnParamsCount(rts, 10);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P1_Size_f);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P2_Size_n);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P3_Size_e);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P4_Size_a);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P5_Size_f);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P6_Size_a);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P7_Size_l);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P8_Size_k);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P9_Size_k);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTESPWM1_P10_Size_b);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.RTESPWM1_PWORK_i);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0, &basic_cosimulation_c_1_sm_master_DW.RTESPWM1_PWORK_i);
      }

      /* registration */
      rte_svpwm(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetInputPortConnected(rts, 1, 1);
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 1, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S107>/RTE Conversion1 (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[7];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [7]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [7]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [7]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [7]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [7]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [7]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [7]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.UPtrs0;
          sfcnUPtrs[0] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1_h[0];
          sfcnUPtrs[1] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1_h[1];
          sfcnUPtrs[2] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o1_h[2];
          sfcnUPtrs[3] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2_b[0];
          sfcnUPtrs[4] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2_b[1];
          sfcnUPtrs[5] = &basic_cosimulation_c_1_sm_master_B.RTESPWM1_o2_b[2];
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 6);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 1);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 6);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.RTEConversion1_j));
        }
      }

      /* path info */
      ssSetModelName(rts, "RTE Conversion1");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/PWM/RTE Conversion1");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.params;
        ssSetSFcnParamsCount(rts, 5);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P1_Size_a);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P2_Size_g);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P3_Size_h);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P4_Size_j);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.RTEConversion1_P5_Size_c);
      }

      /* work vectors */
      ssSetRWork(rts, (real_T *)
                 &basic_cosimulation_c_1_sm_master_DW.RTEConversion1_RWORK_c[0]);
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.RTEConversion1_PWORK_d);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 2);

        /* RWORK */
        ssSetDWorkWidth(rts, 0, 6);
        ssSetDWorkDataType(rts, 0,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0,
                   &basic_cosimulation_c_1_sm_master_DW.RTEConversion1_RWORK_c[0]);

        /* PWORK */
        ssSetDWorkWidth(rts, 1, 1);
        ssSetDWorkDataType(rts, 1,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 1, 0);
        ssSetDWork(rts, 1,
                   &basic_cosimulation_c_1_sm_master_DW.RTEConversion1_PWORK_d);
      }

      /* registration */
      rte_conversion(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetInputPortWidth(rts, 0, 6);
      ssSetInputPortDataType(rts, 0, SS_DOUBLE);
      ssSetInputPortComplexSignal(rts, 0, 0);
      ssSetInputPortFrameData(rts, 0, 0);
      ssSetInputPortUnit(rts, 0, 0);
      ssSetInputPortIsContinuousQuantity(rts, 0, 0);
      ssSetOutputPortWidth(rts, 0, 6);
      ssSetOutputPortDataType(rts, 0, SS_DOUBLE);
      ssSetOutputPortComplexSignal(rts, 0, 0);
      ssSetOutputPortFrameData(rts, 0, 0);
      ssSetOutputPortUnit(rts, 0, 0);
      ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
      ssSetNumRWork(rts, 6);
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S163>/State-Space (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[8];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [8]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [8]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [8]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [8]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [8]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [8]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [8]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.UPtrs0;

          {
            int_T i1;
            const real_T *u0 =
              &basic_cosimulation_c_1_sm_master_B.Tail_e.Product[0];
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1] = &u0[i1];
            }

            sfcnUPtrs[6] = &basic_cosimulation_c_1_sm_master_B.DC_h;
            sfcnUPtrs[7] = &basic_cosimulation_c_1_sm_master_B.SineWaveA;
            sfcnUPtrs[8] = &basic_cosimulation_c_1_sm_master_B.SineWaveB;
            sfcnUPtrs[9] = &basic_cosimulation_c_1_sm_master_B.SineWaveC;
            u0 = &basic_cosimulation_c_1_sm_master_B.Switch_d[0];
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1+ 10] = &u0[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 16);
        }

        /* port 1 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.UPtrs1;

          {
            int_T i1;
            const real_T *u1 =
              basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o;
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1] = &u1[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 1, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidth(rts, 1, 6);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 15);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 6);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            basic_cosimulation_c_1_sm_master_B.StateSpace_o2_f));
        }
      }

      /* path info */
      ssSetModelName(rts, "State-Space");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/powergui/EquivalentModel2/State-Space");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.params;
        ssSetSFcnParamsCount(rts, 2);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.StateSpace_P1_Size_n);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.StateSpace_P2_Size_h);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.StateSpace_PWORK_c);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 11);

        /* xn_g0 */
        ssSetDWorkWidth(rts, 0, 14);
        ssSetDWorkDataType(rts, 0,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWorkUsedAsDState(rts, 0, 1);
        ssSetDWork(rts, 0, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_jn
                   [0]);

        /* un_g0 */
        ssSetDWorkWidth(rts, 1, 16);
        ssSetDWorkDataType(rts, 1,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 1, 0);
        ssSetDWorkUsedAsDState(rts, 1, 1);
        ssSetDWork(rts, 1, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_ly
                   [0]);

        /* sw_g0 */
        ssSetDWorkWidth(rts, 2, 6);
        ssSetDWorkDataType(rts, 2,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 2, 0);
        ssSetDWorkUsedAsDState(rts, 2, 1);
        ssSetDWork(rts, 2, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_c[0]);

        /* sw_volta0 */
        ssSetDWorkWidth(rts, 3, 6);
        ssSetDWorkDataType(rts, 3,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 3, 0);
        ssSetDWorkUsedAsDState(rts, 3, 1);
        ssSetDWork(rts, 3, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_jc
                   [0]);

        /* sw_volta_old0 */
        ssSetDWorkWidth(rts, 4, 6);
        ssSetDWorkDataType(rts, 4,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 4, 0);
        ssSetDWorkUsedAsDState(rts, 4, 1);
        ssSetDWork(rts, 4, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_k[0]);

        /* sw_sta_old_g0 */
        ssSetDWorkWidth(rts, 5, 6);
        ssSetDWorkDataType(rts, 5,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 5, 0);
        ssSetDWorkUsedAsDState(rts, 5, 1);
        ssSetDWork(rts, 5, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_f[0]);

        /* global_sw_g0 */
        ssSetDWorkWidth(rts, 6, 6);
        ssSetDWorkDataType(rts, 6,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 6, 0);
        ssSetDWorkUsedAsDState(rts, 6, 1);
        ssSetDWork(rts, 6, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_ib
                   [0]);

        /* yout_g0 */
        ssSetDWorkWidth(rts, 7, 15);
        ssSetDWorkDataType(rts, 7,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 7, 0);
        ssSetDWorkUsedAsDState(rts, 7, 1);
        ssSetDWork(rts, 7, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_kr
                   [0]);

        /* youtold_g0 */
        ssSetDWorkWidth(rts, 8, 15);
        ssSetDWorkDataType(rts, 8,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 8, 0);
        ssSetDWorkUsedAsDState(rts, 8, 1);
        ssSetDWork(rts, 8, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_er
                   [0]);

        /* MOV_segment0 */
        ssSetDWorkWidth(rts, 9, 1);
        ssSetDWorkDataType(rts, 9,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 9, 0);
        ssSetDWorkUsedAsDState(rts, 9, 1);
        ssSetDWork(rts, 9, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_f0);

        /* PWORK */
        ssSetDWorkWidth(rts, 10, 1);
        ssSetDWorkDataType(rts, 10,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 10, 0);
        ssSetDWork(rts, 10,
                   &basic_cosimulation_c_1_sm_master_DW.StateSpace_PWORK_c);
      }

      /* registration */
      ssn_sfun_v1(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetInputPortConnected(rts, 1, 1);
      _ssSetOutputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 1, 1);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
      ssSetInputPortBufferDstPort(rts, 1, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S14>/S-Function2 (sfun_send_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[9];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [9]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [9]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [9]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [9]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [9]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [9]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [9]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.UPtrs0;
          sfcnUPtrs[0] = &basic_cosimulation_c_1_sm_master_B.dataready25kHz;
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 1);
        }

        /* port 1 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.UPtrs1;
          sfcnUPtrs[0] =
            &basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[0];
          sfcnUPtrs[1] =
            &basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[1];
          sfcnUPtrs[2] =
            &basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[2];
          sfcnUPtrs[3] = &basic_cosimulation_c_1_sm_master_B.Gain1_i[0];
          sfcnUPtrs[4] = &basic_cosimulation_c_1_sm_master_B.Gain1_i[1];
          sfcnUPtrs[5] = &basic_cosimulation_c_1_sm_master_B.Gain1_i[2];
          ssSetInputPortSignalPtrs(rts, 1, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 1, 1);
          ssSetInputPortWidth(rts, 1, 6);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 1);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.SFunction2));
        }
      }

      /* path info */
      ssSetModelName(rts, "S-Function2");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/send message 1/S-Function2");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.params;
        ssSetSFcnParamsCount(rts, 13);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P12_Size);
        ssSetSFcnParam(rts, 12, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.SFunction2_P13_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.SFunction2_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0, &basic_cosimulation_c_1_sm_master_DW.SFunction2_PWORK);
      }

      /* registration */
      sfun_send_async(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetInputPortWidth(rts, 1, 6);
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S3>/OpIPSocketCtrl1 (sfun_gen_async_ctrl) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[10];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.sfcnTsMap;
      (void) memset((void*)sfcnPeriod, 0,
                    sizeof(time_T)*1);
      (void) memset((void*)sfcnOffset, 0,
                    sizeof(time_T)*1);
      ssSetSampleTimePtr(rts, &sfcnPeriod[0]);
      ssSetOffsetTimePtr(rts, &sfcnOffset[0]);
      ssSetSampleTimeTaskIDPtr(rts, sfcnTsMap);

      {
        ssSetBlkInfo2Ptr(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.blkInfo2
                         [10]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [10]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [10]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [10]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [10]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [10]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [10]);
      }

      /* path info */
      ssSetModelName(rts, "OpIPSocketCtrl1");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/OpIPSocketCtrl1");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.params;
        ssSetSFcnParamsCount(rts, 27);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P12_Size);
        ssSetSFcnParam(rts, 12, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P13_Size);
        ssSetSFcnParam(rts, 13, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P14_Size);
        ssSetSFcnParam(rts, 14, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P15_Size);
        ssSetSFcnParam(rts, 15, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P16_Size);
        ssSetSFcnParam(rts, 16, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P17_Size);
        ssSetSFcnParam(rts, 17, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P18_Size);
        ssSetSFcnParam(rts, 18, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P19_Size);
        ssSetSFcnParam(rts, 19, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P20_Size);
        ssSetSFcnParam(rts, 20, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P21_Size);
        ssSetSFcnParam(rts, 21, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P22_Size);
        ssSetSFcnParam(rts, 22, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P23_Size);
        ssSetSFcnParam(rts, 23, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P24_Size);
        ssSetSFcnParam(rts, 24, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P25_Size);
        ssSetSFcnParam(rts, 25, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P26_Size);
        ssSetSFcnParam(rts, 26, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpIPSocketCtrl1_P27_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.OpIPSocketCtrl1_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0,
                   &basic_cosimulation_c_1_sm_master_DW.OpIPSocketCtrl1_PWORK);
      }

      /* registration */
      sfun_gen_async_ctrl(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      /* Update the BufferDstPort flags for each input port */
    }
  }

  /* Initialize Sizes */
  basic_cosimulation_c_1_sm_master_M->Sizes.numContStates = (56);/* Number of continuous states */
  basic_cosimulation_c_1_sm_master_M->Sizes.numPeriodicContStates = (4);
                                      /* Number of periodic continuous states */
  basic_cosimulation_c_1_sm_master_M->Sizes.numY = (0);/* Number of model outputs */
  basic_cosimulation_c_1_sm_master_M->Sizes.numU = (0);/* Number of model inputs */
  basic_cosimulation_c_1_sm_master_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  basic_cosimulation_c_1_sm_master_M->Sizes.numSampTimes = (2);/* Number of sample times */
  basic_cosimulation_c_1_sm_master_M->Sizes.numBlocks = (797);/* Number of blocks */
  basic_cosimulation_c_1_sm_master_M->Sizes.numBlockIO = (604);/* Number of block outputs */
  basic_cosimulation_c_1_sm_master_M->Sizes.numBlockPrms = (698);/* Sum of parameter "widths" */
  return basic_cosimulation_c_1_sm_master_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
