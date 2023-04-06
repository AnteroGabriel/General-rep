/*
 * three_phase_circuit__1_sm_master.h
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

#ifndef RTW_HEADER_three_phase_circuit__1_sm_master_h_
#define RTW_HEADER_three_phase_circuit__1_sm_master_h_
#include <stddef.h>
#include <string.h>
#include <math.h>
#ifndef three_phase_circuit__1_sm_master_COMMON_INCLUDES_
#define three_phase_circuit__1_sm_master_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_logging.h"
#endif                   /* three_phase_circuit__1_sm_master_COMMON_INCLUDES_ */

#include "three_phase_circuit__1_sm_master_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"

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
#define rtmGetFirstInitCondFlag(rtm)   ()
#endif

#ifndef rtmSetFirstInitCondFlag
#define rtmSetFirstInitCondFlag(rtm, val) ()
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ()
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ()
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
#define rtmGetOdeF(rtm)                ()
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ()
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ()
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ()
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
#define rtmIsContinuousTask(rtm, tid)  0
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmIsSampleHit
#define rtmIsSampleHit(rtm, sti, tid)  ((rtm)->Timing.sampleHits[(rtm)->Timing.sampleTimeTaskIDPtr[sti]])
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
#define three_phase_circuit__1_sm_master_rtModel RT_MODEL_three_phase_circuit__1_sm_master_T

/* user code (top of header file) */
/* System '<Root>' */
/* Opal-RT Technologies */
extern int opalSizeDwork;
extern unsigned int opalSizeBlockIO;
extern unsigned int opalSizeRTP;
extern void * pRtModel;                //pointer on the RTmodel struc
int_T Opal_rtmGetNumBlockParams(void *ptr);
int_T Opal_rtmGetNumBlockIO(void *ptr);

/* Block signals (default storage) */
typedef struct {
  real_T SFunction;                    /* '<S1>/S-Function' (Output 1)  */
  real_T Sum;                          /* '<S1>/Sum' (Output 1)  */
  real_T timeout;                      /* '<S2>/timeout' (Output 1)  */
  real_T SFunction1_o1;                /* '<S5>/S-Function1' (Output 1)  */
  real_T SFunction1_o2;                /* '<S5>/S-Function1' (Output 2)  */
  real_T SFunction1_o3[3];             /* '<S5>/S-Function1' (Output 3)  */
  real_T SineWaveA;                    /* '<S30>/Sine Wave A' (Output 1)  */
  real_T SineWaveB;                    /* '<S30>/Sine Wave B' (Output 1)  */
  real_T SineWaveC;                    /* '<S30>/Sine Wave C' (Output 1)  */
  real_T StateSpace[6];                /* '<S51>/State-Space' (Output 1)  */
  real_T donotdeletethisgain;  /* '<S18>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_d;/* '<S19>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_c;/* '<S20>/do not delete this gain' (Output 1)  */
  real_T Kv1[3];                       /* '<S9>/Kv1' (Output 1)  */
  real_T SineWaveA_p;                  /* '<S48>/Sine Wave A' (Output 1)  */
  real_T SineWaveB_p;                  /* '<S48>/Sine Wave B' (Output 1)  */
  real_T SineWaveC_i;                  /* '<S48>/Sine Wave C' (Output 1)  */
  real_T StateSpace_f[6];              /* '<S52>/State-Space' (Output 1)  */
  real_T donotdeletethisgain_j;/* '<S39>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_i;/* '<S40>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_p;/* '<S41>/do not delete this gain' (Output 1)  */
  real_T Kv1_p[3];                     /* '<S31>/Kv1' (Output 1)  */
  real_T donotdeletethisgain_k;/* '<S36>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_g;/* '<S37>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_i1;
                               /* '<S38>/do not delete this gain' (Output 1)  */
  real_T Kv[3];                        /* '<S31>/Kv' (Output 1)  */
  real_T donotdeletethisgain_di;
                               /* '<S15>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_gn;
                               /* '<S16>/do not delete this gain' (Output 1)  */
  real_T donotdeletethisgain_g1;
                               /* '<S17>/do not delete this gain' (Output 1)  */
  real_T Kv_f[3];                      /* '<S9>/Kv' (Output 1)  */
  real_T dataready25kHz;             /* '<S2>/data ready 2.5 kHz' (Output 1)  */
  real_T SFunction2;                   /* '<S8>/S-Function2' (Output 1)  */
} B_three_phase_circuit__1_sm_master_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T StateSpace_DSTATE[6];         /* '<S52>/State-Space' (DWork 3)  */
  real_T SFunction_PreviousInput;      /* '<S1>/S-Function' (DWork 1)  */
  real_T lastSin;                      /* '<S30>/Sine Wave A' (DWork 2)  */
  real_T lastCos;                      /* '<S30>/Sine Wave A' (DWork 3)  */
  real_T lastSin_g;                    /* '<S30>/Sine Wave B' (DWork 2)  */
  real_T lastCos_m;                    /* '<S30>/Sine Wave B' (DWork 3)  */
  real_T lastSin_m;                    /* '<S30>/Sine Wave C' (DWork 2)  */
  real_T lastCos_i;                    /* '<S30>/Sine Wave C' (DWork 3)  */
  real_T lastSin_k;                    /* '<S48>/Sine Wave A' (DWork 2)  */
  real_T lastCos_a;                    /* '<S48>/Sine Wave A' (DWork 3)  */
  real_T lastSin_kg;                   /* '<S48>/Sine Wave B' (DWork 2)  */
  real_T lastCos_l;                    /* '<S48>/Sine Wave B' (DWork 3)  */
  real_T lastSin_p;                    /* '<S48>/Sine Wave C' (DWork 2)  */
  real_T lastCos_h;                    /* '<S48>/Sine Wave C' (DWork 3)  */
  void *SFunction1_PWORK;              /* '<S5>/S-Function1' (DWork 1)  */
  struct {
    void *AS;
    void *BS;
    void *CS;
    void *DS;
    void *DX_COL;
    void *BD_COL;
    void *TMP1;
    void *TMP2;
    void *XTMP;
    void *SWITCH_STATUS;
    void *SWITCH_STATUS_INIT;
    void *SW_CHG;
    void *G_STATE;
    void *USWLAST;
    void *XKM12;
    void *XKP12;
    void *XLAST;
    void *ULAST;
    void *IDX_SW_CHG;
    void *Y_SWITCH;
    void *SWITCH_TYPES;
    void *IDX_OUT_SW;
    void *SWITCH_TOPO_SAVED_IDX;
    void *SWITCH_MAP;
  } StateSpace_PWORK;                  /* '<S51>/State-Space' (DWork 2)  */

  struct {
    void *AS;
    void *BS;
    void *CS;
    void *DS;
    void *DX_COL;
    void *BD_COL;
    void *TMP1;
    void *TMP2;
    void *XTMP;
    void *SWITCH_STATUS;
    void *SWITCH_STATUS_INIT;
    void *SW_CHG;
    void *G_STATE;
    void *USWLAST;
    void *XKM12;
    void *XKP12;
    void *XLAST;
    void *ULAST;
    void *IDX_SW_CHG;
    void *Y_SWITCH;
    void *SWITCH_TYPES;
    void *IDX_OUT_SW;
    void *SWITCH_TOPO_SAVED_IDX;
    void *SWITCH_MAP;
  } StateSpace_PWORK_m;                /* '<S52>/State-Space' (DWork 2)  */

  void *SFunction2_PWORK;              /* '<S8>/S-Function2' (DWork 1)  */
  void *OpIPSocketCtrl_PWORK;          /* '<S2>/OpIPSocketCtrl' (DWork 1)  */
  int32_T systemEnable;                /* '<S30>/Sine Wave A' (DWork 1)  */
  int32_T systemEnable_a;              /* '<S30>/Sine Wave B' (DWork 1)  */
  int32_T systemEnable_k;              /* '<S30>/Sine Wave C' (DWork 1)  */
  int32_T systemEnable_d;              /* '<S48>/Sine Wave A' (DWork 1)  */
  int32_T systemEnable_e;              /* '<S48>/Sine Wave B' (DWork 1)  */
  int32_T systemEnable_g;              /* '<S48>/Sine Wave C' (DWork 1)  */
  int32_T clockTickCounter;           /* '<S2>/data ready 2.5 kHz' (DWork 1)  */
  int_T StateSpace_IWORK[11];          /* '<S51>/State-Space' (DWork 1)  */
  int_T StateSpace_IWORK_p[11];        /* '<S52>/State-Space' (DWork 1)  */
  int_T SFunction_IWORK[5];            /* '<S50>/S-Function' (DWork 1)  */
} DW_three_phase_circuit__1_sm_master_T;

/* Backward compatible GRT Identifiers */
#define rtB                            three_phase_circuit__1_sm_master_B
#define BlockIO                        B_three_phase_circuit__1_sm_master_T
#define rtP                            three_phase_circuit__1_sm_master_P
#define Parameters                     P_three_phase_circuit__1_sm_master_T
#define rtDWork                        three_phase_circuit__1_sm_master_DW
#define D_Work                         DW_three_phase_circuit__1_sm_master_T

/* Parameters (default storage) */
struct P_three_phase_circuit__1_sm_master_T_ {
  real_T ThreePhaseSource_Frequency;
                                   /* Mask Parameter: ThreePhaseSource_Frequency
                                    * Referenced by:
                                    *   '<S30>/Sine Wave A' (Parameter: Frequency)
                                    *   '<S30>/Sine Wave B' (Parameter: Frequency)
                                    *   '<S30>/Sine Wave C' (Parameter: Frequency)
                                    */
  real_T ThreePhaseSource1_Frequency;
                                  /* Mask Parameter: ThreePhaseSource1_Frequency
                                   * Referenced by:
                                   *   '<S48>/Sine Wave A' (Parameter: Frequency)
                                   *   '<S48>/Sine Wave B' (Parameter: Frequency)
                                   *   '<S48>/Sine Wave C' (Parameter: Frequency)
                                   */
  real_T SFunction1_Value;             /* Expression: 0
                                        * Referenced by: '<S1>/S-Function1' (Parameter: Value)
                                        */
  real_T SFunction_InitialCondition;   /* Expression: 0
                                        * Referenced by: '<S1>/S-Function' (Parameter: InitialCondition)
                                        */
  real_T timeout_Value;                /* Expression: 2
                                        * Referenced by: '<S2>/timeout' (Parameter: Value)
                                        */
  real_T SFunction1_P1_Size[2];        /* Computed Parameter: SFunction1_P1_Size
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P1Size)
                                        */
  real_T SFunction1_P1;                /* Expression: ctl_id
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P1)
                                        */
  real_T SFunction1_P2_Size[2];        /* Computed Parameter: SFunction1_P2_Size
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P2Size)
                                        */
  real_T SFunction1_P2;                /* Expression: recv_id
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P2)
                                        */
  real_T SFunction1_P3_Size[2];        /* Computed Parameter: SFunction1_P3_Size
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P3Size)
                                        */
  real_T SFunction1_P3;                /* Expression: fp1
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P3)
                                        */
  real_T SFunction1_P4_Size[2];        /* Computed Parameter: SFunction1_P4_Size
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P4Size)
                                        */
  real_T SFunction1_P4;                /* Expression: fp2
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P4)
                                        */
  real_T SFunction1_P5_Size[2];        /* Computed Parameter: SFunction1_P5_Size
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P5Size)
                                        */
  real_T SFunction1_P5;                /* Expression: fp3
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P5)
                                        */
  real_T SFunction1_P6_Size[2];        /* Computed Parameter: SFunction1_P6_Size
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P6Size)
                                        */
  real_T SFunction1_P6;                /* Expression: fp4
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P6)
                                        */
  real_T SFunction1_P7_Size[2];        /* Computed Parameter: SFunction1_P7_Size
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P7Size)
                                        */
  real_T SFunction1_P7;                /* Expression: fp5
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P7)
                                        */
  real_T SFunction1_P8_Size[2];        /* Computed Parameter: SFunction1_P8_Size
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P8Size)
                                        */
  real_T SFunction1_P8[7];             /* Computed Parameter: SFunction1_P8
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P8)
                                        */
  real_T SFunction1_P9_Size[2];        /* Computed Parameter: SFunction1_P9_Size
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P9Size)
                                        */
  real_T SFunction1_P9[7];             /* Computed Parameter: SFunction1_P9
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P9)
                                        */
  real_T SFunction1_P10_Size[2];      /* Computed Parameter: SFunction1_P10_Size
                                       * Referenced by: '<S5>/S-Function1' (Parameter: P10Size)
                                       */
  real_T SFunction1_P10[7];            /* Computed Parameter: SFunction1_P10
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P10)
                                        */
  real_T SFunction1_P11_Size[2];      /* Computed Parameter: SFunction1_P11_Size
                                       * Referenced by: '<S5>/S-Function1' (Parameter: P11Size)
                                       */
  real_T SFunction1_P11[7];            /* Computed Parameter: SFunction1_P11
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P11)
                                        */
  real_T SFunction1_P12_Size[2];      /* Computed Parameter: SFunction1_P12_Size
                                       * Referenced by: '<S5>/S-Function1' (Parameter: P12Size)
                                       */
  real_T SFunction1_P12[7];            /* Computed Parameter: SFunction1_P12
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P12)
                                        */
  real_T SFunction1_P13_Size[2];      /* Computed Parameter: SFunction1_P13_Size
                                       * Referenced by: '<S5>/S-Function1' (Parameter: P13Size)
                                       */
  real_T SFunction1_P13;               /* Computed Parameter: SFunction1_P13
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P13)
                                        */
  real_T SFunction1_P14_Size[2];      /* Computed Parameter: SFunction1_P14_Size
                                       * Referenced by: '<S5>/S-Function1' (Parameter: P14Size)
                                       */
  real_T SFunction1_P14;               /* Computed Parameter: SFunction1_P14
                                        * Referenced by: '<S5>/S-Function1' (Parameter: P14)
                                        */
  real_T SineWaveA_Amp;                /* Expression: Aa
                                        * Referenced by: '<S30>/Sine Wave A' (Parameter: Amplitude)
                                        */
  real_T SineWaveA_Bias;               /* Expression: 0
                                        * Referenced by: '<S30>/Sine Wave A' (Parameter: Bias)
                                        */
  real_T SineWaveA_Hsin;               /* Computed Parameter: SineWaveA_Hsin
                                        * Referenced by: '<S30>/Sine Wave A' (Parameter: SinH)
                                        */
  real_T SineWaveA_HCos;               /* Computed Parameter: SineWaveA_HCos
                                        * Referenced by: '<S30>/Sine Wave A' (Parameter: CosH)
                                        */
  real_T SineWaveA_PSin;               /* Computed Parameter: SineWaveA_PSin
                                        * Referenced by: '<S30>/Sine Wave A' (Parameter: SinPhi)
                                        */
  real_T SineWaveA_PCos;               /* Computed Parameter: SineWaveA_PCos
                                        * Referenced by: '<S30>/Sine Wave A' (Parameter: CosPhi)
                                        */
  real_T SineWaveB_Amp;                /* Expression: Ab
                                        * Referenced by: '<S30>/Sine Wave B' (Parameter: Amplitude)
                                        */
  real_T SineWaveB_Bias;               /* Expression: 0
                                        * Referenced by: '<S30>/Sine Wave B' (Parameter: Bias)
                                        */
  real_T SineWaveB_Hsin;               /* Computed Parameter: SineWaveB_Hsin
                                        * Referenced by: '<S30>/Sine Wave B' (Parameter: SinH)
                                        */
  real_T SineWaveB_HCos;               /* Computed Parameter: SineWaveB_HCos
                                        * Referenced by: '<S30>/Sine Wave B' (Parameter: CosH)
                                        */
  real_T SineWaveB_PSin;               /* Computed Parameter: SineWaveB_PSin
                                        * Referenced by: '<S30>/Sine Wave B' (Parameter: SinPhi)
                                        */
  real_T SineWaveB_PCos;               /* Computed Parameter: SineWaveB_PCos
                                        * Referenced by: '<S30>/Sine Wave B' (Parameter: CosPhi)
                                        */
  real_T SineWaveC_Amp;                /* Expression: Ac
                                        * Referenced by: '<S30>/Sine Wave C' (Parameter: Amplitude)
                                        */
  real_T SineWaveC_Bias;               /* Expression: 0
                                        * Referenced by: '<S30>/Sine Wave C' (Parameter: Bias)
                                        */
  real_T SineWaveC_Hsin;               /* Computed Parameter: SineWaveC_Hsin
                                        * Referenced by: '<S30>/Sine Wave C' (Parameter: SinH)
                                        */
  real_T SineWaveC_HCos;               /* Computed Parameter: SineWaveC_HCos
                                        * Referenced by: '<S30>/Sine Wave C' (Parameter: CosH)
                                        */
  real_T SineWaveC_PSin;               /* Computed Parameter: SineWaveC_PSin
                                        * Referenced by: '<S30>/Sine Wave C' (Parameter: SinPhi)
                                        */
  real_T SineWaveC_PCos;               /* Computed Parameter: SineWaveC_PCos
                                        * Referenced by: '<S30>/Sine Wave C' (Parameter: CosPhi)
                                        */
  real_T StateSpace_DS_param[36];      /* Expression: S.D
                                        * Referenced by: '<S51>/State-Space' (Parameter: DS_param)
                                        */
  real_T donotdeletethisgain_Gain;     /* Expression: 1
                                        * Referenced by: '<S18>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_d;   /* Expression: 1
                                        * Referenced by: '<S19>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_b;   /* Expression: 1
                                        * Referenced by: '<S20>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T Kv1_Gain;                     /* Expression: Kv
                                        * Referenced by: '<S9>/Kv1' (Parameter: Gain)
                                        */
  real_T SineWaveA_Amp_j;              /* Expression: Aa
                                        * Referenced by: '<S48>/Sine Wave A' (Parameter: Amplitude)
                                        */
  real_T SineWaveA_Bias_k;             /* Expression: 0
                                        * Referenced by: '<S48>/Sine Wave A' (Parameter: Bias)
                                        */
  real_T SineWaveA_Hsin_l;             /* Computed Parameter: SineWaveA_Hsin_l
                                        * Referenced by: '<S48>/Sine Wave A' (Parameter: SinH)
                                        */
  real_T SineWaveA_HCos_c;             /* Computed Parameter: SineWaveA_HCos_c
                                        * Referenced by: '<S48>/Sine Wave A' (Parameter: CosH)
                                        */
  real_T SineWaveA_PSin_f;             /* Computed Parameter: SineWaveA_PSin_f
                                        * Referenced by: '<S48>/Sine Wave A' (Parameter: SinPhi)
                                        */
  real_T SineWaveA_PCos_a;             /* Computed Parameter: SineWaveA_PCos_a
                                        * Referenced by: '<S48>/Sine Wave A' (Parameter: CosPhi)
                                        */
  real_T SineWaveB_Amp_a;              /* Expression: Ab
                                        * Referenced by: '<S48>/Sine Wave B' (Parameter: Amplitude)
                                        */
  real_T SineWaveB_Bias_e;             /* Expression: 0
                                        * Referenced by: '<S48>/Sine Wave B' (Parameter: Bias)
                                        */
  real_T SineWaveB_Hsin_g;             /* Computed Parameter: SineWaveB_Hsin_g
                                        * Referenced by: '<S48>/Sine Wave B' (Parameter: SinH)
                                        */
  real_T SineWaveB_HCos_l;             /* Computed Parameter: SineWaveB_HCos_l
                                        * Referenced by: '<S48>/Sine Wave B' (Parameter: CosH)
                                        */
  real_T SineWaveB_PSin_i;             /* Computed Parameter: SineWaveB_PSin_i
                                        * Referenced by: '<S48>/Sine Wave B' (Parameter: SinPhi)
                                        */
  real_T SineWaveB_PCos_f;             /* Computed Parameter: SineWaveB_PCos_f
                                        * Referenced by: '<S48>/Sine Wave B' (Parameter: CosPhi)
                                        */
  real_T SineWaveC_Amp_c;              /* Expression: Ac
                                        * Referenced by: '<S48>/Sine Wave C' (Parameter: Amplitude)
                                        */
  real_T SineWaveC_Bias_e;             /* Expression: 0
                                        * Referenced by: '<S48>/Sine Wave C' (Parameter: Bias)
                                        */
  real_T SineWaveC_Hsin_e;             /* Computed Parameter: SineWaveC_Hsin_e
                                        * Referenced by: '<S48>/Sine Wave C' (Parameter: SinH)
                                        */
  real_T SineWaveC_HCos_j;             /* Computed Parameter: SineWaveC_HCos_j
                                        * Referenced by: '<S48>/Sine Wave C' (Parameter: CosH)
                                        */
  real_T SineWaveC_PSin_b;             /* Computed Parameter: SineWaveC_PSin_b
                                        * Referenced by: '<S48>/Sine Wave C' (Parameter: SinPhi)
                                        */
  real_T SineWaveC_PCos_f;             /* Computed Parameter: SineWaveC_PCos_f
                                        * Referenced by: '<S48>/Sine Wave C' (Parameter: CosPhi)
                                        */
  real_T StateSpace_AS_param[36];      /* Expression: S.A
                                        * Referenced by: '<S52>/State-Space' (Parameter: AS_param)
                                        */
  real_T StateSpace_BS_param[18];      /* Expression: S.B
                                        * Referenced by: '<S52>/State-Space' (Parameter: BS_param)
                                        */
  real_T StateSpace_CS_param[36];      /* Expression: S.C
                                        * Referenced by: '<S52>/State-Space' (Parameter: CS_param)
                                        */
  real_T StateSpace_DS_param_c[18];    /* Expression: S.D
                                        * Referenced by: '<S52>/State-Space' (Parameter: DS_param)
                                        */
  real_T StateSpace_X0_param[6];       /* Expression: S.x0
                                        * Referenced by: '<S52>/State-Space' (Parameter: X0_param)
                                        */
  real_T donotdeletethisgain_Gain_n;   /* Expression: 1
                                        * Referenced by: '<S39>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_a;   /* Expression: 1
                                        * Referenced by: '<S40>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_dr;  /* Expression: 1
                                        * Referenced by: '<S41>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T Kv1_Gain_d;                   /* Expression: Kv
                                        * Referenced by: '<S31>/Kv1' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_k;   /* Expression: 1
                                        * Referenced by: '<S36>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_g;   /* Expression: 1
                                        * Referenced by: '<S37>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_e;   /* Expression: 1
                                        * Referenced by: '<S38>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T Kv_Gain;                      /* Expression: Ki
                                        * Referenced by: '<S31>/Kv' (Parameter: Gain)
                                        */
  real_T SFunction_P1_Size[2];         /* Computed Parameter: SFunction_P1_Size
                                        * Referenced by: '<S50>/S-Function' (Parameter: P1Size)
                                        */
  real_T SFunction_P1;                 /* Expression: Acqu_group
                                        * Referenced by: '<S50>/S-Function' (Parameter: P1)
                                        */
  real_T donotdeletethisgain_Gain_f;   /* Expression: 1
                                        * Referenced by: '<S15>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_j;   /* Expression: 1
                                        * Referenced by: '<S16>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T donotdeletethisgain_Gain_i;   /* Expression: 1
                                        * Referenced by: '<S17>/do not delete this gain' (Parameter: Gain)
                                        */
  real_T Kv_Gain_h;                    /* Expression: Ki
                                        * Referenced by: '<S9>/Kv' (Parameter: Gain)
                                        */
  real_T dataready25kHz_Amp;           /* Expression: 1
                                        * Referenced by: '<S2>/data ready 2.5 kHz' (Parameter: Amplitude)
                                        */
  real_T dataready25kHz_Period;        /* Expression: 2
                                        * Referenced by: '<S2>/data ready 2.5 kHz' (Parameter: Period)
                                        */
  real_T dataready25kHz_Duty;          /* Expression: 2
                                        * Referenced by: '<S2>/data ready 2.5 kHz' (Parameter: PulseWidth)
                                        */
  real_T dataready25kHz_PhaseDelay;    /* Expression: 0
                                        * Referenced by: '<S2>/data ready 2.5 kHz' (Parameter: PhaseDelay)
                                        */
  real_T SFunction2_P1_Size[2];        /* Computed Parameter: SFunction2_P1_Size
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P1Size)
                                        */
  real_T SFunction2_P1;                /* Expression: ctl_id
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P1)
                                        */
  real_T SFunction2_P2_Size[2];        /* Computed Parameter: SFunction2_P2_Size
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P2Size)
                                        */
  real_T SFunction2_P2;                /* Expression: send_id
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P2)
                                        */
  real_T SFunction2_P3_Size[2];        /* Computed Parameter: SFunction2_P3_Size
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P3Size)
                                        */
  real_T SFunction2_P3;                /* Expression: mode
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P3)
                                        */
  real_T SFunction2_P4_Size[2];        /* Computed Parameter: SFunction2_P4_Size
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P4Size)
                                        */
  real_T SFunction2_P4;                /* Expression: fp1
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P4)
                                        */
  real_T SFunction2_P5_Size[2];        /* Computed Parameter: SFunction2_P5_Size
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P5Size)
                                        */
  real_T SFunction2_P5;                /* Expression: fp2
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P5)
                                        */
  real_T SFunction2_P6_Size[2];        /* Computed Parameter: SFunction2_P6_Size
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P6Size)
                                        */
  real_T SFunction2_P6;                /* Expression: fp3
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P6)
                                        */
  real_T SFunction2_P7_Size[2];        /* Computed Parameter: SFunction2_P7_Size
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P7Size)
                                        */
  real_T SFunction2_P7;                /* Expression: fp4
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P7)
                                        */
  real_T SFunction2_P8_Size[2];        /* Computed Parameter: SFunction2_P8_Size
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P8Size)
                                        */
  real_T SFunction2_P8;                /* Expression: fp5
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P8)
                                        */
  real_T SFunction2_P9_Size[2];        /* Computed Parameter: SFunction2_P9_Size
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P9Size)
                                        */
  real_T SFunction2_P9[7];             /* Computed Parameter: SFunction2_P9
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P9)
                                        */
  real_T SFunction2_P10_Size[2];      /* Computed Parameter: SFunction2_P10_Size
                                       * Referenced by: '<S8>/S-Function2' (Parameter: P10Size)
                                       */
  real_T SFunction2_P10[7];            /* Computed Parameter: SFunction2_P10
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P10)
                                        */
  real_T SFunction2_P11_Size[2];      /* Computed Parameter: SFunction2_P11_Size
                                       * Referenced by: '<S8>/S-Function2' (Parameter: P11Size)
                                       */
  real_T SFunction2_P11[7];            /* Computed Parameter: SFunction2_P11
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P11)
                                        */
  real_T SFunction2_P12_Size[2];      /* Computed Parameter: SFunction2_P12_Size
                                       * Referenced by: '<S8>/S-Function2' (Parameter: P12Size)
                                       */
  real_T SFunction2_P12[7];            /* Computed Parameter: SFunction2_P12
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P12)
                                        */
  real_T SFunction2_P13_Size[2];      /* Computed Parameter: SFunction2_P13_Size
                                       * Referenced by: '<S8>/S-Function2' (Parameter: P13Size)
                                       */
  real_T SFunction2_P13[7];            /* Computed Parameter: SFunction2_P13
                                        * Referenced by: '<S8>/S-Function2' (Parameter: P13)
                                        */
  real_T OpIPSocketCtrl_P1_Size[2];/* Computed Parameter: OpIPSocketCtrl_P1_Size
                                    * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P1Size)
                                    */
  real_T OpIPSocketCtrl_P1;            /* Expression: ctl_id
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P1)
                                        */
  real_T OpIPSocketCtrl_P2_Size[2];/* Computed Parameter: OpIPSocketCtrl_P2_Size
                                    * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P2Size)
                                    */
  real_T OpIPSocketCtrl_P2;            /* Expression: proto
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P2)
                                        */
  real_T OpIPSocketCtrl_P3_Size[2];/* Computed Parameter: OpIPSocketCtrl_P3_Size
                                    * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P3Size)
                                    */
  real_T OpIPSocketCtrl_P3;            /* Expression: ip_port_remote
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P3)
                                        */
  real_T OpIPSocketCtrl_P4_Size[2];/* Computed Parameter: OpIPSocketCtrl_P4_Size
                                    * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P4Size)
                                    */
  real_T OpIPSocketCtrl_P4;            /* Expression: ip_port_local
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P4)
                                        */
  real_T OpIPSocketCtrl_P5_Size[2];/* Computed Parameter: OpIPSocketCtrl_P5_Size
                                    * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P5Size)
                                    */
  real_T OpIPSocketCtrl_P5;            /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P5)
                                        */
  real_T OpIPSocketCtrl_P6_Size[2];/* Computed Parameter: OpIPSocketCtrl_P6_Size
                                    * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P6Size)
                                    */
  real_T OpIPSocketCtrl_P6;            /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P6)
                                        */
  real_T OpIPSocketCtrl_P7_Size[2];/* Computed Parameter: OpIPSocketCtrl_P7_Size
                                    * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P7Size)
                                    */
  real_T OpIPSocketCtrl_P7;            /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P7)
                                        */
  real_T OpIPSocketCtrl_P8_Size[2];/* Computed Parameter: OpIPSocketCtrl_P8_Size
                                    * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P8Size)
                                    */
  real_T OpIPSocketCtrl_P8;            /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P8)
                                        */
  real_T OpIPSocketCtrl_P9_Size[2];/* Computed Parameter: OpIPSocketCtrl_P9_Size
                                    * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P9Size)
                                    */
  real_T OpIPSocketCtrl_P9;            /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P9)
                                        */
  real_T OpIPSocketCtrl_P10_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P10_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P10Size)
                                   */
  real_T OpIPSocketCtrl_P10;           /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P10)
                                        */
  real_T OpIPSocketCtrl_P11_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P11_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P11Size)
                                   */
  real_T OpIPSocketCtrl_P11;           /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P11)
                                        */
  real_T OpIPSocketCtrl_P12_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P12_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P12Size)
                                   */
  real_T OpIPSocketCtrl_P12;           /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P12)
                                        */
  real_T OpIPSocketCtrl_P13_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P13_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P13Size)
                                   */
  real_T OpIPSocketCtrl_P13;           /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P13)
                                        */
  real_T OpIPSocketCtrl_P14_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P14_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P14Size)
                                   */
  real_T OpIPSocketCtrl_P14[13];       /* Computed Parameter: OpIPSocketCtrl_P14
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P14)
                                        */
  real_T OpIPSocketCtrl_P15_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P15_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P15Size)
                                   */
  real_T OpIPSocketCtrl_P15[7];        /* Computed Parameter: OpIPSocketCtrl_P15
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P15)
                                        */
  real_T OpIPSocketCtrl_P16_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P16_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P16Size)
                                   */
  real_T OpIPSocketCtrl_P17_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P17_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P17Size)
                                   */
  real_T OpIPSocketCtrl_P18_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P18_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P18Size)
                                   */
  real_T OpIPSocketCtrl_P19_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P19_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P19Size)
                                   */
  real_T OpIPSocketCtrl_P20_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P20_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P20Size)
                                   */
  real_T OpIPSocketCtrl_P21_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P21_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P21Size)
                                   */
  real_T OpIPSocketCtrl_P22_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P22_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P22Size)
                                   */
  real_T OpIPSocketCtrl_P23_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P23_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P23Size)
                                   */
  real_T OpIPSocketCtrl_P24_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P24_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P24Size)
                                   */
  real_T OpIPSocketCtrl_P25_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P25_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P25Size)
                                   */
  real_T OpIPSocketCtrl_P26_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P26_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P26Size)
                                   */
  real_T OpIPSocketCtrl_P26[7];        /* Computed Parameter: OpIPSocketCtrl_P26
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P26)
                                        */
  real_T OpIPSocketCtrl_P27_Size[2];
                                  /* Computed Parameter: OpIPSocketCtrl_P27_Size
                                   * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P27Size)
                                   */
  real_T OpIPSocketCtrl_P27;           /* Expression: 0
                                        * Referenced by: '<S2>/OpIPSocketCtrl' (Parameter: P27)
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_three_phase_circuit__1_sm_master_T {
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
    time_T *taskTimePtrs[1];
    SimStruct childSFunctions[4];
    SimStruct *childSFunctionPtrs[4];
    struct _ssBlkInfo2 blkInfo2[4];
    struct _ssSFcnModelMethods2 methods2[4];
    struct _ssSFcnModelMethods3 methods3[4];
    struct _ssSFcnModelMethods4 methods4[4];
    struct _ssStatesInfo2 statesInfo2[4];
    ssPeriodicStatesInfo periodicStatesInfo[4];
    struct _ssPortInfo2 inputOutputPortInfo2[4];
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
      struct _ssPortInputs inputPortInfo[1];
      struct _ssInPortUnit inputPortUnits[1];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[1];
      real_T const *UPtrs0[15];
      uint_T attribs[1];
      mxArray *params[1];
      struct _ssDWorkRecord dWork[1];
      struct _ssDWorkAuxRecord dWorkAux[1];
    } Sfcn1;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      struct _ssPortInputs inputPortInfo[2];
      struct _ssInPortUnit inputPortUnits[2];
      struct _ssInPortCoSimAttribute inputPortCoSimAttribute[2];
      real_T const *UPtrs0[1];
      real_T const *UPtrs1[3];
      struct _ssPortOutputs outputPortInfo[1];
      struct _ssOutPortUnit outputPortUnits[1];
      struct _ssOutPortCoSimAttribute outputPortCoSimAttribute[1];
      uint_T attribs[13];
      mxArray *params[13];
      struct _ssDWorkRecord dWork[1];
      struct _ssDWorkAuxRecord dWorkAux[1];
    } Sfcn2;

    struct {
      time_T sfcnPeriod[1];
      time_T sfcnOffset[1];
      int_T sfcnTsMap[1];
      uint_T attribs[27];
      mxArray *params[27];
      struct _ssDWorkRecord dWork[1];
      struct _ssDWorkAuxRecord dWorkAux[1];
    } Sfcn3;
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
    time_T sampleTimesArray[1];
    time_T offsetTimesArray[1];
    int_T sampleTimeTaskIDArray[1];
    int_T sampleHitArray[1];
    int_T perTaskSampleHitsArray[1];
    time_T tArray[1];
  } Timing;
};

/* Block parameters (default storage) */
extern P_three_phase_circuit__1_sm_master_T three_phase_circuit__1_sm_master_P;

/* Block signals (default storage) */
extern B_three_phase_circuit__1_sm_master_T three_phase_circuit__1_sm_master_B;

/* Block states (default storage) */
extern DW_three_phase_circuit__1_sm_master_T three_phase_circuit__1_sm_master_DW;

/*====================*
 * External functions *
 *====================*/
extern three_phase_circuit__1_sm_master_rtModel
  *three_phase_circuit__1_sm_master(void);
extern void MdlInitializeSizes(void);
extern void MdlInitializeSampleTimes(void);
extern void MdlInitialize(void);
extern void MdlStart(void);
extern void MdlOutputs(int_T tid);
extern void MdlUpdate(int_T tid);
extern void MdlTerminate(void);

/* Real-time Model object */
extern RT_MODEL_three_phase_circuit__1_sm_master_T *const
  three_phase_circuit__1_sm_master_M;

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
 * '<Root>' : 'three_phase_circuit__1_sm_master'
 * '<S1>'   : 'three_phase_circuit__1_sm_master/OpCCode_do_not_touch'
 * '<S2>'   : 'three_phase_circuit__1_sm_master/SM_master'
 * '<S3>'   : 'three_phase_circuit__1_sm_master/powergui'
 * '<S4>'   : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source'
 * '<S5>'   : 'three_phase_circuit__1_sm_master/SM_master/OpAsyncRecv'
 * '<S6>'   : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit'
 * '<S7>'   : 'three_phase_circuit__1_sm_master/SM_master/rtlab_send_subsystem'
 * '<S8>'   : 'three_phase_circuit__1_sm_master/SM_master/send message 1'
 * '<S9>'   : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement'
 * '<S10>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase Controlled  Y-Connected Current Source'
 * '<S11>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase Source'
 * '<S12>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Mode I'
 * '<S13>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Mode V'
 * '<S14>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model'
 * '<S15>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/I A:'
 * '<S16>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/I B:'
 * '<S17>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/I C:'
 * '<S18>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/U A:'
 * '<S19>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/U B:'
 * '<S20>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/U C:'
 * '<S21>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/I A:/Model'
 * '<S22>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/I B:/Model'
 * '<S23>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/I C:/Model'
 * '<S24>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/U A:/Model'
 * '<S25>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/U B:/Model'
 * '<S26>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase V-I Measurement/Model/U C:/Model'
 * '<S27>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase Controlled  Y-Connected Current Source/Controlled Current Source'
 * '<S28>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase Controlled  Y-Connected Current Source/Controlled Current Source1'
 * '<S29>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase Controlled  Y-Connected Current Source/Controlled Current Source2'
 * '<S30>'  : 'three_phase_circuit__1_sm_master/SM_master/Circuit_ITM_Source/Three-Phase Source/Model'
 * '<S31>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1'
 * '<S32>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase Source1'
 * '<S33>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Mode I'
 * '<S34>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Mode V'
 * '<S35>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model'
 * '<S36>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/I A:'
 * '<S37>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/I B:'
 * '<S38>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/I C:'
 * '<S39>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/U A:'
 * '<S40>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/U B:'
 * '<S41>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/U C:'
 * '<S42>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/I A:/Model'
 * '<S43>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/I B:/Model'
 * '<S44>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/I C:/Model'
 * '<S45>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/U A:/Model'
 * '<S46>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/U B:/Model'
 * '<S47>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase V-I Measurement1/Model/U C:/Model'
 * '<S48>'  : 'three_phase_circuit__1_sm_master/SM_master/Original_Circuit/Three-Phase Source1/Model'
 * '<S49>'  : 'three_phase_circuit__1_sm_master/SM_master/rtlab_send_subsystem/Subsystem1'
 * '<S50>'  : 'three_phase_circuit__1_sm_master/SM_master/rtlab_send_subsystem/Subsystem1/Send1'
 * '<S51>'  : 'three_phase_circuit__1_sm_master/powergui/EquivalentModel1'
 * '<S52>'  : 'three_phase_circuit__1_sm_master/powergui/EquivalentModel2'
 * '<S53>'  : 'three_phase_circuit__1_sm_master/powergui/EquivalentModel1/Sources'
 * '<S54>'  : 'three_phase_circuit__1_sm_master/powergui/EquivalentModel1/Yout'
 * '<S55>'  : 'three_phase_circuit__1_sm_master/powergui/EquivalentModel2/Sources'
 * '<S56>'  : 'three_phase_circuit__1_sm_master/powergui/EquivalentModel2/Yout'
 */
#endif                      /* RTW_HEADER_three_phase_circuit__1_sm_master_h_ */
