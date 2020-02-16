# How to build this project
Hey! Thanks for considering building the project yourself! In this short instruction I will try to explain how to get your **TemperatUHR** up and running.
This is a step-by-step instruction, so let's begin:

 1. At first, check that you have a suited workspace. You will need a soldering iron as well as some typical consumeables (solder wire, cables and some flux)
 2. Get the GERBER file and order the pcbs on sites like JLCPCB.
 3. order all parts that you don't have allready. An BOM is in the EasyEDA Public worksheet.
 4. Wait till all the stuff arrives!
 5. Flash the D1 mini with the newest microcontroller software from this repo
 6. Begin soldering. I discoverd that the optimal sequence of soldering parts is the following: BPS - BMS - Resistors - USB-Port - Microcontroller - Battery to its connectors on the PCB
 Don't solder the LED and the switch to the PCB now, we'll do that later on.
 After finishing the PCB, solder the temperature probe to a USB-Plug, using the following pins:
 1 - Black Wire / 2 - DATA / 3 - GND / 4 - NC
 7.  now you can 3D-Print the enclosure. I printed the part in a local Makerspace using the Ultimaker 3, 0.06 mm layer hight and white PLA. It came out great and I can totally recommend the settings.
 8. After printing, wrap the battery in some felt and lay the battery down into the case. On top, put the PCB and reassure that the USB port is still accessible throu the hole in the case.
 9. After everything fits, secure it all with some hotglu. Not mutch is needed, everything should fit tight.
 10. Now, you can solder the LED and the switch to the marked connectors on the pcb.
 11. test run the hole thing. If everything goes as planed, the LED should start of red and than switch to yellow and go off after four seconds. Shut the device down, plug the temp. probe in and you should start seeing a flashing green light after turning the device back on. Perfect!
 12. Now you can put on the lid and use the device as intended.
 13. Success!
Definetly let me know when your own "TemperatUHR" works!
Cheers,
Paul from Heidelberg (Germany)
