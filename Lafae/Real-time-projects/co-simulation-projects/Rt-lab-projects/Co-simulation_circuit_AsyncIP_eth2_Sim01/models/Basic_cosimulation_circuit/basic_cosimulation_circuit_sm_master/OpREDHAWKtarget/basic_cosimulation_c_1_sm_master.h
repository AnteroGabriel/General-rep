/*
 * basic_cosimulation_c_1_sm_master.h
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

#ifndef RTW_HEADER_basic_cosimulation_c_1_sm_master_h_
#define RTW_HEADER_basic_cosimulation_c_1_sm_master_h_
#include <stddef.h>
#include <string.h>
#include <float.h>
#include <math.h>
#ifndef basic_cosimulation_c_1_sm_master_COMMON_INCLUDES_
#define basic_cosimulation_c_1_sm_master_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_logging.h"
#endif                   /* basic_cosimulation_c_1_sm_master_COMMON_INCLUDES_ */

#include "basic_cosimulation_c_1_sm_master_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rtGetNaN.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetBlockIO
#define rtmGetBlockIO(rtm)             ((rtm)->ModelData.blockIO)
#endif

#ifndef rtmSetBlockIO
#define rtmSetBlockIO(rtm, val)        ((rtm)->ModelData.blockIO = (val))
#endif

#ifndef rtmGetChecksums
#define rtmGetChecksums(rtm)           ((rtm)->Sizes.checksums)
#endif

#ifndef rtmSetChecksums
#define rtmSetChecksums(rtm, val)      ((rtm)->Sizes.checksums = (val))
#endif

#ifndef rtmGetConstBlockIO
#define rtmGetConstBlockIO(rtm)        ((rtm)->ModelData.constBlockIO)
#endif

#ifndef rtmSetConstBlockIO
#define rtmSetConstBlockIO(rtm, val)   ((rtm)->ModelData.constBlockIO = (val))
#endif

#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->ModelData.contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->ModelData.contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->ModelData.contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->ModelData.contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->ModelData.CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->ModelData.CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetCtrlRateMdlRefTiming
#define rtmGetCtrlRateMdlRefTiming(rtm) ()
#endif

#ifndef rtmSetCtrlRateMdlRefTiming
#define rtmSetCtrlRateMdlRefTiming(rtm, val) ()
#endif

#ifndef rtmGetCtrlRateMdlRefTimingPtr
#define rtmGetCtrlRateMdlRefTimingPtr(rtm) ()
#endif

#ifndef rtmSetCtrlRateMdlRefTimingPtr
#define rtmSetCtrlRateMdlRefTimingPtr(rtm, val) ()
#endif

#ifndef rtmGetCtrlRateNumTicksToNextHit
#define rtmGetCtrlRateNumTicksToNextHit(rtm) ()
#endif

#ifndef rtmSetCtrlRateNumTicksToNextHit
#define rtmSetCtrlRateNumTicksToNextHit(rtm, val) ()
#endif

#ifndef rtmGetDataMapInfo
#define rtmGetDataMapInfo(rtm)         ()
#endif

#ifndef rtmSetDataMapInfo
#define rtmSetDataMapInfo(rtm, val)    ()
#endif

#ifndef rtmGetDefaultParam
#define rtmGetDefaultParam(rtm)        ((rtm)->ModelData.defaultParam)
#endif

#ifndef rtmSetDefaultParam
#define rtmSetDefaultParam(rtm, val)   ((rtm)->ModelData.defaultParam = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->ModelData.derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->ModelData.derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetDirectFeedThrough
#define rtmGetDirectFeedThrough(rtm)   ((rtm)->Sizes.sysDirFeedThru)
#endif

#ifndef rtmSetDirectFeedThrough
#define rtmSetDirectFeedThrough(rtm, val) ((rtm)->Sizes.sysDirFeedThru = (val))
#endif

#ifndef rtmGetErrorStatusFlag
#define rtmGetErrorStatusFlag(rtm)     ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatusFlag
#define rtmSetErrorStatusFlag(rtm, val) ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmSetFinalTime
#define rtmSetFinalTime(rtm, val)      ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmGetFirstInitCondFlag
#define rtmGetFirstInitCondFlag(rtm)   ((rtm)->Timing.firstInitCondFlag)
#endif

#ifndef rtmSetFirstInitCondFlag
#define rtmSetFirstInitCondFlag(rtm, val) ((rtm)->Timing.firstInitCondFlag = (val))
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ((rtm)->ModelData.intgData)
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ((rtm)->ModelData.intgData = (val))
#endif

#ifndef rtmGetMdlRefGlobalRuntimeEventIndices
#define rtmGetMdlRefGlobalRuntimeEventIndices(rtm) ()
#endif

#ifndef rtmSetMdlRefGlobalRuntimeEventIndices
#define rtmSetMdlRefGlobalRuntimeEventIndices(rtm, val) ()
#endif

#ifndef rtmGetMdlRefGlobalTID
#define rtmGetMdlRefGlobalTID(rtm)     ()
#endif

#ifndef rtmSetMdlRefGlobalTID
#define rtmSetMdlRefGlobalTID(rtm, val) ()
#endif

#ifndef rtmGetMdlRefTriggerTID
#define rtmGetMdlRefTriggerTID(rtm)    ()
#endif

#ifndef rtmSetMdlRefTriggerTID
#define rtmSetMdlRefTriggerTID(rtm, val) ()
#endif

#ifndef rtmGetModelMappingInfo
#define rtmGetModelMappingInfo(rtm)    ((rtm)->SpecialInfo.mappingInfo)
#endif

#ifndef rtmSetModelMappingInfo
#define rtmSetModelMappingInfo(rtm, val) ((rtm)->SpecialInfo.mappingInfo = (val))
#endif

#ifndef rtmGetModelName
#define rtmGetModelName(rtm)           ((rtm)->modelName)
#endif

#ifndef rtmSetModelName
#define rtmSetModelName(rtm, val)      ((rtm)->modelName = (val))
#endif

#ifndef rtmGetNonInlinedSFcns
#define rtmGetNonInlinedSFcns(rtm)     ((rtm)->NonInlinedSFcns)
#endif

#ifndef rtmSetNonInlinedSFcns
#define rtmSetNonInlinedSFcns(rtm, val) ((rtm)->NonInlinedSFcns = (val))
#endif

#ifndef rtmGetNumBlockIO
#define rtmGetNumBlockIO(rtm)          ((rtm)->Sizes.numBlockIO)
#endif

#ifndef rtmSetNumBlockIO
#define rtmSetNumBlockIO(rtm, val)     ((rtm)->Sizes.numBlockIO = (val))
#endif

#ifndef rtmGetNumBlockParams
#define rtmGetNumBlockParams(rtm)      ((rtm)->Sizes.numBlockPrms)
#endif

#ifndef rtmSetNumBlockParams
#define rtmSetNumBlockParams(rtm, val) ((rtm)->Sizes.numBlockPrms = (val))
#endif

#ifndef rtmGetNumBlocks
#define rtmGetNumBlocks(rtm)           ((rtm)->Sizes.numBlocks)
#endif

#ifndef rtmSetNumBlocks
#define rtmSetNumBlocks(rtm, val)      ((rtm)->Sizes.numBlocks = (val))
#endif

#ifndef rtmGetNumContStates
#define rtmGetNumContStates(rtm)       ((rtm)->Sizes.numContStates)
#endif

#ifndef rtmSetNumContStates
#define rtmSetNumContStates(rtm, val)  ((rtm)->Sizes.numContStates = (val))
#endif

#ifndef rtmGetNumDWork
#define rtmGetNumDWork(rtm)            ((rtm)->Sizes.numDwork)
#endif

#ifndef rtmSetNumDWork
#define rtmSetNumDWork(rtm, val)       ((rtm)->Sizes.numDwork = (val))
#endif

#ifndef rtmGetNumInputPorts
#define rtmGetNumInputPorts(rtm)       ((rtm)->Sizes.numIports)
#endif

#ifndef rtmSetNumInputPorts
#define rtmSetNumInputPorts(rtm, val)  ((rtm)->Sizes.numIports = (val))
#endif

#ifndef rtmGetNumNonSampledZCs
#define rtmGetNumNonSampledZCs(rtm)    ((rtm)->Sizes.numNonSampZCs)
#endif

#ifndef rtmSetNumNonSampledZCs
#define rtmSetNumNonSampledZCs(rtm, val) ((rtm)->Sizes.numNonSampZCs = (val))
#endif

#ifndef rtmGetNumOutputPorts
#define rtmGetNumOutputPorts(rtm)      ((rtm)->Sizes.numOports)
#endif

#ifndef rtmSetNumOutputPorts
#define rtmSetNumOutputPorts(rtm, val) ((rtm)->Sizes.numOports = (val))
#endif

#ifndef rtmGetNumPeriodicContStates
#define rtmGetNumPeriodicContStates(rtm) ((rtm)->Sizes.numPeriodicContStates)
#endif

#ifndef rtmSetNumPeriodicContStates
#define rtmSetNumPeriodicContStates(rtm, val) ((rtm)->Sizes.numPeriodicContStates = (val))
#endif

#ifndef rtmGetNumSFcnParams
#define rtmGetNumSFcnParams(rtm)       ((rtm)->Sizes.numSFcnPrms)
#endif

#ifndef rtmSetNumSFcnParams
#define rtmSetNumSFcnParams(rtm, val)  ((rtm)->Sizes.numSFcnPrms = (val))
#endif

#ifndef rtmGetNumSFunctions
#define rtmGetNumSFunctions(rtm)       ((rtm)->Sizes.numSFcns)
#endif

#ifndef rtmSetNumSFunctions
#define rtmSetNumSFunctions(rtm, val)  ((rtm)->Sizes.numSFcns = (val))
#endif

#ifndef rtmGetNumSampleTimes
#define rtmGetNumSampleTimes(rtm)      ((rtm)->Sizes.numSampTimes)
#endif

#ifndef rtmSetNumSampleTimes
#define rtmSetNumSampleTimes(rtm, val) ((rtm)->Sizes.numSampTimes = (val))
#endif

#ifndef rtmGetNumU
#define rtmGetNumU(rtm)                ((rtm)->Sizes.numU)
#endif

#ifndef rtmSetNumU
#define rtmSetNumU(rtm, val)           ((rtm)->Sizes.numU = (val))
#endif

#ifndef rtmGetNumY
#define rtmGetNumY(rtm)                ((rtm)->Sizes.numY)
#endif

#ifndef rtmSetNumY
#define rtmSetNumY(rtm, val)           ((rtm)->Sizes.numY = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ((rtm)->ModelData.odeF)
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ((rtm)->ModelData.odeF = (val))
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ((rtm)->ModelData.odeY)
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ((rtm)->ModelData.odeY = (val))
#endif

#ifndef rtmGetOffsetTimeArray
#define rtmGetOffsetTimeArray(rtm)     ((rtm)->Timing.offsetTimesArray)
#endif

#ifndef rtmSetOffsetTimeArray
#define rtmSetOffsetTimeArray(rtm, val) ((rtm)->Timing.offsetTimesArray = (val))
#endif

#ifndef rtmGetOffsetTimePtr
#define rtmGetOffsetTimePtr(rtm)       ((rtm)->Timing.offsetTimes)
#endif

#ifndef rtmSetOffsetTimePtr
#define rtmSetOffsetTimePtr(rtm, val)  ((rtm)->Timing.offsetTimes = (val))
#endif

#ifndef rtmGetOptions
#define rtmGetOptions(rtm)             ((rtm)->Sizes.options)
#endif

#ifndef rtmSetOptions
#define rtmSetOptions(rtm, val)        ((rtm)->Sizes.options = (val))
#endif

#ifndef rtmGetParamIsMalloced
#define rtmGetParamIsMalloced(rtm)     ()
#endif

#ifndef rtmSetParamIsMalloced
#define rtmSetParamIsMalloced(rtm, val) ()
#endif

#ifndef rtmGetPath
#define rtmGetPath(rtm)                ((rtm)->path)
#endif

#ifndef rtmSetPath
#define rtmSetPath(rtm, val)           ((rtm)->path = (val))
#endif

#ifndef rtmGetPerTaskSampleHits
#define rtmGetPerTaskSampleHits(rtm)   ()
#endif

#ifndef rtmSetPerTaskSampleHits
#define rtmSetPerTaskSampleHits(rtm, val) ()
#endif

#ifndef rtmGetPerTaskSampleHitsArray
#define rtmGetPerTaskSampleHitsArray(rtm) ((rtm)->Timing.perTaskSampleHitsArray)
#endif

#ifndef rtmSetPerTaskSampleHitsArray
#define rtmSetPerTaskSampleHitsArray(rtm, val) ((rtm)->Timing.perTaskSampleHitsArray = (val))
#endif

#ifndef rtmGetPerTaskSampleHitsPtr
#define rtmGetPerTaskSampleHitsPtr(rtm) ((rtm)->Timing.perTaskSampleHits)
#endif

#ifndef rtmSetPerTaskSampleHitsPtr
#define rtmSetPerTaskSampleHitsPtr(rtm, val) ((rtm)->Timing.perTaskSampleHits = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->ModelData.periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->ModelData.periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->ModelData.periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->ModelData.periodicContStateRanges = (val))
#endif

#ifndef rtmGetPrevZCSigState
#define rtmGetPrevZCSigState(rtm)      ((rtm)->ModelData.prevZCSigState)
#endif

#ifndef rtmSetPrevZCSigState
#define rtmSetPrevZCSigState(rtm, val) ((rtm)->ModelData.prevZCSigState = (val))
#endif

#ifndef rtmGetRTWExtModeInfo
#define rtmGetRTWExtModeInfo(rtm)      ((rtm)->extModeInfo)
#endif

#ifndef rtmSetRTWExtModeInfo
#define rtmSetRTWExtModeInfo(rtm, val) ((rtm)->extModeInfo = (val))
#endif

#ifndef rtmGetRTWGeneratedSFcn
#define rtmGetRTWGeneratedSFcn(rtm)    ((rtm)->Sizes.rtwGenSfcn)
#endif

#ifndef rtmSetRTWGeneratedSFcn
#define rtmSetRTWGeneratedSFcn(rtm, val) ((rtm)->Sizes.rtwGenSfcn = (val))
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
#endif

#ifndef rtmSetRTWLogInfo
#define rtmSetRTWLogInfo(rtm, val)     ((rtm)->rtwLogInfo = (val))
#endif

#ifndef rtmGetRTWRTModelMethodsInfo
#define rtmGetRTWRTModelMethodsInfo(rtm) ()
#endif

#ifndef rtmSetRTWRTModelMethodsInfo
#define rtmSetRTWRTModelMethodsInfo(rtm, val) ()
#endif

#ifndef rtmGetRTWSfcnInfo
#define rtmGetRTWSfcnInfo(rtm)         ((rtm)->sfcnInfo)
#endif

#ifndef rtmSetRTWSfcnInfo
#define rtmSetRTWSfcnInfo(rtm, val)    ((rtm)->sfcnInfo = (val))
#endif

#ifndef rtmGetRTWSolverInfo
#define rtmGetRTWSolverInfo(rtm)       ((rtm)->solverInfo)
#endif

#ifndef rtmSetRTWSolverInfo
#define rtmSetRTWSolverInfo(rtm, val)  ((rtm)->solverInfo = (val))
#endif

#ifndef rtmGetRTWSolverInfoPtr
#define rtmGetRTWSolverInfoPtr(rtm)    ((rtm)->solverInfoPtr)
#endif

#ifndef rtmSetRTWSolverInfoPtr
#define rtmSetRTWSolverInfoPtr(rtm, val) ((rtm)->solverInfoPtr = (val))
#endif

#ifndef rtmGetReservedForXPC
#define rtmGetReservedForXPC(rtm)      ((rtm)->SpecialInfo.xpcData)
#endif

#ifndef rtmSetReservedForXPC
#define rtmSetReservedForXPC(rtm, val) ((rtm)->SpecialInfo.xpcData = (val))
#endif

#ifndef rtmGetRootDWork
#define rtmGetRootDWork(rtm)           ((rtm)->ModelData.dwork)
#endif

#ifndef rtmSetRootDWork
#define rtmSetRootDWork(rtm, val)      ((rtm)->ModelData.dwork = (val))
#endif

#ifndef rtmGetSFunctions
#define rtmGetSFunctions(rtm)          ((rtm)->childSfunctions)
#endif

#ifndef rtmSetSFunctions
#define rtmSetSFunctions(rtm, val)     ((rtm)->childSfunctions = (val))
#endif

#ifndef rtmGetSampleHitArray
#define rtmGetSampleHitArray(rtm)      ((rtm)->Timing.sampleHitArray)
#endif

#ifndef rtmSetSampleHitArray
#define rtmSetSampleHitArray(rtm, val) ((rtm)->Timing.sampleHitArray = (val))
#endif

#ifndef rtmGetSampleHitPtr
#define rtmGetSampleHitPtr(rtm)        ((rtm)->Timing.sampleHits)
#endif

#ifndef rtmSetSampleHitPtr
#define rtmSetSampleHitPtr(rtm, val)   ((rtm)->Timing.sampleHits = (val))
#endif

#ifndef rtmGetSampleTimeArray
#define rtmGetSampleTimeArray(rtm)     ((rtm)->Timing.sampleTimesArray)
#endif

#ifndef rtmSetSampleTimeArray
#define rtmSetSampleTimeArray(rtm, val) ((rtm)->Timing.sampleTimesArray = (val))
#endif

#ifndef rtmGetSampleTimePtr
#define rtmGetSampleTimePtr(rtm)       ((rtm)->Timing.sampleTimes)
#endif

#ifndef rtmSetSampleTimePtr
#define rtmSetSampleTimePtr(rtm, val)  ((rtm)->Timing.sampleTimes = (val))
#endif

#ifndef rtmGetSampleTimeTaskIDArray
#define rtmGetSampleTimeTaskIDArray(rtm) ((rtm)->Timing.sampleTimeTaskIDArray)
#endif

#ifndef rtmSetSampleTimeTaskIDArray
#define rtmSetSampleTimeTaskIDArray(rtm, val) ((rtm)->Timing.sampleTimeTaskIDArray = (val))
#endif

#ifndef rtmGetSampleTimeTaskIDPtr
#define rtmGetSampleTimeTaskIDPtr(rtm) ((rtm)->Timing.sampleTimeTaskIDPtr)
#endif

#ifndef rtmSetSampleTimeTaskIDPtr
#define rtmSetSampleTimeTaskIDPtr(rtm, val) ((rtm)->Timing.sampleTimeTaskIDPtr = (val))
#endif

#ifndef rtmGetSelf
#define rtmGetSelf(rtm)                ()
#endif

#ifndef rtmSetSelf
#define rtmSetSelf(rtm, val)           ()
#endif

#ifndef rtmGetSimMode
#define rtmGetSimMode(rtm)             ((rtm)->simMode)
#endif

#ifndef rtmSetSimMode
#define rtmSetSimMode(rtm, val)        ((rtm)->simMode = (val))
#endif

#ifndef rtmGetSimTimeStep
#define rtmGetSimTimeStep(rtm)         ((rtm)->Timing.simTimeStep)
#endif

#ifndef rtmSetSimTimeStep
#define rtmSetSimTimeStep(rtm, val)    ((rtm)->Timing.simTimeStep = (val))
#endif

#ifndef rtmGetStartTime
#define rtmGetStartTime(rtm)           ((rtm)->Timing.tStart)
#endif

#ifndef rtmSetStartTime
#define rtmSetStartTime(rtm, val)      ((rtm)->Timing.tStart = (val))
#endif

#ifndef rtmGetStepSize
#define rtmGetStepSize(rtm)            ((rtm)->Timing.stepSize)
#endif

#ifndef rtmSetStepSize
#define rtmSetStepSize(rtm, val)       ((rtm)->Timing.stepSize = (val))
#endif

#ifndef rtmGetStopRequestedFlag
#define rtmGetStopRequestedFlag(rtm)   ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequestedFlag
#define rtmSetStopRequestedFlag(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetTaskCounters
#define rtmGetTaskCounters(rtm)        ()
#endif

#ifndef rtmSetTaskCounters
#define rtmSetTaskCounters(rtm, val)   ()
#endif

#ifndef rtmGetTaskTimeArray
#define rtmGetTaskTimeArray(rtm)       ((rtm)->Timing.tArray)
#endif

#ifndef rtmSetTaskTimeArray
#define rtmSetTaskTimeArray(rtm, val)  ((rtm)->Timing.tArray = (val))
#endif

#ifndef rtmGetTimePtr
#define rtmGetTimePtr(rtm)             ((rtm)->Timing.t)
#endif

#ifndef rtmSetTimePtr
#define rtmSetTimePtr(rtm, val)        ((rtm)->Timing.t = (val))
#endif

#ifndef rtmGetTimingData
#define rtmGetTimingData(rtm)          ((rtm)->Timing.timingData)
#endif

#ifndef rtmSetTimingData
#define rtmSetTimingData(rtm, val)     ((rtm)->Timing.timingData = (val))
#endif

#ifndef rtmGetU
#define rtmGetU(rtm)                   ((rtm)->ModelData.inputs)
#endif

#ifndef rtmSetU
#define rtmSetU(rtm, val)              ((rtm)->ModelData.inputs = (val))
#endif

#ifndef rtmGetVarNextHitTimesListPtr
#define rtmGetVarNextHitTimesListPtr(rtm) ((rtm)->Timing.varNextHitTimesList)
#endif

#ifndef rtmSetVarNextHitTimesListPtr
#define rtmSetVarNextHitTimesListPtr(rtm, val) ((rtm)->Timing.varNextHitTimesList = (val))
#endif

#ifndef rtmGetY
#define rtmGetY(rtm)                   ((rtm)->ModelData.outputs)
#endif

#ifndef rtmSetY
#define rtmSetY(rtm, val)              ((rtm)->ModelData.outputs = (val))
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->ModelData.zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->ModelData.zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetZCSignalValues
#define rtmGetZCSignalValues(rtm)      ((rtm)->ModelData.zcSignalValues)
#endif

#ifndef rtmSetZCSignalValues
#define rtmSetZCSignalValues(rtm, val) ((rtm)->ModelData.zcSignalValues = (val))
#endif

#ifndef rtmGet_TimeOfLastOutput
#define rtmGet_TimeOfLastOutput(rtm)   ((rtm)->Timing.timeOfLastOutput)
#endif

#ifndef rtmSet_TimeOfLastOutput
#define rtmSet_TimeOfLastOutput(rtm, val) ((rtm)->Timing.timeOfLastOutput = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->ModelData.derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->ModelData.derivs = (val))
#endif

#ifndef rtmGettimingBridge
#define rtmGettimingBridge(rtm)        ()
#endif

#ifndef rtmSettimingBridge
#define rtmSettimingBridge(rtm, val)   ()
#endif

#ifndef rtmGetChecksumVal
#define rtmGetChecksumVal(rtm, idx)    ((rtm)->Sizes.checksums[idx])
#endif

#ifndef rtmSetChecksumVal
#define rtmSetChecksumVal(rtm, idx, val) ((rtm)->Sizes.checksums[idx] = (val))
#endif

#ifndef rtmGetDWork
#define rtmGetDWork(rtm, idx)          ((rtm)->ModelData.dwork[idx])
#endif

#ifndef rtmSetDWork
#define rtmSetDWork(rtm, idx, val)     ((rtm)->ModelData.dwork[idx] = (val))
#endif

#ifndef rtmGetOffsetTime
#define rtmGetOffsetTime(rtm, idx)     ((rtm)->Timing.offsetTimes[idx])
#endif

#ifndef rtmSetOffsetTime
#define rtmSetOffsetTime(rtm, idx, val) ((rtm)->Timing.offsetTimes[idx] = (val))
#endif

#ifndef rtmGetSFunction
#define rtmGetSFunction(rtm, idx)      ((rtm)->childSfunctions[idx])
#endif

#ifndef rtmSetSFunction
#define rtmSetSFunction(rtm, idx, val) ((rtm)->childSfunctions[idx] = (val))
#endif

#ifndef rtmGetSampleTime
#define rtmGetSampleTime(rtm, idx)     ((rtm)->Timing.sampleTimes[idx])
#endif

#ifndef rtmSetSampleTime
#define rtmSetSampleTime(rtm, idx, val) ((rtm)->Timing.sampleTimes[idx] = (val))
#endif

#ifndef rtmGetSampleTimeTaskID
#define rtmGetSampleTimeTaskID(rtm, idx) ((rtm)->Timing.sampleTimeTaskIDPtr[idx])
#endif

#ifndef rtmSetSampleTimeTaskID
#define rtmSetSampleTimeTaskID(rtm, idx, val) ((rtm)->Timing.sampleTimeTaskIDPtr[idx] = (val))
#endif

#ifndef rtmGetVarNextHitTimeList
#define rtmGetVarNextHitTimeList(rtm, idx) ((rtm)->Timing.varNextHitTimesList[idx])
#endif

#ifndef rtmSetVarNextHitTimeList
#define rtmSetVarNextHitTimeList(rtm, idx, val) ((rtm)->Timing.varNextHitTimesList[idx] = (val))
#endif

#ifndef rtmIsContinuousTask
#define rtmIsContinuousTask(rtm, tid)  ((tid) == 0)
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmSetFirstInitCond
#define rtmSetFirstInitCond(rtm, val)  ((rtm)->Timing.firstInitCondFlag = (val))
#endif

#ifndef rtmIsFirstInitCond
#define rtmIsFirstInitCond(rtm)        ((rtm)->Timing.firstInitCondFlag)
#endif

#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmIsSampleHit
#define rtmIsSampleHit(rtm, sti, tid)  ((rtmIsMajorTimeStep((rtm)) && (rtm)->Timing.sampleHits[(rtm)->Timing.sampleTimeTaskIDPtr[sti]]))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmSetT
#define rtmSetT(rtm, val)                                        /* Do Nothing */
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

#ifndef rtmSetTStart
#define rtmSetTStart(rtm, val)         ((rtm)->Timing.tStart = (val))
#endif

#ifndef rtmGetTaskTime
#define rtmGetTaskTime(rtm, sti)       (rtmGetTPtr((rtm))[(rtm)->Timing.sampleTimeTaskIDPtr[sti]])
#endif

#ifndef rtmSetTaskTime
#define rtmSetTaskTime(rtm, sti, val)  (rtmGetTPtr((rtm))[sti] = (val))
#endif

#ifndef rtmGetTimeOfLastOutput
#define rtmGetTimeOfLastOutput(rtm)    ((rtm)->Timing.timeOfLastOutput)
#endif

#ifdef rtmGetRTWSolverInfo
#undef rtmGetRTWSolverInfo
#endif

#define rtmGetRTWSolverInfo(rtm)       &((rtm)->solverInfo)

/* Definition for use in the target main file */
#define basic_cosimulation_c_1_sm_master_rtModel RT_MODEL_basic_cosimulation_c_1_sm_master_T

/* user code (top of header file) */
/* System '<Root>' */
/* Opal-RT Technologies */
extern int opalSizeDwork;
extern unsigned int opalSizeBlockIO;
extern unsigned int opalSizeRTP;
extern void * pRtModel;                //pointer on the RTmodel struc
int_T Opal_rtmGetNumBlockParams(void *ptr);
int_T Opal_rtmGetNumBlockIO(void *ptr);

/* Block signals for system '<S37>/Tail' */
typedef struct {
  real_T DiscreteTimeIntegrator[6];
                              /* '<S38>/Discrete-Time Integrator' (Output 1)  */
  real_T u9Tf[6];                      /* '<S41>/-0.9//Tf' (Output 1)  */
  real_T Add[6];                       /* '<S41>/Add' (Output 1)  */
  real_T Saturation1[6];               /* '<S41>/Saturation1' (Output 1)  */
  real_T Add1[6];                      /* '<S41>/Add1' (Output 1)  */
  real_T u1Tt[6];                      /* '<S41>/0.1//Tt' (Output 1)  */
  real_T Saturation2[6];               /* '<S41>/Saturation2' (Output 1)  */
  real_T Add2[6];                      /* '<S41>/Add2' (Output 1)  */
  real_T UnitDelay[6];                 /* '<S38>/Unit Delay' (Output 1)  */
  real_T Switch[6];                    /* '<S38>/Switch' (Output 1)  */
  real_T Product[6];                   /* '<S38>/Product' (Output 1)  */
} B_Tail_basic_cosimulation_c_1_T;

/* Block states (default storage) for system '<S37>/Tail' */
typedef struct {
  real_T DiscreteTimeIntegrator_DSTATE[6];
                               /* '<S38>/Discrete-Time Integrator' (DWork 1)  */
  real_T UnitDelay_DSTATE[6];          /* '<S38>/Unit Delay' (DWork 1)  */
  int8_T DiscreteTimeIntegrator_PrevRese[6];
                               /* '<S38>/Discrete-Time Integrator' (DWork 2)  */
  boolean_T Tail_MODE;                 /* '<S37>/Tail' (DWork 1)  */
} DW_Tail_basic_cosimulation_c__T;

/* Block signals (default storage) */
typedef struct {
  creal_T MagnitudeAngletoComplex[3];
                            /* '<S27>/Magnitude-Angle to Complex' (Output 1)  */
  creal_T PhaseFrequencyOffset[3];
                              /* '<S27>/Phase// Frequency Offset' (Output 1)  */
  creal_T Gain[3];                     /* '<S30>/Gain' (Output 1)  */
  creal_T Sum1[3];                     /* '<S30>/Sum1' (Output 1)  */
  creal_T PhaseFrequencyOffset_f[3];
                              /* '<S30>/Phase// Frequency Offset' (Output 1)  */
  creal_T RealImagtoComplex;      /* '<S57>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_a;    /* '<S59>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_f;    /* '<S61>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_a3;   /* '<S56>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_o;    /* '<S58>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_i;    /* '<S60>/Real-Imag to Complex' (Output 1)  */
  creal_T MultiportSwitch;            /* '<S53>/Multiport Switch' (Output 1)  */
  creal_T RealImagtoComplex_c[3]; /* '<S27>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_l[4];/* '<S136>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_i0[4];
                                 /* '<S138>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_m[4];/* '<S140>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_ak[4];
                                 /* '<S135>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_la[4];
                                 /* '<S137>/Real-Imag to Complex' (Output 1)  */
  creal_T RealImagtoComplex_ov[4];
                                 /* '<S139>/Real-Imag to Complex' (Output 1)  */
  creal_T MultiportSwitch_b[4];      /* '<S132>/Multiport Switch' (Output 1)  */
  creal_T Add[4];                      /* '<S144>/Add' (Output 1)  */
  creal_T Gain1[4];                    /* '<S144>/Gain1' (Output 1)  */
  creal_T MathFunction2[4];            /* '<S132>/Math Function2' (Output 1)  */
  creal_T Add_f[4];                    /* '<S145>/Add' (Output 1)  */
  creal_T Gain1_b[4];                  /* '<S145>/Gain1' (Output 1)  */
  creal_T Product2[4];                 /* '<S132>/Product2' (Output 1)  */
  creal_T Gain3[4];                    /* '<S132>/Gain3' (Output 1)  */
  creal_T alpha[4];                    /* '<S133>/alpha' (Output 1)  */
  creal_T alpha2[4];                   /* '<S133>/alpha2' (Output 1)  */
  creal_T Add_a[4];                    /* '<S133>/Add' (Output 1)  */
  creal_T Gain1_h[4];                  /* '<S133>/Gain1' (Output 1)  */
  creal_T MathFunction1[4];            /* '<S132>/Math Function1' (Output 1)  */
  creal_T alpha_f[4];                  /* '<S134>/alpha' (Output 1)  */
  creal_T alpha2_p[4];                 /* '<S134>/alpha2' (Output 1)  */
  creal_T Add_g[4];                    /* '<S134>/Add' (Output 1)  */
  creal_T Gain1_m[4];                  /* '<S134>/Gain1' (Output 1)  */
  creal_T Product1[4];                 /* '<S132>/Product1' (Output 1)  */
  creal_T Gain1_md[4];                 /* '<S132>/Gain1' (Output 1)  */
  creal_T alpha2_j[4];                 /* '<S141>/alpha2' (Output 1)  */
  creal_T alpha_h[4];                  /* '<S141>/alpha' (Output 1)  */
  creal_T Add_p[4];                    /* '<S141>/Add' (Output 1)  */
  creal_T Gain1_ho[4];                 /* '<S141>/Gain1' (Output 1)  */
  creal_T MathFunction[4];             /* '<S132>/Math Function' (Output 1)  */
  creal_T alpha2_jy[4];                /* '<S142>/alpha2' (Output 1)  */
  creal_T alpha_i[4];                  /* '<S142>/alpha' (Output 1)  */
  creal_T Add_gb[4];                   /* '<S142>/Add' (Output 1)  */
  creal_T Gain1_a[4];                  /* '<S142>/Gain1' (Output 1)  */
  creal_T Product3[4];                 /* '<S132>/Product3' (Output 1)  */
  creal_T Gain2[4];                    /* '<S132>/Gain2' (Output 1)  */
  creal_T Add_au;                      /* '<S65>/Add' (Output 1)  */
  creal_T Gain1_j;                     /* '<S65>/Gain1' (Output 1)  */
  creal_T MathFunction2_b;             /* '<S53>/Math Function2' (Output 1)  */
  creal_T Add_pe;                      /* '<S66>/Add' (Output 1)  */
  creal_T Gain1_l;                     /* '<S66>/Gain1' (Output 1)  */
  creal_T Product2_h;                  /* '<S53>/Product2' (Output 1)  */
  creal_T Gain3_o;                     /* '<S53>/Gain3' (Output 1)  */
  creal_T alpha_j;                     /* '<S54>/alpha' (Output 1)  */
  creal_T alpha2_o;                    /* '<S54>/alpha2' (Output 1)  */
  creal_T Add_m;                       /* '<S54>/Add' (Output 1)  */
  creal_T Gain1_f;                     /* '<S54>/Gain1' (Output 1)  */
  creal_T MathFunction1_n;             /* '<S53>/Math Function1' (Output 1)  */
  creal_T alpha_a;                     /* '<S55>/alpha' (Output 1)  */
  creal_T alpha2_g;                    /* '<S55>/alpha2' (Output 1)  */
  creal_T Add_o;                       /* '<S55>/Add' (Output 1)  */
  creal_T Gain1_bz;                    /* '<S55>/Gain1' (Output 1)  */
  creal_T Product1_k;                  /* '<S53>/Product1' (Output 1)  */
  creal_T Gain1_k;                     /* '<S53>/Gain1' (Output 1)  */
  creal_T alpha2_py;                   /* '<S62>/alpha2' (Output 1)  */
  creal_T alpha_g;                     /* '<S62>/alpha' (Output 1)  */
  creal_T Add_gd;                      /* '<S62>/Add' (Output 1)  */
  creal_T Gain1_mo;                    /* '<S62>/Gain1' (Output 1)  */
  creal_T MathFunction_n;              /* '<S53>/Math Function' (Output 1)  */
  creal_T alpha2_c;                    /* '<S63>/alpha2' (Output 1)  */
  creal_T alpha_m;                     /* '<S63>/alpha' (Output 1)  */
  creal_T Add_l;                       /* '<S63>/Add' (Output 1)  */
  creal_T Gain1_b0;                    /* '<S63>/Gain1' (Output 1)  */
  creal_T Product3_j;                  /* '<S53>/Product3' (Output 1)  */
  creal_T Gain2_p;                     /* '<S53>/Gain2' (Output 1)  */
  real_T SFunction;                    /* '<S2>/S-Function' (Output 1)  */
  real_T Sum;                          /* '<S2>/Sum' (Output 1)  */
  real_T timeout;                      /* '<S3>/timeout' (Output 1)  */
  real_T SFunction1_o1;                /* '<S6>/S-Function1' (Output 1)  */
  real_T SFunction1_o2;                /* '<S6>/S-Function1' (Output 2)  */
  real_T SFunction1_o3[6];             /* '<S6>/S-Function1' (Output 3)  */
  real_T UnitDelay[6];                 /* '<S37>/Unit Delay' (Output 1)  */
  real_T Constant11;                   /* '<S26>/Constant11' (Output 1)  */
  real_T Delay[2];                     /* '<S45>/Delay' (Output 1)  */
  real_T Subtract;                     /* '<S51>/Subtract' (Output 1)  */
  real_T Integrator1;                  /* '<S51>/Integrator1' (Output 1)  */
  real_T Cos;                          /* '<S52>/Cos' (Output 1)  */
  real_T Product;                      /* '<S52>/Product' (Output 1)  */
  real_T Cos1;                         /* '<S52>/Cos1' (Output 1)  */
  real_T Gain_h;                       /* '<S52>/Gain' (Output 1)  */
  real_T Product1_o;                   /* '<S52>/Product1' (Output 1)  */
  real_T Sum1_j;                       /* '<S52>/Sum1' (Output 1)  */
  real_T Sum6;                         /* '<S52>/Sum6' (Output 1)  */
  real_T Cos2;                         /* '<S52>/Cos2' (Output 1)  */
  real_T Product2_c;                   /* '<S52>/Product2' (Output 1)  */
  real_T Sum2;                         /* '<S52>/Sum2' (Output 1)  */
  real_T Cos3;                         /* '<S52>/Cos3' (Output 1)  */
  real_T Gain1_fm;                     /* '<S52>/Gain1' (Output 1)  */
  real_T Product3_n;                   /* '<S52>/Product3' (Output 1)  */
  real_T Sum3;                         /* '<S52>/Sum3' (Output 1)  */
  real_T Sum7;                         /* '<S52>/Sum7' (Output 1)  */
  real_T Cos4;                         /* '<S52>/Cos4' (Output 1)  */
  real_T Product4;                     /* '<S52>/Product4' (Output 1)  */
  real_T Sum4;                         /* '<S52>/Sum4' (Output 1)  */
  real_T Cos5;                         /* '<S52>/Cos5' (Output 1)  */
  real_T Gain2_h;                      /* '<S52>/Gain2' (Output 1)  */
  real_T Product5;                     /* '<S52>/Product5' (Output 1)  */
  real_T Sum5;                         /* '<S52>/Sum5' (Output 1)  */
  real_T SFunction_i[3];               /* '<S96>/S-Function' (Output 1)  */
  real_T Product2_m[3];                /* '<S17>/Product2' (Output 1)  */
  real_T Saturation[3];                /* '<S17>/Saturation' (Output 1)  */
  real_T Sum16;                        /* '<S26>/Sum16' (Output 1)  */
  real_T Divide7;                      /* '<S26>/Divide7' (Output 1)  */
  real_T Sum17;                        /* '<S26>/Sum17' (Output 1)  */
  real_T Divide8;                      /* '<S26>/Divide8' (Output 1)  */
  real_T Sum18;                        /* '<S26>/Sum18' (Output 1)  */
  real_T Divide9;                      /* '<S26>/Divide9' (Output 1)  */
  real_T TmpSignalConversionAtRTESPWM1In[3];
                            /* '<S26>/TmpSignal ConversionAtRTE SPWM1Inport2' */
  real_T RTESPWM1_o1[3];               /* '<S26>/RTE SPWM1' (Output 1)  */
  real_T RTESPWM1_o2[3];               /* '<S26>/RTE SPWM1' (Output 2)  */
  real_T RTEConversion1[6];            /* '<S26>/RTE Conversion1' (Output 1)  */
  real_T DataTypeConversion[6];   /* '<S40>/Data Type Conversion' (Output 1)  */
  real_T DC;                           /* '<S86>/DC' (Output 1)  */
  real_T Gain_f[3];                    /* '<S27>/Gain' (Output 1)  */
  real_T ComplextoRealImag4_o1[3];
                                 /* '<S27>/Complex to Real-Imag4' (Output 1)  */
  real_T ComplextoRealImag4_o2[3];
                                 /* '<S27>/Complex to Real-Imag4' (Output 2)  */
  real_T Switch[6];                    /* '<S39>/Switch' (Output 1)  */
  real_T StateSpace_o1[18];            /* '<S162>/State-Space' (Output 1)  */
  real_T StateSpace_o2[6];             /* '<S162>/State-Space' (Output 2)  */
  real_T donotdeletethisgain;  /* '<S21>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_g;/* '<S22>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_n;/* '<S23>/do not delete this gain' (Output 1)  */
  real_T TmpSignalConversionAtGainInport[3];
                                 /* '<S30>/TmpSignal ConversionAtGainInport1' */
  real_T ComplextoMagnitudeAngle_o1[3];
                            /* '<S30>/Complex to Magnitude-Angle' (Output 1)  */
  real_T ComplextoMagnitudeAngle_o2[3];
                            /* '<S30>/Complex to Magnitude-Angle' (Output 2)  */
  real_T Gain1_i[3];                   /* '<S30>/Gain1' (Output 1)  */
  real_T Delay_l;                      /* '<S5>/Delay' (Output 1)  */
  real_T Gain6;                        /* '<S46>/Gain6' (Output 1)  */
  real_T Delay1;                       /* '<S5>/Delay1' (Output 1)  */
  real_T Sum4_o;                       /* '<S46>/Sum4' (Output 1)  */
  real_T Gain7;                        /* '<S46>/Gain7' (Output 1)  */
  real_T Sum5_c;                       /* '<S46>/Sum5' (Output 1)  */
  real_T Gain8;                        /* '<S46>/Gain8' (Output 1)  */
  real_T Gain10;                       /* '<S46>/Gain10' (Output 1)  */
  real_T Sum6_b;                       /* '<S46>/Sum6' (Output 1)  */
  real_T Gain9;                        /* '<S46>/Gain9' (Output 1)  */
  real_T Delay2[3];                    /* '<S8>/Delay2' (Output 1)  */
  real_T Delay_c;                      /* '<S8>/Delay' (Output 1)  */
  real_T Gain6_h;                      /* '<S125>/Gain6' (Output 1)  */
  real_T Delay1_i;                     /* '<S8>/Delay1' (Output 1)  */
  real_T Sum4_f;                       /* '<S125>/Sum4' (Output 1)  */
  real_T Gain7_b;                      /* '<S125>/Gain7' (Output 1)  */
  real_T Sum5_b;                       /* '<S125>/Sum5' (Output 1)  */
  real_T Gain8_i;                      /* '<S125>/Gain8' (Output 1)  */
  real_T Gain10_i;                     /* '<S125>/Gain10' (Output 1)  */
  real_T Sum6_f;                       /* '<S125>/Sum6' (Output 1)  */
  real_T Gain9_n;                      /* '<S125>/Gain9' (Output 1)  */
  real_T OpInput;                      /* '<S10>/OpInput' (Output 1)  */
  real_T OpInput_a;                    /* '<S9>/OpInput' (Output 1)  */
  real_T OpInput_p;                    /* '<S12>/OpInput' (Output 1)  */
  real_T OpInput_b;                    /* '<S11>/OpInput' (Output 1)  */
  real_T OpInput_f;                    /* '<S15>/OpInput' (Output 1)  */
  real_T ComplextoRealImag1_o1[3];
                                 /* '<S30>/Complex to Real-Imag1' (Output 1)  */
  real_T ComplextoRealImag1_o2[3];
                                 /* '<S30>/Complex to Real-Imag1' (Output 2)  */
  real_T uRon[6];                      /* '<S37>/1//Ron' (Output 1)  */
  real_T Switch_l[6];                  /* '<S37>/Switch' (Output 1)  */
  real_T Saturation_b[6];              /* '<S37>/Saturation' (Output 1)  */
  real_T donotdeletethisgain_l;/* '<S34>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_m;/* '<S35>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_k;/* '<S36>/do not delete this gain' (Output 1)  */
  real_T Delay2_f[3];                  /* '<S5>/Delay2' (Output 1)  */
  real_T Delay_b[2];                   /* '<S47>/Delay' (Output 1)  */
  real_T DiscreteTimeIntegrator;
                              /* '<S45>/Discrete-Time Integrator' (Output 1)  */
  real_T DiscreteTimeIntegrator1;
                             /* '<S45>/Discrete-Time Integrator1' (Output 1)  */
  real_T Delay_i[2];                   /* '<S48>/Delay' (Output 1)  */
  real_T Gain1_o;                      /* '<S49>/Gain1' (Output 1)  */
  real_T Switch_o;                     /* '<S49>/Switch' (Output 1)  */
  real_T Gain_k;                       /* '<S49>/Gain' (Output 1)  */
  real_T Divide;                       /* '<S49>/Divide' (Output 1)  */
  real_T Saturation_a;                 /* '<S45>/Saturation' (Output 1)  */
  real_T Subtract_c;                   /* '<S45>/Subtract' (Output 1)  */
  real_T Product_l;                    /* '<S45>/Product' (Output 1)  */
  real_T Gain1_b2;                     /* '<S45>/Gain1' (Output 1)  */
  real_T Sum1_i;                       /* '<S45>/Sum1' (Output 1)  */
  real_T Gain5;                        /* '<S45>/Gain5' (Output 1)  */
  real_T Sum3_n;                       /* '<S45>/Sum3' (Output 1)  */
  real_T Divide_j;                     /* '<S45>/Divide' (Output 1)  */
  real_T Gain6_a;                      /* '<S45>/Gain6' (Output 1)  */
  real_T Gain3_k;                      /* '<S49>/Gain3' (Output 1)  */
  real_T Gain2_b;                      /* '<S49>/Gain2' (Output 1)  */
  real_T Divide1;                      /* '<S49>/Divide1' (Output 1)  */
  real_T Saturation1;                  /* '<S45>/Saturation1' (Output 1)  */
  real_T Subtract1;                    /* '<S45>/Subtract1' (Output 1)  */
  real_T Product1_d;                   /* '<S45>/Product1' (Output 1)  */
  real_T Gain3_e;                      /* '<S45>/Gain3' (Output 1)  */
  real_T Sum2_b;                       /* '<S45>/Sum2' (Output 1)  */
  real_T Sum4_e;                       /* '<S45>/Sum4' (Output 1)  */
  real_T Divide1_j;                    /* '<S45>/Divide1' (Output 1)  */
  real_T Gain2_k;                      /* '<S45>/Gain2' (Output 1)  */
  real_T Gain4;                        /* '<S45>/Gain4' (Output 1)  */
  real_T TmpSignalConversionAtDelayInpor[2];
                                /* '<S45>/TmpSignal ConversionAtDelayInport1' */
  real_T Cos_g;                        /* '<S47>/Cos' (Output 1)  */
  real_T Sum_a;                        /* '<S47>/Sum' (Output 1)  */
  real_T Cos1_g;                       /* '<S47>/Cos1' (Output 1)  */
  real_T Sum2_p;                       /* '<S47>/Sum2' (Output 1)  */
  real_T Cos2_d;                       /* '<S47>/Cos2' (Output 1)  */
  real_T Cos3_i;                       /* '<S47>/Cos3' (Output 1)  */
  real_T Sum3_c;                       /* '<S47>/Sum3' (Output 1)  */
  real_T Cos4_a;                       /* '<S47>/Cos4' (Output 1)  */
  real_T Sum5_d;                       /* '<S47>/Sum5' (Output 1)  */
  real_T Cos5_j;                       /* '<S47>/Cos5' (Output 1)  */
  real_T Product_o;                    /* '<S47>/Product' (Output 1)  */
  real_T Product1_h;                   /* '<S47>/Product1' (Output 1)  */
  real_T Product2_j;                   /* '<S47>/Product2' (Output 1)  */
  real_T Sum1_d;                       /* '<S47>/Sum1' (Output 1)  */
  real_T Gain_g;                       /* '<S47>/Gain' (Output 1)  */
  real_T Product3_f;                   /* '<S47>/Product3' (Output 1)  */
  real_T Product4_m;                   /* '<S47>/Product4' (Output 1)  */
  real_T Product5_o;                   /* '<S47>/Product5' (Output 1)  */
  real_T Sum4_g;                       /* '<S47>/Sum4' (Output 1)  */
  real_T Gain1_n;                      /* '<S47>/Gain1' (Output 1)  */
  real_T TmpSignalConversionAtDelayInp_h[2];
                                /* '<S47>/TmpSignal ConversionAtDelayInport1' */
  real_T Cos_m;                        /* '<S48>/Cos' (Output 1)  */
  real_T Sum_m;                        /* '<S48>/Sum' (Output 1)  */
  real_T Cos1_e;                       /* '<S48>/Cos1' (Output 1)  */
  real_T Sum2_e;                       /* '<S48>/Sum2' (Output 1)  */
  real_T Cos2_l;                       /* '<S48>/Cos2' (Output 1)  */
  real_T Cos3_e;                       /* '<S48>/Cos3' (Output 1)  */
  real_T Sum3_d;                       /* '<S48>/Sum3' (Output 1)  */
  real_T Cos4_o;                       /* '<S48>/Cos4' (Output 1)  */
  real_T Sum5_m;                       /* '<S48>/Sum5' (Output 1)  */
  real_T Cos5_a;                       /* '<S48>/Cos5' (Output 1)  */
  real_T Product_p;                    /* '<S48>/Product' (Output 1)  */
  real_T Product1_m;                   /* '<S48>/Product1' (Output 1)  */
  real_T Product2_p;                   /* '<S48>/Product2' (Output 1)  */
  real_T Sum1_h;                       /* '<S48>/Sum1' (Output 1)  */
  real_T Gain_n;                       /* '<S48>/Gain' (Output 1)  */
  real_T Product3_k;                   /* '<S48>/Product3' (Output 1)  */
  real_T Product4_e;                   /* '<S48>/Product4' (Output 1)  */
  real_T Product5_n;                   /* '<S48>/Product5' (Output 1)  */
  real_T Sum4_n;                       /* '<S48>/Sum4' (Output 1)  */
  real_T Gain1_b1;                     /* '<S48>/Gain1' (Output 1)  */
  real_T TmpSignalConversionAtDelayIn_h0[2];
                                /* '<S48>/TmpSignal ConversionAtDelayInport1' */
  real_T Bias;                         /* '<S78>/Bias' (Output 1)  */
  real_T Gain_e;                       /* '<S78>/Gain' (Output 1)  */
  real_T RoundingFunction;           /* '<S78>/Rounding Function' (Output 1)  */
  real_T Gain1_nl;                     /* '<S78>/Gain1' (Output 1)  */
  real_T Sum1_e;                       /* '<S78>/Sum1' (Output 1)  */
  real_T Initial;                      /* '<S75>/Initial' (Output 1)  */
  real_T Integrator;                   /* '<S75>/Integrator' (Output 1)  */
  real_T TrigonometricFunction1_o1;
                               /* '<S64>/Trigonometric Function1' (Output 1)  */
  real_T TrigonometricFunction1_o2;
                               /* '<S64>/Trigonometric Function1' (Output 2)  */
  real_T Product1_hb[2];               /* '<S57>/Product1' (Output 1)  */
  real_T Integrator_c[2];              /* '<S57>/Integrator' (Output 1)  */
  real_T UnitDelay1[2];                /* '<S68>/Unit Delay1' (Output 1)  */
  real_T Subtract1_m[2];               /* '<S68>/Subtract1' (Output 1)  */
  real_T delta[2];                     /* '<S68>/delta' (Output 1)  */
  real_T Add1[2];                      /* '<S68>/Add1' (Output 1)  */
  real_T UnitDelay2[2];                /* '<S68>/Unit Delay2' (Output 1)  */
  real_T Subtract3[2];                 /* '<S68>/Subtract3' (Output 1)  */
  real_T Delay_d[2];                   /* '<S68>/Delay' (Output 1)  */
  real_T Subtract2[2];                 /* '<S68>/Subtract2' (Output 1)  */
  real_T delta_p[2];                   /* '<S68>/delta'' (Output 1)  */
  real_T Add2[2];                      /* '<S68>/Add2' (Output 1)  */
  real_T Subtract4[2];                 /* '<S68>/Subtract4' (Output 1)  */
  real_T Add3[2];                      /* '<S68>/Add3' (Output 1)  */
  real_T Gain_ki[2];                   /* '<S68>/Gain' (Output 1)  */
  real_T Gain_hy[2];                   /* '<S57>/Gain' (Output 1)  */
  real_T Product1_ku[2];               /* '<S59>/Product1' (Output 1)  */
  real_T Integrator_a[2];              /* '<S59>/Integrator' (Output 1)  */
  real_T UnitDelay1_g[2];              /* '<S70>/Unit Delay1' (Output 1)  */
  real_T Subtract1_p[2];               /* '<S70>/Subtract1' (Output 1)  */
  real_T delta_f[2];                   /* '<S70>/delta' (Output 1)  */
  real_T Add1_l[2];                    /* '<S70>/Add1' (Output 1)  */
  real_T UnitDelay2_c[2];              /* '<S70>/Unit Delay2' (Output 1)  */
  real_T Subtract3_h[2];               /* '<S70>/Subtract3' (Output 1)  */
  real_T Delay_lg[2];                  /* '<S70>/Delay' (Output 1)  */
  real_T Subtract2_n[2];               /* '<S70>/Subtract2' (Output 1)  */
  real_T delta_c[2];                   /* '<S70>/delta'' (Output 1)  */
  real_T Add2_a[2];                    /* '<S70>/Add2' (Output 1)  */
  real_T Subtract4_n[2];               /* '<S70>/Subtract4' (Output 1)  */
  real_T Add3_h[2];                    /* '<S70>/Add3' (Output 1)  */
  real_T Gain_a[2];                    /* '<S70>/Gain' (Output 1)  */
  real_T Gain_l[2];                    /* '<S59>/Gain' (Output 1)  */
  real_T Product1_e[2];                /* '<S61>/Product1' (Output 1)  */
  real_T Integrator_b[2];              /* '<S61>/Integrator' (Output 1)  */
  real_T UnitDelay1_gn[2];             /* '<S72>/Unit Delay1' (Output 1)  */
  real_T Subtract1_h[2];               /* '<S72>/Subtract1' (Output 1)  */
  real_T delta_cv[2];                  /* '<S72>/delta' (Output 1)  */
  real_T Add1_e[2];                    /* '<S72>/Add1' (Output 1)  */
  real_T UnitDelay2_h[2];              /* '<S72>/Unit Delay2' (Output 1)  */
  real_T Subtract3_d[2];               /* '<S72>/Subtract3' (Output 1)  */
  real_T Delay_k[2];                   /* '<S72>/Delay' (Output 1)  */
  real_T Subtract2_g[2];               /* '<S72>/Subtract2' (Output 1)  */
  real_T delta_e[2];                   /* '<S72>/delta'' (Output 1)  */
  real_T Add2_h[2];                    /* '<S72>/Add2' (Output 1)  */
  real_T Subtract4_m[2];               /* '<S72>/Subtract4' (Output 1)  */
  real_T Add3_m[2];                    /* '<S72>/Add3' (Output 1)  */
  real_T Gain_m[2];                    /* '<S72>/Gain' (Output 1)  */
  real_T Gain_nw[2];                   /* '<S61>/Gain' (Output 1)  */
  real_T Product1_e1[2];               /* '<S56>/Product1' (Output 1)  */
  real_T Integrator_d[2];              /* '<S56>/Integrator' (Output 1)  */
  real_T UnitDelay1_p[2];              /* '<S67>/Unit Delay1' (Output 1)  */
  real_T Subtract1_a[2];               /* '<S67>/Subtract1' (Output 1)  */
  real_T delta_c1[2];                  /* '<S67>/delta' (Output 1)  */
  real_T Add1_k[2];                    /* '<S67>/Add1' (Output 1)  */
  real_T UnitDelay2_a[2];              /* '<S67>/Unit Delay2' (Output 1)  */
  real_T Subtract3_a[2];               /* '<S67>/Subtract3' (Output 1)  */
  real_T Delay_in[2];                  /* '<S67>/Delay' (Output 1)  */
  real_T Subtract2_f[2];               /* '<S67>/Subtract2' (Output 1)  */
  real_T delta_j[2];                   /* '<S67>/delta'' (Output 1)  */
  real_T Add2_i[2];                    /* '<S67>/Add2' (Output 1)  */
  real_T Subtract4_h[2];               /* '<S67>/Subtract4' (Output 1)  */
  real_T Add3_g[2];                    /* '<S67>/Add3' (Output 1)  */
  real_T Gain_d[2];                    /* '<S67>/Gain' (Output 1)  */
  real_T Gain_c[2];                    /* '<S56>/Gain' (Output 1)  */
  real_T Product1_l[2];                /* '<S58>/Product1' (Output 1)  */
  real_T Integrator_m[2];              /* '<S58>/Integrator' (Output 1)  */
  real_T UnitDelay1_k[2];              /* '<S69>/Unit Delay1' (Output 1)  */
  real_T Subtract1_n[2];               /* '<S69>/Subtract1' (Output 1)  */
  real_T delta_n[2];                   /* '<S69>/delta' (Output 1)  */
  real_T Add1_ej[2];                   /* '<S69>/Add1' (Output 1)  */
  real_T UnitDelay2_j[2];              /* '<S69>/Unit Delay2' (Output 1)  */
  real_T Subtract3_aq[2];              /* '<S69>/Subtract3' (Output 1)  */
  real_T Delay_j[2];                   /* '<S69>/Delay' (Output 1)  */
  real_T Subtract2_m[2];               /* '<S69>/Subtract2' (Output 1)  */
  real_T delta_h[2];                   /* '<S69>/delta'' (Output 1)  */
  real_T Add2_l[2];                    /* '<S69>/Add2' (Output 1)  */
  real_T Subtract4_c[2];               /* '<S69>/Subtract4' (Output 1)  */
  real_T Add3_c[2];                    /* '<S69>/Add3' (Output 1)  */
  real_T Gain_kl[2];                   /* '<S69>/Gain' (Output 1)  */
  real_T Gain_o[2];                    /* '<S58>/Gain' (Output 1)  */
  real_T Product1_k4[2];               /* '<S60>/Product1' (Output 1)  */
  real_T Integrator_o[2];              /* '<S60>/Integrator' (Output 1)  */
  real_T UnitDelay1_ka[2];             /* '<S71>/Unit Delay1' (Output 1)  */
  real_T Subtract1_pk[2];              /* '<S71>/Subtract1' (Output 1)  */
  real_T delta_a[2];                   /* '<S71>/delta' (Output 1)  */
  real_T Add1_ea[2];                   /* '<S71>/Add1' (Output 1)  */
  real_T UnitDelay2_b[2];              /* '<S71>/Unit Delay2' (Output 1)  */
  real_T Subtract3_hy[2];              /* '<S71>/Subtract3' (Output 1)  */
  real_T Delay_jw[2];                  /* '<S71>/Delay' (Output 1)  */
  real_T Subtract2_mn[2];              /* '<S71>/Subtract2' (Output 1)  */
  real_T delta_fs[2];                  /* '<S71>/delta'' (Output 1)  */
  real_T Add2_a5[2];                   /* '<S71>/Add2' (Output 1)  */
  real_T Subtract4_me[2];              /* '<S71>/Subtract4' (Output 1)  */
  real_T Add3_n[2];                    /* '<S71>/Add3' (Output 1)  */
  real_T Gain_k2[2];                   /* '<S71>/Gain' (Output 1)  */
  real_T Gain_cs[2];                   /* '<S60>/Gain' (Output 1)  */
  real_T ComplextoRealImag_o1;    /* '<S53>/Complex to Real-Imag' (Output 1)  */
  real_T ComplextoRealImag_o2;    /* '<S53>/Complex to Real-Imag' (Output 2)  */
  real_T Conversion;                   /* '<S73>/Conversion' (Output 1)  */
  real_T Gain_j[3];                    /* '<S51>/Gain' (Output 1)  */
  real_T Gain5_e;                      /* '<S51>/Gain5' (Output 1)  */
  real_T Gain6_p;                      /* '<S51>/Gain6' (Output 1)  */
  real_T Sum1_k;                       /* '<S51>/Sum1' (Output 1)  */
  real_T Gain4_k;                      /* '<S51>/Gain4' (Output 1)  */
  real_T Cos2_h;                       /* '<S51>/Cos2' (Output 1)  */
  real_T Gain2_b3;                     /* '<S51>/Gain2' (Output 1)  */
  real_T Gain3_j;                      /* '<S51>/Gain3' (Output 1)  */
  real_T Sum_l;                        /* '<S51>/Sum' (Output 1)  */
  real_T Gain1_e;                      /* '<S51>/Gain1' (Output 1)  */
  real_T Cos3_f;                       /* '<S51>/Cos3' (Output 1)  */
  real_T Cos_l;                        /* '<S51>/Cos' (Output 1)  */
  real_T Cos1_i;                       /* '<S51>/Cos1' (Output 1)  */
  real_T Product_le;                   /* '<S51>/Product' (Output 1)  */
  real_T Product1_c;                   /* '<S51>/Product1' (Output 1)  */
  real_T Sum3_dz;                      /* '<S51>/Sum3' (Output 1)  */
  real_T Gain7_i;                      /* '<S51>/Gain7' (Output 1)  */
  real_T Integrator_dd;                /* '<S51>/Integrator' (Output 1)  */
  real_T Product2_pp;                  /* '<S51>/Product2' (Output 1)  */
  real_T Product3_o;                   /* '<S51>/Product3' (Output 1)  */
  real_T Sum2_b4;                      /* '<S51>/Sum2' (Output 1)  */
  real_T Sum4_ng;                      /* '<S51>/Sum4' (Output 1)  */
  real_T W;                            /* '<S51>/Sum5' (Output 1)  */
  real_T donotdeletethisgain_o;/* '<S18>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_c;/* '<S19>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_b;/* '<S20>/do not delete this gain' (Output 1)  */
  real_T TmpSignalConversionAtDelay2Inpo[3];
                                /* '<S5>/TmpSignal ConversionAtDelay2Inport1' */
  real_T donotdeletethisgain_gz;
                               /* '<S31>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_e;/* '<S32>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_j;/* '<S24>/do not delete this gain' (Output 1)  */
  real_T UnitDelay_c[6];               /* '<S116>/Unit Delay' (Output 1)  */
  real_T Constant11_i;                 /* '<S107>/Constant11' (Output 1)  */
  real_T Delay_bh[2];                  /* '<S124>/Delay' (Output 1)  */
  real_T Subtract_h;                   /* '<S130>/Subtract' (Output 1)  */
  real_T Integrator2;                  /* '<S130>/Integrator2' (Output 1)  */
  real_T Cos_h;                        /* '<S131>/Cos' (Output 1)  */
  real_T Product_k;                    /* '<S131>/Product' (Output 1)  */
  real_T Cos1_j;                       /* '<S131>/Cos1' (Output 1)  */
  real_T Gain_i;                       /* '<S131>/Gain' (Output 1)  */
  real_T Product1_cr;                  /* '<S131>/Product1' (Output 1)  */
  real_T Sum1_n;                       /* '<S131>/Sum1' (Output 1)  */
  real_T Sum6_g;                       /* '<S131>/Sum6' (Output 1)  */
  real_T Cos2_m;                       /* '<S131>/Cos2' (Output 1)  */
  real_T Product2_g;                   /* '<S131>/Product2' (Output 1)  */
  real_T Sum2_m;                       /* '<S131>/Sum2' (Output 1)  */
  real_T Cos3_g;                       /* '<S131>/Cos3' (Output 1)  */
  real_T Gain1_ov;                     /* '<S131>/Gain1' (Output 1)  */
  real_T Product3_h;                   /* '<S131>/Product3' (Output 1)  */
  real_T Sum3_m;                       /* '<S131>/Sum3' (Output 1)  */
  real_T Sum7_c;                       /* '<S131>/Sum7' (Output 1)  */
  real_T Cos4_f;                       /* '<S131>/Cos4' (Output 1)  */
  real_T Product4_i;                   /* '<S131>/Product4' (Output 1)  */
  real_T Sum4_d;                       /* '<S131>/Sum4' (Output 1)  */
  real_T Cos5_c;                       /* '<S131>/Cos5' (Output 1)  */
  real_T Gain2_e;                      /* '<S131>/Gain2' (Output 1)  */
  real_T Product5_m;                   /* '<S131>/Product5' (Output 1)  */
  real_T Sum5_e;                       /* '<S131>/Sum5' (Output 1)  */
  real_T Product2_f[3];                /* '<S101>/Product2' (Output 1)  */
  real_T Saturation_g[3];              /* '<S101>/Saturation' (Output 1)  */
  real_T Sum16_m;                      /* '<S107>/Sum16' (Output 1)  */
  real_T Divide7_i;                    /* '<S107>/Divide7' (Output 1)  */
  real_T Sum17_d;                      /* '<S107>/Sum17' (Output 1)  */
  real_T Divide8_p;                    /* '<S107>/Divide8' (Output 1)  */
  real_T Sum18_b;                      /* '<S107>/Sum18' (Output 1)  */
  real_T Divide9_d;                    /* '<S107>/Divide9' (Output 1)  */
  real_T TmpSignalConversionAtRTESPWM1_f[3];
                           /* '<S107>/TmpSignal ConversionAtRTE SPWM1Inport2' */
  real_T RTESPWM1_o1_h[3];             /* '<S107>/RTE SPWM1' (Output 1)  */
  real_T RTESPWM1_o2_b[3];             /* '<S107>/RTE SPWM1' (Output 2)  */
  real_T RTEConversion1_j[6];         /* '<S107>/RTE Conversion1' (Output 1)  */
  real_T DataTypeConversion_o[6];/* '<S119>/Data Type Conversion' (Output 1)  */
  real_T DC_h;                         /* '<S152>/DC' (Output 1)  */
  real_T SineWaveA;                    /* '<S157>/Sine Wave A' (Output 1)  */
  real_T SineWaveB;                    /* '<S157>/Sine Wave B' (Output 1)  */
  real_T SineWaveC;                    /* '<S157>/Sine Wave C' (Output 1)  */
  real_T Switch_d[6];                  /* '<S118>/Switch' (Output 1)  */
  real_T StateSpace_o1_h[15];          /* '<S163>/State-Space' (Output 1)  */
  real_T StateSpace_o2_f[6];           /* '<S163>/State-Space' (Output 2)  */
  real_T uRon_o[6];                    /* '<S116>/1//Ron' (Output 1)  */
  real_T Switch_g[6];                  /* '<S116>/Switch' (Output 1)  */
  real_T Saturation_h[6];              /* '<S116>/Saturation' (Output 1)  */
  real_T donotdeletethisgain_c2;
                              /* '<S113>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_h;
                              /* '<S114>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_f;
                              /* '<S115>/do not delete this gain' (Output 1)  */
  real_T Delay_f[2];                   /* '<S126>/Delay' (Output 1)  */
  real_T Delay_iu[2];                  /* '<S127>/Delay' (Output 1)  */
  real_T Integrator_cl[8];             /* '<S136>/Integrator' (Output 1)  */
  real_T TransportDelay[8];           /* '<S136>/Transport Delay' (Output 1)  */
  real_T Subtract_k[8];                /* '<S136>/Subtract' (Output 1)  */
  real_T Gain_hs[8];                   /* '<S136>/Gain' (Output 1)  */
  real_T Integrator_e[8];              /* '<S138>/Integrator' (Output 1)  */
  real_T TransportDelay_m[8];         /* '<S138>/Transport Delay' (Output 1)  */
  real_T Subtract_p[8];                /* '<S138>/Subtract' (Output 1)  */
  real_T Gain_ow[8];                   /* '<S138>/Gain' (Output 1)  */
  real_T Integrator_g[8];              /* '<S140>/Integrator' (Output 1)  */
  real_T TransportDelay_a[8];         /* '<S140>/Transport Delay' (Output 1)  */
  real_T Subtract_b[8];                /* '<S140>/Subtract' (Output 1)  */
  real_T Gain_hk[8];                   /* '<S140>/Gain' (Output 1)  */
  real_T Integrator_bf[8];             /* '<S135>/Integrator' (Output 1)  */
  real_T TransportDelay_d[8];         /* '<S135>/Transport Delay' (Output 1)  */
  real_T Subtract_b2[8];               /* '<S135>/Subtract' (Output 1)  */
  real_T Gain_b[8];                    /* '<S135>/Gain' (Output 1)  */
  real_T Integrator_j[8];              /* '<S137>/Integrator' (Output 1)  */
  real_T TransportDelay_k[8];         /* '<S137>/Transport Delay' (Output 1)  */
  real_T Subtract_i[8];                /* '<S137>/Subtract' (Output 1)  */
  real_T Gain_dj[8];                   /* '<S137>/Gain' (Output 1)  */
  real_T Integrator_n[8];              /* '<S139>/Integrator' (Output 1)  */
  real_T TransportDelay_p[8];         /* '<S139>/Transport Delay' (Output 1)  */
  real_T Subtract_by[8];               /* '<S139>/Subtract' (Output 1)  */
  real_T Gain_bo[8];                   /* '<S139>/Gain' (Output 1)  */
  real_T ComplextoRealImag_o1_b[4];
                                 /* '<S132>/Complex to Real-Imag' (Output 1)  */
  real_T ComplextoRealImag_o2_i[4];
                                 /* '<S132>/Complex to Real-Imag' (Output 2)  */
  real_T DiscreteTimeIntegrator_b;
                             /* '<S124>/Discrete-Time Integrator' (Output 1)  */
  real_T DiscreteTimeIntegrator1_p;
                            /* '<S124>/Discrete-Time Integrator1' (Output 1)  */
  real_T Gain1_lp;                     /* '<S128>/Gain1' (Output 1)  */
  real_T Switch_e;                     /* '<S128>/Switch' (Output 1)  */
  real_T Gain_hn;                      /* '<S128>/Gain' (Output 1)  */
  real_T Divide_g;                     /* '<S128>/Divide' (Output 1)  */
  real_T Saturation_n;                 /* '<S124>/Saturation' (Output 1)  */
  real_T Subtract_cj;                  /* '<S124>/Subtract' (Output 1)  */
  real_T Product_o0;                   /* '<S124>/Product' (Output 1)  */
  real_T Gain1_c;                      /* '<S124>/Gain1' (Output 1)  */
  real_T Sum1_g;                       /* '<S124>/Sum1' (Output 1)  */
  real_T Gain5_j;                      /* '<S124>/Gain5' (Output 1)  */
  real_T Sum3_l;                       /* '<S124>/Sum3' (Output 1)  */
  real_T Divide_c;                     /* '<S124>/Divide' (Output 1)  */
  real_T Gain6_h5;                     /* '<S124>/Gain6' (Output 1)  */
  real_T Gain3_ez;                     /* '<S128>/Gain3' (Output 1)  */
  real_T Gain2_et;                     /* '<S128>/Gain2' (Output 1)  */
  real_T Divide1_d;                    /* '<S128>/Divide1' (Output 1)  */
  real_T Saturation1_k;                /* '<S124>/Saturation1' (Output 1)  */
  real_T Subtract1_d;                  /* '<S124>/Subtract1' (Output 1)  */
  real_T Product1_f;                   /* '<S124>/Product1' (Output 1)  */
  real_T Gain3_ko;                     /* '<S124>/Gain3' (Output 1)  */
  real_T Sum2_pd;                      /* '<S124>/Sum2' (Output 1)  */
  real_T Sum4_m;                       /* '<S124>/Sum4' (Output 1)  */
  real_T Divide1_h;                    /* '<S124>/Divide1' (Output 1)  */
  real_T Gain2_n;                      /* '<S124>/Gain2' (Output 1)  */
  real_T Gain4_m;                      /* '<S124>/Gain4' (Output 1)  */
  real_T TmpSignalConversionAtDelayI_h0n[2];
                               /* '<S124>/TmpSignal ConversionAtDelayInport1' */
  real_T Cos_ln;                       /* '<S126>/Cos' (Output 1)  */
  real_T Sum_n;                        /* '<S126>/Sum' (Output 1)  */
  real_T Cos1_m;                       /* '<S126>/Cos1' (Output 1)  */
  real_T Sum2_c;                       /* '<S126>/Sum2' (Output 1)  */
  real_T Cos2_k;                       /* '<S126>/Cos2' (Output 1)  */
  real_T Cos3_iw;                      /* '<S126>/Cos3' (Output 1)  */
  real_T Sum3_mo;                      /* '<S126>/Sum3' (Output 1)  */
  real_T Cos4_l;                       /* '<S126>/Cos4' (Output 1)  */
  real_T Sum5_f;                       /* '<S126>/Sum5' (Output 1)  */
  real_T Cos5_g;                       /* '<S126>/Cos5' (Output 1)  */
  real_T Product_n;                    /* '<S126>/Product' (Output 1)  */
  real_T Product1_j;                   /* '<S126>/Product1' (Output 1)  */
  real_T Product2_g1;                  /* '<S126>/Product2' (Output 1)  */
  real_T Sum1_gm;                      /* '<S126>/Sum1' (Output 1)  */
  real_T Gain_nq;                      /* '<S126>/Gain' (Output 1)  */
  real_T Product3_e;                   /* '<S126>/Product3' (Output 1)  */
  real_T Product4_b;                   /* '<S126>/Product4' (Output 1)  */
  real_T Product5_l;                   /* '<S126>/Product5' (Output 1)  */
  real_T Sum4_c;                       /* '<S126>/Sum4' (Output 1)  */
  real_T Gain1_ft;                     /* '<S126>/Gain1' (Output 1)  */
  real_T TmpSignalConversionAtDelay_h0n4[2];
                               /* '<S126>/TmpSignal ConversionAtDelayInport1' */
  real_T Cos_a;                        /* '<S127>/Cos' (Output 1)  */
  real_T Sum_f;                        /* '<S127>/Sum' (Output 1)  */
  real_T Cos1_c;                       /* '<S127>/Cos1' (Output 1)  */
  real_T Sum2_j;                       /* '<S127>/Sum2' (Output 1)  */
  real_T Cos2_j;                       /* '<S127>/Cos2' (Output 1)  */
  real_T Cos3_p;                       /* '<S127>/Cos3' (Output 1)  */
  real_T Sum3_o;                       /* '<S127>/Sum3' (Output 1)  */
  real_T Cos4_e;                       /* '<S127>/Cos4' (Output 1)  */
  real_T Sum5_k;                       /* '<S127>/Sum5' (Output 1)  */
  real_T Cos5_e;                       /* '<S127>/Cos5' (Output 1)  */
  real_T Product_i;                    /* '<S127>/Product' (Output 1)  */
  real_T Product1_c2;                  /* '<S127>/Product1' (Output 1)  */
  real_T Product2_jv;                  /* '<S127>/Product2' (Output 1)  */
  real_T Sum1_ju;                      /* '<S127>/Sum1' (Output 1)  */
  real_T Gain_e4;                      /* '<S127>/Gain' (Output 1)  */
  real_T Product3_g;                   /* '<S127>/Product3' (Output 1)  */
  real_T Product4_d;                   /* '<S127>/Product4' (Output 1)  */
  real_T Product5_c;                   /* '<S127>/Product5' (Output 1)  */
  real_T Sum4_k;                       /* '<S127>/Sum4' (Output 1)  */
  real_T Gain1_ka;                     /* '<S127>/Gain1' (Output 1)  */
  real_T TmpSignalConversionAtDela_h0n42[2];
                               /* '<S127>/TmpSignal ConversionAtDelayInport1' */
  real_T Integrator_i[4];              /* '<S147>/Integrator' (Output 1)  */
  real_T TrigonometricFunction1_o1_h[4];
                              /* '<S143>/Trigonometric Function1' (Output 1)  */
  real_T TrigonometricFunction1_o2_l[4];
                              /* '<S143>/Trigonometric Function1' (Output 2)  */
  real_T Product1_er[8];               /* '<S135>/Product1' (Output 1)  */
  real_T Product1_kj[8];               /* '<S136>/Product1' (Output 1)  */
  real_T Product1_ew[8];               /* '<S137>/Product1' (Output 1)  */
  real_T Product1_b[8];                /* '<S138>/Product1' (Output 1)  */
  real_T Product1_bw[8];               /* '<S139>/Product1' (Output 1)  */
  real_T Product1_l0[8];               /* '<S140>/Product1' (Output 1)  */
  real_T DataTypeConversion_h[4];/* '<S147>/Data Type Conversion' (Output 1)  */
  real_T Integrator_g5;                /* '<S130>/Integrator' (Output 1)  */
  real_T Gain_oh[3];                   /* '<S130>/Gain' (Output 1)  */
  real_T Gain5_o;                      /* '<S130>/Gain5' (Output 1)  */
  real_T Gain6_k;                      /* '<S130>/Gain6' (Output 1)  */
  real_T Sum1_f;                       /* '<S130>/Sum1' (Output 1)  */
  real_T Gain4_c;                      /* '<S130>/Gain4' (Output 1)  */
  real_T Cos_j;                        /* '<S130>/Cos' (Output 1)  */
  real_T Product_c;                    /* '<S130>/Product' (Output 1)  */
  real_T Gain2_a;                      /* '<S130>/Gain2' (Output 1)  */
  real_T Gain3_m;                      /* '<S130>/Gain3' (Output 1)  */
  real_T Sum_mo;                       /* '<S130>/Sum' (Output 1)  */
  real_T Gain1_il;                     /* '<S130>/Gain1' (Output 1)  */
  real_T Cos1_h;                       /* '<S130>/Cos1' (Output 1)  */
  real_T Product1_g;                   /* '<S130>/Product1' (Output 1)  */
  real_T Sum3_e;                       /* '<S130>/Sum3' (Output 1)  */
  real_T Gain7_n;                      /* '<S130>/Gain7' (Output 1)  */
  real_T Sum4_b;                       /* '<S130>/Sum4' (Output 1)  */
  real_T W_a;                          /* '<S130>/Sum5' (Output 1)  */
  real_T Cos2_lc;                      /* '<S130>/Cos2' (Output 1)  */
  real_T Cos3_n;                       /* '<S130>/Cos3' (Output 1)  */
  real_T Product2_i;                   /* '<S130>/Product2' (Output 1)  */
  real_T Product3_fy;                  /* '<S130>/Product3' (Output 1)  */
  real_T Sum2_g;                       /* '<S130>/Sum2' (Output 1)  */
  real_T donotdeletethisgain_ge;
                              /* '<S102>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_c5;
                              /* '<S103>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_nq;
                              /* '<S104>/do not delete this gain' (Output 1)  */
  real_T TmpSignalConversionAtDelay2In_p[3];
                                /* '<S8>/TmpSignal ConversionAtDelay2Inport1' */
  real_T donotdeletethisgain_he;
                              /* '<S110>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_i;
                              /* '<S111>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_lx;
                              /* '<S105>/do not delete this gain' (Output 1)  */
  real_T dataready25kHz;             /* '<S3>/data ready 2.5 kHz' (Output 1)  */
  real_T SFunction2;                   /* '<S14>/S-Function2' (Output 1)  */
  boolean_T RelationalOperator;    /* '<S51>/Relational Operator' (Output 1)  */
  boolean_T Compare;                   /* '<S76>/Compare' (Output 1)  */
  boolean_T Compare_c;                 /* '<S77>/Compare' (Output 1)  */
  boolean_T LogicalOperator;          /* '<S75>/Logical Operator' (Output 1)  */
  boolean_T RelationalOperator_i; /* '<S130>/Relational Operator' (Output 1)  */
  B_Tail_basic_cosimulation_c_1_T Tail_e;/* '<S116>/Tail' */
  B_Tail_basic_cosimulation_c_1_T Tail;/* '<S37>/Tail' */
} B_basic_cosimulation_c_1_sm_master_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay_DSTATE[6];          /* '<S37>/Unit Delay' (DWork 1)  */
  real_T Delay_DSTATE[2];              /* '<S45>/Delay' (DWork 1)  */
  real_T freqTnow;             /* '<S27>/Phase// Frequency Offset' (DWork 1)  */
  real_T MOV_segment9[14];             /* '<S162>/State-Space' (DWork 1)  */
  real_T MOV_segment9_o[16];           /* '<S162>/State-Space' (DWork 2)  */
  real_T MOV_segment9_m[6];            /* '<S162>/State-Space' (DWork 3)  */
  real_T MOV_segment9_n[6];            /* '<S162>/State-Space' (DWork 4)  */
  real_T MOV_segment9_e[6];            /* '<S162>/State-Space' (DWork 5)  */
  real_T MOV_segment9_mt[6];           /* '<S162>/State-Space' (DWork 6)  */
  real_T MOV_segment9_i[6];            /* '<S162>/State-Space' (DWork 7)  */
  real_T MOV_segment9_j[18];           /* '<S162>/State-Space' (DWork 8)  */
  real_T MOV_segment9_d[18];           /* '<S162>/State-Space' (DWork 9)  */
  real_T MOV_segment9_l;               /* '<S162>/State-Space' (DWork 10)  */
  real_T freqTnow_p;           /* '<S30>/Phase// Frequency Offset' (DWork 1)  */
  real_T Delay_DSTATE_b;               /* '<S5>/Delay' (DWork 1)  */
  real_T Delay1_DSTATE;                /* '<S5>/Delay1' (DWork 1)  */
  real_T Delay2_DSTATE[3];             /* '<S8>/Delay2' (DWork 1)  */
  real_T Delay_DSTATE_f;               /* '<S8>/Delay' (DWork 1)  */
  real_T Delay1_DSTATE_a;              /* '<S8>/Delay1' (DWork 1)  */
  real_T Delay2_DSTATE_p[3];           /* '<S5>/Delay2' (DWork 1)  */
  real_T Delay_DSTATE_c[2];            /* '<S47>/Delay' (DWork 1)  */
  real_T DiscreteTimeIntegrator_DSTATE;
                               /* '<S45>/Discrete-Time Integrator' (DWork 1)  */
  real_T DiscreteTimeIntegrator1_DSTATE;
                              /* '<S45>/Discrete-Time Integrator1' (DWork 1)  */
  real_T Delay_DSTATE_p[2];            /* '<S48>/Delay' (DWork 1)  */
  real_T Integrator_DSTATE;            /* '<S75>/Integrator' (DWork 1)  */
  real_T Integrator_DSTATE_h[2];       /* '<S57>/Integrator' (DWork 1)  */
  real_T UnitDelay1_DSTATE[2];         /* '<S68>/Unit Delay1' (DWork 1)  */
  real_T UnitDelay2_DSTATE[2];         /* '<S68>/Unit Delay2' (DWork 1)  */
  real_T Delay_DSTATE_l[666];          /* '<S68>/Delay' (DWork 1)  */
  real_T Integrator_DSTATE_f[2];       /* '<S59>/Integrator' (DWork 1)  */
  real_T UnitDelay1_DSTATE_j[2];       /* '<S70>/Unit Delay1' (DWork 1)  */
  real_T UnitDelay2_DSTATE_p[2];       /* '<S70>/Unit Delay2' (DWork 1)  */
  real_T Delay_DSTATE_g[666];          /* '<S70>/Delay' (DWork 1)  */
  real_T Integrator_DSTATE_j[2];       /* '<S61>/Integrator' (DWork 1)  */
  real_T UnitDelay1_DSTATE_e[2];       /* '<S72>/Unit Delay1' (DWork 1)  */
  real_T UnitDelay2_DSTATE_c[2];       /* '<S72>/Unit Delay2' (DWork 1)  */
  real_T Delay_DSTATE_cx[666];         /* '<S72>/Delay' (DWork 1)  */
  real_T Integrator_DSTATE_jv[2];      /* '<S56>/Integrator' (DWork 1)  */
  real_T UnitDelay1_DSTATE_d[2];       /* '<S67>/Unit Delay1' (DWork 1)  */
  real_T UnitDelay2_DSTATE_k[2];       /* '<S67>/Unit Delay2' (DWork 1)  */
  real_T Delay_DSTATE_a[666];          /* '<S67>/Delay' (DWork 1)  */
  real_T Integrator_DSTATE_g[2];       /* '<S58>/Integrator' (DWork 1)  */
  real_T UnitDelay1_DSTATE_i[2];       /* '<S69>/Unit Delay1' (DWork 1)  */
  real_T UnitDelay2_DSTATE_pc[2];      /* '<S69>/Unit Delay2' (DWork 1)  */
  real_T Delay_DSTATE_o[666];          /* '<S69>/Delay' (DWork 1)  */
  real_T Integrator_DSTATE_l[2];       /* '<S60>/Integrator' (DWork 1)  */
  real_T UnitDelay1_DSTATE_k[2];       /* '<S71>/Unit Delay1' (DWork 1)  */
  real_T UnitDelay2_DSTATE_d[2];       /* '<S71>/Unit Delay2' (DWork 1)  */
  real_T Delay_DSTATE_m[666];          /* '<S71>/Delay' (DWork 1)  */
  real_T UnitDelay_DSTATE_g[6];        /* '<S116>/Unit Delay' (DWork 1)  */
  real_T Delay_DSTATE_h[2];            /* '<S124>/Delay' (DWork 1)  */
  real_T MOV_segment9_jn[14];          /* '<S163>/State-Space' (DWork 1)  */
  real_T MOV_segment9_ly[16];          /* '<S163>/State-Space' (DWork 2)  */
  real_T MOV_segment9_c[6];            /* '<S163>/State-Space' (DWork 3)  */
  real_T MOV_segment9_jc[6];           /* '<S163>/State-Space' (DWork 4)  */
  real_T MOV_segment9_k[6];            /* '<S163>/State-Space' (DWork 5)  */
  real_T MOV_segment9_f[6];            /* '<S163>/State-Space' (DWork 6)  */
  real_T MOV_segment9_ib[6];           /* '<S163>/State-Space' (DWork 7)  */
  real_T MOV_segment9_kr[15];          /* '<S163>/State-Space' (DWork 8)  */
  real_T MOV_segment9_er[15];          /* '<S163>/State-Space' (DWork 9)  */
  real_T MOV_segment9_f0;              /* '<S163>/State-Space' (DWork 10)  */
  real_T Delay_DSTATE_cr[2];           /* '<S126>/Delay' (DWork 1)  */
  real_T Delay_DSTATE_ou[2];           /* '<S127>/Delay' (DWork 1)  */
  real_T DiscreteTimeIntegrator_DSTATE_h;
                              /* '<S124>/Discrete-Time Integrator' (DWork 1)  */
  real_T DiscreteTimeIntegrator1_DSTAT_m;
                             /* '<S124>/Discrete-Time Integrator1' (DWork 1)  */
  real_T SFunction_PreviousInput;      /* '<S2>/S-Function' (DWork 1)  */
  real_T lastSin;                      /* '<S157>/Sine Wave A' (DWork 2)  */
  real_T lastCos;                      /* '<S157>/Sine Wave A' (DWork 3)  */
  real_T lastSin_f;                    /* '<S157>/Sine Wave B' (DWork 2)  */
  real_T lastCos_m;                    /* '<S157>/Sine Wave B' (DWork 3)  */
  real_T lastSin_k;                    /* '<S157>/Sine Wave C' (DWork 2)  */
  real_T lastCos_i;                    /* '<S157>/Sine Wave C' (DWork 3)  */
  real_T RTEConversion1_RWORK[6];      /* '<S26>/RTE Conversion1' (DWork 1)  */
  real_T RTEConversion1_RWORK_c[6];    /* '<S107>/RTE Conversion1' (DWork 1)  */
  struct {
    real_T modelTStart;
    real_T TUbufferArea[65536];
  } TransportDelay_RWORK;              /* '<S136>/Transport Delay' (DWork 1)  */

  struct {
    real_T modelTStart;
    real_T TUbufferArea[65536];
  } TransportDelay_RWORK_d;            /* '<S138>/Transport Delay' (DWork 1)  */

  struct {
    real_T modelTStart;
    real_T TUbufferArea[65536];
  } TransportDelay_RWORK_a;            /* '<S140>/Transport Delay' (DWork 1)  */

  struct {
    real_T modelTStart;
    real_T TUbufferArea[65536];
  } TransportDelay_RWORK_f;            /* '<S135>/Transport Delay' (DWork 1)  */

  struct {
    real_T modelTStart;
    real_T TUbufferArea[65536];
  } TransportDelay_RWORK_j;            /* '<S137>/Transport Delay' (DWork 1)  */

  struct {
    real_T modelTStart;
    real_T TUbufferArea[65536];
  } TransportDelay_RWORK_p;            /* '<S139>/Transport Delay' (DWork 1)  */

  void *SFunction1_PWORK;              /* '<S6>/S-Function1' (DWork 1)  */
  void *RTESPWM1_PWORK;                /* '<S26>/RTE SPWM1' (DWork 1)  */
  void *RTEConversion1_PWORK;          /* '<S26>/RTE Conversion1' (DWork 2)  */
  void *StateSpace_PWORK;              /* '<S162>/State-Space' (DWork 11)  */
  void *RTESPWM1_PWORK_i;              /* '<S107>/RTE SPWM1' (DWork 1)  */
  void *RTEConversion1_PWORK_d;        /* '<S107>/RTE Conversion1' (DWork 2)  */
  void *StateSpace_PWORK_c;            /* '<S163>/State-Space' (DWork 11)  */
  struct {
    void *TUbufferPtrs[16];
  } TransportDelay_PWORK;              /* '<S136>/Transport Delay' (DWork 3)  */

  struct {
    void *TUbufferPtrs[16];
  } TransportDelay_PWORK_b;            /* '<S138>/Transport Delay' (DWork 3)  */

  struct {
    void *TUbufferPtrs[16];
  } TransportDelay_PWORK_n;            /* '<S140>/Transport Delay' (DWork 3)  */

  struct {
    void *TUbufferPtrs[16];
  } TransportDelay_PWORK_h;            /* '<S135>/Transport Delay' (DWork 3)  */

  struct {
    void *TUbufferPtrs[16];
  } TransportDelay_PWORK_bn;           /* '<S137>/Transport Delay' (DWork 3)  */

  struct {
    void *TUbufferPtrs[16];
  } TransportDelay_PWORK_d;            /* '<S139>/Transport Delay' (DWork 3)  */

  void *SFunction2_PWORK;              /* '<S14>/S-Function2' (DWork 1)  */
  void *OpIPSocketCtrl1_PWORK;         /* '<S3>/OpIPSocketCtrl1' (DWork 1)  */
  int32_T systemEnable;                /* '<S157>/Sine Wave A' (DWork 1)  */
  int32_T systemEnable_n;              /* '<S157>/Sine Wave B' (DWork 1)  */
  int32_T systemEnable_j;              /* '<S157>/Sine Wave C' (DWork 1)  */
  int32_T clockTickCounter;           /* '<S3>/data ready 2.5 kHz' (DWork 1)  */
  int_T Integrator1_IWORK;             /* '<S51>/Integrator1' (DWork 1)  */
  int_T SFunction_IWORK[5];            /* '<S161>/S-Function' (DWork 1)  */
  int_T Integrator2_IWORK;             /* '<S130>/Integrator2' (DWork 1)  */
  struct {
    int_T Tail[8];
    int_T Head[8];
    int_T Last[8];
    int_T CircularBufSize[8];
  } TransportDelay_IWORK;              /* '<S136>/Transport Delay' (DWork 2)  */

  struct {
    int_T Tail[8];
    int_T Head[8];
    int_T Last[8];
    int_T CircularBufSize[8];
  } TransportDelay_IWORK_j;            /* '<S138>/Transport Delay' (DWork 2)  */

  struct {
    int_T Tail[8];
    int_T Head[8];
    int_T Last[8];
    int_T CircularBufSize[8];
  } TransportDelay_IWORK_b;            /* '<S140>/Transport Delay' (DWork 2)  */

  struct {
    int_T Tail[8];
    int_T Head[8];
    int_T Last[8];
    int_T CircularBufSize[8];
  } TransportDelay_IWORK_k;            /* '<S135>/Transport Delay' (DWork 2)  */

  struct {
    int_T Tail[8];
    int_T Head[8];
    int_T Last[8];
    int_T CircularBufSize[8];
  } TransportDelay_IWORK_l;            /* '<S137>/Transport Delay' (DWork 2)  */

  struct {
    int_T Tail[8];
    int_T Head[8];
    int_T Last[8];
    int_T CircularBufSize[8];
  } TransportDelay_IWORK_a;            /* '<S139>/Transport Delay' (DWork 2)  */

  uint8_T Integrator_SYSTEM_ENABLE;    /* '<S57>/Integrator' (DWork 2)  */
  uint8_T Integrator_SYSTEM_ENABLE_p;  /* '<S59>/Integrator' (DWork 2)  */
  uint8_T Integrator_SYSTEM_ENABLE_i;  /* '<S61>/Integrator' (DWork 2)  */
  uint8_T Integrator_SYSTEM_ENABLE_h;  /* '<S56>/Integrator' (DWork 2)  */
  uint8_T Integrator_SYSTEM_ENABLE_iy; /* '<S58>/Integrator' (DWork 2)  */
  uint8_T Integrator_SYSTEM_ENABLE_a;  /* '<S60>/Integrator' (DWork 2)  */
  boolean_T Initial_FirstOutputTime;   /* '<S75>/Initial' (DWork 1)  */
  DW_Tail_basic_cosimulation_c__T Tail_e;/* '<S116>/Tail' */
  DW_Tail_basic_cosimulation_c__T Tail;/* '<S37>/Tail' */
} DW_basic_cosimulation_c_1_sm_master_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator1_CSTATE;           /* '<S51>/Integrator1' */
  real_T Integrator_CSTATE;            /* '<S51>/Integrator' */
  real_T Integrator2_CSTATE;           /* '<S130>/Integrator2' */
  real_T Integrator_CSTATE_h[8];       /* '<S136>/Integrator' */
  real_T Integrator_CSTATE_a[8];       /* '<S138>/Integrator' */
  real_T Integrator_CSTATE_d[8];       /* '<S140>/Integrator' */
  real_T Integrator_CSTATE_g[8];       /* '<S135>/Integrator' */
  real_T Integrator_CSTATE_m[8];       /* '<S137>/Integrator' */
  real_T Integrator_CSTATE_ho[8];      /* '<S139>/Integrator' */
  real_T Integrator_CSTATE_b[4];       /* '<S147>/Integrator' */
  real_T Integrator_CSTATE_hp;         /* '<S130>/Integrator' */
} X_basic_cosimulation_c_1_sm_master_T;

/* Periodic continuous state vector (global) */
typedef int_T PeriodicIndX_basic_cosimulation_c_1_sm_master_T[4];
typedef real_T PeriodicRngX_basic_cosimulation_c_1_sm_master_T[8];

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator1_CSTATE;           /* '<S51>/Integrator1' */
  real_T Integrator_CSTATE;            /* '<S51>/Integrator' */
  real_T Integrator2_CSTATE;           /* '<S130>/Integrator2' */
  real_T Integrator_CSTATE_h[8];       /* '<S136>/Integrator' */
  real_T Integrator_CSTATE_a[8];       /* '<S138>/Integrator' */
  real_T Integrator_CSTATE_d[8];       /* '<S140>/Integrator' */
  real_T Integrator_CSTATE_g[8];       /* '<S135>/Integrator' */
  real_T Integrator_CSTATE_m[8];       /* '<S137>/Integrator' */
  real_T Integrator_CSTATE_ho[8];      /* '<S139>/Integrator' */
  real_T Integrator_CSTATE_b[4];       /* '<S147>/Integrator' */
  real_T Integrator_CSTATE_hp;         /* '<S130>/Integrator' */
} XDot_basic_cosimulation_c_1_sm_master_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator1_CSTATE;        /* '<S51>/Integrator1' */
  boolean_T Integrator_CSTATE;         /* '<S51>/Integrator' */
  boolean_T Integrator2_CSTATE;        /* '<S130>/Integrator2' */
  boolean_T Integrator_CSTATE_h[8];    /* '<S136>/Integrator' */
  boolean_T Integrator_CSTATE_a[8];    /* '<S138>/Integrator' */
  boolean_T Integrator_CSTATE_d[8];    /* '<S140>/Integrator' */
  boolean_T Integrator_CSTATE_g[8];    /* '<S135>/Integrator' */
  boolean_T Integrator_CSTATE_m[8];    /* '<S137>/Integrator' */
  boolean_T Integrator_CSTATE_ho[8];   /* '<S139>/Integrator' */
  boolean_T Integrator_CSTATE_b[4];    /* '<S147>/Integrator' */
  boolean_T Integrator_CSTATE_hp;      /* '<S130>/Integrator' */
} XDis_basic_cosimulation_c_1_sm_master_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState Integrator1_Reset_ZCE;    /* '<S51>/Integrator1' */
  ZCSigState Integrator2_Reset_ZCE;    /* '<S130>/Integrator2' */
} PrevZCX_basic_cosimulation_c_1_sm_master_T;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* Backward compatible GRT Identifiers */
#define rtB                            basic_cosimulation_c_1_sm_master_B
#define BlockIO                        B_basic_cosimulation_c_1_sm_master_T
#define rtX                            basic_cosimulation_c_1_sm_master_X
#define ContinuousStates               X_basic_cosimulation_c_1_sm_master_T
#define rtXdot                         basic_cosimulation_c_1_sm_master_XDot
#define StateDerivatives               XDot_basic_cosimulation_c_1_sm_master_T
#define tXdis                          basic_cosimulation_c_1_sm_master_XDis
#define StateDisabled                  XDis_basic_cosimulation_c_1_sm_master_T
#define rtP                            basic_cosimulation_c_1_sm_master_P
#define Parameters                     P_basic_cosimulation_c_1_sm_master_T
#define rtDWork                        basic_cosimulation_c_1_sm_master_DW
#define D_Work                         DW_basic_cosimulation_c_1_sm_master_T
#define rtPrevZCSigState               basic_cosimulation_c_1_sm_master_PrevZCX
#define PrevZCSigStates                PrevZCX_basic_cosimulation_c_1_sm_master_T

/* Parameters for system: '<S37>/Tail' */
struct P_Tail_basic_cosimulation_c_1_T_ {
  real_T itail_Y0;                     /* Expression: 0
                                        * Referenced by: '<S38>/itail' (Parameter: InitialOutput)
                                        */
  real_T u_Value;                      /* Expression: 1
                                        * Referenced by: '<S38>/1' (Parameter: Value)
                                        */
  real_T u_Value_f;                    /* Computed Parameter: u_Value_f
                                        * Referenced by: '<S38>/2' (Parameter: Value)
                                        */
  real_T DiscreteTimeIntegrator_gainval;
                           /* Computed Parameter: DiscreteTimeIntegrator_gainval
                            * Referenced by: '<S38>/Discrete-Time Integrator' (Parameter: gainval)
                            */
  real_T DiscreteTimeIntegrator_IC;    /* Expression: 0
                                        * Referenced by: '<S38>/Discrete-Time Integrator' (Parameter: InitialCondition)
                                        */
  real_T Constant_Value;               /* Expression: 0.9
                                        * Referenced by: '<S41>/Constant' (Parameter: Value)
                                        */
  real_T Saturation1_UpperSat;         /* Expression: 0.9
                                        * Referenced by: '<S41>/Saturation1' (Parameter: UpperLimit)
                                        */
  real_T Saturation1_LowerSat;         /* Expression: 0
                                        * Referenced by: '<S41>/Saturation1' (Parameter: LowerLimit)
                                        */
  real_T Saturation2_UpperSat;         /* Expression: 0.1
                                        * Referenced by: '<S41>/Saturation2' (Parameter: UpperLimit)
                                        */
  real_T Saturation2_LowerSat;         /* Expression: 0
                                        * Referenced by: '<S41>/Saturation2' (Parameter: LowerLimit)
                                        */
  real_T UnitDelay_InitialCondition;   /* Expression: 0
                                        * Referenced by: '<S38>/Unit Delay' (Parameter: InitialCondition)
                                        */
  real_T Switch_Threshold;             /* Expression: 0.5
                                        * Referenced by: '<S38>/Switch' (Parameter: Threshold)
                                        */
};

/* Parameters (default storage) */
struct P_basic_cosimulation_c_1_sm_master_T_ {
  real_T Inom_peak;                    /* Variable: Inom_peak
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
  real_T Ts;                           /* Variable: Ts
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
  real_T Vdc;                          /* Variable: Vdc
                                        * Referenced by:
                                        *   '<S45>/Constant2' (Parameter: Value)
                                        *   '<S86>/DC' (Parameter: Value)
                                        *   '<S124>/Constant2' (Parameter: Value)
                                        *   '<S152>/DC' (Parameter: Value)
                                        */
  real_T Vnom_peak;                    /* Variable: Vnom_peak
                                        * Referenced by:
                                        *   '<S49>/Constant' (Parameter: Value)
                                        *   '<S51>/Gain' (Parameter: Gain)
                                        *   '<S128>/Constant' (Parameter: Value)
                                        *   '<S130>/Gain' (Parameter: Gain)
                                        */
  real_T decouple;                     /* Variable: decouple
                                        * Referenced by:
                                        *   '<S45>/Gain5' (Parameter: Gain)
                                        *   '<S45>/Gain6' (Parameter: Gain)
                                        *   '<S124>/Gain5' (Parameter: Gain)
                                        *   '<S124>/Gain6' (Parameter: Gain)
                                        */
  real_T fsw_inv;                      /* Variable: fsw_inv
                                        * Referenced by:
                                        *   '<S26>/Constant11' (Parameter: Value)
                                        *   '<S107>/Constant11' (Parameter: Value)
                                        */
  real_T ki_current;                   /* Variable: ki_current
                                        * Referenced by:
                                        *   '<S45>/Gain2' (Parameter: Gain)
                                        *   '<S45>/Gain4' (Parameter: Gain)
                                        *   '<S124>/Gain2' (Parameter: Gain)
                                        *   '<S124>/Gain4' (Parameter: Gain)
                                        */
  real_T kp_current;                   /* Variable: kp_current
                                        * Referenced by:
                                        *   '<S45>/Gain1' (Parameter: Gain)
                                        *   '<S45>/Gain3' (Parameter: Gain)
                                        *   '<S124>/Gain1' (Parameter: Gain)
                                        *   '<S124>/Gain3' (Parameter: Gain)
                                        */
  real_T PowerMeasurementThreePhase_F;
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
  real_T PowerMeasurementThreePhase_F_f;
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
  real_T ThreePhaseSource_Frequency;
                                   /* Mask Parameter: ThreePhaseSource_Frequency
                                    * Referenced by:
                                    *   '<S157>/Sine Wave A' (Parameter: Frequency)
                                    *   '<S157>/Sine Wave B' (Parameter: Frequency)
                                    *   '<S157>/Sine Wave C' (Parameter: Frequency)
                                    */
  real_T PowerMeasurementThreePhase_K;
                                 /* Mask Parameter: PowerMeasurementThreePhase_K
                                  * Referenced by: '<S64>/Radial frequencies' (Parameter: Value)
                                  */
  real_T PowerMeasurementThreePhase_K_j[4];
                               /* Mask Parameter: PowerMeasurementThreePhase_K_j
                                * Referenced by: '<S143>/Radial frequencies' (Parameter: Value)
                                */
  real_T Tail_Tf;                      /* Mask Parameter: Tail_Tf
                                        * Referenced by: '<S37>/Tail' (Parameter: System parameter #0)
                                        */
  real_T Tail_Tf_a;                    /* Mask Parameter: Tail_Tf_a
                                        * Referenced by: '<S116>/Tail' (Parameter: System parameter #2)
                                        */
  real_T Tail_Tt;                      /* Mask Parameter: Tail_Tt
                                        * Referenced by: '<S37>/Tail' (Parameter: System parameter #1)
                                        */
  real_T Tail_Tt_m;                    /* Mask Parameter: Tail_Tt_m
                                        * Referenced by: '<S116>/Tail' (Parameter: System parameter #3)
                                        */
  real_T IntegratorwithWrappedStateDiscr;
                              /* Mask Parameter: IntegratorwithWrappedStateDiscr
                               * Referenced by:
                               *   '<S76>/Constant' (Parameter: Value)
                               *   '<S78>/Bias' (Parameter: Bias)
                               *   '<S78>/Gain' (Parameter: Gain)
                               *   '<S78>/Gain1' (Parameter: Gain)
                               */
  real_T IntegratorwithWrappedStateDis_j;
                              /* Mask Parameter: IntegratorwithWrappedStateDis_j
                               * Referenced by:
                               *   '<S77>/Constant' (Parameter: Value)
                               *   '<S78>/Gain' (Parameter: Gain)
                               *   '<S78>/Gain1' (Parameter: Gain)
                               */
  real_T Phasor_to_Time_Domain_SFA_offse;
                              /* Mask Parameter: Phasor_to_Time_Domain_SFA_offse
                               * Referenced by: '<S27>/Phase// Frequency Offset' (Parameter: Freq)
                               */
  real_T Time_Domain_To_Phasor_SFA_offse;
                              /* Mask Parameter: Time_Domain_To_Phasor_SFA_offse
                               * Referenced by: '<S30>/Phase// Frequency Offset' (Parameter: Freq)
                               */
  real_T IntegratorwithWrappedStateDis_a;
                              /* Mask Parameter: IntegratorwithWrappedStateDis_a
                               * Referenced by: '<S75>/Initial' (Parameter: Value)
                               */
  real_T IntegratorwithWrappedStateDis_h;
                              /* Mask Parameter: IntegratorwithWrappedStateDis_h
                               * Referenced by: '<S147>/Integrator' (Parameter: InitialCondition)
                               */
  real_T Gain1_Gain;                   /* Expression: 1/3
                                        * Referenced by: '<S62>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_p;                 /* Expression: 1/3
                                        * Referenced by: '<S63>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain2_Gain;                   /* Expression: 3/2
                                        * Referenced by: '<S53>/Gain2' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_a;                 /* Expression: 1/3
                                        * Referenced by: '<S54>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_px;                /* Expression: 1/3
                                        * Referenced by: '<S55>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_p3;                /* Expression: 3/2
                                        * Referenced by: '<S53>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_f;                 /* Expression: 1/3
                                        * Referenced by: '<S65>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_a0;                /* Expression: 1/3
                                        * Referenced by: '<S66>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain3_Gain;                   /* Expression: 3/2
                                        * Referenced by: '<S53>/Gain3' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_k;                 /* Expression: 1/3
                                        * Referenced by: '<S141>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_m;                 /* Expression: 1/3
                                        * Referenced by: '<S142>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain2_Gain_f;                 /* Expression: 3/2
                                        * Referenced by: '<S132>/Gain2' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_c;                 /* Expression: 1/3
                                        * Referenced by: '<S133>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_j;                 /* Expression: 1/3
                                        * Referenced by: '<S134>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_b;                 /* Expression: 3/2
                                        * Referenced by: '<S132>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_i;                 /* Expression: 1/3
                                        * Referenced by: '<S144>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_o;                 /* Expression: 1/3
                                        * Referenced by: '<S145>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain3_Gain_g;                 /* Expression: 3/2
                                        * Referenced by: '<S132>/Gain3' (Parameter: Gain)
                                        */
  real_T SFunction1_Value;             /* Expression: 0
                                        * Referenced by: '<S2>/S-Function1' (Parameter: Value)
                                        */
  real_T SFunction_InitialCondition;   /* Expression: 0
                                        * Referenced by: '<S2>/S-Function' (Parameter: InitialCondition)
                                        */
  real_T timeout_Value;                /* Expression: 2
                                        * Referenced by: '<S3>/timeout' (Parameter: Value)
                                        */
  real_T SFunction1_P1_Size[2];        /* Computed Parameter: SFunction1_P1_Size
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P1Size)
                                        */
  real_T SFunction1_P1;                /* Expression: ctl_id
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P1)
                                        */
  real_T SFunction1_P2_Size[2];        /* Computed Parameter: SFunction1_P2_Size
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P2Size)
                                        */
  real_T SFunction1_P2;                /* Expression: recv_id
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P2)
                                        */
  real_T SFunction1_P3_Size[2];        /* Computed Parameter: SFunction1_P3_Size
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P3Size)
                                        */
  real_T SFunction1_P3;                /* Expression: fp1
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P3)
                                        */
  real_T SFunction1_P4_Size[2];        /* Computed Parameter: SFunction1_P4_Size
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P4Size)
                                        */
  real_T SFunction1_P4;                /* Expression: fp2
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P4)
                                        */
  real_T SFunction1_P5_Size[2];        /* Computed Parameter: SFunction1_P5_Size
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P5Size)
                                        */
  real_T SFunction1_P5;                /* Expression: fp3
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P5)
                                        */
  real_T SFunction1_P6_Size[2];        /* Computed Parameter: SFunction1_P6_Size
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P6Size)
                                        */
  real_T SFunction1_P6;                /* Expression: fp4
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P6)
                                        */
  real_T SFunction1_P7_Size[2];        /* Computed Parameter: SFunction1_P7_Size
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P7Size)
                                        */
  real_T SFunction1_P7;                /* Expression: fp5
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P7)
                                        */
  real_T SFunction1_P8_Size[2];        /* Computed Parameter: SFunction1_P8_Size
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P8Size)
                                        */
  real_T SFunction1_P8[7];             /* Computed Parameter: SFunction1_P8
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P8)
                                        */
  real_T SFunction1_P9_Size[2];        /* Computed Parameter: SFunction1_P9_Size
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P9Size)
                                        */
  real_T SFunction1_P9[7];             /* Computed Parameter: SFunction1_P9
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P9)
                                        */
  real_T SFunction1_P10_Size[2];      /* Computed Parameter: SFunction1_P10_Size
                                       * Referenced by: '<S6>/S-Function1' (Parameter: P10Size)
                                       */
  real_T SFunction1_P10[7];            /* Computed Parameter: SFunction1_P10
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P10)
                                        */
  real_T SFunction1_P11_Size[2];      /* Computed Parameter: SFunction1_P11_Size
                                       * Referenced by: '<S6>/S-Function1' (Parameter: P11Size)
                                       */
  real_T SFunction1_P11[7];            /* Computed Parameter: SFunction1_P11
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P11)
                                        */
  real_T SFunction1_P12_Size[2];      /* Computed Parameter: SFunction1_P12_Size
                                       * Referenced by: '<S6>/S-Function1' (Parameter: P12Size)
                                       */
  real_T SFunction1_P12[7];            /* Computed Parameter: SFunction1_P12
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P12)
                                        */
  real_T SFunction1_P13_Size[2];      /* Computed Parameter: SFunction1_P13_Size
                                       * Referenced by: '<S6>/S-Function1' (Parameter: P13Size)
                                       */
  real_T SFunction1_P13;               /* Computed Parameter: SFunction1_P13
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P13)
                                        */
  real_T SFunction1_P14_Size[2];      /* Computed Parameter: SFunction1_P14_Size
                                       * Referenced by: '<S6>/S-Function1' (Parameter: P14Size)
                                       */
  real_T SFunction1_P14;               /* Computed Parameter: SFunction1_P14
                                        * Referenced by: '<S6>/S-Function1' (Parameter: P14)
                                        */
  real_T UnitDelay_InitialCondition;   /* Expression: 0
                                        * Referenced by: '<S37>/Unit Delay' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition;       /* Expression: 0.0
                                        * Referenced by: '<S45>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Constant2_Value;              /* Expression: 2*pi
                                        * Referenced by: '<S51>/Constant2' (Parameter: Value)
                                        */
  real_T Integrator1_UpperSat;         /* Expression: inf
                                        * Referenced by: '<S51>/Integrator1' (Parameter: UpperSaturationLimit)
                                        */
  real_T Integrator1_LowerSat;         /* Expression: -inf
                                        * Referenced by: '<S51>/Integrator1' (Parameter: LowerSaturationLimit)
                                        */
  real_T Gain_Gain;                    /* Expression: -1
                                        * Referenced by: '<S52>/Gain' (Parameter: Gain)
                                        */
  real_T Constant3_Value;              /* Expression: 2*pi/3
                                        * Referenced by: '<S52>/Constant3' (Parameter: Value)
                                        */
  real_T Constant1_Value;              /* Expression: 2*pi/3
                                        * Referenced by: '<S52>/Constant1' (Parameter: Value)
                                        */
  real_T Gain1_Gain_d;                 /* Expression: -1
                                        * Referenced by: '<S52>/Gain1' (Parameter: Gain)
                                        */
  real_T Constant4_Value;              /* Expression: 2*pi/3
                                        * Referenced by: '<S52>/Constant4' (Parameter: Value)
                                        */
  real_T Constant2_Value_m;            /* Expression: 2*pi/3
                                        * Referenced by: '<S52>/Constant2' (Parameter: Value)
                                        */
  real_T Gain2_Gain_e;                 /* Expression: -1
                                        * Referenced by: '<S52>/Gain2' (Parameter: Gain)
                                        */
  real_T SFunction_P1_Size[2];         /* Computed Parameter: SFunction_P1_Size
                                        * Referenced by: '<S96>/S-Function' (Parameter: P1Size)
                                        */
  real_T SFunction_P1;                 /* Expression: Data_width
                                        * Referenced by: '<S96>/S-Function' (Parameter: P1)
                                        */
  real_T SFunction_P2_Size[2];         /* Computed Parameter: SFunction_P2_Size
                                        * Referenced by: '<S96>/S-Function' (Parameter: P2Size)
                                        */
  real_T SFunction_P2[3];              /* Expression: InitialConditions
                                        * Referenced by: '<S96>/S-Function' (Parameter: P2)
                                        */
  real_T Saturation_UpperSat;          /* Expression: 1.1
                                        * Referenced by: '<S17>/Saturation' (Parameter: UpperLimit)
                                        */
  real_T Saturation_LowerSat;          /* Expression: -1.1
                                        * Referenced by: '<S17>/Saturation' (Parameter: LowerLimit)
                                        */
  real_T Constant9_Value;              /* Expression: 1
                                        * Referenced by: '<S26>/Constant9' (Parameter: Value)
                                        */
  real_T Constant10_Value;             /* Expression: 2
                                        * Referenced by: '<S26>/Constant10' (Parameter: Value)
                                        */
  real_T RTESPWM1_P1_Size[2];          /* Computed Parameter: RTESPWM1_P1_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P1Size)
                                        */
  real_T RTESPWM1_P1;                  /* Expression: MaxEvents
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P1)
                                        */
  real_T RTESPWM1_P2_Size[2];          /* Computed Parameter: RTESPWM1_P2_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P2Size)
                                        */
  real_T RTESPWM1_P2;                  /* Expression: SampleTime
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P2)
                                        */
  real_T RTESPWM1_P3_Size[2];          /* Computed Parameter: RTESPWM1_P3_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P3Size)
                                        */
  real_T RTESPWM1_P3;                  /* Expression: MaxFreq
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P3)
                                        */
  real_T RTESPWM1_P4_Size[2];          /* Computed Parameter: RTESPWM1_P4_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P4Size)
                                        */
  real_T RTESPWM1_P4;                  /* Expression: MinFreq
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P4)
                                        */
  real_T RTESPWM1_P5_Size[2];          /* Computed Parameter: RTESPWM1_P5_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P5Size)
                                        */
  real_T RTESPWM1_P5;                  /* Expression: EnablingPort
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P5)
                                        */
  real_T RTESPWM1_P6_Size[2];          /* Computed Parameter: RTESPWM1_P6_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P6Size)
                                        */
  real_T RTESPWM1_P6;                  /* Expression: NumberPhases
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P6)
                                        */
  real_T RTESPWM1_P7_Size[2];          /* Computed Parameter: RTESPWM1_P7_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P7Size)
                                        */
  real_T RTESPWM1_P7;                  /* Expression: ComplementaryMode
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P7)
                                        */
  real_T RTESPWM1_P8_Size[2];          /* Computed Parameter: RTESPWM1_P8_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P8Size)
                                        */
  real_T RTESPWM1_P8;                  /* Expression: RiseTimeDelay
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P8)
                                        */
  real_T RTESPWM1_P9_Size[2];          /* Computed Parameter: RTESPWM1_P9_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P9Size)
                                        */
  real_T RTESPWM1_P9;                  /* Expression: CenterAlignmentMode
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P9)
                                        */
  real_T RTESPWM1_P10_Size[2];         /* Computed Parameter: RTESPWM1_P10_Size
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P10Size)
                                        */
  real_T RTESPWM1_P10;                 /* Expression: SpaceVector
                                        * Referenced by: '<S26>/RTE SPWM1' (Parameter: P10)
                                        */
  real_T RTEConversion1_P1_Size[2];/* Computed Parameter: RTEConversion1_P1_Size
                                    * Referenced by: '<S26>/RTE Conversion1' (Parameter: P1Size)
                                    */
  real_T RTEConversion1_P1;            /* Expression: nbMaxEvents
                                        * Referenced by: '<S26>/RTE Conversion1' (Parameter: P1)
                                        */
  real_T RTEConversion1_P2_Size[2];/* Computed Parameter: RTEConversion1_P2_Size
                                    * Referenced by: '<S26>/RTE Conversion1' (Parameter: P2Size)
                                    */
  real_T RTEConversion1_P2;            /* Expression: inputdatatype
                                        * Referenced by: '<S26>/RTE Conversion1' (Parameter: P2)
                                        */
  real_T RTEConversion1_P3_Size[2];/* Computed Parameter: RTEConversion1_P3_Size
                                    * Referenced by: '<S26>/RTE Conversion1' (Parameter: P3Size)
                                    */
  real_T RTEConversion1_P3;            /* Expression: outputdatatype
                                        * Referenced by: '<S26>/RTE Conversion1' (Parameter: P3)
                                        */
  real_T RTEConversion1_P4_Size[2];/* Computed Parameter: RTEConversion1_P4_Size
                                    * Referenced by: '<S26>/RTE Conversion1' (Parameter: P4Size)
                                    */
  real_T RTEConversion1_P4;            /* Expression: compensation
                                        * Referenced by: '<S26>/RTE Conversion1' (Parameter: P4)
                                        */
  real_T RTEConversion1_P5_Size[2];/* Computed Parameter: RTEConversion1_P5_Size
                                    * Referenced by: '<S26>/RTE Conversion1' (Parameter: P5Size)
                                    */
  real_T RTEConversion1_P5;            /* Expression: sampleTime
                                        * Referenced by: '<S26>/RTE Conversion1' (Parameter: P5)
                                        */
  real_T NotinARTEMIS_Value;           /* Expression: 0
                                        * Referenced by: '<S37>/Not in ARTEMIS' (Parameter: Value)
                                        */
  real_T Gain_Gain_e;                  /* Expression: pi/180
                                        * Referenced by: '<S27>/Gain' (Parameter: Gain)
                                        */
  real_T PhaseFrequencyOffset_Phase;
                               /* Computed Parameter: PhaseFrequencyOffset_Phase
                                * Referenced by: '<S27>/Phase// Frequency Offset' (Parameter: Phase)
                                */
  real_T VfDevicesClampingDiodes_Value[6];/* Expression: Vf_SwitchOn
                                           * Referenced by: '<S39>/Vf Devices & Clamping Diodes' (Parameter: Value)
                                           */
  real_T VfDiodes_Value[6];            /* Expression: Vf_DiodeOn
                                        * Referenced by: '<S39>/Vf Diodes' (Parameter: Value)
                                        */
  real_T StateSpace_P1_Size[2];        /* Computed Parameter: StateSpace_P1_Size
                                        * Referenced by: '<S162>/State-Space' (Parameter: P1Size)
                                        */
  real_T StateSpace_P1;                /* Expression: 0
                                        * Referenced by: '<S162>/State-Space' (Parameter: P1)
                                        */
  real_T StateSpace_P2_Size[2];        /* Computed Parameter: StateSpace_P2_Size
                                        * Referenced by: '<S162>/State-Space' (Parameter: P2Size)
                                        */
  real_T StateSpace_P2;                /* Expression: 0
                                        * Referenced by: '<S162>/State-Space' (Parameter: P2)
                                        */
  real_T donotdeletethisgain_Gain;     /* Expression: 1
                                        * Referenced by: '<S21>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_p;   /* Expression: 1
                                        * Referenced by: '<S22>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_a;   /* Expression: 1
                                        * Referenced by: '<S23>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T PhaseFrequencyOffset_Phase_p;
                             /* Computed Parameter: PhaseFrequencyOffset_Phase_p
                              * Referenced by: '<S30>/Phase// Frequency Offset' (Parameter: Phase)
                              */
  real_T Gain1_Gain_kr;                /* Expression: 180/pi
                                        * Referenced by: '<S30>/Gain1' (Parameter: Gain)
                                        */
  real_T Delay_InitialCondition_b;     /* Expression: 0.0
                                        * Referenced by: '<S5>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Gain6_Gain;                   /* Expression: 2
                                        * Referenced by: '<S46>/Gain6' (Parameter: Gain)
                                        */
  real_T Delay1_InitialCondition;      /* Expression: 0.0
                                        * Referenced by: '<S5>/Delay1' (Parameter: InitialCondition)
                                        */
  real_T Gain7_Gain;                   /* Expression: 1/3
                                        * Referenced by: '<S46>/Gain7' (Parameter: Gain)
                                        */
  real_T Gain8_Gain;                   /* Expression: 1/3
                                        * Referenced by: '<S46>/Gain8' (Parameter: Gain)
                                        */
  real_T Gain10_Gain;                  /* Expression: 2
                                        * Referenced by: '<S46>/Gain10' (Parameter: Gain)
                                        */
  real_T Gain9_Gain;                   /* Expression: 1/3
                                        * Referenced by: '<S46>/Gain9' (Parameter: Gain)
                                        */
  real_T Delay2_InitialCondition;      /* Expression: 0.0
                                        * Referenced by: '<S8>/Delay2' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_l;     /* Expression: 0.0
                                        * Referenced by: '<S8>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Gain6_Gain_p;                 /* Expression: 2
                                        * Referenced by: '<S125>/Gain6' (Parameter: Gain)
                                        */
  real_T Delay1_InitialCondition_c;    /* Expression: 0.0
                                        * Referenced by: '<S8>/Delay1' (Parameter: InitialCondition)
                                        */
  real_T Gain7_Gain_d;                 /* Expression: 1/3
                                        * Referenced by: '<S125>/Gain7' (Parameter: Gain)
                                        */
  real_T Gain8_Gain_j;                 /* Expression: 1/3
                                        * Referenced by: '<S125>/Gain8' (Parameter: Gain)
                                        */
  real_T Gain10_Gain_a;                /* Expression: 2
                                        * Referenced by: '<S125>/Gain10' (Parameter: Gain)
                                        */
  real_T Gain9_Gain_n;                 /* Expression: 1/3
                                        * Referenced by: '<S125>/Gain9' (Parameter: Gain)
                                        */
  real_T In1_Value;                    /* Expression: 0
                                        * Referenced by: '<S10>/In1' (Parameter: Value)
                                        */
  real_T In1_Value_d;                  /* Expression: 0
                                        * Referenced by: '<S9>/In1' (Parameter: Value)
                                        */
  real_T In1_Value_a;                  /* Expression: 0
                                        * Referenced by: '<S12>/In1' (Parameter: Value)
                                        */
  real_T In1_Value_dr;                 /* Expression: 0
                                        * Referenced by: '<S11>/In1' (Parameter: Value)
                                        */
  real_T In1_Value_i;                  /* Expression: 0
                                        * Referenced by: '<S15>/In1' (Parameter: Value)
                                        */
  real_T SFunction_P1_Size_j[2];      /* Computed Parameter: SFunction_P1_Size_j
                                       * Referenced by: '<S161>/S-Function' (Parameter: P1Size)
                                       */
  real_T SFunction_P1_b;               /* Expression: Acqu_group
                                        * Referenced by: '<S161>/S-Function' (Parameter: P1)
                                        */
  real_T u4_Value;                     /* Expression: 0
                                        * Referenced by: '<S37>/0 4' (Parameter: Value)
                                        */
  real_T uRon_Gain;                    /* Expression: 1./Ron
                                        * Referenced by: '<S37>/1//Ron' (Parameter: Gain)
                                        */
  real_T Switch_Threshold;             /* Expression: 0.5
                                        * Referenced by: '<S37>/Switch' (Parameter: Threshold)
                                        */
  real_T Saturation_UpperSat_c;        /* Expression: inf
                                        * Referenced by: '<S37>/Saturation' (Parameter: UpperLimit)
                                        */
  real_T Saturation_LowerSat_l;        /* Expression: 0
                                        * Referenced by: '<S37>/Saturation' (Parameter: LowerLimit)
                                        */
  real_T donotdeletethisgain_Gain_o;   /* Expression: 1
                                        * Referenced by: '<S34>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_oq;  /* Expression: 1
                                        * Referenced by: '<S35>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_n;   /* Expression: 1
                                        * Referenced by: '<S36>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T Delay2_InitialCondition_i;    /* Expression: 0.0
                                        * Referenced by: '<S5>/Delay2' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_i;     /* Expression: 0.0
                                        * Referenced by: '<S47>/Delay' (Parameter: InitialCondition)
                                        */
  real_T DiscreteTimeIntegrator_gainval;
                           /* Computed Parameter: DiscreteTimeIntegrator_gainval
                            * Referenced by: '<S45>/Discrete-Time Integrator' (Parameter: gainval)
                            */
  real_T DiscreteTimeIntegrator_IC;    /* Expression: 0
                                        * Referenced by: '<S45>/Discrete-Time Integrator' (Parameter: InitialCondition)
                                        */
  real_T DiscreteTimeIntegrator1_gainval;
                          /* Computed Parameter: DiscreteTimeIntegrator1_gainval
                           * Referenced by: '<S45>/Discrete-Time Integrator1' (Parameter: gainval)
                           */
  real_T DiscreteTimeIntegrator1_IC;   /* Expression: 0
                                        * Referenced by: '<S45>/Discrete-Time Integrator1' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_a;     /* Expression: 0.0
                                        * Referenced by: '<S48>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Gain1_Gain_kd;                /* Expression: 2
                                        * Referenced by: '<S49>/Gain1' (Parameter: Gain)
                                        */
  real_T Switch_Threshold_l;           /* Expression: 0
                                        * Referenced by: '<S49>/Switch' (Parameter: Threshold)
                                        */
  real_T Gain_Gain_c;                  /* Expression: 3
                                        * Referenced by: '<S49>/Gain' (Parameter: Gain)
                                        */
  real_T Gain3_Gain_a;                 /* Expression: -2
                                        * Referenced by: '<S49>/Gain3' (Parameter: Gain)
                                        */
  real_T Gain2_Gain_n;                 /* Expression: 3
                                        * Referenced by: '<S49>/Gain2' (Parameter: Gain)
                                        */
  real_T Constant_Value;               /* Expression: 2*pi/3
                                        * Referenced by: '<S47>/Constant' (Parameter: Value)
                                        */
  real_T Constant1_Value_f;            /* Expression: 2*pi/3
                                        * Referenced by: '<S47>/Constant1' (Parameter: Value)
                                        */
  real_T Constant2_Value_c;            /* Expression: 2*pi/3
                                        * Referenced by: '<S47>/Constant2' (Parameter: Value)
                                        */
  real_T Constant3_Value_k;            /* Expression: 2*pi/3
                                        * Referenced by: '<S47>/Constant3' (Parameter: Value)
                                        */
  real_T Gain_Gain_em;                 /* Expression: 0.666
                                        * Referenced by: '<S47>/Gain' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_g;                 /* Expression: -0.666
                                        * Referenced by: '<S47>/Gain1' (Parameter: Gain)
                                        */
  real_T Constant_Value_k;             /* Expression: 2*pi/3
                                        * Referenced by: '<S48>/Constant' (Parameter: Value)
                                        */
  real_T Constant1_Value_l;            /* Expression: 2*pi/3
                                        * Referenced by: '<S48>/Constant1' (Parameter: Value)
                                        */
  real_T Constant2_Value_n;            /* Expression: 2*pi/3
                                        * Referenced by: '<S48>/Constant2' (Parameter: Value)
                                        */
  real_T Constant3_Value_a;            /* Expression: 2*pi/3
                                        * Referenced by: '<S48>/Constant3' (Parameter: Value)
                                        */
  real_T Gain_Gain_m;                  /* Expression: 0.666
                                        * Referenced by: '<S48>/Gain' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_oe;                /* Expression: -0.666
                                        * Referenced by: '<S48>/Gain1' (Parameter: Gain)
                                        */
  real_T Sequence_Value;               /* Expression: S
                                        * Referenced by: '<S53>/Sequence' (Parameter: Value)
                                        */
  real_T Integrator_gainval;           /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S75>/Integrator' (Parameter: gainval)
                                        */
  real_T Integrator_gainval_l;       /* Computed Parameter: Integrator_gainval_l
                                      * Referenced by: '<S57>/Integrator' (Parameter: gainval)
                                      */
  real_T Integrator_IC;                /* Expression: 0
                                        * Referenced by: '<S57>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay1_InitialCondition;  /* Expression: 0
                                        * Referenced by: '<S68>/Unit Delay1' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay2_InitialCondition;  /* Expression: 0
                                        * Referenced by: '<S68>/Unit Delay2' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_k;     /* Expression: 0.0
                                        * Referenced by: '<S68>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Gain_Gain_e0;                 /* Expression: 0.5
                                        * Referenced by: '<S68>/Gain' (Parameter: Gain)
                                        */
  real_T Integrator_gainval_b;       /* Computed Parameter: Integrator_gainval_b
                                      * Referenced by: '<S59>/Integrator' (Parameter: gainval)
                                      */
  real_T Integrator_IC_j;              /* Expression: 0
                                        * Referenced by: '<S59>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay1_InitialCondition_b;/* Expression: 0
                                        * Referenced by: '<S70>/Unit Delay1' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay2_InitialCondition_a;/* Expression: 0
                                        * Referenced by: '<S70>/Unit Delay2' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_lj;    /* Expression: 0.0
                                        * Referenced by: '<S70>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Gain_Gain_j;                  /* Expression: 0.5
                                        * Referenced by: '<S70>/Gain' (Parameter: Gain)
                                        */
  real_T Integrator_gainval_d;       /* Computed Parameter: Integrator_gainval_d
                                      * Referenced by: '<S61>/Integrator' (Parameter: gainval)
                                      */
  real_T Integrator_IC_a;              /* Expression: 0
                                        * Referenced by: '<S61>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay1_InitialCondition_d;/* Expression: 0
                                        * Referenced by: '<S72>/Unit Delay1' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay2_InitialCondition_b;/* Expression: 0
                                        * Referenced by: '<S72>/Unit Delay2' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_p;     /* Expression: 0.0
                                        * Referenced by: '<S72>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Gain_Gain_cw;                 /* Expression: 0.5
                                        * Referenced by: '<S72>/Gain' (Parameter: Gain)
                                        */
  real_T Integrator_gainval_c;       /* Computed Parameter: Integrator_gainval_c
                                      * Referenced by: '<S56>/Integrator' (Parameter: gainval)
                                      */
  real_T Integrator_IC_m;              /* Expression: 0
                                        * Referenced by: '<S56>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay1_InitialCondition_p;/* Expression: 0
                                        * Referenced by: '<S67>/Unit Delay1' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay2_InitialCondition_h;/* Expression: 0
                                        * Referenced by: '<S67>/Unit Delay2' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_o;     /* Expression: 0.0
                                        * Referenced by: '<S67>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Gain_Gain_a;                  /* Expression: 0.5
                                        * Referenced by: '<S67>/Gain' (Parameter: Gain)
                                        */
  real_T Integrator_gainval_h;       /* Computed Parameter: Integrator_gainval_h
                                      * Referenced by: '<S58>/Integrator' (Parameter: gainval)
                                      */
  real_T Integrator_IC_c;              /* Expression: 0
                                        * Referenced by: '<S58>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay1_InitialCondition_a;/* Expression: 0
                                        * Referenced by: '<S69>/Unit Delay1' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay2_InitialCondition_e;/* Expression: 0
                                        * Referenced by: '<S69>/Unit Delay2' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_kv;    /* Expression: 0.0
                                        * Referenced by: '<S69>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Gain_Gain_jx;                 /* Expression: 0.5
                                        * Referenced by: '<S69>/Gain' (Parameter: Gain)
                                        */
  real_T Integrator_gainval_i;       /* Computed Parameter: Integrator_gainval_i
                                      * Referenced by: '<S60>/Integrator' (Parameter: gainval)
                                      */
  real_T Integrator_IC_my;             /* Expression: 0
                                        * Referenced by: '<S60>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T UnitDelay1_InitialCondition_po;/* Expression: 0
                                         * Referenced by: '<S71>/Unit Delay1' (Parameter: InitialCondition)
                                         */
  real_T UnitDelay2_InitialCondition_k;/* Expression: 0
                                        * Referenced by: '<S71>/Unit Delay2' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_c;     /* Expression: 0.0
                                        * Referenced by: '<S71>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Gain_Gain_n;                  /* Expression: 0.5
                                        * Referenced by: '<S71>/Gain' (Parameter: Gain)
                                        */
  real_T Gain5_Gain;                   /* Expression: 0.866
                                        * Referenced by: '<S51>/Gain5' (Parameter: Gain)
                                        */
  real_T Gain6_Gain_p3;                /* Expression: -0.866
                                        * Referenced by: '<S51>/Gain6' (Parameter: Gain)
                                        */
  real_T Gain4_Gain;                   /* Expression: 0.666
                                        * Referenced by: '<S51>/Gain4' (Parameter: Gain)
                                        */
  real_T Gain2_Gain_i;                 /* Expression: -0.5
                                        * Referenced by: '<S51>/Gain2' (Parameter: Gain)
                                        */
  real_T Gain3_Gain_c;                 /* Expression: -0.5
                                        * Referenced by: '<S51>/Gain3' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_kw;                /* Expression: 0.666
                                        * Referenced by: '<S51>/Gain1' (Parameter: Gain)
                                        */
  real_T Constant_Value_p;             /* Expression: 2*pi*60
                                        * Referenced by: '<S51>/Constant' (Parameter: Value)
                                        */
  real_T Gain7_Gain_o;                 /* Expression: 92
                                        * Referenced by: '<S51>/Gain7' (Parameter: Gain)
                                        */
  real_T Integrator_IC_i;              /* Expression: 0
                                        * Referenced by: '<S51>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T donotdeletethisgain_Gain_h;   /* Expression: 1
                                        * Referenced by: '<S18>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_b;   /* Expression: 1
                                        * Referenced by: '<S19>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_e;   /* Expression: 1
                                        * Referenced by: '<S20>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_f;   /* Expression: 1
                                        * Referenced by: '<S31>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_ow;  /* Expression: 1
                                        * Referenced by: '<S32>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_ht;  /* Expression: 1
                                        * Referenced by: '<S24>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T u4_Value_f;                   /* Expression: 0
                                        * Referenced by: '<S116>/0 4' (Parameter: Value)
                                        */
  real_T UnitDelay_InitialCondition_a; /* Expression: 0
                                        * Referenced by: '<S116>/Unit Delay' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_f;     /* Expression: 0.0
                                        * Referenced by: '<S124>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Constant2_Value_o;            /* Expression: 2*pi
                                        * Referenced by: '<S130>/Constant2' (Parameter: Value)
                                        */
  real_T Integrator2_UpperSat;         /* Expression: inf
                                        * Referenced by: '<S130>/Integrator2' (Parameter: UpperSaturationLimit)
                                        */
  real_T Integrator2_LowerSat;         /* Expression: -inf
                                        * Referenced by: '<S130>/Integrator2' (Parameter: LowerSaturationLimit)
                                        */
  real_T Gain_Gain_o;                  /* Expression: -1
                                        * Referenced by: '<S131>/Gain' (Parameter: Gain)
                                        */
  real_T Constant3_Value_h;            /* Expression: 2*pi/3
                                        * Referenced by: '<S131>/Constant3' (Parameter: Value)
                                        */
  real_T Constant1_Value_g;            /* Expression: 2*pi/3
                                        * Referenced by: '<S131>/Constant1' (Parameter: Value)
                                        */
  real_T Gain1_Gain_gz;                /* Expression: -1
                                        * Referenced by: '<S131>/Gain1' (Parameter: Gain)
                                        */
  real_T Constant4_Value_n;            /* Expression: 2*pi/3
                                        * Referenced by: '<S131>/Constant4' (Parameter: Value)
                                        */
  real_T Constant2_Value_e;            /* Expression: 2*pi/3
                                        * Referenced by: '<S131>/Constant2' (Parameter: Value)
                                        */
  real_T Gain2_Gain_o;                 /* Expression: -1
                                        * Referenced by: '<S131>/Gain2' (Parameter: Gain)
                                        */
  real_T Saturation_UpperSat_l;        /* Expression: 1.1
                                        * Referenced by: '<S101>/Saturation' (Parameter: UpperLimit)
                                        */
  real_T Saturation_LowerSat_f;        /* Expression: -1.1
                                        * Referenced by: '<S101>/Saturation' (Parameter: LowerLimit)
                                        */
  real_T Constant9_Value_k;            /* Expression: 1
                                        * Referenced by: '<S107>/Constant9' (Parameter: Value)
                                        */
  real_T Constant10_Value_l;           /* Expression: 2
                                        * Referenced by: '<S107>/Constant10' (Parameter: Value)
                                        */
  real_T RTESPWM1_P1_Size_f[2];        /* Computed Parameter: RTESPWM1_P1_Size_f
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P1Size)
                                        */
  real_T RTESPWM1_P1_l;                /* Expression: MaxEvents
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P1)
                                        */
  real_T RTESPWM1_P2_Size_n[2];        /* Computed Parameter: RTESPWM1_P2_Size_n
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P2Size)
                                        */
  real_T RTESPWM1_P2_j;                /* Expression: SampleTime
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P2)
                                        */
  real_T RTESPWM1_P3_Size_e[2];        /* Computed Parameter: RTESPWM1_P3_Size_e
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P3Size)
                                        */
  real_T RTESPWM1_P3_h;                /* Expression: MaxFreq
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P3)
                                        */
  real_T RTESPWM1_P4_Size_a[2];        /* Computed Parameter: RTESPWM1_P4_Size_a
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P4Size)
                                        */
  real_T RTESPWM1_P4_c;                /* Expression: MinFreq
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P4)
                                        */
  real_T RTESPWM1_P5_Size_f[2];        /* Computed Parameter: RTESPWM1_P5_Size_f
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P5Size)
                                        */
  real_T RTESPWM1_P5_d;                /* Expression: EnablingPort
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P5)
                                        */
  real_T RTESPWM1_P6_Size_a[2];        /* Computed Parameter: RTESPWM1_P6_Size_a
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P6Size)
                                        */
  real_T RTESPWM1_P6_k;                /* Expression: NumberPhases
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P6)
                                        */
  real_T RTESPWM1_P7_Size_l[2];        /* Computed Parameter: RTESPWM1_P7_Size_l
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P7Size)
                                        */
  real_T RTESPWM1_P7_f;                /* Expression: ComplementaryMode
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P7)
                                        */
  real_T RTESPWM1_P8_Size_k[2];        /* Computed Parameter: RTESPWM1_P8_Size_k
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P8Size)
                                        */
  real_T RTESPWM1_P8_h;                /* Expression: RiseTimeDelay
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P8)
                                        */
  real_T RTESPWM1_P9_Size_k[2];        /* Computed Parameter: RTESPWM1_P9_Size_k
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P9Size)
                                        */
  real_T RTESPWM1_P9_n;                /* Expression: CenterAlignmentMode
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P9)
                                        */
  real_T RTESPWM1_P10_Size_b[2];      /* Computed Parameter: RTESPWM1_P10_Size_b
                                       * Referenced by: '<S107>/RTE SPWM1' (Parameter: P10Size)
                                       */
  real_T RTESPWM1_P10_d;               /* Expression: SpaceVector
                                        * Referenced by: '<S107>/RTE SPWM1' (Parameter: P10)
                                        */
  real_T RTEConversion1_P1_Size_a[2];
                                 /* Computed Parameter: RTEConversion1_P1_Size_a
                                  * Referenced by: '<S107>/RTE Conversion1' (Parameter: P1Size)
                                  */
  real_T RTEConversion1_P1_o;          /* Expression: nbMaxEvents
                                        * Referenced by: '<S107>/RTE Conversion1' (Parameter: P1)
                                        */
  real_T RTEConversion1_P2_Size_g[2];
                                 /* Computed Parameter: RTEConversion1_P2_Size_g
                                  * Referenced by: '<S107>/RTE Conversion1' (Parameter: P2Size)
                                  */
  real_T RTEConversion1_P2_g;          /* Expression: inputdatatype
                                        * Referenced by: '<S107>/RTE Conversion1' (Parameter: P2)
                                        */
  real_T RTEConversion1_P3_Size_h[2];
                                 /* Computed Parameter: RTEConversion1_P3_Size_h
                                  * Referenced by: '<S107>/RTE Conversion1' (Parameter: P3Size)
                                  */
  real_T RTEConversion1_P3_b;          /* Expression: outputdatatype
                                        * Referenced by: '<S107>/RTE Conversion1' (Parameter: P3)
                                        */
  real_T RTEConversion1_P4_Size_j[2];
                                 /* Computed Parameter: RTEConversion1_P4_Size_j
                                  * Referenced by: '<S107>/RTE Conversion1' (Parameter: P4Size)
                                  */
  real_T RTEConversion1_P4_l;          /* Expression: compensation
                                        * Referenced by: '<S107>/RTE Conversion1' (Parameter: P4)
                                        */
  real_T RTEConversion1_P5_Size_c[2];
                                 /* Computed Parameter: RTEConversion1_P5_Size_c
                                  * Referenced by: '<S107>/RTE Conversion1' (Parameter: P5Size)
                                  */
  real_T RTEConversion1_P5_e;          /* Expression: sampleTime
                                        * Referenced by: '<S107>/RTE Conversion1' (Parameter: P5)
                                        */
  real_T NotinARTEMIS_Value_k;         /* Expression: 0
                                        * Referenced by: '<S116>/Not in ARTEMIS' (Parameter: Value)
                                        */
  real_T SineWaveA_Amp;                /* Expression: Aa
                                        * Referenced by: '<S157>/Sine Wave A' (Parameter: Amplitude)
                                        */
  real_T SineWaveA_Bias;               /* Expression: 0
                                        * Referenced by: '<S157>/Sine Wave A' (Parameter: Bias)
                                        */
  real_T SineWaveA_Hsin;               /* Computed Parameter: SineWaveA_Hsin
                                        * Referenced by: '<S157>/Sine Wave A' (Parameter: SinH)
                                        */
  real_T SineWaveA_HCos;               /* Computed Parameter: SineWaveA_HCos
                                        * Referenced by: '<S157>/Sine Wave A' (Parameter: CosH)
                                        */
  real_T SineWaveA_PSin;               /* Computed Parameter: SineWaveA_PSin
                                        * Referenced by: '<S157>/Sine Wave A' (Parameter: SinPhi)
                                        */
  real_T SineWaveA_PCos;               /* Computed Parameter: SineWaveA_PCos
                                        * Referenced by: '<S157>/Sine Wave A' (Parameter: CosPhi)
                                        */
  real_T SineWaveB_Amp;                /* Expression: Ab
                                        * Referenced by: '<S157>/Sine Wave B' (Parameter: Amplitude)
                                        */
  real_T SineWaveB_Bias;               /* Expression: 0
                                        * Referenced by: '<S157>/Sine Wave B' (Parameter: Bias)
                                        */
  real_T SineWaveB_Hsin;               /* Computed Parameter: SineWaveB_Hsin
                                        * Referenced by: '<S157>/Sine Wave B' (Parameter: SinH)
                                        */
  real_T SineWaveB_HCos;               /* Computed Parameter: SineWaveB_HCos
                                        * Referenced by: '<S157>/Sine Wave B' (Parameter: CosH)
                                        */
  real_T SineWaveB_PSin;               /* Computed Parameter: SineWaveB_PSin
                                        * Referenced by: '<S157>/Sine Wave B' (Parameter: SinPhi)
                                        */
  real_T SineWaveB_PCos;               /* Computed Parameter: SineWaveB_PCos
                                        * Referenced by: '<S157>/Sine Wave B' (Parameter: CosPhi)
                                        */
  real_T SineWaveC_Amp;                /* Expression: Ac
                                        * Referenced by: '<S157>/Sine Wave C' (Parameter: Amplitude)
                                        */
  real_T SineWaveC_Bias;               /* Expression: 0
                                        * Referenced by: '<S157>/Sine Wave C' (Parameter: Bias)
                                        */
  real_T SineWaveC_Hsin;               /* Computed Parameter: SineWaveC_Hsin
                                        * Referenced by: '<S157>/Sine Wave C' (Parameter: SinH)
                                        */
  real_T SineWaveC_HCos;               /* Computed Parameter: SineWaveC_HCos
                                        * Referenced by: '<S157>/Sine Wave C' (Parameter: CosH)
                                        */
  real_T SineWaveC_PSin;               /* Computed Parameter: SineWaveC_PSin
                                        * Referenced by: '<S157>/Sine Wave C' (Parameter: SinPhi)
                                        */
  real_T SineWaveC_PCos;               /* Computed Parameter: SineWaveC_PCos
                                        * Referenced by: '<S157>/Sine Wave C' (Parameter: CosPhi)
                                        */
  real_T VfDevicesClampingDiodes_Value_p[6];/* Expression: Vf_SwitchOn
                                             * Referenced by: '<S118>/Vf Devices & Clamping Diodes' (Parameter: Value)
                                             */
  real_T VfDiodes_Value_b[6];          /* Expression: Vf_DiodeOn
                                        * Referenced by: '<S118>/Vf Diodes' (Parameter: Value)
                                        */
  real_T StateSpace_P1_Size_n[2];    /* Computed Parameter: StateSpace_P1_Size_n
                                      * Referenced by: '<S163>/State-Space' (Parameter: P1Size)
                                      */
  real_T StateSpace_P1_n;              /* Expression: 0
                                        * Referenced by: '<S163>/State-Space' (Parameter: P1)
                                        */
  real_T StateSpace_P2_Size_h[2];    /* Computed Parameter: StateSpace_P2_Size_h
                                      * Referenced by: '<S163>/State-Space' (Parameter: P2Size)
                                      */
  real_T StateSpace_P2_l;              /* Expression: 0
                                        * Referenced by: '<S163>/State-Space' (Parameter: P2)
                                        */
  real_T uRon_Gain_a;                  /* Expression: 1./Ron
                                        * Referenced by: '<S116>/1//Ron' (Parameter: Gain)
                                        */
  real_T Switch_Threshold_lp;          /* Expression: 0.5
                                        * Referenced by: '<S116>/Switch' (Parameter: Threshold)
                                        */
  real_T Saturation_UpperSat_j;        /* Expression: inf
                                        * Referenced by: '<S116>/Saturation' (Parameter: UpperLimit)
                                        */
  real_T Saturation_LowerSat_fu;       /* Expression: 0
                                        * Referenced by: '<S116>/Saturation' (Parameter: LowerLimit)
                                        */
  real_T donotdeletethisgain_Gain_hq;  /* Expression: 1
                                        * Referenced by: '<S113>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_ha;  /* Expression: 1
                                        * Referenced by: '<S114>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_bv;  /* Expression: 1
                                        * Referenced by: '<S115>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T Delay_InitialCondition_fz;    /* Expression: 0.0
                                        * Referenced by: '<S126>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Delay_InitialCondition_av;    /* Expression: 0.0
                                        * Referenced by: '<S127>/Delay' (Parameter: InitialCondition)
                                        */
  real_T Sequence_Value_o;             /* Expression: S
                                        * Referenced by: '<S132>/Sequence' (Parameter: Value)
                                        */
  real_T Integrator_IC_g;              /* Expression: 0
                                        * Referenced by: '<S136>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T TransportDelay_InitOutput;    /* Expression: 0
                                        * Referenced by: '<S136>/Transport Delay' (Parameter: InitialOutput)
                                        */
  real_T Integrator_IC_e;              /* Expression: 0
                                        * Referenced by: '<S138>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T TransportDelay_InitOutput_g;  /* Expression: 0
                                        * Referenced by: '<S138>/Transport Delay' (Parameter: InitialOutput)
                                        */
  real_T Integrator_IC_i3;             /* Expression: 0
                                        * Referenced by: '<S140>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T TransportDelay_InitOutput_l;  /* Expression: 0
                                        * Referenced by: '<S140>/Transport Delay' (Parameter: InitialOutput)
                                        */
  real_T Integrator_IC_b;              /* Expression: 0
                                        * Referenced by: '<S135>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T TransportDelay_InitOutput_h;  /* Expression: 0
                                        * Referenced by: '<S135>/Transport Delay' (Parameter: InitialOutput)
                                        */
  real_T Integrator_IC_i2;             /* Expression: 0
                                        * Referenced by: '<S137>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T TransportDelay_InitOutput_i;  /* Expression: 0
                                        * Referenced by: '<S137>/Transport Delay' (Parameter: InitialOutput)
                                        */
  real_T Integrator_IC_h;              /* Expression: 0
                                        * Referenced by: '<S139>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T TransportDelay_InitOutput_j;  /* Expression: 0
                                        * Referenced by: '<S139>/Transport Delay' (Parameter: InitialOutput)
                                        */
  real_T DiscreteTimeIntegrator_gainva_m;
                          /* Computed Parameter: DiscreteTimeIntegrator_gainva_m
                           * Referenced by: '<S124>/Discrete-Time Integrator' (Parameter: gainval)
                           */
  real_T DiscreteTimeIntegrator_IC_d;  /* Expression: 0
                                        * Referenced by: '<S124>/Discrete-Time Integrator' (Parameter: InitialCondition)
                                        */
  real_T DiscreteTimeIntegrator1_gainv_i;
                          /* Computed Parameter: DiscreteTimeIntegrator1_gainv_i
                           * Referenced by: '<S124>/Discrete-Time Integrator1' (Parameter: gainval)
                           */
  real_T DiscreteTimeIntegrator1_IC_l; /* Expression: 0
                                        * Referenced by: '<S124>/Discrete-Time Integrator1' (Parameter: InitialCondition)
                                        */
  real_T Gain1_Gain_gy;                /* Expression: 2
                                        * Referenced by: '<S128>/Gain1' (Parameter: Gain)
                                        */
  real_T Switch_Threshold_d;           /* Expression: 0
                                        * Referenced by: '<S128>/Switch' (Parameter: Threshold)
                                        */
  real_T Gain_Gain_o3;                 /* Expression: 3
                                        * Referenced by: '<S128>/Gain' (Parameter: Gain)
                                        */
  real_T Gain3_Gain_az;                /* Expression: -2
                                        * Referenced by: '<S128>/Gain3' (Parameter: Gain)
                                        */
  real_T Gain2_Gain_fz;                /* Expression: 3
                                        * Referenced by: '<S128>/Gain2' (Parameter: Gain)
                                        */
  real_T Constant_Value_i;             /* Expression: 2*pi/3
                                        * Referenced by: '<S126>/Constant' (Parameter: Value)
                                        */
  real_T Constant1_Value_i;            /* Expression: 2*pi/3
                                        * Referenced by: '<S126>/Constant1' (Parameter: Value)
                                        */
  real_T Constant2_Value_p;            /* Expression: 2*pi/3
                                        * Referenced by: '<S126>/Constant2' (Parameter: Value)
                                        */
  real_T Constant3_Value_hr;           /* Expression: 2*pi/3
                                        * Referenced by: '<S126>/Constant3' (Parameter: Value)
                                        */
  real_T Gain_Gain_h;                  /* Expression: 0.666
                                        * Referenced by: '<S126>/Gain' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_oj;                /* Expression: -0.666
                                        * Referenced by: '<S126>/Gain1' (Parameter: Gain)
                                        */
  real_T Constant_Value_d;             /* Expression: 2*pi/3
                                        * Referenced by: '<S127>/Constant' (Parameter: Value)
                                        */
  real_T Constant1_Value_gs;           /* Expression: 2*pi/3
                                        * Referenced by: '<S127>/Constant1' (Parameter: Value)
                                        */
  real_T Constant2_Value_d;            /* Expression: 2*pi/3
                                        * Referenced by: '<S127>/Constant2' (Parameter: Value)
                                        */
  real_T Constant3_Value_e;            /* Expression: 2*pi/3
                                        * Referenced by: '<S127>/Constant3' (Parameter: Value)
                                        */
  real_T Gain_Gain_g;                  /* Expression: 0.666
                                        * Referenced by: '<S127>/Gain' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_a0v;               /* Expression: -0.666
                                        * Referenced by: '<S127>/Gain1' (Parameter: Gain)
                                        */
  real_T Integrator_WrappedStateUpperVal;/* Expression: WrappedStateUpperValue
                                          * Referenced by: '<S147>/Integrator' (Parameter: WrappedStateUpperValue)
                                          */
  real_T Integrator_WrappedStateLowerVal;/* Expression: WrappedStateLowerValue
                                          * Referenced by: '<S147>/Integrator' (Parameter: WrappedStateLowerValue)
                                          */
  real_T Integrator_IC_n;              /* Expression: 0
                                        * Referenced by: '<S130>/Integrator' (Parameter: InitialCondition)
                                        */
  real_T Gain5_Gain_c;                 /* Expression: 0.866
                                        * Referenced by: '<S130>/Gain5' (Parameter: Gain)
                                        */
  real_T Gain6_Gain_m;                 /* Expression: -0.866
                                        * Referenced by: '<S130>/Gain6' (Parameter: Gain)
                                        */
  real_T Gain4_Gain_b;                 /* Expression: 0.666
                                        * Referenced by: '<S130>/Gain4' (Parameter: Gain)
                                        */
  real_T Gain2_Gain_p;                 /* Expression: -0.5
                                        * Referenced by: '<S130>/Gain2' (Parameter: Gain)
                                        */
  real_T Gain3_Gain_k;                 /* Expression: -0.5
                                        * Referenced by: '<S130>/Gain3' (Parameter: Gain)
                                        */
  real_T Gain1_Gain_jm;                /* Expression: 0.666
                                        * Referenced by: '<S130>/Gain1' (Parameter: Gain)
                                        */
  real_T Gain7_Gain_dd;                /* Expression: 92
                                        * Referenced by: '<S130>/Gain7' (Parameter: Gain)
                                        */
  real_T Constant_Value_px;            /* Expression: 2*pi*60
                                        * Referenced by: '<S130>/Constant' (Parameter: Value)
                                        */
  real_T donotdeletethisgain_Gain_j;   /* Expression: 1
                                        * Referenced by: '<S102>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_hj;  /* Expression: 1
                                        * Referenced by: '<S103>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_l;   /* Expression: 1
                                        * Referenced by: '<S104>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_je;  /* Expression: 1
                                        * Referenced by: '<S110>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_d;   /* Expression: 1
                                        * Referenced by: '<S111>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_nc;  /* Expression: 1
                                        * Referenced by: '<S105>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T dataready25kHz_Amp;           /* Expression: 1
                                        * Referenced by: '<S3>/data ready 2.5 kHz' (Parameter: Amplitude)
                                        */
  real_T dataready25kHz_Period;        /* Expression: 2
                                        * Referenced by: '<S3>/data ready 2.5 kHz' (Parameter: Period)
                                        */
  real_T dataready25kHz_Duty;          /* Expression: 2
                                        * Referenced by: '<S3>/data ready 2.5 kHz' (Parameter: PulseWidth)
                                        */
  real_T dataready25kHz_PhaseDelay;    /* Expression: 0
                                        * Referenced by: '<S3>/data ready 2.5 kHz' (Parameter: PhaseDelay)
                                        */
  real_T SFunction2_P1_Size[2];        /* Computed Parameter: SFunction2_P1_Size
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P1Size)
                                        */
  real_T SFunction2_P1;                /* Expression: ctl_id
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P1)
                                        */
  real_T SFunction2_P2_Size[2];        /* Computed Parameter: SFunction2_P2_Size
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P2Size)
                                        */
  real_T SFunction2_P2;                /* Expression: send_id
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P2)
                                        */
  real_T SFunction2_P3_Size[2];        /* Computed Parameter: SFunction2_P3_Size
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P3Size)
                                        */
  real_T SFunction2_P3;                /* Expression: mode
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P3)
                                        */
  real_T SFunction2_P4_Size[2];        /* Computed Parameter: SFunction2_P4_Size
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P4Size)
                                        */
  real_T SFunction2_P4;                /* Expression: fp1
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P4)
                                        */
  real_T SFunction2_P5_Size[2];        /* Computed Parameter: SFunction2_P5_Size
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P5Size)
                                        */
  real_T SFunction2_P5;                /* Expression: fp2
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P5)
                                        */
  real_T SFunction2_P6_Size[2];        /* Computed Parameter: SFunction2_P6_Size
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P6Size)
                                        */
  real_T SFunction2_P6;                /* Expression: fp3
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P6)
                                        */
  real_T SFunction2_P7_Size[2];        /* Computed Parameter: SFunction2_P7_Size
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P7Size)
                                        */
  real_T SFunction2_P7;                /* Expression: fp4
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P7)
                                        */
  real_T SFunction2_P8_Size[2];        /* Computed Parameter: SFunction2_P8_Size
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P8Size)
                                        */
  real_T SFunction2_P8;                /* Expression: fp5
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P8)
                                        */
  real_T SFunction2_P9_Size[2];        /* Computed Parameter: SFunction2_P9_Size
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P9Size)
                                        */
  real_T SFunction2_P9[7];             /* Computed Parameter: SFunction2_P9
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P9)
                                        */
  real_T SFunction2_P10_Size[2];      /* Computed Parameter: SFunction2_P10_Size
                                       * Referenced by: '<S14>/S-Function2' (Parameter: P10Size)
                                       */
  real_T SFunction2_P10[7];            /* Computed Parameter: SFunction2_P10
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P10)
                                        */
  real_T SFunction2_P11_Size[2];      /* Computed Parameter: SFunction2_P11_Size
                                       * Referenced by: '<S14>/S-Function2' (Parameter: P11Size)
                                       */
  real_T SFunction2_P11[7];            /* Computed Parameter: SFunction2_P11
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P11)
                                        */
  real_T SFunction2_P12_Size[2];      /* Computed Parameter: SFunction2_P12_Size
                                       * Referenced by: '<S14>/S-Function2' (Parameter: P12Size)
                                       */
  real_T SFunction2_P12[7];            /* Computed Parameter: SFunction2_P12
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P12)
                                        */
  real_T SFunction2_P13_Size[2];      /* Computed Parameter: SFunction2_P13_Size
                                       * Referenced by: '<S14>/S-Function2' (Parameter: P13Size)
                                       */
  real_T SFunction2_P13[7];            /* Computed Parameter: SFunction2_P13
                                        * Referenced by: '<S14>/S-Function2' (Parameter: P13)
                                        */
  real_T OpIPSocketCtrl1_P1_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl1_P1_Size
                                   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P1Size)
                                   */
  real_T OpIPSocketCtrl1_P1;           /* Expression: ctl_id
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P1)
                                        */
  real_T OpIPSocketCtrl1_P2_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl1_P2_Size
                                   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P2Size)
                                   */
  real_T OpIPSocketCtrl1_P2;           /* Expression: proto
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P2)
                                        */
  real_T OpIPSocketCtrl1_P3_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl1_P3_Size
                                   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P3Size)
                                   */
  real_T OpIPSocketCtrl1_P3;           /* Expression: ip_port_remote
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P3)
                                        */
  real_T OpIPSocketCtrl1_P4_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl1_P4_Size
                                   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P4Size)
                                   */
  real_T OpIPSocketCtrl1_P4;           /* Expression: ip_port_local
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P4)
                                        */
  real_T OpIPSocketCtrl1_P5_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl1_P5_Size
                                   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P5Size)
                                   */
  real_T OpIPSocketCtrl1_P5;           /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P5)
                                        */
  real_T OpIPSocketCtrl1_P6_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl1_P6_Size
                                   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P6Size)
                                   */
  real_T OpIPSocketCtrl1_P6;           /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P6)
                                        */
  real_T OpIPSocketCtrl1_P7_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl1_P7_Size
                                   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P7Size)
                                   */
  real_T OpIPSocketCtrl1_P7;           /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P7)
                                        */
  real_T OpIPSocketCtrl1_P8_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl1_P8_Size
                                   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P8Size)
                                   */
  real_T OpIPSocketCtrl1_P8;           /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P8)
                                        */
  real_T OpIPSocketCtrl1_P9_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl1_P9_Size
                                   * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P9Size)
                                   */
  real_T OpIPSocketCtrl1_P9;           /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P9)
                                        */
  real_T OpIPSocketCtrl1_P10_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P10_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P10Size)
                                  */
  real_T OpIPSocketCtrl1_P10;          /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P10)
                                        */
  real_T OpIPSocketCtrl1_P11_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P11_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P11Size)
                                  */
  real_T OpIPSocketCtrl1_P11;          /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P11)
                                        */
  real_T OpIPSocketCtrl1_P12_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P12_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P12Size)
                                  */
  real_T OpIPSocketCtrl1_P12;          /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P12)
                                        */
  real_T OpIPSocketCtrl1_P13_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P13_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P13Size)
                                  */
  real_T OpIPSocketCtrl1_P13;          /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P13)
                                        */
  real_T OpIPSocketCtrl1_P14_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P14_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P14Size)
                                  */
  real_T OpIPSocketCtrl1_P14[13];     /* Computed Parameter: OpIPSocketCtrl1_P14
                                       * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P14)
                                       */
  real_T OpIPSocketCtrl1_P15_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P15_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P15Size)
                                  */
  real_T OpIPSocketCtrl1_P15[7];      /* Computed Parameter: OpIPSocketCtrl1_P15
                                       * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P15)
                                       */
  real_T OpIPSocketCtrl1_P16_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P16_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P16Size)
                                  */
  real_T OpIPSocketCtrl1_P17_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P17_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P17Size)
                                  */
  real_T OpIPSocketCtrl1_P18_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P18_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P18Size)
                                  */
  real_T OpIPSocketCtrl1_P19_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P19_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P19Size)
                                  */
  real_T OpIPSocketCtrl1_P20_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P20_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P20Size)
                                  */
  real_T OpIPSocketCtrl1_P21_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P21_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P21Size)
                                  */
  real_T OpIPSocketCtrl1_P22_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P22_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P22Size)
                                  */
  real_T OpIPSocketCtrl1_P23_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P23_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P23Size)
                                  */
  real_T OpIPSocketCtrl1_P24_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P24_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P24Size)
                                  */
  real_T OpIPSocketCtrl1_P25_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P25_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P25Size)
                                  */
  real_T OpIPSocketCtrl1_P26_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P26_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P26Size)
                                  */
  real_T OpIPSocketCtrl1_P26[7];      /* Computed Parameter: OpIPSocketCtrl1_P26
                                       * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P26)
                                       */
  real_T OpIPSocketCtrl1_P27_Size[2];
                                 /* Computed Parameter: OpIPSocketCtrl1_P27_Size
                                  * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P27Size)
                                  */
  real_T OpIPSocketCtrl1_P27;          /* Expression: 0
                                        * Referenced by: '<S3>/OpIPSocketCtrl1' (Parameter: P27)
                                        */
  creal_T alpha2_Gain;                 /* Expression: -0.5*(1+1j*sqrt(3))
                                        * Referenced by: '<S62>/alpha2' (Parameter: Gain)
                                        */
  creal_T alpha_Gain;                  /* Expression: -0.5*(1-1j*sqrt(3))
                                        * Referenced by: '<S62>/alpha' (Parameter: Gain)
                                        */
  creal_T alpha2_Gain_p;               /* Expression: -0.5*(1+1j*sqrt(3))
                                        * Referenced by: '<S63>/alpha2' (Parameter: Gain)
                                        */
  creal_T alpha_Gain_i;                /* Expression: -0.5*(1-1j*sqrt(3))
                                        * Referenced by: '<S63>/alpha' (Parameter: Gain)
                                        */
  creal_T alpha_Gain_o;                /* Expression: -0.5*(1-1j*sqrt(3))
                                        * Referenced by: '<S54>/alpha' (Parameter: Gain)
                                        */
  creal_T alpha2_Gain_f;               /* Expression: -0.5*(1+1j*sqrt(3))
                                        * Referenced by: '<S54>/alpha2' (Parameter: Gain)
                                        */
  creal_T alpha_Gain_a;                /* Expression: -0.5*(1-1j*sqrt(3))
                                        * Referenced by: '<S55>/alpha' (Parameter: Gain)
                                        */
  creal_T alpha2_Gain_h;               /* Expression: -0.5*(1+1j*sqrt(3))
                                        * Referenced by: '<S55>/alpha2' (Parameter: Gain)
                                        */
  creal_T alpha2_Gain_m;               /* Expression: -0.5*(1+1j*sqrt(3))
                                        * Referenced by: '<S141>/alpha2' (Parameter: Gain)
                                        */
  creal_T alpha_Gain_p;                /* Expression: -0.5*(1-1j*sqrt(3))
                                        * Referenced by: '<S141>/alpha' (Parameter: Gain)
                                        */
  creal_T alpha2_Gain_mb;              /* Expression: -0.5*(1+1j*sqrt(3))
                                        * Referenced by: '<S142>/alpha2' (Parameter: Gain)
                                        */
  creal_T alpha_Gain_ay;               /* Expression: -0.5*(1-1j*sqrt(3))
                                        * Referenced by: '<S142>/alpha' (Parameter: Gain)
                                        */
  creal_T alpha_Gain_j;                /* Expression: -0.5*(1-1j*sqrt(3))
                                        * Referenced by: '<S133>/alpha' (Parameter: Gain)
                                        */
  creal_T alpha2_Gain_i;               /* Expression: -0.5*(1+1j*sqrt(3))
                                        * Referenced by: '<S133>/alpha2' (Parameter: Gain)
                                        */
  creal_T alpha_Gain_i1;               /* Expression: -0.5*(1-1j*sqrt(3))
                                        * Referenced by: '<S134>/alpha' (Parameter: Gain)
                                        */
  creal_T alpha2_Gain_o;               /* Expression: -0.5*(1+1j*sqrt(3))
                                        * Referenced by: '<S134>/alpha2' (Parameter: Gain)
                                        */
  creal_T Gain_Gain_mm[9]; /* Expression: j*(1/sqrt(3))*[0 1 -1; -1 0 1; 1 -1 0]
                            * Referenced by: '<S30>/Gain' (Parameter: Gain)
                            */
  P_Tail_basic_cosimulation_c_1_T Tail_e;/* '<S116>/Tail' */
  P_Tail_basic_cosimulation_c_1_T Tail;/* '<S37>/Tail' */
};

/* Real-time Model Data Structure */
struct tag_RTM_basic_cosimulation_c_1_sm_master_T {
  const char_T *path;
  const char_T *modelName;
  struct SimStruct_tag * *childSfunctions;
  const char_T *errorStatus;
  SS_SimMode simMode;
  RTWLogInfo *rtwLogInfo;
  RTWExtModeInfo *extModeInfo;
  RTWSolverInfo solverInfo;
  RTWSolverInfo *solverInfoPtr;
  void *sfcnInfo;

  /*
   * NonInlinedSFcns:
   * The following substructure contains information regarding
   * non-inlined s-functions used in the model.
   */
  struct {
    RTWSfcnInfo sfcnInfo;
    time_T *taskTimePtrs[2];
    SimStruct childSFunctions[11];
    SimStruct *childSFunctionPtrs[11];
    struct _ssBlkInfo2 blkInfo2[11];
    struct _ssSFcnModelMethods2 methods2[11];
    struct _ssSFcnModelMethods3 methods3[11];
    struct _ssSFcnModelMethods4 methods4[11];
    struct _ssStatesInfo2 statesInfo2[11];
    ssPeriodicStatesInfo periodicStatesInfo[11];
    struct _ssPortInfo2 inputOutputPortInfo2[11];
    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[1];
      struct _ssInPortUnit inputPortUnits[1];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[1];
      real_T const *UPtrs0[1];
      struct _ssPortOutputs outputPortInfo[3];
      struct _ssOutPortUnit outputPortUnits[3];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[3];
      uint_T attribs[14];
      mxArray *params[14];
      struct _ssDWorkRecord dWork[1];
      struct _ssDWorkAuxRecord dWorkAux[1];
    } Sfcn0;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortOutputs outputPortInfo[1];
      struct _ssOutPortUnit outputPortUnits[1];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[1];
      uint_T attribs[2];
      mxArray *params[2];
    } Sfcn1;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[2];
      struct _ssInPortUnit inputPortUnits[2];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[2];
      struct _ssPortOutputs outputPortInfo[2];
      struct _ssOutPortUnit outputPortUnits[2];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[2];
      uint_T attribs[10];
      mxArray *params[10];
      struct _ssDWorkRecord dWork[1];
      struct _ssDWorkAuxRecord dWorkAux[1];
    } Sfcn2;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[1];
      struct _ssInPortUnit inputPortUnits[1];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[1];
      real_T const *UPtrs0[6];
      struct _ssPortOutputs outputPortInfo[1];
      struct _ssOutPortUnit outputPortUnits[1];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[1];
      uint_T attribs[5];
      mxArray *params[5];
      struct _ssDWorkRecord dWork[2];
      struct _ssDWorkAuxRecord dWorkAux[2];
    } Sfcn3;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[2];
      struct _ssInPortUnit inputPortUnits[2];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[2];
      real_T const *UPtrs0[16];
      real_T const *UPtrs1[6];
      struct _ssPortOutputs outputPortInfo[2];
      struct _ssOutPortUnit outputPortUnits[2];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[2];
      uint_T attribs[2];
      mxArray *params[2];
      struct _ssDWorkRecord dWork[11];
      struct _ssDWorkAuxRecord dWorkAux[11];
    } Sfcn4;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[1];
      struct _ssInPortUnit inputPortUnits[1];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[1];
      real_T const *UPtrs0[26];
      uint_T attribs[1];
      mxArray *params[1];
      struct _ssDWorkRecord dWork[1];
      struct _ssDWorkAuxRecord dWorkAux[1];
    } Sfcn5;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[2];
      struct _ssInPortUnit inputPortUnits[2];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[2];
      struct _ssPortOutputs outputPortInfo[2];
      struct _ssOutPortUnit outputPortUnits[2];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[2];
      uint_T attribs[10];
      mxArray *params[10];
      struct _ssDWorkRecord dWork[1];
      struct _ssDWorkAuxRecord dWorkAux[1];
    } Sfcn6;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[1];
      struct _ssInPortUnit inputPortUnits[1];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[1];
      real_T const *UPtrs0[6];
      struct _ssPortOutputs outputPortInfo[1];
      struct _ssOutPortUnit outputPortUnits[1];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[1];
      uint_T attribs[5];
      mxArray *params[5];
      struct _ssDWorkRecord dWork[2];
      struct _ssDWorkAuxRecord dWorkAux[2];
    } Sfcn7;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[2];
      struct _ssInPortUnit inputPortUnits[2];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[2];
      real_T const *UPtrs0[16];
      real_T const *UPtrs1[6];
      struct _ssPortOutputs outputPortInfo[2];
      struct _ssOutPortUnit outputPortUnits[2];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[2];
      uint_T attribs[2];
      mxArray *params[2];
      struct _ssDWorkRecord dWork[11];
      struct _ssDWorkAuxRecord dWorkAux[11];
    } Sfcn8;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[2];
      struct _ssInPortUnit inputPortUnits[2];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[2];
      real_T const *UPtrs0[1];
      real_T const *UPtrs1[6];
      struct _ssPortOutputs outputPortInfo[1];
      struct _ssOutPortUnit outputPortUnits[1];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[1];
      uint_T attribs[13];
      mxArray *params[13];
      struct _ssDWorkRecord dWork[1];
      struct _ssDWorkAuxRecord dWorkAux[1];
    } Sfcn9;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      uint_T attribs[27];
      mxArray *params[27];
      struct _ssDWorkRecord dWork[1];
      struct _ssDWorkAuxRecord dWorkAux[1];
    } Sfcn10;
  } NonInlinedSFcns;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    void *blockIO;
    const void *constBlockIO;
    void *defaultParam;
    ZCSigState *prevZCSigState;
    real_T *contStates;
    int_T *periodicContStateIndices;
    real_T *periodicContStateRanges;
    real_T *derivs;
    void *zcSignalValues;
    void *inputs;
    void *outputs;
    boolean_T *contStateDisabled;
    boolean_T zCCacheNeedsReset;
    boolean_T derivCacheNeedsReset;
    boolean_T CTOutputIncnstWithState;
    real_T odeY[56];
    real_T odeF[4][56];
    ODE4_IntgData intgData;
    void *dwork;
  } ModelData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    uint32_T checksums[4];
    uint32_T options;
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numU;
    int_T numY;
    int_T numSampTimes;
    int_T numBlocks;
    int_T numBlockIO;
    int_T numBlockPrms;
    int_T numDwork;
    int_T numSFcnPrms;
    int_T numSFcns;
    int_T numIports;
    int_T numOports;
    int_T numNonSampZCs;
    int_T sysDirFeedThru;
    int_T rtwGenSfcn;
  } Sizes;

  /*
   * SpecialInfo:
   * The following substructure contains special information
   * related to other components that are dependent on RTW.
   */
  struct {
    const void *mappingInfo;
    void *xpcData;
  } SpecialInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T stepSize;
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    time_T stepSize1;
    boolean_T firstInitCondFlag;
    time_T tStart;
    time_T tFinal;
    time_T timeOfLastOutput;
    void *timingData;
    real_T *varNextHitTimesList;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *sampleTimes;
    time_T *offsetTimes;
    int_T *sampleTimeTaskIDPtr;
    int_T *sampleHits;
    int_T *perTaskSampleHits;
    time_T *t;
    time_T sampleTimesArray[2];
    time_T offsetTimesArray[2];
    int_T sampleTimeTaskIDArray[2];
    int_T sampleHitArray[2];
    int_T perTaskSampleHitsArray[4];
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_basic_cosimulation_c_1_sm_master_T basic_cosimulation_c_1_sm_master_P;

/* Block signals (default storage) */
extern B_basic_cosimulation_c_1_sm_master_T basic_cosimulation_c_1_sm_master_B;

/* Continuous states (default storage) */
extern X_basic_cosimulation_c_1_sm_master_T basic_cosimulation_c_1_sm_master_X;

/* Block states (default storage) */
extern DW_basic_cosimulation_c_1_sm_master_T basic_cosimulation_c_1_sm_master_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_basic_cosimulation_c_1_sm_master_T
  basic_cosimulation_c_1_sm_master_PrevZCX;

/* Model entry point functions */
extern void basic_cosimulation_c_1_sm_master_initialize(void);
extern void basic_cosimulation_c_1_sm_master_output(void);
extern void basic_cosimulation_c_1_sm_master_update(void);
extern void basic_cosimulation_c_1_sm_master_terminate(void);

/*====================*
 * External functions *
 *====================*/
extern basic_cosimulation_c_1_sm_master_rtModel
  *basic_cosimulation_c_1_sm_master(void);
extern void MdlInitializeSizes(void);
extern void MdlInitializeSampleTimes(void);
extern void MdlInitialize(void);
extern void MdlStart(void);
extern void MdlOutputs(int_T tid);
extern void MdlUpdate(int_T tid);
extern void MdlTerminate(void);

/* Real-time Model object */
extern RT_MODEL_basic_cosimulation_c_1_sm_master_T *const
  basic_cosimulation_c_1_sm_master_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'basic_cosimulation_c_1_sm_master'
 * '<S1>'   : 'basic_cosimulation_c_1_sm_master/ARTEMIS Guide'
 * '<S2>'   : 'basic_cosimulation_c_1_sm_master/OpCCode_do_not_touch'
 * '<S3>'   : 'basic_cosimulation_c_1_sm_master/SM_Master'
 * '<S4>'   : 'basic_cosimulation_c_1_sm_master/powergui'
 * '<S5>'   : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1'
 * '<S6>'   : 'basic_cosimulation_c_1_sm_master/SM_Master/OpAsyncRecv'
 * '<S7>'   : 'basic_cosimulation_c_1_sm_master/SM_Master/OpComm'
 * '<S8>'   : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit'
 * '<S9>'   : 'basic_cosimulation_c_1_sm_master/SM_Master/epoch_time_nsec'
 * '<S10>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/epoch_time_sec'
 * '<S11>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/ptp_slave_offset'
 * '<S12>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/ptp_sync_state'
 * '<S13>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/rtlab_send_subsystem'
 * '<S14>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/send message 1'
 * '<S15>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/sync_accuracy'
 * '<S16>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC'
 * '<S17>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control'
 * '<S18>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement'
 * '<S19>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement1'
 * '<S20>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement2'
 * '<S21>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement3'
 * '<S22>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement4'
 * '<S23>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement5'
 * '<S24>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement6'
 * '<S25>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/DC Voltage Source'
 * '<S26>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/PWM'
 * '<S27>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Phasor_to_Time_Domain_SFA'
 * '<S28>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Three-Phase Transformer (Two Windings)'
 * '<S29>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Three-Phase Controlled  Y-Connected Voltage Source '
 * '<S30>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Time_Domain_To_Phasor_SFA'
 * '<S31>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Voltage Measurement1'
 * '<S32>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Voltage Measurement2'
 * '<S33>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/Model'
 * '<S34>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/iA'
 * '<S35>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/iB'
 * '<S36>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/iC'
 * '<S37>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT'
 * '<S38>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT/Tail'
 * '<S39>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT/Vf 1'
 * '<S40>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT/fts5conversion'
 * '<S41>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT/Tail/y=f(t)'
 * '<S42>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/iA/Model'
 * '<S43>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/iB/Model'
 * '<S44>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/ARTEMiS SSN IVIC/iC/Model'
 * '<S45>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Current Control'
 * '<S46>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Meas_3_Wire'
 * '<S47>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Park_Transformation'
 * '<S48>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Park_Transformation1'
 * '<S49>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Control'
 * '<S50>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)'
 * '<S51>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/SRF_PLL'
 * '<S52>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Current Control/Inverse_Park_Transformation'
 * '<S53>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete'
 * '<S54>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Negative I2'
 * '<S55>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Negative V2'
 * '<S56>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-A I'
 * '<S57>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-A V'
 * '<S58>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-B I'
 * '<S59>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-B V'
 * '<S60>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-C I'
 * '<S61>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-C V'
 * '<S62>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Positive I1'
 * '<S63>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Positive V1'
 * '<S64>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/SinCos'
 * '<S65>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Zero I0'
 * '<S66>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Zero V0'
 * '<S67>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-A I/Corrected discrete transport delay'
 * '<S68>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-A V/Corrected discrete transport delay'
 * '<S69>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-B I/Corrected discrete transport delay'
 * '<S70>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-B V/Corrected discrete transport delay'
 * '<S71>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-C I/Corrected discrete transport delay'
 * '<S72>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/Phase-C V/Corrected discrete transport delay'
 * '<S73>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/SinCos/Data Type Conversion Inherited'
 * '<S74>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/SinCos/Integrator with Wrapped State (Discrete or Continuous)'
 * '<S75>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/SinCos/Integrator with Wrapped State (Discrete or Continuous)/Discrete'
 * '<S76>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/SinCos/Integrator with Wrapped State (Discrete or Continuous)/Discrete/Compare To Constant'
 * '<S77>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/SinCos/Integrator with Wrapped State (Discrete or Continuous)/Discrete/Compare To Constant1'
 * '<S78>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Control/Power Measurement (Three-Phase)/Discrete/SinCos/Integrator with Wrapped State (Discrete or Continuous)/Discrete/Reinitialization'
 * '<S79>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement/Model'
 * '<S80>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement1/Model'
 * '<S81>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement2/Model'
 * '<S82>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement3/Model'
 * '<S83>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement4/Model'
 * '<S84>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement5/Model'
 * '<S85>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Current Measurement6/Model'
 * '<S86>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/DC Voltage Source/Model'
 * '<S87>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Three-Phase Transformer (Two Windings)/Model'
 * '<S88>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Three-Phase Transformer (Two Windings)/Model/Linear'
 * '<S89>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Three-Phase Transformer (Two Windings)/Model/Linear1'
 * '<S90>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Three-Phase Transformer (Two Windings)/Model/Linear2'
 * '<S91>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Three-Phase Controlled  Y-Connected Voltage Source /Controlled Voltage Source'
 * '<S92>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Three-Phase Controlled  Y-Connected Voltage Source /Controlled Voltage Source1'
 * '<S93>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Three-Phase Controlled  Y-Connected Voltage Source /Controlled Voltage Source2'
 * '<S94>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Voltage Measurement1/Model'
 * '<S95>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/Circuit1/Voltage Measurement2/Model'
 * '<S96>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/OpComm/Receive'
 * '<S97>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/OpComm/busStruct'
 * '<S98>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/OpComm/busStruct/Sub1'
 * '<S99>'  : 'basic_cosimulation_c_1_sm_master/SM_Master/OpComm/busStruct/Sub2'
 * '<S100>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC'
 * '<S101>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control'
 * '<S102>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Current Measurement'
 * '<S103>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Current Measurement1'
 * '<S104>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Current Measurement2'
 * '<S105>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Current Measurement6'
 * '<S106>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/DC Voltage Source'
 * '<S107>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/PWM'
 * '<S108>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Three-Phase Transformer (Two Windings)'
 * '<S109>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Three-Phase Source'
 * '<S110>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Voltage Measurement1'
 * '<S111>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Voltage Measurement2'
 * '<S112>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/Model'
 * '<S113>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/iA'
 * '<S114>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/iB'
 * '<S115>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/iC'
 * '<S116>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT'
 * '<S117>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT/Tail'
 * '<S118>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT/Vf 1'
 * '<S119>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT/fts5conversion'
 * '<S120>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/Model/ArtemisdiscreteIGBT/Tail/y=f(t)'
 * '<S121>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/iA/Model'
 * '<S122>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/iB/Model'
 * '<S123>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/ARTEMiS SSN IVIC/iC/Model'
 * '<S124>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Current Control'
 * '<S125>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Meas_3_Wire'
 * '<S126>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Park_Transformation'
 * '<S127>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Park_Transformation1'
 * '<S128>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Control'
 * '<S129>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)'
 * '<S130>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/SRF_PLL'
 * '<S131>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Current Control/Inverse_Park_Transformation'
 * '<S132>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous'
 * '<S133>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Negative I2'
 * '<S134>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Negative V2'
 * '<S135>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Phase-A I'
 * '<S136>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Phase-A V'
 * '<S137>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Phase-B I'
 * '<S138>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Phase-B V'
 * '<S139>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Phase-C I'
 * '<S140>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Phase-C V'
 * '<S141>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Positive I1'
 * '<S142>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Positive V1'
 * '<S143>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/SinCos'
 * '<S144>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Zero I0'
 * '<S145>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/Zero V0'
 * '<S146>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/SinCos/Integrator with Wrapped State (Discrete or Continuous)'
 * '<S147>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Control/Power Measurement (Three-Phase)/Continuous/SinCos/Integrator with Wrapped State (Discrete or Continuous)/Continuous'
 * '<S148>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Current Measurement/Model'
 * '<S149>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Current Measurement1/Model'
 * '<S150>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Current Measurement2/Model'
 * '<S151>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Current Measurement6/Model'
 * '<S152>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/DC Voltage Source/Model'
 * '<S153>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Three-Phase Transformer (Two Windings)/Model'
 * '<S154>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Three-Phase Transformer (Two Windings)/Model/Linear'
 * '<S155>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Three-Phase Transformer (Two Windings)/Model/Linear1'
 * '<S156>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Three-Phase Transformer (Two Windings)/Model/Linear2'
 * '<S157>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Three-Phase Source/Model'
 * '<S158>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Voltage Measurement1/Model'
 * '<S159>' : 'basic_cosimulation_c_1_sm_master/SM_Master/Original Circuit/Voltage Measurement2/Model'
 * '<S160>' : 'basic_cosimulation_c_1_sm_master/SM_Master/rtlab_send_subsystem/Subsystem1'
 * '<S161>' : 'basic_cosimulation_c_1_sm_master/SM_Master/rtlab_send_subsystem/Subsystem1/Send1'
 * '<S162>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel1'
 * '<S163>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel2'
 * '<S164>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel1/Gates'
 * '<S165>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel1/Sources'
 * '<S166>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel1/Status'
 * '<S167>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel1/Yout'
 * '<S168>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel2/Gates'
 * '<S169>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel2/Sources'
 * '<S170>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel2/Status'
 * '<S171>' : 'basic_cosimulation_c_1_sm_master/powergui/EquivalentModel2/Yout'
 */
#endif                      /* RTW_HEADER_basic_cosimulation_c_1_sm_master_h_ */
