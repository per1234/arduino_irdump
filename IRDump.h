// Copyright (c) 2014-2015 José Carlos Nieto, https://menteslibres.net/xiam
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// The IRDump library provides utility functions for sending and receiving
// infrared pulses, using this library you can sniff some signals sent by
// infrared remote controllers and craft your own signals too!

#ifndef MAKERWORKSHOP_IRDUMP
#define MAKERWORKSHOP_IRDUMP

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// System's clock.
#ifdef F_CPU
  #define SYSCLOCK F_CPU
#else
  #define SYSCLOCK 16000000
#endif

#define TIMER_ENABLE_PWM     (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_PWM    (TCCR2A &= ~(_BV(COM2B1)))

#define TIMER_DISABLE_INTR   (TIMSK2 = 0)

#define TIMER_CONFIG_KHZ(val) ({ \
  const uint8_t pwmval = SYSCLOCK / 2000 / (val); \
  TCCR2A = _BV(WGM20); \
  TCCR2B = _BV(WGM22) | _BV(CS20); \
  OCR2A = pwmval; \
  OCR2B = pwmval / 3; \
})

class IRDump {
private:
public:
  IRDump();

  // Match returns true if the given signal `input` matches `signal`. A
  // `threshold` defines the maximum error tolerance.
  bool Match(unsigned int *signal, unsigned int *input, int threshold);

  // Capture reads data from the IR sensor given by `irSensorPin` and writes it
  // to the `signal` array. The user must define the maximum number of pulses
  // this signal will have (`maxPulses`) and the maximun lenght of each pulse
  // (`pulseMaxLength`) in milliseconds.
  bool Capture(int irSensorPin, unsigned int *signal, int maxPulses, int pulseMaxLength);

  // Emit sends a signal using the IR LED given by `irLedPin` at a custom
  // frequency (`kHz`).
  void Emit(int irLedPin, unsigned int *signal, int kHz);
};

#endif
