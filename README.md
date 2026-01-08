# :loud_sound: EMBEDDED MUSIC PLAYER

## :open_book: OVERVIEW
Date: May 2025\
Developer(s): Ashneet Rathore\
Based on assignment instructions from Prof. Amir Rahmani

Embedded music player is an embedded systems project using an ATMega32 microcontroller. The system integrates a keypad, LCD, and speaker on a breadboard circuit to play predefined songs, display song titles, and respond to real-time user input. Using the keypad, users can start or stop songs, switch between songs, adjust pitch, and change tempo.

## :brain: FIRMWARE DESIGN
The core software logic of the music player focuses on generating singular music notes at the microcontroller level and combining them to play complete songs. A `PlayingNote` struct represents a single note, storing its pitch and duration, and songs are encoded as arrays of these structs. The `play_song()` function iterates through each struct and calls `play_note`, which generates sound by toggling a GPIO output pin to produce a square-wave signal at the desired frequency for the specified duration.

Precise timing is critical for accurate audio generation, so the program defines an `avr_wait_us()` function that provides fine-grained delay control in units of 8 microseconds, This high-resolution delay is necessary to accurately control the high and low periods (`th` and `tl`) of each waveform cycle, ensuring consistent pitch and tempo during playback.

User input is handled through polling-based keypad scanning, allowing the user to start or stop playback, select from three pre-encoded songs - *Twinkle Twinkle Little Star*, *Mary Had a Little Lamb*, and *Birthday Song* — and adjust pitch and tempo in real time. The program updates the song title displayed on the LCD accordingly.

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

## :electric_plug: CIRCUIT SET UP GUIDE

### BUILDING THE POWER SUPPLY
