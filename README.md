# 2023_06_21_ArduinoToX360
In the repository you can find an Arduino wrapper of  Arduino XInput that allows you to control X360 with Brook from your TTL or HC06


## How to use ?

Required: 
- Arduino board that accept 'Keyboard.h'
- Brook (or similar) device

Install:  
- Install Arduino Editor
- Read documentation of XInput Arduino GitHub creator:
  - https://github.com/dmadison/ArduinoXInput
- Set the preference link of Arduino to following link to be able to target Xinput Board.
  - https://raw.githubusercontent.com/dmadison/ArduinoXInput_Boards/master/package_dmadison_xinput_index.json
 
![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/f2ce191f-7c30-4fd9-b89f-73645d572f13)

You can install Serial Debug Assitance on Window:  
https://apps.microsoft.com/store/detail/serial-debug-assistant/9NBLGGH43HDM?hl=fr-be&gl=be&rtc=1  
![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/0ac98b37-984a-41b6-845a-4cfaeba7b05e)  

Or Bluetooth Electronics on Android if you want to communicate with HC06    
https://play.google.com/store/apps/details?id=com.keuwl.arduinobluetooth&hl=en_US  
![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/c252738d-39a6-4f3f-a295-9d43a4dd95c2)  


⚠️ WARNING ⚠️ When you will upload the sketch on your Arduino with the XInput board. Your Arduino will be detected as Xbox Controller ⚠️
- Meaning that you can't upload sketch anymore without reseting your Arduino.  
  
I found a way by uploading an empty sketch at the right moment but it is tricky and maybe dangeroux. Not expert enough to know.  



