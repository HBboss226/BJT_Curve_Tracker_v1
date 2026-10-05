# BJT Curve Tracer (Breadboard Prototype v1)

## Overview
![LTspice Schematic](images/LTspice_sim.png)
![Physical Breadboard Setup](images/Physical_setup.jpg)
A low-cost, automated BJT curve tracer built on the ESP32 platform. This system utilizes a Howland Current Pump to step base current ($I_B$) and an ADC feedback loop to capture true hardware voltage, bypassing microcontroller DAC non-linearities. Data is logged serially and plotted via Octave.

## Hardware Architecture
* **Controller:** ESP32 (8-bit DAC, 12-bit ADC)
* **Current Source:** LM358 Op-Amp configured as a Howland Current Pump ($R_{pump} = 20k\Omega$)
* **Device Under Test (DUT):** 2N2222 NPN BJT
* **Collector Load:** $1k\Omega$ sensing resistor

## Known System Constraints & Physical Realities
1. **ESP32 DAC Deadzone:** The internal 8-bit DAC exhibits severe non-linearity near ground. To calculate accurate base currents at the micro-amp level, the Howland pump's driving voltage is actively measured via a dedicated ADC feedback loop rather than relying on idealized mathematical output.
2. **Low-Current Beta Roll-off:** At micro-amp base drives ($I_B < 20\mu A$), the 2N2222 exhibits significant $\beta$ degradation, which is accurately mapped by the ADC feedback.
3. **Load-Line Saturation:** Operating the collector sweep on a 5V/3.3V rail with a $1k\Omega$ current-sensing resistor creates a hard physical $V_{CE}$ saturation wall at roughly 3.3mA. 

## Next Steps (v2 Architecture)
* Migrate firmware from the Arduino wrapper to bare-metal C using ESP-IDF and FreeRTOS.
* Replace the $1k\Omega$ resistor with a buffered LM358 current driver and a $220\Omega$ resistor to increase the $I_C$ ceiling to 15mA.
* Route a custom KiCad PCB to eliminate breadboard parasitic capacitance and EMI.