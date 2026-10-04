const int dacBase = 25;         // Declaring teh pin that feeds the Howland pump
const int dacCollector = 26;    // Feeds collector via 1k resistor (0V to 2.4V)
const int dacreader = 34;       // Senses the actual voltage powering teh howland pump
const int adcCollector = 35;    // Senses V_CE

const float Rc = 1000.0;      // 1k ohm collector resistor
const float Rpump = 20000.0;  // 20k ohm Howland pump resistors

void setup() {
  Serial.begin(115200);
  delay(2000);      // This gives the serial port time to initialize
  
  // This prints the header that octave is already coded to ignore
  Serial.println("Vce_V,Ib_A,Ic_A"); 
}

void loop() {
  // Sweep Base DAC from ~0.13V (10) to ~0.38V (30) in steps of one
  for (int baseDacVal = 10; baseDacVal <= 30; baseDacVal += 1) {
    dacWrite(dacBase, baseDacVal);
    
    // Delay to let the physical Howland op-amp voltage settle
    delay(2); 

    // Oversampling the Base ADC to crush electrical noise
    // Do note that an low pass RC filter would've worked better here
    long baseAdcSum = 0;
    for (int i = 0; i < 50; i++) {
      baseAdcSum += analogRead(dacreader);
    }
    float actualBaseVal = baseAdcSum / 50.0;
    
    // Calculating the true voltage
    float vDacBase = (actualBaseVal / 4095.0) * 3.3;
    float Ib = vDacBase / Rpump;
    
    // Sweep Collector DAC from 0V (0) to ~3.2V (~245)
    for (int collDacVal = 0; collDacVal <= 245; collDacVal += 3) {
      dacWrite(dacCollector, collDacVal);
      
      // A delay line so we can watch the code run in the serial monitor or octave 
      // command window(given you run the file without the ';') and 
      // see the values which helps in easy troubleshooting
      delay(10); 
      
      // Oversampling the Collector ADC 50 times to eliminate digital noise
      long sumVce = 0;
      for (int i = 0; i < 50; i++) {
        sumVce += analogRead(adcCollector);
        delay(10);
      }
      float avgAdcVce = sumVce / 50.0;
      
      // Converting the averaged ADC reading to Voltage
      float Vce = (avgAdcVce / 4095.0) * 3.3;
      
      // Calculating Ic by finding the voltage drop across the 1k collector resistor and using ohm's law
      float vDacCollector = (collDacVal / 255.0) * 3.3;
      float Ic = (vDacCollector - Vce) / Rc;
      
      // Transmit clean CSV data
      Serial.print(Vce, 4);
      Serial.print(",");
      Serial.print(Ib, 7); 
      Serial.print(",");
      Serial.println(Ic, 5);
    }
  }
  

  dacWrite(dacBase, 0);
  dacWrite(dacCollector, 0);
  
  // The exit trigger for the Octave script
  Serial.println("Sweep_Complete");
  
  // Trap the ESP32 here so it only runs the sweep once per reset
  while(true) {
    delay(100);
  }
}