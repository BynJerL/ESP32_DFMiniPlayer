#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

int Player_state = 13;

bool isPaused = false;

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 27; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 26; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;

void menu_opcoes() {
  Serial.println();
  Serial.println("=======================================");
  Serial.println("Commands:");
  Serial.println("[1-3]    -> select the MP3 file");
  Serial.println("[s]      -> stop the music");
  Serial.println("[p]      -> pause/continue music");
  Serial.println("[+ or -] -> increase or decrease audio volume");
  Serial.println();
  Serial.println("=======================================");
}

void setup() {
  pinMode(Player_state, INPUT_PULLUP);

  // Init USB serial port for debugging
  Serial.begin(9600);
  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);

  Serial.println("Initializing DFPlayer Mini...");

  if (!player.begin(softwareSerial)) {
    Serial.println("Failed to initialize DFPlayer Mini!");
    Serial.println("Try this:");
    Serial.println("1. Check the DFPlayer Mini connections");
    Serial.println("2. Insert an SD card");
    return;
  }

  Serial.println("Initialization success!");

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

    // Play music
    if ((command >= '1') &&
        (command <= '3')) {
      Serial.printf("Music %c played.\n", command);
      player.playMp3Folder(command - 48);
    }

    // Stop music
    if (command == 's') {
      player.stop();
      Serial.println("Music has stopped.");
    }

    // Pause/Continue music
    if (command == 'p') {
      isPaused = !isPaused;

      if (!isPaused) {
        player.start();
        Serial.println("Music continue...");
      } else {
        player.pause();
        Serial.println("Music paused!");
      }
    }

    if (command == '+') {
      player.volumeUp();
      Serial.printf("Current volume: %d", player.readVolume());
    }

    if (command == '-') {
      player.volumeDown();
      Serial.printf("Current volume: %d", player.readVolume());
    }

    menu_opcoes();
  }
}