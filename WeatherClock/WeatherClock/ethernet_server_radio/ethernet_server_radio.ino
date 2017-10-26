// This is a demo of the RBBB running as webserver with the Ether Card
// 2010-05-28 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>



#include <VirtualWire.h>
char *controller;


// ethernet interface mac address, must be unique on the LAN
//static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,0,203 };

//byte Ethernet::buffer[500];
BufferFiller bfill;

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[1200];
static uint32_t timer;

void setup1 () {
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println( "Failed to access Ethernet controller");
  ether.staticSetup(myip);
  
}

//const char website[] PROGMEM = "www.yahoo.com";

//static byte website[] = { 192,168,0,6 };

const char website[] PROGMEM = "ownmeca0.w15.wh-2.com";
///weatherclock/t.html

void setup () {
  
  setup2();
  Serial.begin(9600);
  Serial.println(F("\n[webClient]"));

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  



  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
  
  ether.printIp("SRV: ", ether.hisip);

}


void setup2() {
  //pinMode(13,OUTPUT);
vw_set_ptt_inverted(true); //
vw_set_tx_pin(A0);
vw_setup(4000);// speed of data transfer Kbps
}



static word homePage() {
  long t = millis() / 1000;
  word h = t / 3600;
  byte m = (t / 60) % 60;
  byte s = t % 60;
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='1'/>"
    "<title>RBBB server</title>"
    "<h1>$D$D:$D$D:$D$D</h1>"),
      h/10, h%10, m/10, m%10, s/10, s%10);
  return bfill.position();
}

void loop1 () {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if (pos)  // check if valid tcp data is received
    ether.httpServerReply(homePage()); // send web page data
}


// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}


void loop () {
  ether.packetLoop(ether.packetReceive());
  
  if (millis() > timer) {
    timer = millis() + 5000;
    Serial.println();
    Serial.print("<<< REQ ");
    //ether.browseUrl(PSTR("/foo/"), "bar", website, my_callback);
    ether.browseUrl(PSTR("/weatherclock/"), "t.html", website, my_callback);

send("T-40");
delay(1000);
send("W0");
delay(1000);
send("T0");
delay(1000);
send("W3");
/*
delay(1000);
send("T40");
delay(1000);
send("W9");
delay(1000);
*/
  }

}


void send (char *message)
{
vw_send((uint8_t *)message, strlen(message));
vw_wait_tx(); // Wait until the whole message is gone
}



