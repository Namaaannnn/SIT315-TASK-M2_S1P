const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(METER_PIN, INPUT);

    Serial.begin(9600);

    double timerFrequency = getPotentiometerFrequency();
    startTimer(timerFrequency);
}

void loop() {
    double timerFrequency = getPotentiometerFrequency();
    startTimer(timerFrequency);
    delay(500);  // Prevent excessive updates
}

// Function to start timer with dynamic frequency
void startTimer(double timerFrequency) {
    noInterrupts();  // Disable interrupts

    TCCR1A = 0;  // Reset Timer1 registers
    TCCR1B = 0;
    TCNT1 = 0;

    // Calculate OCR1A value for desired frequency
    long timerCompare = (16000000 / (1024 * timerFrequency)) - 1;
    OCR1A = (uint16_t)timerCompare;

    TCCR1B |= (1 << WGM12);  // CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler 1024
    TIMSK1 |= (1 << OCIE1A);  // Enable Timer1 Compare Interrupt

    interrupts();  // Enable interrupts
}

// Read potentiometer and map to frequency range (0.1Hz to 5Hz)
double getPotentiometerFrequency() {
    int sensorValue = analogRead(METER_PIN);
    return map(sensorValue, 0, 1023, 1, 5);  // Map to 1Hz - 5Hz
}

// Timer interrupt: Toggles LED
ISR(TIMER1_COMPA_vect) {
    digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
}
