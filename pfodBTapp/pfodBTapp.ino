
/* ===== pfod Command for Menu_1 ====
pfodApp msg {.} --> {,~Arduino data`1000~V1|!A`775~A0~V`1023`0~3.3~0~}
 */
// Using Serial and 9600 baud for send and receive
// Serial D0 (RX) and D1 (TX) on Arduino Uno, Micro, ProMicro, Due, Mega, Mini, Nano, Pro and Ethernet
// This code uses Serial so remove shield when programming the board
/* Code generated by pfodDesignerV2 V2.0.2142
 * (c)2014-2016 Forward Computing and Control Pty. Ltd.
 * NSW Australia, www.forward.com.au
 * This generated code may be freely used for both private and commercial use
 */
 #include <pfodEEPROM.h>
 #include <pfodParser.h>
 #include <SoftwareSerial.h>
 SoftwareSerial BTserial(2, 3); // RX | TX
int swap01(int); // method prototype for slider end swaps

// ======================
// this is the pfodParser.h V2 file with the class renamed pfodParser_codeGenerated and with comments, constants and un-used methods removed
class pfodParser_codeGenerated: public Print { 
  public:
    pfodParser_codeGenerated(const char* version); pfodParser_codeGenerated(); void connect(Stream* ioPtr); void closeConnection(); byte parse(); bool isRefresh(); 
    const char* getVersion(); void setVersion(const char* version); void sendVersion(); byte* getCmd(); byte* getFirstArg();
    byte* getNextArg(byte *start); byte getArgsCount(); byte* parseLong(byte* idxPtr, long *result); byte getParserState();
    void setCmd(byte cmd); void setDebugStream(Print* debugOut); size_t write(uint8_t c); int available(); int read();
    int peek(); void flush(); void setIdleTimeout(unsigned long timeout); Stream* getPfodAppStream(); void init(); byte parse(byte in); 
  private:
    Stream* io; byte emptyVersion[1] = {0}; byte argsCount; byte argsIdx; byte parserState; byte args[255 + 1]; byte *versionStart;
    byte *cmdStart; bool refresh; const char *version;
};
//============= end of pfodParser_codeGenerated.h
pfodParser parser("V2"); // create a parser to handle the pfod messages

// give the board pins names, if you change the pin number here you will change the pin controlled
int cmd_A_var; // name the variable for 'A0'
unsigned long cmd_A_adcStartTime=0; // ADC timer
unsigned long cmd_A_ADC_READ_INTERVAL = 1000;// 1sec, edit this to change adc read interval
const int cmd_A_pin = A3; // name the pin for 'A0'

// the setup routine runs once on reset:
void setup() {
  Serial.begin(9600);
  for (int i=3; i>0; i--) {
    // wait a few secs to see if we are being programmed
    delay(1000);
  }
  BTserial.begin(9600);
  parser.connect(&Serial); // connect the parser to the i/o stream

  // <<<<<<<<< Your extra setup code goes here
}

// the loop routine runs over and over again forever:
void loop() {
  byte cmd = parser.parse(); // parse incoming data from connection
  // parser returns non-zero when a pfod command is fully parsed
  if (cmd != 0) { // have parsed a complete msg { to }
    byte* pfodFirstArg = parser.getFirstArg(); // may point to \0 if no arguments in this msg.
    long pfodLongRtn; // used for parsing long return arguments, if any
    if ('.' == cmd) {
      // pfodApp has connected and sent {.} , it is asking for the main menu
      if (!parser.isRefresh()) {
        sendMainMenu(); // send back the menu designed
      } else {
        sendMainMenuUpdate(); // menu is cached just send update
      }

    // now handle commands returned from button/sliders
//    } else if('A'==cmd) { // this is a label. pfodApp NEVER sends this cmd -- 'A0'
//      // in the main Menu of Menu_1 

    } else if ('!' == cmd) {
      // CloseConnection command
      closeConnection(parser.getPfodAppStream());
    } else {
      // unknown command
      parser.print(F("{}")); // always send back a pfod msg otherwise pfodApp will disconnect.
    }
  }
  cmd_A_readADC(); 
  //  <<<<<<<<<<<  Your other loop() code goes here 
  
}

void closeConnection(Stream *io) {
  // add any special code here to force connection to be dropped
}
void cmd_A_readADC() {
  if ((millis() - cmd_A_adcStartTime) > cmd_A_ADC_READ_INTERVAL) {
    cmd_A_adcStartTime = millis(); // restart timer
    // cmd_A_var = analogRead(cmd_A_pin);  // read ADC input
    cmd_A_var = 2;
  }
}


void sendMainMenu() {
  // !! Remember to change the parser version string
  //    every time you edit this method
  parser.print(F("{,"));  // start a Menu screen pfod message
  // send menu background, format, prompt, refresh and version
  parser.print(F("~Arduino data`1000"));
  parser.sendVersion(); // send the menu version 
  // send menu items
  parser.print(F("|!A"));
  parser.print('`');
  parser.print(cmd_A_var); // output the current ADC reading
  parser.print(F("~A0~V`1023`0~3.3~0~"));
  parser.print(F("}"));  // close pfod message
}

void sendMainMenuUpdate() {
  parser.print(F("{;"));  // start an Update Menu pfod message
  // send menu items
  parser.print(F("|A"));
  parser.print('`');
  parser.print(cmd_A_var); // output the current ADC reading
  parser.print(F("}"));  // close pfod message
  // ============ end of menu ===========
}
/* You can remove from here on if you have the pfodParser V2 library installed from
    http://www.forward.com.au/pfod/pfodParserLibraries/index.html
  * and add 
 #include <pfodEEPROM.h
 #include <pfodParser.h>
  * at the top of this file
  * and replace the line
 pfodParser_codeGenerated parser("V1"); // create a parser to handle the pfod messages
  * with
 pfodParser parser("V1");
*/

 

