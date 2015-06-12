/*
 Publishing in the callback 
 
  - connects to an MQTT server
  - subscribes to the topic "inTopic"
  - when a message is received, republishes it to "outTopic"
  
  This example shows how to publish messages within the
  callback function. The callback function header needs to
  be declared before the PubSubClient constructor and the 
  actual callback defined afterwards.
  This ensures the client reference in the callback function
  is valid.
  
*/

#include <SPI.h>
#include <YunClient.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 192, 168, 43, 138 };
byte ip[]     = { 172, 16, 0, 100 };
char charVoltage[20];
char alto[20];
char bajo[20];
int potPin = A0;
int servo = 7;
int valec = 0;

#define INPUT_SIZE 8
char charBuf[8];
// Callback function header

YunClient yunClient;

PubSubClient client(server, 1883, callback, yunClient);

// Set a generic code that will trigger our Blue Led
// think of this as a set of codes for automation you might write
byte triggerBlue[6] = "12345";
byte triggerBlue1[6] = "5432";

// handles messages that are returned from the broker on our subscribed channel
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("New message from broker on topic:");
  Serial.println(topic);
  
  Serial.print("Payload:");
  Serial.write(payload, length);
  
  // Check and see ie34f our payload matches our simple trigger test
  if ((length == 5) & (memcmp(payload, triggerBlue, 5) == 0) )
  {
   digitalWrite(servo, HIGH); // Encendemos el LED
   Serial.println(" prendido");
//delay(2000);
   //digitalWrite(servo, LOW); // Apagamos el LED
  }
  
  
    if ((length == 4) & (memcmp(payload, triggerBlue1, 4) == 0) )
  {
   digitalWrite(servo, LOW); // Encendemos el LED
   Serial.println(" apagado");
//delay(2000);
   //digitalWrite(servo, LOW); // Apagamos el LED
  }
  
    if ((length >=7))
  {
    char message_buff[8];
    int i=0;
    
  for(i=0; i<length; i++) {
    
    message_buff[i] = payload[i];
  }
  String msgString = String(message_buff);
  
  
msgString.toCharArray(charBuf, 8) ;

   
//client.publish("devolucion",charBuf);
// Read each command pair 
  // Read each command pair 
  
  // Get next command from Serial (add 1 for final 0)
// Add the final 0 to end the C string
char* command = charBuf;

//char* command = strtok(charBuf, "&");
//while (command != 0)
//{
    // Split the command in two values
    char* separator = strchr(command, ':');
    if (separator != 0)
    {
        // Actually split the string in 2: replace ':' with 0
        *separator = 0;
        int servoId = atoi(command);
        ++separator;
        int position1 = atoi(separator);
dtostrf(servoId,5,2,bajo);
dtostrf(position1,5,2,alto);
       client.publish("devolucion",bajo);
       client.publish("devolucion",alto);
      int a = atoi (bajo);
       int b = atoi (alto);
       
       digitalWrite(servo, LOW);
       delay(a);
       digitalWrite(servo, HIGH);
       delay(b);
       
    }
    // Find the next command in input string
  //  command = strtok(0, "&");
//}

  
  

  }
  


}

void setup()
{
  Serial.begin(9600);
  pinMode(servo, OUTPUT);
  Bridge.begin();
  
  if (client.connect("arduinoClient")) {
    client.publish("otro","hello world");
    client.subscribe("auth");
  }
}

void loop()
{
  
   int pot = analogRead(potPin); 
   //Serial.println(pot);
float voltage = pot * (5.0 / 1024); 
dtostrf(voltage,5,2,charVoltage);
client.publish("iduser",charVoltage);
   
   Bridge.begin();
   //Serial.println("recibe");
  // valec = digitalRead(lector);  // leemos el estado del pulsador
  //if (valec == LOW) {         // verificamos si el valor es alto (HIGH)
                             // que corresponde al boton sin pulsar
   //client.publish("iduser","03765");  // Apagamos el LED
    //Serial.println("button");
  //} 

client.loop();
delay(300); 
}

