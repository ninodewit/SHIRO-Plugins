#include "gen_exported.h"

namespace gen_exported {


/*******************************************************************************************************************
Copyright (c) 2012 Cycling '74

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************************************************/


// global noise generator
Noise noise;
static const int GENLIB_LOOPCOUNT_BAIL = 100000;


// The State struct contains all the state and procedures for the gendsp kernel
typedef struct State { 
	CommonState __commonstate;
	Delay m_delay_3;
	Delay m_delay_4;
	Phasor __m_phasor_11;
	int __exception;
	int vectorsize;
	t_sample m_mix_9;
	t_sample m_rate_8;
	t_sample m_smth_2;
	t_sample m_width_10;
	t_sample m_fdbck_1;
	t_sample m_duty_7;
	t_sample samples_to_seconds;
	t_sample m_tz_6;
	t_sample samplerate;
	t_sample m_feedback_5;
	// re-initialize all member variables;
	inline void reset(t_param __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_fdbck_1 = 0;
		m_smth_2 = 0;
		m_delay_3.reset("m_delay_3", samplerate);
		m_delay_4.reset("m_delay_4", samplerate);
		m_feedback_5 = 30;
		m_tz_6 = 1;
		m_duty_7 = 0.5;
		m_rate_8 = 0.5;
		m_mix_9 = (-100);
		m_width_10 = 3;
		samples_to_seconds = (1 / samplerate);
		__m_phasor_11.reset(0);
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) { 
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		t_sample * __out1 = __outs[0];
		if (__exception) { 
			return __exception;
			
		} else if (( (__in1 == 0) || (__out1 == 0) )) { 
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		t_sample div_7 = (m_mix_9 * 0.01);
		t_sample div_6 = (m_feedback_5 * 0.01);
		t_sample round_1 = round(m_tz_6);
		t_sample div_8 = (m_width_10 * 0.5);
		t_sample mul_9 = (round_1 * div_8);
		t_sample mstosamps_16 = (mul_9 * (samplerate * 0.001));
		samples_to_seconds = (1 / samplerate);
		// the main sample loop;
		while ((__n--)) { 
			const t_sample in1 = (*(__in1++));
			t_sample mul_10 = (m_fdbck_1 * div_6);
			t_sample add_11 = (in1 + mul_10);
			t_sample tap_18 = m_delay_4.read_linear(mstosamps_16);
			t_sample phasor_12 = __m_phasor_11(m_rate_8, samples_to_seconds);
			t_sample triangle_13 = triangle(phasor_12, m_duty_7);
			t_sample add_22 = (triangle_13 + 1);
			t_sample mul_21 = (add_22 * 0.5);
			t_sample mul_23 = (m_width_10 * mul_21);
			t_sample mstosamps_20 = (mul_23 * (samplerate * 0.001));
			t_sample mix_38 = (mstosamps_20 + (0.999 * (m_smth_2 - mstosamps_20)));
			t_sample mix_19 = mix_38;
			t_sample tap_25 = m_delay_3.read_linear(mix_19);
			t_sample mul_14 = (tap_25 * div_7);
			t_sample add_15 = (tap_18 + mul_14);
			t_sample out1 = add_15;
			t_sample mul_5 = (add_15 * add_15);
			t_sample mul_4 = (mul_5 * 0.28);
			t_sample add_3 = (mul_4 + 1);
			t_sample div_2 = safediv(add_15, add_3);
			t_sample smth_next_34 = fixdenorm(mix_19);
			t_sample fdbck_next_35 = fixdenorm(div_2);
			m_delay_4.write(add_11);
			m_delay_3.write(add_11);
			m_smth_2 = smth_next_34;
			m_fdbck_1 = fdbck_next_35;
			m_delay_3.step();
			m_delay_4.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			
		};
		return __exception;
		
	};
	inline void set_feedback(t_param _value) {
		m_feedback_5 = (_value < 0 ? 0 : (_value > 50 ? 50 : _value));
	};
	inline void set_tz(t_param _value) {
		m_tz_6 = (_value < 0 ? 0 : (_value > 1 ? 1 : _value));
	};
	inline void set_duty(t_param _value) {
		m_duty_7 = (_value < 0.01 ? 0.01 : (_value > 0.99 ? 0.99 : _value));
	};
	inline void set_rate(t_param _value) {
		m_rate_8 = (_value < 0.1 ? 0.1 : (_value > 10 ? 10 : _value));
	};
	inline void set_mix(t_param _value) {
		m_mix_9 = (_value < -100 ? -100 : (_value > 100 ? 100 : _value));
	};
	inline void set_width(t_param _value) {
		m_width_10 = (_value < 1 ? 1 : (_value > 10 ? 10 : _value));
	};
	
} State;


/// 
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs 

int gen_kernel_numins = 1;
int gen_kernel_numouts = 1;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 6; }

/// Assistive lables for the signal inputs and outputs 

const char * gen_kernel_innames[] = { "in1" };
const char * gen_kernel_outnames[] = { "out1" };

/// Invoke the signal process of a State object

int perform(CommonState *cself, t_sample **ins, long numins, t_sample **outs, long numouts, long n) { 
	State * self = (State *)cself;
	return self->perform(ins, outs, n);
}

/// Reset all parameters and stateful operators of a State object

void reset(CommonState *cself) { 
	State * self = (State *)cself;
	self->reset(cself->sr, cself->vs); 
}

/// Set a parameter of a State object 

void setparameter(CommonState *cself, long index, t_param value, void *ref) {
	State * self = (State *)cself;
	switch (index) {
		case 0: self->set_duty(value); break;
		case 1: self->set_feedback(value); break;
		case 2: self->set_mix(value); break;
		case 3: self->set_rate(value); break;
		case 4: self->set_tz(value); break;
		case 5: self->set_width(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_duty_7; break;
		case 1: *value = self->m_feedback_5; break;
		case 2: *value = self->m_mix_9; break;
		case 3: *value = self->m_rate_8; break;
		case 4: *value = self->m_tz_6; break;
		case 5: *value = self->m_width_10; break;
		
		default: break;
	}
}

/// Get the name of a parameter of a State object

const char *getparametername(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].name;
	}
	return 0;
}

/// Get the minimum value of a parameter of a State object

t_param getparametermin(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmin;
	}
	return 0;
}

/// Get the maximum value of a parameter of a State object

t_param getparametermax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].outputmax;
	}
	return 0;
}

/// Get parameter of a State object has a minimum and maximum value

char getparameterhasminmax(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].hasminmax;
	}
	return 0;
}

/// Get the units of a parameter of a State object

const char *getparameterunits(CommonState *cself, long index) {
	if (index >= 0 && index < cself->numparams) {
		return cself->params[index].units;
	}
	return 0;
}

/// Get the size of the state of all parameters of a State object

size_t getstatesize(CommonState *cself) {
	return genlib_getstatesize(cself, &getparameter);
}

/// Get the state of all parameters of a State object

short getstate(CommonState *cself, char *state) {
	return genlib_getstate(cself, state, &getparameter);
}

/// set the state of all parameters of a State object

short setstate(CommonState *cself, const char *state) {
	return genlib_setstate(cself, state, &setparameter);
}

/// Allocate and configure a new State object and it's internal CommonState:

void * create(t_param sr, long vs) {
	State *self = new State;
	self->reset(sr, vs);
	ParamInfo *pi;
	self->__commonstate.inputnames = gen_kernel_innames;
	self->__commonstate.outputnames = gen_kernel_outnames;
	self->__commonstate.numins = gen_kernel_numins;
	self->__commonstate.numouts = gen_kernel_numouts;
	self->__commonstate.sr = sr;
	self->__commonstate.vs = vs;
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(6 * sizeof(ParamInfo));
	self->__commonstate.numparams = 6;
	// initialize parameter 0 ("m_duty_7")
	pi = self->__commonstate.params + 0;
	pi->name = "duty";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_duty_7;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.01;
	pi->outputmax = 0.99;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_feedback_5")
	pi = self->__commonstate.params + 1;
	pi->name = "feedback";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_feedback_5;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 50;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_mix_9")
	pi = self->__commonstate.params + 2;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = -100;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_rate_8")
	pi = self->__commonstate.params + 3;
	pi->name = "rate";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_rate_8;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 10;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_tz_6")
	pi = self->__commonstate.params + 4;
	pi->name = "tz";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tz_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_width_10")
	pi = self->__commonstate.params + 5;
	pi->name = "width";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_width_10;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 1;
	pi->outputmax = 10;
	pi->exp = 0;
	pi->units = "";		// no units defined
	
	return self;
}

/// Release all resources and memory used by a State object:

void destroy(CommonState *cself) { 
	State * self = (State *)cself;
	genlib_sysmem_freeptr(cself->params);
		
	delete self; 
}


} // gen_exported::
