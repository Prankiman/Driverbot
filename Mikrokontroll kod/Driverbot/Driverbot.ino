#include "EspMQTTClient.h"
//Install libraries PubSubClient and EspMQTTClient
#include <Servo.h>

Servo servo;

void onConnectionEstablished();

/*  "ABB_Indgym_Guest",           // Wifi ssid
  "Welcome2abb",           // Wifi password
  "maqiatto.com",  // MQTT broker ip
  1883,             // MQTT broker port
  "arvid.kolsmyr@abbindustrigymnasium.se",            // MQTT username
  "apaapaapa",       // MQTT password
  "Microdatowwwr2311",          // Client name
  onConnectionEstablished, // Connection established callback
  true,             // Enable web updater
  true              // Enable debug messages*/

EspMQTTClient client(
   "LAN_ROUTER",           // Wifi ssid ABB_Indgym_Guest
  "LAN_ROUTER",           // Wifi password  Welcome2abb
  "192.168.1.116",  // MQTT broker ip
  "arvid.kolsmyr@abbindustrigymnasium.se",            // MQTT username
  "apaapaapa",       // MQTT password
  "Microdatowwwr2311",             // Client name
  1883
);


#define motorPinRightDir  0//D2
#define motorPinRightSpeed 5//D1

#define motorPinLeftDir 2
#define motorPinLeftSpeed 4

void setup() {
  servo.attach(2); //D4
  pinMode(motorPinRightDir, OUTPUT);
  pinMode(motorPinRightSpeed, OUTPUT);
  pinMode(motorPinLeftDir, OUTPUT);
  pinMode(motorPinLeftSpeed, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  servo.write(0);

  delay(2000);
}

bool off = false;

void turn(bool left, int degrees) {

  if (left == true)
  {
    degrees -= 90;

    if (degrees < 0)
    {
      degrees = 0;
      digitalWrite(motorPinLeftDir, 1);
      analogWrite(motorPinLeftSpeed, 1000);
    }
    servo.write(degrees);
    Serial.println("Åk Vänster!");
  }
  else
  {

    degrees += 90;
    if (degrees > 180)
    {
      degrees = 180;
      digitalWrite(motorPinLeftDir, 1);
      analogWrite(motorPinLeftSpeed, 1000);
    }
    servo.write(degrees);
    Serial.println("Åk Höger!");

  }
  Serial.println(degrees);


}

void drive(bool dir, int speed) {

  //Om du vill åka rakt fram eller bak
  servo.write(90);

  Serial.println("Åk!");

  Serial.println(speed);
  digitalWrite(motorPinLeftDir, dir);
  analogWrite(motorPinLeftSpeed, speed);

  digitalWrite(motorPinRightDir, dir);
  analogWrite(motorPinRightSpeed, speed);

  digitalWrite(LED_BUILTIN, dir);

}

void onConnectionEstablished()
{

  client.subscribe("arvid.kolsmyr@abbindustrigymnasium.se/drive", [] (const String & payload)
  {

    char info = payload.charAt(0);
    int length = payload.length();
    String value = payload.substring(1, length);
    int speed = value.toInt();
    if (info == 'f' || info == 'b'  )
    {
      bool dir = false;
      if (info == 'f')
        dir = true;
      drive(dir, speed);
    }
    else if (info == 'r' || info == 'l'  )
    {
      bool dir = false;
      if (info == 'l')
        dir = true;
      turn(dir, speed);
    }
    Serial.println(payload);

  });

  client.publish("arvid.kolsmyr@abbindustrigymnasium.se/drive", "This is a message");

  client.executeDelayed(5 * 1000, []() {
    client.publish("arvid.kolsmyr@abbindustrigymnasium.se/drive", "This is a message sent 5 seconds later");
  });
}


void loop() {


  // put your main code here, to run repeatedly:
  client.loop();
}
