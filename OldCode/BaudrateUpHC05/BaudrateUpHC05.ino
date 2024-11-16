#include <SoftwareSerial.h>

//SOURCE: https://zalophusdokdo.github.io/iDrawController/bluetooth-device-setup.html

SoftwareSerial BTSerial(9, 7); // RX | TX

//AT+UART=115200,0,0  
//AT+UART=57600,0,0  
void setup() {
 //pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  //digitalWrite(9, HIGH);
  Serial.begin(9600);

  Serial.print("For Arduino Rx use pin ");
  Serial.println(10);
   
  Serial.print("For Arduino Tx use pin ");
  Serial.println(11);  

  Serial.println(" -- Command Reference ---");
  Serial.println("To Read use '?', Like AT+PSWD?");
  Serial.println("AT (simply checks connection)");
  Serial.println("AT+VERSION (requests the firmware verison)");
  Serial.println("AT+ROLE=x (0 =Slave role, 1 =  Master role, 2 = Slave-Loop role  default = 0)");
  Serial.println("AT+NAME=xxxxx (to change name to xxxxx default=HC-05");
  Serial.println("AT+PSWD=nnnn (to change password to 4 digit nnnn default = 1234");
  Serial.println("AT+UART=nnnn,s,p (nnnn=Baud, s=stop bits (0=1, 1=2), p=parity (0=None, 1=Odd, 2=Even)");
  Serial.println("AT+POLAR=a,b (a=PIO8 (LED), b=PIO9 for both 0=low turn on, 1 = high turn on.");  
  Serial.println("AT+ORGL (reset all parameters to defaults)");
  Serial.println("AT+RESET (restarts the HC-05. Will not be in AT mode afterward unless button held");

  Serial.println("Enter AT commands:");
  BTSerial.begin(38400);  // HC-05 default speed in AT command more

  

}

void loop() {
  
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    BTSerial.write(Serial.read());
}
