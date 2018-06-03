/*
 * external_interrupts.cpp
 *
 *  Created on: 3 cze 2018
 *      Author: rafal
 */
#include "external_interrupts.h"
#include <avr/interrupt.h>


reg8b_ref ExtInterrupt::eimsk = EIMSK;
eimsk_register& ExtInterrupt::eimsk_reg = (eimsk_register&)EIMSK;
eicra_register& ExtInterruptA::eicra_reg = (eicra_register&)EICRA;
reg8b_ref ExtInterruptA::eicra = EICRA;
eicrb_register& ExtInterruptB::eicrb_reg = (eicrb_register&)EICRB;
reg8b_ref ExtInterruptB::eicrb = EICRB;

#ifndef COMMON_H_
void null_function(){
	/*empty function for empty function pointers
	 *
	 */
}
#endif

func_ptr INT0_FUNC_GLOB_PTR = null_function;
func_ptr INT1_FUNC_GLOB_PTR = null_function;
func_ptr INT2_FUNC_GLOB_PTR = null_function;
func_ptr INT3_FUNC_GLOB_PTR = null_function;
func_ptr INT4_FUNC_GLOB_PTR = null_function;
func_ptr INT5_FUNC_GLOB_PTR = null_function;
func_ptr INT6_FUNC_GLOB_PTR = null_function;
func_ptr INT7_FUNC_GLOB_PTR = null_function;

ISR(INT0_vect){
	INT0_FUNC_GLOB_PTR();
}

ISR(INT1_vect){
	INT1_FUNC_GLOB_PTR();
}

ISR(INT2_vect){
	INT2_FUNC_GLOB_PTR();
}

ISR(INT3_vect){
	INT3_FUNC_GLOB_PTR();
}

ISR(INT4_vect){
	INT4_FUNC_GLOB_PTR();
}

ISR(INT5_vect){
	INT5_FUNC_GLOB_PTR();
}

ISR(INT6_vect){
	INT6_FUNC_GLOB_PTR();
}

ISR(INT7_vect){
	INT7_FUNC_GLOB_PTR();
}

ExtInterruptA::ExtInterruptA(interrupt_pin pin, interrupt_sense_control_setup sense_control, func_ptr fptr){
	int_pin = pin;
	eimsk_reg.set_bit(int_pin);
	set_interrupt_sense_control(sense_control);
	//eicra_reg.set_mask(sense_control, pin*2);
	bind_function_to_interrupt(fptr);
}

void ExtInterrupt::bind_function_to_interrupt(func_ptr fptr){
	switch (int_pin) {
		case 0:
			INT0_FUNC_GLOB_PTR = fptr;
			break;
		case 1:
			INT1_FUNC_GLOB_PTR = fptr;
			break;
		case 2:
			INT2_FUNC_GLOB_PTR = fptr;
			break;
		case 3:
			INT3_FUNC_GLOB_PTR = fptr;
			break;
		case 4:
			INT4_FUNC_GLOB_PTR = fptr;
			break;
		case 5:
			INT5_FUNC_GLOB_PTR = fptr;
			break;
		case 6:
			INT6_FUNC_GLOB_PTR = fptr;
			break;
		case 7:
			INT7_FUNC_GLOB_PTR = fptr;
			break;
		default:
			break;
	}
}

void ExtInterruptA::set_interrupt_sense_control(interrupt_sense_control_setup sense_control){
	eicra_reg.set_mask(sense_control, int_pin*2);
}

void ExtInterrupt::disable_interrupt(){
	eimsk_reg.clear_bit(int_pin);
}

ExtInterruptB::ExtInterruptB(interrupt_pin pin, interrupt_sense_control_setup sense_control, func_ptr fptr){
	int_pin = pin + 4;
	eimsk_reg.set_bit(int_pin);
	set_interrupt_sense_control(sense_control);
	bind_function_to_interrupt(fptr);
}

void ExtInterruptB::set_interrupt_sense_control(interrupt_sense_control_setup sense_control){
	eicrb_reg.set_mask(sense_control, (int_pin-4)*2);
}
