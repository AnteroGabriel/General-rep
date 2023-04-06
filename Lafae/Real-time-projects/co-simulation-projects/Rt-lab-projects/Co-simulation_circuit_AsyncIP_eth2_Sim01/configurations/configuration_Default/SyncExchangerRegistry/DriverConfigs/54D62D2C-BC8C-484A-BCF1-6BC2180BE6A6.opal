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
  name=54D62D2C-BC8C-484A-BCF1-6BC2180BE6A6_ConfigDDFCFB8C-B24D-4A18-9CF3-5C672B511403
  m10_signalGroupConfigList=54D62D2C-BC8C-484A-BCF1-6BC2180BE6A6_ConfigDDFCFB8C-B24D-4A18-9CF3-5C672B511403/SignalGroupConfigList
  parent=/
}
IOConfigListMap<DataLogger::SignalGroupConfig> {
  resizable=1
  uiName=SIGNAL_GROUP_
  name=54D62D2C-BC8C-484A-BCF1-6BC2180BE6A6_ConfigDDFCFB8C-B24D-4A18-9CF3-5C672B511403/SignalGroupConfigList
  items {
    item {
      IOConfigItem_id=SIGNAL_GROUP_1
      isDeletable=1
      listParent=488D6247-3E48-4459-9CF3-BFB9B594881D-default/SyncExchangerRegistry/3C6D5B30-9181-439E-84B6-7C60488011F6/54D62D2C-BC8C-484A-BCF1-6BC2180BE6A6_ConfigDDFCFB8C-B24D-4A18-9CF3-5C672B511403/SignalGroupConfigList
      instance {
        guid=EF42E7B1-8B87-4A3A-9783-CA6BE2CDD138
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
  parent=54D62D2C-BC8C-484A-BCF1-6BC2180BE6A6_ConfigDDFCFB8C-B24D-4A18-9CF3-5C672B511403
}