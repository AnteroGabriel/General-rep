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
  name=1A76F139-61E8-4A01-89D6-591E6E8EB8C7_ConfigF4D88DDB-3E1F-4D6D-B7EF-9BB1B19A2AF6
  m10_signalGroupConfigList=1A76F139-61E8-4A01-89D6-591E6E8EB8C7_ConfigF4D88DDB-3E1F-4D6D-B7EF-9BB1B19A2AF6/SignalGroupConfigList
  parent=/
}
IOConfigListMap<DataLogger::SignalGroupConfig> {
  resizable=1
  uiName=SIGNAL_GROUP_
  name=1A76F139-61E8-4A01-89D6-591E6E8EB8C7_ConfigF4D88DDB-3E1F-4D6D-B7EF-9BB1B19A2AF6/SignalGroupConfigList
  items {
    item {
      IOConfigItem_id=SIGNAL_GROUP_1
      isDeletable=1
      listParent=488D6247-3E48-4459-9CF3-BFB9B594881D-default/SyncExchangerRegistry/C649EC60-EFF2-4A13-84A4-C97BB36548D4/1A76F139-61E8-4A01-89D6-591E6E8EB8C7_ConfigF4D88DDB-3E1F-4D6D-B7EF-9BB1B19A2AF6/SignalGroupConfigList
      instance {
        guid=4B19C97E-ED18-4BAE-995D-A7D8286884D5
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
  parent=1A76F139-61E8-4A01-89D6-591E6E8EB8C7_ConfigF4D88DDB-3E1F-4D6D-B7EF-9BB1B19A2AF6
}