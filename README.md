# 🏎️Speed-Trap🏎️
This project uses an Arduino and IR break-beam sensors to calculate the velocity of toy cars racing down a track. It displays the real-time speed on LED matrices, showing the relationship between distance and time.

🧩 How It Works
The system monitors two independent racing lanes. When a car passes the first sensor, it triggers a timer. When it breaks the second beam, the system stops the timer, calculates the velocity based on the fixed distance between sensors, and renders the result to a MAX7219 LED display.



https://github.com/user-attachments/assets/73f6bc99-b4e9-4745-91a6-5b4594b3e304



Key Features
Dual-Lane Tracking: Handles two simultaneous races using non-blocking logic.

Real-time Physics: Speed = Distance / Time calculated on the fly.

Interactive Feedback: Displays speed and lights up LEDs to signal the winner/finish line.

Robust State Machine: Prevents false triggers from sensor noise.

🛠️ Hardware Stack
Microcontroller: Arduino (Uno/Mega)

Sensors: 4x IR Break Beam Sensors (2 per lane)

Display: 2x MAX7219 Dot Matrix Modules

Libraries: LedControl.h

🏎️🏎️

📄 Usage
Connect the sensors to pins 2, 3, 4, and 5.

Connect the MAX7219 displays to the SPI pins defined in the code.

Upload RaceTrackTimer.ino to the Arduino.

Race!
