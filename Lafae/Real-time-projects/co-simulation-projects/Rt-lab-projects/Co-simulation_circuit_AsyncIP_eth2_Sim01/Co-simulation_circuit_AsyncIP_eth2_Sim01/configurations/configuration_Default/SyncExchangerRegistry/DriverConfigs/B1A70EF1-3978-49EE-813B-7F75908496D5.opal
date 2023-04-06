OPAL-1.0 Object
TCPUDP::Configuration {
  m00_useRTCore=1
  m01_verbose=0
  m999_isVirtualModeEnabled=0
  name=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F
  streamList=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F/streamList
  parent=/
}
IOConfigListMap<TCPUDP::StreamConfig> {
  resizable=1
  uiName=OPALRT-STREAM_
  name=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F/streamList
  items {
    item {
      IOConfigItem_id=OPALRT-STREAM_1
      isDeletable=1
      listParent=488D6247-3E48-4459-9CF3-BFB9B594881D-default/SyncExchangerRegistry/615642FB-65BC-4151-81CE-83BC0C9E3D14/B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F/streamList
      instance {
        connectedOffset=0
        guid=9B2AF23A-25A3-4970-8A2C-D637B352AB68
        m01_protocol=UDP
        m02_tcpMode=Server
        m03_tcpPort=23000
        m03_udpLocalPort=23001
        m04_udpRemotePort=23002
        m05_0_nicName=eth2
        m06_0_localIP=10.10.1.5
        m06_0_nicForceSpeed=Force1000
        m06_ip=192.168.10.16
        m07_decimationFactor=1
        m08_enableTxControl=1
        m09_enableRxControl=1
        m10_swapByteOrder=0
        offset=0
        rxBytesOffset=0
        rxCtrlOffset=0
        rxSize=0
        txBytesOffset=0
        txCtrlOffset=0
        txSize=0
        m100_txDataList=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F/streamList/CCE93E33-A734-4964-B2BF-3A3B59391054
        m200_rxDataList=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F/streamList/B3ECFF37-B3FE-4318-9C48-0BCA6AD77152
      }
    }
  }
  parent=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F
}
IOConfigList<FrameBuilder::DataProperties<FrameBuilder::Profiles::GenericDataStruct>> {
  resizable=1
  name=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F/streamList/CCE93E33-A734-4964-B2BF-3A3B59391054
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
  parent=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F/streamList
}
IOConfigList<FrameBuilder::DataProperties<FrameBuilder::Profiles::GenericDataStruct>> {
  resizable=1
  name=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F/streamList/B3ECFF37-B3FE-4318-9C48-0BCA6AD77152
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
  parent=B1A70EF1-3978-49EE-813B-7F75908496D5_ConfigB30A2680-09DB-4A0B-9757-4E9425ECDA8F/streamList
}