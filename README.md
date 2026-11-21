# High Power Two Axis Solar Tracking System With MPPT Controller

**Student:** Poonam Autade  
**Degree:** Bachelor of Engineering (BE), Electronics and Telecommunication Engineering  
**Academic year:** 2015–2016  
**Institute:** Gokhale Education Society's R. H. Sapat College of Engineering, Management Studies and Research, Nashik  
**Project guide:** Prof. Y. A. Salame  
**Project support/sponsorship :** Mr. Vipul Bumb, Pradnya Electronics

## Project overview

This project combines:

1. **Mechanical dual-axis solar tracking** — the panel follows the sun in east-west and north-south directions.
2. **Electrical maximum power point tracking (MPPT)** — a DC-DC converter adjusts the PV operating point to extract maximum available power.

The report describes four LDR sensors arranged in two pairs. One pair is used for one tracking axis and the other pair for the second axis. A further LDR is described for night-mode detection. Two 12 V geared stepper motors move the panel, and the report mentions an L293D motor-driver IC.

## MPPT method

The report discusses the Perturb and Observe (P&O) method. The controller slightly changes the PV operating point and observes the resulting power:

- If measured power increases, continue perturbing in the same direction.
- If measured power decreases, reverse the perturbation direction.
- The operating point settles around the maximum power point, with some steady-state oscillation.

A buck-boost DC-DC converter is described as the interface between the PV panel and the battery/load. Its duty cycle is adjusted to change the operating point seen by the PV module.

## Repository files

| File | Purpose |
|---|---|
| `solar_tracker.c` | LDR comparison and two-axis motor-control logic |
| `mppt_po.c` | Perturb and Observe MPPT decision algorithm |
| `mppt_main_template.c` | Example integration outline for ADC measurements and converter duty command |

## Hardware described in the report

- PV solar panel
- Four LDRs for two-axis sensing, plus a night-mode LDR described in the text
- Two 12 V geared stepper motors
- L293D motor driver mentioned in the report
- DC-DC buck-boost converter
- Battery charger and battery
- Controller/motor-driver circuitry

## Microcontroller note

The report references more than one controller family in different sections: 89V51RD2 for the solar-tracking controller, P89LPC938 in the controller feature description, and PIC16LF1503 in a separate controller section. The report does not provide one complete, consistent MCU pin map or the full original firmware. For that reason, the supplied `.c` files are portable control-logic examples and do not assume specific MCU registers or pin assignments. Select the actual controller from the final schematic before adding device-specific code.

## Build and hardware integration

These files are logic-level examples. To build a working embedded project:

1. Select the exact MCU and compiler used by the physical circuit.
2. Implement the board functions declared in the source files (ADC reads, motor outputs, delays and PWM).
3. Confirm the LDR divider/comparator output polarity and calibrate the tracking deadband.
4. Confirm motor direction wiring, step sequence, gearing and safe travel limits.
5. Calibrate PV voltage and current measurements using the actual sensing circuits.
6. Verify whether increasing converter duty cycle raises or lowers PV voltage for the selected buck-boost topology.
7. Add over-current, over-voltage, battery-charge, thermal and motor travel protections before operating the hardware.
8. Test with a current-limited supply and suitable supervision before connecting a solar panel or battery.

## Important limitations

- The project report does not include the complete original firmware.
- It does not provide a complete pin-to-pin schematic or a consistent microcontroller selection across all sections.
- ADC scaling constants, LDR threshold values, motor step angle, PWM frequency and converter protection limits are not fully specified.
- The P&O implementation is a control-algorithm example. Converter duty-cycle polarity and safe limits must be validated on the actual power stage.
- Do not use these files as production firmware without hardware-specific implementation and testing.

## Reported project observations

The report's conclusion compares a static PV panel with a static panel using MPPT and states efficiencies of 75.53% and 97.19%, respectively, a stated difference of 21.66 percentage points. It also notes that measurements were taken in different months/seasons, so this is the report's own comparison and should not be treated as a controlled same-condition test.

## References

1. Poonam Autade, *High Power Two Axis Solar Tracking System With MPPT Controller*, BE Project Seminar Report, Electronics and Telecommunication Engineering, Gokhale Education Society's R. H. Sapat College of Engineering, Management Studies and Research, Nashik, Academic Year 2015–2016.
2. The report's cited technical references on photovoltaic maximum power point tracking, LDR sensing, stepper motors and buck-boost converters.
