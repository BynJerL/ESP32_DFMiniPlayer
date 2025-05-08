#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

int Player_state = 13;

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 27; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 26; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;

void setup() {
    pinMode(Player_state, INPUT_PULLUP);

    // Init USB serial port for debugging
    Serial.begin(9600);
    // Init serial port for DFPlayer Mini
    softwareSerial.begin(9600);

    // Start communication with DFPlayer Mini
    if (player.begin(softwareSerial)) {
        Serial.println("OK");
        

        // Set volume to maximum (0 to 30).
        player.volume(20);

        //----Set different EQ----
        // player.EQ(DFPLAYER_EQ_NORMAL);
        player.EQ(DFPLAYER_EQ_POP);
        //  player.EQ(DFPLAYER_EQ_ROCK);
        //  player.EQ(DFPLAYER_EQ_JAZZ);
        //  player.EQ(DFPLAYER_EQ_CLASSIC);
        //  player.EQ(DFPLAYER_EQ_BASS);

        // Play the first MP3 file on the SD card
        // player.play(1);
        player.playMp3Folder(1); // playing music at mp3 folder
    } else {
        Serial.println("Connecting to DFPlayer Mini failed!");
    }
}

void loop() {
    if (digitalRead(Player_state) == LOW) {
        Serial.println("audio is playing");
    } else {
        Serial.println("audio is not playing");
    }
}