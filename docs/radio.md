# Martlet 2 Radio Protocol

Document version: 1.0

This document describes the physical and data link level radio protocol for 
Martlet 2. See the telemetry documentation for description of the application 
layer data packets.

## Physical Layer

Martlet 2 is equipped with a FSK radio module whose carrier frequency is 
controlled by the input voltage. For immunity to small drifts caused by 
miscalibration and by Doppler offset during flight, data is modulated as FSK 
over FM audio. A receiver tuned to narrowband FM will output audible tones 
which carry the FSK data.

### Telemetry Downlink Mode

While downlinking telemetry packets, the following settings apply:

                                |
------------------------------------------------
Nominal Carrier Frequency       | 434.650MHz
Modulation                      | AFSK over FM
Peak FM Deviation               | 3kHz TBC
'0' tone                        | 500Hz
'1' tone                        | 1500Hz
Bit rate                        | 2kbps


### Identification Mode

In compliance with FCC 97.119 b 3, Martlet 2 will also identify itself and its 
data mode by a CW transmission, repeated 3 times. The transmitted message is:

    AD6AM/AM MARTLET 2 CUSF.CO.UK/M2R

                                |
------------------------------------------------
Nominal Carrier Frequency       | 434.650MHz
Modulation                      | Morse OOK
Speed                           | 20wpm


## Data Link Layer

16 byte application packets are first randomised by combining with an LFSR of 
polynomial x^8 + x^6 + x^4 + x^3 + x^2 + x^1 + 1. This ensures sufficient 
transitions in the transmitted data stream and aids in balancing the 
transmitted spectrum. For further details see CCSDS 231.0-B-2 section 5.

Randomised application packets are then encoded with the systematic (256,128) 
LDPC code specified in CCSDS 231.1-O-1. The resulting codewords are transmitted 
left bit first, which corresponds to the most significant bit in the lowest 
byte of the application payload.

Each codeword is then prefixed with the 16 bit synchronisation pattern 
1110101110010000, transmitted left bit first.

To transmit a frame including synchronisation pattern, the following sequence 
applies.

1. Carrier enabled
2. If necessary, radio enters identification mode and repeats identification 
   message three times. Identification occurs once on startup and at least 
   every ten minutes thereafter.
3. Radio transitions to telemetry downlink mode and transmits a brief 
   unmodulated carrier.
4. Carrier modulated with repeating 1010 preamble for 16 bit periods
5. Data frame transmitted, starting with leading 1 in synchronisation pattern,
   ending with final (rightmost) parity bit of codeword.
6. Up to fifteen further frames transmitted starting with synchronisation 
   pattern, with no additional preamble.
7. Go to (2).
