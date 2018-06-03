/*
 * external_interrupts.h
 *
 *  Created on: 3 cze 2018
 *      Author: ravmiecznik@gmail.com
 *
 *      This library supports External interrupts for atmega128.
 *      The External Interrupts are triggered by the INT7:0 pins.
 *      Registers:
 *
 *		control reg A
 *      EICRA (INT3:0)
 *      ISC31 ISC30 ISC21 ISC20 ISC11 ISC10 ISC01 ISC00
 *
 *		control reg B
 *      EICRB (INT7:4)
 *      ISC71 ISC70 ISC61 ISC60 ISC51 ISC50 ISC41 ISC40
 *
 *		External Interrupt Mask Register
 *		EIMSK
 *		INT7 INT6 INT5 INT4 INT3 INT2 INT1 IINT0
 *
 *
 *		Usage:
 *		ExtInterruptB interrupt_pin7(ExtInterruptB::int7, ExtInterruptB::change_triggers_interrupt, optional_function_ptr);
 *		optional_function_ptr: pointer to function which will be executed in given interrup triggered by porivided pin
 *
 *		interrupt_pin7.set_interrupt_sense_control(ExtInterruptB::rising_edge_triggers_interrupt);
 *		interrupt_pin7.bind_function_to_interrupt(new_pointer);
 *		interrupt_pin7.disable_interrupt();
 *
 */

#ifndef EXTERNAL_INTERRUPTS_H_
#define EXTERNAL_INTERRUPTS_H_
#include <avr/io.h>
//common.h is not required, remove it if you like to get rid of errors
#include "common.h"

typedef void (*func_ptr)();
typedef volatile uint8_t& reg8b_ref;

#ifndef COMMON_H_
void null_function();
#endif

class interrupt_sense_control{
	union{
		uint8_t sense_control_bits:2;
		struct{
			bool isc0:1;
			bool isc1:1;
		};
	};
};

struct register_bits{
	uint8_t reg_bits;
	void set_bit(uint8_t bit){
		reg_bits |= _BV(bit);
	}
	void clear_bit(uint8_t bit){
		reg_bits &= ~_BV(bit);
	}
	void set_mask(uint8_t mask, uint8_t pos){
		reg_bits |= (mask << pos);
	}
	void clear_bit(uint8_t mask, uint8_t pos){
		reg_bits &= ~(mask << pos);
	}
};

struct eimsk_register:register_bits
{
	union{
		uint8_t reg;
		struct{
			bool int0:1;
			bool int1:1;
			bool int2:1;
			bool int3:1;
			bool int4:1;
			bool int5:1;
			bool int6:1;
			bool int7:1;
		};
	};
	eimsk_register & operator=(uint8_t val){
		reg = val;
		return *this;
	}
};

struct eicra_register:register_bits
{
	union{
		uint8_t reg;
		struct{
			bool ics00:1;
			bool ics01:1;
			bool ics10:1;
			bool ics12:1;
			bool ics20:1;
			bool ics21:1;
			bool ics30:1;
			bool ics31:1;
		};
		struct{
			uint8_t ics0:2;
			uint8_t ics1:2;
			uint8_t ics2:2;
			uint8_t ics3:2;
		};
	};
	eicra_register & operator=(uint8_t val){
		reg = val;
		return *this;
	}
};

struct eicrb_register:register_bits
{
	union{
		uint8_t reg;
		struct{
			bool ics40:1;
			bool ics41:1;
			bool ics50:1;
			bool ics52:1;
			bool ics60:1;
			bool ics61:1;
			bool ics70:1;
			bool ics71:1;
		};
		struct{
			uint8_t ics4:2;
			uint8_t ics5:2;
			uint8_t ics6:2;
			uint8_t ics7:2;
		};
	};
	eicrb_register & operator=(uint8_t val){
		reg = val;
		return *this;
	}
};

class ExtInterrupt{
protected:
	static reg8b_ref eimsk;
	static eimsk_register& eimsk_reg;
	void disable_interrupt();
	void bind_function_to_interrupt(func_ptr);
	uint8_t int_pin;
};

class ExtInterruptA: ExtInterrupt{
	/*
	 * This class serves int3:0. PORTD: PD3, PD2, PD1, PD0 of atmega128
	 */
private:
	static reg8b_ref eicra;
	static eicra_register& eicra_reg;
public:
	typedef enum{
		int0,
		int1,
		int2,
		int3,
	}interrupt_pin;
	typedef enum{
		low_level_generates_interrupt,
		reserved,
		falling_edge_triggers_interrupt,
		rising_edge_triggers_interrupt,
	}interrupt_sense_control_setup;
	void set_interrupt_sense_control(interrupt_sense_control_setup);
	ExtInterruptA(interrupt_pin pin, interrupt_sense_control_setup sense_control, func_ptr=null_function);
};

class ExtInterruptB: ExtInterrupt{
	/*
	 * This class serves int7:4. PORTE: PE7, PE6, PE5, PE4 of atmega128
	 */
private:
	static reg8b_ref eicrb;
	static eicrb_register& eicrb_reg;
public:
	typedef enum{
		int4,
		int5,
		int6,
		int7,
	}interrupt_pin;
	typedef enum{
		low_level_generates_interrupt,
		change_triggers_interrupt,
		falling_edge_triggers_interrupt,
		rising_edge_triggers_interrupt,
	}interrupt_sense_control_setup;
	void set_interrupt_sense_control(interrupt_sense_control_setup);
	ExtInterruptB(interrupt_pin pin, interrupt_sense_control_setup sense_control, func_ptr=null_function);
};


#endif /* EXTERNAL_INTERRUPTS_H_ */
