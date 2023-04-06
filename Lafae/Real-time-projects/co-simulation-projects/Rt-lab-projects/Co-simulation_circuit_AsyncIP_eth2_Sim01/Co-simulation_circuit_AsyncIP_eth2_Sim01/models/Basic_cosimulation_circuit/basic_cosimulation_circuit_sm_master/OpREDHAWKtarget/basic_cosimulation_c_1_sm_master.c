/*
 * basic_cosimulation_c_1_sm_master.c
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
 *    '<S102>/Tail'
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
 *    '<S102>/Tail'
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
 *    '<S102>/Tail'
 */
void basic_cosimulation_Tail_Disable(DW_Tail_basic_cosimulation_c__T *localDW)
{
  localDW->Tail_MODE = false;
}

/*
 * Outputs for enable system:
 *    '<S37>/Tail'
 *    '<S102>/Tail'
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
        localP->u_Value_l;
    }
  }

  /* End of Outputs for SubSystem: '<S37>/Tail' */
}

/*
 * Update for enable system:
 *    '<S37>/Tail'
 *    '<S102>/Tail'
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
  creal_T tmp_a;
  creal_T tmp_b;
  creal_T tmp_c;
  real_T Bias;
  real_T b;
  real_T b_idx_0;
  real_T b_idx_1;
  real_T b_idx_2;
  real_T b_idx_3;
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
  real_T im_1m;
  real_T im_1n;
  real_T im_1o;
  real_T im_1p;
  real_T im_1q;
  real_T im_1r;
  real_T im_1s;
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
  real_T re_1m;
  real_T re_1n;
  real_T re_1o;
  real_T re_1p;
  real_T re_1q;
  real_T re_1r;
  real_T re_1s;
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
  real_T u;
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
  int32_T i_j;
  int32_T i_k;
  int32_T i_l;
  int32_T i_m;
  int_T tmp;
  int_T tmp_0;
  int_T tmp_1;
  int_T tmp_2;
  int_T tmp_3;
  int_T tmp_4;
  int_T tmp_5;
  int_T tmp_6;
  int_T tmp_7;
  int_T tmp_8;
  int_T tmp_9;
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

    /* S-Function (sfun_recv_async): '<S5>/S-Function1' */

    /* Level2 S-Function Block: '<S5>/S-Function1' (sfun_recv_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[0];
      sfcnOutputs(rts,0);
    }

    for (i_a = 0; i_a < 6; i_a = i_a + 1) {
      /* UnitDelay: '<S102>/Unit Delay' */
      basic_cosimulation_c_1_sm_master_B.UnitDelay[i_a] =
        basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE[i_a];
    }

    /* Constant: '<S91>/Constant11' */
    basic_cosimulation_c_1_sm_master_B.Constant11 =
      basic_cosimulation_c_1_sm_master_P.fsw_inv;

    /* Delay: '<S110>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[0];
    basic_cosimulation_c_1_sm_master_B.Delay[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[1];
  }

  /* RelationalOperator: '<S116>/Relational Operator' incorporates:
   *  Constant: '<S116>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.RelationalOperator =
    (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE >
     basic_cosimulation_c_1_sm_master_P.Constant2_Value);

  /* Sum: '<S116>/Subtract' incorporates:
   *  Constant: '<S116>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Subtract =
    basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value;

  /* Integrator: '<S116>/Integrator1' */
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

  /* Integrator: '<S116>/Integrator1' */
  basic_cosimulation_c_1_sm_master_B.Integrator1 =
    basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE;

  /* Trigonometry: '<S117>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Product: '<S117>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product =
    basic_cosimulation_c_1_sm_master_B.Delay[0] *
    basic_cosimulation_c_1_sm_master_B.Cos;

  /* Trigonometry: '<S117>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1 = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Gain: '<S117>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_h =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain *
    basic_cosimulation_c_1_sm_master_B.Cos1;

  /* Product: '<S117>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_o =
    basic_cosimulation_c_1_sm_master_B.Delay[1] *
    basic_cosimulation_c_1_sm_master_B.Gain_h;

  /* Sum: '<S117>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1 =
    basic_cosimulation_c_1_sm_master_B.Product +
    basic_cosimulation_c_1_sm_master_B.Product1_o;

  /* Sum: '<S117>/Sum6' incorporates:
   *  Constant: '<S117>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum6 =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant3_Value;

  /* Trigonometry: '<S117>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2 = cos
    (basic_cosimulation_c_1_sm_master_B.Sum6);

  /* Product: '<S117>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_c =
    basic_cosimulation_c_1_sm_master_B.Delay[0] *
    basic_cosimulation_c_1_sm_master_B.Cos2;

  /* Sum: '<S117>/Sum2' incorporates:
   *  Constant: '<S117>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2 =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant1_Value;

  /* Trigonometry: '<S117>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3 = sin
    (basic_cosimulation_c_1_sm_master_B.Sum2);

  /* Gain: '<S117>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_fm =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_d *
    basic_cosimulation_c_1_sm_master_B.Cos3;

  /* Product: '<S117>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_n =
    basic_cosimulation_c_1_sm_master_B.Delay[1] *
    basic_cosimulation_c_1_sm_master_B.Gain1_fm;

  /* Sum: '<S117>/Sum3' */
  basic_cosimulation_c_1_sm_master_B.Sum3 =
    basic_cosimulation_c_1_sm_master_B.Product2_c +
    basic_cosimulation_c_1_sm_master_B.Product3_n;

  /* Sum: '<S117>/Sum7' incorporates:
   *  Constant: '<S117>/Constant4'
   */
  basic_cosimulation_c_1_sm_master_B.Sum7 =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant4_Value;

  /* Trigonometry: '<S117>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4 = cos
    (basic_cosimulation_c_1_sm_master_B.Sum7);

  /* Product: '<S117>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4 =
    basic_cosimulation_c_1_sm_master_B.Delay[0] *
    basic_cosimulation_c_1_sm_master_B.Cos4;

  /* Sum: '<S117>/Sum4' incorporates:
   *  Constant: '<S117>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum4 =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_m;

  /* Trigonometry: '<S117>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5 = sin
    (basic_cosimulation_c_1_sm_master_B.Sum4);

  /* Gain: '<S117>/Gain2' */
  basic_cosimulation_c_1_sm_master_B.Gain2_h =
    basic_cosimulation_c_1_sm_master_P.Gain2_Gain_e *
    basic_cosimulation_c_1_sm_master_B.Cos5;

  /* Product: '<S117>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5 =
    basic_cosimulation_c_1_sm_master_B.Delay[1] *
    basic_cosimulation_c_1_sm_master_B.Gain2_h;

  /* Sum: '<S117>/Sum5' */
  basic_cosimulation_c_1_sm_master_B.Sum5 =
    basic_cosimulation_c_1_sm_master_B.Product4 +
    basic_cosimulation_c_1_sm_master_B.Product5;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* S-Function (RECV_Param): '<S17>/S-Function' */

    /* Level2 S-Function Block: '<S17>/S-Function' (RECV_Param) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[1];
      sfcnOutputs(rts,0);
    }
  }

  /* Product: '<S82>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_m[0] =
    basic_cosimulation_c_1_sm_master_B.Sum1 *
    basic_cosimulation_c_1_sm_master_B.SFunction_l[0];
  basic_cosimulation_c_1_sm_master_B.Product2_m[1] =
    basic_cosimulation_c_1_sm_master_B.Sum3 *
    basic_cosimulation_c_1_sm_master_B.SFunction_l[0];
  basic_cosimulation_c_1_sm_master_B.Product2_m[2] =
    basic_cosimulation_c_1_sm_master_B.Sum5 *
    basic_cosimulation_c_1_sm_master_B.SFunction_l[0];

  /* Saturate: '<S82>/Saturation' */
  u0 = basic_cosimulation_c_1_sm_master_B.Product2_m[0];
  if (u0 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat) {
    y = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat;
  } else if (u0 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat) {
    y = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat;
  } else {
    y = u0;
  }

  /* Saturate: '<S82>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation[0] = y;

  /* Saturate: '<S82>/Saturation' */
  u0_0 = basic_cosimulation_c_1_sm_master_B.Product2_m[1];
  if (u0_0 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat) {
    y_0 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat;
  } else if (u0_0 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat) {
    y_0 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat;
  } else {
    y_0 = u0_0;
  }

  /* Saturate: '<S82>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation[1] = y_0;

  /* Saturate: '<S82>/Saturation' */
  u0_1 = basic_cosimulation_c_1_sm_master_B.Product2_m[2];
  if (u0_1 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat) {
    y_1 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat;
  } else if (u0_1 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat) {
    y_1 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat;
  } else {
    y_1 = u0_1;
  }

  /* Saturate: '<S82>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation[2] = y_1;

  /* Sum: '<S91>/Sum16' incorporates:
   *  Constant: '<S91>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum16 =
    basic_cosimulation_c_1_sm_master_B.Saturation[0] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value;

  /* Product: '<S91>/Divide7' incorporates:
   *  Constant: '<S91>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide7 =
    basic_cosimulation_c_1_sm_master_B.Sum16 /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value;

  /* Sum: '<S91>/Sum17' incorporates:
   *  Constant: '<S91>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum17 =
    basic_cosimulation_c_1_sm_master_B.Saturation[1] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value;

  /* Product: '<S91>/Divide8' incorporates:
   *  Constant: '<S91>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide8 =
    basic_cosimulation_c_1_sm_master_B.Sum17 /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value;

  /* Sum: '<S91>/Sum18' incorporates:
   *  Constant: '<S91>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum18 =
    basic_cosimulation_c_1_sm_master_B.Saturation[2] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value;

  /* Product: '<S91>/Divide9' incorporates:
   *  Constant: '<S91>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide9 =
    basic_cosimulation_c_1_sm_master_B.Sum18 /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* SignalConversion generated from: '<S91>/RTE SPWM1' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In[0] =
      basic_cosimulation_c_1_sm_master_B.Divide7;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In[1] =
      basic_cosimulation_c_1_sm_master_B.Divide8;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1In[2] =
      basic_cosimulation_c_1_sm_master_B.Divide9;

    /* S-Function (rte_svpwm): '<S91>/RTE SPWM1' */

    /* Level2 S-Function Block: '<S91>/RTE SPWM1' (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[2];
      sfcnOutputs(rts,0);
    }

    /* S-Function (rte_conversion): '<S91>/RTE Conversion1' */

    /* Level2 S-Function Block: '<S91>/RTE Conversion1' (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[3];
      sfcnOutputs(rts,0);
    }

    /* DataTypeConversion: '<S105>/Data Type Conversion' */
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

    /* Outputs for Enabled SubSystem: '<S102>/Tail' */
    /* Outputs for Enabled SubSystem: '<S102>/Tail' */
    /* Constant: '<S102>/Not in ARTEMIS' */
    basic_cosimulation_c_1_sm__Tail(basic_cosimulation_c_1_sm_master_M,
      basic_cosimulation_c_1_sm_master_P.NotinARTEMIS_Value,
      basic_cosimulation_c_1_sm_master_B.UnitDelay,
      basic_cosimulation_c_1_sm_master_B.DataTypeConversion,
      basic_cosimulation_c_1_sm_master_P.Tail_Tf,
      basic_cosimulation_c_1_sm_master_P.Tail_Tt,
      &basic_cosimulation_c_1_sm_master_B.Tail_f,
      &basic_cosimulation_c_1_sm_master_DW.Tail_f,
      &basic_cosimulation_c_1_sm_master_P.Tail_f);

    /* End of Outputs for SubSystem: '<S102>/Tail' */
    /* End of Outputs for SubSystem: '<S102>/Tail' */

    /* Constant: '<S151>/DC' */
    basic_cosimulation_c_1_sm_master_B.DC =
      basic_cosimulation_c_1_sm_master_P.Vdc;

    /* Gain: '<S92>/Deg->Rad' */
    basic_cosimulation_c_1_sm_master_B.DegRad[0] =
      basic_cosimulation_c_1_sm_master_P.DegRad_Gain *
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[0 + 3];
    basic_cosimulation_c_1_sm_master_B.DegRad[1] =
      basic_cosimulation_c_1_sm_master_P.DegRad_Gain *
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[1 + 3];
    basic_cosimulation_c_1_sm_master_B.DegRad[2] =
      basic_cosimulation_c_1_sm_master_P.DegRad_Gain *
      basic_cosimulation_c_1_sm_master_B.SFunction1_o3[2 + 3];

    /* MagnitudeAngleToComplex: '<S92>/Magnitude-Angle to Complex' */
    u0_2 = basic_cosimulation_c_1_sm_master_B.DegRad[0];
    u1 = basic_cosimulation_c_1_sm_master_B.SFunction1_o3[0];
    re_1q = cos(u0_2);
    im_1q = sin(u0_2);
    tmp_a.re = u1 * re_1q;
    tmp_a.im = u1 * im_1q;

    /* MagnitudeAngleToComplex: '<S92>/Magnitude-Angle to Complex' */
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[0].re = tmp_a.re;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[0].im = tmp_a.im;

    /* MagnitudeAngleToComplex: '<S92>/Magnitude-Angle to Complex' */
    u0_3 = basic_cosimulation_c_1_sm_master_B.DegRad[1];
    u1_0 = basic_cosimulation_c_1_sm_master_B.SFunction1_o3[1];
    re_1r = cos(u0_3);
    im_1r = sin(u0_3);
    tmp_b.re = u1_0 * re_1r;
    tmp_b.im = u1_0 * im_1r;

    /* MagnitudeAngleToComplex: '<S92>/Magnitude-Angle to Complex' */
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[1].re = tmp_b.re;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[1].im = tmp_b.im;

    /* MagnitudeAngleToComplex: '<S92>/Magnitude-Angle to Complex' */
    u0_4 = basic_cosimulation_c_1_sm_master_B.DegRad[2];
    u1_1 = basic_cosimulation_c_1_sm_master_B.SFunction1_o3[2];
    re_1s = cos(u0_4);
    im_1s = sin(u0_4);
    tmp_c.re = u1_1 * re_1s;
    tmp_c.im = u1_1 * im_1s;

    /* MagnitudeAngleToComplex: '<S92>/Magnitude-Angle to Complex' */
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[2].re = tmp_c.re;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[2].im = tmp_c.im;

    /* RelationalOperator: '<S154>/Compare' incorporates:
     *  Constant: '<S154>/Constant'
     */
    basic_cosimulation_c_1_sm_master_B.Compare =
      (basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE <
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDiscr);

    /* RelationalOperator: '<S155>/Compare' incorporates:
     *  Constant: '<S155>/Constant'
     */
    basic_cosimulation_c_1_sm_master_B.Compare_j =
      (basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE >=
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_k);

    /* Logic: '<S153>/Logical Operator' */
    basic_cosimulation_c_1_sm_master_B.LogicalOperator =
      (basic_cosimulation_c_1_sm_master_B.Compare ||
       basic_cosimulation_c_1_sm_master_B.Compare_j);

    /* Bias: '<S156>/Bias' */
    u = -basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDiscr;

    /* Bias: '<S156>/Bias' */
    basic_cosimulation_c_1_sm_master_B.Bias =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE + u;

    /* Gain: '<S156>/Gain' */
    u = 1.0 /
      (basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_k -
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDiscr);

    /* Gain: '<S156>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_a = u *
      basic_cosimulation_c_1_sm_master_B.Bias;

    /* Rounding: '<S156>/Rounding Function' */
    basic_cosimulation_c_1_sm_master_B.RoundingFunction = floor
      (basic_cosimulation_c_1_sm_master_B.Gain_a);

    /* Gain: '<S156>/Gain1' */
    u = basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_k -
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDiscr;

    /* Gain: '<S156>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_ku = u *
      basic_cosimulation_c_1_sm_master_B.RoundingFunction;

    /* Sum: '<S156>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_g =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE -
      basic_cosimulation_c_1_sm_master_B.Gain1_ku;

    /* InitialCondition: '<S153>/Initial' */
    if (basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime) {
      basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime = false;

      /* InitialCondition: '<S153>/Initial' */
      basic_cosimulation_c_1_sm_master_B.Initial =
        basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDi_mg;
    } else {
      /* InitialCondition: '<S153>/Initial' */
      basic_cosimulation_c_1_sm_master_B.Initial =
        basic_cosimulation_c_1_sm_master_B.Sum1_g;
    }

    /* End of InitialCondition: '<S153>/Initial' */

    /* DiscreteIntegrator: '<S153>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_B.LogicalOperator) {
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE =
        basic_cosimulation_c_1_sm_master_B.Initial;
    }

    /* DiscreteIntegrator: '<S153>/Integrator' */
    basic_cosimulation_c_1_sm_master_B.Integrator =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE;

    /* Gain: '<S92>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4.re =
      basic_cosimulation_c_1_sm_master_P.Gain4_Gain_f.re *
      basic_cosimulation_c_1_sm_master_B.Integrator;
    basic_cosimulation_c_1_sm_master_B.Gain4.im =
      basic_cosimulation_c_1_sm_master_P.Gain4_Gain_f.im *
      basic_cosimulation_c_1_sm_master_B.Integrator;

    /* Math: '<S92>/Math Function' incorporates:
     *  Gain: '<S92>/Gain4'
     *
     * About '<S92>/Math Function':
     *  Operator: exp
     */
    re_1o = basic_cosimulation_c_1_sm_master_B.Gain4.re;
    im_1o = basic_cosimulation_c_1_sm_master_B.Gain4.im;
    u = exp(re_1o);
    if (im_1o == 0.0) {
      /* Math: '<S92>/Math Function'
       *
       * About '<S92>/Math Function':
       *  Operator: exp
       */
      basic_cosimulation_c_1_sm_master_B.PhaseShift.re = u;
      basic_cosimulation_c_1_sm_master_B.PhaseShift.im = 0.0;
    } else {
      /* Math: '<S92>/Math Function'
       *
       * About '<S92>/Math Function':
       *  Operator: exp
       */
      basic_cosimulation_c_1_sm_master_B.PhaseShift.re = u * cos(im_1o);
      basic_cosimulation_c_1_sm_master_B.PhaseShift.im = u * sin(im_1o);
    }

    /* End of Math: '<S92>/Math Function' */

    /* Product: '<S92>/Product' incorporates:
     *  MagnitudeAngleToComplex: '<S92>/Magnitude-Angle to Complex'
     *  Math: '<S92>/Math Function'
     *
     * About '<S92>/Math Function':
     *  Operator: exp
     */
    re = basic_cosimulation_c_1_sm_master_B.DynamicPhasor[0].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.re -
      basic_cosimulation_c_1_sm_master_B.DynamicPhasor[0].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.im;
    im = basic_cosimulation_c_1_sm_master_B.DynamicPhasor[0].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.im +
      basic_cosimulation_c_1_sm_master_B.DynamicPhasor[0].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.re;

    /* Product: '<S92>/Product' */
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[0].re = re;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[0].im = im;

    /* Product: '<S92>/Product' incorporates:
     *  MagnitudeAngleToComplex: '<S92>/Magnitude-Angle to Complex'
     *  Math: '<S92>/Math Function'
     *
     * About '<S92>/Math Function':
     *  Operator: exp
     */
    re_0 = basic_cosimulation_c_1_sm_master_B.DynamicPhasor[1].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.re -
      basic_cosimulation_c_1_sm_master_B.DynamicPhasor[1].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.im;
    im_0 = basic_cosimulation_c_1_sm_master_B.DynamicPhasor[1].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.im +
      basic_cosimulation_c_1_sm_master_B.DynamicPhasor[1].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.re;

    /* Product: '<S92>/Product' */
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[1].re = re_0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[1].im = im_0;

    /* Product: '<S92>/Product' incorporates:
     *  MagnitudeAngleToComplex: '<S92>/Magnitude-Angle to Complex'
     *  Math: '<S92>/Math Function'
     *
     * About '<S92>/Math Function':
     *  Operator: exp
     */
    re_1 = basic_cosimulation_c_1_sm_master_B.DynamicPhasor[2].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.re -
      basic_cosimulation_c_1_sm_master_B.DynamicPhasor[2].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.im;
    im_1 = basic_cosimulation_c_1_sm_master_B.DynamicPhasor[2].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.im +
      basic_cosimulation_c_1_sm_master_B.DynamicPhasor[2].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift.re;

    /* Product: '<S92>/Product' */
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[2].re = re_1;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[2].im = im_1;

    /* ComplexToRealImag: '<S92>/Complex to Real-Imag4' incorporates:
     *  Product: '<S92>/Product'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[0] =
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal[0].re;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[1] =
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal[1].re;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[2] =
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal[2].re;

    /* ComplexToRealImag: '<S92>/Complex to Real-Imag4' incorporates:
     *  Product: '<S92>/Product'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[0] =
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal[0].im;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[1] =
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal[1].im;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[2] =
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal[2].im;

    /* Switch: '<S104>/Switch' */
    for (i = 0; i < 6; i = i + 1) {
      if (basic_cosimulation_c_1_sm_master_B.UnitDelay[i] != 0.0) {
        /* Switch: '<S104>/Switch' incorporates:
         *  Constant: '<S104>/Vf Devices & Clamping Diodes'
         */
        basic_cosimulation_c_1_sm_master_B.Switch[i] =
          basic_cosimulation_c_1_sm_master_P.VfDevicesClampingDiodes_Value[i];
      } else {
        /* Switch: '<S104>/Switch' incorporates:
         *  Constant: '<S104>/Vf Diodes'
         */
        basic_cosimulation_c_1_sm_master_B.Switch[i] =
          basic_cosimulation_c_1_sm_master_P.VfDiodes_Value[i];
      }
    }

    /* End of Switch: '<S104>/Switch' */

    /* S-Function (ssn_sfun_v1): '<S174>/State-Space' */

    /* Level2 S-Function Block: '<S174>/State-Space' (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[4];
      sfcnOutputs(rts,0);
    }

    /* Gain: '<S86>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[14];

    /* Gain: '<S87>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_g =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_p *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[15];

    /* Gain: '<S88>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_n =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_a *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[16];

    /* SignalConversion generated from: '<S95>/Gain' */
    basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[0] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain;
    basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[1] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_g;
    basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[2] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_n;
    for (i_i = 0; i_i < 3; i_i = i_i + 1) {
      /* Gain: '<S95>/Gain' */
      re_1p = 0.0;
      im_1p = 0.0;
      re_1p = re_1p + basic_cosimulation_c_1_sm_master_P.Gain_Gain_l[i_i + 0].re
        * basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[0];
      im_1p = im_1p + basic_cosimulation_c_1_sm_master_P.Gain_Gain_l[i_i + 0].im
        * basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[0];
      re_1p = re_1p + basic_cosimulation_c_1_sm_master_P.Gain_Gain_l[i_i + 3].re
        * basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[1];
      im_1p = im_1p + basic_cosimulation_c_1_sm_master_P.Gain_Gain_l[i_i + 3].im
        * basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[1];
      re_1p = re_1p + basic_cosimulation_c_1_sm_master_P.Gain_Gain_l[i_i + (3 <<
        1)].re * basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[2];
      im_1p = im_1p + basic_cosimulation_c_1_sm_master_P.Gain_Gain_l[i_i + (3 <<
        1)].im * basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[2];
      basic_cosimulation_c_1_sm_master_B.Gain[i_i].re = re_1p;
      basic_cosimulation_c_1_sm_master_B.Gain[i_i].im = im_1p;
    }

    for (i_j = 0; i_j < 3; i_j = i_j + 1) {
      /* Gain: '<S95>/Gain2' */
      basic_cosimulation_c_1_sm_master_B.Gain2_j[i_j] = 0.0;
      basic_cosimulation_c_1_sm_master_B.Gain2_j[i_j] =
        basic_cosimulation_c_1_sm_master_B.Gain2_j[i_j] +
        basic_cosimulation_c_1_sm_master_P.Gain2_Gain_p[i_j + 0] *
        basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[0];
      basic_cosimulation_c_1_sm_master_B.Gain2_j[i_j] =
        basic_cosimulation_c_1_sm_master_B.Gain2_j[i_j] +
        basic_cosimulation_c_1_sm_master_P.Gain2_Gain_p[i_j + 3] *
        basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[1];
      basic_cosimulation_c_1_sm_master_B.Gain2_j[i_j] =
        basic_cosimulation_c_1_sm_master_B.Gain2_j[i_j] +
        basic_cosimulation_c_1_sm_master_P.Gain2_Gain_p[i_j + (3 << 1)] *
        basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[2];
    }

    /* Sum: '<S95>/Sum1' incorporates:
     *  Gain: '<S95>/Gain'
     */
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[0].re =
      basic_cosimulation_c_1_sm_master_B.Gain[0].re +
      basic_cosimulation_c_1_sm_master_B.Gain2_j[0];
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[0].im =
      basic_cosimulation_c_1_sm_master_B.Gain[0].im + 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[1].re =
      basic_cosimulation_c_1_sm_master_B.Gain[1].re +
      basic_cosimulation_c_1_sm_master_B.Gain2_j[1];
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[1].im =
      basic_cosimulation_c_1_sm_master_B.Gain[1].im + 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[2].re =
      basic_cosimulation_c_1_sm_master_B.Gain[2].re +
      basic_cosimulation_c_1_sm_master_B.Gain2_j[2];
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[2].im =
      basic_cosimulation_c_1_sm_master_B.Gain[2].im + 0.0;

    /* RelationalOperator: '<S166>/Compare' incorporates:
     *  Constant: '<S166>/Constant'
     */
    basic_cosimulation_c_1_sm_master_B.Compare_h =
      (basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a <
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_h);

    /* RelationalOperator: '<S167>/Compare' incorporates:
     *  Constant: '<S167>/Constant'
     */
    basic_cosimulation_c_1_sm_master_B.Compare_i =
      (basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a >=
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_m);

    /* Logic: '<S165>/Logical Operator' */
    basic_cosimulation_c_1_sm_master_B.LogicalOperator_o =
      (basic_cosimulation_c_1_sm_master_B.Compare_h ||
       basic_cosimulation_c_1_sm_master_B.Compare_i);

    /* Bias: '<S168>/Bias' */
    u = -basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_h;

    /* Bias: '<S168>/Bias' */
    basic_cosimulation_c_1_sm_master_B.Bias_l =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a + u;

    /* Gain: '<S168>/Gain' */
    u = 1.0 /
      (basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_m -
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_h);

    /* Gain: '<S168>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_j = u *
      basic_cosimulation_c_1_sm_master_B.Bias_l;

    /* Rounding: '<S168>/Rounding Function' */
    basic_cosimulation_c_1_sm_master_B.RoundingFunction_e = floor
      (basic_cosimulation_c_1_sm_master_B.Gain_j);

    /* Gain: '<S168>/Gain1' */
    u = basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_m -
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_h;

    /* Gain: '<S168>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_e = u *
      basic_cosimulation_c_1_sm_master_B.RoundingFunction_e;

    /* Sum: '<S168>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_p =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a -
      basic_cosimulation_c_1_sm_master_B.Gain1_e;

    /* InitialCondition: '<S165>/Initial' */
    if (basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime_i) {
      basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime_i = false;

      /* InitialCondition: '<S165>/Initial' */
      basic_cosimulation_c_1_sm_master_B.Initial_b =
        basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_a;
    } else {
      /* InitialCondition: '<S165>/Initial' */
      basic_cosimulation_c_1_sm_master_B.Initial_b =
        basic_cosimulation_c_1_sm_master_B.Sum1_p;
    }

    /* End of InitialCondition: '<S165>/Initial' */

    /* DiscreteIntegrator: '<S165>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_B.LogicalOperator_o) {
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a =
        basic_cosimulation_c_1_sm_master_B.Initial_b;
    }

    /* DiscreteIntegrator: '<S165>/Integrator' */
    basic_cosimulation_c_1_sm_master_B.Integrator_m =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a;

    /* Gain: '<S95>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4_e.re =
      basic_cosimulation_c_1_sm_master_P.Gain4_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.Integrator_m;
    basic_cosimulation_c_1_sm_master_B.Gain4_e.im =
      basic_cosimulation_c_1_sm_master_P.Gain4_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.Integrator_m;

    /* Math: '<S95>/Math Function' incorporates:
     *  Gain: '<S95>/Gain4'
     *
     * About '<S95>/Math Function':
     *  Operator: exp
     */
    re_1o = basic_cosimulation_c_1_sm_master_B.Gain4_e.re;
    im_1o = basic_cosimulation_c_1_sm_master_B.Gain4_e.im;
    u = exp(re_1o);
    if (im_1o == 0.0) {
      /* Math: '<S95>/Math Function'
       *
       * About '<S95>/Math Function':
       *  Operator: exp
       */
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.re = u;
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.im = 0.0;
    } else {
      /* Math: '<S95>/Math Function'
       *
       * About '<S95>/Math Function':
       *  Operator: exp
       */
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.re = u * cos(im_1o);
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.im = u * sin(im_1o);
    }

    /* End of Math: '<S95>/Math Function' */

    /* Product: '<S95>/Product' incorporates:
     *  Math: '<S95>/Math Function'
     *  Sum: '<S95>/Sum1'
     *
     * About '<S95>/Math Function':
     *  Operator: exp
     */
    re_2 = basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[0].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.re -
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[0].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.im;
    im_2 = basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[0].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.im +
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[0].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.re;

    /* Product: '<S95>/Product' */
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[0].re = re_2;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[0].im = im_2;

    /* Product: '<S95>/Product' incorporates:
     *  Math: '<S95>/Math Function'
     *  Sum: '<S95>/Sum1'
     *
     * About '<S95>/Math Function':
     *  Operator: exp
     */
    re_3 = basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[1].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.re -
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[1].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.im;
    im_3 = basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[1].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.im +
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[1].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.re;

    /* Product: '<S95>/Product' */
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[1].re = re_3;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[1].im = im_3;

    /* Product: '<S95>/Product' incorporates:
     *  Math: '<S95>/Math Function'
     *  Sum: '<S95>/Sum1'
     *
     * About '<S95>/Math Function':
     *  Operator: exp
     */
    re_4 = basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[2].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.re -
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[2].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.im;
    im_4 = basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[2].re *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.im +
      basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[2].im *
      basic_cosimulation_c_1_sm_master_B.PhaseShift_i.re;

    /* Product: '<S95>/Product' */
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[2].re = re_4;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[2].im = im_4;

    /* ComplexToMagnitudeAngle: '<S95>/Complex to Magnitude-Angle' incorporates:
     *  Product: '<S95>/Product'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[0] =
      rt_hypotd_snf(basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[0].re,
                    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[0].im);
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[1] =
      rt_hypotd_snf(basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[1].re,
                    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[1].im);
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[2] =
      rt_hypotd_snf(basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[2].re,
                    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[2].im);

    /* ComplexToMagnitudeAngle: '<S95>/Complex to Magnitude-Angle' incorporates:
     *  Product: '<S95>/Product'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[0] =
      rt_atan2d_snf(basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[0].im,
                    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[0].re);
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[1] =
      rt_atan2d_snf(basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[1].im,
                    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[1].re);
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[2] =
      rt_atan2d_snf(basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[2].im,
                    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[2].re);

    /* Gain: '<S95>/Rad->Deg' */
    basic_cosimulation_c_1_sm_master_B.RadDeg[0] =
      basic_cosimulation_c_1_sm_master_P.RadDeg_Gain *
      basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[0];
    basic_cosimulation_c_1_sm_master_B.RadDeg[1] =
      basic_cosimulation_c_1_sm_master_P.RadDeg_Gain *
      basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[1];
    basic_cosimulation_c_1_sm_master_B.RadDeg[2] =
      basic_cosimulation_c_1_sm_master_P.RadDeg_Gain *
      basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[2];

    /* Delay: '<S9>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_l =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_b;

    /* Gain: '<S111>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6 =
      basic_cosimulation_c_1_sm_master_P.Gain6_Gain *
      basic_cosimulation_c_1_sm_master_B.Delay_l;

    /* Delay: '<S9>/Delay1' */
    basic_cosimulation_c_1_sm_master_B.Delay1 =
      basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE;

    /* Sum: '<S111>/Sum4' */
    basic_cosimulation_c_1_sm_master_B.Sum4_o =
      basic_cosimulation_c_1_sm_master_B.Gain6 +
      basic_cosimulation_c_1_sm_master_B.Delay1;

    /* Gain: '<S111>/Gain7' */
    basic_cosimulation_c_1_sm_master_B.Gain7 =
      basic_cosimulation_c_1_sm_master_P.Gain7_Gain *
      basic_cosimulation_c_1_sm_master_B.Sum4_o;

    /* Sum: '<S111>/Sum5' */
    basic_cosimulation_c_1_sm_master_B.Sum5_c =
      basic_cosimulation_c_1_sm_master_B.Delay1 -
      basic_cosimulation_c_1_sm_master_B.Delay_l;

    /* Gain: '<S111>/Gain8' */
    basic_cosimulation_c_1_sm_master_B.Gain8 =
      basic_cosimulation_c_1_sm_master_P.Gain8_Gain *
      basic_cosimulation_c_1_sm_master_B.Sum5_c;

    /* Gain: '<S111>/Gain10' */
    basic_cosimulation_c_1_sm_master_B.Gain10 =
      basic_cosimulation_c_1_sm_master_P.Gain10_Gain *
      basic_cosimulation_c_1_sm_master_B.Delay1;

    /* Sum: '<S111>/Sum6' */
    Bias = 0.0;

    /* Sum: '<S111>/Sum6' */
    basic_cosimulation_c_1_sm_master_B.Sum6_b = (Bias -
      basic_cosimulation_c_1_sm_master_B.Gain10) -
      basic_cosimulation_c_1_sm_master_B.Delay_l;

    /* Gain: '<S111>/Gain9' */
    basic_cosimulation_c_1_sm_master_B.Gain9 =
      basic_cosimulation_c_1_sm_master_P.Gain9_Gain *
      basic_cosimulation_c_1_sm_master_B.Sum6_b;

    /* Delay: '<S7>/Delay2' */
    basic_cosimulation_c_1_sm_master_B.Delay2[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[0];
    basic_cosimulation_c_1_sm_master_B.Delay2[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1];
    basic_cosimulation_c_1_sm_master_B.Delay2[2] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[2];

    /* Delay: '<S7>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_c =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_f;

    /* Gain: '<S46>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_h =
      basic_cosimulation_c_1_sm_master_P.Gain6_Gain_p *
      basic_cosimulation_c_1_sm_master_B.Delay_c;

    /* Delay: '<S7>/Delay1' */
    basic_cosimulation_c_1_sm_master_B.Delay1_i =
      basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE_a;

    /* Sum: '<S46>/Sum4' */
    basic_cosimulation_c_1_sm_master_B.Sum4_f =
      basic_cosimulation_c_1_sm_master_B.Gain6_h +
      basic_cosimulation_c_1_sm_master_B.Delay1_i;

    /* Gain: '<S46>/Gain7' */
    basic_cosimulation_c_1_sm_master_B.Gain7_b =
      basic_cosimulation_c_1_sm_master_P.Gain7_Gain_d *
      basic_cosimulation_c_1_sm_master_B.Sum4_f;

    /* Sum: '<S46>/Sum5' */
    basic_cosimulation_c_1_sm_master_B.Sum5_b =
      basic_cosimulation_c_1_sm_master_B.Delay1_i -
      basic_cosimulation_c_1_sm_master_B.Delay_c;

    /* Gain: '<S46>/Gain8' */
    basic_cosimulation_c_1_sm_master_B.Gain8_i =
      basic_cosimulation_c_1_sm_master_P.Gain8_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Sum5_b;

    /* Gain: '<S46>/Gain10' */
    basic_cosimulation_c_1_sm_master_B.Gain10_i =
      basic_cosimulation_c_1_sm_master_P.Gain10_Gain_a *
      basic_cosimulation_c_1_sm_master_B.Delay1_i;

    /* Sum: '<S46>/Sum6' */
    Bias = 0.0;

    /* Sum: '<S46>/Sum6' */
    basic_cosimulation_c_1_sm_master_B.Sum6_f = (Bias -
      basic_cosimulation_c_1_sm_master_B.Gain10_i) -
      basic_cosimulation_c_1_sm_master_B.Delay_c;

    /* Gain: '<S46>/Gain9' */
    basic_cosimulation_c_1_sm_master_B.Gain9_n =
      basic_cosimulation_c_1_sm_master_P.Gain9_Gain_n *
      basic_cosimulation_c_1_sm_master_B.Sum6_f;

    /* DataTypeConversion: '<S11>/OpInput' incorporates:
     *  Constant: '<S11>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput =
      basic_cosimulation_c_1_sm_master_P.In1_Value;

    /* DataTypeConversion: '<S10>/OpInput' incorporates:
     *  Constant: '<S10>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput_a =
      basic_cosimulation_c_1_sm_master_P.In1_Value_d;

    /* DataTypeConversion: '<S13>/OpInput' incorporates:
     *  Constant: '<S13>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput_p =
      basic_cosimulation_c_1_sm_master_P.In1_Value_a;

    /* DataTypeConversion: '<S12>/OpInput' incorporates:
     *  Constant: '<S12>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput_b =
      basic_cosimulation_c_1_sm_master_P.In1_Value_dr;

    /* DataTypeConversion: '<S16>/OpInput' incorporates:
     *  Constant: '<S16>/In1'
     */
    basic_cosimulation_c_1_sm_master_B.OpInput_f =
      basic_cosimulation_c_1_sm_master_P.In1_Value_i;

    /* S-Function (OP_SEND): '<S172>/S-Function' */

    /* Level2 S-Function Block: '<S172>/S-Function' (OP_SEND) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[5];
      sfcnOutputs(rts,0);
    }

    /* Delay: '<S3>/Delay4' */
    tmp = (int32_T)basic_cosimulation_c_1_sm_master_DW.CircBufIdx * 6;
    for (i_k = 0; i_k < 6; i_k = i_k + 1) {
      /* Delay: '<S3>/Delay4' */
      basic_cosimulation_c_1_sm_master_B.Delay4[i_k] =
        basic_cosimulation_c_1_sm_master_DW.Delay4_DSTATE[tmp + i_k];
    }

    /* S-Function (opwritefile): '<S8>/OpWriteFile26' */

    /* Level2 S-Function Block: '<S8>/OpWriteFile26' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[6];
      sfcnOutputs(rts,0);
    }

    /* Delay: '<S3>/Delay5' */
    tmp_0 = (int32_T)basic_cosimulation_c_1_sm_master_DW.CircBufIdx_d * 6;
    for (i_l = 0; i_l < 6; i_l = i_l + 1) {
      /* Delay: '<S3>/Delay5' */
      basic_cosimulation_c_1_sm_master_B.Delay5[i_l] =
        basic_cosimulation_c_1_sm_master_DW.Delay5_DSTATE[tmp_0 + i_l];
    }

    /* S-Function (opwritefile): '<S8>/OpWriteFile27' */

    /* Level2 S-Function Block: '<S8>/OpWriteFile27' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[7];
      sfcnOutputs(rts,0);
    }

    /* Delay: '<S3>/Delay6' */
    tmp_1 = (int32_T)basic_cosimulation_c_1_sm_master_DW.CircBufIdx_a * 3;

    /* Delay: '<S3>/Delay6' */
    basic_cosimulation_c_1_sm_master_B.Delay6[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay6_DSTATE[tmp_1 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay6[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay6_DSTATE[tmp_1 + 1];
    basic_cosimulation_c_1_sm_master_B.Delay6[2] =
      basic_cosimulation_c_1_sm_master_DW.Delay6_DSTATE[tmp_1 + 2];

    /* S-Function (opwritefile): '<S8>/OpWriteFile28' */

    /* Level2 S-Function Block: '<S8>/OpWriteFile28' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[8];
      sfcnOutputs(rts,0);
    }

    /* Delay: '<S3>/Delay7' */
    tmp_2 = (int32_T)basic_cosimulation_c_1_sm_master_DW.CircBufIdx_o * 6;
    for (i_m = 0; i_m < 6; i_m = i_m + 1) {
      /* Delay: '<S3>/Delay7' */
      basic_cosimulation_c_1_sm_master_B.Delay7[i_m] =
        basic_cosimulation_c_1_sm_master_DW.Delay7_DSTATE[tmp_2 + i_m];
    }

    /* S-Function (opwritefile): '<S8>/OpWriteFile29' */

    /* Level2 S-Function Block: '<S8>/OpWriteFile29' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[9];
      sfcnOutputs(rts,0);
    }

    /* Delay: '<S3>/Delay8' */
    tmp_3 = (int32_T)basic_cosimulation_c_1_sm_master_DW.CircBufIdx_g << 1;

    /* Delay: '<S3>/Delay8' */
    basic_cosimulation_c_1_sm_master_B.Delay8[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay8_DSTATE[tmp_3 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay8[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay8_DSTATE[tmp_3 + 1];

    /* S-Function (opwritefile): '<S8>/OpWriteFile30' */

    /* Level2 S-Function Block: '<S8>/OpWriteFile30' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[10];
      sfcnOutputs(rts,0);
    }

    for (i_b = 0; i_b < 6; i_b = i_b + 1) {
      /* UnitDelay: '<S37>/Unit Delay' */
      basic_cosimulation_c_1_sm_master_B.UnitDelay_c[i_b] =
        basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE_g[i_b];
    }

    /* Constant: '<S28>/Constant11' */
    basic_cosimulation_c_1_sm_master_B.Constant11_i =
      basic_cosimulation_c_1_sm_master_P.fsw_inv;

    /* Delay: '<S45>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_b[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[0];
    basic_cosimulation_c_1_sm_master_B.Delay_b[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[1];
  }

  /* RelationalOperator: '<S51>/Relational Operator' incorporates:
   *  Constant: '<S51>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.RelationalOperator_i =
    (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE >
     basic_cosimulation_c_1_sm_master_P.Constant2_Value_o);

  /* Sum: '<S51>/Subtract' incorporates:
   *  Constant: '<S51>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Subtract_h =
    basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_o;

  /* Integrator: '<S51>/Integrator2' */
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

  /* Integrator: '<S51>/Integrator2' */
  basic_cosimulation_c_1_sm_master_B.Integrator2 =
    basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE;

  /* Trigonometry: '<S52>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_h = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Product: '<S52>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_k =
    basic_cosimulation_c_1_sm_master_B.Delay_b[0] *
    basic_cosimulation_c_1_sm_master_B.Cos_h;

  /* Trigonometry: '<S52>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_j = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Gain: '<S52>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_i =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_o *
    basic_cosimulation_c_1_sm_master_B.Cos1_j;

  /* Product: '<S52>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_cr =
    basic_cosimulation_c_1_sm_master_B.Delay_b[1] *
    basic_cosimulation_c_1_sm_master_B.Gain_i;

  /* Sum: '<S52>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_n =
    basic_cosimulation_c_1_sm_master_B.Product_k +
    basic_cosimulation_c_1_sm_master_B.Product1_cr;

  /* Sum: '<S52>/Sum6' incorporates:
   *  Constant: '<S52>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum6_g =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_h;

  /* Trigonometry: '<S52>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_m = cos
    (basic_cosimulation_c_1_sm_master_B.Sum6_g);

  /* Product: '<S52>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_g =
    basic_cosimulation_c_1_sm_master_B.Delay_b[0] *
    basic_cosimulation_c_1_sm_master_B.Cos2_m;

  /* Sum: '<S52>/Sum2' incorporates:
   *  Constant: '<S52>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_m =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_g;

  /* Trigonometry: '<S52>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_g = sin
    (basic_cosimulation_c_1_sm_master_B.Sum2_m);

  /* Gain: '<S52>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_o =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_gz *
    basic_cosimulation_c_1_sm_master_B.Cos3_g;

  /* Product: '<S52>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_h =
    basic_cosimulation_c_1_sm_master_B.Delay_b[1] *
    basic_cosimulation_c_1_sm_master_B.Gain1_o;

  /* Sum: '<S52>/Sum3' */
  basic_cosimulation_c_1_sm_master_B.Sum3_m =
    basic_cosimulation_c_1_sm_master_B.Product2_g +
    basic_cosimulation_c_1_sm_master_B.Product3_h;

  /* Sum: '<S52>/Sum7' incorporates:
   *  Constant: '<S52>/Constant4'
   */
  basic_cosimulation_c_1_sm_master_B.Sum7_c =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant4_Value_n;

  /* Trigonometry: '<S52>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_f = cos
    (basic_cosimulation_c_1_sm_master_B.Sum7_c);

  /* Product: '<S52>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_i =
    basic_cosimulation_c_1_sm_master_B.Delay_b[0] *
    basic_cosimulation_c_1_sm_master_B.Cos4_f;

  /* Sum: '<S52>/Sum4' incorporates:
   *  Constant: '<S52>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum4_d =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_e;

  /* Trigonometry: '<S52>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_c = sin
    (basic_cosimulation_c_1_sm_master_B.Sum4_d);

  /* Gain: '<S52>/Gain2' */
  basic_cosimulation_c_1_sm_master_B.Gain2_e =
    basic_cosimulation_c_1_sm_master_P.Gain2_Gain_o *
    basic_cosimulation_c_1_sm_master_B.Cos5_c;

  /* Product: '<S52>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_m =
    basic_cosimulation_c_1_sm_master_B.Delay_b[1] *
    basic_cosimulation_c_1_sm_master_B.Gain2_e;

  /* Sum: '<S52>/Sum5' */
  basic_cosimulation_c_1_sm_master_B.Sum5_e =
    basic_cosimulation_c_1_sm_master_B.Product4_i +
    basic_cosimulation_c_1_sm_master_B.Product5_m;

  /* Product: '<S22>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_f[0] =
    basic_cosimulation_c_1_sm_master_B.Sum1_n *
    basic_cosimulation_c_1_sm_master_B.SFunction_l[0];
  basic_cosimulation_c_1_sm_master_B.Product2_f[1] =
    basic_cosimulation_c_1_sm_master_B.Sum3_m *
    basic_cosimulation_c_1_sm_master_B.SFunction_l[0];
  basic_cosimulation_c_1_sm_master_B.Product2_f[2] =
    basic_cosimulation_c_1_sm_master_B.Sum5_e *
    basic_cosimulation_c_1_sm_master_B.SFunction_l[0];

  /* Saturate: '<S22>/Saturation' */
  u0_5 = basic_cosimulation_c_1_sm_master_B.Product2_f[0];
  if (u0_5 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l) {
    y_2 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l;
  } else if (u0_5 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f) {
    y_2 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f;
  } else {
    y_2 = u0_5;
  }

  /* Saturate: '<S22>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation_g[0] = y_2;

  /* Saturate: '<S22>/Saturation' */
  u0_6 = basic_cosimulation_c_1_sm_master_B.Product2_f[1];
  if (u0_6 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l) {
    y_3 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l;
  } else if (u0_6 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f) {
    y_3 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f;
  } else {
    y_3 = u0_6;
  }

  /* Saturate: '<S22>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation_g[1] = y_3;

  /* Saturate: '<S22>/Saturation' */
  u0_7 = basic_cosimulation_c_1_sm_master_B.Product2_f[2];
  if (u0_7 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l) {
    y_4 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_l;
  } else if (u0_7 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f) {
    y_4 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_f;
  } else {
    y_4 = u0_7;
  }

  /* Saturate: '<S22>/Saturation' */
  basic_cosimulation_c_1_sm_master_B.Saturation_g[2] = y_4;

  /* Sum: '<S28>/Sum16' incorporates:
   *  Constant: '<S28>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum16_m =
    basic_cosimulation_c_1_sm_master_B.Saturation_g[0] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value_k;

  /* Product: '<S28>/Divide7' incorporates:
   *  Constant: '<S28>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide7_i =
    basic_cosimulation_c_1_sm_master_B.Sum16_m /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value_l;

  /* Sum: '<S28>/Sum17' incorporates:
   *  Constant: '<S28>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum17_d =
    basic_cosimulation_c_1_sm_master_B.Saturation_g[1] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value_k;

  /* Product: '<S28>/Divide8' incorporates:
   *  Constant: '<S28>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide8_p =
    basic_cosimulation_c_1_sm_master_B.Sum17_d /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value_l;

  /* Sum: '<S28>/Sum18' incorporates:
   *  Constant: '<S28>/Constant9'
   */
  basic_cosimulation_c_1_sm_master_B.Sum18_b =
    basic_cosimulation_c_1_sm_master_B.Saturation_g[2] +
    basic_cosimulation_c_1_sm_master_P.Constant9_Value_k;

  /* Product: '<S28>/Divide9' incorporates:
   *  Constant: '<S28>/Constant10'
   */
  basic_cosimulation_c_1_sm_master_B.Divide9_d =
    basic_cosimulation_c_1_sm_master_B.Sum18_b /
    basic_cosimulation_c_1_sm_master_P.Constant10_Value_l;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* SignalConversion generated from: '<S28>/RTE SPWM1' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f[0] =
      basic_cosimulation_c_1_sm_master_B.Divide7_i;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f[1] =
      basic_cosimulation_c_1_sm_master_B.Divide8_p;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtRTESPWM1_f[2] =
      basic_cosimulation_c_1_sm_master_B.Divide9_d;

    /* S-Function (rte_svpwm): '<S28>/RTE SPWM1' */

    /* Level2 S-Function Block: '<S28>/RTE SPWM1' (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[11];
      sfcnOutputs(rts,0);
    }

    /* S-Function (rte_conversion): '<S28>/RTE Conversion1' */

    /* Level2 S-Function Block: '<S28>/RTE Conversion1' (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[12];
      sfcnOutputs(rts,0);
    }

    /* DataTypeConversion: '<S40>/Data Type Conversion' */
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

    /* Outputs for Enabled SubSystem: '<S37>/Tail' */
    /* Outputs for Enabled SubSystem: '<S37>/Tail' */
    /* Constant: '<S37>/Not in ARTEMIS' */
    basic_cosimulation_c_1_sm__Tail(basic_cosimulation_c_1_sm_master_M,
      basic_cosimulation_c_1_sm_master_P.NotinARTEMIS_Value_k,
      basic_cosimulation_c_1_sm_master_B.UnitDelay_c,
      basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o,
      basic_cosimulation_c_1_sm_master_P.Tail_Tf_a,
      basic_cosimulation_c_1_sm_master_P.Tail_Tt_m,
      &basic_cosimulation_c_1_sm_master_B.Tail,
      &basic_cosimulation_c_1_sm_master_DW.Tail,
      &basic_cosimulation_c_1_sm_master_P.Tail);

    /* End of Outputs for SubSystem: '<S37>/Tail' */
    /* End of Outputs for SubSystem: '<S37>/Tail' */

    /* Constant: '<S73>/DC' */
    basic_cosimulation_c_1_sm_master_B.DC_h =
      basic_cosimulation_c_1_sm_master_P.Vdc;

    /* Sin: '<S78>/Sine Wave A' */
    if (basic_cosimulation_c_1_sm_master_DW.systemEnable != 0) {
      u = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastSin = sin(u *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      u = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastCos = cos(u *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      basic_cosimulation_c_1_sm_master_DW.systemEnable = 0;
    }

    /* Sin: '<S78>/Sine Wave A' */
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

    /* Sin: '<S78>/Sine Wave B' */
    if (basic_cosimulation_c_1_sm_master_DW.systemEnable_n != 0) {
      u = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastSin_f = sin(u *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      u = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastCos_m = cos(u *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      basic_cosimulation_c_1_sm_master_DW.systemEnable_n = 0;
    }

    /* Sin: '<S78>/Sine Wave B' */
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

    /* Sin: '<S78>/Sine Wave C' */
    if (basic_cosimulation_c_1_sm_master_DW.systemEnable_j != 0) {
      u = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastSin_k = sin(u *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      u = 2.0 * 3.1415926535897931 *
        basic_cosimulation_c_1_sm_master_P.ThreePhaseSource_Frequency;
      basic_cosimulation_c_1_sm_master_DW.lastCos_i = cos(u *
        basic_cosimulation_c_1_sm_master_M->Timing.t[1]);
      basic_cosimulation_c_1_sm_master_DW.systemEnable_j = 0;
    }

    /* Sin: '<S78>/Sine Wave C' */
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

    /* Switch: '<S39>/Switch' */
    for (i = 0; i < 6; i = i + 1) {
      if (basic_cosimulation_c_1_sm_master_B.UnitDelay_c[i] != 0.0) {
        /* Switch: '<S39>/Switch' incorporates:
         *  Constant: '<S39>/Vf Devices & Clamping Diodes'
         */
        basic_cosimulation_c_1_sm_master_B.Switch_d[i] =
          basic_cosimulation_c_1_sm_master_P.VfDevicesClampingDiodes_Value_p[i];
      } else {
        /* Switch: '<S39>/Switch' incorporates:
         *  Constant: '<S39>/Vf Diodes'
         */
        basic_cosimulation_c_1_sm_master_B.Switch_d[i] =
          basic_cosimulation_c_1_sm_master_P.VfDiodes_Value_b[i];
      }
    }

    /* End of Switch: '<S39>/Switch' */

    /* S-Function (ssn_sfun_v1): '<S173>/State-Space' */

    /* Level2 S-Function Block: '<S173>/State-Space' (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[13];
      sfcnOutputs(rts,0);
    }

    for (i_0 = 0; i_0 < 6; i_0 = i_0 + 1) {
      /* Gain: '<S37>/1//Ron' */
      basic_cosimulation_c_1_sm_master_B.uRon[i_0] =
        basic_cosimulation_c_1_sm_master_P.uRon_Gain *
        basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[i_0];
    }

    /* Switch: '<S37>/Switch' */
    for (i = 0; i < 6; i = i + 1) {
      if (basic_cosimulation_c_1_sm_master_B.StateSpace_o2_c[i] >=
          basic_cosimulation_c_1_sm_master_P.Switch_Threshold) {
        /* Switch: '<S37>/Switch' */
        basic_cosimulation_c_1_sm_master_B.Switch_g[i] =
          basic_cosimulation_c_1_sm_master_B.uRon[i];
      } else {
        /* Switch: '<S37>/Switch' incorporates:
         *  Constant: '<S37>/0 4'
         */
        basic_cosimulation_c_1_sm_master_B.Switch_g[i] =
          basic_cosimulation_c_1_sm_master_P.u4_Value;
      }
    }

    /* End of Switch: '<S37>/Switch' */
    for (i_1 = 0; i_1 < 6; i_1 = i_1 + 1) {
      /* Saturate: '<S37>/Saturation' */
      u0_8 = basic_cosimulation_c_1_sm_master_B.Switch_g[i_1];
      if (u0_8 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_j) {
        y_5 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_j;
      } else if (u0_8 <
                 basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_fu) {
        y_5 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_fu;
      } else {
        y_5 = u0_8;
      }

      /* Saturate: '<S37>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_h[i_1] = y_5;
    }

    /* Gain: '<S34>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_h *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[8];

    /* Gain: '<S35>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_h =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_ha *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[9];

    /* Gain: '<S36>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_f =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_b *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[10];

    /* Delay: '<S47>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_f[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[0];
    basic_cosimulation_c_1_sm_master_B.Delay_f[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[1];

    /* Delay: '<S48>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_i[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[0];
    basic_cosimulation_c_1_sm_master_B.Delay_i[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[1];
  }

  /* Integrator: '<S57>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_c[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S57>/Transport Delay' */
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
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F) : 0.0);
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
    /* Sum: '<S57>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_k[i_c] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c[i_c] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay[i_c];
  }

  /* Gain: '<S57>/Gain' */
  u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;
  for (i_2 = 0; i_2 < 8; i_2 = i_2 + 1) {
    /* Gain: '<S57>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_hs[i_2] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract_k[i_2];
  }

  /* RealImagToComplex: '<S57>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_hs[3 + 4];

  /* Integrator: '<S59>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_e[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_a[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S59>/Transport Delay' */
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
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F) : 0.0);
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
    /* Sum: '<S59>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_p[i_d] =
      basic_cosimulation_c_1_sm_master_B.Integrator_e[i_d] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_m[i_d];
  }

  /* Gain: '<S59>/Gain' */
  u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;
  for (i_3 = 0; i_3 < 8; i_3 = i_3 + 1) {
    /* Gain: '<S59>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_o[i_3] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract_p[i_3];
  }

  /* RealImagToComplex: '<S59>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_o[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_o[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_o[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_o[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_o[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_o[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_o[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_o[3 + 4];

  /* Integrator: '<S61>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_g[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_d[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S61>/Transport Delay' */
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
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F) : 0.0);
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
    /* Sum: '<S61>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_b[i_e] =
      basic_cosimulation_c_1_sm_master_B.Integrator_g[i_e] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_a[i_e];
  }

  /* Gain: '<S61>/Gain' */
  u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;
  for (i_4 = 0; i_4 < 8; i_4 = i_4 + 1) {
    /* Gain: '<S61>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_hk[i_4] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract_b[i_4];
  }

  /* RealImagToComplex: '<S61>/Real-Imag to Complex' */
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

  /* Integrator: '<S56>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_b[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_g[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S56>/Transport Delay' */
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
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F) : 0.0);
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
    /* Sum: '<S56>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_b2[i_f] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b[i_f] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_d[i_f];
  }

  /* Gain: '<S56>/Gain' */
  u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;
  for (i_5 = 0; i_5 < 8; i_5 = i_5 + 1) {
    /* Gain: '<S56>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_b[i_5] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract_b2[i_5];
  }

  /* RealImagToComplex: '<S56>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_b[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_b[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_b[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_b[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_b[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_b[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_b[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_b[3 + 4];

  /* Integrator: '<S58>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_j[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_m[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S58>/Transport Delay' */
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
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F) : 0.0);
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
    /* Sum: '<S58>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_i[i_g] =
      basic_cosimulation_c_1_sm_master_B.Integrator_j[i_g] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_k[i_g];
  }

  /* Gain: '<S58>/Gain' */
  u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;
  for (i_6 = 0; i_6 < 8; i_6 = i_6 + 1) {
    /* Gain: '<S58>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_d[i_6] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract_i[i_6];
  }

  /* RealImagToComplex: '<S58>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_d[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_d[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_d[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_d[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_d[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_d[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_d[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_d[3 + 4];

  /* Integrator: '<S60>/Integrator' */
  memcpy(&basic_cosimulation_c_1_sm_master_B.Integrator_n[0],
         &basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_h[0], 8U * sizeof
         (real_T));

  /* TransportDelay: '<S60>/Transport Delay' */
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
                         basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F)
                        > 0.0) ? (1.0 /
          basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F) : 0.0);
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
    /* Sum: '<S60>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_by[i_h] =
      basic_cosimulation_c_1_sm_master_B.Integrator_n[i_h] -
      basic_cosimulation_c_1_sm_master_B.TransportDelay_p[i_h];
  }

  /* Gain: '<S60>/Gain' */
  u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;
  for (i_7 = 0; i_7 < 8; i_7 = i_7 + 1) {
    /* Gain: '<S60>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_bo[i_7] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract_by[i_7];
  }

  /* RealImagToComplex: '<S60>/Real-Imag to Complex' */
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].re =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[0];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].im =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[0 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].re =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[1];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].im =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[1 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].re =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[2];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].im =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[2 + 4];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].re =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[3];
  basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].im =
    basic_cosimulation_c_1_sm_master_B.Gain_bo[3 + 4];

  /* MultiPortSwitch: '<S53>/Multiport Switch' incorporates:
   *  Constant: '<S53>/Sequence'
   */
  switch ((int32_T)basic_cosimulation_c_1_sm_master_P.Sequence_Value) {
   case 1:
    /* Gain: '<S62>/alpha2' incorporates:
     *  RealImagToComplex: '<S60>/Real-Imag to Complex'
     */
    re_5 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].im;
    im_5 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].re;

    /* Gain: '<S62>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_j[0].re = re_5;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[0].im = im_5;

    /* Gain: '<S62>/alpha2' incorporates:
     *  RealImagToComplex: '<S60>/Real-Imag to Complex'
     */
    re_7 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].im;
    im_7 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].re;

    /* Gain: '<S62>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_j[1].re = re_7;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[1].im = im_7;

    /* Gain: '<S62>/alpha2' incorporates:
     *  RealImagToComplex: '<S60>/Real-Imag to Complex'
     */
    re_9 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].im;
    im_9 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].re;

    /* Gain: '<S62>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_j[2].re = re_9;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[2].im = im_9;

    /* Gain: '<S62>/alpha2' incorporates:
     *  RealImagToComplex: '<S60>/Real-Imag to Complex'
     */
    re_b = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].im;
    im_b = basic_cosimulation_c_1_sm_master_P.alpha2_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].re;

    /* Gain: '<S62>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_j[3].re = re_b;
    basic_cosimulation_c_1_sm_master_B.alpha2_j[3].im = im_b;

    /* Gain: '<S62>/alpha' incorporates:
     *  RealImagToComplex: '<S58>/Real-Imag to Complex'
     */
    re_d = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im;
    im_d = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re;

    /* Gain: '<S62>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_h[0].re = re_d;
    basic_cosimulation_c_1_sm_master_B.alpha_h[0].im = im_d;

    /* Gain: '<S62>/alpha' incorporates:
     *  RealImagToComplex: '<S58>/Real-Imag to Complex'
     */
    re_f = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im;
    im_f = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re;

    /* Gain: '<S62>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_h[1].re = re_f;
    basic_cosimulation_c_1_sm_master_B.alpha_h[1].im = im_f;

    /* Gain: '<S62>/alpha' incorporates:
     *  RealImagToComplex: '<S58>/Real-Imag to Complex'
     */
    re_h = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im;
    im_h = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re;

    /* Gain: '<S62>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_h[2].re = re_h;
    basic_cosimulation_c_1_sm_master_B.alpha_h[2].im = im_h;

    /* Gain: '<S62>/alpha' incorporates:
     *  RealImagToComplex: '<S58>/Real-Imag to Complex'
     */
    re_j = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im;
    im_j = basic_cosimulation_c_1_sm_master_P.alpha_Gain.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re;

    /* Gain: '<S62>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_h[3].re = re_j;
    basic_cosimulation_c_1_sm_master_B.alpha_h[3].im = im_j;

    /* Sum: '<S62>/Add' incorporates:
     *  Gain: '<S62>/alpha'
     *  Gain: '<S62>/alpha2'
     *  RealImagToComplex: '<S56>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_p4[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].re +
       basic_cosimulation_c_1_sm_master_B.alpha_h[0].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_p4[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].im +
       basic_cosimulation_c_1_sm_master_B.alpha_h[0].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_p4[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].re +
       basic_cosimulation_c_1_sm_master_B.alpha_h[1].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_p4[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].im +
       basic_cosimulation_c_1_sm_master_B.alpha_h[1].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_p4[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].re +
       basic_cosimulation_c_1_sm_master_B.alpha_h[2].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_p4[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].im +
       basic_cosimulation_c_1_sm_master_B.alpha_h[2].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_p4[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].re +
       basic_cosimulation_c_1_sm_master_B.alpha_h[3].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_p4[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].im +
       basic_cosimulation_c_1_sm_master_B.alpha_h[3].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_j[3].im;

    /* Gain: '<S62>/Gain1' incorporates:
     *  Sum: '<S62>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
      basic_cosimulation_c_1_sm_master_B.Add_p4[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
      basic_cosimulation_c_1_sm_master_B.Add_p4[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
      basic_cosimulation_c_1_sm_master_B.Add_p4[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
      basic_cosimulation_c_1_sm_master_B.Add_p4[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
      basic_cosimulation_c_1_sm_master_B.Add_p4[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
      basic_cosimulation_c_1_sm_master_B.Add_p4[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
      basic_cosimulation_c_1_sm_master_B.Add_p4[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain *
      basic_cosimulation_c_1_sm_master_B.Add_p4[3].im;

    /* Math: '<S53>/Math Function' incorporates:
     *  Gain: '<S62>/Gain1'
     */
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[0].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[0].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[1].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[1].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[2].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[2].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[3].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[3].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].im;

    /* Gain: '<S63>/alpha2' incorporates:
     *  RealImagToComplex: '<S61>/Real-Imag to Complex'
     */
    re_m = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im;
    im_m = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re;

    /* Gain: '<S63>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].re = re_m;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].im = im_m;

    /* Gain: '<S63>/alpha2' incorporates:
     *  RealImagToComplex: '<S61>/Real-Imag to Complex'
     */
    re_p = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im;
    im_p = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re;

    /* Gain: '<S63>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].re = re_p;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].im = im_p;

    /* Gain: '<S63>/alpha2' incorporates:
     *  RealImagToComplex: '<S61>/Real-Imag to Complex'
     */
    re_s = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im;
    im_s = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re;

    /* Gain: '<S63>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].re = re_s;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].im = im_s;

    /* Gain: '<S63>/alpha2' incorporates:
     *  RealImagToComplex: '<S61>/Real-Imag to Complex'
     */
    re_v = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im;
    im_v = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_m.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re;

    /* Gain: '<S63>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].re = re_v;
    basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].im = im_v;

    /* Gain: '<S63>/alpha' incorporates:
     *  RealImagToComplex: '<S59>/Real-Imag to Complex'
     */
    re_x = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].im;
    im_x = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].re;

    /* Gain: '<S63>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_i[0].re = re_x;
    basic_cosimulation_c_1_sm_master_B.alpha_i[0].im = im_x;

    /* Gain: '<S63>/alpha' incorporates:
     *  RealImagToComplex: '<S59>/Real-Imag to Complex'
     */
    re_z = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].im;
    im_z = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].re;

    /* Gain: '<S63>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_i[1].re = re_z;
    basic_cosimulation_c_1_sm_master_B.alpha_i[1].im = im_z;

    /* Gain: '<S63>/alpha' incorporates:
     *  RealImagToComplex: '<S59>/Real-Imag to Complex'
     */
    re_11 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].im;
    im_11 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].re;

    /* Gain: '<S63>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_i[2].re = re_11;
    basic_cosimulation_c_1_sm_master_B.alpha_i[2].im = im_11;

    /* Gain: '<S63>/alpha' incorporates:
     *  RealImagToComplex: '<S59>/Real-Imag to Complex'
     */
    re_13 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].im;
    im_13 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_a.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].re;

    /* Gain: '<S63>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_i[3].re = re_13;
    basic_cosimulation_c_1_sm_master_B.alpha_i[3].im = im_13;

    /* Sum: '<S63>/Add' incorporates:
     *  Gain: '<S63>/alpha'
     *  Gain: '<S63>/alpha2'
     *  RealImagToComplex: '<S57>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_gb[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].re +
       basic_cosimulation_c_1_sm_master_B.alpha_i[0].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_gb[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].im +
       basic_cosimulation_c_1_sm_master_B.alpha_i[0].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_gb[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].re +
       basic_cosimulation_c_1_sm_master_B.alpha_i[1].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_gb[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].im +
       basic_cosimulation_c_1_sm_master_B.alpha_i[1].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_gb[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].re +
       basic_cosimulation_c_1_sm_master_B.alpha_i[2].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_gb[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].im +
       basic_cosimulation_c_1_sm_master_B.alpha_i[2].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_gb[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].re +
       basic_cosimulation_c_1_sm_master_B.alpha_i[3].re) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_gb[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].im +
       basic_cosimulation_c_1_sm_master_B.alpha_i[3].im) +
      basic_cosimulation_c_1_sm_master_B.alpha2_jy[3].im;

    /* Gain: '<S63>/Gain1' incorporates:
     *  Sum: '<S63>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_az[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Add_gb[3].im;

    /* Product: '<S53>/Product3' incorporates:
     *  Gain: '<S63>/Gain1'
     *  Math: '<S53>/Math Function'
     */
    re_15 = basic_cosimulation_c_1_sm_master_B.Gain1_az[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[0].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_az[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[0].im;
    im_15 = basic_cosimulation_c_1_sm_master_B.Gain1_az[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[0].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_az[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[0].re;

    /* Product: '<S53>/Product3' */
    basic_cosimulation_c_1_sm_master_B.Product3_d[0].re = re_15;
    basic_cosimulation_c_1_sm_master_B.Product3_d[0].im = im_15;

    /* Product: '<S53>/Product3' incorporates:
     *  Gain: '<S63>/Gain1'
     *  Math: '<S53>/Math Function'
     */
    re_17 = basic_cosimulation_c_1_sm_master_B.Gain1_az[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[1].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_az[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[1].im;
    im_17 = basic_cosimulation_c_1_sm_master_B.Gain1_az[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[1].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_az[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[1].re;

    /* Product: '<S53>/Product3' */
    basic_cosimulation_c_1_sm_master_B.Product3_d[1].re = re_17;
    basic_cosimulation_c_1_sm_master_B.Product3_d[1].im = im_17;

    /* Product: '<S53>/Product3' incorporates:
     *  Gain: '<S63>/Gain1'
     *  Math: '<S53>/Math Function'
     */
    re_19 = basic_cosimulation_c_1_sm_master_B.Gain1_az[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[2].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_az[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[2].im;
    im_19 = basic_cosimulation_c_1_sm_master_B.Gain1_az[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[2].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_az[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[2].re;

    /* Product: '<S53>/Product3' */
    basic_cosimulation_c_1_sm_master_B.Product3_d[2].re = re_19;
    basic_cosimulation_c_1_sm_master_B.Product3_d[2].im = im_19;

    /* Product: '<S53>/Product3' incorporates:
     *  Gain: '<S63>/Gain1'
     *  Math: '<S53>/Math Function'
     */
    re_1b = basic_cosimulation_c_1_sm_master_B.Gain1_az[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[3].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_az[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[3].im;
    im_1b = basic_cosimulation_c_1_sm_master_B.Gain1_az[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[3].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_az[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction_m[3].re;

    /* Product: '<S53>/Product3' */
    basic_cosimulation_c_1_sm_master_B.Product3_d[3].re = re_1b;
    basic_cosimulation_c_1_sm_master_B.Product3_d[3].im = im_1b;

    /* Gain: '<S53>/Gain2' incorporates:
     *  Product: '<S53>/Product3'
     */
    basic_cosimulation_c_1_sm_master_B.Gain2_p[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
      basic_cosimulation_c_1_sm_master_B.Product3_d[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
      basic_cosimulation_c_1_sm_master_B.Product3_d[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
      basic_cosimulation_c_1_sm_master_B.Product3_d[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
      basic_cosimulation_c_1_sm_master_B.Product3_d[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
      basic_cosimulation_c_1_sm_master_B.Product3_d[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
      basic_cosimulation_c_1_sm_master_B.Product3_d[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
      basic_cosimulation_c_1_sm_master_B.Product3_d[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain *
      basic_cosimulation_c_1_sm_master_B.Product3_d[3].im;

    /* MultiPortSwitch: '<S53>/Multiport Switch' incorporates:
     *  Gain: '<S53>/Gain2'
     */
    memcpy(&basic_cosimulation_c_1_sm_master_B.MultiportSwitch[0],
           &basic_cosimulation_c_1_sm_master_B.Gain2_p[0], 4U * sizeof(creal_T));
    break;

   case 2:
    /* Gain: '<S54>/alpha' incorporates:
     *  RealImagToComplex: '<S60>/Real-Imag to Complex'
     */
    re_6 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].im;
    im_6 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].re;

    /* Gain: '<S54>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_mb[0].re = re_6;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[0].im = im_6;

    /* Gain: '<S54>/alpha' incorporates:
     *  RealImagToComplex: '<S60>/Real-Imag to Complex'
     */
    re_8 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].im;
    im_8 = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].re;

    /* Gain: '<S54>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_mb[1].re = re_8;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[1].im = im_8;

    /* Gain: '<S54>/alpha' incorporates:
     *  RealImagToComplex: '<S60>/Real-Imag to Complex'
     */
    re_a = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].im;
    im_a = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].re;

    /* Gain: '<S54>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_mb[2].re = re_a;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[2].im = im_a;

    /* Gain: '<S54>/alpha' incorporates:
     *  RealImagToComplex: '<S60>/Real-Imag to Complex'
     */
    re_c = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].im;
    im_c = basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_j.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].re;

    /* Gain: '<S54>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_mb[3].re = re_c;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[3].im = im_c;

    /* Gain: '<S54>/alpha2' incorporates:
     *  RealImagToComplex: '<S58>/Real-Imag to Complex'
     */
    re_e = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im;
    im_e = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re;

    /* Gain: '<S54>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_b[0].re = re_e;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[0].im = im_e;

    /* Gain: '<S54>/alpha2' incorporates:
     *  RealImagToComplex: '<S58>/Real-Imag to Complex'
     */
    re_g = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im;
    im_g = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re;

    /* Gain: '<S54>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_b[1].re = re_g;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[1].im = im_g;

    /* Gain: '<S54>/alpha2' incorporates:
     *  RealImagToComplex: '<S58>/Real-Imag to Complex'
     */
    re_i = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im;
    im_i = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re;

    /* Gain: '<S54>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_b[2].re = re_i;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[2].im = im_i;

    /* Gain: '<S54>/alpha2' incorporates:
     *  RealImagToComplex: '<S58>/Real-Imag to Complex'
     */
    re_k = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im;
    im_k = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re;

    /* Gain: '<S54>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_b[3].re = re_k;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[3].im = im_k;

    /* Sum: '<S54>/Add' incorporates:
     *  Gain: '<S54>/alpha'
     *  Gain: '<S54>/alpha2'
     *  RealImagToComplex: '<S56>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_a[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_b[0].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_mb[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_a[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_b[0].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_mb[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_a[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_b[1].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_mb[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_a[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_b[1].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_mb[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_a[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_b[2].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_mb[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_a[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_b[2].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_mb[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_a[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_b[3].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_mb[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_a[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_b[3].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_mb[3].im;

    /* Gain: '<S54>/Gain1' incorporates:
     *  Sum: '<S54>/Add'
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

    /* Math: '<S53>/Math Function1' incorporates:
     *  Gain: '<S54>/Gain1'
     */
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[0].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_h[0].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[0].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_h[0].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[1].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_h[1].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[1].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_h[1].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[2].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_h[2].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[2].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_h[2].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[3].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_h[3].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[3].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_h[3].im;

    /* Gain: '<S55>/alpha' incorporates:
     *  RealImagToComplex: '<S61>/Real-Imag to Complex'
     */
    re_n = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im;
    im_n = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re;

    /* Gain: '<S55>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_f[0].re = re_n;
    basic_cosimulation_c_1_sm_master_B.alpha_f[0].im = im_n;

    /* Gain: '<S55>/alpha' incorporates:
     *  RealImagToComplex: '<S61>/Real-Imag to Complex'
     */
    re_q = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im;
    im_q = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re;

    /* Gain: '<S55>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_f[1].re = re_q;
    basic_cosimulation_c_1_sm_master_B.alpha_f[1].im = im_q;

    /* Gain: '<S55>/alpha' incorporates:
     *  RealImagToComplex: '<S61>/Real-Imag to Complex'
     */
    re_t = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im;
    im_t = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re;

    /* Gain: '<S55>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_f[2].re = re_t;
    basic_cosimulation_c_1_sm_master_B.alpha_f[2].im = im_t;

    /* Gain: '<S55>/alpha' incorporates:
     *  RealImagToComplex: '<S61>/Real-Imag to Complex'
     */
    re_w = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im;
    im_w = basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha_Gain_i.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re;

    /* Gain: '<S55>/alpha' */
    basic_cosimulation_c_1_sm_master_B.alpha_f[3].re = re_w;
    basic_cosimulation_c_1_sm_master_B.alpha_f[3].im = im_w;

    /* Gain: '<S55>/alpha2' incorporates:
     *  RealImagToComplex: '<S59>/Real-Imag to Complex'
     */
    re_y = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].im;
    im_y = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].re;

    /* Gain: '<S55>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[0].re = re_y;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[0].im = im_y;

    /* Gain: '<S55>/alpha2' incorporates:
     *  RealImagToComplex: '<S59>/Real-Imag to Complex'
     */
    re_10 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].im;
    im_10 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].re;

    /* Gain: '<S55>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[1].re = re_10;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[1].im = im_10;

    /* Gain: '<S55>/alpha2' incorporates:
     *  RealImagToComplex: '<S59>/Real-Imag to Complex'
     */
    re_12 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].im;
    im_12 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].re;

    /* Gain: '<S55>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[2].re = re_12;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[2].im = im_12;

    /* Gain: '<S55>/alpha2' incorporates:
     *  RealImagToComplex: '<S59>/Real-Imag to Complex'
     */
    re_14 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].re -
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].im;
    im_14 = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.re *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].im +
      basic_cosimulation_c_1_sm_master_P.alpha2_Gain_o.im *
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].re;

    /* Gain: '<S55>/alpha2' */
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[3].re = re_14;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[3].im = im_14;

    /* Sum: '<S55>/Add' incorporates:
     *  Gain: '<S55>/alpha'
     *  Gain: '<S55>/alpha2'
     *  RealImagToComplex: '<S57>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_go[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_pr[0].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_go[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_pr[0].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_go[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_pr[1].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_go[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_pr[1].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_go[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_pr[2].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_go[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_pr[2].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_go[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].re +
       basic_cosimulation_c_1_sm_master_B.alpha2_pr[3].re) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_go[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].im +
       basic_cosimulation_c_1_sm_master_B.alpha2_pr[3].im) +
      basic_cosimulation_c_1_sm_master_B.alpha_f[3].im;

    /* Gain: '<S55>/Gain1' incorporates:
     *  Sum: '<S55>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_go[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_go[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_go[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_go[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_go[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_go[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_go[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add_go[3].im;

    /* Product: '<S53>/Product1' incorporates:
     *  Gain: '<S55>/Gain1'
     *  Math: '<S53>/Math Function1'
     */
    re_16 = basic_cosimulation_c_1_sm_master_B.Gain1_mb[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[0].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_mb[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[0].im;
    im_16 = basic_cosimulation_c_1_sm_master_B.Gain1_mb[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[0].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_mb[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[0].re;

    /* Product: '<S53>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_c[0].re = re_16;
    basic_cosimulation_c_1_sm_master_B.Product1_c[0].im = im_16;

    /* Product: '<S53>/Product1' incorporates:
     *  Gain: '<S55>/Gain1'
     *  Math: '<S53>/Math Function1'
     */
    re_18 = basic_cosimulation_c_1_sm_master_B.Gain1_mb[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[1].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_mb[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[1].im;
    im_18 = basic_cosimulation_c_1_sm_master_B.Gain1_mb[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[1].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_mb[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[1].re;

    /* Product: '<S53>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_c[1].re = re_18;
    basic_cosimulation_c_1_sm_master_B.Product1_c[1].im = im_18;

    /* Product: '<S53>/Product1' incorporates:
     *  Gain: '<S55>/Gain1'
     *  Math: '<S53>/Math Function1'
     */
    re_1a = basic_cosimulation_c_1_sm_master_B.Gain1_mb[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[2].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_mb[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[2].im;
    im_1a = basic_cosimulation_c_1_sm_master_B.Gain1_mb[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[2].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_mb[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[2].re;

    /* Product: '<S53>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_c[2].re = re_1a;
    basic_cosimulation_c_1_sm_master_B.Product1_c[2].im = im_1a;

    /* Product: '<S53>/Product1' incorporates:
     *  Gain: '<S55>/Gain1'
     *  Math: '<S53>/Math Function1'
     */
    re_1c = basic_cosimulation_c_1_sm_master_B.Gain1_mb[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[3].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_mb[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[3].im;
    im_1c = basic_cosimulation_c_1_sm_master_B.Gain1_mb[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[3].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_mb[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction1_b[3].re;

    /* Product: '<S53>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_c[3].re = re_1c;
    basic_cosimulation_c_1_sm_master_B.Product1_c[3].im = im_1c;

    /* Gain: '<S53>/Gain1' incorporates:
     *  Product: '<S53>/Product1'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_md[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1_c[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1_c[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1_c[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1_c[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1_c[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1_c[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1_c[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_md[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_b *
      basic_cosimulation_c_1_sm_master_B.Product1_c[3].im;

    /* MultiPortSwitch: '<S53>/Multiport Switch' incorporates:
     *  Gain: '<S53>/Gain1'
     */
    memcpy(&basic_cosimulation_c_1_sm_master_B.MultiportSwitch[0],
           &basic_cosimulation_c_1_sm_master_B.Gain1_md[0], 4U * sizeof(creal_T));
    break;

   default:
    /* Sum: '<S65>/Add' incorporates:
     *  RealImagToComplex: '<S56>/Real-Imag to Complex'
     *  RealImagToComplex: '<S58>/Real-Imag to Complex'
     *  RealImagToComplex: '<S60>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_oq[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_oq[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_oq[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_oq[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_oq[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_oq[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_oq[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_oq[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].im;

    /* Gain: '<S65>/Gain1' incorporates:
     *  Sum: '<S65>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_a[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add_oq[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add_oq[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add_oq[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add_oq[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add_oq[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add_oq[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add_oq[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Add_oq[3].im;

    /* Math: '<S53>/Math Function2' incorporates:
     *  Gain: '<S65>/Gain1'
     */
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[0].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_a[0].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[0].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_a[0].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[1].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_a[1].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[1].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_a[1].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[2].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_a[2].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[2].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_a[2].im;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[3].re =
      basic_cosimulation_c_1_sm_master_B.Gain1_a[3].re;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[3].im =
      -basic_cosimulation_c_1_sm_master_B.Gain1_a[3].im;

    /* Sum: '<S66>/Add' incorporates:
     *  RealImagToComplex: '<S57>/Real-Imag to Complex'
     *  RealImagToComplex: '<S59>/Real-Imag to Complex'
     *  RealImagToComplex: '<S61>/Real-Imag to Complex'
     */
    basic_cosimulation_c_1_sm_master_B.Add_f[0].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re;
    basic_cosimulation_c_1_sm_master_B.Add_f[0].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im;
    basic_cosimulation_c_1_sm_master_B.Add_f[1].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re;
    basic_cosimulation_c_1_sm_master_B.Add_f[1].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im;
    basic_cosimulation_c_1_sm_master_B.Add_f[2].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re;
    basic_cosimulation_c_1_sm_master_B.Add_f[2].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im;
    basic_cosimulation_c_1_sm_master_B.Add_f[3].re =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].re +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].re) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re;
    basic_cosimulation_c_1_sm_master_B.Add_f[3].im =
      (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].im +
       basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].im) +
      basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im;

    /* Gain: '<S66>/Gain1' incorporates:
     *  Sum: '<S66>/Add'
     */
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_o *
      basic_cosimulation_c_1_sm_master_B.Add_f[3].im;

    /* Product: '<S53>/Product2' incorporates:
     *  Gain: '<S66>/Gain1'
     *  Math: '<S53>/Math Function2'
     */
    re_l = basic_cosimulation_c_1_sm_master_B.Gain1_bv[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[0].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_bv[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[0].im;
    im_l = basic_cosimulation_c_1_sm_master_B.Gain1_bv[0].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[0].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_bv[0].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[0].re;

    /* Product: '<S53>/Product2' */
    basic_cosimulation_c_1_sm_master_B.Product2_b[0].re = re_l;
    basic_cosimulation_c_1_sm_master_B.Product2_b[0].im = im_l;

    /* Product: '<S53>/Product2' incorporates:
     *  Gain: '<S66>/Gain1'
     *  Math: '<S53>/Math Function2'
     */
    re_o = basic_cosimulation_c_1_sm_master_B.Gain1_bv[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[1].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_bv[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[1].im;
    im_o = basic_cosimulation_c_1_sm_master_B.Gain1_bv[1].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[1].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_bv[1].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[1].re;

    /* Product: '<S53>/Product2' */
    basic_cosimulation_c_1_sm_master_B.Product2_b[1].re = re_o;
    basic_cosimulation_c_1_sm_master_B.Product2_b[1].im = im_o;

    /* Product: '<S53>/Product2' incorporates:
     *  Gain: '<S66>/Gain1'
     *  Math: '<S53>/Math Function2'
     */
    re_r = basic_cosimulation_c_1_sm_master_B.Gain1_bv[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[2].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_bv[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[2].im;
    im_r = basic_cosimulation_c_1_sm_master_B.Gain1_bv[2].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[2].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_bv[2].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[2].re;

    /* Product: '<S53>/Product2' */
    basic_cosimulation_c_1_sm_master_B.Product2_b[2].re = re_r;
    basic_cosimulation_c_1_sm_master_B.Product2_b[2].im = im_r;

    /* Product: '<S53>/Product2' incorporates:
     *  Gain: '<S66>/Gain1'
     *  Math: '<S53>/Math Function2'
     */
    re_u = basic_cosimulation_c_1_sm_master_B.Gain1_bv[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[3].re -
      basic_cosimulation_c_1_sm_master_B.Gain1_bv[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[3].im;
    im_u = basic_cosimulation_c_1_sm_master_B.Gain1_bv[3].re *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[3].im +
      basic_cosimulation_c_1_sm_master_B.Gain1_bv[3].im *
      basic_cosimulation_c_1_sm_master_B.MathFunction2_i[3].re;

    /* Product: '<S53>/Product2' */
    basic_cosimulation_c_1_sm_master_B.Product2_b[3].re = re_u;
    basic_cosimulation_c_1_sm_master_B.Product2_b[3].im = im_u;

    /* Gain: '<S53>/Gain3' incorporates:
     *  Product: '<S53>/Product2'
     */
    basic_cosimulation_c_1_sm_master_B.Gain3_c[0].re =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
      basic_cosimulation_c_1_sm_master_B.Product2_b[0].re;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[0].im =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
      basic_cosimulation_c_1_sm_master_B.Product2_b[0].im;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[1].re =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
      basic_cosimulation_c_1_sm_master_B.Product2_b[1].re;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[1].im =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
      basic_cosimulation_c_1_sm_master_B.Product2_b[1].im;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[2].re =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
      basic_cosimulation_c_1_sm_master_B.Product2_b[2].re;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[2].im =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
      basic_cosimulation_c_1_sm_master_B.Product2_b[2].im;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[3].re =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
      basic_cosimulation_c_1_sm_master_B.Product2_b[3].re;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[3].im =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain *
      basic_cosimulation_c_1_sm_master_B.Product2_b[3].im;

    /* MultiPortSwitch: '<S53>/Multiport Switch' incorporates:
     *  Gain: '<S53>/Gain3'
     */
    memcpy(&basic_cosimulation_c_1_sm_master_B.MultiportSwitch[0],
           &basic_cosimulation_c_1_sm_master_B.Gain3_c[0], 4U * sizeof(creal_T));
    break;
  }

  /* End of MultiPortSwitch: '<S53>/Multiport Switch' */

  /* ComplexToRealImag: '<S53>/Complex to Real-Imag' incorporates:
   *  MultiPortSwitch: '<S53>/Multiport Switch'
   */
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1[0] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[0].re;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1[1] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[1].re;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1[2] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[2].re;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1[3] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[3].re;

  /* ComplexToRealImag: '<S53>/Complex to Real-Imag' incorporates:
   *  MultiPortSwitch: '<S53>/Multiport Switch'
   */
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2[0] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[0].im;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2[1] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[1].im;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2[2] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[2].im;
  basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2[3] =
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[3].im;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* DiscreteIntegrator: '<S45>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE;

    /* DiscreteIntegrator: '<S45>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1 =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE;

    /* Gain: '<S49>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_lp =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_gy *
      basic_cosimulation_c_1_sm_master_B.SFunction_l[1];

    /* Switch: '<S49>/Switch' */
    if (basic_cosimulation_c_1_sm_master_B.SFunction_l[0] >
        basic_cosimulation_c_1_sm_master_P.Switch_Threshold_d) {
      /* Switch: '<S49>/Switch' */
      basic_cosimulation_c_1_sm_master_B.Switch_e =
        basic_cosimulation_c_1_sm_master_B.Delay_i[0];
    } else {
      /* Switch: '<S49>/Switch' incorporates:
       *  Constant: '<S49>/Constant'
       */
      basic_cosimulation_c_1_sm_master_B.Switch_e =
        basic_cosimulation_c_1_sm_master_P.Vnom_peak;
    }

    /* End of Switch: '<S49>/Switch' */

    /* Gain: '<S49>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_hn =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_o3 *
      basic_cosimulation_c_1_sm_master_B.Switch_e;

    /* Product: '<S49>/Divide' */
    basic_cosimulation_c_1_sm_master_B.Divide =
      basic_cosimulation_c_1_sm_master_B.Gain1_lp /
      basic_cosimulation_c_1_sm_master_B.Gain_hn;

    /* Saturate: '<S45>/Saturation' */
    u = -1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    Bias = 1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    if (basic_cosimulation_c_1_sm_master_B.Divide > Bias) {
      /* Saturate: '<S45>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_n = Bias;
    } else if (basic_cosimulation_c_1_sm_master_B.Divide < u) {
      /* Saturate: '<S45>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_n = u;
    } else {
      /* Saturate: '<S45>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_n =
        basic_cosimulation_c_1_sm_master_B.Divide;
    }

    /* End of Saturate: '<S45>/Saturation' */

    /* Sum: '<S45>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_c =
      basic_cosimulation_c_1_sm_master_B.Saturation_n -
      basic_cosimulation_c_1_sm_master_B.Delay_f[0];

    /* Product: '<S45>/Product' */
    basic_cosimulation_c_1_sm_master_B.Product_o =
      basic_cosimulation_c_1_sm_master_B.Subtract_c *
      basic_cosimulation_c_1_sm_master_B.SFunction_l[0];

    /* Gain: '<S45>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_c =
      basic_cosimulation_c_1_sm_master_P.kp_current *
      basic_cosimulation_c_1_sm_master_B.Product_o;

    /* Sum: '<S45>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_gp =
      basic_cosimulation_c_1_sm_master_B.Gain1_c +
      basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator;

    /* Gain: '<S45>/Gain5' */
    basic_cosimulation_c_1_sm_master_B.Gain5 =
      basic_cosimulation_c_1_sm_master_P.decouple *
      basic_cosimulation_c_1_sm_master_B.Delay_f[1];

    /* Sum: '<S45>/Sum3' */
    basic_cosimulation_c_1_sm_master_B.Sum3_l =
      (basic_cosimulation_c_1_sm_master_B.Delay_i[0] +
       basic_cosimulation_c_1_sm_master_B.Sum1_gp) -
      basic_cosimulation_c_1_sm_master_B.Gain5;

    /* Product: '<S45>/Divide' incorporates:
     *  Constant: '<S45>/Constant2'
     */
    u = basic_cosimulation_c_1_sm_master_P.Vdc / 2.0;

    /* Product: '<S45>/Divide' */
    basic_cosimulation_c_1_sm_master_B.Divide_c =
      basic_cosimulation_c_1_sm_master_B.Sum3_l / u;

    /* Gain: '<S45>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_h5 =
      basic_cosimulation_c_1_sm_master_P.decouple *
      basic_cosimulation_c_1_sm_master_B.Delay_f[0];

    /* Gain: '<S49>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_e =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_a *
      basic_cosimulation_c_1_sm_master_B.SFunction_l[2];

    /* Gain: '<S49>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_et =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_f *
      basic_cosimulation_c_1_sm_master_B.Switch_e;

    /* Product: '<S49>/Divide1' */
    basic_cosimulation_c_1_sm_master_B.Divide1 =
      basic_cosimulation_c_1_sm_master_B.Gain3_e /
      basic_cosimulation_c_1_sm_master_B.Gain2_et;

    /* Saturate: '<S45>/Saturation1' */
    u = -1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    Bias = 1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    if (basic_cosimulation_c_1_sm_master_B.Divide1 > Bias) {
      /* Saturate: '<S45>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1 = Bias;
    } else if (basic_cosimulation_c_1_sm_master_B.Divide1 < u) {
      /* Saturate: '<S45>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1 = u;
    } else {
      /* Saturate: '<S45>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1 =
        basic_cosimulation_c_1_sm_master_B.Divide1;
    }

    /* End of Saturate: '<S45>/Saturation1' */

    /* Sum: '<S45>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1 =
      basic_cosimulation_c_1_sm_master_B.Saturation1 -
      basic_cosimulation_c_1_sm_master_B.Delay_f[1];

    /* Product: '<S45>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_f =
      basic_cosimulation_c_1_sm_master_B.SFunction_l[0] *
      basic_cosimulation_c_1_sm_master_B.Subtract1;

    /* Gain: '<S45>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_k =
      basic_cosimulation_c_1_sm_master_P.kp_current *
      basic_cosimulation_c_1_sm_master_B.Product1_f;

    /* Sum: '<S45>/Sum2' */
    basic_cosimulation_c_1_sm_master_B.Sum2_p =
      basic_cosimulation_c_1_sm_master_B.Gain3_k +
      basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1;

    /* Sum: '<S45>/Sum4' */
    basic_cosimulation_c_1_sm_master_B.Sum4_m =
      (basic_cosimulation_c_1_sm_master_B.Gain6_h5 +
       basic_cosimulation_c_1_sm_master_B.Sum2_p) +
      basic_cosimulation_c_1_sm_master_B.Delay_i[1];

    /* Product: '<S45>/Divide1' incorporates:
     *  Constant: '<S45>/Constant2'
     */
    u = basic_cosimulation_c_1_sm_master_P.Vdc / 2.0;

    /* Product: '<S45>/Divide1' */
    basic_cosimulation_c_1_sm_master_B.Divide1_h =
      basic_cosimulation_c_1_sm_master_B.Sum4_m / u;

    /* Gain: '<S45>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_n =
      basic_cosimulation_c_1_sm_master_P.ki_current *
      basic_cosimulation_c_1_sm_master_B.Product_o;

    /* Gain: '<S45>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4_m =
      basic_cosimulation_c_1_sm_master_P.ki_current *
      basic_cosimulation_c_1_sm_master_B.Product1_f;

    /* SignalConversion generated from: '<S45>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[0] =
      basic_cosimulation_c_1_sm_master_B.Divide_c;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[1] =
      basic_cosimulation_c_1_sm_master_B.Divide1_h;
  }

  /* Trigonometry: '<S47>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_l = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Sum: '<S47>/Sum' incorporates:
   *  Constant: '<S47>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.Sum_n =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant_Value;

  /* Trigonometry: '<S47>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_m = cos
    (basic_cosimulation_c_1_sm_master_B.Sum_n);

  /* Sum: '<S47>/Sum2' incorporates:
   *  Constant: '<S47>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_c =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_i;

  /* Trigonometry: '<S47>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_k = cos
    (basic_cosimulation_c_1_sm_master_B.Sum2_c);

  /* Trigonometry: '<S47>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_i = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Sum: '<S47>/Sum3' incorporates:
   *  Constant: '<S47>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum3_mo =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_p;

  /* Trigonometry: '<S47>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_l = sin
    (basic_cosimulation_c_1_sm_master_B.Sum3_mo);

  /* Sum: '<S47>/Sum5' incorporates:
   *  Constant: '<S47>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum5_f =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_hr;

  /* Trigonometry: '<S47>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_g = sin
    (basic_cosimulation_c_1_sm_master_B.Sum5_f);

  /* Product: '<S47>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_n =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.Cos_l;

  /* Product: '<S47>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_j =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.Cos1_m;

  /* Product: '<S47>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_g1 =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.Cos2_k;

  /* Sum: '<S47>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_gm =
    (basic_cosimulation_c_1_sm_master_B.Product_n +
     basic_cosimulation_c_1_sm_master_B.Product1_j) +
    basic_cosimulation_c_1_sm_master_B.Product2_g1;

  /* Gain: '<S47>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_n =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_h *
    basic_cosimulation_c_1_sm_master_B.Sum1_gm;

  /* Product: '<S47>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_e =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.Cos3_i;

  /* Product: '<S47>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_b =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.Cos4_l;

  /* Product: '<S47>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_l =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.Cos5_g;

  /* Sum: '<S47>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_c =
    (basic_cosimulation_c_1_sm_master_B.Product3_e +
     basic_cosimulation_c_1_sm_master_B.Product4_b) +
    basic_cosimulation_c_1_sm_master_B.Product5_l;

  /* Gain: '<S47>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_ft =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_oj *
    basic_cosimulation_c_1_sm_master_B.Sum4_c;

  /* Trigonometry: '<S48>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_a = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Sum: '<S48>/Sum' incorporates:
   *  Constant: '<S48>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.Sum_f =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant_Value_d;

  /* Trigonometry: '<S48>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_c = cos
    (basic_cosimulation_c_1_sm_master_B.Sum_f);

  /* Sum: '<S48>/Sum2' incorporates:
   *  Constant: '<S48>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_j =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_gs;

  /* Trigonometry: '<S48>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_j = cos
    (basic_cosimulation_c_1_sm_master_B.Sum2_j);

  /* Trigonometry: '<S48>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_p = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Sum: '<S48>/Sum3' incorporates:
   *  Constant: '<S48>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum3_o =
    basic_cosimulation_c_1_sm_master_B.Integrator2 -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_d;

  /* Trigonometry: '<S48>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_e = sin
    (basic_cosimulation_c_1_sm_master_B.Sum3_o);

  /* Sum: '<S48>/Sum5' incorporates:
   *  Constant: '<S48>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum5_k =
    basic_cosimulation_c_1_sm_master_B.Integrator2 +
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_e;

  /* Trigonometry: '<S48>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_e = sin
    (basic_cosimulation_c_1_sm_master_B.Sum5_k);

  /* Product: '<S48>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_i =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.Cos_a;

  /* Product: '<S48>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_c2 =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.Cos1_c;

  /* Product: '<S48>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_j =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.Cos2_j;

  /* Sum: '<S48>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_j =
    (basic_cosimulation_c_1_sm_master_B.Product_i +
     basic_cosimulation_c_1_sm_master_B.Product1_c2) +
    basic_cosimulation_c_1_sm_master_B.Product2_j;

  /* Gain: '<S48>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_e =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_g *
    basic_cosimulation_c_1_sm_master_B.Sum1_j;

  /* Product: '<S48>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_g =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.Cos3_p;

  /* Product: '<S48>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_d =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.Cos4_e;

  /* Product: '<S48>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_c =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.Cos5_e;

  /* Sum: '<S48>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_k =
    (basic_cosimulation_c_1_sm_master_B.Product3_g +
     basic_cosimulation_c_1_sm_master_B.Product4_d) +
    basic_cosimulation_c_1_sm_master_B.Product5_c;

  /* Gain: '<S48>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_ka =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a0v *
    basic_cosimulation_c_1_sm_master_B.Sum4_k;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* SignalConversion generated from: '<S47>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[0] =
      basic_cosimulation_c_1_sm_master_B.Gain_n;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[1] =
      basic_cosimulation_c_1_sm_master_B.Gain1_ft;

    /* SignalConversion generated from: '<S48>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[0] =
      basic_cosimulation_c_1_sm_master_B.Gain_e;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[1] =
      basic_cosimulation_c_1_sm_master_B.Gain1_ka;
  }

  /* Integrator: '<S68>/Integrator' */
  basic_cosimulation_c_1_sm_master_B.Integrator_i[0] =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[0];
  basic_cosimulation_c_1_sm_master_B.Integrator_i[1] =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[1];
  basic_cosimulation_c_1_sm_master_B.Integrator_i[2] =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[2];
  basic_cosimulation_c_1_sm_master_B.Integrator_i[3] =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[3];

  /* Trigonometry: '<S64>/Trigonometric Function1' */
  for (i = 0; i < 4; i = i + 1) {
    u = basic_cosimulation_c_1_sm_master_B.Integrator_i[i];
    Bias = sin(u);
    u = cos(u);
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[i] = Bias;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[i] = u;
  }

  /* End of Trigonometry: '<S64>/Trigonometric Function1' */

  /* Product: '<S56>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_e[0] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[0];
  basic_cosimulation_c_1_sm_master_B.Product1_e[1] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[1];
  basic_cosimulation_c_1_sm_master_B.Product1_e[2] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[2];
  basic_cosimulation_c_1_sm_master_B.Product1_e[3] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[3];
  basic_cosimulation_c_1_sm_master_B.Product1_e[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[0];
  basic_cosimulation_c_1_sm_master_B.Product1_e[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[1];
  basic_cosimulation_c_1_sm_master_B.Product1_e[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[2];
  basic_cosimulation_c_1_sm_master_B.Product1_e[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[0] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[3];

  /* Product: '<S57>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_k[0] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[0];
  basic_cosimulation_c_1_sm_master_B.Product1_k[1] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[1];
  basic_cosimulation_c_1_sm_master_B.Product1_k[2] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[2];
  basic_cosimulation_c_1_sm_master_B.Product1_k[3] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[3];
  basic_cosimulation_c_1_sm_master_B.Product1_k[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[0];
  basic_cosimulation_c_1_sm_master_B.Product1_k[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[1];
  basic_cosimulation_c_1_sm_master_B.Product1_k[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[2];
  basic_cosimulation_c_1_sm_master_B.Product1_k[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain7_b *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[3];

  /* Product: '<S58>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_ew[0] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[0];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[1] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[1];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[2] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[2];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[3] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[3];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[0];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[1];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[2];
  basic_cosimulation_c_1_sm_master_B.Product1_ew[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[1] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[3];

  /* Product: '<S59>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_b[0] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[0];
  basic_cosimulation_c_1_sm_master_B.Product1_b[1] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[1];
  basic_cosimulation_c_1_sm_master_B.Product1_b[2] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[2];
  basic_cosimulation_c_1_sm_master_B.Product1_b[3] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[3];
  basic_cosimulation_c_1_sm_master_B.Product1_b[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[0];
  basic_cosimulation_c_1_sm_master_B.Product1_b[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[1];
  basic_cosimulation_c_1_sm_master_B.Product1_b[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[2];
  basic_cosimulation_c_1_sm_master_B.Product1_b[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain8_i *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[3];

  /* Product: '<S60>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_bw[0] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[0];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[1] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[1];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[2] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[2];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[3] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[3];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[0];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[1];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[2];
  basic_cosimulation_c_1_sm_master_B.Product1_bw[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Delay2[2] *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[3];

  /* Product: '<S61>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_l[0] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[0];
  basic_cosimulation_c_1_sm_master_B.Product1_l[1] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[1];
  basic_cosimulation_c_1_sm_master_B.Product1_l[2] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[2];
  basic_cosimulation_c_1_sm_master_B.Product1_l[3] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[3];
  basic_cosimulation_c_1_sm_master_B.Product1_l[0 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[0];
  basic_cosimulation_c_1_sm_master_B.Product1_l[1 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[1];
  basic_cosimulation_c_1_sm_master_B.Product1_l[2 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[2];
  basic_cosimulation_c_1_sm_master_B.Product1_l[3 + 4] =
    basic_cosimulation_c_1_sm_master_B.Gain9_n *
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[3];
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* DataTypeConversion: '<S68>/Data Type Conversion' incorporates:
     *  Constant: '<S64>/Radial frequencies'
     */
    Bias = 2.0 * 3.1415926535897931 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F;
    b_idx_0 = basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K[0];
    b_idx_1 = basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K[1];
    b_idx_2 = basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K[2];
    b_idx_3 = basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K[3];
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

    /* DataTypeConversion: '<S68>/Data Type Conversion' incorporates:
     *  Constant: '<S64>/Radial frequencies'
     */
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[0] = b_idx_0;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[1] = b_idx_1;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[2] = b_idx_2;
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[3] = b_idx_3;

    /* Gain: '<S51>/Gain' */
    u = 1.0 / basic_cosimulation_c_1_sm_master_P.Vnom_peak;

    /* Gain: '<S51>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_oh[0] = u *
      basic_cosimulation_c_1_sm_master_B.Gain7_b;
    basic_cosimulation_c_1_sm_master_B.Gain_oh[1] = u *
      basic_cosimulation_c_1_sm_master_B.Gain8_i;
    basic_cosimulation_c_1_sm_master_B.Gain_oh[2] = u *
      basic_cosimulation_c_1_sm_master_B.Gain9_n;

    /* Gain: '<S51>/Gain5' */
    basic_cosimulation_c_1_sm_master_B.Gain5_o =
      basic_cosimulation_c_1_sm_master_P.Gain5_Gain *
      basic_cosimulation_c_1_sm_master_B.Gain_oh[1];

    /* Gain: '<S51>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_k =
      basic_cosimulation_c_1_sm_master_P.Gain6_Gain_m *
      basic_cosimulation_c_1_sm_master_B.Gain_oh[2];

    /* Sum: '<S51>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_f =
      basic_cosimulation_c_1_sm_master_B.Gain5_o +
      basic_cosimulation_c_1_sm_master_B.Gain6_k;

    /* Gain: '<S51>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4_c =
      basic_cosimulation_c_1_sm_master_P.Gain4_Gain *
      basic_cosimulation_c_1_sm_master_B.Sum1_f;
  }

  /* Integrator: '<S51>/Integrator' */
  basic_cosimulation_c_1_sm_master_B.Integrator_g5 =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_hp;

  /* Trigonometry: '<S51>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_j = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Product: '<S51>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_c =
    basic_cosimulation_c_1_sm_master_B.Gain4_c *
    basic_cosimulation_c_1_sm_master_B.Cos_j;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Gain: '<S51>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_a =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_pj *
      basic_cosimulation_c_1_sm_master_B.Gain_oh[1];

    /* Gain: '<S51>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_m =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_k *
      basic_cosimulation_c_1_sm_master_B.Gain_oh[2];

    /* Sum: '<S51>/Sum' */
    basic_cosimulation_c_1_sm_master_B.Sum_m =
      (basic_cosimulation_c_1_sm_master_B.Gain_oh[0] +
       basic_cosimulation_c_1_sm_master_B.Gain2_a) +
      basic_cosimulation_c_1_sm_master_B.Gain3_m;

    /* Gain: '<S51>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_i =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_jm *
      basic_cosimulation_c_1_sm_master_B.Sum_m;
  }

  /* Trigonometry: '<S51>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_h = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Product: '<S51>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_g =
    basic_cosimulation_c_1_sm_master_B.Gain1_i *
    basic_cosimulation_c_1_sm_master_B.Cos1_h;

  /* Sum: '<S51>/Sum3' */
  basic_cosimulation_c_1_sm_master_B.Sum3_e =
    basic_cosimulation_c_1_sm_master_B.Product_c -
    basic_cosimulation_c_1_sm_master_B.Product1_g;

  /* Gain: '<S51>/Gain7' */
  basic_cosimulation_c_1_sm_master_B.Gain7_n =
    basic_cosimulation_c_1_sm_master_P.Gain7_Gain_dd *
    basic_cosimulation_c_1_sm_master_B.Sum3_e;

  /* Sum: '<S51>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_b =
    basic_cosimulation_c_1_sm_master_B.Integrator_g5 +
    basic_cosimulation_c_1_sm_master_B.Gain7_n;

  /* Sum: '<S51>/Sum5' incorporates:
   *  Constant: '<S51>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.W =
    basic_cosimulation_c_1_sm_master_B.Sum4_b +
    basic_cosimulation_c_1_sm_master_P.Constant_Value_p;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
  }

  /* Trigonometry: '<S51>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_l = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator2);

  /* Trigonometry: '<S51>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_n = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator2);
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
  }

  /* Product: '<S51>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_i =
    basic_cosimulation_c_1_sm_master_B.Gain4_c *
    basic_cosimulation_c_1_sm_master_B.Cos2_l;

  /* Product: '<S51>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_f =
    basic_cosimulation_c_1_sm_master_B.Gain1_i *
    basic_cosimulation_c_1_sm_master_B.Cos3_n;

  /* Sum: '<S51>/Sum2' */
  basic_cosimulation_c_1_sm_master_B.Sum2_g =
    basic_cosimulation_c_1_sm_master_B.Product2_i +
    basic_cosimulation_c_1_sm_master_B.Product3_f;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Gain: '<S23>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_ge =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_j *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[11];

    /* Gain: '<S24>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c5 =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_hj *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[12];

    /* Gain: '<S25>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_nq =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_l *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[13];

    /* SignalConversion generated from: '<S7>/Delay2' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[0] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_ge;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[1] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c5;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[2] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_nq;

    /* Gain: '<S31>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_he =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_je *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[6];

    /* Gain: '<S32>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_i =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_d *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[7];

    /* Gain: '<S26>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_l =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_n *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1_h[14];

    /* SignalConversion generated from: '<S3>/Delay4' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay4Inpo[0] =
      basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[0];
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay4Inpo[1] =
      basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[1];
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay4Inpo[2] =
      basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[2];
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay4Inpo[0 + 3] =
      basic_cosimulation_c_1_sm_master_B.RadDeg[0];
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay4Inpo[1 + 3] =
      basic_cosimulation_c_1_sm_master_B.RadDeg[1];
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay4Inpo[2 + 3] =
      basic_cosimulation_c_1_sm_master_B.RadDeg[2];

    /* SignalConversion generated from: '<S3>/Delay6' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[0] =
      basic_cosimulation_c_1_sm_master_B.Gain7;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[1] =
      basic_cosimulation_c_1_sm_master_B.Gain8;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[2] =
      basic_cosimulation_c_1_sm_master_B.Gain9;

    /* SignalConversion generated from: '<S3>/Delay7' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay7Inpo[0] =
      basic_cosimulation_c_1_sm_master_B.Delay2[0];
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay7Inpo[1] =
      basic_cosimulation_c_1_sm_master_B.Delay2[1];
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay7Inpo[2] =
      basic_cosimulation_c_1_sm_master_B.Delay2[2];
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay7Inpo[3] =
      basic_cosimulation_c_1_sm_master_B.Gain7_b;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay7Inpo[4] =
      basic_cosimulation_c_1_sm_master_B.Gain8_i;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay7Inpo[5] =
      basic_cosimulation_c_1_sm_master_B.Gain9_n;

    /* Gain: '<S95>/Gain3' incorporates:
     *  Constant: '<S95>/Frequency Offset'
     */
    basic_cosimulation_c_1_sm_master_B.Gain3_a =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_l *
      basic_cosimulation_c_1_sm_master_P.Time_Domain_To_Phasor_SFA_offse;
    for (i_8 = 0; i_8 < 6; i_8 = i_8 + 1) {
      /* Gain: '<S102>/1//Ron' */
      basic_cosimulation_c_1_sm_master_B.uRon_n[i_8] =
        basic_cosimulation_c_1_sm_master_P.uRon_Gain_j *
        basic_cosimulation_c_1_sm_master_B.StateSpace_o1[i_8];
    }

    /* Switch: '<S102>/Switch' */
    for (i = 0; i < 6; i = i + 1) {
      if (basic_cosimulation_c_1_sm_master_B.StateSpace_o2[i] >=
          basic_cosimulation_c_1_sm_master_P.Switch_Threshold_b) {
        /* Switch: '<S102>/Switch' */
        basic_cosimulation_c_1_sm_master_B.Switch_l[i] =
          basic_cosimulation_c_1_sm_master_B.uRon_n[i];
      } else {
        /* Switch: '<S102>/Switch' incorporates:
         *  Constant: '<S102>/0 4'
         */
        basic_cosimulation_c_1_sm_master_B.Switch_l[i] =
          basic_cosimulation_c_1_sm_master_P.u4_Value_i;
      }
    }

    /* End of Switch: '<S102>/Switch' */
    for (i_9 = 0; i_9 < 6; i_9 = i_9 + 1) {
      /* Saturate: '<S102>/Saturation' */
      u0_9 = basic_cosimulation_c_1_sm_master_B.Switch_l[i_9];
      if (u0_9 > basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_c) {
        y_6 = basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_c;
      } else if (u0_9 < basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_l)
      {
        y_6 = basic_cosimulation_c_1_sm_master_P.Saturation_LowerSat_l;
      } else {
        y_6 = u0_9;
      }

      /* Saturate: '<S102>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_b[i_9] = y_6;
    }

    /* Gain: '<S99>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_lv =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_o *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[8];

    /* Gain: '<S100>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_m =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_oq *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[9];

    /* Gain: '<S101>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_k =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_np *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[10];

    /* Delay: '<S9>/Delay2' */
    basic_cosimulation_c_1_sm_master_B.Delay2_f[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[0];
    basic_cosimulation_c_1_sm_master_B.Delay2_f[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1];
    basic_cosimulation_c_1_sm_master_B.Delay2_f[2] =
      basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[2];

    /* Delay: '<S112>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_b0[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c3[0];
    basic_cosimulation_c_1_sm_master_B.Delay_b0[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c3[1];

    /* DiscreteIntegrator: '<S110>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator_k =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_i;

    /* DiscreteIntegrator: '<S110>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1_k =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_l;

    /* Delay: '<S113>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_i4[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[0];
    basic_cosimulation_c_1_sm_master_B.Delay_i4[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[1];

    /* Gain: '<S114>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_ow =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_k *
      basic_cosimulation_c_1_sm_master_B.SFunction_l[1];

    /* Switch: '<S114>/Switch' */
    if (basic_cosimulation_c_1_sm_master_B.SFunction_l[0] >
        basic_cosimulation_c_1_sm_master_P.Switch_Threshold_l) {
      /* Switch: '<S114>/Switch' */
      basic_cosimulation_c_1_sm_master_B.Switch_o =
        basic_cosimulation_c_1_sm_master_B.Delay_i4[0];
    } else {
      /* Switch: '<S114>/Switch' incorporates:
       *  Constant: '<S114>/Constant'
       */
      basic_cosimulation_c_1_sm_master_B.Switch_o =
        basic_cosimulation_c_1_sm_master_P.Vnom_peak;
    }

    /* End of Switch: '<S114>/Switch' */

    /* Gain: '<S114>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_k =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Switch_o;

    /* Product: '<S114>/Divide' */
    basic_cosimulation_c_1_sm_master_B.Divide_h =
      basic_cosimulation_c_1_sm_master_B.Gain1_ow /
      basic_cosimulation_c_1_sm_master_B.Gain_k;

    /* Saturate: '<S110>/Saturation' */
    u = -1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    Bias = 1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    if (basic_cosimulation_c_1_sm_master_B.Divide_h > Bias) {
      /* Saturate: '<S110>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_a = Bias;
    } else if (basic_cosimulation_c_1_sm_master_B.Divide_h < u) {
      /* Saturate: '<S110>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_a = u;
    } else {
      /* Saturate: '<S110>/Saturation' */
      basic_cosimulation_c_1_sm_master_B.Saturation_a =
        basic_cosimulation_c_1_sm_master_B.Divide_h;
    }

    /* End of Saturate: '<S110>/Saturation' */

    /* Sum: '<S110>/Subtract' */
    basic_cosimulation_c_1_sm_master_B.Subtract_ce =
      basic_cosimulation_c_1_sm_master_B.Saturation_a -
      basic_cosimulation_c_1_sm_master_B.Delay_b0[0];

    /* Product: '<S110>/Product' */
    basic_cosimulation_c_1_sm_master_B.Product_l =
      basic_cosimulation_c_1_sm_master_B.Subtract_ce *
      basic_cosimulation_c_1_sm_master_B.SFunction_l[0];

    /* Gain: '<S110>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_b2 =
      basic_cosimulation_c_1_sm_master_P.kp_current *
      basic_cosimulation_c_1_sm_master_B.Product_l;

    /* Sum: '<S110>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_i =
      basic_cosimulation_c_1_sm_master_B.Gain1_b2 +
      basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator_k;

    /* Gain: '<S110>/Gain5' */
    basic_cosimulation_c_1_sm_master_B.Gain5_l =
      basic_cosimulation_c_1_sm_master_P.decouple *
      basic_cosimulation_c_1_sm_master_B.Delay_b0[1];

    /* Sum: '<S110>/Sum3' */
    basic_cosimulation_c_1_sm_master_B.Sum3_n =
      (basic_cosimulation_c_1_sm_master_B.Delay_i4[0] +
       basic_cosimulation_c_1_sm_master_B.Sum1_i) -
      basic_cosimulation_c_1_sm_master_B.Gain5_l;

    /* Product: '<S110>/Divide' incorporates:
     *  Constant: '<S110>/Constant2'
     */
    u = basic_cosimulation_c_1_sm_master_P.Vdc / 2.0;

    /* Product: '<S110>/Divide' */
    basic_cosimulation_c_1_sm_master_B.Divide_j =
      basic_cosimulation_c_1_sm_master_B.Sum3_n / u;

    /* Gain: '<S110>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_a =
      basic_cosimulation_c_1_sm_master_P.decouple *
      basic_cosimulation_c_1_sm_master_B.Delay_b0[0];

    /* Gain: '<S114>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_kn =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_au *
      basic_cosimulation_c_1_sm_master_B.SFunction_l[2];

    /* Gain: '<S114>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_b =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_n *
      basic_cosimulation_c_1_sm_master_B.Switch_o;

    /* Product: '<S114>/Divide1' */
    basic_cosimulation_c_1_sm_master_B.Divide1_a =
      basic_cosimulation_c_1_sm_master_B.Gain3_kn /
      basic_cosimulation_c_1_sm_master_B.Gain2_b;

    /* Saturate: '<S110>/Saturation1' */
    u = -1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    Bias = 1.2 * basic_cosimulation_c_1_sm_master_P.Inom_peak;
    if (basic_cosimulation_c_1_sm_master_B.Divide1_a > Bias) {
      /* Saturate: '<S110>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1_d = Bias;
    } else if (basic_cosimulation_c_1_sm_master_B.Divide1_a < u) {
      /* Saturate: '<S110>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1_d = u;
    } else {
      /* Saturate: '<S110>/Saturation1' */
      basic_cosimulation_c_1_sm_master_B.Saturation1_d =
        basic_cosimulation_c_1_sm_master_B.Divide1_a;
    }

    /* End of Saturate: '<S110>/Saturation1' */

    /* Sum: '<S110>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_p =
      basic_cosimulation_c_1_sm_master_B.Saturation1_d -
      basic_cosimulation_c_1_sm_master_B.Delay_b0[1];

    /* Product: '<S110>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_d =
      basic_cosimulation_c_1_sm_master_B.SFunction_l[0] *
      basic_cosimulation_c_1_sm_master_B.Subtract1_p;

    /* Gain: '<S110>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_et =
      basic_cosimulation_c_1_sm_master_P.kp_current *
      basic_cosimulation_c_1_sm_master_B.Product1_d;

    /* Sum: '<S110>/Sum2' */
    basic_cosimulation_c_1_sm_master_B.Sum2_b =
      basic_cosimulation_c_1_sm_master_B.Gain3_et +
      basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1_k;

    /* Sum: '<S110>/Sum4' */
    basic_cosimulation_c_1_sm_master_B.Sum4_e =
      (basic_cosimulation_c_1_sm_master_B.Gain6_a +
       basic_cosimulation_c_1_sm_master_B.Sum2_b) +
      basic_cosimulation_c_1_sm_master_B.Delay_i4[1];

    /* Product: '<S110>/Divide1' incorporates:
     *  Constant: '<S110>/Constant2'
     */
    u = basic_cosimulation_c_1_sm_master_P.Vdc / 2.0;

    /* Product: '<S110>/Divide1' */
    basic_cosimulation_c_1_sm_master_B.Divide1_j =
      basic_cosimulation_c_1_sm_master_B.Sum4_e / u;

    /* Gain: '<S110>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_k =
      basic_cosimulation_c_1_sm_master_P.ki_current *
      basic_cosimulation_c_1_sm_master_B.Product_l;

    /* Gain: '<S110>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4_d =
      basic_cosimulation_c_1_sm_master_P.ki_current *
      basic_cosimulation_c_1_sm_master_B.Product1_d;

    /* SignalConversion generated from: '<S110>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[0] =
      basic_cosimulation_c_1_sm_master_B.Divide_j;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[1] =
      basic_cosimulation_c_1_sm_master_B.Divide1_j;
  }

  /* Trigonometry: '<S112>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_g = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Sum: '<S112>/Sum' incorporates:
   *  Constant: '<S112>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.Sum_a =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant_Value_g;

  /* Trigonometry: '<S112>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_g = cos
    (basic_cosimulation_c_1_sm_master_B.Sum_a);

  /* Sum: '<S112>/Sum2' incorporates:
   *  Constant: '<S112>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_pk =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_f;

  /* Trigonometry: '<S112>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_d = cos
    (basic_cosimulation_c_1_sm_master_B.Sum2_pk);

  /* Trigonometry: '<S112>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_il = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Sum: '<S112>/Sum3' incorporates:
   *  Constant: '<S112>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum3_c =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_c;

  /* Trigonometry: '<S112>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_a = sin
    (basic_cosimulation_c_1_sm_master_B.Sum3_c);

  /* Sum: '<S112>/Sum5' incorporates:
   *  Constant: '<S112>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum5_d =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_k;

  /* Trigonometry: '<S112>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_j = sin
    (basic_cosimulation_c_1_sm_master_B.Sum5_d);

  /* Product: '<S112>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_oy =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[0] *
    basic_cosimulation_c_1_sm_master_B.Cos_g;

  /* Product: '<S112>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_h =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[1] *
    basic_cosimulation_c_1_sm_master_B.Cos1_g;

  /* Product: '<S112>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_jw =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[2] *
    basic_cosimulation_c_1_sm_master_B.Cos2_d;

  /* Sum: '<S112>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_d =
    (basic_cosimulation_c_1_sm_master_B.Product_oy +
     basic_cosimulation_c_1_sm_master_B.Product1_h) +
    basic_cosimulation_c_1_sm_master_B.Product2_jw;

  /* Gain: '<S112>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_g =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_e *
    basic_cosimulation_c_1_sm_master_B.Sum1_d;

  /* Product: '<S112>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_f2 =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[0] *
    basic_cosimulation_c_1_sm_master_B.Cos3_il;

  /* Product: '<S112>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_m =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[1] *
    basic_cosimulation_c_1_sm_master_B.Cos4_a;

  /* Product: '<S112>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_o =
    basic_cosimulation_c_1_sm_master_B.Delay2_f[2] *
    basic_cosimulation_c_1_sm_master_B.Cos5_j;

  /* Sum: '<S112>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_g =
    (basic_cosimulation_c_1_sm_master_B.Product3_f2 +
     basic_cosimulation_c_1_sm_master_B.Product4_m) +
    basic_cosimulation_c_1_sm_master_B.Product5_o;

  /* Gain: '<S112>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_n =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_ga *
    basic_cosimulation_c_1_sm_master_B.Sum4_g;

  /* Trigonometry: '<S113>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_m = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Sum: '<S113>/Sum' incorporates:
   *  Constant: '<S113>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.Sum_mi =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant_Value_k;

  /* Trigonometry: '<S113>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_e = cos
    (basic_cosimulation_c_1_sm_master_B.Sum_mi);

  /* Sum: '<S113>/Sum2' incorporates:
   *  Constant: '<S113>/Constant1'
   */
  basic_cosimulation_c_1_sm_master_B.Sum2_e =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant1_Value_l;

  /* Trigonometry: '<S113>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_lp = cos
    (basic_cosimulation_c_1_sm_master_B.Sum2_e);

  /* Trigonometry: '<S113>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_e = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Sum: '<S113>/Sum3' incorporates:
   *  Constant: '<S113>/Constant2'
   */
  basic_cosimulation_c_1_sm_master_B.Sum3_d =
    basic_cosimulation_c_1_sm_master_B.Integrator1 -
    basic_cosimulation_c_1_sm_master_P.Constant2_Value_n;

  /* Trigonometry: '<S113>/Cos4' */
  basic_cosimulation_c_1_sm_master_B.Cos4_o = sin
    (basic_cosimulation_c_1_sm_master_B.Sum3_d);

  /* Sum: '<S113>/Sum5' incorporates:
   *  Constant: '<S113>/Constant3'
   */
  basic_cosimulation_c_1_sm_master_B.Sum5_m =
    basic_cosimulation_c_1_sm_master_B.Integrator1 +
    basic_cosimulation_c_1_sm_master_P.Constant3_Value_a;

  /* Trigonometry: '<S113>/Cos5' */
  basic_cosimulation_c_1_sm_master_B.Cos5_a = sin
    (basic_cosimulation_c_1_sm_master_B.Sum5_m);

  /* Product: '<S113>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_p =
    basic_cosimulation_c_1_sm_master_B.Gain7 *
    basic_cosimulation_c_1_sm_master_B.Cos_m;

  /* Product: '<S113>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_m =
    basic_cosimulation_c_1_sm_master_B.Gain8 *
    basic_cosimulation_c_1_sm_master_B.Cos1_e;

  /* Product: '<S113>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_p =
    basic_cosimulation_c_1_sm_master_B.Gain9 *
    basic_cosimulation_c_1_sm_master_B.Cos2_lp;

  /* Sum: '<S113>/Sum1' */
  basic_cosimulation_c_1_sm_master_B.Sum1_h =
    (basic_cosimulation_c_1_sm_master_B.Product_p +
     basic_cosimulation_c_1_sm_master_B.Product1_m) +
    basic_cosimulation_c_1_sm_master_B.Product2_p;

  /* Gain: '<S113>/Gain' */
  basic_cosimulation_c_1_sm_master_B.Gain_n0 =
    basic_cosimulation_c_1_sm_master_P.Gain_Gain_m *
    basic_cosimulation_c_1_sm_master_B.Sum1_h;

  /* Product: '<S113>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_k =
    basic_cosimulation_c_1_sm_master_B.Gain7 *
    basic_cosimulation_c_1_sm_master_B.Cos3_e;

  /* Product: '<S113>/Product4' */
  basic_cosimulation_c_1_sm_master_B.Product4_e =
    basic_cosimulation_c_1_sm_master_B.Gain8 *
    basic_cosimulation_c_1_sm_master_B.Cos4_o;

  /* Product: '<S113>/Product5' */
  basic_cosimulation_c_1_sm_master_B.Product5_n =
    basic_cosimulation_c_1_sm_master_B.Gain9 *
    basic_cosimulation_c_1_sm_master_B.Cos5_a;

  /* Sum: '<S113>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_n =
    (basic_cosimulation_c_1_sm_master_B.Product3_k +
     basic_cosimulation_c_1_sm_master_B.Product4_e) +
    basic_cosimulation_c_1_sm_master_B.Product5_n;

  /* Gain: '<S113>/Gain1' */
  basic_cosimulation_c_1_sm_master_B.Gain1_b1 =
    basic_cosimulation_c_1_sm_master_P.Gain1_Gain_oe *
    basic_cosimulation_c_1_sm_master_B.Sum4_n;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* SignalConversion generated from: '<S112>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[0] =
      basic_cosimulation_c_1_sm_master_B.Gain_g;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[1] =
      basic_cosimulation_c_1_sm_master_B.Gain1_n;

    /* SignalConversion generated from: '<S113>/Delay' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[0] =
      basic_cosimulation_c_1_sm_master_B.Gain_n0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[1] =
      basic_cosimulation_c_1_sm_master_B.Gain1_b1;

    /* RelationalOperator: '<S141>/Compare' incorporates:
     *  Constant: '<S141>/Constant'
     */
    basic_cosimulation_c_1_sm_master_B.Compare_hk =
      (basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f <
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_n);

    /* RelationalOperator: '<S142>/Compare' incorporates:
     *  Constant: '<S142>/Constant'
     */
    basic_cosimulation_c_1_sm_master_B.Compare_c =
      (basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f >=
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_j);

    /* Logic: '<S140>/Logical Operator' */
    basic_cosimulation_c_1_sm_master_B.LogicalOperator_f =
      (basic_cosimulation_c_1_sm_master_B.Compare_hk ||
       basic_cosimulation_c_1_sm_master_B.Compare_c);

    /* Bias: '<S143>/Bias' */
    u = -basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_n;

    /* Bias: '<S143>/Bias' */
    basic_cosimulation_c_1_sm_master_B.Bias_g =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f + u;

    /* Gain: '<S143>/Gain' */
    u = 1.0 /
      (basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_j -
       basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_n);

    /* Gain: '<S143>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_el = u *
      basic_cosimulation_c_1_sm_master_B.Bias_g;

    /* Rounding: '<S143>/Rounding Function' */
    basic_cosimulation_c_1_sm_master_B.RoundingFunction_j = floor
      (basic_cosimulation_c_1_sm_master_B.Gain_el);

    /* Gain: '<S143>/Gain1' */
    u = basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_j -
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_n;

    /* Gain: '<S143>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_nl = u *
      basic_cosimulation_c_1_sm_master_B.RoundingFunction_j;

    /* Sum: '<S143>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_e =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f -
      basic_cosimulation_c_1_sm_master_B.Gain1_nl;

    /* InitialCondition: '<S140>/Initial' */
    if (basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime_ic) {
      basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime_ic = false;

      /* InitialCondition: '<S140>/Initial' */
      basic_cosimulation_c_1_sm_master_B.Initial_f =
        basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDi_al;
    } else {
      /* InitialCondition: '<S140>/Initial' */
      basic_cosimulation_c_1_sm_master_B.Initial_f =
        basic_cosimulation_c_1_sm_master_B.Sum1_e;
    }

    /* End of InitialCondition: '<S140>/Initial' */

    /* DiscreteIntegrator: '<S140>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_B.LogicalOperator_f) {
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f =
        basic_cosimulation_c_1_sm_master_B.Initial_f;
    }

    /* DiscreteIntegrator: '<S140>/Integrator' */
    basic_cosimulation_c_1_sm_master_B.Integrator_g3 =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f;

    /* Trigonometry: '<S129>/Trigonometric Function1' */
    u = basic_cosimulation_c_1_sm_master_B.Integrator_g3;
    Bias = sin(u);
    u = cos(u);

    /* Trigonometry: '<S129>/Trigonometric Function1' */
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_d = Bias;

    /* Trigonometry: '<S129>/Trigonometric Function1' */
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_e = u;

    /* Product: '<S122>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_hb[0] =
      basic_cosimulation_c_1_sm_master_B.Gain7 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_d;
    basic_cosimulation_c_1_sm_master_B.Product1_hb[1] =
      basic_cosimulation_c_1_sm_master_B.Gain7 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_e;

    /* DiscreteIntegrator: '<S122>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE != 0) {
      /* DiscreteIntegrator: '<S122>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[1];
    } else {
      /* DiscreteIntegrator: '<S122>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_l *
        basic_cosimulation_c_1_sm_master_B.Product1_hb[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_l *
        basic_cosimulation_c_1_sm_master_B.Product1_hb[1];
    }

    /* End of DiscreteIntegrator: '<S122>/Integrator' */

    /* UnitDelay: '<S133>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[1];

    /* Sum: '<S133>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_m[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_m[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1[1];

    /* Gain: '<S133>/delta' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S133>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_m[0];
    basic_cosimulation_c_1_sm_master_B.delta[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_m[1];

    /* Sum: '<S133>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1[0] =
      basic_cosimulation_c_1_sm_master_B.delta[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1[0];
    basic_cosimulation_c_1_sm_master_B.Add1[1] =
      basic_cosimulation_c_1_sm_master_B.delta[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1[1];

    /* UnitDelay: '<S133>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[1];

    /* Sum: '<S133>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3[0] =
      basic_cosimulation_c_1_sm_master_B.Add1[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3[1] =
      basic_cosimulation_c_1_sm_master_B.Add1[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[1];

    /* Delay: '<S133>/Delay' */
    tmp_4 = (int32_T)0U << 1;

    /* Delay: '<S133>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_d[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[tmp_4 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_d[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[tmp_4 + 1];

    /* Sum: '<S133>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_d[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_d[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[1];

    /* Gain: '<S133>/delta'' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S133>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_p[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2[0];
    basic_cosimulation_c_1_sm_master_B.delta_p[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2[1];

    /* Sum: '<S133>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2[0] =
      basic_cosimulation_c_1_sm_master_B.delta_p[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[0];
    basic_cosimulation_c_1_sm_master_B.Add2[1] =
      basic_cosimulation_c_1_sm_master_B.delta_p[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2[1];

    /* Sum: '<S133>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[0] -
      basic_cosimulation_c_1_sm_master_B.Add2[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[1] -
      basic_cosimulation_c_1_sm_master_B.Add2[1];

    /* Sum: '<S133>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4[0];
    basic_cosimulation_c_1_sm_master_B.Add3[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4[1];

    /* Gain: '<S133>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_ki[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_e0 *
      basic_cosimulation_c_1_sm_master_B.Add3[0];
    basic_cosimulation_c_1_sm_master_B.Gain_ki[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_e0 *
      basic_cosimulation_c_1_sm_master_B.Add3[1];

    /* Gain: '<S122>/Gain' */
    u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e;

    /* Gain: '<S122>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_hy[0] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_ki[0];
    basic_cosimulation_c_1_sm_master_B.Gain_hy[1] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_ki[1];

    /* RealImagToComplex: '<S122>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.re =
      basic_cosimulation_c_1_sm_master_B.Gain_hy[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.im =
      basic_cosimulation_c_1_sm_master_B.Gain_hy[1];

    /* Product: '<S124>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_ku[0] =
      basic_cosimulation_c_1_sm_master_B.Gain8 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_d;
    basic_cosimulation_c_1_sm_master_B.Product1_ku[1] =
      basic_cosimulation_c_1_sm_master_B.Gain8 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_e;

    /* DiscreteIntegrator: '<S124>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_p != 0) {
      /* DiscreteIntegrator: '<S124>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[1];
    } else {
      /* DiscreteIntegrator: '<S124>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_b *
        basic_cosimulation_c_1_sm_master_B.Product1_ku[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_b *
        basic_cosimulation_c_1_sm_master_B.Product1_ku[1];
    }

    /* End of DiscreteIntegrator: '<S124>/Integrator' */

    /* UnitDelay: '<S135>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[1];

    /* Sum: '<S135>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_pp[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_pp[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[1];

    /* Gain: '<S135>/delta' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S135>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_f[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_pp[0];
    basic_cosimulation_c_1_sm_master_B.delta_f[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_pp[1];

    /* Sum: '<S135>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_l[0] =
      basic_cosimulation_c_1_sm_master_B.delta_f[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[0];
    basic_cosimulation_c_1_sm_master_B.Add1_l[1] =
      basic_cosimulation_c_1_sm_master_B.delta_f[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_g[1];

    /* UnitDelay: '<S135>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[1];

    /* Sum: '<S135>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_h[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_l[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_h[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_l[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[1];

    /* Delay: '<S135>/Delay' */
    tmp_5 = (int32_T)0U << 1;

    /* Delay: '<S135>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_lg[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[tmp_5 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_lg[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[tmp_5 + 1];

    /* Sum: '<S135>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_n[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_lg[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_n[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_lg[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[1];

    /* Gain: '<S135>/delta'' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S135>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_c[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_n[0];
    basic_cosimulation_c_1_sm_master_B.delta_c[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_n[1];

    /* Sum: '<S135>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_a[0] =
      basic_cosimulation_c_1_sm_master_B.delta_c[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[0];
    basic_cosimulation_c_1_sm_master_B.Add2_a[1] =
      basic_cosimulation_c_1_sm_master_B.delta_c[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_c[1];

    /* Sum: '<S135>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_n[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_a[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_n[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_a[1];

    /* Sum: '<S135>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_h[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_h[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_n[0];
    basic_cosimulation_c_1_sm_master_B.Add3_h[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_h[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_n[1];

    /* Gain: '<S135>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_ab[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add3_h[0];
    basic_cosimulation_c_1_sm_master_B.Gain_ab[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_j *
      basic_cosimulation_c_1_sm_master_B.Add3_h[1];

    /* Gain: '<S124>/Gain' */
    u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e;

    /* Gain: '<S124>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_l[0] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_ab[0];
    basic_cosimulation_c_1_sm_master_B.Gain_l[1] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_ab[1];

    /* RealImagToComplex: '<S124>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.re =
      basic_cosimulation_c_1_sm_master_B.Gain_l[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.im =
      basic_cosimulation_c_1_sm_master_B.Gain_l[1];

    /* Product: '<S126>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_ew1[0] =
      basic_cosimulation_c_1_sm_master_B.Gain9 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_d;
    basic_cosimulation_c_1_sm_master_B.Product1_ew1[1] =
      basic_cosimulation_c_1_sm_master_B.Gain9 *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_e;

    /* DiscreteIntegrator: '<S126>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_i != 0) {
      /* DiscreteIntegrator: '<S126>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[1];
    } else {
      /* DiscreteIntegrator: '<S126>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_d *
        basic_cosimulation_c_1_sm_master_B.Product1_ew1[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_d *
        basic_cosimulation_c_1_sm_master_B.Product1_ew1[1];
    }

    /* End of DiscreteIntegrator: '<S126>/Integrator' */

    /* UnitDelay: '<S137>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[1];

    /* Sum: '<S137>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_h[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_h[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[1];

    /* Gain: '<S137>/delta' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S137>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_cv[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_h[0];
    basic_cosimulation_c_1_sm_master_B.delta_cv[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_h[1];

    /* Sum: '<S137>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_e[0] =
      basic_cosimulation_c_1_sm_master_B.delta_cv[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[0];
    basic_cosimulation_c_1_sm_master_B.Add1_e[1] =
      basic_cosimulation_c_1_sm_master_B.delta_cv[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_gn[1];

    /* UnitDelay: '<S137>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[1];

    /* Sum: '<S137>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_d[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_e[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_d[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_e[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[1];

    /* Delay: '<S137>/Delay' */
    tmp_6 = (int32_T)0U << 1;

    /* Delay: '<S137>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_k[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[tmp_6 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_k[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[tmp_6 + 1];

    /* Sum: '<S137>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_g[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_k[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_g[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_k[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[1];

    /* Gain: '<S137>/delta'' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S137>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_e[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_g[0];
    basic_cosimulation_c_1_sm_master_B.delta_e[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_g[1];

    /* Sum: '<S137>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_h[0] =
      basic_cosimulation_c_1_sm_master_B.delta_e[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[0];
    basic_cosimulation_c_1_sm_master_B.Add2_h[1] =
      basic_cosimulation_c_1_sm_master_B.delta_e[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_h[1];

    /* Sum: '<S137>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_m[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_h[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_m[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_h[1];

    /* Sum: '<S137>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_m[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_d[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_m[0];
    basic_cosimulation_c_1_sm_master_B.Add3_m[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_d[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_m[1];

    /* Gain: '<S137>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_m[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_cw *
      basic_cosimulation_c_1_sm_master_B.Add3_m[0];
    basic_cosimulation_c_1_sm_master_B.Gain_m[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_cw *
      basic_cosimulation_c_1_sm_master_B.Add3_m[1];

    /* Gain: '<S126>/Gain' */
    u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e;

    /* Gain: '<S126>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_nw[0] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_m[0];
    basic_cosimulation_c_1_sm_master_B.Gain_nw[1] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_m[1];

    /* RealImagToComplex: '<S126>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re =
      basic_cosimulation_c_1_sm_master_B.Gain_nw[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im =
      basic_cosimulation_c_1_sm_master_B.Gain_nw[1];

    /* Product: '<S121>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_e1[0] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[0] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_d;
    basic_cosimulation_c_1_sm_master_B.Product1_e1[1] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[0] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_e;

    /* DiscreteIntegrator: '<S121>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_h != 0) {
      /* DiscreteIntegrator: '<S121>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[1];
    } else {
      /* DiscreteIntegrator: '<S121>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_c *
        basic_cosimulation_c_1_sm_master_B.Product1_e1[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_c *
        basic_cosimulation_c_1_sm_master_B.Product1_e1[1];
    }

    /* End of DiscreteIntegrator: '<S121>/Integrator' */

    /* UnitDelay: '<S132>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[1];

    /* Sum: '<S132>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_a[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_a[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[1];

    /* Gain: '<S132>/delta' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S132>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_c1[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_a[0];
    basic_cosimulation_c_1_sm_master_B.delta_c1[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_a[1];

    /* Sum: '<S132>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_k[0] =
      basic_cosimulation_c_1_sm_master_B.delta_c1[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[0];
    basic_cosimulation_c_1_sm_master_B.Add1_k[1] =
      basic_cosimulation_c_1_sm_master_B.delta_c1[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_p[1];

    /* UnitDelay: '<S132>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[1];

    /* Sum: '<S132>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_a[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_k[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_a[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_k[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[1];

    /* Delay: '<S132>/Delay' */
    tmp_7 = (int32_T)0U << 1;

    /* Delay: '<S132>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_in[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[tmp_7 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_in[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[tmp_7 + 1];

    /* Sum: '<S132>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_f[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_in[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_f[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_in[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[1];

    /* Gain: '<S132>/delta'' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S132>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_j[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_f[0];
    basic_cosimulation_c_1_sm_master_B.delta_j[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_f[1];

    /* Sum: '<S132>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_i[0] =
      basic_cosimulation_c_1_sm_master_B.delta_j[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[0];
    basic_cosimulation_c_1_sm_master_B.Add2_i[1] =
      basic_cosimulation_c_1_sm_master_B.delta_j[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_a[1];

    /* Sum: '<S132>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_h[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_i[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_h[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_i[1];

    /* Sum: '<S132>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_g[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_a[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_h[0];
    basic_cosimulation_c_1_sm_master_B.Add3_g[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_a[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_h[1];

    /* Gain: '<S132>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_db[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_a *
      basic_cosimulation_c_1_sm_master_B.Add3_g[0];
    basic_cosimulation_c_1_sm_master_B.Gain_db[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_a *
      basic_cosimulation_c_1_sm_master_B.Add3_g[1];

    /* Gain: '<S121>/Gain' */
    u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e;

    /* Gain: '<S121>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_c[0] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_db[0];
    basic_cosimulation_c_1_sm_master_B.Gain_c[1] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_db[1];

    /* RealImagToComplex: '<S121>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re =
      basic_cosimulation_c_1_sm_master_B.Gain_c[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im =
      basic_cosimulation_c_1_sm_master_B.Gain_c[1];

    /* Product: '<S123>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_lz[0] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[1] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_d;
    basic_cosimulation_c_1_sm_master_B.Product1_lz[1] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[1] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_e;

    /* DiscreteIntegrator: '<S123>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_iy != 0) {
      /* DiscreteIntegrator: '<S123>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[1];
    } else {
      /* DiscreteIntegrator: '<S123>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_h *
        basic_cosimulation_c_1_sm_master_B.Product1_lz[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_h *
        basic_cosimulation_c_1_sm_master_B.Product1_lz[1];
    }

    /* End of DiscreteIntegrator: '<S123>/Integrator' */

    /* UnitDelay: '<S134>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[1];

    /* Sum: '<S134>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_n[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_n[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[1];

    /* Gain: '<S134>/delta' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S134>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_n[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_n[0];
    basic_cosimulation_c_1_sm_master_B.delta_n[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_n[1];

    /* Sum: '<S134>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_ej[0] =
      basic_cosimulation_c_1_sm_master_B.delta_n[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[0];
    basic_cosimulation_c_1_sm_master_B.Add1_ej[1] =
      basic_cosimulation_c_1_sm_master_B.delta_n[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_k[1];

    /* UnitDelay: '<S134>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[1];

    /* Sum: '<S134>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_aq[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_ej[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_aq[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_ej[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[1];

    /* Delay: '<S134>/Delay' */
    tmp_8 = (int32_T)0U << 1;

    /* Delay: '<S134>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_j[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[tmp_8 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_j[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[tmp_8 + 1];

    /* Sum: '<S134>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_m[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_j[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_m[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_j[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[1];

    /* Gain: '<S134>/delta'' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S134>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_h[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_m[0];
    basic_cosimulation_c_1_sm_master_B.delta_h[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_m[1];

    /* Sum: '<S134>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_l[0] =
      basic_cosimulation_c_1_sm_master_B.delta_h[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[0];
    basic_cosimulation_c_1_sm_master_B.Add2_l[1] =
      basic_cosimulation_c_1_sm_master_B.delta_h[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_j[1];

    /* Sum: '<S134>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_c[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_l[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_c[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_l[1];

    /* Sum: '<S134>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_c[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_aq[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_c[0];
    basic_cosimulation_c_1_sm_master_B.Add3_c[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_aq[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_c[1];

    /* Gain: '<S134>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_kl[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_jx *
      basic_cosimulation_c_1_sm_master_B.Add3_c[0];
    basic_cosimulation_c_1_sm_master_B.Gain_kl[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_jx *
      basic_cosimulation_c_1_sm_master_B.Add3_c[1];

    /* Gain: '<S123>/Gain' */
    u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e;

    /* Gain: '<S123>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_oy[0] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_kl[0];
    basic_cosimulation_c_1_sm_master_B.Gain_oy[1] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_kl[1];

    /* RealImagToComplex: '<S123>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.re =
      basic_cosimulation_c_1_sm_master_B.Gain_oy[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.im =
      basic_cosimulation_c_1_sm_master_B.Gain_oy[1];

    /* Product: '<S125>/Product1' */
    basic_cosimulation_c_1_sm_master_B.Product1_k4[0] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[2] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_d;
    basic_cosimulation_c_1_sm_master_B.Product1_k4[1] =
      basic_cosimulation_c_1_sm_master_B.Delay2_f[2] *
      basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_e;

    /* DiscreteIntegrator: '<S125>/Integrator' */
    if (basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_a != 0) {
      /* DiscreteIntegrator: '<S125>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[1];
    } else {
      /* DiscreteIntegrator: '<S125>/Integrator' */
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[0] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_i *
        basic_cosimulation_c_1_sm_master_B.Product1_k4[0];
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] =
        basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[1] +
        basic_cosimulation_c_1_sm_master_P.Integrator_gainval_i *
        basic_cosimulation_c_1_sm_master_B.Product1_k4[1];
    }

    /* End of DiscreteIntegrator: '<S125>/Integrator' */

    /* UnitDelay: '<S136>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[1];

    /* Sum: '<S136>/Subtract1' */
    basic_cosimulation_c_1_sm_master_B.Subtract1_pk[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[0];
    basic_cosimulation_c_1_sm_master_B.Subtract1_pk[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[1];

    /* Gain: '<S136>/delta' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = 1.0 / basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts - (Bias - 1.0);

    /* Gain: '<S136>/delta' */
    basic_cosimulation_c_1_sm_master_B.delta_a[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_pk[0];
    basic_cosimulation_c_1_sm_master_B.delta_a[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract1_pk[1];

    /* Sum: '<S136>/Add1' */
    basic_cosimulation_c_1_sm_master_B.Add1_ea[0] =
      basic_cosimulation_c_1_sm_master_B.delta_a[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[0];
    basic_cosimulation_c_1_sm_master_B.Add1_ea[1] =
      basic_cosimulation_c_1_sm_master_B.delta_a[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay1_ka[1];

    /* UnitDelay: '<S136>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[0] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[0];
    basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[1] =
      basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[1];

    /* Sum: '<S136>/Subtract3' */
    basic_cosimulation_c_1_sm_master_B.Subtract3_hy[0] =
      basic_cosimulation_c_1_sm_master_B.Add1_ea[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[0];
    basic_cosimulation_c_1_sm_master_B.Subtract3_hy[1] =
      basic_cosimulation_c_1_sm_master_B.Add1_ea[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[1];

    /* Delay: '<S136>/Delay' */
    tmp_9 = (int32_T)0U << 1;

    /* Delay: '<S136>/Delay' */
    basic_cosimulation_c_1_sm_master_B.Delay_jw[0] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[tmp_9 + 0];
    basic_cosimulation_c_1_sm_master_B.Delay_jw[1] =
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[tmp_9 + 1];

    /* Sum: '<S136>/Subtract2' */
    basic_cosimulation_c_1_sm_master_B.Subtract2_mn[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_jw[0] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[0];
    basic_cosimulation_c_1_sm_master_B.Subtract2_mn[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_jw[1] -
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[1];

    /* Gain: '<S136>/delta'' */
    Bias = 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;
    Bias = ceil(Bias);
    u = Bias - 1.0 /
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e /
      basic_cosimulation_c_1_sm_master_P.Ts;

    /* Gain: '<S136>/delta'' */
    basic_cosimulation_c_1_sm_master_B.delta_fs[0] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_mn[0];
    basic_cosimulation_c_1_sm_master_B.delta_fs[1] = u *
      basic_cosimulation_c_1_sm_master_B.Subtract2_mn[1];

    /* Sum: '<S136>/Add2' */
    basic_cosimulation_c_1_sm_master_B.Add2_a5[0] =
      basic_cosimulation_c_1_sm_master_B.delta_fs[0] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[0];
    basic_cosimulation_c_1_sm_master_B.Add2_a5[1] =
      basic_cosimulation_c_1_sm_master_B.delta_fs[1] +
      basic_cosimulation_c_1_sm_master_B.UnitDelay2_b[1];

    /* Sum: '<S136>/Subtract4' */
    basic_cosimulation_c_1_sm_master_B.Subtract4_me[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] -
      basic_cosimulation_c_1_sm_master_B.Add2_a5[0];
    basic_cosimulation_c_1_sm_master_B.Subtract4_me[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] -
      basic_cosimulation_c_1_sm_master_B.Add2_a5[1];

    /* Sum: '<S136>/Add3' */
    basic_cosimulation_c_1_sm_master_B.Add3_n[0] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_hy[0] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_me[0];
    basic_cosimulation_c_1_sm_master_B.Add3_n[1] =
      basic_cosimulation_c_1_sm_master_B.Subtract3_hy[1] +
      basic_cosimulation_c_1_sm_master_B.Subtract4_me[1];

    /* Gain: '<S136>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_k2[0] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_n *
      basic_cosimulation_c_1_sm_master_B.Add3_n[0];
    basic_cosimulation_c_1_sm_master_B.Gain_k2[1] =
      basic_cosimulation_c_1_sm_master_P.Gain_Gain_n *
      basic_cosimulation_c_1_sm_master_B.Add3_n[1];

    /* Gain: '<S125>/Gain' */
    u = 2.0 * basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e;

    /* Gain: '<S125>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_cs[0] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_k2[0];
    basic_cosimulation_c_1_sm_master_B.Gain_cs[1] = u *
      basic_cosimulation_c_1_sm_master_B.Gain_k2[1];

    /* RealImagToComplex: '<S125>/Real-Imag to Complex' */
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.re =
      basic_cosimulation_c_1_sm_master_B.Gain_cs[0];
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.im =
      basic_cosimulation_c_1_sm_master_B.Gain_cs[1];

    /* MultiPortSwitch: '<S118>/Multiport Switch' incorporates:
     *  Constant: '<S118>/Sequence'
     */
    switch ((int32_T)basic_cosimulation_c_1_sm_master_P.Sequence_Value_h) {
     case 1:
      /* Gain: '<S127>/alpha2' incorporates:
       *  RealImagToComplex: '<S125>/Real-Imag to Complex'
       */
      re_1d = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_j.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.re -
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_j.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.im;
      im_1d = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_j.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.im +
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_j.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.re;

      /* Gain: '<S127>/alpha2' */
      basic_cosimulation_c_1_sm_master_B.alpha2_p.re = re_1d;
      basic_cosimulation_c_1_sm_master_B.alpha2_p.im = im_1d;

      /* Gain: '<S127>/alpha' incorporates:
       *  RealImagToComplex: '<S123>/Real-Imag to Complex'
       */
      re_1f = basic_cosimulation_c_1_sm_master_P.alpha_Gain_c.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.re -
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_c.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.im;
      im_1f = basic_cosimulation_c_1_sm_master_P.alpha_Gain_c.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.im +
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_c.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.re;

      /* Gain: '<S127>/alpha' */
      basic_cosimulation_c_1_sm_master_B.alpha_g.re = re_1f;
      basic_cosimulation_c_1_sm_master_B.alpha_g.im = im_1f;

      /* Sum: '<S127>/Add' incorporates:
       *  Gain: '<S127>/alpha'
       *  Gain: '<S127>/alpha2'
       *  RealImagToComplex: '<S121>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_g.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re +
         basic_cosimulation_c_1_sm_master_B.alpha_g.re) +
        basic_cosimulation_c_1_sm_master_B.alpha2_p.re;
      basic_cosimulation_c_1_sm_master_B.Add_g.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im +
         basic_cosimulation_c_1_sm_master_B.alpha_g.im) +
        basic_cosimulation_c_1_sm_master_B.alpha2_p.im;

      /* Gain: '<S127>/Gain1' incorporates:
       *  Sum: '<S127>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_m.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_g *
        basic_cosimulation_c_1_sm_master_B.Add_g.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_m.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_g *
        basic_cosimulation_c_1_sm_master_B.Add_g.im;

      /* Math: '<S118>/Math Function' incorporates:
       *  Gain: '<S127>/Gain1'
       */
      basic_cosimulation_c_1_sm_master_B.MathFunction.re =
        basic_cosimulation_c_1_sm_master_B.Gain1_m.re;
      basic_cosimulation_c_1_sm_master_B.MathFunction.im =
        -basic_cosimulation_c_1_sm_master_B.Gain1_m.im;

      /* Gain: '<S128>/alpha2' incorporates:
       *  RealImagToComplex: '<S126>/Real-Imag to Complex'
       */
      re_1i = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_p.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re -
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_p.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im;
      im_1i = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_p.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im +
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_p.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re;

      /* Gain: '<S128>/alpha2' */
      basic_cosimulation_c_1_sm_master_B.alpha2_c.re = re_1i;
      basic_cosimulation_c_1_sm_master_B.alpha2_c.im = im_1i;

      /* Gain: '<S128>/alpha' incorporates:
       *  RealImagToComplex: '<S124>/Real-Imag to Complex'
       */
      re_1k = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ib.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.re -
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_ib.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.im;
      im_1k = basic_cosimulation_c_1_sm_master_P.alpha_Gain_ib.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.im +
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_ib.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.re;

      /* Gain: '<S128>/alpha' */
      basic_cosimulation_c_1_sm_master_B.alpha_m.re = re_1k;
      basic_cosimulation_c_1_sm_master_B.alpha_m.im = im_1k;

      /* Sum: '<S128>/Add' incorporates:
       *  Gain: '<S128>/alpha'
       *  Gain: '<S128>/alpha2'
       *  RealImagToComplex: '<S122>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_l.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.re +
         basic_cosimulation_c_1_sm_master_B.alpha_m.re) +
        basic_cosimulation_c_1_sm_master_B.alpha2_c.re;
      basic_cosimulation_c_1_sm_master_B.Add_l.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.im +
         basic_cosimulation_c_1_sm_master_B.alpha_m.im) +
        basic_cosimulation_c_1_sm_master_B.alpha2_c.im;

      /* Gain: '<S128>/Gain1' incorporates:
       *  Sum: '<S128>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_b0.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_p *
        basic_cosimulation_c_1_sm_master_B.Add_l.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_b0.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_p *
        basic_cosimulation_c_1_sm_master_B.Add_l.im;

      /* Product: '<S118>/Product3' incorporates:
       *  Gain: '<S128>/Gain1'
       *  Math: '<S118>/Math Function'
       */
      re_1m = basic_cosimulation_c_1_sm_master_B.Gain1_b0.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction.re -
        basic_cosimulation_c_1_sm_master_B.Gain1_b0.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction.im;
      im_1m = basic_cosimulation_c_1_sm_master_B.Gain1_b0.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction.im +
        basic_cosimulation_c_1_sm_master_B.Gain1_b0.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction.re;

      /* Product: '<S118>/Product3' */
      basic_cosimulation_c_1_sm_master_B.Product3.re = re_1m;
      basic_cosimulation_c_1_sm_master_B.Product3.im = im_1m;

      /* Gain: '<S118>/Gain2' incorporates:
       *  Product: '<S118>/Product3'
       */
      basic_cosimulation_c_1_sm_master_B.Gain2.re =
        basic_cosimulation_c_1_sm_master_P.Gain2_Gain_d *
        basic_cosimulation_c_1_sm_master_B.Product3.re;
      basic_cosimulation_c_1_sm_master_B.Gain2.im =
        basic_cosimulation_c_1_sm_master_P.Gain2_Gain_d *
        basic_cosimulation_c_1_sm_master_B.Product3.im;

      /* MultiPortSwitch: '<S118>/Multiport Switch' incorporates:
       *  Gain: '<S118>/Gain2'
       */
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.re =
        basic_cosimulation_c_1_sm_master_B.Gain2.re;
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.im =
        basic_cosimulation_c_1_sm_master_B.Gain2.im;
      break;

     case 2:
      /* Gain: '<S119>/alpha' incorporates:
       *  RealImagToComplex: '<S125>/Real-Imag to Complex'
       */
      re_1e = basic_cosimulation_c_1_sm_master_P.alpha_Gain_o.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.re -
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_o.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.im;
      im_1e = basic_cosimulation_c_1_sm_master_P.alpha_Gain_o.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.im +
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_o.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.re;

      /* Gain: '<S119>/alpha' */
      basic_cosimulation_c_1_sm_master_B.alpha.re = re_1e;
      basic_cosimulation_c_1_sm_master_B.alpha.im = im_1e;

      /* Gain: '<S119>/alpha2' incorporates:
       *  RealImagToComplex: '<S123>/Real-Imag to Complex'
       */
      re_1g = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_f.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.re -
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_f.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.im;
      im_1g = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_f.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.im +
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_f.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.re;

      /* Gain: '<S119>/alpha2' */
      basic_cosimulation_c_1_sm_master_B.alpha2.re = re_1g;
      basic_cosimulation_c_1_sm_master_B.alpha2.im = im_1g;

      /* Sum: '<S119>/Add' incorporates:
       *  Gain: '<S119>/alpha'
       *  Gain: '<S119>/alpha2'
       *  RealImagToComplex: '<S121>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_m.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re +
         basic_cosimulation_c_1_sm_master_B.alpha2.re) +
        basic_cosimulation_c_1_sm_master_B.alpha.re;
      basic_cosimulation_c_1_sm_master_B.Add_m.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im +
         basic_cosimulation_c_1_sm_master_B.alpha2.im) +
        basic_cosimulation_c_1_sm_master_B.alpha.im;

      /* Gain: '<S119>/Gain1' incorporates:
       *  Sum: '<S119>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_f.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a *
        basic_cosimulation_c_1_sm_master_B.Add_m.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_f.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a *
        basic_cosimulation_c_1_sm_master_B.Add_m.im;

      /* Math: '<S118>/Math Function1' incorporates:
       *  Gain: '<S119>/Gain1'
       */
      basic_cosimulation_c_1_sm_master_B.MathFunction1.re =
        basic_cosimulation_c_1_sm_master_B.Gain1_f.re;
      basic_cosimulation_c_1_sm_master_B.MathFunction1.im =
        -basic_cosimulation_c_1_sm_master_B.Gain1_f.im;

      /* Gain: '<S120>/alpha' incorporates:
       *  RealImagToComplex: '<S126>/Real-Imag to Complex'
       */
      re_1j = basic_cosimulation_c_1_sm_master_P.alpha_Gain_az.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re -
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_az.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im;
      im_1j = basic_cosimulation_c_1_sm_master_P.alpha_Gain_az.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im +
        basic_cosimulation_c_1_sm_master_P.alpha_Gain_az.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re;

      /* Gain: '<S120>/alpha' */
      basic_cosimulation_c_1_sm_master_B.alpha_a.re = re_1j;
      basic_cosimulation_c_1_sm_master_B.alpha_a.im = im_1j;

      /* Gain: '<S120>/alpha2' incorporates:
       *  RealImagToComplex: '<S124>/Real-Imag to Complex'
       */
      re_1l = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_h.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.re -
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_h.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.im;
      im_1l = basic_cosimulation_c_1_sm_master_P.alpha2_Gain_h.re *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.im +
        basic_cosimulation_c_1_sm_master_P.alpha2_Gain_h.im *
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.re;

      /* Gain: '<S120>/alpha2' */
      basic_cosimulation_c_1_sm_master_B.alpha2_g.re = re_1l;
      basic_cosimulation_c_1_sm_master_B.alpha2_g.im = im_1l;

      /* Sum: '<S120>/Add' incorporates:
       *  Gain: '<S120>/alpha'
       *  Gain: '<S120>/alpha2'
       *  RealImagToComplex: '<S122>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_o.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.re +
         basic_cosimulation_c_1_sm_master_B.alpha2_g.re) +
        basic_cosimulation_c_1_sm_master_B.alpha_a.re;
      basic_cosimulation_c_1_sm_master_B.Add_o.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.im +
         basic_cosimulation_c_1_sm_master_B.alpha2_g.im) +
        basic_cosimulation_c_1_sm_master_B.alpha_a.im;

      /* Gain: '<S120>/Gain1' incorporates:
       *  Sum: '<S120>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_b.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_px *
        basic_cosimulation_c_1_sm_master_B.Add_o.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_b.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_px *
        basic_cosimulation_c_1_sm_master_B.Add_o.im;

      /* Product: '<S118>/Product1' incorporates:
       *  Gain: '<S120>/Gain1'
       *  Math: '<S118>/Math Function1'
       */
      re_1n = basic_cosimulation_c_1_sm_master_B.Gain1_b.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction1.re -
        basic_cosimulation_c_1_sm_master_B.Gain1_b.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction1.im;
      im_1n = basic_cosimulation_c_1_sm_master_B.Gain1_b.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction1.im +
        basic_cosimulation_c_1_sm_master_B.Gain1_b.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction1.re;

      /* Product: '<S118>/Product1' */
      basic_cosimulation_c_1_sm_master_B.Product1.re = re_1n;
      basic_cosimulation_c_1_sm_master_B.Product1.im = im_1n;

      /* Gain: '<S118>/Gain1' incorporates:
       *  Product: '<S118>/Product1'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_k.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_p3 *
        basic_cosimulation_c_1_sm_master_B.Product1.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_k.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_p3 *
        basic_cosimulation_c_1_sm_master_B.Product1.im;

      /* MultiPortSwitch: '<S118>/Multiport Switch' incorporates:
       *  Gain: '<S118>/Gain1'
       */
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.re =
        basic_cosimulation_c_1_sm_master_B.Gain1_k.re;
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.im =
        basic_cosimulation_c_1_sm_master_B.Gain1_k.im;
      break;

     default:
      /* Sum: '<S130>/Add' incorporates:
       *  RealImagToComplex: '<S121>/Real-Imag to Complex'
       *  RealImagToComplex: '<S123>/Real-Imag to Complex'
       *  RealImagToComplex: '<S125>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re +
         basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.re) +
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.re;
      basic_cosimulation_c_1_sm_master_B.Add.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im +
         basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.im) +
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.im;

      /* Gain: '<S130>/Gain1' incorporates:
       *  Sum: '<S130>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_f *
        basic_cosimulation_c_1_sm_master_B.Add.re;
      basic_cosimulation_c_1_sm_master_B.Gain1.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_f *
        basic_cosimulation_c_1_sm_master_B.Add.im;

      /* Math: '<S118>/Math Function2' incorporates:
       *  Gain: '<S130>/Gain1'
       */
      basic_cosimulation_c_1_sm_master_B.MathFunction2.re =
        basic_cosimulation_c_1_sm_master_B.Gain1.re;
      basic_cosimulation_c_1_sm_master_B.MathFunction2.im =
        -basic_cosimulation_c_1_sm_master_B.Gain1.im;

      /* Sum: '<S131>/Add' incorporates:
       *  RealImagToComplex: '<S122>/Real-Imag to Complex'
       *  RealImagToComplex: '<S124>/Real-Imag to Complex'
       *  RealImagToComplex: '<S126>/Real-Imag to Complex'
       */
      basic_cosimulation_c_1_sm_master_B.Add_p.re =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.re +
         basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.re) +
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re;
      basic_cosimulation_c_1_sm_master_B.Add_p.im =
        (basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.im +
         basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.im) +
        basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im;

      /* Gain: '<S131>/Gain1' incorporates:
       *  Sum: '<S131>/Add'
       */
      basic_cosimulation_c_1_sm_master_B.Gain1_l.re =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a0 *
        basic_cosimulation_c_1_sm_master_B.Add_p.re;
      basic_cosimulation_c_1_sm_master_B.Gain1_l.im =
        basic_cosimulation_c_1_sm_master_P.Gain1_Gain_a0 *
        basic_cosimulation_c_1_sm_master_B.Add_p.im;

      /* Product: '<S118>/Product2' incorporates:
       *  Gain: '<S131>/Gain1'
       *  Math: '<S118>/Math Function2'
       */
      re_1h = basic_cosimulation_c_1_sm_master_B.Gain1_l.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction2.re -
        basic_cosimulation_c_1_sm_master_B.Gain1_l.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction2.im;
      im_1h = basic_cosimulation_c_1_sm_master_B.Gain1_l.re *
        basic_cosimulation_c_1_sm_master_B.MathFunction2.im +
        basic_cosimulation_c_1_sm_master_B.Gain1_l.im *
        basic_cosimulation_c_1_sm_master_B.MathFunction2.re;

      /* Product: '<S118>/Product2' */
      basic_cosimulation_c_1_sm_master_B.Product2.re = re_1h;
      basic_cosimulation_c_1_sm_master_B.Product2.im = im_1h;

      /* Gain: '<S118>/Gain3' incorporates:
       *  Product: '<S118>/Product2'
       */
      basic_cosimulation_c_1_sm_master_B.Gain3.re =
        basic_cosimulation_c_1_sm_master_P.Gain3_Gain_n *
        basic_cosimulation_c_1_sm_master_B.Product2.re;
      basic_cosimulation_c_1_sm_master_B.Gain3.im =
        basic_cosimulation_c_1_sm_master_P.Gain3_Gain_n *
        basic_cosimulation_c_1_sm_master_B.Product2.im;

      /* MultiPortSwitch: '<S118>/Multiport Switch' incorporates:
       *  Gain: '<S118>/Gain3'
       */
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.re =
        basic_cosimulation_c_1_sm_master_B.Gain3.re;
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.im =
        basic_cosimulation_c_1_sm_master_B.Gain3.im;
      break;
    }

    /* End of MultiPortSwitch: '<S118>/Multiport Switch' */

    /* ComplexToRealImag: '<S118>/Complex to Real-Imag' incorporates:
     *  MultiPortSwitch: '<S118>/Multiport Switch'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_m =
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.re;

    /* ComplexToRealImag: '<S118>/Complex to Real-Imag' incorporates:
     *  MultiPortSwitch: '<S118>/Multiport Switch'
     */
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_f =
      basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.im;

    /* DataTypeConversion: '<S138>/Conversion' incorporates:
     *  Constant: '<S129>/Radial frequencies'
     */
    basic_cosimulation_c_1_sm_master_B.Conversion = 2.0 * 3.1415926535897931 *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_F_e *
      basic_cosimulation_c_1_sm_master_P.PowerMeasurementThreePhase_K_p;

    /* Gain: '<S116>/Gain' */
    u = 1.0 / basic_cosimulation_c_1_sm_master_P.Vnom_peak;

    /* Gain: '<S116>/Gain' */
    basic_cosimulation_c_1_sm_master_B.Gain_jf[0] = u *
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[0];
    basic_cosimulation_c_1_sm_master_B.Gain_jf[1] = u *
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[1];
    basic_cosimulation_c_1_sm_master_B.Gain_jf[2] = u *
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[2];

    /* Gain: '<S116>/Gain5' */
    basic_cosimulation_c_1_sm_master_B.Gain5_e =
      basic_cosimulation_c_1_sm_master_P.Gain5_Gain_l *
      basic_cosimulation_c_1_sm_master_B.Gain_jf[1];

    /* Gain: '<S116>/Gain6' */
    basic_cosimulation_c_1_sm_master_B.Gain6_p =
      basic_cosimulation_c_1_sm_master_P.Gain6_Gain_p3 *
      basic_cosimulation_c_1_sm_master_B.Gain_jf[2];

    /* Sum: '<S116>/Sum1' */
    basic_cosimulation_c_1_sm_master_B.Sum1_k =
      basic_cosimulation_c_1_sm_master_B.Gain5_e +
      basic_cosimulation_c_1_sm_master_B.Gain6_p;

    /* Gain: '<S116>/Gain4' */
    basic_cosimulation_c_1_sm_master_B.Gain4_k =
      basic_cosimulation_c_1_sm_master_P.Gain4_Gain_h *
      basic_cosimulation_c_1_sm_master_B.Sum1_k;

    /* Gain: '<S116>/Gain2' */
    basic_cosimulation_c_1_sm_master_B.Gain2_b3 =
      basic_cosimulation_c_1_sm_master_P.Gain2_Gain_i *
      basic_cosimulation_c_1_sm_master_B.Gain_jf[1];

    /* Gain: '<S116>/Gain3' */
    basic_cosimulation_c_1_sm_master_B.Gain3_j =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_c *
      basic_cosimulation_c_1_sm_master_B.Gain_jf[2];

    /* Sum: '<S116>/Sum' */
    basic_cosimulation_c_1_sm_master_B.Sum_l =
      (basic_cosimulation_c_1_sm_master_B.Gain_jf[0] +
       basic_cosimulation_c_1_sm_master_B.Gain2_b3) +
      basic_cosimulation_c_1_sm_master_B.Gain3_j;

    /* Gain: '<S116>/Gain1' */
    basic_cosimulation_c_1_sm_master_B.Gain1_ew =
      basic_cosimulation_c_1_sm_master_P.Gain1_Gain_kw *
      basic_cosimulation_c_1_sm_master_B.Sum_l;
  }

  /* Trigonometry: '<S116>/Cos2' */
  basic_cosimulation_c_1_sm_master_B.Cos2_h = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Trigonometry: '<S116>/Cos3' */
  basic_cosimulation_c_1_sm_master_B.Cos3_f = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
  }

  /* Trigonometry: '<S116>/Cos' */
  basic_cosimulation_c_1_sm_master_B.Cos_lc = cos
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Trigonometry: '<S116>/Cos1' */
  basic_cosimulation_c_1_sm_master_B.Cos1_i = sin
    (basic_cosimulation_c_1_sm_master_B.Integrator1);

  /* Product: '<S116>/Product' */
  basic_cosimulation_c_1_sm_master_B.Product_le =
    basic_cosimulation_c_1_sm_master_B.Gain4_k *
    basic_cosimulation_c_1_sm_master_B.Cos_lc;

  /* Product: '<S116>/Product1' */
  basic_cosimulation_c_1_sm_master_B.Product1_cs =
    basic_cosimulation_c_1_sm_master_B.Gain1_ew *
    basic_cosimulation_c_1_sm_master_B.Cos1_i;

  /* Sum: '<S116>/Sum3' */
  basic_cosimulation_c_1_sm_master_B.Sum3_dz =
    basic_cosimulation_c_1_sm_master_B.Product_le -
    basic_cosimulation_c_1_sm_master_B.Product1_cs;

  /* Gain: '<S116>/Gain7' */
  basic_cosimulation_c_1_sm_master_B.Gain7_i =
    basic_cosimulation_c_1_sm_master_P.Gain7_Gain_o *
    basic_cosimulation_c_1_sm_master_B.Sum3_dz;

  /* Integrator: '<S116>/Integrator' */
  basic_cosimulation_c_1_sm_master_B.Integrator_dd =
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_o;

  /* Product: '<S116>/Product2' */
  basic_cosimulation_c_1_sm_master_B.Product2_pp =
    basic_cosimulation_c_1_sm_master_B.Gain4_k *
    basic_cosimulation_c_1_sm_master_B.Cos2_h;

  /* Product: '<S116>/Product3' */
  basic_cosimulation_c_1_sm_master_B.Product3_o =
    basic_cosimulation_c_1_sm_master_B.Gain1_ew *
    basic_cosimulation_c_1_sm_master_B.Cos3_f;

  /* Sum: '<S116>/Sum2' */
  basic_cosimulation_c_1_sm_master_B.Sum2_b4 =
    basic_cosimulation_c_1_sm_master_B.Product2_pp +
    basic_cosimulation_c_1_sm_master_B.Product3_o;

  /* Sum: '<S116>/Sum4' */
  basic_cosimulation_c_1_sm_master_B.Sum4_ng =
    basic_cosimulation_c_1_sm_master_B.Integrator_dd +
    basic_cosimulation_c_1_sm_master_B.Gain7_i;

  /* Sum: '<S116>/Sum5' incorporates:
   *  Constant: '<S116>/Constant'
   */
  basic_cosimulation_c_1_sm_master_B.W_o =
    basic_cosimulation_c_1_sm_master_B.Sum4_ng +
    basic_cosimulation_c_1_sm_master_P.Constant_Value_ps;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Gain: '<S83>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_o =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_hy *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[11];

    /* Gain: '<S84>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_cr =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_b3 *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[12];

    /* Gain: '<S85>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_b =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_e *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[13];

    /* SignalConversion generated from: '<S9>/Delay2' */
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[0] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_o;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[1] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_cr;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[2] =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_b;

    /* Gain: '<S96>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_gz =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_f *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[6];

    /* Gain: '<S97>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_e =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_ow *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[7];

    /* Gain: '<S92>/Gain3' incorporates:
     *  Constant: '<S92>/Constant'
     */
    basic_cosimulation_c_1_sm_master_B.Gain3_p =
      basic_cosimulation_c_1_sm_master_P.Gain3_Gain_g *
      basic_cosimulation_c_1_sm_master_P.Phasor_to_Time_Domain_SFA_offse;

    /* Gain: '<S89>/do not delete this gain' */
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_j =
      basic_cosimulation_c_1_sm_master_P.donotdeletethisgain_Gain_ht *
      basic_cosimulation_c_1_sm_master_B.StateSpace_o1[17];

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

    /* S-Function (sfun_send_async): '<S15>/S-Function2' */

    /* Level2 S-Function Block: '<S15>/S-Function2' (sfun_send_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[14];
      sfcnOutputs(rts,0);
    }

    /* S-Function (sfun_gen_async_ctrl): '<S3>/OpIPSocketCtrl1' */

    /* Level2 S-Function Block: '<S3>/OpIPSocketCtrl1' (sfun_gen_async_ctrl) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[15];
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
  int32_T i_1;
  int32_T i_2;
  int32_T i_3;
  int_T idxDelay;

  /* Update for Integrator: '<S116>/Integrator1' */
  basic_cosimulation_c_1_sm_master_DW.Integrator1_IWORK = 0;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Update for Memory: '<S2>/S-Function' */
    basic_cosimulation_c_1_sm_master_DW.SFunction_PreviousInput =
      basic_cosimulation_c_1_sm_master_B.Sum;

    /* Update for UnitDelay: '<S102>/Unit Delay' */
    for (i = 0; i < 6; i = i + 1) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE[i] =
        basic_cosimulation_c_1_sm_master_B.Saturation_b[i];
    }

    /* End of Update for UnitDelay: '<S102>/Unit Delay' */

    /* Update for Delay: '<S110>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[1];

    /* Update for Enabled SubSystem: '<S102>/Tail' */
    basic_cosimulation__Tail_Update
      (basic_cosimulation_c_1_sm_master_B.DataTypeConversion,
       &basic_cosimulation_c_1_sm_master_B.Tail_f,
       &basic_cosimulation_c_1_sm_master_DW.Tail_f,
       &basic_cosimulation_c_1_sm_master_P.Tail_f);

    /* End of Update for SubSystem: '<S102>/Tail' */

    /* Update for DiscreteIntegrator: '<S153>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval *
      basic_cosimulation_c_1_sm_master_B.Gain3_p;

    /* Update for DiscreteIntegrator: '<S165>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_m *
      basic_cosimulation_c_1_sm_master_B.Gain3_a;

    /* Update for Delay: '<S9>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_b =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_e;

    /* Update for Delay: '<S9>/Delay1' */
    basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_gz;

    /* Update for Delay: '<S7>/Delay2' */
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[0];
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[1];
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1 << 1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[2];

    /* Update for Delay: '<S7>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_f =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_i;

    /* Update for Delay: '<S7>/Delay1' */
    basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE_a =
      basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_he;

    /* Update for Delay: '<S3>/Delay4' */
    for (i_0 = 0; i_0 < 6; i_0 = i_0 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay4_DSTATE[(int32_T)
        basic_cosimulation_c_1_sm_master_DW.CircBufIdx * 6 + i_0] =
        basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay4Inpo[i_0];
    }

    if (basic_cosimulation_c_1_sm_master_DW.CircBufIdx < 4U) {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx =
        basic_cosimulation_c_1_sm_master_DW.CircBufIdx + 1U;
    } else {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx = 0U;
    }

    /* End of Update for Delay: '<S3>/Delay4' */

    /* Update for Delay: '<S3>/Delay5' */
    for (i_1 = 0; i_1 < 6; i_1 = i_1 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay5_DSTATE[(int32_T)
        basic_cosimulation_c_1_sm_master_DW.CircBufIdx_d * 6 + i_1] =
        basic_cosimulation_c_1_sm_master_B.SFunction1_o3[i_1];
    }

    if (basic_cosimulation_c_1_sm_master_DW.CircBufIdx_d < 4U) {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_d =
        basic_cosimulation_c_1_sm_master_DW.CircBufIdx_d + 1U;
    } else {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_d = 0U;
    }

    /* End of Update for Delay: '<S3>/Delay5' */

    /* Update for Delay: '<S3>/Delay6' */
    basic_cosimulation_c_1_sm_master_DW.Delay6_DSTATE[(int32_T)
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_a * 3 + 0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[0];
    basic_cosimulation_c_1_sm_master_DW.Delay6_DSTATE[(int32_T)
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_a * 3 + 1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[1];
    basic_cosimulation_c_1_sm_master_DW.Delay6_DSTATE[(int32_T)
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_a * 3 + (1 << 1)] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[2];
    if (basic_cosimulation_c_1_sm_master_DW.CircBufIdx_a < 4U) {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_a =
        basic_cosimulation_c_1_sm_master_DW.CircBufIdx_a + 1U;
    } else {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_a = 0U;
    }

    /* End of Update for Delay: '<S3>/Delay6' */

    /* Update for Delay: '<S3>/Delay7' */
    for (i_2 = 0; i_2 < 6; i_2 = i_2 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay7_DSTATE[(int32_T)
        basic_cosimulation_c_1_sm_master_DW.CircBufIdx_o * 6 + i_2] =
        basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay7Inpo[i_2];
    }

    if (basic_cosimulation_c_1_sm_master_DW.CircBufIdx_o < 4U) {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_o =
        basic_cosimulation_c_1_sm_master_DW.CircBufIdx_o + 1U;
    } else {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_o = 0U;
    }

    /* End of Update for Delay: '<S3>/Delay7' */

    /* Update for Delay: '<S3>/Delay8' */
    basic_cosimulation_c_1_sm_master_DW.Delay8_DSTATE[((int32_T)
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_g << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.SFunction_l[0 + 1];
    basic_cosimulation_c_1_sm_master_DW.Delay8_DSTATE[((int32_T)
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_g << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.SFunction_l[1 + 1];
    if (basic_cosimulation_c_1_sm_master_DW.CircBufIdx_g < 4U) {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_g =
        basic_cosimulation_c_1_sm_master_DW.CircBufIdx_g + 1U;
    } else {
      basic_cosimulation_c_1_sm_master_DW.CircBufIdx_g = 0U;
    }

    /* End of Update for Delay: '<S3>/Delay8' */

    /* Update for UnitDelay: '<S37>/Unit Delay' */
    for (i_3 = 0; i_3 < 6; i_3 = i_3 + 1) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE_g[i_3] =
        basic_cosimulation_c_1_sm_master_B.Saturation_h[i_3];
    }

    /* End of Update for UnitDelay: '<S37>/Unit Delay' */

    /* Update for Delay: '<S45>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[1];
  }

  /* Update for Integrator: '<S51>/Integrator2' */
  basic_cosimulation_c_1_sm_master_DW.Integrator2_IWORK = 0;
  if (rtmIsMajorTimeStep(basic_cosimulation_c_1_sm_master_M)) {
    /* Update for Enabled SubSystem: '<S37>/Tail' */
    basic_cosimulation__Tail_Update
      (basic_cosimulation_c_1_sm_master_B.DataTypeConversion_o,
       &basic_cosimulation_c_1_sm_master_B.Tail,
       &basic_cosimulation_c_1_sm_master_DW.Tail,
       &basic_cosimulation_c_1_sm_master_P.Tail);

    /* End of Update for SubSystem: '<S37>/Tail' */

    /* Update for Sin: '<S78>/Sine Wave A' */
    HoldSine = basic_cosimulation_c_1_sm_master_DW.lastSin;
    HoldCosine = basic_cosimulation_c_1_sm_master_DW.lastCos;
    basic_cosimulation_c_1_sm_master_DW.lastSin = HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveA_HCos + HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveA_Hsin;
    basic_cosimulation_c_1_sm_master_DW.lastCos = HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveA_HCos - HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveA_Hsin;

    /* Update for Sin: '<S78>/Sine Wave B' */
    HoldSine = basic_cosimulation_c_1_sm_master_DW.lastSin_f;
    HoldCosine = basic_cosimulation_c_1_sm_master_DW.lastCos_m;
    basic_cosimulation_c_1_sm_master_DW.lastSin_f = HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveB_HCos + HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveB_Hsin;
    basic_cosimulation_c_1_sm_master_DW.lastCos_m = HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveB_HCos - HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveB_Hsin;

    /* Update for Sin: '<S78>/Sine Wave C' */
    HoldSine = basic_cosimulation_c_1_sm_master_DW.lastSin_k;
    HoldCosine = basic_cosimulation_c_1_sm_master_DW.lastCos_i;
    basic_cosimulation_c_1_sm_master_DW.lastSin_k = HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveC_HCos + HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveC_Hsin;
    basic_cosimulation_c_1_sm_master_DW.lastCos_i = HoldCosine *
      basic_cosimulation_c_1_sm_master_P.SineWaveC_HCos - HoldSine *
      basic_cosimulation_c_1_sm_master_P.SineWaveC_Hsin;

    /* Update for Delay: '<S47>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[1];

    /* Update for Delay: '<S48>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[1];
  }

  /* Update for TransportDelay: '<S57>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];

    {
      int_T i1;
      const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Integrator_c[0];
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

  /* Update for TransportDelay: '<S59>/Transport Delay' */
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

  /* Update for TransportDelay: '<S61>/Transport Delay' */
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

  /* Update for TransportDelay: '<S56>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &basic_cosimulation_c_1_sm_master_DW.TransportDelay_PWORK_h.TUbufferPtrs[0];
    real_T simTime = basic_cosimulation_c_1_sm_master_M->Timing.t[0];

    {
      int_T i1;
      const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Integrator_b[0];
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

  /* Update for TransportDelay: '<S58>/Transport Delay' */
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

  /* Update for TransportDelay: '<S60>/Transport Delay' */
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
    /* Update for DiscreteIntegrator: '<S45>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE +
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator_gainval *
      basic_cosimulation_c_1_sm_master_B.Gain2_n;

    /* Update for DiscreteIntegrator: '<S45>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE +
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator1_gainval *
      basic_cosimulation_c_1_sm_master_B.Gain4_m;

    /* Update for Delay: '<S9>/Delay2' */
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[0];
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[1];
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1 << 1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[2];

    /* Update for Delay: '<S112>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c3[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c3[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[1];

    /* Update for DiscreteIntegrator: '<S110>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_i =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_i +
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator_gainva_j *
      basic_cosimulation_c_1_sm_master_B.Gain2_k;

    /* Update for DiscreteIntegrator: '<S110>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_l =
      basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_l +
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator1_gainv_g *
      basic_cosimulation_c_1_sm_master_B.Gain4_d;

    /* Update for Delay: '<S113>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[1];

    /* Update for DiscreteIntegrator: '<S140>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f =
      basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_o *
      basic_cosimulation_c_1_sm_master_B.Conversion;

    /* Update for DiscreteIntegrator: '<S122>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_l *
      basic_cosimulation_c_1_sm_master_B.Product1_hb[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_l *
      basic_cosimulation_c_1_sm_master_B.Product1_hb[1];

    /* Update for UnitDelay: '<S133>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[1];

    /* Update for UnitDelay: '<S133>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_d[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_d[1];

    /* Update for Delay: '<S133>/Delay' */
    for (idxDelay = 0; idxDelay < 332; idxDelay = idxDelay + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[(idxDelay << 1) + 0] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[((idxDelay + 1) << 1)
        + 0];
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[(idxDelay << 1) + 1] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[((idxDelay + 1) << 1)
        + 1];
    }

    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[(332 << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[(332 << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_c3[1];

    /* End of Update for Delay: '<S133>/Delay' */

    /* Update for DiscreteIntegrator: '<S124>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_p = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_b *
      basic_cosimulation_c_1_sm_master_B.Product1_ku[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_b *
      basic_cosimulation_c_1_sm_master_B.Product1_ku[1];

    /* Update for UnitDelay: '<S135>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_a[1];

    /* Update for UnitDelay: '<S135>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_lg[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_lg[1];

    /* Update for Delay: '<S135>/Delay' */
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

    /* End of Update for Delay: '<S135>/Delay' */

    /* Update for DiscreteIntegrator: '<S126>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_i = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_d *
      basic_cosimulation_c_1_sm_master_B.Product1_ew1[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_d *
      basic_cosimulation_c_1_sm_master_B.Product1_ew1[1];

    /* Update for UnitDelay: '<S137>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[1];

    /* Update for UnitDelay: '<S137>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_k[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_k[1];

    /* Update for Delay: '<S137>/Delay' */
    for (idxDelay = 0; idxDelay < 332; idxDelay = idxDelay + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[(idxDelay << 1) + 0] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[((idxDelay + 1) << 1)
        + 0];
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[(idxDelay << 1) + 1] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[((idxDelay + 1) << 1)
        + 1];
    }

    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[(332 << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[(332 << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_b5[1];

    /* End of Update for Delay: '<S137>/Delay' */

    /* Update for DiscreteIntegrator: '<S121>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_h = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_c *
      basic_cosimulation_c_1_sm_master_B.Product1_e1[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_c *
      basic_cosimulation_c_1_sm_master_B.Product1_e1[1];

    /* Update for UnitDelay: '<S132>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_d[1];

    /* Update for UnitDelay: '<S132>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_in[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_in[1];

    /* Update for Delay: '<S132>/Delay' */
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

    /* End of Update for Delay: '<S132>/Delay' */

    /* Update for DiscreteIntegrator: '<S123>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_iy = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_h *
      basic_cosimulation_c_1_sm_master_B.Product1_lz[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_h *
      basic_cosimulation_c_1_sm_master_B.Product1_lz[1];

    /* Update for UnitDelay: '<S134>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[1];

    /* Update for UnitDelay: '<S134>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_j[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_j[1];

    /* Update for Delay: '<S134>/Delay' */
    for (idxDelay = 0; idxDelay < 332; idxDelay = idxDelay + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[(idxDelay << 1) + 0] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[((idxDelay + 1) << 1)
        + 0];
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[(idxDelay << 1) + 1] =
        basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[((idxDelay + 1) << 1)
        + 1];
    }

    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[(332 << 1) + 0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[0];
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[(332 << 1) + 1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_mb[1];

    /* End of Update for Delay: '<S134>/Delay' */

    /* Update for DiscreteIntegrator: '<S125>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_a = 0;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_i *
      basic_cosimulation_c_1_sm_master_B.Product1_k4[0];
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1] +
      basic_cosimulation_c_1_sm_master_P.Integrator_gainval_i *
      basic_cosimulation_c_1_sm_master_B.Product1_k4[1];

    /* Update for UnitDelay: '<S136>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[0] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[1] =
      basic_cosimulation_c_1_sm_master_B.Integrator_o[1];

    /* Update for UnitDelay: '<S136>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[0] =
      basic_cosimulation_c_1_sm_master_B.Delay_jw[0];
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[1] =
      basic_cosimulation_c_1_sm_master_B.Delay_jw[1];

    /* Update for Delay: '<S136>/Delay' */
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

    /* End of Update for Delay: '<S136>/Delay' */
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

  /* Derivatives for Integrator: '<S116>/Integrator1' */
  lsat = (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE <=
          basic_cosimulation_c_1_sm_master_P.Integrator1_LowerSat);
  usat = (basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE >=
          basic_cosimulation_c_1_sm_master_P.Integrator1_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (basic_cosimulation_c_1_sm_master_B.W_o >
        0.0)) || (usat && (basic_cosimulation_c_1_sm_master_B.W_o < 0.0))) {
    _rtXdot->Integrator1_CSTATE = basic_cosimulation_c_1_sm_master_B.W_o;
  } else {
    /* in saturation */
    _rtXdot->Integrator1_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S116>/Integrator1' */

  /* Derivatives for Integrator: '<S51>/Integrator2' */
  lsat = (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE <=
          basic_cosimulation_c_1_sm_master_P.Integrator2_LowerSat);
  usat = (basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE >=
          basic_cosimulation_c_1_sm_master_P.Integrator2_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (basic_cosimulation_c_1_sm_master_B.W >
        0.0)) || (usat && (basic_cosimulation_c_1_sm_master_B.W < 0.0))) {
    _rtXdot->Integrator2_CSTATE = basic_cosimulation_c_1_sm_master_B.W;
  } else {
    /* in saturation */
    _rtXdot->Integrator2_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S51>/Integrator2' */

  /* Derivatives for Integrator: '<S57>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_k[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S59>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_a[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_b[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S61>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_d[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_l[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S56>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_g[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_e[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S58>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_m[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_ew[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S60>/Integrator' */
  memcpy(&_rtXdot->Integrator_CSTATE_h[0],
         &basic_cosimulation_c_1_sm_master_B.Product1_bw[0], 8U * sizeof(real_T));

  /* Derivatives for Integrator: '<S68>/Integrator' */
  _rtXdot->Integrator_CSTATE_b[0] =
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[0];
  _rtXdot->Integrator_CSTATE_b[1] =
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[1];
  _rtXdot->Integrator_CSTATE_b[2] =
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[2];
  _rtXdot->Integrator_CSTATE_b[3] =
    basic_cosimulation_c_1_sm_master_B.DataTypeConversion_h[3];

  /* Derivatives for Integrator: '<S51>/Integrator' */
  _rtXdot->Integrator_CSTATE_hp = basic_cosimulation_c_1_sm_master_B.Gain7_n;

  /* Derivatives for Integrator: '<S116>/Integrator' */
  _rtXdot->Integrator_CSTATE_o = basic_cosimulation_c_1_sm_master_B.Gain7_i;
}

/* Model initialize function */
void basic_cosimulation_c_1_sm_master_initialize(void)
{
  /* Start for Constant: '<S3>/timeout' */
  basic_cosimulation_c_1_sm_master_B.timeout =
    basic_cosimulation_c_1_sm_master_P.timeout_Value;

  /* Start for Constant: '<S91>/Constant11' */
  basic_cosimulation_c_1_sm_master_B.Constant11 =
    basic_cosimulation_c_1_sm_master_P.fsw_inv;

  /* Start for S-Function (rte_svpwm): '<S91>/RTE SPWM1' */
  /* Level2 S-Function Block: '<S91>/RTE SPWM1' (rte_svpwm) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[2];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for S-Function (rte_conversion): '<S91>/RTE Conversion1' */
  /* Level2 S-Function Block: '<S91>/RTE Conversion1' (rte_conversion) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[3];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for InitialCondition: '<S153>/Initial' */
  basic_cosimulation_c_1_sm_master_B.Initial =
    basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDi_mg;

  /* Start for InitialCondition: '<S153>/Initial' */
  basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime = true;

  /* Start for S-Function (ssn_sfun_v1): '<S174>/State-Space' */
  /* Level2 S-Function Block: '<S174>/State-Space' (ssn_sfun_v1) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[4];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for InitialCondition: '<S165>/Initial' */
  basic_cosimulation_c_1_sm_master_B.Initial_b =
    basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDis_a;

  /* Start for InitialCondition: '<S165>/Initial' */
  basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime_i = true;

  /* Start for Constant: '<S28>/Constant11' */
  basic_cosimulation_c_1_sm_master_B.Constant11_i =
    basic_cosimulation_c_1_sm_master_P.fsw_inv;

  /* Start for S-Function (rte_svpwm): '<S28>/RTE SPWM1' */
  /* Level2 S-Function Block: '<S28>/RTE SPWM1' (rte_svpwm) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[11];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for S-Function (rte_conversion): '<S28>/RTE Conversion1' */
  /* Level2 S-Function Block: '<S28>/RTE Conversion1' (rte_conversion) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[12];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for S-Function (ssn_sfun_v1): '<S173>/State-Space' */
  /* Level2 S-Function Block: '<S173>/State-Space' (ssn_sfun_v1) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[13];
    sfcnStart(rts);
    if (ssGetErrorStatus(rts) != (NULL))
      return;
  }

  /* Start for TransportDelay: '<S57>/Transport Delay' */
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

  /* Start for TransportDelay: '<S59>/Transport Delay' */
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

  /* Start for TransportDelay: '<S61>/Transport Delay' */
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

  /* Start for TransportDelay: '<S56>/Transport Delay' */
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

  /* Start for TransportDelay: '<S58>/Transport Delay' */
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

  /* Start for TransportDelay: '<S60>/Transport Delay' */
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

  /* Start for InitialCondition: '<S140>/Initial' */
  basic_cosimulation_c_1_sm_master_B.Initial_f =
    basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDi_al;

  /* Start for InitialCondition: '<S140>/Initial' */
  basic_cosimulation_c_1_sm_master_DW.Initial_FirstOutputTime_ic = true;
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
    int32_T i_d;
    int32_T i_e;
    int32_T i_f;
    int32_T i_g;
    int32_T i_h;

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

    /* InitializeConditions for S-Function (sfun_recv_async): '<S5>/S-Function1' */
    /* Level2 S-Function Block: '<S5>/S-Function1' (sfun_recv_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[0];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for UnitDelay: '<S102>/Unit Delay' */
    for (i = 0; i < 6; i = i + 1) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE[i] =
        basic_cosimulation_c_1_sm_master_P.UnitDelay_InitialCondition;
    }

    /* End of InitializeConditions for UnitDelay: '<S102>/Unit Delay' */

    /* InitializeConditions for Delay: '<S110>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition;

    /* InitializeConditions for Integrator: '<S116>/Integrator1' */
    if (rtmIsFirstInitCond(basic_cosimulation_c_1_sm_master_M)) {
      basic_cosimulation_c_1_sm_master_X.Integrator1_CSTATE = 0.0;
    }

    basic_cosimulation_c_1_sm_master_DW.Integrator1_IWORK = 1;

    /* End of InitializeConditions for Integrator: '<S116>/Integrator1' */

    /* InitializeConditions for S-Function (RECV_Param): '<S17>/S-Function' */
    /* Level2 S-Function Block: '<S17>/S-Function' (RECV_Param) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[1];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (rte_svpwm): '<S91>/RTE SPWM1' */
    /* Level2 S-Function Block: '<S91>/RTE SPWM1' (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[2];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (rte_conversion): '<S91>/RTE Conversion1' */
    /* Level2 S-Function Block: '<S91>/RTE Conversion1' (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[3];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for DiscreteIntegrator: '<S153>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE =
      basic_cosimulation_c_1_sm_master_B.Initial;

    /* InitializeConditions for S-Function (ssn_sfun_v1): '<S174>/State-Space' */
    /* Level2 S-Function Block: '<S174>/State-Space' (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[4];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for DiscreteIntegrator: '<S165>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a =
      basic_cosimulation_c_1_sm_master_B.Initial_b;

    /* InitializeConditions for Delay: '<S9>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_b =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_b;

    /* InitializeConditions for Delay: '<S9>/Delay1' */
    basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE =
      basic_cosimulation_c_1_sm_master_P.Delay1_InitialCondition;

    /* InitializeConditions for Delay: '<S7>/Delay2' */
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[2] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition;

    /* InitializeConditions for Delay: '<S7>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_f =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_l;

    /* InitializeConditions for Delay: '<S7>/Delay1' */
    basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE_a =
      basic_cosimulation_c_1_sm_master_P.Delay1_InitialCondition_c;

    /* InitializeConditions for S-Function (OP_SEND): '<S172>/S-Function' */
    /* Level2 S-Function Block: '<S172>/S-Function' (OP_SEND) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[5];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for Delay: '<S3>/Delay4' */
    for (i_0 = 0; i_0 < 30; i_0 = i_0 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay4_DSTATE[i_0] =
        basic_cosimulation_c_1_sm_master_P.Delay4_InitialCondition;
    }

    basic_cosimulation_c_1_sm_master_DW.CircBufIdx = 0U;

    /* End of InitializeConditions for Delay: '<S3>/Delay4' */

    /* InitializeConditions for S-Function (opwritefile): '<S8>/OpWriteFile26' */
    /* Level2 S-Function Block: '<S8>/OpWriteFile26' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[6];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for Delay: '<S3>/Delay5' */
    for (i_1 = 0; i_1 < 30; i_1 = i_1 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay5_DSTATE[i_1] =
        basic_cosimulation_c_1_sm_master_P.Delay5_InitialCondition;
    }

    basic_cosimulation_c_1_sm_master_DW.CircBufIdx_d = 0U;

    /* End of InitializeConditions for Delay: '<S3>/Delay5' */

    /* InitializeConditions for S-Function (opwritefile): '<S8>/OpWriteFile27' */
    /* Level2 S-Function Block: '<S8>/OpWriteFile27' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[7];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for Delay: '<S3>/Delay6' */
    for (i_2 = 0; i_2 < 15; i_2 = i_2 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay6_DSTATE[i_2] =
        basic_cosimulation_c_1_sm_master_P.Delay6_InitialCondition;
    }

    basic_cosimulation_c_1_sm_master_DW.CircBufIdx_a = 0U;

    /* End of InitializeConditions for Delay: '<S3>/Delay6' */

    /* InitializeConditions for S-Function (opwritefile): '<S8>/OpWriteFile28' */
    /* Level2 S-Function Block: '<S8>/OpWriteFile28' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[8];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for Delay: '<S3>/Delay7' */
    for (i_3 = 0; i_3 < 30; i_3 = i_3 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay7_DSTATE[i_3] =
        basic_cosimulation_c_1_sm_master_P.Delay7_InitialCondition;
    }

    basic_cosimulation_c_1_sm_master_DW.CircBufIdx_o = 0U;

    /* End of InitializeConditions for Delay: '<S3>/Delay7' */

    /* InitializeConditions for S-Function (opwritefile): '<S8>/OpWriteFile29' */
    /* Level2 S-Function Block: '<S8>/OpWriteFile29' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[9];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for Delay: '<S3>/Delay8' */
    for (i_4 = 0; i_4 < 10; i_4 = i_4 + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay8_DSTATE[i_4] =
        basic_cosimulation_c_1_sm_master_P.Delay8_InitialCondition;
    }

    basic_cosimulation_c_1_sm_master_DW.CircBufIdx_g = 0U;

    /* End of InitializeConditions for Delay: '<S3>/Delay8' */

    /* InitializeConditions for S-Function (opwritefile): '<S8>/OpWriteFile30' */
    /* Level2 S-Function Block: '<S8>/OpWriteFile30' (opwritefile) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[10];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for UnitDelay: '<S37>/Unit Delay' */
    for (i_5 = 0; i_5 < 6; i_5 = i_5 + 1) {
      basic_cosimulation_c_1_sm_master_DW.UnitDelay_DSTATE_g[i_5] =
        basic_cosimulation_c_1_sm_master_P.UnitDelay_InitialCondition_a;
    }

    /* End of InitializeConditions for UnitDelay: '<S37>/Unit Delay' */

    /* InitializeConditions for Delay: '<S45>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_f;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_h[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_f;

    /* InitializeConditions for Integrator: '<S51>/Integrator2' */
    if (rtmIsFirstInitCond(basic_cosimulation_c_1_sm_master_M)) {
      basic_cosimulation_c_1_sm_master_X.Integrator2_CSTATE = 0.0;
    }

    basic_cosimulation_c_1_sm_master_DW.Integrator2_IWORK = 1;

    /* End of InitializeConditions for Integrator: '<S51>/Integrator2' */

    /* InitializeConditions for S-Function (rte_svpwm): '<S28>/RTE SPWM1' */
    /* Level2 S-Function Block: '<S28>/RTE SPWM1' (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[11];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (rte_conversion): '<S28>/RTE Conversion1' */
    /* Level2 S-Function Block: '<S28>/RTE Conversion1' (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[12];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (ssn_sfun_v1): '<S173>/State-Space' */
    /* Level2 S-Function Block: '<S173>/State-Space' (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[13];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for Delay: '<S47>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_fz;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_fz;

    /* InitializeConditions for Delay: '<S48>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_a;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_a;

    /* InitializeConditions for Integrator: '<S57>/Integrator' */
    for (i_6 = 0; i_6 < 8; i_6 = i_6 + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE[i_6] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC;
    }

    /* End of InitializeConditions for Integrator: '<S57>/Integrator' */

    /* InitializeConditions for Integrator: '<S59>/Integrator' */
    for (i_7 = 0; i_7 < 8; i_7 = i_7 + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_a[i_7] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_e;
    }

    /* End of InitializeConditions for Integrator: '<S59>/Integrator' */

    /* InitializeConditions for Integrator: '<S61>/Integrator' */
    for (i_8 = 0; i_8 < 8; i_8 = i_8 + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_d[i_8] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_i;
    }

    /* End of InitializeConditions for Integrator: '<S61>/Integrator' */

    /* InitializeConditions for Integrator: '<S56>/Integrator' */
    for (i_9 = 0; i_9 < 8; i_9 = i_9 + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_g[i_9] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_b;
    }

    /* End of InitializeConditions for Integrator: '<S56>/Integrator' */

    /* InitializeConditions for Integrator: '<S58>/Integrator' */
    for (i_a = 0; i_a < 8; i_a = i_a + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_m[i_a] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_i2;
    }

    /* End of InitializeConditions for Integrator: '<S58>/Integrator' */

    /* InitializeConditions for Integrator: '<S60>/Integrator' */
    for (i_b = 0; i_b < 8; i_b = i_b + 1) {
      basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_h[i_b] =
        basic_cosimulation_c_1_sm_master_P.Integrator_IC_h;
    }

    /* End of InitializeConditions for Integrator: '<S60>/Integrator' */

    /* InitializeConditions for DiscreteIntegrator: '<S45>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE =
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator_IC;

    /* InitializeConditions for DiscreteIntegrator: '<S45>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE =
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator1_IC;

    /* InitializeConditions for Integrator: '<S68>/Integrator' */
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[0] =
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDi_h2;
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[1] =
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDi_h2;
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[2] =
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDi_h2;
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_b[3] =
      basic_cosimulation_c_1_sm_master_P.IntegratorwithWrappedStateDi_h2;

    /* InitializeConditions for Integrator: '<S51>/Integrator' */
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_hp =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_n;

    /* InitializeConditions for Delay: '<S9>/Delay2' */
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition_i;
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition_i;
    basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[2] =
      basic_cosimulation_c_1_sm_master_P.Delay2_InitialCondition_i;

    /* InitializeConditions for Delay: '<S112>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c3[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_i;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c3[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_i;

    /* InitializeConditions for DiscreteIntegrator: '<S110>/Discrete-Time Integrator' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_i =
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator_IC_p;

    /* InitializeConditions for DiscreteIntegrator: '<S110>/Discrete-Time Integrator1' */
    basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_l =
      basic_cosimulation_c_1_sm_master_P.DiscreteTimeIntegrator1_IC_m;

    /* InitializeConditions for Delay: '<S113>/Delay' */
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_ai;
    basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_ai;

    /* InitializeConditions for DiscreteIntegrator: '<S140>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f =
      basic_cosimulation_c_1_sm_master_B.Initial_f;

    /* InitializeConditions for DiscreteIntegrator: '<S122>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_eu;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_h[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_eu;

    /* InitializeConditions for UnitDelay: '<S133>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition;

    /* InitializeConditions for UnitDelay: '<S133>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition;

    /* InitializeConditions for Delay: '<S133>/Delay' */
    for (i_c = 0; i_c < 666; i_c = i_c + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_l[i_c] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_k;
    }

    /* End of InitializeConditions for Delay: '<S133>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S124>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_j;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_j;

    /* InitializeConditions for UnitDelay: '<S135>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_b;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_j[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_b;

    /* InitializeConditions for UnitDelay: '<S135>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_a;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_p[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_a;

    /* InitializeConditions for Delay: '<S135>/Delay' */
    for (i_d = 0; i_d < 666; i_d = i_d + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_g[i_d] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_lj;
    }

    /* End of InitializeConditions for Delay: '<S135>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S126>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_a;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_j[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_a;

    /* InitializeConditions for UnitDelay: '<S137>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_d;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_e[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_d;

    /* InitializeConditions for UnitDelay: '<S137>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_b;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_c[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_b;

    /* InitializeConditions for Delay: '<S137>/Delay' */
    for (i_e = 0; i_e < 666; i_e = i_e + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_cx[i_e] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_p;
    }

    /* End of InitializeConditions for Delay: '<S137>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S121>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_m;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_jv[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_m;

    /* InitializeConditions for UnitDelay: '<S132>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_p;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_d[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_p;

    /* InitializeConditions for UnitDelay: '<S132>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_h;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_k[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_h;

    /* InitializeConditions for Delay: '<S132>/Delay' */
    for (i_f = 0; i_f < 666; i_f = i_f + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_a[i_f] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_o;
    }

    /* End of InitializeConditions for Delay: '<S132>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S123>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_c;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_g[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_c;

    /* InitializeConditions for UnitDelay: '<S134>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_a;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_i[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_a;

    /* InitializeConditions for UnitDelay: '<S134>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_e;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_pc[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_e;

    /* InitializeConditions for Delay: '<S134>/Delay' */
    for (i_g = 0; i_g < 666; i_g = i_g + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[i_g] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_kv;
    }

    /* End of InitializeConditions for Delay: '<S134>/Delay' */

    /* InitializeConditions for DiscreteIntegrator: '<S125>/Integrator' */
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[0] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_my;
    basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_l[1] =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_my;

    /* InitializeConditions for UnitDelay: '<S136>/Unit Delay1' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_po;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay1_DSTATE_k[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay1_InitialCondition_po;

    /* InitializeConditions for UnitDelay: '<S136>/Unit Delay2' */
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[0] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_k;
    basic_cosimulation_c_1_sm_master_DW.UnitDelay2_DSTATE_d[1] =
      basic_cosimulation_c_1_sm_master_P.UnitDelay2_InitialCondition_k;

    /* InitializeConditions for Delay: '<S136>/Delay' */
    for (i_h = 0; i_h < 666; i_h = i_h + 1) {
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_m[i_h] =
        basic_cosimulation_c_1_sm_master_P.Delay_InitialCondition_c;
    }

    /* End of InitializeConditions for Delay: '<S136>/Delay' */

    /* InitializeConditions for Integrator: '<S116>/Integrator' */
    basic_cosimulation_c_1_sm_master_X.Integrator_CSTATE_o =
      basic_cosimulation_c_1_sm_master_P.Integrator_IC_ie;

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

    /* InitializeConditions for S-Function (sfun_send_async): '<S15>/S-Function2' */
    /* Level2 S-Function Block: '<S15>/S-Function2' (sfun_send_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[14];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* InitializeConditions for S-Function (sfun_gen_async_ctrl): '<S3>/OpIPSocketCtrl1' */
    /* Level2 S-Function Block: '<S3>/OpIPSocketCtrl1' (sfun_gen_async_ctrl) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[15];
      sfcnInitializeConditions(rts);
      if (ssGetErrorStatus(rts) != (NULL))
        return;
    }

    /* SystemInitialize for Enabled SubSystem: '<S102>/Tail' */
    basic_cosimulation_c__Tail_Init(&basic_cosimulation_c_1_sm_master_B.Tail_f,
      &basic_cosimulation_c_1_sm_master_DW.Tail_f,
      &basic_cosimulation_c_1_sm_master_P.Tail_f);

    /* End of SystemInitialize for SubSystem: '<S102>/Tail' */

    /* SystemInitialize for Enabled SubSystem: '<S37>/Tail' */
    basic_cosimulation_c__Tail_Init(&basic_cosimulation_c_1_sm_master_B.Tail,
      &basic_cosimulation_c_1_sm_master_DW.Tail,
      &basic_cosimulation_c_1_sm_master_P.Tail);

    /* End of SystemInitialize for SubSystem: '<S37>/Tail' */

    /* InitializeConditions for root-level periodic continuous states */
    {
      int_T rootPeriodicContStateIndices[4] = { 50, 51, 52, 53 };

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

  /* Enable for Sin: '<S78>/Sine Wave A' */
  basic_cosimulation_c_1_sm_master_DW.systemEnable = 1;

  /* Enable for Sin: '<S78>/Sine Wave B' */
  basic_cosimulation_c_1_sm_master_DW.systemEnable_n = 1;

  /* Enable for Sin: '<S78>/Sine Wave C' */
  basic_cosimulation_c_1_sm_master_DW.systemEnable_j = 1;

  /* Enable for DiscreteIntegrator: '<S122>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE = 1;

  /* Enable for DiscreteIntegrator: '<S124>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_p = 1;

  /* Enable for DiscreteIntegrator: '<S126>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_i = 1;

  /* Enable for DiscreteIntegrator: '<S121>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_h = 1;

  /* Enable for DiscreteIntegrator: '<S123>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_iy = 1;

  /* Enable for DiscreteIntegrator: '<S125>/Integrator' */
  basic_cosimulation_c_1_sm_master_DW.Integrator_SYSTEM_ENABLE_a = 1;
}

/* Model terminate function */
void basic_cosimulation_c_1_sm_master_terminate(void)
{
  /* Terminate for S-Function (sfun_recv_async): '<S5>/S-Function1' */
  /* Level2 S-Function Block: '<S5>/S-Function1' (sfun_recv_async) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[0];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (RECV_Param): '<S17>/S-Function' */
  /* Level2 S-Function Block: '<S17>/S-Function' (RECV_Param) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[1];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (rte_svpwm): '<S91>/RTE SPWM1' */
  /* Level2 S-Function Block: '<S91>/RTE SPWM1' (rte_svpwm) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[2];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (rte_conversion): '<S91>/RTE Conversion1' */
  /* Level2 S-Function Block: '<S91>/RTE Conversion1' (rte_conversion) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[3];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (ssn_sfun_v1): '<S174>/State-Space' */
  /* Level2 S-Function Block: '<S174>/State-Space' (ssn_sfun_v1) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[4];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (OP_SEND): '<S172>/S-Function' */
  /* Level2 S-Function Block: '<S172>/S-Function' (OP_SEND) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[5];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (opwritefile): '<S8>/OpWriteFile26' */
  /* Level2 S-Function Block: '<S8>/OpWriteFile26' (opwritefile) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[6];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (opwritefile): '<S8>/OpWriteFile27' */
  /* Level2 S-Function Block: '<S8>/OpWriteFile27' (opwritefile) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[7];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (opwritefile): '<S8>/OpWriteFile28' */
  /* Level2 S-Function Block: '<S8>/OpWriteFile28' (opwritefile) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[8];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (opwritefile): '<S8>/OpWriteFile29' */
  /* Level2 S-Function Block: '<S8>/OpWriteFile29' (opwritefile) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[9];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (opwritefile): '<S8>/OpWriteFile30' */
  /* Level2 S-Function Block: '<S8>/OpWriteFile30' (opwritefile) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[10];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (rte_svpwm): '<S28>/RTE SPWM1' */
  /* Level2 S-Function Block: '<S28>/RTE SPWM1' (rte_svpwm) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[11];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (rte_conversion): '<S28>/RTE Conversion1' */
  /* Level2 S-Function Block: '<S28>/RTE Conversion1' (rte_conversion) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[12];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (ssn_sfun_v1): '<S173>/State-Space' */
  /* Level2 S-Function Block: '<S173>/State-Space' (ssn_sfun_v1) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[13];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (sfun_send_async): '<S15>/S-Function2' */
  /* Level2 S-Function Block: '<S15>/S-Function2' (sfun_send_async) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[14];
    sfcnTerminate(rts);
  }

  /* Terminate for S-Function (sfun_gen_async_ctrl): '<S3>/OpIPSocketCtrl1' */
  /* Level2 S-Function Block: '<S3>/OpIPSocketCtrl1' (sfun_gen_async_ctrl) */
  {
    SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[15];
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
  basic_cosimulation_c_1_sm_master_P.Integrator2_UpperSat = rtInf;
  basic_cosimulation_c_1_sm_master_P.Integrator2_LowerSat = rtMinusInf;
  basic_cosimulation_c_1_sm_master_P.RTESPWM1_P3_h = rtInf;
  basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_j = rtInf;
  basic_cosimulation_c_1_sm_master_P.Saturation_UpperSat_c = rtInf;

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
      basic_cosimulation_c_1_sm_master_B.Delay4[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Delay5[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Delay7[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_B.StateSpace_o2_c[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.uRon[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Switch_g[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Saturation_h[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Integrator_c[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_B.Gain_o[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_B.Integrator_b[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_B.Gain_d[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_B.Product1_e[i] = 0.0;
    }

    for (i = 0; i < 8; i++) {
      basic_cosimulation_c_1_sm_master_B.Product1_k[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_B.Product1_l[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay4Inpo[i] =
        0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay7Inpo[i] =
        0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.uRon_n[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Switch_l[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Saturation_b[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.DiscreteTimeIntegrator[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.u9Tf[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.Add[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.Saturation1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.Add1[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.u1Tt[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.Saturation2[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.Add2[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.UnitDelay[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.Switch[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_B.Tail_f.Product[i] = 0.0;
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

    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseShift.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseShift.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.AnalyticSignal_p[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4_e.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4_e.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseShift_i.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.PhaseShift_i.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.DynamicPhasor_p[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_i[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_m[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_l[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_o[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_mo.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_ax.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_f.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_a3.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_oi.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.RealImagtoComplex_iz.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MultiportSwitch_h.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_l.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_l.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_m.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_m.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_f.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_f.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_a.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_a.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_g.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_g.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_o.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_o.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_k.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_k.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_p.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_g.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_g.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_g.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_m.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_c.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_c.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_m.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_m.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_l.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_l.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b0.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b0.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2.re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2.im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_oq[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_oq[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_oq[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_oq[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_oq[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_oq[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_oq[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_oq[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_a[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction2_i[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_f[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_bv[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_b[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_b[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_b[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_b[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_b[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_b[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_b[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_b[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_c[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_mb[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_b[3].im = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction1_b[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha_f[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.alpha2_pr[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_go[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_go[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_go[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_go[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_go[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_go[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_go[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_go[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_mb[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_c[3].im = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Add_p4[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p4[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p4[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p4[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p4[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p4[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p4[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Add_p4[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ho[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.MathFunction_m[3].im = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Gain1_az[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_az[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_d[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_d[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_d[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_d[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_d[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_d[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_d[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_d[3].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[0].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[0].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[1].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[1].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[2].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[2].im = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[3].re = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_p[3].im = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Sum1 = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.SFunction_l[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.SFunction_l[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.SFunction_l[2] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.DegRad[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DegRad[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DegRad[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Bias = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.RoundingFunction = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ku = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Initial = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o1[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag4_o2[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.EMTThreephaseSignal[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_j[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_j[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_j[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Bias_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.RoundingFunction_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Initial_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o1[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoMagnitudeAngle_o2[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RadDeg[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RadDeg[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.RadDeg[2] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.OpWriteFile26_o1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpWriteFile26_o2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpWriteFile27_o1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpWriteFile27_o2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay6[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay6[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay6[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpWriteFile28_o1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpWriteFile28_o2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpWriteFile29_o1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpWriteFile29_o2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay8[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay8[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpWriteFile30_o1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.OpWriteFile30_o2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Constant11_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_b[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_b[1] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Gain1_o = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_f[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_f[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_i[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_i[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator = 0.0;
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_lp = 0.0;
    basic_cosimulation_c_1_sm_master_B.Switch_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_hn = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_gp = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain6_h5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_et = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide1_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInpor[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_mo = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos4_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos5_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_g1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_gm = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ft = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayInp_h[1] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Product2_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ka = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayIn_h0[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_i[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_i[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_i[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_i[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1[3] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2[3] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Sum_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain7_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.W = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_ge = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_c5 = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_nq = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2Inpo[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_he = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay6Inpo[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_lv = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay2_f[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay2_f[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay2_f[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_b0[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_b0[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.DiscreteTimeIntegrator1_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_i4[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Delay_i4[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ow = 0.0;
    basic_cosimulation_c_1_sm_master_B.Switch_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract_ce = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain5_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain6_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_kn = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide1_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Saturation1_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_et = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Divide1_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelayI_h0n[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_pk = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_il = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_c = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos4_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos5_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_oy = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_jw = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_f2 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay_h0n4[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_mi = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_lp = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos4_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum5_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos5_a = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_n0 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product4_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product5_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_n = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_b1 = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDela_h0n42[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Bias_g = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_el = 0.0;
    basic_cosimulation_c_1_sm_master_B.RoundingFunction_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_nl = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Initial_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_g3 = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o1_d = 0.0;
    basic_cosimulation_c_1_sm_master_B.TrigonometricFunction1_o2_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_hb[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_hb[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_c3[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_c3[1] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Subtract1_pp[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Subtract1_pp[1] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Gain_ab[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_ab[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_l[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_l[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_ew1[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_ew1[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_b5[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_b5[1] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Gain_db[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_db[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_c[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_c[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_lz[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_lz[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_mb[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_mb[1] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.Gain_oy[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_oy[1] = 0.0;
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
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o1_m = 0.0;
    basic_cosimulation_c_1_sm_master_B.ComplextoRealImag_o2_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Conversion = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_jf[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_jf[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain_jf[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain5_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain6_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum1_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain4_k = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos2_h = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain2_b3 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_j = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum_l = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain1_ew = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos3_f = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos_lc = 0.0;
    basic_cosimulation_c_1_sm_master_B.Cos1_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product_le = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product1_cs = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum3_dz = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain7_i = 0.0;
    basic_cosimulation_c_1_sm_master_B.Integrator_dd = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product2_pp = 0.0;
    basic_cosimulation_c_1_sm_master_B.Product3_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum2_b4 = 0.0;
    basic_cosimulation_c_1_sm_master_B.Sum4_ng = 0.0;
    basic_cosimulation_c_1_sm_master_B.W_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_o = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_cr = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_b = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[0] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[1] = 0.0;
    basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay2In_p[2] = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_gz = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_e = 0.0;
    basic_cosimulation_c_1_sm_master_B.Gain3_p = 0.0;
    basic_cosimulation_c_1_sm_master_B.donotdeletethisgain_j = 0.0;
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
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE = 0.0;

  {
    int32_T i;
    for (i = 0; i < 14; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 16; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_l[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_j[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_i[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 18; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_kr[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 18; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_e[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.MOV_segment9_f0 = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_a = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_b = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE[2] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_f = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay1_DSTATE_a = 0.0;

  {
    int32_T i;
    for (i = 0; i < 30; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay4_DSTATE[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 30; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay5_DSTATE[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 15; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay6_DSTATE[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 30; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay7_DSTATE[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 10; i++) {
      basic_cosimulation_c_1_sm_master_DW.Delay8_DSTATE[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_c0[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_ea[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_i0[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 15; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_jx[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 15; i++) {
      basic_cosimulation_c_1_sm_master_DW.MOV_segment9_d[i] = 0.0;
    }
  }

  basic_cosimulation_c_1_sm_master_DW.MOV_segment9_lz = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_o[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE = 0.0;
  basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTATE = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay2_DSTATE_p[2] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c3[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_c3[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator_DSTATE_i = 0.0;
  basic_cosimulation_c_1_sm_master_DW.DiscreteTimeIntegrator1_DSTAT_l = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_p[1] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_f = 0.0;
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

  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[0] = 0.0;
  basic_cosimulation_c_1_sm_master_DW.Integrator_DSTATE_fa[1] = 0.0;
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
      basic_cosimulation_c_1_sm_master_DW.Delay_DSTATE_ol[i] = 0.0;
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
      basic_cosimulation_c_1_sm_master_DW.Tail_f.DiscreteTimeIntegrator_DSTATE[i]
        = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 6; i++) {
      basic_cosimulation_c_1_sm_master_DW.Tail_f.UnitDelay_DSTATE[i] = 0.0;
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

  basic_cosimulation_c_1_sm_master_M->Sizes.numSFcns = (16);

  /* register each child */
  {
    (void) memset((void *)
                  &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.childSFunctions
                  [0], 0,
                  16*sizeof(SimStruct));
    basic_cosimulation_c_1_sm_master_M->childSfunctions =
      (&basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.childSFunctionPtrs[0]);

    {
      int_T i;
      for (i = 0; i < 16; i++) {
        basic_cosimulation_c_1_sm_master_M->childSfunctions[i] =
          (&basic_cosimulation_c_1_sm_master_M->
           NonInlinedSFcns.childSFunctions[i]);
      }
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S5>/S-Function1 (sfun_recv_async) */
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S17>/S-Function (RECV_Param) */
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
            basic_cosimulation_c_1_sm_master_B.SFunction_l));
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S91>/RTE SPWM1 (rte_svpwm) */
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
                "basic_cosimulation_c_1_sm_master/SM_Master/VSC_ITM_SFA/PWM/RTE SPWM1");
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S91>/RTE Conversion1 (rte_conversion) */
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
                "basic_cosimulation_c_1_sm_master/SM_Master/VSC_ITM_SFA/PWM/RTE Conversion1");
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S174>/State-Space (ssn_sfun_v1) */
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
            const real_T *u0 =
              &basic_cosimulation_c_1_sm_master_B.Tail_f.Product[0];
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
                "basic_cosimulation_c_1_sm_master/powergui/EquivalentModel2/State-Space");
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
        ssSetDWork(rts, 1, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_l[0]);

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
        ssSetDWork(rts, 3, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_j[0]);

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
        ssSetDWork(rts, 6, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_i[0]);

        /* yout_g0 */
        ssSetDWorkWidth(rts, 7, 18);
        ssSetDWorkDataType(rts, 7,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 7, 0);
        ssSetDWorkUsedAsDState(rts, 7, 1);
        ssSetDWork(rts, 7, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_kr
                   [0]);

        /* youtold_g0 */
        ssSetDWorkWidth(rts, 8, 18);
        ssSetDWorkDataType(rts, 8,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 8, 0);
        ssSetDWorkUsedAsDState(rts, 8, 1);
        ssSetDWork(rts, 8, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_e[0]);

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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S172>/S-Function (OP_SEND) */
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
            sfcnUPtrs[9] = &basic_cosimulation_c_1_sm_master_B.RadDeg[0];
            sfcnUPtrs[10] = &basic_cosimulation_c_1_sm_master_B.RadDeg[1];
            sfcnUPtrs[11] = &basic_cosimulation_c_1_sm_master_B.RadDeg[2];
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
                       basic_cosimulation_c_1_sm_master_P.SFunction_P1_Size_g);
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S8>/OpWriteFile26 (opwritefile) */
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
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.UPtrs0;

          {
            int_T i1;
            const real_T *u0 = basic_cosimulation_c_1_sm_master_B.Delay4;
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1] = &u0[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 6);
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
          ssSetOutputPortWidth(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile26_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile26_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "OpWriteFile26");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/Saving RT Data1/OpWriteFile26");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn6.params;
        ssSetSFcnParamsCount(rts, 12);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile26_P12_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.OpWriteFile26_PWORK);

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
        ssSetDWork(rts, 0,
                   &basic_cosimulation_c_1_sm_master_DW.OpWriteFile26_PWORK);
      }

      /* registration */
      opwritefile(rts);
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
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 0, 0);
      _ssSetOutputPortConnected(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S8>/OpWriteFile27 (opwritefile) */
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

          {
            int_T i1;
            const real_T *u0 = basic_cosimulation_c_1_sm_master_B.Delay5;
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1] = &u0[i1];
            }
          }

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
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile27_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile27_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "OpWriteFile27");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/Saving RT Data1/OpWriteFile27");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.params;
        ssSetSFcnParamsCount(rts, 12);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile27_P12_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.OpWriteFile27_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn7.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0,
                   &basic_cosimulation_c_1_sm_master_DW.OpWriteFile27_PWORK);
      }

      /* registration */
      opwritefile(rts);
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
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 0, 0);
      _ssSetOutputPortConnected(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S8>/OpWriteFile28 (opwritefile) */
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
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.UPtrs0;
          sfcnUPtrs[0] = basic_cosimulation_c_1_sm_master_B.Delay6;
          sfcnUPtrs[1] = &basic_cosimulation_c_1_sm_master_B.Delay6[1];
          sfcnUPtrs[2] = &basic_cosimulation_c_1_sm_master_B.Delay6[2];
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 3);
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
          ssSetOutputPortWidth(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile28_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile28_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "OpWriteFile28");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/Saving RT Data1/OpWriteFile28");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.params;
        ssSetSFcnParamsCount(rts, 12);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile28_P12_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.OpWriteFile28_PWORK);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn8.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 1);

        /* PWORK */
        ssSetDWorkWidth(rts, 0, 1);
        ssSetDWorkDataType(rts, 0,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWork(rts, 0,
                   &basic_cosimulation_c_1_sm_master_DW.OpWriteFile28_PWORK);
      }

      /* registration */
      opwritefile(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetInputPortWidth(rts, 0, 3);
      ssSetInputPortDataType(rts, 0, SS_DOUBLE);
      ssSetInputPortComplexSignal(rts, 0, 0);
      ssSetInputPortFrameData(rts, 0, 0);
      ssSetInputPortUnit(rts, 0, 0);
      ssSetInputPortIsContinuousQuantity(rts, 0, 0);
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 0, 0);
      _ssSetOutputPortConnected(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S8>/OpWriteFile29 (opwritefile) */
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
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.UPtrs0;

          {
            int_T i1;
            const real_T *u0 = basic_cosimulation_c_1_sm_master_B.Delay7;
            for (i1=0; i1 < 6; i1++) {
              sfcnUPtrs[i1] = &u0[i1];
            }
          }

          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 6);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile29_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile29_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "OpWriteFile29");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/Saving RT Data1/OpWriteFile29");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn9.params;
        ssSetSFcnParamsCount(rts, 12);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile29_P12_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.OpWriteFile29_PWORK);

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
        ssSetDWork(rts, 0,
                   &basic_cosimulation_c_1_sm_master_DW.OpWriteFile29_PWORK);
      }

      /* registration */
      opwritefile(rts);
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
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 0, 0);
      _ssSetOutputPortConnected(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S8>/OpWriteFile30 (opwritefile) */
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

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.UPtrs0;
          sfcnUPtrs[0] = basic_cosimulation_c_1_sm_master_B.Delay8;
          sfcnUPtrs[1] = &basic_cosimulation_c_1_sm_master_B.Delay8[1];
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 2);
        }
      }

      /* outputs */
      {
        ssSetPortInfoForOutputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.outputPortCoSimAttribute
          [0]);
        ssSetOutputPortIsContinuousQuantity(rts, 0, 0);
        ssSetOutputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          _ssSetOutputPortNumDimensions(rts, 0, 1);
          ssSetOutputPortWidth(rts, 0, 1);
          ssSetOutputPortSignal(rts, 0, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile30_o1));
        }

        /* port 1 */
        {
          _ssSetOutputPortNumDimensions(rts, 1, 1);
          ssSetOutputPortWidth(rts, 1, 1);
          ssSetOutputPortSignal(rts, 1, ((real_T *)
            &basic_cosimulation_c_1_sm_master_B.OpWriteFile30_o2));
        }
      }

      /* path info */
      ssSetModelName(rts, "OpWriteFile30");
      ssSetPath(rts,
                "basic_cosimulation_c_1_sm_master/SM_Master/Saving RT Data1/OpWriteFile30");
      ssSetRTModel(rts,basic_cosimulation_c_1_sm_master_M);
      ssSetParentSS(rts, (NULL));
      ssSetRootSS(rts, rts);
      ssSetVersion(rts, SIMSTRUCT_VERSION_LEVEL2);

      /* parameters */
      {
        mxArray **sfcnParams = (mxArray **)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn10.params;
        ssSetSFcnParamsCount(rts, 12);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P1_Size);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P2_Size);
        ssSetSFcnParam(rts, 2, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P3_Size);
        ssSetSFcnParam(rts, 3, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P4_Size);
        ssSetSFcnParam(rts, 4, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P5_Size);
        ssSetSFcnParam(rts, 5, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P6_Size);
        ssSetSFcnParam(rts, 6, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P7_Size);
        ssSetSFcnParam(rts, 7, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P8_Size);
        ssSetSFcnParam(rts, 8, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P9_Size);
        ssSetSFcnParam(rts, 9, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P10_Size);
        ssSetSFcnParam(rts, 10, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P11_Size);
        ssSetSFcnParam(rts, 11, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.OpWriteFile30_P12_Size);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.OpWriteFile30_PWORK);

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
                   &basic_cosimulation_c_1_sm_master_DW.OpWriteFile30_PWORK);
      }

      /* registration */
      opwritefile(rts);
      sfcnInitializeSizes(rts);
      sfcnInitializeSampleTimes(rts);

      /* adjust sample time */
      ssSetSampleTime(rts, 0, 5.0E-5);
      ssSetOffsetTime(rts, 0, 0.0);
      sfcnTsMap[0] = 1;

      /* set compiled values of dynamic vector attributes */
      ssSetInputPortWidth(rts, 0, 2);
      ssSetInputPortDataType(rts, 0, SS_DOUBLE);
      ssSetInputPortComplexSignal(rts, 0, 0);
      ssSetInputPortFrameData(rts, 0, 0);
      ssSetInputPortUnit(rts, 0, 0);
      ssSetInputPortIsContinuousQuantity(rts, 0, 0);
      ssSetNumNonsampledZCs(rts, 0);

      /* Update connectivity flags for each port */
      _ssSetInputPortConnected(rts, 0, 1);
      _ssSetOutputPortConnected(rts, 0, 0);
      _ssSetOutputPortConnected(rts, 1, 0);
      _ssSetOutputPortBeingMerged(rts, 0, 0);
      _ssSetOutputPortBeingMerged(rts, 1, 0);

      /* Update the BufferDstPort flags for each input port */
      ssSetInputPortBufferDstPort(rts, 0, -1);
    }

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S28>/RTE SPWM1 (rte_svpwm) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[11];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.sfcnTsMap;
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
                         [11]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [11]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [11]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [11]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [11]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [11]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [11]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.inputPortCoSimAttribute
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.outputPortCoSimAttribute
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.params;
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn11.dWorkAux;
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S28>/RTE Conversion1 (rte_conversion) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[12];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.sfcnTsMap;
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
                         [12]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [12]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [12]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [12]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [12]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [12]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [12]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 1);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.UPtrs0;
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 1);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.outputPortCoSimAttribute
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.params;
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn12.dWorkAux;
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S173>/State-Space (ssn_sfun_v1) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[13];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.sfcnTsMap;
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
                         [13]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [13]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [13]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [13]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [13]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [13]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [13]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.UPtrs0;

          {
            int_T i1;
            const real_T *u0 = &basic_cosimulation_c_1_sm_master_B.Tail.Product
              [0];
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
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.UPtrs1;

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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 2);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        ssSetOutputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.outputPortCoSimAttribute
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
            basic_cosimulation_c_1_sm_master_B.StateSpace_o2_c));
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.params;
        ssSetSFcnParamsCount(rts, 2);
        ssSetSFcnParamsPtr(rts, &sfcnParams[0]);
        ssSetSFcnParam(rts, 0, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.StateSpace_P1_Size_i);
        ssSetSFcnParam(rts, 1, (mxArray*)
                       basic_cosimulation_c_1_sm_master_P.StateSpace_P2_Size_f);
      }

      /* work vectors */
      ssSetPWork(rts, (void **)
                 &basic_cosimulation_c_1_sm_master_DW.StateSpace_PWORK_i);

      {
        struct _ssDWorkRecord *dWorkRecord = (struct _ssDWorkRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn13.dWorkAux;
        ssSetSFcnDWork(rts, dWorkRecord);
        ssSetSFcnDWorkAux(rts, dWorkAuxRecord);
        _ssSetNumDWork(rts, 11);

        /* xn_g0 */
        ssSetDWorkWidth(rts, 0, 14);
        ssSetDWorkDataType(rts, 0,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 0, 0);
        ssSetDWorkUsedAsDState(rts, 0, 1);
        ssSetDWork(rts, 0, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_c0
                   [0]);

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
        ssSetDWork(rts, 4, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_ea
                   [0]);

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
        ssSetDWork(rts, 6, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_i0
                   [0]);

        /* yout_g0 */
        ssSetDWorkWidth(rts, 7, 15);
        ssSetDWorkDataType(rts, 7,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 7, 0);
        ssSetDWorkUsedAsDState(rts, 7, 1);
        ssSetDWork(rts, 7, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_jx
                   [0]);

        /* youtold_g0 */
        ssSetDWorkWidth(rts, 8, 15);
        ssSetDWorkDataType(rts, 8,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 8, 0);
        ssSetDWorkUsedAsDState(rts, 8, 1);
        ssSetDWork(rts, 8, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_d[0]);

        /* MOV_segment0 */
        ssSetDWorkWidth(rts, 9, 1);
        ssSetDWorkDataType(rts, 9,SS_DOUBLE);
        ssSetDWorkComplexSignal(rts, 9, 0);
        ssSetDWorkUsedAsDState(rts, 9, 1);
        ssSetDWork(rts, 9, &basic_cosimulation_c_1_sm_master_DW.MOV_segment9_lz);

        /* PWORK */
        ssSetDWorkWidth(rts, 10, 1);
        ssSetDWorkDataType(rts, 10,SS_POINTER);
        ssSetDWorkComplexSignal(rts, 10, 0);
        ssSetDWork(rts, 10,
                   &basic_cosimulation_c_1_sm_master_DW.StateSpace_PWORK_i);
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

    /* Level2 S-Function Block: basic_cosimulation_c_1_sm_master/<S15>/S-Function2 (sfun_send_async) */
    {
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[14];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.sfcnTsMap;
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
                         [14]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [14]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [14]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [14]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [14]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [14]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [14]);
      }

      /* inputs */
      {
        _ssSetNumInputPorts(rts, 2);
        ssSetPortInfoForInputs(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.inputPortInfo
          [0]);
        _ssSetPortInfo2ForInputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.inputPortUnits
          [0]);
        ssSetInputPortUnit(rts, 0, 0);
        ssSetInputPortUnit(rts, 1, 0);
        _ssSetPortInfo2ForInputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.inputPortCoSimAttribute
          [0]);
        ssSetInputPortIsContinuousQuantity(rts, 0, 0);
        ssSetInputPortIsContinuousQuantity(rts, 1, 0);

        /* port 0 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.UPtrs0;
          sfcnUPtrs[0] = &basic_cosimulation_c_1_sm_master_B.dataready25kHz;
          ssSetInputPortSignalPtrs(rts, 0, (InputPtrsType)&sfcnUPtrs[0]);
          _ssSetInputPortNumDimensions(rts, 0, 1);
          ssSetInputPortWidth(rts, 0, 1);
        }

        /* port 1 */
        {
          real_T const **sfcnUPtrs = (real_T const **)
            &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.UPtrs1;

          {
            int_T i1;
            const real_T *u1 =
              basic_cosimulation_c_1_sm_master_B.TmpSignalConversionAtDelay4Inpo;
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.outputPortInfo
          [0]);
        _ssSetNumOutputPorts(rts, 1);
        _ssSetPortInfo2ForOutputUnits(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.outputPortUnits
          [0]);
        ssSetOutputPortUnit(rts, 0, 0);
        _ssSetPortInfo2ForOutputCoSimAttribute(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.outputPortCoSimAttribute
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.params;
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn14.dWorkAux;
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
      SimStruct *rts = basic_cosimulation_c_1_sm_master_M->childSfunctions[15];

      /* timing info */
      time_T *sfcnPeriod =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn15.sfcnPeriod;
      time_T *sfcnOffset =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn15.sfcnOffset;
      int_T *sfcnTsMap =
        basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn15.sfcnTsMap;
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
                         [15]);
      }

      _ssSetBlkInfo2PortInfo2Ptr(rts,
        &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.inputOutputPortInfo2
        [15]);

      /* Set up the mdlInfo pointer */
      ssSetRTWSfcnInfo(rts, basic_cosimulation_c_1_sm_master_M->sfcnInfo);

      /* Allocate memory of model methods 2 */
      {
        ssSetModelMethods2(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods2
                           [15]);
      }

      /* Allocate memory of model methods 3 */
      {
        ssSetModelMethods3(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods3
                           [15]);
      }

      /* Allocate memory of model methods 4 */
      {
        ssSetModelMethods4(rts,
                           &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.methods4
                           [15]);
      }

      /* Allocate memory for states auxilliary information */
      {
        ssSetStatesInfo2(rts,
                         &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.statesInfo2
                         [15]);
        ssSetPeriodicStatesInfo(rts,
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.periodicStatesInfo
          [15]);
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn15.params;
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
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn15.dWork;
        struct _ssDWorkAuxRecord *dWorkAuxRecord = (struct _ssDWorkAuxRecord *)
          &basic_cosimulation_c_1_sm_master_M->NonInlinedSFcns.Sfcn15.dWorkAux;
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
  basic_cosimulation_c_1_sm_master_M->Sizes.numBlocks = (839);/* Number of blocks */
  basic_cosimulation_c_1_sm_master_M->Sizes.numBlockIO = (646);/* Number of block outputs */
  basic_cosimulation_c_1_sm_master_M->Sizes.numBlockPrms = (1004);/* Sum of parameter "widths" */
  return basic_cosimulation_c_1_sm_master_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
