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
	DCBlock __m_dcblock_15;
	DCBlock __m_dcblock_13;
	Delay m_delay_7;
	Delay m_delay_5;
	Delay m_delay_6;
	int __exception;
	int vectorsize;
	t_sample m_time_10;
	t_sample m_mix_11;
	t_sample m_bleed_12;
	t_sample m_repeats_9;
	t_sample m_tone_8;
	t_sample samplerate;
	t_sample m_y_2;
	t_sample m_smth_4;
	t_sample m_y_1;
	t_sample m_smth_3;
	// re-initialize all member variables;
	inline void reset(t_param __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_y_1 = 0;
		m_y_2 = 0;
		m_smth_3 = 0;
		m_smth_4 = 0;
		m_delay_5.reset("m_delay_5", samplerate);
		m_delay_6.reset("m_delay_6", samplerate);
		m_delay_7.reset("m_delay_7", samplerate);
		m_tone_8 = 3000;
		m_repeats_9 = 75;
		m_time_10 = 500;
		m_mix_11 = 75;
		m_bleed_12 = 0;
		__m_dcblock_13.reset();
		__m_dcblock_15.reset();
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) { 
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		t_sample * __out1 = __outs[0];
		t_sample * __out2 = __outs[1];
		if (__exception) { 
			return __exception;
			
		} else if (( (__in1 == 0) || (__out1 == 0) || (__out2 == 0) )) { 
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		t_sample expr_30 = (((m_tone_8 * 2) * 3.1415926535898) * 2.0833333333333e-05);
		t_sample mul_17 = (m_mix_11 * 0.01);
		t_sample mstosamps_29 = (m_time_10 * (samplerate * 0.001));
		t_sample mul_7 = (mstosamps_29 * 2);
		t_sample mul_1 = (m_bleed_12 * 0.01);
		t_sample sin_24 = sin(expr_30);
		t_sample clamp_25 = ((sin_24 <= 1e-05) ? 1e-05 : ((sin_24 >= 0.99999) ? 0.99999 : sin_24));
		t_sample mul_11 = (m_repeats_9 * 0.01);
		int min_14 = (-1);
		t_sample mul_21 = (m_repeats_9 * 0.01);
		int min_16 = (-1);
		// the main sample loop;
		while ((__n--)) { 
			const t_sample in1 = (*(__in1++));
			t_sample mix_53 = (mstosamps_29 + (0.999 * (m_smth_4 - mstosamps_29)));
			t_sample mix_4 = mix_53;
			t_sample tap_6 = m_delay_7.read_linear(mix_4);
			t_sample mix_54 = (mul_7 + (0.999 * (m_smth_3 - mul_7)));
			t_sample mix_18 = mix_54;
			t_sample tap_15 = m_delay_6.read_linear(mix_18);
			t_sample tap_28 = m_delay_5.read_linear(mix_18);
			t_sample add_56 = (tap_15 + tap_6);
			t_sample mix_55 = (m_y_2 + (clamp_25 * (add_56 - m_y_2)));
			t_sample mix_12 = mix_55;
			t_sample mix_57 = (m_y_1 + (clamp_25 * (tap_28 - m_y_1)));
			t_sample mix_22 = mix_57;
			t_sample mix_58 = (mix_12 + (mul_1 * (mix_22 - mix_12)));
			t_sample mul_8 = (mix_58 * mul_17);
			t_sample out2 = (mul_8 + in1);
			t_sample mix_59 = (mix_22 + (mul_1 * (mix_12 - mix_22)));
			t_sample mul_16 = (mix_59 * mul_17);
			t_sample out1 = (mul_16 + in1);
			t_sample mul_13 = (mix_12 * mul_11);
			t_sample dcblock_9 = __m_dcblock_13(mul_13);
			t_sample clamp_10 = ((dcblock_9 <= min_14) ? min_14 : ((dcblock_9 >= 1) ? 1 : dcblock_9));
			t_sample mul_26 = (mix_22 * mul_21);
			t_sample dcblock_19 = __m_dcblock_15(mul_26);
			t_sample clamp_20 = ((dcblock_19 <= min_16) ? min_16 : ((dcblock_19 >= 1) ? 1 : dcblock_19));
			t_sample smth2_next_31 = fixdenorm(mix_4);
			t_sample smth1_next_32 = fixdenorm(mix_18);
			t_sample y1_next_33 = fixdenorm(mix_12);
			t_sample y0_next_34 = fixdenorm(mix_22);
			m_delay_7.write(in1);
			m_delay_6.write(clamp_10);
			m_delay_5.write((clamp_20 + in1));
			m_smth_4 = smth2_next_31;
			m_smth_3 = smth1_next_32;
			m_y_2 = y1_next_33;
			m_y_1 = y0_next_34;
			m_delay_5.step();
			m_delay_6.step();
			m_delay_7.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_tone(t_param _value) {
		m_tone_8 = (_value < 500 ? 500 : (_value > 6000 ? 6000 : _value));
	};
	inline void set_repeats(t_param _value) {
		m_repeats_9 = (_value < 0 ? 0 : (_value > 110 ? 110 : _value));
	};
	inline void set_time(t_param _value) {
		m_time_10 = (_value < 20 ? 20 : (_value > 1000 ? 1000 : _value));
	};
	inline void set_mix(t_param _value) {
		m_mix_11 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_bleed(t_param _value) {
		m_bleed_12 = (_value < 0 ? 0 : (_value > 50 ? 50 : _value));
	};
	
} State;


/// 
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs 

int gen_kernel_numins = 1;
int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 5; }

/// Assistive lables for the signal inputs and outputs 

const char * gen_kernel_innames[] = { "in1" };
const char * gen_kernel_outnames[] = { "out1", "out2" };

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
		case 0: self->set_bleed(value); break;
		case 1: self->set_mix(value); break;
		case 2: self->set_repeats(value); break;
		case 3: self->set_time(value); break;
		case 4: self->set_tone(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_bleed_12; break;
		case 1: *value = self->m_mix_11; break;
		case 2: *value = self->m_repeats_9; break;
		case 3: *value = self->m_time_10; break;
		case 4: *value = self->m_tone_8; break;
		
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
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(5 * sizeof(ParamInfo));
	self->__commonstate.numparams = 5;
	// initialize parameter 0 ("m_bleed_12")
	pi = self->__commonstate.params + 0;
	pi->name = "bleed";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_bleed_12;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 50;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_mix_11")
	pi = self->__commonstate.params + 1;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_11;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_repeats_9")
	pi = self->__commonstate.params + 2;
	pi->name = "repeats";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_repeats_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 110;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_time_10")
	pi = self->__commonstate.params + 3;
	pi->name = "time";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_time_10;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 20;
	pi->outputmax = 1000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_tone_8")
	pi = self->__commonstate.params + 4;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_8;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 500;
	pi->outputmax = 6000;
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