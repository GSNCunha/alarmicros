The objective of this project is to create a monitored alarm system for a house, built using a 16-bit AVR microprocessor (Arduino), a 16x2 LCD, and a 16-key matrix keypad.

Video of project working: https://www.youtube.com/watch?v=65h0ZLZ5vOo

The alarm system starts in a locked state.
• To activate it, the user must first enter an access code (five digits).
Note: For security reasons, the display shows "*" instead of the password characters.
• If the password does not match, the display will show "INVALID PASSWORD."
• If the password is accepted, the system arms itself and initiates a countdown timer to allow the owner to exit the premises before sensor monitoring begins.

The armed system must read four sensors, simulated by the matrix keypad:
– Window Sensor 1
– Window Sensor 2
– Front Door Sensor
– Garage Door Sensor

When any of the sensors are triggered for more than 200ms (to avoid false alarms), it activates, causing an alert LED to flash (twice per second).

• This flashing LED at a frequency of 2Hz serves as a waiting period to alert the owner before the audible alarm is triggered.
• This provides the owner with time to disarm the alarm by entering the same activation password.

If the alarm is not disarmed within the time limit, an audible alert will be generated using a buzzer with at least two distinct frequencies (e.g., 500Hz and 800Hz).
• A message will be sent to the monitoring center.
• The audible signal will continue until the owner disarms the system (by entering the same activation password).

Once disarmed (owner enters the activation/deactivation password), after an audible alarm condition, the system should display the cause of the alarm trigger (which sensor activated it) along with the timestamp of the event.
• If it's a false alarm or an intrusion, this should be reported to the monitoring center.

The system should also include the feature of automatic nighttime activation, arming itself from 24 hours to 6 AM.

Since it's a monitored system, it must periodically (every 20 seconds) send an "OK" message to indicate that it's operating correctly.

If this message times out three times, a security guard will be dispatched to the location to investigate the cause.

The system should include a password for the monitoring center:
– Central Password: "89134"
• This password is intended for cases where a company security guard needs to visit the location, allowing them to deactivate and reactivate the system and report the reason for the visit.

Additionally, the system should accommodate at least three other passwords:
– User 1 Password: "12369"
– User 2 Password: "32190"
– Administrator Password: "01267"
• The administrator's password is the only one that allows configuration of system parameters.

Possible configurations by the administrator include:
– Enabling/disabling User 1
– Enabling/disabling User 2
– Requesting the system time*
– Enabling/disabling automatic nighttime activation

In this option, the administrator should be able to change the passwords for any user, including their own, except for the central and intruder passwords.
• It should be required to enter the new password twice for confirmation before the change is applied.
• A "Factory Reset Passwords" feature should be included for unforeseen circumstances.
