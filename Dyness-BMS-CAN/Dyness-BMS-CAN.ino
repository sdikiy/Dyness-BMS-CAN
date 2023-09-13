#include "STM32_CAN.h"

#define BAUDRATE 750000

STM32_CAN Can( CAN1, ALT ); //Use PB8/9 pins for CAN1.
static CAN_message_t CAN_RX_msg;

void printCanMsg() {
  if ( CAN_RX_msg.flags.extended )
    Serial.print( "E " );
  else
    Serial.print( "S " );
  Serial.print( CAN_RX_msg.id, HEX );

  if ( !CAN_RX_msg.flags.remote ) {
    Serial.print( " " );
    for ( int i = 0; i < CAN_RX_msg.len; i++ ) {
      if ( CAN_RX_msg.buf[i] < 0x10u ) Serial.print("0");
      Serial.print( CAN_RX_msg.buf[i], HEX );
    }
  }

  Serial.println(); //no data for remote request frame
}

HardwareSerial Serial2( PA3, PA2 );
HardwareSerial Serial3( PB11, PB10 );

uint32_t counter = 0;
uint64_t lastMillis = 0;

void setup() {
  pinMode( LED_BUILTIN, OUTPUT );
  Serial.begin( BAUDRATE );
  Serial2.begin( BAUDRATE );
  Serial3.begin( BAUDRATE );
  Serial.println( "Serial 1" );
  Serial2.println( "Serial 2" );
  Serial3.println( "Serial 3" );
  Can.begin();
  Can.enableSilentMode();
  Can.setBaudRate( 500000 ); //500KBPS
}

void loop() {
  if ( Can.read( CAN_RX_msg ) ) {
    counter++;
    digitalWrite( LED_BUILTIN, counter % 2 );

    printCanMsg();
  }

  uint64_t currMillis = millis();
  if ( currMillis - lastMillis > 1000 ) {
    lastMillis = currMillis;
    counter++;
    Serial.println( counter );
    Serial2.println( counter );
    Serial3.println( counter );
    digitalWrite( LED_BUILTIN, counter % 2 );
  }
}
