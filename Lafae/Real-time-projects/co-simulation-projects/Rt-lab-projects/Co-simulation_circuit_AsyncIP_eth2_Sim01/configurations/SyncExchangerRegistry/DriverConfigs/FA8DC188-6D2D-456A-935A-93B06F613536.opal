OPAL-1.0 Object
DataLogger::Configuration {
  m01_recordMode=Automatic
  m05_useRTCore=0
  m06_verbose=0
  m09_noDataLoss=0
  m13_usageMode=Basic
  m14_logLevel=Minimal
  m15_toolPriority=NORMAL
  m17_showDLTConsole=0
  name=FA8DC188-6D2D-456A-935A-93B06F613536_Config84DF013D-2BE0-44C7-B741-E1E8E640196A
  m10_signalGroupConfigList=FA8DC188-6D2D-456A-935A-93B06F613536_Config84DF013D-2BE0-44C7-B741-E1E8E640196A/SignalGroupConfigList
  parent=/
}
IOConfigListMap<DataLogger::SignalGroupConfig> {
  resizable=1
  uiName=SIGNAL_GROUP_
  name=FA8DC188-6D2D-456A-935A-93B06F613536_Config84DF013D-2BE0-44C7-B741-E1E8E640196A/SignalGroupConfigList
  items {
    item {
      IOConfigItem_id=SIGNAL_GROUP_1
      isDeletable=1
      listParent=488D6247-3E48-4459-9CF3-BFB9B594881D-default/SyncExchangerRegistry/2E005EDF-BE58-4120-BF5B-6C9626EA3EFB/FA8DC188-6D2D-456A-935A-93B06F613536_Config84DF013D-2BE0-44C7-B741-E1E8E640196A/SignalGroupConfigList
      instance {
        guid=558CD6B0-15F0-49DC-BE7E-E028FA68C6F0
        m003_recordMode=Inherit
        m006_exportFormat=OPREC
        m007_fileAutoNaming=1
        m010_fileName=data
        m011_decimationFactor=1
        m015_frameLength=1
        m016_frameLengthUnits=Seconds
        m020_nbRecordedFrames=10
        m021_fileLength=10
        m022_fileLengthUnits=Seconds
        m11_showTriggerConfiguration=1
        m12_triggerReferenceValue=0
        m13_triggerMode=Normal
        m14_triggerFunction=Edge
        m15_triggerPolarity=Positive
        m18_preTriggerPercent=0
        m19_triggerHoldoff=0
        m20_triggerSignalPath=
        m35_enableSubFraming=1
        m36_subFrameSizeMillis=10
      }
    }
  }
  parent=FA8DC188-6D2D-456A-935A-93B06F613536_Config84DF013D-2BE0-44C7-B741-E1E8E640196A
}