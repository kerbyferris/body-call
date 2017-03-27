// Pin Definitions
// Rows are connected to
//const int row1 = 3;
//const int row2 = 4;
//const int row3 = 5;
//const int row4 = 6;
//const int row5 = 7;
const int row1 = 9;
const int row2 = 10;
const int row3 = 11;
const int row4 = 12;
const int row5 = 13;

// The 74HC595 uses a serial communication 
// link which has three pins
//const int clock = 9;
//const int latch = 10;
//const int data = 11;
const int clock = 6;
const int latch = 7;
const int data = 8;


uint8_t keyToMidiMap[37];

boolean keyPressed[37];

int noteVelocity = 127;


// use prepared bit vectors instead of shifting bit left everytime
int bits[] = { B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000  };


// 74HC595 shift to next column
void scanColumn(int value) {
    digitalWrite(latch, LOW); //Pulls the chips latch low
    shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
    digitalWrite(latch, HIGH); //Pulls the latch high displaying the data

}

void setup() {

    // Map scan matrix buttons/keys to actual Midi note number. Lowest num 41 corresponds to F MIDI note.
    keyToMidiMap[0] = 36;
    keyToMidiMap[1] = 29;
    keyToMidiMap[2] = 30; 
    keyToMidiMap[3] = 31; 
    keyToMidiMap[4] = 32;
    keyToMidiMap[5] = 33; 
    keyToMidiMap[6] = 34;
    keyToMidiMap[7] = 35;

    keyToMidiMap[8] = 44;    
    keyToMidiMap[1 + 8] = 37;
    keyToMidiMap[2 + 8] = 38;
    keyToMidiMap[3 + 8] = 39;
    keyToMidiMap[4 + 8] = 40;
    keyToMidiMap[5 + 8] = 41;
    keyToMidiMap[6 + 8] = 42;
    keyToMidiMap[7 + 8] = 43;

    keyToMidiMap[16] = 52;
    keyToMidiMap[1 + 16] = 45;
    keyToMidiMap[2 + 16] = 46;
    keyToMidiMap[3 + 16] = 47;
    keyToMidiMap[4 + 16] = 48;
    keyToMidiMap[5 + 16] = 49;
    keyToMidiMap[6 + 16] = 50;
    keyToMidiMap[7 + 16] = 51;

    keyToMidiMap[24] = 60;
    keyToMidiMap[1 + 24] = 53;
    keyToMidiMap[2 + 24] = 54;
    keyToMidiMap[3 + 24] = 55;
    keyToMidiMap[4 + 24] = 56;
    keyToMidiMap[5 + 24] = 57;
    keyToMidiMap[6 + 24] = 58;
    keyToMidiMap[7 + 24] = 59;

    keyToMidiMap[32] = 68;
    keyToMidiMap[1 + 32] = 61;
    keyToMidiMap[2 + 32] = 62;
    keyToMidiMap[3 + 32] = 63;
    keyToMidiMap[4 + 32] = 64;
    keyToMidiMap[5 + 32] = 65;
    keyToMidiMap[6 + 32] = 66;
    keyToMidiMap[7 + 32] = 67;

    // setup pins output/input mode
    pinMode(data, OUTPUT);
    pinMode(clock, OUTPUT);
    pinMode(latch, OUTPUT);

    pinMode(row1, INPUT);
    pinMode(row2, INPUT);
    pinMode(row3, INPUT);
    pinMode(row4, INPUT);
    pinMode(row5, INPUT);

    Serial.begin(31250);

    delay(1000);


}

void loop() {

    for (int col = 0; col < 8; col++) {

        // shift scan matrix to following column
        scanColumn(bits[col]);

        // check if any keys were pressed - rows will have HIGH output in this case corresponding
        int groupValue1 = digitalRead(row1);
        int groupValue2 = digitalRead(row2);
        int groupValue3 = digitalRead(row3);
        int groupValue4 = digitalRead(row4);
        int groupValue5 = digitalRead(row5);

        // process if any combination of keys pressed
        if (groupValue1 != 0 || groupValue2 != 0 || groupValue3 != 0 || groupValue4 != 0 || groupValue5 != 0) {

            if (groupValue1 != 0 && !keyPressed[col]) {
                keyPressed[col] = true;
                noteOn(0x90, keyToMidiMap[col], noteVelocity);

            }

            if (groupValue2 != 0 && !keyPressed[col + 8]) {
                keyPressed[col + 8] = true;
                noteOn(0x90, keyToMidiMap[col + 8], noteVelocity);

            }

            if (groupValue3 != 0 && !keyPressed[col + 16]) {
                keyPressed[col + 16] = true;
                noteOn(0x90, keyToMidiMap[col + 16], noteVelocity);

            }

            if (groupValue4 != 0 && !keyPressed[col + 24]) {
                keyPressed[col + 24] = true;
                noteOn(0x90, keyToMidiMap[col + 24], noteVelocity);

            }

            //if (groupValue5 != 0 && !keyPressed[col + 32]) {
            //    keyPressed[col + 32] = true;
            //    noteOn(0x90, keyToMidiMap[col + 32], noteVelocity);

            //}


        }

        //  process if any combination of keys released
        if (groupValue1 == 0 && keyPressed[col]) {
            keyPressed[col] = false;
            noteOn(0x90, keyToMidiMap[col], 0);

        }

        if (groupValue2 == 0 && keyPressed[col + 8]) {
            keyPressed[col + 8] = false;
            noteOn(0x90, keyToMidiMap[col + 8], 0);

        }

        if (groupValue3 == 0 && keyPressed[col + 16]) {
            keyPressed[col + 16] = false;
            noteOn(0x90, keyToMidiMap[col + 16], 0);

        }

        if (groupValue4 == 0 && keyPressed[col + 24]) {
            keyPressed[col + 24] = false;
            noteOn(0x90, keyToMidiMap[col + 24], 0);

        }

        //if (groupValue5 == 0 && keyPressed[col + 32]) {
        //    keyPressed[col + 32] = false;
        //    noteOn(0x90, keyToMidiMap[col + 32], 0);

        //}


    }


}


void noteOn(int cmd, int pitch, int velocity) {
    Serial.write(cmd);
    Serial.write(pitch);
    Serial.write(velocity);
}
