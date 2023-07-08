
// My name is Eloi stree, this code is a wrapper of XInput.h
// Source XInput: https://github.com/dmadison/ArduinoXInput

// Note that I use Arduino but I am not medior or senior in the topic.
// This code works but is not guaranty to not bug fail or be not optimized.
// Feel free to ping me on Discord if you want to help me improve it.
// https://eloi.page.link/discord
// This code is under beerware license: If you like it, feel free to send me a beer or support my works.
// https://eloi.page.link/beer

#include <XInput.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <math.h>


const int maxCommandSize=14;


bool m_targetXInput = false;

//To Tweakable value
bool m_debugModeSerial = true;       // Will send serial message to debug
bool m_wantToSendXboxReport = true;  //To put on false when you debug on none XInput board.

//Arduino Leonardo
//const byte m_rxPin = 9; // RX/TX pin to the TTL or HC06
//const byte m_txPin = 8; // RX/TX pin to the TTL or HC06

//Adruino MICRO PRO
const byte m_rxPin = 9;                     // RX/TX pin to the TTL or HC06
const byte m_txPin = 7;                     // RX/TX pin to the TTL or HC06
SoftwareSerial BTserial(m_rxPin, m_txPin);  // RX //TX
// Note that this code is not design to work without a TTL/HC06
// Note that you can't use classic serial port as it is used to simulate the x360.



const int m_messageBufferMax=240;
char  m_messageBuffer [240] ;
//PRIVATE
const int JoyMax = 32767;
const int TriggerMax = 255;
// Some temp value used in the code.
bool m_up;
bool m_down;
bool m_right;
bool m_left;
float m_jlh = 0;
float m_jlv = 0;
float m_jrh = 0;
float m_jrv = 0;
bool m_newInputFound;
String m_receivedMessageSerial = "";
String m_receveivedToProcess = "";
String m_debugLine = "--------------------------------------------------------------------------------";
String m_joystickReport;




///////////////////////////////////::::


class CharArrayTarget {
private:
  char* m_array;
  int m_start;
  int m_end;
  int m_lenght;

public:

 CharArrayTarget() {
    m_array = NULL;
    m_start=0;
    m_end=0;
    m_lenght=0;
  }
  // Constructor
  CharArrayTarget(char* array, int start, int end) {
    m_array = array;
    m_start=start;
    m_end=end;
    m_lenght=end-start;
  }
 void set(char* array, int start, int end) {
    m_array = array;
    m_start=start;
    m_end=end;
    m_lenght=end-start;
  }
  int getAsInt(char c){

    if(c=='0') return 0;
    if(c=='1') return 1;
    if(c=='2') return 2;
    if(c=='3') return 3;
    if(c=='4') return 4;
    if(c=='5') return 5;
    if(c=='6') return 6;
    if(c=='7') return 7;
    if(c=='8') return 8;
    if(c=='9') return 9;
    return 0;

  }

int powdecimal(int pow){
  int value =1;
  if(pow==0)
    return value;
  for(int i = pow; i >0; i--) 
    value*= 10;
    return value;

}
  int getAsInt(){
     int value = 0 ;
     for(int i = 0; i <m_lenght; i++){
       char c = GetChar(m_lenght-1-i);
      // Serial.print(i);
      // Serial.print(" ");
      //  Serial.print(c);
      //  Serial.print(" * ");
      //  Serial.println(String(powdecimal(i)));
       value += getAsInt( c ) * powdecimal(i) ;
      
     }
      //  Serial.print(" R ");
      //  Serial.println(value);
     return value;
  }
 

  void trim() {
    while( (m_array[m_start]==' ' || m_array[m_start]=='\n') && m_start<m_end)
      m_start+=1;
      
    while( (m_array[m_end]==' ' || m_array[m_end]=='\n') && m_end>m_start )
      m_end-=1;
      
    computerLenght();
  }
  bool endwith(char c ) { 
   
    return m_array[m_end]==c ;
  } 
  bool startwith(char c ) {
   
    return m_array[m_start]==c ;
  }
  void removelastchar() {
    m_end-=1;
    computerLenght();
  } 
  void removefirstchar() {
    m_start+=1;
    computerLenght();
  }
  void computerLenght(){

    m_lenght=(m_end-m_start)+1;

  }

  char* getArrayStart() {
    return m_array[m_start];
  }
  char* getArrayEnd() {
    return m_array[m_end];
  }
  int getArrayLength() {
    return m_lenght;
  }

  char operator[](int index) {
    if (index >= 0 && index < m_lenght) {
      return m_array[m_start + index];
    } else {
      Serial.println("Index out of bounds");
      return '\0';
    }
  }
  char GetChar(int index) {
    if (index >= 0 && index < m_lenght) {
      return m_array[m_start + index];
    } else {
      Serial.println("Index out of bounds");
      return '\0';
    }
  } 
  void SetChar(int index, char c) {
    if (index >= 0 && index < m_lenght)
      m_array[m_start + index]=c;
  }

  

  // Get the character at the specified index
  char getCharAtIndex(int index) {
    if (index >= 0 && index < m_array) {
      return m_array[m_start + index];
    } else {
      // Handle out-of-bounds access gracefully
      Serial.println("Index out of bounds");
      // You can return a default value or handle the error as needed
      // Here, we return the null character
      return '\0';
    }
  }
};


class DelayMessage {
public:
  DelayMessage() {
    flush();
    m_timestamp = millis();
    m_hadBedRelay=false;
  }

  void set_command_at(CharArrayTarget command, unsigned long timestamp) {

    for (int i = 0; i < maxCommandSize  ; i++) {
      if(i < command.getArrayLength())
          m_command[i]=command[i];
      else 
          m_command[i]=' ';
    }
    m_timestamp = timestamp;
    m_hadBedRelay=false;
  }

  void set_command(String command, unsigned long millisecond) {
      for (int i = 0; i < maxCommandSize  ; i++) {
      if(i < command.length())
          m_command[i]=command[i];
      else 
          m_command[i]=' ';
    }
    m_timestamp = millis() + millisecond;
    m_hadBedRelay=false;
  }

  void set_command_at(String command, unsigned long timestamp) {

    for (int i = 0; i < maxCommandSize  ; i++) {
      if(i < command.length())
          m_command[i]=command[i];
      else 
          m_command[i]=' ';
    }
    m_timestamp = timestamp;
    m_hadBedRelay=false;
  }

  void set_as_relayed() {
    m_hadBedRelay=true;
  }
  bool had_been_relay(){
    return m_hadBedRelay;

  }


  bool is_time_reach() {
    return millis() >= m_timestamp;
  }

  void flush(){
     for (int i = 0; i < maxCommandSize ; i++) {
       m_command[i]=' ';
    }

  }

  char  m_command [14];
  unsigned long m_timestamp;
  bool m_hadBedRelay;



};

const int maxIndex = 40;
DelayMessage delayArray[maxIndex];






///////////////////////////////:


void DebugLogWaiting(){

      BTserial.println("------------START-------------");
      BTserial.println("Index, is dealed, is reached, command, timestamp");
      for (int i = 0; i < maxIndex; i++) {
            
              BTserial.println();
              BTserial.print(i);
              BTserial.print(" - ");
              BTserial.print(delayArray[i].had_been_relay());
              BTserial.print(" - ");
              BTserial.print(delayArray[i].is_time_reach());
              BTserial.print(" - ");
              BTserial.print(delayArray[i].m_command);
              BTserial.print(" - ");
              BTserial.print(delayArray[i].m_timestamp);
              
            
        }
        BTserial.println("-----------END--------------");
    

}



/////////////////////////////////////////////////////////////////////////////////////:



bool usePrintLog = true;

void add_delay_command(String shortcut,  unsigned long  msToAdd) {
    for (int i = 0; i < maxIndex; i++) {
        if (delayArray[i].had_been_relay()) {
            delayArray[i].set_command(shortcut, msToAdd);
            return;
        }
    }
}

void add_time_command(String shortcut, unsigned long timestamp) {
    for (int i = 0; i < maxIndex; i++) {
        if (delayArray[i].had_been_relay()) {
            delayArray[i].set_command_at(shortcut, timestamp);
            return;
        }
    }
}
void add_time_command(CharArrayTarget shortcut, unsigned long timestamp) {
    for (int i = 0; i < maxIndex; i++) {
        if (delayArray[i].had_been_relay()) {
            delayArray[i].set_command_at(shortcut, timestamp);
            return;
        }
    }
}

/////////////////////
int charToInt(char c) {
  switch (c) {
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case '0':
      return 0;
    default:
      return -1;
  }
}

/////////////////////





//////////////////////////////////////:::














// Some function

bool StartBy4(String text, char c1, char c2, char c3, char c4) {

  return text[0] == c1 && text[1] == c2 && text[2] == c3 && text[3] == c4;
}
bool StartBy3(String text, char c1, char c2, char c3) {

  return text[0] == c1 && text[1] == c2 && text[2] == c3;
}
bool StartBy2(String text, char c1, char c2) {

  return text[0] == c1 && text[1] == c2 ;
}
bool EndWith(String text, char c) {

  return text[ text.length() - 1 ] == c;
}
float GetFloatFromPercent(String text) {
  int start = text.indexOf('%') + 1;
  int end = text.length();
  String integerValue = text.substring(start, end);
  return integerValue.toFloat();
}


String GetDebugLineReport() {
  m_debugLine[0] = 'l';
  m_debugLine[2] = XInput.getButton(BUTTON_LOGO) ? '1' : '0';
  m_debugLine[3] = ' ';
  m_debugLine[4] = 'a';
  m_debugLine[5] = XInput.getDpad(DPAD_UP) ? '1' : '0';
  m_debugLine[6] = XInput.getDpad(DPAD_DOWN) ? '1' : '0';
  m_debugLine[7] = XInput.getDpad(DPAD_LEFT) ? '1' : '0';
  m_debugLine[8] = XInput.getDpad(DPAD_RIGHT) ? '1' : '0';
  m_debugLine[9] = ' ';
  m_debugLine[10] = 'b';
  m_debugLine[12] = XInput.getButton(BUTTON_A) ? '1' : '0';
  m_debugLine[13] = XInput.getButton(BUTTON_B) ? '1' : '0';
  m_debugLine[14] = XInput.getButton(BUTTON_X) ? '1' : '0';
  m_debugLine[15] = XInput.getButton(BUTTON_Y) ? '1' : '0';
  m_debugLine[16] = ' ';
  m_debugLine[17] = 's';
  m_debugLine[18] = XInput.getButton(BUTTON_LB) ? '1' : '0';
  m_debugLine[19] = XInput.getButton(BUTTON_RB) ? '1' : '0';
  m_debugLine[20] = ' ';
  m_debugLine[22] = 'm';
  m_debugLine[23] = XInput.getButton(BUTTON_BACK) ? '1' : '0';
  m_debugLine[24] = XInput.getButton(BUTTON_START) ? '1' : '0';
  m_debugLine[25] = ' ';
  m_debugLine[26] = 't';
  m_debugLine[27] = XInput.getTrigger(TRIGGER_LEFT) ? '1' : '0';
  m_debugLine[28] = XInput.getTrigger(TRIGGER_RIGHT) ? '1' : '0';
  m_debugLine[29] = ' ';
  m_debugLine[30] = 'j';
  m_debugLine[31] = XInput.getButton(BUTTON_L3) ? '1' : '0';
  m_debugLine[32] = XInput.getButton(BUTTON_R3) ? '1' : '0';
  return m_debugLine;
}
String GetDebugJoystickLineReport() {

  m_joystickReport = "";
  m_joystickReport += " JLH " + String(XInput.getJoystickX(JOY_LEFT));
  m_joystickReport += " JLV " + String(XInput.getJoystickY(JOY_LEFT));
  m_joystickReport += " JRH " + String(XInput.getJoystickX(JOY_RIGHT));
  m_joystickReport += " JRV " + String(XInput.getJoystickY(JOY_RIGHT));
  m_joystickReport += " TL " + String(XInput.getTrigger(TRIGGER_LEFT));
  m_joystickReport += " TR " + String(XInput.getTrigger(TRIGGER_RIGHT));
  return m_joystickReport;
}


String toLowerCase(String str) {
  String lowerCaseStr = "";
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (c >= 'A' && c <= 'Z') {
      c = c + 32; // Convert uppercase to lowercase
    }
    lowerCaseStr += c;
  }
  return lowerCaseStr;
}

String trim(String str) {
  int startIdx = 0;
  int endIdx = str.length() - 1;
  while (startIdx <= endIdx && str.charAt(startIdx) == ' ') {
    startIdx++;
  }
  while (endIdx >= startIdx && str.charAt(endIdx) == ' ') {
    endIdx--;
  }
  return str.substring(startIdx, endIdx + 1);
}







const int ADC_Max = 1023;  // 10 bit


void OverrideBuffer(){
   for (int i = 0; i < m_messageBufferMax; i++) {     
               m_messageBuffer[i]=';';
   }
}

int GetIndexofLineEnd(){
  for (int i = 0; i < m_messageBufferMax; i++) {     
          if(     m_messageBuffer[i]==';')
            return i;
   }
   return -1;
}

void FlushBuffer(char c){

  for (int i = 0; i < m_messageBufferMax; i++) {
    
    m_messageBuffer[i]='c';
   }
}void FlushBuffer(){

  for (int i = 0; i < m_messageBufferMax; i++) {
    
    m_messageBuffer[i]=';';
   }
}

void SerialDebugDebuff(){

  for (int i = 0; i < m_messageBufferMax; i++) {
    
    Serial.print(m_messageBuffer[i]);     
    BTserial.print(m_messageBuffer[i]);
   }
    Serial.println();
    BTserial.println();

}
void Log(CharArrayTarget target){

  for (int i = 0; i < target.getArrayLength(); i++) {
    
    Serial.print(target[i]);     
    BTserial.print(target[i]);
   }
    Serial.println();
    BTserial.println();

}

// At start of the Arduino, we check that the serial is connected.
void setup() {

  XInput.setJoystickRange(0, ADC_Max); 
  BTserial.begin(9600);
  Serial.begin(9600);
  while (!Serial) { ; }
  //while (!BTserial) { ; }
  delay(500);
  BTserial.println("Serial available !");
  delay(500);
  XInput.setAutoSend(false);
  XInput.begin();
  FlushBuffer();
    SerialDebugDebuff();
    for (int i = 0; i < maxIndex; i++) { 
        
                delayArray[i].set_as_relayed();
            
    } 
    m_messageBuffer[0]='0';
    m_messageBuffer[1]='1';
    m_messageBuffer[2]='2';
    m_messageBuffer[3]='3';
    m_messageBuffer[4]='4';
    SerialDebugDebuff();
CharArrayTarget chunkTarget = CharArrayTarget(m_messageBuffer, 0,3);
add_time_command(chunkTarget, millis()+2000);

DebugLogWaiting();
Log(chunkTarget);


}

void ExecutionLog(String cmd){

            BTserial.print("Exe|");
            BTserial.print(cmd);
            BTserial.print("|");
            BTserial.println((millis()));
}
void ExecutionLog(DelayMessage cmd){

            BTserial.print("Exe|");
            BTserial.print(cmd.m_command);
            BTserial.print("|");
            BTserial.print((millis()));
            BTserial.print(" - ");
            BTserial.println((cmd.m_timestamp));
}




void Replace(CharArrayTarget message, char toReplace, char by){
  for (int i = 0; i < message.getArrayLength(); i++) {     
          if(message.GetChar(i)==toReplace)
            message.SetChar(i,by);
  }
}
void ToLower(CharArrayTarget message){
  for (int i = 0; i < message.getArrayLength(); i++) {     
    if(message.GetChar(i)=='A') message.SetChar(i,'a');
    else if(message.GetChar(i)=='B')message.SetChar(i,'b');
    else if(message.GetChar(i)=='C')message.SetChar(i,'c');
    else if(message.GetChar(i)=='D')message.SetChar(i,'d');
    else if(message.GetChar(i)=='E')message.SetChar(i,'e');
    else if(message.GetChar(i)=='F')message.SetChar(i,'f');
    else if(message.GetChar(i)=='G')message.SetChar(i,'g');
    else if(message.GetChar(i)=='H')message.SetChar(i,'h');
    else if(message.GetChar(i)=='I')message.SetChar(i,'i');
    else if(message.GetChar(i)=='J')message.SetChar(i,'j');
    else if(message.GetChar(i)=='K')message.SetChar(i,'k');
    else if(message.GetChar(i)=='L')message.SetChar(i,'l');
    else if(message.GetChar(i)=='M')message.SetChar(i,'m');
    else if(message.GetChar(i)=='N')message.SetChar(i,'n');
    else if(message.GetChar(i)=='O')message.SetChar(i,'o');
    else if(message.GetChar(i)=='P')message.SetChar(i,'p');
    else if(message.GetChar(i)=='Q')message.SetChar(i,'q');
    else if(message.GetChar(i)=='R')message.SetChar(i,'r');
    else if(message.GetChar(i)=='S')message.SetChar(i,'s');
    else if(message.GetChar(i)=='T')message.SetChar(i,'t');
    else if(message.GetChar(i)=='U')message.SetChar(i,'u');
    else if(message.GetChar(i)=='V')message.SetChar(i,'v');
    else if(message.GetChar(i)=='W')message.SetChar(i,'w');
    else if(message.GetChar(i)=='X')message.SetChar(i,'x');
    else if(message.GetChar(i)=='Y')message.SetChar(i,'y');
    else if(message.GetChar(i)=='Z')message.SetChar(i,'z');
  }
}

void LogString(String serial){
          BTserial.println(serial);
          Serial.println(serial);
}
void Log(char * arrayChar){

      for (int i = 0; i < strlen(arrayChar); i++) 
      {     
          BTserial.print(arrayChar[i]);
          Serial.print(arrayChar[i]);
      }
      BTserial.println();
      Serial.println();
}

void ExecuteCommand(CharArrayTarget mArray){

    LogString("> MessageA:");
    Log(mArray);
    Replace(mArray,';', ' ');
    Replace(mArray,'\n', ';');
    Replace(mArray,'\r', ';');
    ToLower(mArray);
    Log(mArray);
    LogString("Time Clock:"+ String(millis()));
    Log(m_messageBuffer);

    int tstart = millis();
    int msToAdd = 0;

  int start=0;
  int end=0;
  CharArrayTarget shortcut = CharArrayTarget(m_messageBufferMax,0,0);
  for(int i = 0 ; i < m_messageBufferMax; i++)
  {
    start= end;
    if(m_messageBuffer[i]==' ' || m_messageBuffer[i]==';'){
      end = i;
      shortcut.set(m_messageBuffer, start, end);
      //LogString("T1");
      //Log(shortcut);
      //LogString("T2s");
      //Log(shortcut);
      shortcut.trim();
      //Log(shortcut);
      //LogString("T2e");
      //Log(shortcut);
       if(shortcut.endwith('|')){
           shortcut.removelastchar();
     // LogString("T3");
           int t = shortcut.getAsInt();
           msToAdd=t;
           LogString("AT "+String(t));
       //    Log(shortcut);
       }
       else if(shortcut.endwith('>')){
           shortcut.removelastchar();
           int t = shortcut.getAsInt();
           msToAdd+=t;
           LogString("DELAY "+ String(t));
        //   Log(shortcut);
       } 
      else  if(shortcut.endwith('\'')){
          LogString("ShortCut release");
          Log(shortcut);
      } 
      else if(shortcut.endwith('.')){
          LogString("ShortCut press");
          Log(shortcut);
      }
      else{
        LogString("ShortCut:");
        Log(shortcut);

      }
        


      if(m_messageBuffer[i]==';')
        break;
    }
  }
  return;

  /*
   String message="";
   int count=0;

    // Print the substrings
    for (int i = 0; i < count; i++) {

      message = String(message);
      message = toLowerCase(message);
      message = trim(message);
      shortcut=message;
      if(shortcut.length()==0)
        continue;
        
        if (m_debugModeSerial) {

          BTserial.print("#");
          BTserial.print(shortcut);
          BTserial.println("#");
        }
      

      if (shortcut.endsWith(">")) {
        int intValue = shortcut.substring(0, shortcut.length() - 1).toInt();
        msToAdd += intValue;
        if (m_debugModeSerial) {
          BTserial.print("MS ");
          BTserial.println(msToAdd);
        }
      } else if (shortcut.endsWith("|")) {
        int intValue = shortcut.substring(0, shortcut.length() - 1).toInt();
        msToAdd = intValue;
        if (m_debugModeSerial) {
          BTserial.print("MS AT ");
          BTserial.println(msToAdd);
        }
      } else {
        if (msToAdd > 0) {
          add_time_command(shortcut, tstart + msToAdd);
          if (m_debugModeSerial) {
            BTserial.print("Delay ");
            BTserial.print(shortcut);
            BTserial.print(" MS ");
            BTserial.print(msToAdd);
            BTserial.print(" T");
            BTserial.println(tstart + msToAdd);
          }
        }

        if (msToAdd <= 0) {

          if (message.indexOf('%') > -1) {

            float value = GetFloatFromPercent(message);
            if (m_debugLine)
              BTserial.print("\nValue " + String(value));
            if (StartBy4(message, 'j', 'l', 'h', '%')) {
              m_jlh = value;
              if (m_debugModeSerial) BTserial.print("\njlh " + String(m_jlh) + " " + String(m_jlv));
              XInput.setJoystick(JOY_LEFT, m_jlh * JoyMax, m_jlv * JoyMax);
            }
            if (StartBy4(message, 'j', 'l', 'v', '%')) {
              m_jlv = value;
              if (m_debugModeSerial) BTserial.print("\njlv " + String(m_jlh) + " " + String(m_jlv));
              XInput.setJoystick(JOY_LEFT, m_jlh * JoyMax, m_jlv * JoyMax);
            }
            if (StartBy4(message, 'j', 'r', 'h', '%')) {
              m_jrh = value;
              if (m_debugModeSerial) BTserial.print("\njrh " + String(m_jrh) + " " + String(m_jrv));
              XInput.setJoystick(JOY_RIGHT, -m_jrh * JoyMax, m_jrv * JoyMax);
            }
            if (StartBy4(message, 'j', 'r', 'v', '%')) {
              m_jrv = value;
              if (m_debugModeSerial) BTserial.print("\njrv " + String(m_jrh) + " " + String(m_jrv));
              XInput.setJoystick(JOY_RIGHT, -m_jrh * JoyMax, m_jrv * JoyMax);
            }

            if (StartBy3(message, 't', 'l', '%')) {

              if (m_debugModeSerial) BTserial.print("\ntl " + String(value));
              //XInput.setTrigger(TRIGGER_LEFT, value );
              XInput.setTrigger(TRIGGER_LEFT, value * ADC_Max);
            }
            if (StartBy3(message, 't', 'r', '%')) {

              if (m_debugModeSerial) BTserial.print("\ntr " + String(value));
              //XInput.setTrigger(TRIGGER_RIGHT, value );
              XInput.setTrigger(TRIGGER_RIGHT, value * ADC_Max);
            }
            //100> jlh%1 5000> jlh%-1 5000> jlh%0 ;
            //100> tl%1 5000> tl%-1 5000> tl%0 ;
          }

        else{
                unsigned long t = millis();  // Get timestamp for comparison
                bool press=EndWith(message,'\'');
                bool release=EndWith(message,'.');
                uint8_t button=0;
                
                message.replace('.', ' ');
                message.replace('\'', ' ');
                message.trim();

                if (message.startsWith("a") || message .startsWith("bd")) button=(BUTTON_A);
                else if (message .startsWith("x") || message .startsWith("bl")) button=(BUTTON_X);
                else if (message .startsWith("b") || message .startsWith("br")) button=(BUTTON_B);
                else if (message .startsWith("y") || message .startsWith("bu")) button=(BUTTON_Y);
                else if (message .startsWith("start") || message .startsWith("mr")) button=(BUTTON_START);
                else if (message .startsWith("back") || message .startsWith("ml")) button=(BUTTON_BACK);
                else if (message .startsWith("xbox") || message .startsWith("logo") || message .startsWith("mc")) button=(BUTTON_LOGO);

                else if (message .startsWith("debug on")) m_debugModeSerial = true;
                else if (message .startsWith("debug off")) m_debugModeSerial = false;
                else if (message .startsWith("debug")) m_debugModeSerial = !m_debugModeSerial;
                else if (message .startsWith("jl") || message .startsWith("l3")) button=(BUTTON_L3);
                else if (message .startsWith("jr") || message .startsWith("r3")) button=(BUTTON_R3);

                else if (message .startsWith("tl") || message .startsWith("l2")) button=(TRIGGER_LEFT);
                else if (message .startsWith("tr") || message .startsWith("r2")) button=(TRIGGER_RIGHT);

                else if (message .startsWith("sbl") || message .startsWith("l1")) button=(BUTTON_LB);
                else if (message .startsWith("sbr") || message .startsWith("r1")) button=(BUTTON_RB);

                else if (message .startsWith("d.") || message .startsWith("ad.")) {
                  m_down = true;
                } else if (message .startsWith("d'") || message .startsWith("ad'")) {
                  m_down = false;
                } else if (message .startsWith("l.") || message .startsWith("al.")) {
                  m_left = true;
                } else if (message .startsWith("l'") || message .startsWith("al'")) {
                  m_left = false;
                } else if (message .startsWith("r.") || message .startsWith("ar.")) {
                  m_right = true;
                } else if (message .startsWith("r'") || message .startsWith("ar'")) {
                  m_right = false;
                } else if (message .startsWith("u.") || message .startsWith("au.")) {
                  m_up = true;
                } else if (message .startsWith("u'") || message .startsWith("au'")) {
                  m_up = false;
                } else if (message .startsWith("release")) {
                  XInput.releaseAll();
                  if (m_debugModeSerial)
                    BTserial.print("Release all");
                }
              if(press){
                  XInput.press(button);
              }
              if(release){
                  XInput.release(button);
              }
                


                XInput.setDpad(m_up, m_down, m_left, m_right);


              if (m_debugModeSerial) BTserial.print("\nXinput: " + String(button) + " " + String(press) + " " + String(release));
              
          
        }


         


         
          if (m_wantToSendXboxReport)
            XInput.send();

          ExecutionLog(message);
           if ( m_debugModeSerial) {
            BTserial.println(GetDebugLineReport());
            BTserial.println(GetDebugJoystickLineReport());
          }
        }
      }
    }
    */

    
}



int serialIndex=0;
CharArrayTarget readSerialArray = CharArrayTarget();
void loop() {

    for (int i = 0; i < maxIndex; i++) { 
        if (! delayArray[i].had_been_relay()) {
            if (delayArray[i].is_time_reach()) {
                delayArray[i].set_as_relayed();
                Log("Exe:");
                Log(delayArray[i].m_command);
                //ExecuteCommand(toExe);
            }
        }
    }





  if (BTserial.available() > 0) {
 
    char c = BTserial.read();
    m_messageBuffer[serialIndex]=c;
    serialIndex++;
    if (c == '\n' || c == ';') {
      Log(m_messageBuffer);
      // if (m_debugModeSerial) {
      //   LogString("\nArduino Recieved:\n"+m_receivedMessageSerial);
      // }
      readSerialArray.set(m_messageBuffer, 0, serialIndex);
       ExecuteCommand(readSerialArray);

      DebugLogWaiting();
        m_receveivedToProcess = "";
        m_receivedMessageSerial = "";
        serialIndex=0;
    }
    }
  
}

