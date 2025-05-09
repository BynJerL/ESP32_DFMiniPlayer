#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

int Player_state = 13;

bool isPaused = false;
bool isLoopEnabled = false;

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 27; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 26; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;

void menu_opcoes() {
  Serial.println();
  Serial.println(F("======================================="));
  Serial.println(F("Commands:"));
  Serial.println(F("[1-3]    -> select the MP3 file"));
  Serial.println(F("[s]      -> stop the music"));
  Serial.println(F("[p]      -> pause/continue music"));
  Serial.println(F("[+ or -] -> increase or decrease audio volume"));
  Serial.println(F("[< or >] -> go to previous or next track"));
  Serial.println(F("[l]      -> enable or disable loop"));
  Serial.println();
  Serial.println(F("======================================="));
}

void setup() {
  pinMode(Player_state, INPUT_PULLUP);

  // Init USB serial port for debugging
  Serial.begin(9600);
  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);

  Serial.println("Initializing DFPlayer Mini...");

  if (player.begin(softwareSerial)) {
    Serial.println("Initialization success!");
  } else {
    Serial.println("Failed to initialize DFPlayer Mini!");
    Serial.println("Try this:");
    Serial.println("1. Check the DFPlayer Mini connections");
    Serial.println("2. Insert an SD card");
    return;
  }

  // Setups
  player.setTimeOut(500);
  player.volume(15);
  player.EQ(DFPLAYER_EQ_NORMAL);
  player.playMp3Folder(1);

  menu_opcoes();
}

void loop() {
    // if (digitalRead(Player_state) == LOW) {
    //     Serial.println("audio is playing");
    // } else {
    //     Serial.println("audio is not playing");
    // }
  if (Serial.available() > 0) {
    uint8_t command = Serial.read();

    switch (command) {
      // Play the music
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
        Serial.printf("Track %c played.\n", command);
        player.playMp3Folder(command - 48);
        break;
      
      // Stop the music
      case 's':
        player.stop();
        Serial.println("Music has stopped.");
        break;
      
      // Pause or continue the music
      case 'p':
        isPaused = !isPaused;

        if (!isPaused) {
          player.start();
          Serial.println("Music continue...");
        } else {
          player.pause();
          Serial.println("Music paused!");
        }
        break;
      
      // Increase the volume
      case '+':
        player.volumeUp();
        Serial.printf("Current volume: %d", player.readVolume());
        break;
      
      // Decrease the volume
      case '-':
        player.volumeDown();
        Serial.printf("Current volume: %d", player.readVolume());
        break;
      
      case '<':
        player.previous();
        Serial.printf("Track %d played.\n", player.readCurrentFileNumber());
        break;
      
      case '>':
        player.next();
        Serial.printf("Track %d played.\n", player.readCurrentFileNumber());
        break;
      
      case 'l':
        isLoopEnabled = !isLoopEnabled;

        if (isLoopEnabled) {
          player.enableLoop();
          Serial.println("Track loop has been enabled");
        } else {
          player.disableLoop();
          Serial.println("Track loop has been disabled");
        }
        
        break;
      
      default:
        Serial.println("Invalid command.");
        break;
    }

    menu_opcoes();
  }
}