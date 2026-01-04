#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

void avr_init(void)
{
	WDTCR = 15;
}

void avr_wait(unsigned short msec)
{
	TCCR0 = 3;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

void avr_wait_us(unsigned short usec) // Units of 8 microseconds
{
	TCCR0 = 3;
	while (usec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.000008);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

const char keys[] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
	'*', '0', '#', 'D'
};

int is_pressed(int r, int c) {
	// Set all 8 GPIOs to N/C (DDR = 0, PORT = 0)
	// Result: All rows/columns are floating (no connection -> clean slate)
	DDRC = 0x00;
	PORTC = 0x00;
	
	// Set row r to "0" to activate a specific row
	SET_BIT(DDRC, r); // Set row as output (1)
	CLR_BIT(PORTC, r); // Drive row LOW (0)
	
	// Set column c to "w1" to make it a listening input
	SET_BIT(PORTC, c + 4); // Drive column WEAK 1 (1)
	
	avr_wait(5); // Small delay for signal stabilization
	
	// If the pin value of column c reads 0, it means there's a connection between r and c
	// 0 means the column was driven from WEAK 1 to LOW externally (through a button press)
	// Key is pressed
	if (!GET_BIT(PINC, c + 4)) {
		return 1;
	}
	// Key isn't pressed
	return 0;
}

int get_key() {
	int i, j;
	// Scans the 4 by 4 keypad, checking if the key at the i-th row and j-th column was pressed
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			if (is_pressed(i, j)) {
				// Maps the (i, j) position into a number from 1 to 16
				// EX: (0, 0) -> 0*4 + 0+1 = 1
				// EX: (3, 0) -> 3*4 + 0+1 = 13
				return i*4+j+1;
			}
		}
	}
}

// Define 12 frequencies + WAIT
typedef enum {
	A, As, B, C, Cs, D, Ds, Ee, F, Fs, G, Gs, WAIT
} Note;

// Define note duration (double whole, whole, half, quarter, eighth)
typedef enum {
	DW, W, H, Q, E
} Duration;

// Define note structure
typedef struct {
	Note note;
	Duration duration;
} PlayingNote;

PlayingNote twinkle_twinkle[] = {
    // twinkle twinkle little star
    {C, H}, {C, H},
    {G, H}, {G, H},
    {Gs, H}, {Gs, H},
    {G, W},

    // how i wonder where you are
    {F, H}, {F, H},
    {Ee, H}, {Ee, H},
    {D, H}, {D, H},
    {C, W},

    // up above the world so high
    {G, H}, {G, H},
    {F, H}, {F, H},
    {Ee, H}, {Ee, H},
    {D, W},

    // like a diamond in the sky
    {G, H}, {G, H},
    {F, H}, {F, H},
    {Ee, H}, {Ee, H},
    {D, W},

    // twinkle twinkle little star
    {C, H}, {C, H},
    {G, H}, {G, H},
    {Gs, H}, {Gs, H},
    {G, W},

    // how i wonder where you are
    {F, H}, {F, H},
    {Ee, H}, {Ee, H},
    {D, H}, {D, H},
    {C, DW}
};

PlayingNote happy_birthday[] = {
	// happy birthday to you
	{C, Q}, {C, E}, {D, H},
	{C, H}, {F, H}, {Ee, W},

	// happy birthday to you
	{C, Q}, {C, E}, {D, H},
	{C, H}, {G, H}, {F, W},

	// happy birthday dear ...
	{C, Q}, {C, E}, {Gs, H},
	{E, H}, {F, H}, {E, H}, {D, W},

	// happy birthday to you
	{As, Q}, {As, E}, {A, H},
	{F, H}, {G, H}, {F, W}
};

PlayingNote little_lamb[] = {
	// Mary had a little lamb
	{Ee, Q}, {D, Q}, {C, Q}, {D, Q},
	{Ee, Q}, {Ee, Q}, {Ee, H},

	// little lamb, little lamb
	{D, Q}, {D, Q}, {D, H},
	{Ee, Q}, {G, Q}, {G, H},

	// Mary had a little lamb
	{Ee, Q}, {D, Q}, {C, Q}, {D, Q},
	{Ee, Q}, {Ee, Q}, {Ee, Q}, {Ee, Q},

	// it's fleece as white as snow
	{WAIT, E},
	{D, Q}, {D, Q}, {Ee, Q}, {D, Q}, {C, W}
};

// Tempo options (normal, slow, fast) in microseconds
const long duration_mapping_normal[] = {2000000, 1000000, 500000, 250000, 125000};
const long duration_mapping_slow[] = {4000000, 2000000, 1000000, 500000, 250000};
const long duration_mapping_fast[] = {1000000, 500000, 250000, 125000, 62500};

// Frequency options (220 Hz, 440 Hz, 880 Hz)
const float freq_mapping_220[] = {220.0, 233.1, 246.9, 261.6, 277.2, 293.7, 311.1, 329.6, 349.2, 370.0, 392.0, 415.3};
const float freq_mapping_440[] = {440.0, 466.2, 493.9, 523.3, 554.4, 587.3, 622.3, 659.3, 698.5, 740.0, 784.0, 830.6};
const float freq_mapping_880[] = {880.0, 932.3, 987.8, 1046.5, 1108.7, 1174.7, 1244.5, 1318.5, 1396.9, 1480.0, 1568.0, 1661.2};
	
float period_mapping_220[12]; // in Hz
float th_tl_mapping_220[12]; // in microseconds

// Default values (initially, music is not playing, freq = 440 Hz, tempo = normal, song = twinkle twinkle little star)
int playing = 0;
const float* current_freq = freq_mapping_440;
const long* current_tempo = duration_mapping_normal;
const PlayingNote* current_song = twinkle_twinkle;
int length_of_song = 42;

// Initialize arrays for variable P (P = 1,000,000 us / F) and ...
// TH = TL = P / 2
void init_arrays() {
	for (int i = 0; i < 12; i++) {
		// 1 s = 1000 ms = 1000000 us
		period_mapping_220[i] = 1000000.0f / current_freq[i];
		th_tl_mapping_220[i] = period_mapping_220[i] / 2.0f;
	}
}

void play_note(const PlayingNote* note) {
	int i, k;
	if (note->note == WAIT) {
		avr_wait_us(current_tempo[note->note] / 8); // Divide by 8 to get units of 8 us
		return;
	}
	
	k = current_tempo[note->duration] / period_mapping_220[note->note];
	float th = th_tl_mapping_220[note->note];
	float tl = th_tl_mapping_220[note->note];
	
	for (i = 0; i < k; i++) {
		SET_BIT(PORTB, 3);
		avr_wait_us(th / 8); // Divide by 8 to get units of 8 us
		CLR_BIT(PORTB, 3);
		avr_wait_us(tl / 8); // Divide by 8 to get units of 8 us
	}
}


void play_song(const PlayingNote song[], int length) {
	int i;
	for (i = 0; i < length; i++) {
		int key = get_key();
		if (key != 0 && keys[key - 1] == '*') {
			playing = 0;
			// Wait until the '*' key is released to avoid multiple triggers for 1 press
			while (get_key() != 0);
			// Exist when '*' is pressed
			return;
		}
		play_note(&song[i]);
	}
}

void init_lcd_lamb_song() {
	// Display song name "Mary Had a Little Lamb" on LCD
	char buf[17];
	lcd_clr();
	lcd_pos(0, 0);
	sprintf(buf, "Mary Had A");
	lcd_puts2(buf);
	lcd_pos(1, 0);
	sprintf(buf, "Little Lamb");
	lcd_puts2(buf);
}

void init_lcd_twinkle_song() {
	// Display song name "Twinkle Twinkle Little Star" on LCD
	char buf[17];
	lcd_clr();
	lcd_pos(0, 0);
	sprintf(buf, "Twinkle Twinkle");
	lcd_puts2(buf);
	lcd_pos(1, 0);
	sprintf(buf, "Little Star");
	lcd_puts2(buf);
}

void init_lcd_bday_song() {
	// Display song name "Birthday Song" on LCD
	char buf[17];
	lcd_clr();
	lcd_pos(0, 0);
	sprintf(buf, "Birthday Song");
	lcd_puts2(buf);
}

int main() {
	avr_init();
	lcd_init();
	SET_BIT(DDRB, 3);
	init_arrays();
	init_lcd_twinkle_song();
	
	while(1) {
		int key = get_key();

		if (key != 0) {
			char key_pressed = keys[key - 1];
			
			// Keypad options guide:
			// '*' = START/STOP song
			// Song options: (1) 'A' for "Twinkle Twinkle Little Star", (2) 'B' for "Mary Had A Little Lamb", (3) 'C' for "Birthday Song'
			// Frequency options: (1) '1' for 220 Hz (low pitch), (2) '2' for 440 Hz (normal pitch), '3' for 880 Hz (high pitch)
			// Tempo options: (1) '4' for slow, '5' for normal, '6' for fast

			if (key_pressed == '*') {
				playing = !playing;
			}
			else if (key_pressed == 'A') {
				current_song = twinkle_twinkle;
				length_of_song = 42;
				init_lcd_twinkle_song();
			}
			else if (key_pressed == 'B') {
				current_song = little_lamb;
				length_of_song = 27;
				init_lcd_lamb_song();
			}
			else if (key_pressed == 'C') {
				current_song = happy_birthday;
				length_of_song = 25;
				init_lcd_bday_song();
			}
			else if (key_pressed == '1') {
				current_freq = freq_mapping_220;
				init_arrays();
			}
			else if (key_pressed == '2') {
				current_freq = freq_mapping_440;
				init_arrays();
			}
			else if (key_pressed == '3') {
				current_freq = freq_mapping_880;
				init_arrays();
			}
			else if (key_pressed == '4') {
				current_tempo = duration_mapping_slow;
			}
			else if (key_pressed == '5') {
				current_tempo = duration_mapping_normal;
			}
			else if (key_pressed == '6') {
				current_tempo = duration_mapping_fast;
			}

			// Wait until key is released to avoid multiple triggers for 1 press
			while (get_key() != 0) {
				avr_wait(10);
			}
		}

		// If in playing mode, play the song
		if (playing) {
			play_song(current_song, length_of_song);
			// End playing mode, at the end of the song so it doesn't restart immediately
			playing = 0;
		}

	}
	return 0;
}
