//Port of the NESPlayer project from Joel Yliluoma : https://bisqwit.iki.fi/jutut/kuvat/programming_examples/pic/
//
//Connect a speaker to PA3 and GND

#include <stdint.h>
#include "pdk/device.h"
#include "easy-pdk/calibrate.h"

#define PWM_PORT        PA
#define PWM_PORTC       PAC
#define PWM_PIN         3
#define SEND_SAMPLE(n)  {TM2B = n;}
#define TMR0            TM3CT

unsigned char _sdcc_external_startup(void)
{
  PDK_USE_8MHZ_IHRC_SYSCLOCK();                                    //use 8MHz sysclock
  EASY_PDK_CALIBRATE_IHRC(10000000,5000);                          //tune SYSCLK to 10MHz (overclock) @5.00V

  //// PWM setup
  PWM_PORTC |= (1<<PWM_PIN);                                       //PWM output pin
  TM2C = TM2C_CLK_IHRC | TM2C_OUT_PA3 | TM2C_MODE_PWM;             //PWM clock and output mode
  TM2S = TM2S_PWM_RES_8BIT | TM2S_PRESCALE_NONE | TM2S_SCALE_NONE; //PWM speed IHRC(20MHz) /1 /1 /256(8bit) = 78125 Hz
  TM2B = 0x80;                                                     //PWM output silence (50%)

  //// TIMER setup
  TM3C = TM3C_CLK_IHRC;
  TM3S = TM3S_PRESCALE_DIV64 | TM3S_SCALE_DIV16;                   //IHRC(20MHz) / 64 / 16 = 19530 Hz
  TM3B = 0xFF;                                                     //upper bound max

  return 0;                                                        //perform normal initialization
}

static const uint8_t songdata[];
#define PTR0 0
#define PTR1 366
#define PTR2 720
#define PTR3 1000

#define MAX_LOOPS    3
#define NUM_CHANNELS 4

static uint16_t cp;  // CurrentPointer[CurChannel]
static uint8_t  vcl; // VolumeCurveLength[CurChannel]
static uint8_t  vpp; // VolumeCurvePosition[CurChannel];

static uint8_t  CurChannel;                        // 2-bit (0-3)
static uint8_t  NoteBaseOffset[NUM_CHANNELS];      // 6-bit (24-55)
static uint8_t  VolumeAndEnv[NUM_CHANNELS];

static uint8_t  VolumeCurveLength[NUM_CHANNELS];   // sign-bit + 7-bit integer
static uint8_t  VolumeCurvePosition[NUM_CHANNELS]; // 7-bit
static uint8_t  VolumeCurveStrength[NUM_CHANNELS]; // 0..15 or -1..-15

static uint8_t  ActiveVolume[NUM_CHANNELS];        // 0..15
static uint8_t  DutyCycle[NUM_CHANNELS];           // 2-bit
static uint16_t LengthCounter[NUM_CHANNELS];       // 16-bit, because multiplied by Speed
static uint8_t  CurrentNote[NUM_CHANNELS];         // 7-bit (24-70)
static uint8_t  KeyOnDelay[NUM_CHANNELS];
static uint8_t  SongSpeed[NUM_CHANNELS];

static uint16_t CurrentPhase[NUM_CHANNELS]; //= {0,0,0,0};
static uint16_t CurrentRate[NUM_CHANNELS];  //= {0,0,0,0};

// Song is ~700 lines long. These pointers need only be 10-bit.
static uint16_t CurrentPointer[NUM_CHANNELS] = {PTR0,PTR1,PTR2,PTR3};
static uint8_t  LoopCounters[MAX_LOOPS][NUM_CHANNELS]; // 4-bit
static uint16_t LoopPointers[MAX_LOOPS][NUM_CHANNELS] = { {PTR0,PTR1,PTR2,PTR3}, {PTR0,PTR1,PTR2,PTR3} };

#define U (uint8_t)
#define W (uint16_t)

#define GetByte() songdata[cp++]

static uint8_t CalculateVolume(uint8_t v/*channel volume*/, uint8_t wave/*momentary wave value*/)
{
    // Return value: approximately (wave-7.5) * v/2  (-52..+52)
    uint8_t result = 0;

    if(!(wave & 8)) wave ^= U 7;

    if(U(v & 1)) result += U(wave&0x07); // 0x08; 0..7
    wave <<= 1;

    if(U(v & 2)) result += U(wave&0x0E); // 0x10; 0..14 or 0..21
    wave <<= 1;

    if(U(v & 4)) result += U(wave&0x1C); // 0x20; 0..28 or 0..49
    wave <<= 1;

    if(U(v & 8)) result += U(wave&0x38); // 0x40; 0..56 or 0..105
    result >>= 1;                    // 0..52

    if(U(wave & 0x40)) return result;
    return -result;
}

static const uint8_t RndData[64] =
{
    0xC8,0xE8,0xE7,0xCB,0x31,0x99,0xB0,0x10,0x3F,0xED,0xC0,0x19,0x76,0xF3,0x5B,0x45,
    0x13,0xE7,0x9E,0x8C,0xDF,0x30,0xC1,0x3C,0x72,0x5F,0xC1,0x83,0x3A,0x11,0x95,0x4E,
    0x8D,0x9C,0x8F,0xE3,0x4F,0x18,0x3B,0x3B,0x32,0x3F,0xF1,0xDF,0xFF,0x69,0x40,0xFA,
    0x18,0xE8,0x64,0x26,0x8E,0xB8,0xEE,0xCD,0xD1,0x81,0x2E,0xCB,0x9B,0x63,0x9B,0x2E
};

int main(void)
{
    // This constant controls the song playback rate.
    // It is calculated with round(720 * 16 / 76.2939),
    // where 720 Hz is the desired playback tick rate
    // and 76.2939 Hz is the interval between TMR0
    // wrapping back to 0 (which comes from 20e6/4/256/256).
    enum { TIMING_NOMINATOR = 151 };

    // This constant controls the pitch of the melody.
    // It is calculated with 121.6796875 / 19531.25
    // where 19531.25 Hz is the interval between TMR0 increments
    // (which on the PIC16F628A comes from 20e6/4/256),
    // and 121.6796875 happens to be some finetuning.
    #define FREQ_SCALE 0.00623

    // Amplitudes. Chosen carefully in such manner that the sample never clips.
    enum { SQUARE0 = 3, SQUARE1 = 13, TRIVOLUME = 12, NOISE0 = 8, NOISE1 = 13, OFFSET = 0x66 };

    // Loop forever.
    for(;;)
    {
        // Update the waveform on each channel.
        // Because of timer0 interrupts (and the code executed therein
        // taking different amounts of time at different times), the
        // PCM generation may occur at uneven intervals. To compensate,
        // we take a reading from timer0, to see how many cycles we are
        // behind from what we are supposed to do. TMR0 will change
        // 20e6/4/256 = 19531.25 times per second, which is more than
        // enough for our purposes (simulation tests indicate we are
        // getting about 12500 Hz playback).
        static uint8_t  prevmul = 0, flag, speed;

        enum { FLAG_TICK = 0x01, FLAG_CURVECHANGED = 0x02 };

        if(prevmul == TMR0) continue;

        // Catch up as many cycles as we missed
        do {
            if(!prevmul) flag |= FLAG_TICK; // Did we hit the 76.2939 Hz interval?

            // Time to execute new song row?
            CurrentPhase[0] += CurrentRate[0];
            CurrentPhase[1] += CurrentRate[1];
            CurrentPhase[2] += CurrentRate[2];
            CurrentPhase[3] += CurrentRate[3];
        } while(U ++prevmul != TMR0);

        /* This function returns a waveform for a given channel
         * and its phase: Square, triangle or noise.
         * For channels 0 & 1:
         *        Square. Uses: CurrentPhase high 3 bits, and DutyCycle.
         *                Returns: 14 or 4.
         * For channel 2:
         *        Triangle. Uses: CurrentPhase high 5 bits.
         *                  Returns 0..15.
         * For channel 3:
         *        Periodic noise. Uses: CurrentPhase, and RndData[].
         *                        Returs 0 or 7.
         */

        // One triangle channel.
        uint8_t
        sample = CalculateVolume( TRIVOLUME,
                                  U(U(CurrentPhase[2] >> 11) & 15) ^ U((CurrentPhase[2] & (16<<11)) ? U 15 : U 0) );

        // One noise channel.
        sample += CalculateVolume( ActiveVolume[3],
                                   U(U((U RndData[  U(U(CurrentPhase[3] >> 2) & 0x3F) ]) >> U(CurrentPhase[3] >> 13)) & 1)
                                   ? U NOISE1 : U NOISE0);

        // Two square channels.
        sample += CalculateVolume( ActiveVolume[1],
                                   U(CurrentPhase[1] >> U(12 + DutyCycle[1])) ? U SQUARE1 : U SQUARE0 );

        sample += CalculateVolume( ActiveVolume[0],
                                   U(CurrentPhase[0] >> U(12 + DutyCycle[0])) ? U SQUARE1 : U SQUARE0 );

        // Normalize to 0..255 range (no negative numbers)
        sample += OFFSET;

        // Signal the value.
        SEND_SAMPLE(sample);

        if(flag & FLAG_TICK)
        {
            speed += TIMING_NOMINATOR;

            // Update the song state
            for(CurChannel=0; CurChannel!=NUM_CHANNELS; ++CurChannel)
            {
                flag &= ~(FLAG_TICK | FLAG_CURVECHANGED);

                // Cache important variables
                cp  = CurrentPointer[CurChannel];
                vcl = VolumeCurveLength[CurChannel];
                vpp = VolumeCurvePosition[CurChannel];

                uint16_t new_length_counter = LengthCounter[CurChannel];

                // The length intervals used by our code are for 720 Hz. (60*12).
                // Because we are running at 76.2939 Hz, we need a factor
                // of 9.437184 for preciseness.
                // We use 151/16 = 9.4375, which is pretty close.
                // It gets 720.02411 for song speed.
                // 147456/15625 would be precise, but is inconvenient.
                int8_t frames_remain = -(speed >> 4);

                uint8_t note = CurrentNote[CurChannel];
                uint8_t kod  = KeyOnDelay[CurChannel] + frames_remain;

                // Update timing
                if(W(new_length_counter & 0x8000)) // negative?
                {
                    // Take back the part we did not consume yet.
                    frames_remain += new_length_counter;
                    new_length_counter = 0;
                }
                if(!new_length_counter)
                {
                    // Read any following commands.
                    // Stop when the first note/pause is encountered.
                    uint8_t length_counter = 6;
                    uint8_t note_delay     = 0;
                    for(;;)
                    {
                        uint8_t byte  = GetByte();
                        if( U(byte & 0x80))
                        {
                            if( U(byte < U 0xE0))
                            {
                                length_counter = U(U length_counter << U(U(byte >> 4) & U 7));
                                // Length can be: 6,12,24,48,96,192
                                //            Or: 9,18,36,72,144,288 (288 is too large though)
                                //            Or: 4,8,16,32,64,128
                            NoteCommon:;
//                                new_length_counter = length_counter * SongSpeed[CurChannel];
//                                kod                = note_delay     * SongSpeed[CurChannel] - 1;
                                uint8_t s = SongSpeed[CurChannel];
                                uint16_t length_counter_16bit = length_counter;
                                for(kod=0xFF ; ; )
                                {
                                    if(U(s & 1))
                                    {
                                        new_length_counter += length_counter_16bit;
                                        kod                += note_delay;
                                    }
                                    if(!U(s >>= 1)) break;
                                    length_counter_16bit <<= 1;
                                    note_delay           <<= 1;
                                }
                                note = byte & 0x0F;
                                if(note) note += NoteBaseOffset[CurChannel];
                                break;
                            }
                            // byte >= 0xE0, i.e. SetNoteBaseOffset

                            uint8_t tmp = U(byte & 0x1F) + 24 - 1;
                            if(byte == 0xFF) tmp = GetByte()-1;
                            NoteBaseOffset[CurChannel] = tmp;
                        }
                        else
                        {
                            uint8_t command = byte & 7;
                            byte >>= 3; // 4-bit parameter
                            if( U(command & 4))
                            {
                                // 4..7
                                if( U(command & 2))
                                {
                                    // 6..7
                                    if( U(command & 1))  // 7: Set Volume Curve (next 2 bytes)
                                    {
                                        vcl = GetByte();
                                        if( U(vcl & 0x80) ) byte = -byte;
                                        VolumeCurveStrength[CurChannel] = byte;
                                        flag |= FLAG_CURVECHANGED;
                                    }
                                    else // 6: Set Volume And Env (next byte)
                                    {
                                        if(byte)
                                            SongSpeed[CurChannel] = byte; // Set Speed
                                        else
                                            VolumeAndEnv[CurChannel] = GetByte();
                                    }
                                }
                                else
                                {
                                    // 4..5
                                    uint8_t loop_id = 1; // subloop
                                #if MAX_LOOPS >= 3
                                    if( U(command & 1) ) // 5: End Subloop2
                                    {
                                        ++loop_id;
                                        //passthru
                                    }
                                #endif
                                    // 4: End loop
                                #if MAX_LOOPS >= 2
                                    if(!byte)
                                        --loop_id;
                                    else // 0=unconditional mainloop
                                    {
                                        uint8_t lf = LoopCounters[loop_id][CurChannel];
                                        if( U(lf++) == byte) lf = 0; // done
                                        LoopCounters[loop_id][CurChannel] = lf;
                                        if(!lf) goto didloop;
                                    }
                                #else
                                    --loop_id;
                                #endif
                                    // Goto loop
                                    cp = LoopPointers[loop_id][CurChannel];
                                 didloop:;
                                }
                            }
                            else
                            {
                                // 0..3
                                if( U(command & 2) )
                                {
                                    // 2..3
                                    if( U(command & 1) ) // 3: Use fixed 72-length note.
                                    {
                                        length_counter = 72;
                                        goto NoteCommon;
                                    }
                                    if( U(byte & 8) )
                                        if( U(byte & 4) ) // Begin loop
                                        {
                                            // It does not matter whether we make the anchor
                                            // at the beginning of this instruction or after it.
                                            LoopPointers[ U(byte & 3) ][CurChannel] = cp;
                                        }
                                        else // Set duty cycle
                                            DutyCycle[CurChannel] = U(1 + U(byte & 3));
                                    else
                                        note_delay = byte; // Delay next note by some length-ticks
                                }
                                else
                                {
                                    // 0: Interpret next note at 3/2 its nominal length (9)
                                    // 1: Triplet timing (next note at 2/3 its nominal length) (4)
                                    if( U(command&1) )
                                        length_counter = U(6*2/3);
                                    else
                                        length_counter = U(6*3/2);
                                }
                            }
                        }
                    }
                }

                // Check if a note is due to be played right now
                if( U(kod & 0x80) && !U(note & 0x80))
                {
                    // Begin note.
                    uint16_t period = 0;
                    if(note)
                    {
                        static const uint16_t RateTable[24] =
                        {
                            // These constants are 65535 * 2^(n/12)/ with
                            // -11<=n<=0, representing the equal temperament
                            // tuning with maximum 16-bit precision.
                            34716*FREQ_SCALE*0.5, 36780*FREQ_SCALE*0.5, 38967*FREQ_SCALE*0.5,
                            41284*FREQ_SCALE*0.5, 43739*FREQ_SCALE*0.5, 46340*FREQ_SCALE*0.5,
                            49096*FREQ_SCALE*0.5, 52015*FREQ_SCALE*0.5, 55108*FREQ_SCALE*0.5,
                            58385*FREQ_SCALE*0.5, 61857*FREQ_SCALE*0.5, 65535*FREQ_SCALE*0.5,
                            34716*FREQ_SCALE*1.0, 36780*FREQ_SCALE*1.0, 38967*FREQ_SCALE*1.0,
                            41284*FREQ_SCALE*1.0, 43739*FREQ_SCALE*1.0, 46340*FREQ_SCALE*1.0,
                            49096*FREQ_SCALE*1.0, 52015*FREQ_SCALE*1.0, 55108*FREQ_SCALE*1.0,
                            58385*FREQ_SCALE*1.0, 61857*FREQ_SCALE*1.0, 65535*FREQ_SCALE*1.0
                        };
                        if(CurChannel == 3) note += 6;//-= 20;
                        if(CurChannel == 2) note -= 12;
                        uint8_t o = 0;
                        for(; note >= 24; note -= 24) o += 2; // div-mod
                        // With note range 24-70, the octave range is 2..5.

                        period = W((W RateTable[note]) << o);
                    }
                    CurrentRate[CurChannel] = period;
                    note |= 0x80;
                    flag |= FLAG_CURVECHANGED;
                }

                if( U(flag & FLAG_CURVECHANGED))
                {
                    // Reset volume curve
                    if(CurChannel == 2)
                    {
                        // Linear counter
                        vpp = U(VolumeAndEnv[CurChannel] & 0x7F);
                    }
                    else if(vcl)
                    {
                        ActiveVolume[CurChannel] = U(vcl & 0x80) ? U 15 : U 0;
                        vpp = 0;
                    }
                }

                // Update timing
                LengthCounter[CurChannel] = W(new_length_counter + frames_remain);

                // Update volume curve
                if(CurChannel == 2)
                {
                    // Linear counter, if enabled
                    if(!U(VolumeAndEnv[CurChannel] & 0x80))
                    {
                        // Accumulate the linear counter twice per frame
                        if(vpp && (!U --vpp || !U --vpp))
                            CurrentRate[CurChannel] = 0;
                    }
                }
                else
                {
                    // At VolumeCurveLength intervals, VolumeCurveStrength is added
                    // to the volume, clamping between 0 and (VolumeEnv & 0xF).
                    uint8_t s = ActiveVolume[CurChannel];
                    if(U(vcl & 0x7F) == U ++vpp)
                    {
                        vpp = 0;
                        // Add S (or -S if l&0x80)
                        s += VolumeCurveStrength[CurChannel];
                        // Clamp to 0..v
                        if(U(s & 0x80)) s = 0;
                    }

                    uint8_t v = VolumeAndEnv[CurChannel] & 0x0F;
                    if(s >= v)   s = v;
                    ActiveVolume[CurChannel] = s;
                }

                // Save cached variables back
                CurrentPointer[CurChannel]      = cp;
                VolumeCurveLength[CurChannel]   = vcl;
                VolumeCurvePosition[CurChannel] = vpp;
                CurrentNote[CurChannel] = note;
                KeyOnDelay[CurChannel] = kod;
            }

            speed &= 0xF;
        }
    }
}

static const uint8_t songdata[] = {
  0x62,0x2E,0x06,0x3E,0x4A,0x0F,0x84,0xED,0xA1,0x6A,0x94,0x94,0xA4,0x0C,0xA1,0xA0,0x91,0x91,0xA4,0x94,0x94,0xA4,0xA1,0xA0,0xA8,0xA6,0xA8,0xA0,
  0x6A,0x94,0x94,0xA4,0x0C,0xA1,0xA0,0xA8,0xA0,0xA6,0xA0,0xA4,0xA0,0xA6,0x03,0x6A,0x96,0x96,0xA6,0x0C,0xA3,0xA0,0xA8,0xA0,0xA6,0xA0,0xA4,0xA0,
  0xA3,0xA0,0xA1,0xA0,0xA1,0xA8,0xAB,0x53,0xA1,0xA0,0xA1,0xA8,0xAB,0xAA,0xA0,0xAF,0xF0,0xAD,0xA0,0x6A,0x91,0x91,0xA1,0x0C,0xED,0xA1,0xA0,0x91,
  0x91,0xA4,0x94,0x94,0xA4,0xA1,0xA0,0xA8,0xA6,0xA8,0xA0,0x6A,0x94,0x94,0xA4,0x0C,0xA1,0xA0,0xA8,0xA0,0xA6,0xA0,0xA4,0xA0,0xA6,0x03,0x6A,0x96,
  0x96,0xA6,0x0C,0xA3,0xA0,0xA8,0xA0,0xA6,0xA0,0xA4,0xA0,0xA3,0xA0,0xA1,0xA0,0xA1,0xA8,0xAB,0x53,0xA1,0xA0,0xA1,0xA8,0xAB,0xAA,0xA0,0xAB,0x52,
  0x0F,0x90,0x12,0xAD,0xBD,0xCD,0xAB,0xF0,0xBD,0xBA,0xB8,0xBA,0xA0,0xC8,0xA8,0x53,0xA5,0xA6,0xA5,0xA1,0xA0,0xA1,0xA5,0xA8,0x00,0xCA,0xA8,0xBD,
  0xBA,0xB8,0xBA,0x0A,0xA8,0xC8,0xA8,0xA5,0xA8,0xA9,0xA0,0xA9,0xA9,0xAC,0xFF,0x38,0xC1,0x06,0x3F,0x42,0x1F,0x95,0xE3,0x00,0xC6,0xA4,0xB9,0xB7,
  0xB6,0xB7,0x12,0xA6,0xC6,0xB6,0xA4,0xB9,0xB7,0xB6,0xB7,0x0A,0xA2,0xC2,0xA2,0xA4,0xA6,0xD1,0x52,0xAB,0xA0,0xAB,0xED,0xA8,0xAB,0x53,0xA1,0xA0,
  0xA1,0xA8,0xAB,0xAA,0xA0,0xAB,0x4A,0x0A,0xAD,0xCD,0xBD,0xAB,0xAD,0xA0,0xF7,0x33,0xAA,0xA8,0xA6,0xA5,0xC3,0xB3,0xA1,0xA3,0xA0,0x33,0xA6,0xA8,
  0xB6,0xC5,0xA5,0xA3,0xA1,0x53,0xB8,0xB6,0xB5,0xA5,0xA6,0xA5,0xC3,0xED,0xA1,0xA0,0xA1,0xA8,0xAB,0xAA,0xA0,0xAB,0x52,0x12,0xAD,0xBD,0xCD,0xAB,
  0xF0,0xBD,0xBA,0xB8,0xBA,0xA0,0xC8,0xA8,0x53,0xA5,0xA6,0xA5,0xA1,0xA0,0xA1,0xA5,0xA8,0x00,0xCA,0xA8,0xBD,0xBA,0xB8,0xBA,0x0A,0xA8,0xC8,0xA8,
  0xA5,0xA8,0xA9,0xA0,0xA9,0xA9,0xAC,0xFF,0x38,0x0B,0x4A,0xED,0xA4,0xA0,0xA4,0xA4,0xA3,0xB4,0xAD,0xBB,0xB9,0xB8,0xB6,0xA6,0xA0,0xA8,0xA8,0xA0,
  0xA8,0xB0,0xA6,0xA0,0xA8,0xA8,0xA0,0xA8,0xA0,0xA6,0xA4,0xA0,0xA4,0xA4,0xA3,0xB4,0xAD,0xBB,0xB9,0xB8,0xB6,0xA0,0xA3,0xA4,0xA3,0x0A,0xA1,0xC1,
  0xD0,0x04,0x62,0x6A,0x2E,0x06,0x3A,0x4A,0x0F,0x84,0xE8,0xA0,0x72,0x96,0x96,0xA6,0x0D,0xA1,0xA0,0x91,0x91,0xA6,0x96,0x96,0xA6,0xA1,0xA0,0xA9,
  0xA8,0xA9,0xA0,0x72,0x96,0x96,0xA6,0x0D,0xA1,0xA0,0xA9,0xA0,0xA8,0xA0,0xA6,0xA0,0xA8,0x03,0x72,0x98,0x98,0xA8,0x0D,0xA4,0xA0,0xA9,0xA0,0xA8,
  0xA0,0xA6,0xA0,0xA4,0xB0,0xA6,0xA0,0xA6,0xAD,0xED,0xAB,0x53,0xA1,0xA0,0xA1,0xA8,0xAB,0xAA,0xAB,0xAD,0x0C,0x52,0x0F,0x92,0x90,0x12,0xAD,0xBD,
  0xCD,0xAB,0xF0,0xBD,0xBA,0xB8,0xBA,0xA0,0xC8,0xA8,0x53,0xA5,0xA6,0xA5,0xA1,0xA0,0xA1,0xA5,0xA8,0x00,0xCA,0xA8,0xBD,0xBA,0xB8,0xBA,0x0A,0xA8,
  0xC8,0xA8,0xA5,0xA8,0xA9,0xA0,0xA9,0xA9,0xAC,0xFF,0x38,0x12,0xB1,0xED,0x94,0x42,0xA1,0x6A,0x94,0x94,0xA4,0x0C,0xA1,0xA0,0x91,0x91,0xA4,0x94,
  0x94,0xA4,0xA1,0xA0,0xA8,0xA6,0xA8,0xA0,0x6A,0x94,0x94,0xA4,0x0C,0xA1,0xA0,0xA8,0xA0,0xA6,0xA0,0xA4,0xA0,0xA6,0x03,0x6A,0x96,0x96,0xA6,0x0C,
  0xA3,0xA0,0xA8,0xA0,0xA6,0xA0,0xA4,0xA0,0xA3,0x52,0xB0,0xA1,0xA0,0xA1,0xA8,0xAB,0x53,0xA1,0xA0,0xA1,0xA8,0xAB,0xAA,0xFE,0xA1,0xA3,0x42,0xA0,
  0x6A,0xF0,0x91,0x91,0xA1,0x0C,0xED,0xA1,0xA0,0x91,0x91,0xA4,0x94,0x94,0xA4,0xA1,0xA0,0xA8,0xA6,0xA8,0xA0,0x6A,0x94,0x94,0xA4,0x0C,0xA1,0xA0,
  0xA8,0xA0,0xA6,0xA0,0xA4,0xA0,0xA6,0x03,0x6A,0x96,0x96,0xA6,0x0C,0xA3,0xA0,0xA8,0xA0,0xA6,0xA0,0xA4,0xA0,0xA3,0x52,0xA0,0xA1,0xA0,0xA1,0xA8,
  0xAB,0xCA,0xA1,0xA0,0xA1,0xA8,0xAB,0xAA,0xF4,0xAB,0xAD,0x0F,0x8A,0x90,0x12,0xA6,0xB6,0xC6,0xA4,0xB9,0xB6,0xB4,0xB6,0xA0,0xC4,0xA4,0x33,0xA1,
  0xA2,0xA1,0xF0,0xA1,0xA0,0xA1,0xA5,0xA8,0x00,0xCA,0xA8,0xBD,0xBA,0xB8,0xBA,0x0A,0xA8,0xC8,0xA8,0xA5,0xA8,0xA9,0xA0,0xA9,0xA9,0xAC,0x00,0xA0,
  0x4A,0xE8,0xA6,0xA0,0xA6,0xA6,0xA4,0xB6,0xAE,0xBB,0xBB,0xB9,0xB8,0xA8,0xA0,0xA9,0xA9,0xA0,0xA9,0xB0,0xA8,0xA0,0xA9,0xA9,0xA0,0xA9,0xA0,0xA8,
  0xA6,0xA0,0xA6,0xA6,0xA4,0xB6,0xAE,0xBB,0xBB,0xB9,0xB8,0xA0,0xA4,0xA6,0xA4,0x0A,0xA1,0xC1,0xD0,0x04,0x62,0x6A,0x72,0x2E,0x06,0x41,0xF5,0xA5,
  0x95,0x95,0x3D,0x72,0xA1,0x91,0x91,0x3D,0x72,0xA3,0x93,0x93,0x3D,0x72,0xA5,0x95,0x95,0x35,0xA3,0x0A,0xA5,0x0C,0x6A,0xA6,0x96,0x96,0x0C,0xA6,
  0x9D,0x9D,0x9A,0x9A,0xAA,0xA0,0x96,0x96,0xAA,0x96,0x96,0xF7,0xAF,0x98,0x98,0xAB,0x94,0x94,0x6A,0xA3,0x93,0x93,0x0C,0xA3,0x9A,0x9A,0x96,0x96,
  0xAD,0xA0,0x93,0x93,0xA6,0x93,0x93,0xAD,0x96,0x96,0xAA,0x93,0x93,0x6A,0xA4,0x94,0x94,0x0C,0xA4,0x9B,0x9B,0x98,0x98,0xA8,0xA0,0x94,0x94,0xA8,
  0x94,0x94,0xAF,0x98,0x98,0xAB,0x94,0x94,0x6A,0xA1,0x91,0x91,0x14,0xA1,0xA2,0xA0,0xA2,0xA2,0xA2,0xB2,0xB0,0x6A,0x72,0xF5,0xA5,0x95,0x95,0x3D,
  0x72,0xA1,0x91,0x91,0x3D,0x72,0xA3,0x93,0x93,0x3D,0x72,0xA5,0x95,0x95,0x35,0xA3,0x0A,0xA5,0x0C,0x6A,0xA6,0x96,0x96,0x0C,0xA6,0x9D,0x9D,0x9A,
  0x9A,0xAA,0xA0,0x96,0x96,0xAA,0x96,0x96,0xF7,0xAF,0x98,0x98,0xAB,0x94,0x94,0x6A,0xA3,0x93,0x93,0x0C,0xA3,0x9A,0x9A,0x96,0x96,0xAD,0xA0,0x93,
  0x93,0xA6,0x93,0x93,0xAD,0x96,0x96,0xAA,0x93,0x93,0x6A,0xA4,0x94,0x94,0x0C,0xA4,0x9B,0x9B,0x98,0x98,0xA8,0xA0,0x94,0x94,0xA8,0x94,0x94,0xAF,
  0x98,0x98,0xAB,0x94,0x94,0x6A,0xA1,0x91,0x91,0x14,0xA1,0xA2,0xA0,0xA2,0xA2,0xA2,0xB2,0xA0,0xED,0xB9,0x6A,0x99,0x99,0xA9,0x14,0xBB,0x6A,0x9B,
  0x9B,0xAB,0x14,0x6A,0xED,0xB1,0xAD,0xA3,0xAF,0xA4,0xEF,0xAE,0xA1,0x0C,0xB7,0x6A,0x97,0x97,0xA7,0x14,0xB9,0x6A,0x99,0x99,0xA9,0x0C,0x99,0x99,
  0xBA,0xBB,0xF7,0x9B,0x9B,0x9A,0x9A,0x98,0x98,0x96,0x96,0xA1,0xA3,0xA0,0xA1,0xB3,0xA1,0xA2,0xB3,0x04,0x62,0x2E,0x06,0x3F,0x57,0x82,0xE2,0x91,
  0x90,0x91,0x91,0x27,0x83,0xA6,0x57,0x82,0x91,0x91,0x04 };
