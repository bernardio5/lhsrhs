<img width="845" height="613" alt="Screenshot from 2026-11-12 14-29-18" src="https://github.com/user-attachments/assets/fad4ee3b-5029-415c-b742-c175810ca7d8" />


README for lhsrhs ("Left-Hand Side/Right-Hand Side") ("Lezzerhess")

Reader beware! This is an "Esoteric" interface. It's inscrutable! It might not be for you. You will have to read this to get it to do anything. Control-Q quits! Control-c is the "cut" part of cut and paste, which might frustrate some people. Also, it's *nerdy*: the interface is built in ncurses, which is also used by Vim: esoteric. VT-100 support: check! 

I really made this for myself. I imagine using it in a performance! In the performance, I'd project my desktop, and the audience would see what I'm doing, and it would be dense, fast, colorful, and clearly, controlling the music generation, somehow. I have prioritized feature inclusion, control terseness and interface density (20 sequencers simultaneously editable! 160 controls lerping simultaneously), not ease of use or scrutability. For instance, it uses its own numbering system! For instance, do you know what "lerping" is? Read on!

It's a Linux console program; to run it, you'll probably have to build it, and to do that, you'll need to duplicate this project, install the STK audio toolkit, and copy its "include" directory into the directory that contains this README.
Then, in a console, cd to that directory and use the "make" command. It should build in, like, 2 seconds. You might also have to install the "ncurses" library, but usually, ncurses comes installed with your Linux.  

Then enter "./lhsrhs" and off you go!

Your mouse does nothing! Use the cursor keys. 

The program is a sequencer. When it's running, it should immediately start with a basic drum beat at 120 BPM. If it doesn't, it's broken; blame me and quit. 

The console will show ~20 rows. The top row sets tempo; the bottom row sets tuning, and all the others are identical sequencers that operate independently.

Most rows have a left-hand-side (LHS) and a RHS-- hence the program name. You make changes on the LHS; when you're ready, you get the program to copy your edit over to the RHS. LHS->RHS! "Lezzerhess." (Yes, it is a reference to programming language design/expression evaluation. But kinda backwards; maybe the editable part should be on the right? Me and my English-language predispositions, hrmph.)

The idea is, DJ's have headphones. They're playing a set: on one turntable, there's something playing, and on the other, they put on the next thing, pick the part they want to play and sync up the beats. If they made everyone listen to them doing that, it would not be so, so charming, so they pipe the work-in-progress track to their headphones, make it just right, then mix it in. 

The top row is the TIMER row. It controls BPM, beats/min. You'll see a "120" right there, and also, to the side should be a little blurb that says,  "timer row / BPM:120". 

You can move the cursor to the '120' on the LHS and type in other numbers. You can't edit the RHS directly, or even get the cursor to go over there. 

Editing the LHS has no immediate effect! Say you want "090" there-- 90 beats per minute. First, enter the '0'-- if the tempo responded immediately, you'd be going at 20BPM for a little bit! So, no, it doesn't do that. Enter all your digits, and then scoot left, over to the red '00'. 

The rightmost red '0' is the TIMER. If you put a number in it, that number decreases by one at the beginning of every loop. When the TIMER goes to 0, your edits are copied over to the RHS. If you've edited the BPM, you'll hear the tempo change. Edits are only done at the beginning of measures; it's more musical. 

The other red digit is the LERP. 'LERP' stands for "LinEar intRPolate" (yes, they really just wanted to say "lerp")-- to change something at a constant rate over some period of time. The default lerp time is 0-- instant change. Setting the lerp digit to 2 causes the lerp to go on for 2 measures. 

So, try setting the tempo to 180, the lerp to 8, and then set the timer to 1: the tempo will increase to 180 over 8 measures. The things that are yellow-orange can be lerped; that's why I made them yellow-orange.  

Okay, set BPM, TIMER digit, LERP: that's what the top row, the TIMER row, does.

Oh, but wait! You, the alert reader, might have just muttered to yourself: "only up to 9 measures?" Because the TIMER and LERP are single digits. Well, actually, almost all the characters in the program are ORCA numbers. If you know about hexadecimal numbers, ORCAs are like that, but more so. That is, 0 is 0, 1 is 1, 2 is 2, .. 9 is 9, and then, like hexadecimal, a is 10, b is 11, .. f is 15, BUT THEN g is 16, and then we just keep going till we get to z being 35. They're called "Orcas" in worshipful homage to Orca (https://100r.co/site/orca.html). A mathematician would call it "Triantaexial notataion" or something? "Orca". 

So, you can LERP for up to 35 measures. And the timer can be set to wait for up to 35 measures. But I kept the BPM as decimal digits so I wouldn't have to convert 110 to a two-digit Orca (32) in my head. 

Some of the Orcas are used as integers: the TIMER does. But they can also be used as fractions. 

Below the TIMER row are the SEQencer (or SEQ) rows, second row to next-to-last. 

The first 16 columns of the SEQs are green: they are the sequence that the sequencer is playing. Each green digit is an Orca, usually specifying the pitch to be played, but that depends on the INSTRUMENT the row is playing. What you hear depends on the instrument. That's a fancy topic, discussed below. 

If you move the cursor to the LHS green part of a SEQ, and type a lower-case letter or numeral there, that edits the LHS sequence. You won't hear the change in what's playing until you set the TIMER and it goes to 0, at which point, what's playing on the RHS changes. Charming! 

Ctrl-g adds 1 to the current row's timer, without having to move the cursor to column 16. Contol-g means Go! 

The 8 yellow-orange Orcas on the right side of the LHS are CONTROLs. They can be lerped. What they control depends on the INSTRUMENT, but 
control 7, the rightmost orange character, is the mix control. 

!! Each row controls its own volume. You could blow out your speakers: don't.

To summarize: each sequencer row has three regions: the first 16 characters, the green ones, are the sequencer. The last 8, the yellow-orange ones, are the controls, and the rightmost is the mix/volume of the row. Of the red ones in the middle, the rightmost is the TIMER, the second one is the LERP, and the third is the INSTrument, and the fourth is the PACE.

Okay, let's talk about the INSTRUMENTs! The INST selector doesn't lerp, but you do have to copy it over.

Instrument 0 is the null instrument. Not very useful!

INSTruments 1, 2 and 3 are sampler drumsets a, b, and c. 

There is a data directory. In it, there are many files, some beginning with the letter 'a', then an Orca, then a '_'. Instrument 1 loads the 'a' samples, and the second letter of the filename determines what Orca you put in the sequencer to play that sound. Drumset 'a' is a standard drum kit. Instrument 'b' is conga and timbale samples, and 'c' is tabla samples (courtesy of the "One Laptop Per Child Initiative" (those were so cute (and doomed)!): thanks, y'all!). The loading is automatic; change the files/filenames and you'll hear different sounds the next time you start Lezzerhess. They don't have to be drum sounds, you know. What makes a drum sound drummy?

Notice that the controls for the sampler drums default to 'xxxxxxx5'. The 'x''s mean that that control isn't used. The rightmost control is the volume-- drums are loud! 5 is turning it way down. I'd like to have controls for playback speed, maybe some granular synthesis, IDK, someday, todo!

INST4 is the "synth drum"; it doesn't work. 

INST5 is a sine oscillator with an ADSR envelope and a volume control (col 27). ADSRs are set by first 4 controls. Sequencer Orcas control the pitch. The sine instrument is TUNED (see below). The sine inst wants "note off" commands; put '.''s in the sequence. If you don't, it just plays forever, your call. 

Wow, I just realized that you might not know what an "ADSR envelope" is. Or what an "oscillator" being a "sine" would signify. This *is* esoteric. https://cmtext.com/synthesis/chapter4_waveforms.php

INST6 is a square osc; same ADSR. TBD: control 5 is a "high-pass filter z-axis setting". I want control 6 to be the duty cycle someday. INST6 TUNED. Also wants noteoffs.

Note that the sine doesn't have a filter: filters don't affect sine waves-- no harmonics!

7 is sawtooth: no duty cycle, otherwise same.

8 is noise. HPF freq is controlled in the seq, ADSR same, volume, same. But I hear you saying: but what about low-pass? Shush. 

Instruments a-i are more-or-less directly included "Instrmnt"s (that's the parent class name) from "The Synthesis ToolKit in C++ (STK)" by Perry R. Cook & Gary P. Scavone-- which is not the usual STK that millions of C++ folks use daily, so good luck Googling it. https://ccrma.stanford.edu/software/stk/index.html

Instruments a-d are the presets for the "BandedWG" Instrmnt. They're kinda quiet, so up the volume. Uniform Bar, Tuned Bar, Glass Harmonica, and Tibetan Bowl. Kinda drone-y, kinda chime-y. BandedWG.h specifies 5 control messages, which I've mapped to controls 0-4.

Instruments e-i are FM-synthesis Instrmnts: BeeThree, a Hammond Organ; FMVoices, which kinda can sometimes sound like a human;  Rhodey, a piano-ish thing that Herbie Hancock might like; TubeBell, which is chimey and kinda a standard FM synth voice; and Wurley, another Hancocky piano. 

Instrument Z is also the default, so if you go past 'e', you always get a STK "Simple" Instrmnt, which has a sine generator connected to a noise generator and some reverb and feedback. It has an ADSR envelope, controls 0-3, and a cross-fade between the noise and the tone, control 4. It *seems* like it would be good for synthetic percussion, but I haven't been pleased by my results yet. 

All the instruments have default settings. Not all instruments use all 8 controls; the default settings put 'x''s on those by convention. You can edit the x's, but nothing will happen. Ctrl-X is cut; if you use cut when the cursor is over a row's controls, they'll all get set to defaults. 

ctrl-x, ctrl-c, and ctrl-v are cut, copy, and paste, like my muscle memory says. (Oh, your muscle memory is for Vim? You poor thing.) There are separate copy/paste memories for sequences and controls, so you can't accidentally paste a sequence onto controls. 

Other instruments are WIP. Something fm, something granular, something theremin-ish, the synth-drum, something with different filters. A filter bank would be groovy, maybe something with 4 rows and then a filter bank maybe. Existing instruments are 20-150 lines of code, so watch this space. 

Okay, so: lhsrhs is for making techno, right? So having a track that's beat plus a kewl/sick sound sample that you play every 4 (or 1) measures for the whole track like it's ironic or a comment (looking at you, Orbital)-- gotta support that. 

"But wait!" mutters the alert reader. "Every 4 measures? How am I going to do that here?" 

Which brings us to column 19, the rightmost red column: the PACE. It speeds up its row or slows it down. It is not lerped. It is a multiplier. The default is 8 right now, but you could set everything to 9 or 12, but wait! 

120 BPM conventionally means two beats a second, one measure per 2 seconds, 4/4. To defy this convention is folly! But also, we have 16 sequencer steps per measure, by t'other ancient convention. If each step was a beat, 120 would give one measure every 8 seconds, which would shock and disgust any reasonable person. 

So, "beats" are the unit used to set tempo, and the sequencers use 16 steps per measure, and the "steps per second" rate is 4x the BPM. PACE sets "ticks per step". The default is 8 ticks per step, 32 ticks per beat, 128 ticks per measure. 

So, now I come along and want to run rows at different rates; PACE does this. PACE defaults to 8. If you set it to 16('g'), the row runs twice as fast. If you set it to 4, half as fast, 2 gives one-quarter speed, and now you could play something every 4 measures. 32 is 'w'.

Non-powers-of-2 speedups will either make a mess or showcase the vision of your chaotic genius. Probably the former! 

Resynchronizing rows after you monkey with their steps will require finesse. I ought to make ctrl-u ("Unite!") make all the rows pop back to step 0 someday.

I intend for the second-from-bottom row to be "active": whatever changes you make copy over immediately-- no timer. Risky!

The bottom row is the TUNER row. It is active! It has a sequence. It does not use timers or lerps. TUNED instruments use the TUNER row. 

Columns 16 and 17 of the tuner row are the decimal digits of the MIDI semitone number of the TONIC of your tuning. They're not Orcas, so putting 'j' in one will make it weird. Middle-C is 60. A-440 is 69. MIDI gives you 12 semitones per octave, so other C's are at 48, 36, 24, 72, etc. 

The tonic of a scale is just its first note. The controls are used to set the other semitones in the scale. This allows 8, so serialism is unavailable, but I'm okay with not enabling serialism. For lhsrhs, the tonic is also the lowest note that can be played, so you probably should pick a number between 12 and 48 or so. 

For instance, to use a c-major key, set the tonic to 24, and the other controls to "24579a00". The '2' is two semitones above the tonic: D, and is the second note of the scale, the first being the fundamental, which is the tonic, so you don't need to include it. Controls set to 0 are ignored. Duplicates are allowed but would be weird. Scales that are different for ascending vs descending sequences are not supported.

"Wait!" mutters the GD alert reader, "how do you do accidentals?" Sigh. Can't. 

So, rows that use the tuner provide a "degree" and use the tuner to convert the degree to a frequency for the oscillator or filter. To get a sine instrument to play a scale, put "0.1.2.3.4.5.6.7." in its sequence. Chords are easy: 0,2, and 4 is the major triad. 

"Wait!" says the alert reader, "you could just include whatever accidental you want in the scale, and then adjust how you form chords, right?" Sure-- do that.

You can't use one row to play two notes at once-- use two rows. There's only one timer row, so everything is chronologically synchronized, and there's only one tuner row, so everything is tuned the same. 

A lot of scales are not the major scale: 2579 is like only playing the black keys-- "Pentatonic". This is late-semester Music Theory 101 content; look up "alternate tunings" or "non-western musical scales". Suffice it to say, yes Dorian okay. Not the columns. 

The sequencer part of the tuner row sets a sequence of tonic modifiers, which get added to every note of the scale-- lets you do key progressions.  The tuner row's sequencer runs very slowly (PACE=1). I always felt like key changes are overused and kinda tedious, mostly about musicians showing off how much they've practiced their scales? But I haven't thought of anything better to do with the tonic sequencer yet.

Shortcuts!
ctrl-g adds one to the timer, which causes LHS to get copied to RHS for that row, at the beginning of the next measure. ctrl-g means "go!".

ctrl-x cuts. If the cursor is in columns 0-15, this copies the contents of the sequencer to the clipboard. If the cursor is in the controls, they get copied. There are two clipboards, one for the sequencers and one for controls. If you cut the sequencer, it gets blanked out. If you cut the controls, they get reset to instrument defaults. Cutting in columns 16,17,18,or 19-- the red ones-- does nothing. 

ctrl-v pastes the contents of the sequencer or control clipboards, depending on which column you're in.

ctrl-c copies.

ctrl-m sets control 7, the rightmost one, to 0, which always the row's volume. ctrl-m means "mute". It doesn't set the timer, so you'll have to use strl-g, too, to make something happen. 

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
The LHS previewing is not yet implemented. Bug with decimal lerps. 
Many more instruments: tunable samplers, synth drum, FILTERS. Filterbank rows. 
Something that chains rows for the sake of having multi-measure sequences.
Hotkeys for SEQ edits: reverse, shuffle, randomize, decimate, harmonize.
Instruments can give helpful text strings so I don't have to memorize everything; I need to update those. 
The tuner's sequencing is off by one. 
Controls that are not LERPing should be cheaper
Sound synthesis should be on its own thread. STK streams might already be doing that.


