
void usbthread() {
  while(1) {
    myusb.Task();
    asix1.read();
    fnet_poll();
    fnet_service_poll();
    #ifdef STATS
    LoopedUSB++;
    #endif
  }
}

static void dhcp_cln_callback_updated(fnet_dhcp_cln_desc_t _dhcp_desc, fnet_netif_desc_t netif, void *p ) { //Called when DHCP updates
  struct fnet_dhcp_cln_options current_options;
  fnet_dhcp_cln_get_options(dhcp_desc, &current_options);
  
  uint8_t *ip = (uint8_t*)&current_options.ip_address.s_addr;
  Serial.print("IPAddress: ");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.println((uint8_t)*ip);

  ip = (uint8_t*)&current_options.netmask.s_addr;
  Serial.print("SubnetMask: ");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.println((uint8_t)*ip);

  ip = (uint8_t*)&current_options.gateway.s_addr;
  Serial.print("Gateway: ");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.println((uint8_t)*ip);

  ip = (uint8_t*)&current_options.dhcp_server.s_addr;
  Serial.print("DHCPServer: ");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.print((uint8_t)*ip++);
  Serial.print(".");
  Serial.println((uint8_t)*ip);

  
  Serial.print("State: ");
  Serial.println(fnet_dhcp_cln_get_state(_dhcp_desc));
  Serial.println();
  Serial.println();

  

}

void handleOutput(fnet_netif_t *netif, fnet_netbuf_t *nb) { //Called when a message is sent
    if(nb && (nb->total_length >= FNET_ETH_HDR_SIZE))
    {
        _fnet_netbuf_to_buf(nb, 0u, FNET_NETBUF_COPYALL, sbuf);
    }
  uint8_t* p = sbuf;
//  p += 4;
//  Serial.print("Message Transmitted: ");
//  Serial.println(nb->total_length);
//  const uint8_t* end = p + nb->total_length;
//  while(p < end){
//    if(*p <= 0x0F) Serial.print("0");
//    Serial.print(*p++, HEX);
//    Serial.print(" ");
//  }
//  Serial.println();
//  p = sbuf;
  asix1.sendPacket(p, nb->total_length);
}

void handleRecieve(const uint8_t* data, uint16_t length) { //Called when ASIX gets a message
  uint8_t* p = rbuf;
  const uint8_t* end = data + length;
  length = (data[1] << 8) | data[0];
  data += 6;
//  Serial.print("Message Recieved: ");
  while(data < end){
    *p = *data;
    data++;
//    if(*p <= 0x0F) Serial.print("0");
//    Serial.print(*p, HEX);
//    Serial.print(" ");
    p++;
  }
//  Serial.println();
//  if(rbuf[6] == 0x90) {
//    Serial.print("Length: ");
//    Serial.println(length);
//    Serial.print("Message Recieved: ");
//    for(uint16_t i = 0; i < length; i++){
//      if(rbuf[i] <= 0x0F) Serial.print("0");
//      Serial.print(rbuf[i], HEX);
//      Serial.print(" ");
//    }
//    Serial.println();
//  }
  
  _fnet_eth_input(&fnet_eth0_if, rbuf, length);
}

void handleSetMACAddress(uint8_t * hw_addr) { //Gets calls on initialization
  Serial.print("SetMACAddress: ");
  for(uint8_t i = 0; i < 6; i++) {
    if(hw_addr[i] <= 0x0F) Serial.print("0");
    Serial.print(hw_addr[i], HEX);
  }
  Serial.println();
}

void handleGetMACAddress(fnet_mac_addr_t * hw_addr) { //Gets called everytime a message is sent
  (*hw_addr)[0] = asix1.nodeID[0];
  (*hw_addr)[1] = asix1.nodeID[1];
  (*hw_addr)[2] = asix1.nodeID[2];
  (*hw_addr)[3] = asix1.nodeID[3];
  (*hw_addr)[4] = asix1.nodeID[4];
  (*hw_addr)[5] = asix1.nodeID[5];
//  Serial.print("GetMACAddress: ");
//  for(uint8_t i = 0; i < 6; i++) {
//    if(hw_addr[i] <= 0x0F) Serial.print("0");
//    Serial.print(hw_addr[i], HEX);
//  }
//  Serial.println();
}

void handlePHYRead(fnet_uint32_t reg_addr, fnet_uint16_t *data) { //Could be called
  asix1.readPHY(reg_addr, data);
  Serial.println("PHYRead");
}

void handlePHYWrite(fnet_uint32_t reg_addr, fnet_uint16_t data) { //Could be called
  asix1.writePHY(reg_addr, data);
  Serial.println("PHYWrite");
}

void handleMulticastJoin(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr) { //Called when joining multicast group
  //Not setup yet
  Serial.print("MulticastJoin: ");
  for(uint8_t i = 0; i < 6; i++) {
    if(multicast_addr[i] <= 0x0F) Serial.print("0");
    Serial.print(multicast_addr[i], HEX);
  }
  Serial.println();
}

void handleMulticastLeave(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr) { //Called when leaving multicast group
  //Not setup yet
  Serial.println("MulticastLeave: ");
  for(uint8_t i = 0; i < 6; i++) {
    if(multicast_addr[i] <= 0x0F) Serial.print("0");
    Serial.print(multicast_addr[i], HEX);
  }
  Serial.println();
}
