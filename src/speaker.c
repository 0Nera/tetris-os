#include "speaker.h"

#define NOTE_NONE 12

// SEE: https://wiki.osdev.org/PC_Speaker
// SEE ALSO: https://web.archive.org/web/20171115162742/http://guideme.itgo.com/atozofc/ch23.pdf
// Precalculated LUT table for notes

static u16 notes[7][12] = {
    { 36485, 34437, 32505, 30680, 28958, 27333, 25799, 24351, 22984, 21694, 20477, 19327 },
    { 18243, 17219, 16252, 15340, 14479, 13666, 12899, 12175, 11492, 10847, 10238, 9664 },
    { 9121, 8609, 8126, 7670, 7240, 6833, 6450, 6088, 5746, 5424, 5119, 4832 },
    { 4561, 4305, 4063, 3835, 3620, 3417, 3225, 3044, 2873, 2712, 2560, 2416 }, 
    { 2280, 2152, 2032, 1918, 1810, 1708, 1612, 1522, 1437, 1356, 1280, 1208 },
    { 1140, 1076, 1016, 959, 905, 854, 806, 761, 718, 678, 640, 604},
    { 570, 538, 508, 479, 452, 427, 403, 380, 359, 339, 320, 302 }
};

bool note_playing = false;

void speaker_note(u8 octave, u8 note) {
    if (note == NOTE_NONE)
    {
        speaker_pause();

        return;
    }

    speaker_play(notes[octave][note]);
}

void speaker_play(u16 d) {
    outportb(0x43, 0xB6);
    outportb(0x42, (u8) (d & 0xFF));
    outportb(0x42, (u8) ((d >> 8) & 0xFF));

    // If there already is a note playing, re-enabling it just makes the timer start over - thus it gets choppy. 
    // By just changing the frequency when the speaker output is already enabled, we can change frequencys without any choppyness penalty
    if (!note_playing)
    {
        note_playing = true;
        outportb(0x61, inportb(0x61) | 0x3);
    }
}

void speaker_pause() {
    note_playing = false;

    outportb(0x61, inportb(0x61) & 0xFC);
}
