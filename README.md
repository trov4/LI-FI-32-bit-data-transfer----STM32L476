# LI-FI-32-bit-data-transfer--STM32L476
This is the code to transfer data using LI-FI on the STM32L476
youtube link of it working: https://youtu.be/d1cvxR-UF_k

Overview:
All of the files use boiler plate code that can be found online for the STM32lxx serires.
Our goal was to send a 32 bit number over light. To accomplish this, we modulated our binary string
through frequency. We sent a preamble frequency for one period, then our 0b0 value was moduled at a slower
rate, and our 0b1 value was modulated at a faster rate. Every bit was represented as one period of the frequency.

Our hardware was simple. On the tx end, the LED was in series with a 220 Ohm resistor to prevent the LED from burning out.
The LED was driven by the ODR register in the STM, with a simple wait function used to modulate frequency.
On the Rx end, we used a photoresistor in series with a 8K resistor and tapped in between the two into the STM
using an interrupt to count the distance in time between two positive edge triggers. The photoresistor 
would have a value of ~300 kOhms when there was not a light shined on it, and a value of ~7.8 kOhms when 
a light was shined on it. This difference is what helped to differentiate between on and off.

The speed of our transfer in this code is 1 second. Our idea for getting a faster speed include:
- use a photoresistor that also reacts to non-visible light changes (our photoresistor could only see frequency changes
> ~10 ms)
- modulate every byte. This would take a little more work, but really each frequncy needs it's on 6 ms band for the accuracy we obtained here, so it might save some time only having to send a preamble, then 8 bytes, rather than a preamble and 32 bits.

Authors:
TX: myself
RX: Interrupt: myself
    The rest of Rx: <github account pending>
