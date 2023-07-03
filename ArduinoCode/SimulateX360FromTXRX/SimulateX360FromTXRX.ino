
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






///////////////////////////////////::::


class DelayMessage {
public:
  DelayMessage() {
    m_timeReach = true;
    m_command = "0123";
    m_timestamp = millis();
  }

  void set_command(String command, unsigned long millisecond) {
    m_timeReach = false;
    m_command = command;
    m_timestamp = millis() + millisecond;
  }

  void set_command_at(String command, unsigned long timestamp) {
    m_timeReach = false;
    m_command = command;
    m_timestamp = timestamp;
  }

  void set_as_used() {
    m_timeReach = true;
  }

  bool is_to_dealed_with() {
    return !m_timeReach;
  }

  bool is_time_reach() {
    return millis() >= m_timestamp;
  }

  bool m_timeReach;
  String m_command;
  unsigned long m_timestamp;
};

const int maxIndex = 40;
DelayMessage delayArray[maxIndex];

bool usePrintLog = true;

void add_delay_command(String shortcut,  unsigned long  msToAdd) {
    for (int i = 0; i < maxIndex; i++) {
        if (!delayArray[i].is_to_dealed_with()) {
            delayArray[i].set_command(shortcut, msToAdd);
        }
    }
}

void add_time_command(String shortcut, unsigned long timestamp) {
    for (int i = 0; i < maxIndex; i++) {
        if (!delayArray[i].is_to_dealed_with()) {
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



// Some function

bool StartBy4(String text, char c1, char c2, char c3, char c4) {

  return text[0] == c1 && text[1] == c2 && text[2] == c3 && text[3] == c4;
}
bool StartBy3(String text, char c1, char c2, char c3) {

  return text[0] == c1 && text[1] == c2 && text[2] == c3;
}
float GetFloatFromPercent(String text) {
  int start = text.indexOf('%') + 1;
  int end = text.length();
  String integerValue = text.substring(start, end);
  return integerValue.toFloat();
}


// Some function to debug in the serial console.

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


// At start of the Arduino, we check that the serial is connected.
void setup() {

  XInput.setJoystickRange(0, ADC_Max); 
  BTserial.begin(9600);
  Serial.begin(9600);
  while (!BTserial) { ; }
  delay(500);
  BTserial.println("Serial available !");
  delay(500);
  XInput.setAutoSend(false);
  XInput.begin();
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


void loop() {

m_newInputFound = false;
for (int i = 0; i < maxIndex; i++) {
    if (delayArray[i].is_to_dealed_with()) {
        if (delayArray[i].is_time_reach()) {
            delayArray[i].set_as_used();
            String toExe=  delayArray[i].m_command;
            ExecuteCommand(toExe);
        }
    }
}




  if (BTserial.available() > 0) {

    char c = BTserial.read();
    m_receivedMessageSerial += c;
    if (c == '\n' || c == ';') {
      if (m_debugModeSerial) {
        Serial.print("\n");
        Serial.print("Arduino Recieved:");
        Serial.print(m_receivedMessageSerial);
        Serial.print("\n");
        BTserial.print("Arduino Recieved:");
        BTserial.print(m_receivedMessageSerial);
      }
      m_receveivedToProcess = m_receivedMessageSerial;
      m_newInputFound = true;
    }
  }

    if (m_newInputFound) {
      ExecuteCommand(m_receveivedToProcess);
      m_receveivedToProcess = "";
      m_receivedMessageSerial = "";
      m_newInputFound = false;
    
  }
  
}
void ExecuteCommand(String message){

////////////////: Split the text from the ' ' space between:////////////////:
    message.replace('|', ' ');
    message.replace(';', ' ');
    message.replace('\n', ' ');
    message.replace('\r', ' ');
    message.trim();
    message.toLowerCase();

    message = " " + message + " ";






    const int maxStrings = 10;      // Maximum number of substrings
    String substrings[maxStrings];  // String array to store the substrings
    int count = 0;                  // Counter for the number of substrings


    // Convert the input string to a character array
    char charArray[message.length() + 1];
    message.toCharArray(charArray, sizeof(charArray));

    // Tokenize the string using spaces as delimiters
    char* token = strtok(charArray, " ");
    while (token != NULL && count < maxStrings) {
      substrings[count] = token;
      count++;
      token = strtok(NULL, " ");
    }

    int tstart = millis();
    int msToAdd = 0;
    String shortcut;
    // Print the substrings
    for (int i = 0; i < count; i++) {


      message = substrings[i];
      message = String(message);
      message = toLowerCase(message);
      message = trim(message);
      shortcut=message;
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


          unsigned long t = millis();  // Get timestamp for comparison

          if (message == String("a.") || message == String("bd.")) XInput.press(BUTTON_A);
          else if (message == String("a'") || message == String("bd'")) XInput.release(BUTTON_A);
          else if (message == String("x.") || message == String("bl.")) XInput.press(BUTTON_X);
          else if (message == String("x'") || message == String("bl'")) XInput.release(BUTTON_X);
          else if (message == String("b.") || message == String("br.")) XInput.press(BUTTON_B);
          else if (message == String("b'") || message == String("br'")) XInput.release(BUTTON_B);
          else if (message == String("y.") || message == String("bu.")) XInput.press(BUTTON_Y);
          else if (message == String("y'") || message == String("bu'")) XInput.release(BUTTON_Y);
          else if (message == String("start.") || message == String("mr.")) XInput.press(BUTTON_START);
          else if (message == String("start'") || message == String("mr'")) XInput.release(BUTTON_START);
          else if (message == String("back.") || message == String("ml.")) XInput.press(BUTTON_BACK);
          else if (message == String("back'") || message == String("ml'")) XInput.release(BUTTON_BACK);
          else if (message == String("xbox.") || message == String("logo.") || message == String("mc.")) XInput.press(BUTTON_LOGO);
          else if (message == String("xbox'") || message == String("logo'") || message == String("mc'")) XInput.release(BUTTON_LOGO);

          else if (message == String("debug on")) m_debugModeSerial = true;
          else if (message == String("debug off")) m_debugModeSerial = false;
          else if (message == String("debug")) m_debugModeSerial = !m_debugModeSerial;
          else if (message == String("jl.") || message == String("l3.")) XInput.press(BUTTON_L3);
          else if (message == String("jl'") || message == String("l3.")) XInput.release(BUTTON_L3);
          else if (message == String("jr.") || message == String("r3.")) XInput.press(BUTTON_R3);
          else if (message == String("jr'") || message == String("r3.")) XInput.release(BUTTON_R3);

          else if (message == String("tl.") || message == String("l2.")) XInput.press(TRIGGER_LEFT);
          else if (message == String("tl'") || message == String("l2.")) XInput.release(TRIGGER_LEFT);
          else if (message == String("tr.") || message == String("r2.")) XInput.press(TRIGGER_RIGHT);
          else if (message == String("tr'") || message == String("r2.")) XInput.release(TRIGGER_RIGHT);

          else if (message == String("sbl.") || message == String("l1.")) XInput.press(BUTTON_LB);
          else if (message == String("sbl'") || message == String("l1.")) XInput.release(BUTTON_LB);
          else if (message == String("sbr.") || message == String("r1.")) XInput.press(BUTTON_RB);
          else if (message == String("sbr'") || message == String("r1.")) XInput.release(BUTTON_RB);

          else if (message == String("d.") || message == String("ad.")) {
            m_down = true;
          } else if (message == String("d'") || message == String("ad'")) {
            m_down = false;
          } else if (message == String("l.") || message == String("al.")) {
            m_left = true;
          } else if (message == String("l'") || message == String("al'")) {
            m_left = false;
          } else if (message == String("r.") || message == String("ar.")) {
            m_right = true;
          } else if (message == String("r'") || message == String("ar'")) {
            m_right = false;
          } else if (message == String("u.") || message == String("au.")) {
            m_up = true;
          } else if (message == String("u'") || message == String("au'")) {
            m_up = false;
          } else if (message == String("release")) {
            XInput.releaseAll();
            if (m_debugModeSerial)
              BTserial.print("Release all");
          }


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


}

