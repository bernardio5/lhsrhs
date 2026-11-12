

README for lhsrhs ("Left-Hand Side/Right-Hand Side") ("Lezzerhess")

Reader beware! This is an "Esoteric" interface. It's inscrutable! It might not be for you. You will have to read this to get it to do anything. Control-Q quits! Control-c is the "cut" part of cut and paste, which might frustrate some people. Also, it's *nerdy*: the interface is built in ncurses, which is also used by Vim: esoteric. VT-100 terminal support, check! 

I really made this for myself. I imagine using it in a performance! In the performance, I'd project my desktop, and the audience would see what I'm doing, and it would be dense, fast, colorful, and clearly, controlling the music generation, somehow. I have prioritized feature inclusion, control terseness and interface density (20 sequencers simultaneously editable! 160 controls lerping simultaneously), not ease of use or scrutability. For instance, it uses its own numbering system! For instance, do you know what "lerping" is? Read on!

It's a Linux console program; to run it, you'll probably have to build it, and to do that, you'll need to duplicate this project, install the STK audio toolkit, and copy its "include" directory into the directory that contains this README.
Then, in a console, cd to that directory and use the "make" command. It should build in, like, 2 seconds. You might also have to install the "ncurses" library, but usually, ncurses comes installed with your Linux.  

Then enter "./lhsrhs" and off you go!

The program is a sequencer. When it's running, it should immediately start with a basic drum beat at 120 BPM. If it doesn't, it's broken; blame me and quit.

The program takes over a console! It doesn't make a window-- too 21st-century-- so *tired* of the 21st century. If you want graphics, I guess technically numbers and letters are graphical? When the thing is running, the console will show ~20 rows. Each row is its own little thing. There are colors! 

The top row is the Timer row. It controls BPM, beats/min. You'll see a "120" right there, and also, to the side should be a little blurb that says,  "timer row / BPM:120". 

The rows repeat; there's a left-hand-side (LHS) and a RHS-- hence the program name. You can edit the LHS, then tell the program to copy your edit over to the RHS. 

The idea is, DJ's have headphones. They're playing a set; they line the next thing up and sync it up and pick the part they want to play. If they made everyone listen to them doing that, it would not be so, so charming, so they pipe the work-in-progress track to their headphones, make it just right, then mix it in. 

Below the timer row are the sequencer rows, second row to next-to-last. 

They have have a LHS that you play into headphones, and a RHS that you play on the speaker-- allowing you, too, to be so charming. 

The sequencer rows have LHS and RHS sequences, set by the first 16 characters of the row. If you move the cursor to the left side of a row, and type a lower-case letter or numeral there, that edits the LHS sequence, and you'll start hearing a note on your LHS-- your left ear-- once per measure.

You can't edit the RHS directly, or even get the cursor to go over there.

What you hear depends on the instrument. That's a fancy topic, below.

To copy LHS to RHS, set the TIMER character, which is the 16th character-- the leftmost red one. Try editing a row's sequence, and then setting the timer to '1'. At the start of the next measure, the green bar pops left, the '1' changes to a '0', and pop!-- your changes get copied over. 

At which point, what's playing on the RHS changes. Charming! 

Moving the cursor! You can use arrow keys, but we might be in a hurry and not wanting to flap our hands all around, so ctrl-wasd also moves them, and also the number pad. Home goes to column 0, end to column 16, pg-up to the top and pg-dn to the bottom. 

Ctrl-g adds 1 to the current row's timer, without having to move the cursor to column 16. Contol-g means Go! 

The very last column on each side is the MIX control. 

Oo, controls! Lots of controls. Each row has 8 on LHS and RHS. They're the rightmost numberletters on each side. 

Almost all the numberletters in the program use ORCA numbers. If you know about hexadecimal numbers, they're like that, but more so. That is, 0 is 0, 1 is 1, 2 is 2, .. 9 is 9, and then, like hexadecimal, a is 10, b is 11, .. f is 15, BUT THEN g is 16, and then we just keep going till we get to z being 35. They're called "Orcas" in worshipful homage to Orca (https://100r.co/site/orca.html).

Some of the Orcas are used as integers: the seqencer, usually. But they can also be used as fractions: 1 is 1/35, or 0.028571429. There are scaling issues sometimes. For instance, most instruments have an ADSR envelope, and for them the A is in seconds (a fraction), the D is in milliseconds (how long to drop to the sustain level), the S is a fraction, in 35ths, and the R is release in seconds, a fraction, but then I multiply it by 2 or something. 

So! Controls! Each row has 8, on the right of the LHS and RHS. What they do depends on the instrument. There are 4 special controls in red, in the middle. 

As disclosed above, the leftmost red one, right next to the sequence, right in the middle, is the timer (yes, you can set it to 'q'-- wow, long-term planning!). The one next to it is the Lerp timer.

You know Daft Punk does all those really long, slow filter changes? To "lerp" something is the "Linearly Interpolate" it, that is, to change it as a constant rate over some period of time. The sequence isn't lerped ('twould be silly) but the controls are. 

So, to fade a sequence out of your mix, set its rightmost control to '0', set the lerptimer to, say, '2', and then the timer to '1': starting at the next measure, the volume of that row will fade out over two measures. 

You can lerp all the controls-- the ADSR, the filters, the volume. On multiple rows at once. Sequencer playback speeds. You could have 80 lerps going at once. 

To summarize: each sequencer row has three regions: the first 16 characters are the sequencer. The last 8 are the controls, and the rightmost is the mix/volume of the row. Character 16 is the timer; 17 is the Lerp timer. 

Column 18 is the Instrument selector! It doesn't lerp, but you do have to copy it over. 

Instrument 0 is the null instrument. Not very useful!

1 is the drumsampler. Sequence letters select which file to play. The files are in the 'data' directory. There are 10, so put 0-9 in the sequencer part of the rows. I need more than 10: someday. The filenames are hardcoded, which sucks but I'll get to it. 

2 is a sine oscillator with an ADSR envelope and a volume control (col 27). ADSRs are set by first 4 controls. Sequencer Orcas control the pitch. The sine instrument is TUNED (see below). The sine inst wants "note off" commands; put '.''s in the seq. If you don't, it just plays forever.

Wow, I just realized that you might not know what an "ADSR envelope" is. Or what an "oscillator" being a "sine" would signify. This *is* esoteric. 

3 is a square osc; same ADSR. Col 24 (control 5) is the duty cycle, col 25 is a high-pass filter z-axis setting. Also TUNED. Also wants noteoffs.

Note that the sine doesn't have a filter; filters don't affect sine waves-- no harmonics!

4 is sawtooth: no duty cycle, otherwise same.

5 is noise. HPF freq is controlled in the seq, ADSR same. volume same. But I hear you saying: but what about low-pass? Shush. 

Instruments 6-e are more-or-less directly included Instrmnt's (that's the parent class name) from "The Synthesis ToolKit in C++ (STK)" by Perry R. Cook & Gary P. Scavone-- which is not the usual STK that millions of C++ folks use daily, so good luck Googling it. https://ccrma.stanford.edu/software/stk/index.html

Instruments 6-9 are the presets for the "BandedWG" Instrmnt. They're kinda quiet, so up the volume. 
Uniform Bar, Tuned Bar, Glass Harmonica, and Tibetan Bowl. Kinda drone-y, kinda chime-y. BandedWG.h specifies 5 control messages, and the leftmost controls give you access to those settings. 

Instruments a-e are FM-synthesis Instrmnts: BeeThree, a Hammond Organ; FMVoices, which kinda can sometimes sound like a human;  Rhodey, a piano-ish thing that Herbie Hancock might like; TubeBell, which is chimey and kinda a standard FM synth voice; and Wurley, another Hancocky piano. 

Instrument Z is also the default, so if you go past 'e', you always get a STK "Simple" Instrmnt, which has a sine generator connected to a noise generator and some reverb and feedback. It *seems* like it would be good for synthetic percussion, but I haven't been pleased by my results yet. 

Other instruments are WIP. Something fm, something granular, something theremin-ish, something synth-drummy, something with different filters. A filter bank would be groovy, maybe something with 4 rows and then a filter bank maybe. Existing instruments are 20-150 lines of code, so watch this space. 

Top priority is just a straightforward sampler. lhsrhs is for making techno, right? So having a track that's beat plus a kewl/sick sound sample that you play every 1-4 measures for the whole track like it's ironic or a comment (looking at you, Orbital)-- gotta support that. 

"But wait!" mutters the alert reader. "Every 4 measures? How am I going to do that here?" 

Which brings us to column 19, the rightmost red column: the speedup factor. The timer tick duration is the same for all rows. A timer tick is 1/4 of a "beat" as set in the "BPM" thing in the top row. 

120 BPM conventionally means two beats a second, one measure per 2 seconds, 4/4. To defy this convention is folly! But also, we have 16 sequencer steps per measure, by t'other ancient convention. If each step was a beat, 120 would give one measure every 8 seconds-- which would be STUPID. 

So, "beats" are the unit used to set tempo, and the sequencers use 16 steps per measure, and the "steps per second" rate is 4x the BPM. "Speedup" sets "ticks per step". The default is 8 ticks per step, 32 ticks per beat, 128 ticks per measure. 

"Speedup" defaults to 8, which is "no speedup". If you set it to 4, the row runs twice as fast. If you set it to 16 ('g'), half as fast, 32 ('w') one-quarter speed, and now you could play something every 4 measures. 

Non-powers-of-2 speedups will either make a mess or showcase the vision of your chaotic genius. Probably the former! 

Resynchronizing rows after you monkey with their steps will require finesse. I ought to make ctrl-u ("Unite!") make all the rows pop back to step 0. Someday.

I intend for the second-from-bottom row to be "active": whatever changes you make copy over immediately-- no timer. Risky!

The bottom row is the TUNER row. It is active! It has a sequence. It does not use timers or lerps. TUNED instruments use the TUNER row, so let's talk about that. 

Columns 16 and 17 of the tuner row are the decimal digits of the MIDI semitone number of the TONIC of your tuning. They're not Orcas, so putting 'j' in one will make it weird. Middle-C is 60. A-440 is 69. MIDI gives you 12 semitones per octave, so other C's are at 48, 36, 24, 72, etc. 

The tonic of a scale is just its first note. The controls are used to set the other semitones in the scale. This allows 8, so serialism is unavailable, but I'm okay with not enabling serialism. For lhsrhs, the tonic is also the lowest note that can be played, so you probably should pick a number between 12 and 48 or so. 

For instance, to use a c-major key, set the tonic to 24, and the other controls to "24579a00". The '2' is two semitones above the tonic: D, and is the second note of the scale, the first being the octave, which everyone uses so it's always included and you don't need to specify it. Controls set to 0 are ignored. Duplicates are allowed but would be weird. Scales that are different for ascending vs descending sequences are not supported.

"Wait!" mutters the goddamn alert reader, "how do you do accidentals?" Sigh. Can't. 

So, rows that use the tuner provide a "degree" and use the tuner to convert the degree to a frequency for the oscillator or filter. To get a sine instrument to play a scale, put "0.1.2.3.4.5.6.7." in its sequence. Chords are easy: 0,2, and 4 is the major triad. 

"Wait!" says the alert reader, "you could just include whatever accidental you want in the scale, and then adjust how you form chords, right?" Sure-- do that.

You can't use one row to play two notes at once-- use two rows. There's only one timer row, so everything is chronologically synchronized, and there's only one tuner row, so everything is tuned the same. 

A lot of scales are not the major scale: 2579 is like only playing the black keys-- "Pentatonic". This is late-semester Music Theory 101 content; look up "alternate tunings" or "non-western musical scales". Suffice it to say, yes Dorian okay. Not the columns. 

The sequencer part of the tuner row sets a sequence of tonic modifiers, which get added to every note of the scale-- lets you do key progressions.  The tuner row's sequencer runs very slowly. I always felt like key changes are overused and kinda tedious, mostly about musicians showing off how much they've practiced their scales? But I haven't thought of anything better to do with the tonic sequencer yet.

Shortcuts!
ctrl-g adds one to the timer, which causes LHS to get copied to RHS for that row, at the beginning of the next measure. ctrl-g means "go!".

ctrl-x cuts. If the cursor is in columns 0-15, this copies the contents of the sequencer to the clipboard. If the cursor is in the controls, they get copied. There are two clipboards, one for the sequencers and one for controls. If you cut the sequencer, it gets blanked out. If you cut the controls, they get reset to instrument defaults. Cutting in columns 16,17,18,or 19 does nothing. 

ctrl-v pastes the contents of the sequencer or control clipboards, depending on which column you're in.

ctrl-c copies.

ctrl-m sets control 7, the rightmost one, to 0. it's always the row's volume. ctrl-m means "mute". It doesn't set the timer, so you'll have to use strl-g, too to make something happen. 

'[' and ']' increment and decrement the volume.

'-' and '=' increment and decrement the LERP timer. 

ctrl-q makes the program quit/exit/stop/end. 

ctrl-wasd do the video-game standard up,left,down, and right.


To sum up: 

The top row is the timer row, which sets overall tempo.
The bottom row is the tuner row, which sets the tuning and lowest note. 

All the other rows are sequencer rows, which have an instrument and a sequence and controls, and a left-hand-side and a right-.

The LHS's are editable. The timer copies left to right. 
ctrl-q quits, ctrl-g means 'go'.

Now form a band.


To Do: 
The LHS previewing is not yet implemented. 
The sampler file set being hardcoded is crappy-- scan the directory!
More filters
Many more instruments 
Instruments can give helpful text strings so I don't have to memorize everything; I need to update those. 
There are LERP bugs
The tuner's sequencing is off by one. 
Controls that are not LERPing should be cheaper
Sound synthesis should be on its own thread. STK streams might already be doing that.
STK's sound-file format requirements demand circumvention. Load .wav files, damn you. 


