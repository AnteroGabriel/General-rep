OPAL-1.0 Object
TCPUDP::Configuration {
  m00_useRTCore=1
  m01_verbose=1
  m999_isVirtualModeEnabled=0
  name=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694
  streamList=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694/streamList
  parent=/
}
IOConfigListMap<TCPUDP::StreamConfig> {
  resizable=1
  uiName=OPALRT-STREAM_
  name=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694/streamList
  items {
    item {
      IOConfigItem_id=OPALRT-STREAM_1
      isDeletable=1
      listParent=488D6247-3E48-4459-9CF3-BFB9B594881D-default/SyncExchangerRegistry/5F2BE304-113B-4613-9960-161B6652AA6A/1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694/streamList
      instance {
        connectedOffset=0
        guid=72121C99-103F-48D8-9448-E0B399C52915
        m01_protocol=UDP
        m02_tcpMode=Server
        m03_tcpPort=23000
        m03_udpLocalPort=23001
        m04_udpRemotePort=23002
        m05_0_nicName=eth2
        m06_0_localIP=10.10.1.5
        m06_0_nicForceSpeed=Force1000
        m06_ip=192.168.10.16
        m07_decimationFactor=10
        m08_enableTxControl=1
        m09_enableRxControl=1
        m10_swapByteOrder=0
        offset=0
        rxBytesOffset=0
        rxCtrlOffset=1599034446
        rxSize=1
        txBytesOffset=0
        txCtrlOffset=0
        txSize=0
        m100_txDataList=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694/streamList/C75842D8-78F5-4B7E-BB12-710EDB413EC0
        m200_rxDataList=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694/streamList/2854BBEC-7378-4EA1-86DD-4DAE6B379422
      }
    }
  }
  parent=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694
}
IOConfigList<FrameBuilder::DataProperties<FrameBuilder::Profiles::GenericDataStruct>> {
  resizable=1
  name=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694/streamList/C75842D8-78F5-4B7E-BB12-710EDB413EC0
  items {
    item {
      checkInitialValue=1
      m110_name=send
      m130_datatype=Unsigned_8
      m135_vectorSize=1
      m149_byteOffset=0
      m150_bitOffset=0
      m160_numberOfBits=8
      m190_data=0
      m200_valueMin=0
      m210_valueMax=255
    }
  }
  parent=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694/streamList
}
IOConfigList<FrameBuilder::DataProperties<FrameBuilder::Profiles::GenericDataStruct>> {
  resizable=1
  name=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694/streamList/2854BBEC-7378-4EA1-86DD-4DAE6B379422
  items {
    item {
      checkInitialValue=1
      m110_name=recev
      m130_datatype=Unsigned_8
      m135_vectorSize=1
      m149_byteOffset=0
      m150_bitOffset=0
      m160_numberOfBits=8
      m190_data=0
      m200_valueMin=0
      m210_valueMax=255
    }
  }
  parent=1C8D0702-3B10-4000-9BE1-F60AEB7D99E2_Config742B0684-ED3F-412D-BF73-A854AABD7694/streamList
}