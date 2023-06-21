

#include <XInput.h>
#include <SoftwareSerial.h>


const byte rxPin = 9;
const byte txPin = 8;
SoftwareSerial  BTserial(rxPin, txPin);  // RX //TX

// Config Settings
const unsigned long CycleTime = 5000;  // ms


// Joystick Setup
const int JoyMax = 32767;                                   // int16_t max

bool m_wantToSendXboxReport=true;
const int TriggerMax = 255;  // uint8_t max

void setup() {
  BTserial.begin(9600); 
  Serial.begin(9600);
  while(!BTserial) {; }
  delay(500);
  BTserial.println("Serial available !");
  XInput.setAutoSend(false);  
  XInput.begin();
}

bool m_up;
bool m_down;
bool m_right;
bool m_left;
bool m_newInputFound;

String m_receivedMessageSerial = "";
String m_receveivedToProcess = "";

float m_jlh=0;
float m_jlv=0;
float m_jrh=0;
float m_jrv=0;
bool m_debugMode=true;

bool StartBy4(String text, char c1, char c2,char c3,  char c4 ){

  return text[0]==c1 && text[1]==c2 && text[2]==c3 && text[3]==c4 ;

}
bool StartBy3(String text, char c1, char c2,char c3){

  return text[0]==c1 && text[1]==c2 && text[2]==c3;

}
float GetFloatFromPercent(String text){
  int start = text.indexOf('%') + 1;
  int end = text.length();
  String integerValue = text.substring(start, end);
  return  integerValue.toFloat();
}

String m_debugLine="--------------------------------------------------------------------------------";
String GetDebugLineReport(){
 m_debugLine[0]= 'l' ;
 m_debugLine[2]= XInput.getButton(BUTTON_LOGO)?'1':'0';
 m_debugLine[3]= ' ';
 m_debugLine[4]= 'a' ;
 m_debugLine[5]= XInput.getDpad(DPAD_UP )?'1':'0';
 m_debugLine[6]= XInput.getDpad(DPAD_DOWN )?'1':'0';
 m_debugLine[7]= XInput.getDpad(DPAD_LEFT )?'1':'0';
 m_debugLine[8]= XInput.getDpad(DPAD_RIGHT )?'1':'0';
 m_debugLine[9]= ' ';
 m_debugLine[10]= 'b';
 m_debugLine[12]= XInput.getButton(BUTTON_A)?'1':'0';
 m_debugLine[13]= XInput.getButton(BUTTON_B)?'1':'0';
 m_debugLine[14]= XInput.getButton(BUTTON_X)?'1':'0';
 m_debugLine[15]= XInput.getButton(BUTTON_Y)?'1':'0';
 m_debugLine[16]= ' ';
 m_debugLine[17]= 's';
 m_debugLine[18]= XInput.getButton(BUTTON_LB )?'1':'0';
 m_debugLine[19]= XInput.getButton(BUTTON_RB )?'1':'0';
 m_debugLine[20]= ' ';
 m_debugLine[22]= 'm';
 m_debugLine[23]= XInput.getButton(BUTTON_BACK  )?'1':'0';
 m_debugLine[24]= XInput.getButton(BUTTON_START  )?'1':'0';
 m_debugLine[25]= ' ';
 m_debugLine[26]= 't';
 m_debugLine[27]= XInput.getTrigger(TRIGGER_LEFT )?'1':'0';
 m_debugLine[28]= XInput.getTrigger(TRIGGER_RIGHT )?'1':'0';
 m_debugLine[29]= ' ';
 m_debugLine[30]= 'j';
 m_debugLine[31]= XInput.getButton(BUTTON_L3)?'1':'0';
 m_debugLine[32]= XInput.getButton(BUTTON_R3)?'1':'0';
 return m_debugLine;


//boolean getButton(uint8_t button) const;
//boolean getDpad(XInputControl dpad) const;
//uint8_t getTrigger(XInputControl trigger) const;
//int16_t getJoystickX(XInputControl joy) const;
//int16_t getJoystickY(XInputControl joy) const;

}
String m_joystickReport;
String GetDebugJoystickLineReport(){

m_joystickReport="";
m_joystickReport+=" JLH "+ String(XInput.getJoystickX(JOY_LEFT));
m_joystickReport+=" JLV "+ String(XInput.getJoystickY(JOY_LEFT));
m_joystickReport+=" JRH "+ String(XInput.getJoystickX(JOY_RIGHT));
m_joystickReport+=" JRV "+ String(XInput.getJoystickY(JOY_RIGHT));
m_joystickReport+=" TL "+ String(XInput.getTrigger(TRIGGER_LEFT));
m_joystickReport+=" TR "+ String(XInput.getTrigger(TRIGGER_RIGHT));

 return m_joystickReport;


//boolean getButton(uint8_t button) const;
//boolean getDpad(XInputControl dpad) const;
//uint8_t getTrigger(XInputControl trigger) const;
//int16_t getJoystickX(XInputControl joy) const;
//int16_t getJoystickY(XInputControl joy) const;

}



void loop() {

  m_newInputFound = false;
  if ( BTserial.available() > 0 ) {
    char c = BTserial.read();
    m_receivedMessageSerial += c;
    if (c == '\n') {
      Serial.print("\n");
      Serial.print("Arduino Recieved:");
      Serial.print(m_receivedMessageSerial);
      Serial.print("\n");
      BTserial.print("Arduino Recieved:");
      BTserial.print(m_receivedMessageSerial);
      m_receveivedToProcess = m_receivedMessageSerial;
      m_newInputFound = true;
    }
  }





  if (m_newInputFound) {
m_receveivedToProcess = " "+m_receveivedToProcess+" ";
  const int maxStrings = 10;  // Maximum number of substrings
  String substrings[maxStrings];  // String array to store the substrings
  int count = 0;  // Counter for the number of substrings
  
  // Convert the input string to a character array
  char charArray[m_receveivedToProcess.length() + 1];
  m_receveivedToProcess.toCharArray(charArray, sizeof(charArray));
  
  // Tokenize the string using spaces as delimiters
  char* token = strtok(charArray, " ");
  while (token != NULL && count < maxStrings) {
    substrings[count] = token;
    count++;
    token = strtok(NULL, " ");
  }
  
  // Print the substrings
  for (int i = 0; i < count; i++) {
        m_receveivedToProcess =substrings[i];







        m_receveivedToProcess.trim();
        m_receveivedToProcess.toLowerCase();
        m_receveivedToProcess.replace('\n',' ');
        m_receveivedToProcess.replace('\r',' ');


        unsigned long t = millis();  // Get timestamp for comparison

        if (m_receveivedToProcess == String("a.") || m_receveivedToProcess == String("bd.")) XInput.press(BUTTON_A);
        else if (m_receveivedToProcess == String("a'") || m_receveivedToProcess == String("bd'")) XInput.release(BUTTON_A);
        else if (m_receveivedToProcess == String("x.") || m_receveivedToProcess == String("bl.")) XInput.press(BUTTON_X);
        else if (m_receveivedToProcess == String("x'") || m_receveivedToProcess == String("bl'")) XInput.release(BUTTON_X);
        else if (m_receveivedToProcess == String("b.") || m_receveivedToProcess == String("br.")) XInput.press(BUTTON_B);
        else if (m_receveivedToProcess == String("b'") || m_receveivedToProcess == String("br'")) XInput.release(BUTTON_B);
        else if (m_receveivedToProcess == String("y.") || m_receveivedToProcess == String("bu.")) XInput.press(BUTTON_Y);
        else if (m_receveivedToProcess == String("y'") || m_receveivedToProcess == String("bu'")) XInput.release(BUTTON_Y);
        else if (m_receveivedToProcess == String("start.") || m_receveivedToProcess == String("mr.")) XInput.press(BUTTON_START);
        else if (m_receveivedToProcess == String("start'") || m_receveivedToProcess == String("mr'")) XInput.release(BUTTON_START);
        else if (m_receveivedToProcess == String("back." )|| m_receveivedToProcess == String("ml.")) XInput.press(BUTTON_BACK);
        else if (m_receveivedToProcess == String("back'" ) || m_receveivedToProcess == String("ml'")) XInput.release(BUTTON_BACK);
        else if (m_receveivedToProcess == String("xbox." ) ||m_receveivedToProcess == String("logo." ) || m_receveivedToProcess == String("mc.")) XInput.press(BUTTON_LOGO);
        else if (m_receveivedToProcess == String("xbox'" ) ||m_receveivedToProcess == String("logo'" ) || m_receveivedToProcess == String("mc'")) XInput.release(BUTTON_LOGO);

        else if (m_receveivedToProcess == String("debug on")) m_debugMode=true;
        else if (m_receveivedToProcess == String("debug off")) m_debugMode=false;
        else if (m_receveivedToProcess == String("debug")) m_debugMode=! m_debugMode;
        else if (m_receveivedToProcess == String("jl.") || m_receveivedToProcess == String("l3.")) XInput.press(BUTTON_L3);
        else if (m_receveivedToProcess == String("jl'") || m_receveivedToProcess == String("l3.")) XInput.release(BUTTON_L3);
        else if (m_receveivedToProcess == String("jr.") || m_receveivedToProcess == String("r3.")) XInput.press(BUTTON_R3);
        else if (m_receveivedToProcess == String("jr'") || m_receveivedToProcess == String("r3.")) XInput.release(BUTTON_R3);

        else if (m_receveivedToProcess == String("tl.") || m_receveivedToProcess == String("l2.")) XInput.press(TRIGGER_LEFT);
        else if (m_receveivedToProcess == String("tl'") || m_receveivedToProcess == String("l2.")) XInput.release(TRIGGER_LEFT);
        else if (m_receveivedToProcess == String("tr.") || m_receveivedToProcess == String("r2.")) XInput.press(TRIGGER_RIGHT);
        else if (m_receveivedToProcess == String("tr'") || m_receveivedToProcess == String("r2.")) XInput.release(TRIGGER_RIGHT);
    
        else if (m_receveivedToProcess == String("sbl.") || m_receveivedToProcess == String("l1.")) XInput.press(BUTTON_LB);
        else if (m_receveivedToProcess == String("sbl'") || m_receveivedToProcess == String("l1.")) XInput.release(BUTTON_LB);
        else if (m_receveivedToProcess == String("sbr.") || m_receveivedToProcess == String("r1.")) XInput.press(BUTTON_RB);
        else if (m_receveivedToProcess == String("sbr'") || m_receveivedToProcess == String("r1.")) XInput.release(BUTTON_RB);
    
        else if (m_receveivedToProcess == String("d.") || m_receveivedToProcess == String("ad.")){m_down=true; BTserial.print(  "V ad.");} 
        else if (m_receveivedToProcess == String("d'") || m_receveivedToProcess == String("ad'")){m_down=false; BTserial.print( "V ad'");} 
        else if (m_receveivedToProcess == String("l.") || m_receveivedToProcess == String("al.")){m_left=true; BTserial.print(  "V al.");} 
        else if (m_receveivedToProcess == String("l'") || m_receveivedToProcess == String("al'")){m_left=false; BTserial.print( "V al'");} 
        else if (m_receveivedToProcess == String("r.") || m_receveivedToProcess == String("ar.")){m_right=true; BTserial.print( "V ar.");} 
        else if (m_receveivedToProcess == String("r'") || m_receveivedToProcess == String("ar'")){m_right=false; BTserial.print("V ar'");} 
        else if (m_receveivedToProcess == String("u.") || m_receveivedToProcess == String("au.")){m_up=true; BTserial.print(    "V au.");} 
        else if (m_receveivedToProcess == String("u'") || m_receveivedToProcess == String("au'")){m_up=false; BTserial.print(   "V au'");} 
        else if (m_receveivedToProcess == String("release") ){
          XInput.releaseAll(); BTserial.print(   "Release all");} 
    
        XInput.setDpad(m_up, m_down, m_left, m_right);
        //XInput.setTrigger(TRIGGER_LEFT, triggerVal);
        //XInput.setTrigger(TRIGGER_RIGHT, TriggerMax - triggerVal);  // Inverse

        // Calculate joystick x/y values using trig
        // int axis_x = sin(angle) * JoyMax;
        // int axis_y = cos(angle) * JoyMax;
        //  angle += angle_precision;
        // if (angle >= 360) {
        //   angle -= 360;
        // }
        //


        if(m_receveivedToProcess.indexOf('%')>-1){

              float value = GetFloatFromPercent(m_receveivedToProcess);
              BTserial.print("Value "+ String(value)  ); 
              if(StartBy4(m_receveivedToProcess,'j','l','h','%')) { 
                  m_jlh = value;
                  if(m_debugMode) BTserial.print("jlh "+ String(m_jlh)+" " + String(m_jlv)); 
                  XInput.setJoystick(JOY_LEFT, m_jlh*JoyMax, m_jlv*JoyMax); 
              }
              if(StartBy4(m_receveivedToProcess,'j','l','v','%')) { 
                  m_jlv = value; 
                  if(m_debugMode) BTserial.print("jlv "+ String(m_jlh)+" " + String(m_jlv)); 
                  XInput.setJoystick(JOY_LEFT, m_jlh*JoyMax, m_jlv*JoyMax);  
                  }
              if(StartBy4(m_receveivedToProcess,'j','r','h','%')) {
                  m_jrh = value; 
                  if(m_debugMode) BTserial.print("jrh "+ String(m_jrh)+" " + String(m_jrv)); 
                  XInput.setJoystick(JOY_RIGHT, -m_jrh*JoyMax, m_jrv*JoyMax);  }
              if(StartBy4(m_receveivedToProcess,'j','r','v','%')) { 
                  m_jrv = value; 
                  if(m_debugMode) BTserial.print("jrv "+ String(m_jrh)+" " + String(m_jrv)); 
                  XInput.setJoystick(JOY_RIGHT, -m_jrh*JoyMax, m_jrv*JoyMax); }

              if(StartBy3(m_receveivedToProcess,'t','l','%')) { 
                
                  if(m_debugMode) BTserial.print("tl "+ String(value)); 
                  XInput.setTrigger(TRIGGER_LEFT, value*4095);
              } 
              if(StartBy3(m_receveivedToProcess,'t','r','%')) { 
                
                  if(m_debugMode) BTserial.print("tr "+ String(value)); 
                  XInput.setTrigger(TRIGGER_RIGHT, value*4095);
              } 
          }
        }

        if(m_wantToSendXboxReport)
          XInput.send();
        if( m_newInputFound && m_debugMode){
        BTserial.println(GetDebugLineReport()); 
        BTserial.println(GetDebugJoystickLineReport()); 

        } 
     }
    if(m_newInputFound){

        m_receveivedToProcess = "";
        m_receivedMessageSerial="";
        m_newInputFound = false;
    }
  }
