/* Arduino Keyboard HID driver and USB host shield pass through */

/*-
 * Copyright (c) 2011 Darran Hunt (darran [at] hunt dot net dot nz)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Spi.h>
#include <Max3421e.h>
#include <Usb.h>

/* keyboard data taken from configuration descriptor */
#define KBD_ADDR        1
#define KBD_EP          1
#define KBD_IF          0
#define EP_MAXPKTSIZE   8
#define EP_POLL         0x0a

/* Sticky keys */
#define CAPSLOCK    0x39
#define NUMLOCK     0x53
#define SCROLLLOCK  0x47

/* Sticky keys output report bitmasks */
#define REP_NUMLOCK       0x01
#define REP_CAPSLOCK      0x02
#define REP_SCROLLLOCK    0x04

EP_RECORD ep_record[2];  //endpoint record structure for the keyboard

uint8_t buf[8] = { 0 };      // keyboard buffer
uint8_t old_buf[8] = { 0 };  // last poll

//esc, f1 - f12
byte line0[] = {41,58,59,60,61,63,64,65,66,67,68,69};
//~123..90-
byte line1[] = {53,30,31,32,33,34,35,36,37,38,39,45,46,42};
//tab, qwerty..p[
byte line2[] = {43,20,26,80,21,23,28,24,12,18,19,47};
//caps lock, adf..;'
byte line3[] = {57,4,22,7,9,10,11,13,14,15,51,52};
//zxcvbnm,./
byte line4[] = {29,27,60,25,5,17,16,54,55,56};

byte backspace=42;
byte space=44;
byte del=76;
byte enter=40;
byte rwkey[8]={8,0,0,0,0,0,0,0};
byte rctrl[8]={1,0,0,0,0,0,0,0};
byte ralt[8]={4,0,0,0,0,0,0,0};
byte ctraltdelete[]={5,0,76,0,0,0,0,0};

byte windows_run[]={8,0,21,0,0,0,0,0};

/* Sticky key state */
bool numLock = false;
bool capsLock = false;
bool scrollLock = false;

void setup();
void loop();
bool key_is_new(byte key, byte *report);

MAX3421E Max;
USB Usb;

long randNumber;
int keycount = 180;
int leave_alone = 30;

void setup() 
{
    Serial.begin(9600);
    Max.powerOn();
    delay(200);
    randomSeed(analogRead(0));
    
    
}

void loop() 
{
    Max.Task();
    Usb.Task();

    if (Usb.getUsbTaskState() == USB_STATE_CONFIGURING) {
        kbd_init();
        Usb.setUsbTaskState( USB_STATE_RUNNING);
    }

    if (Usb.getUsbTaskState() == USB_STATE_RUNNING) {
        kbd_poll();
    }
}

/* Initialize keyboard */
void kbd_init( void )
{
    byte rcode = 0;  //return code

    /* Initialize data structures */
    ep_record[0] = *(Usb.getDevTableEntry(0,0));  //copy endpoint 0 parameters
    ep_record[1].MaxPktSize = EP_MAXPKTSIZE;
    ep_record[1].Interval  = EP_POLL;
    ep_record[1].sndToggle = bmSNDTOG0;
    ep_record[1].rcvToggle = bmRCVTOG0;
    Usb.setDevTableEntry(1, ep_record);              //plug kbd.endpoint parameters to devtable

    /* Configure device */
    rcode = Usb.setConf(KBD_ADDR, 0, 1);                    
    if (rcode) {
        while(1);  //stop
    }
    delay(2000);
}

/* Poll USB keyboard for new key presses, send through to host via 
 * the Keyboard HID driver in the atmega8u2
 */
void kbd_poll(void)
{
    char i;
    byte rcode = 0;     //return code
    uint8_t ledReport;
    static uint8_t lastLedReport = 0;
    static uint8_t leds = 0;
    static uint8_t lastLeds = 0;
    
    /* poll keyboard */
    rcode = Usb.inTransfer(KBD_ADDR, KBD_EP, 8, (char *)buf);
    if (rcode != 0) {
	return;
    }

    /* Check for change */
    for (i=0; i<8; i++) {
        if (buf[i] != old_buf[i]) {
          
          if (leave_alone > 0)
            leave_alone--;
          if (leave_alone <= 0)
          {
             if (keycount > 5)
              keycount--;
            
            
            if (keycount == 10)
            {            
               //notepad
               byte keyup[]={0,0,0,0,0,0,0,0};
             
             byte n[]={0,0,17,0,0,0,0,0};
             byte o[]={0,0,18,0,0,0,0,0};
             byte t[]={0,0,23,0,0,0,0,0};
             byte e[]={0,0,8,0,0,0,0,0};
             byte p[]={0,0,19,0,0,0,0,0};
             byte a[]={0,0,4,0,0,0,0,0};
             byte d[]={0,0,7,0,0,0,0,0};
             byte enter[]={0,0,40,0,0,0,0,0};
             
             byte H[]={2, 0, 11, 0, 0, 0, 0, 0};
             byte i[]={0, 0, 12, 0, 0, 0, 0, 0};
             byte exclamation[]={2, 0, 30, 0, 0, 0, 0, 0};

               Serial.write(windows_run, 8);
               delay(50);
               Serial.write(keyup, 8);
               
             
               delay(400);
               Serial.write(n, 8);
               delay(50);
               Serial.write(keyup, 8);
               
               delay(40);
               Serial.write(o, 8);
               delay(50);
               Serial.write(keyup, 8);
               
               delay(40);
               Serial.write(t, 8);
               delay(50);
               Serial.write(keyup, 8);
               
               delay(40);
               Serial.write(e, 8);
               delay(50);
               Serial.write(keyup, 8);
               
               delay(40);
               Serial.write(p, 8);
               delay(50);
               Serial.write(keyup, 8);
               
               delay(40);
               Serial.write(a, 8);
               delay(50);
               Serial.write(keyup, 8);
               
               delay(40);
               Serial.write(d, 8);
               delay(50);
               Serial.write(keyup, 8);
               
               delay(40);
               Serial.write(enter, 8);
               delay(50);
               Serial.write(keyup, 8);               
               
               delay(3000);
               Serial.write(H, 8);
               delay(50);
               Serial.write(keyup, 8);               
               delay(40);
               Serial.write(i, 8);
               delay(50);
               Serial.write(keyup, 8);               
               delay(50);
               Serial.write(exclamation, 8);               
               delay(50);
               Serial.write(keyup, 8);               
               delay(4000);
               
               Serial.write(enter, 8);
               delay(50);
               Serial.write(keyup, 8);               
               
            }
            
            
            randNumber = random(keycount);
            //Serial.println(keycount);
            //Serial.println(randNumber);
            
            if (randNumber < 3 && keycount > 10)
            {
            
              //Serial.println("hitme");
              
              for (int i=0;i<12;i++)
              {
                if(line2[i] == buf[2])
                {
                   if (i > 0)
                     buf[2] = line2[i-1];
                   break;
                }
                
                if(line3[i] == buf[2])
                {
                   if (i > 0)
                     buf[2] = line3[i - 1];
                   break;
                }
                
                if ((i < 10)&&(line4[i] == buf[2]))
                {
                   if (i > 0)
                     buf[2] = line4[i - 1];
                   break;
                  
                }
              }
            }            
          }
          
            //switch M and N
          /*  if (buf[2] == 17)
            {
              buf[2] = 16;
            }
            else
            {
              if (buf[2] == 16)
              buf[2] = 17;  
            }
            */
	    /* Send the new key presses to the host */
	    if (keycount != 10)
              Serial.write(buf, 8);
            /*
            for(int i; i < 8; i++)
            {
              Serial.print(buf[i], DEC);
              Serial.print(' ');
            }
            Serial.println(' ');
            /*

	    /* Get led report */
	    ledReport = Serial.read();
#if 0
            /* Not working yet, ledReport is always 0  */
	    if (ledReport != lastLedReport) {
		rcode = Usb.setReport( KBD_ADDR, 0, 1, KBD_IF, 0x02, 0, (char *)&ledReport );
		lastLedReport = ledReport;
	    }
#endif
            break;
	}
    }

    /* Check for status keys and adjust led status */
    for (i=2; i<8; i++) {
	if (buf[i] == 0) {
	    /* 0 marks end of keys in the report */
	    break;
	}
	/* Check new keys for status change keys */
	if (key_is_new(buf[i], old_buf)) {
	    switch (buf[i]) {
	    case CAPSLOCK:
		capsLock =! capsLock;
		leds = capsLock ? leds |= REP_CAPSLOCK : leds &= ~REP_CAPSLOCK;
		break;
	    case NUMLOCK:
		numLock =! numLock;
		leds = numLock ? leds |= REP_NUMLOCK : leds &= ~REP_NUMLOCK;
		break;
	    case SCROLLLOCK:
		scrollLock =! scrollLock;
		leds = scrollLock ? leds |= REP_SCROLLLOCK : leds &= ~REP_SCROLLLOCK;
		break;
	    default:
	        break;
	    }
	}
    }

    /* Got a change in LED status? */
    if (lastLeds != leds) {
	Usb.setReport( KBD_ADDR, 0, 1, KBD_IF, 0x02, 0, (char *)&leds );
	lastLeds = leds;
    }

    /* update the old buffer */
    for (i=0; i<8; i++) {
	old_buf[i] = buf[i];
    }

}

/*
* Function:    key_is_new(key, report)
* Description: see if key is new or is already in report
* Returns:     false if found, true if not
*/
bool key_is_new(uint8_t key, uint8_t *report)
{
    uint8_t ind;
    for (ind=2; ind<8; ind++) {
	if (report[ind] == key) {
	    return false;
	}
    }
    return true;
}
