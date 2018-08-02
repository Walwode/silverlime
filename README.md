# silverlime
Silverlime is a project to control 433MHz and InfraRed devices, especially by Alexa Smart Home integration (on/off). It's possible to record and send values.

## Usage

### Alexa
Create Switch objects for Alexa. Define callback functions for on and off commands with bool return value. Add switch.serverLoop() to general loop function.

### IR
Uncomment IrHandler::loop(); in general loop() function to enable recording of infrared signals. The recorded values are printed to the serial monitor.

### 433MHz
Uncomment RcHandler::loop(); in the general loop() function to enable record of 433MHz signals. The record values are printed to the serial monitor.