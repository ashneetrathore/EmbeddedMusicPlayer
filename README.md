# :loud_sound: EMBEDDED MUSIC PLAYER

## :open_book: OVERVIEW
Date: May 2025\
Developer(s): Ashneet Rathore\
Based on assignment instructions from Prof. Amir Rahmani

Embedded Music Player is an embedded systems project using an ATMega32 microcontroller. The system integrates a keypad, LCD, and speaker on a breadboard circuit to play predefined songs, display song titles, and respond to real-time user input. Using the keypad, users can start or stop songs, switch between songs, adjust pitch, and change tempo.

## :brain: FIRMWARE DESIGN
The core software logic of the music player focuses on generating singular music notes at the microcontroller level and combining them to play complete songs. A `PlayingNote` struct represents a single note, storing its pitch and duration, and songs are encoded as arrays of these structs. The `play_song()` function iterates through each struct and calls `play_note`, which generates sound by toggling a GPIO output pin to produce a square-wave signal at the desired frequency for the specified duration.

**Precise timing** is critical for accurate audio generation, so the program defines an `avr_wait_us()` function that provides fine-grained delay control in units of 8 microseconds, This high-resolution delay is necessary to accurately control the high and low periods (`th` and `tl`) of each waveform cycle, ensuring consistent pitch and tempo during playback.

User input is handled through **polling-based keypad scanning**, allowing the user to start or stop playback, select from three pre-encoded songs - *Twinkle Twinkle Little Star*, *Mary Had a Little Lamb*, and *Birthday Song* — and adjust pitch and tempo in real time. The program updates the song title displayed on the LCD accordingly.

## :open_file_folder: PROJECT FILE STRUCTURE
```bash
EmbeddedMusicPlayer/
│── main.c       # Implements main program logic for the music player
│── avr.h        # Defines AVR macros and timing utilities
│── lcd.h        # Declares LCD control and display functions
│── lcd.c        # Implements LCD control and display functions
│── README.md    # Project documentation
└── .gitignore   # Excludes files and folders from version control
```

## :gear: CIRCUIT SET UP GUIDE
### :toolbox: REQUIRED HARDWARE


### :battery: BUILDING THE POWER SUPPLY
The voltage regulator has three pins: input (Pin 1), ground (Pin 2), and output (Pin 3). The circuit takes +9V as input and regulates it down to a +5V output, using the capacitor to smooth out the output voltage.
1. Connect the positive terminal of the 9V Battery to Pin 1.
2. Connect the negative terminal of the 9V Battery to Pin 2.
3. Connect the negative (shorter) leg of a capacitor to Pin 2, and the positive leg of a capacitor to Pin 3.
4. Connect Pin 3 to the positive rail of the breadboard and Pin 2 to the negative rail to power one side of the breadboard.
5. Join the two power rails on each side of the breadboard (positive together, negative together) to deliver voltage to both sides.

### :eletric_plug: CONNECTING THE PROGRAMMER TO THE MICROCONTROLLER
1. Connect the ATMEGA32 microcontroller to the breadboard securely.
2. Connect the 6 programmer pins to the microcontroller as shown below:
    | Programmer Pin    | Microcontroller Pin |
    |-------------------|---------------------|
    | 1                 | PB6 (Pin 7)         |
    | 2                 | PB7 (Pin 8)         |
    | 3                 | RESET (Pin 9)       |
    | 4                 | VCC (Pin 10)        |
    | 5                 | PB5 (Pin 6)         |
    | 6                 | GND (Pin 11)        | 
3. Power the microcontroller by connecting VCC (Pin 10) to the positive rail of the breadboard and GND (Pin 11) to the negative rail.

### :hourglass: CONNECTING THE 8 MHZ CRYSTAL TO THE MICROCONTROLLER
1. Connect one leg of a 8 MHz crystal to XTAL2 (Pin 12) on the microcontroller.
2. Connect the other leg of the crystal to XTAL1 (Pin 13) on the microcontroller.
>[!IMPORTANT]
> When running the program in Microchip Studio, configure the fuses:
> 1. In the top navigation bar, go to *Tools* → *Device Programming* → *Fuses* → *LOW_SUT_CKSEL*.
> 2. Select *Ext.Crystal/Resonator High Freq: Start-up time: 16k CK + 64 ms*.
> 3. Click *Program*.

### :1234: CONNECTING THE KEYPAD TO THE MICROCONTROLLER
1. Connect the 8 keypad pins to PORT C of the microcontroller as shown below:
    | Keypad Pin        | Microcontroller Pin |
    |-------------------|---------------------|
    | C0                | PC0 (Pin 22)        |
    | C1                | PC1 (Pin 23)        |
    | C2                | PC2 (Pin 24)        |
    | C3                | PC3 (Pin 25)        |
    | R0                | PC7 (Pin 29)        |
    | R1                | PC6 (Pin 28)        | 
    | R2                | PC5 (Pin 27)        |
    | R3                | PC4 (Pin ) 26       | 

### :framed_picture: CONNECTING THE LCD TO THE MICROCONTROLLER


### :loudspeaker: CONNECTING THE SPEAKER TO THE MICROCONTROLLER
1. Connect the negative terminal of the speaker to the negative rail of the breadboard.
2. Connect one leg of a capacitor to PB3 (Pin 4) on the microcontroller.
3. Connect the other leg of a capacitor to the positive terminal of the speaker.