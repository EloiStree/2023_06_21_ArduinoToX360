# 2023_06_21_ArduinoToX360
In the repository you can find an Arduino wrapper of  Arduino XInput that allows you to control X360 with Brook from your TTL or HC06


### Archive step by step video

In this video, I show the step to install and use the project.  
It is low quality but it is just to archived and be able to remind myself how I did it.  
Hope it helps you if you want to do the same as me.  

[![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/af2f3635-20b3-432a-9b5c-5eed4a6ee2cf)](https://youtu.be/cKlV_1cfsJc)
Video: https://youtu.be/cKlV_1cfsJc  


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
Window App :https://apps.microsoft.com/store/detail/serial-debug-assistant/9NBLGGH43HDM?hl=fr-be&gl=be&rtc=1  
![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/0ac98b37-984a-41b6-845a-4cfaeba7b05e)  

Or Bluetooth Electronics on Android if you want to communicate with HC06    
App on the Google Store : https://play.google.com/store/apps/details?id=com.keuwl.arduinobluetooth&hl=en_US  
![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/33357ab3-8be6-4686-9293-8bf0ec639b58)



⚠️ WARNING ⚠️ When you will upload the sketch on your Arduino with the XInput board. Your Arduino will be detected as Xbox Controller ⚠️
- Meaning that you can't upload sketch anymore without reseting your Arduino.  
  
I found a way by uploading an empty sketch at the right moment but it is tricky and maybe dangeroux. Not expert enough to know.  


To interact with the tool you need to set a TX/RX TTL or HC06 on the Arduino on pin 9 & 8 (by default)  
![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/1c91fae3-42a9-41a2-8b8e-d281abeabfc7)  
The TTL I use is  PL2303 https://amzn.to/3XjFpsM but you need to do Window update in the device manager.  
(Youtube of how to do it: https://www.youtube.com/results?search_query=PL2303+driver+install)   

If you use an HC06 and never use on before you need to learn how to do a voltage split.
![image](https://github.com/EloiStree/2023_06_21_ArduinoToX360/assets/20149493/4bf54c29-52ba-4031-9d99-657d1ba86a69)

Video about it: https://youtu.be/NXlyo0goBrU


## Interaction

In my code . means press and ' means release.  
So you can write ` x. y. b' \n` to press x and y and release b.

For the joystick you can write command like this    
`jlh%0.5` to set the joystick at  50% on the left joystick to the left side.    
`tr%0.8` to press the trigger right at 80%     

`release` with release everything  
`debug on` will active debug message on the HC06/TTL but so losing some time doing it.  
`debug off` will remove debug message to "go faster".  

To know all the input you can check in the Ino file of the repository.
 

### Example of serial message you can send

`a. 100> a' ;` Press A then release A
`ar. 100> ar' ;` Press arrow right then release arrow right
`a. b. x. y. 200> release ;`press all power button then release all
`bd. bl. br. bt. 200> release ;` press all power button then release all
`ad. al. ar. at. 200> release ;` press all arrow then release all
`jl. 400> jl' ;` Press joystick left stick
`jlh%1.0 400> jlh%-1.0 400> jlh%0  ;` Move left then right then stop





