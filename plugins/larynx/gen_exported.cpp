#include "gen_exported.h"

namespace gen_exported {

/*******************************************************************************************************************
Cycling '74 License for Max-Generated Code for Export
Copyright (c) 2016 Cycling '74
The code that Max generates automatically and that end users are capable of exporting and using, and any
  associated documentation files (the “Software”) is a work of authorship for which Cycling '74 is the author
  and owner for copyright purposes.  A license is hereby granted, free of charge, to any person obtaining a
  copy of the Software (“Licensee”) to use, copy, modify, merge, publish, and distribute copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The Software is licensed to Licensee only for non-commercial use. Users who wish to make commercial use of the
  Software must contact the copyright owner to determine if a license for commercial use is available, and the
  terms and conditions for same, which may include fees or royalties. For commercial use, please send inquiries
  to licensing (at) cycling74.com.  The determination of whether a use is commercial use or non-commercial use is based
  upon the use, not the user. The Software may be used by individuals, institutions, governments, corporations, or
  other business whether for-profit or non-profit so long as the use itself is not a commercialization of the
  materials or a use that generates or is intended to generate income, revenue, sales or profit.
The above copyright notice and this license shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*******************************************************************************************************************/

// global noise generator
Noise noise;
static const int GENLIB_LOOPCOUNT_BAIL = 100000;


// The State struct contains all the state and procedures for the gendsp kernel
typedef struct State {
	CommonState __commonstate;
	Delay m_delay_3;
	Phasor __m_phasor_12;
	SineCycle __m_cycle_11;
	SineData __sinedata;
	int __exception;
	int vectorsize;
	t_sample m_smth_1;
	t_sample m_shape_10;
	t_sample samplerate;
	t_sample m_depth_9;
	t_sample m_y_2;
	t_sample m_rampdepth_8;
	t_sample samples_to_seconds;
	t_sample m_dry_7;
	t_sample m_ramprate_4;
	t_sample m_rate_5;
	t_sample m_tone_6;
	// re-initialize all member variables;
	inline void reset(t_param __sr, int __vs) {
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_smth_1 = ((int)0);
		m_y_2 = ((int)0);
		m_delay_3.reset("m_delay_3", samplerate);
		m_ramprate_4 = ((t_sample)0.1);
		m_rate_5 = ((int)5);
		m_tone_6 = ((int)6000);
		m_dry_7 = ((int)0);
		m_rampdepth_8 = ((int)0);
		m_depth_9 = ((int)1);
		m_shape_10 = ((t_sample)0.5);
		__m_cycle_11.reset(samplerate, 0);
		samples_to_seconds = (1 / samplerate);
		__m_phasor_12.reset(0);
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
		t_sample expr_2126 = (((m_tone_6 * ((int)2)) * ((t_sample)3.1415926535898)) * ((t_sample)2.0833333333333e-05));
		t_sample sin_2116 = sin(expr_2126);
		t_sample clamp_2117 = ((sin_2116 <= ((t_sample)1e-05)) ? ((t_sample)1e-05) : ((sin_2116 >= ((t_sample)0.99999)) ? ((t_sample)0.99999) : sin_2116));
		t_sample mul_2098 = (m_dry_7 * ((t_sample)0.0025));
		t_sample rsub_2097 = (((int)1) - mul_2098);
		t_sample mul_2100 = (m_dry_7 * ((t_sample)0.01));
		t_sample mul_2095 = (m_rampdepth_8 * ((t_sample)0.01));
		t_sample mul_2105 = (m_rate_5 * mul_2095);
		samples_to_seconds = (1 / samplerate);
		// the main sample loop;
		while ((__n--)) {
			const t_sample in1 = (*(__in1++));
			t_sample mix_2132 = (m_y_2 + (clamp_2117 * (in1 - m_y_2)));
			t_sample mix_2114 = mix_2132;
			t_sample mul_2101 = (mix_2114 * mul_2100);
			__m_cycle_11.freq(m_ramprate_4);
			t_sample cycle_2110 = __m_cycle_11(__sinedata);
			t_sample cycleindex_2111 = __m_cycle_11.phase();
			t_sample mul_2102 = (mul_2095 * cycle_2110);
			t_sample mul_2104 = (mul_2102 * (-1));
			t_sample add_2103 = (m_depth_9 + mul_2104);
			t_sample mul_2106 = (mul_2105 * cycle_2110);
			t_sample add_2107 = (m_rate_5 + mul_2106);
			t_sample phasor_2109 = __m_phasor_12(add_2107, samples_to_seconds);
			t_sample triangle_2108 = triangle(phasor_2109, m_shape_10);
			t_sample mul_2118 = (add_2103 * triangle_2108);
			t_sample mstosamps_2113 = (mul_2118 * (samplerate * 0.001));
			t_sample mix_2133 = (mstosamps_2113 + (((t_sample)0.999) * (m_smth_1 - mstosamps_2113)));
			t_sample mix_2112 = mix_2133;
			t_sample tap_2120 = m_delay_3.read_linear(mix_2112);
			t_sample add_2096 = (tap_2120 + mul_2101);
			t_sample mul_2099 = (add_2096 * rsub_2097);
			t_sample out1 = mul_2099;
			t_sample y0_next_2128 = fixdenorm(mix_2114);
			t_sample smth_next_2129 = fixdenorm(mix_2112);
			m_delay_3.write(mix_2114);
			m_y_2 = y0_next_2128;
			m_smth_1 = smth_next_2129;
			m_delay_3.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			
		};
		return __exception;
		
	};
	inline void set_ramprate(t_param _value) {
		m_ramprate_4 = (_value < 0.1 ? 0.1 : (_value > 1 ? 1 : _value));
	};
	inline void set_rate(t_param _value) {
		m_rate_5 = (_value < 0.1 ? 0.1 : (_value > 10 ? 10 : _value));
	};
	inline void set_tone(t_param _value) {
		m_tone_6 = (_value < 500 ? 500 : (_value > 12000 ? 12000 : _value));
	};
	inline void set_dry(t_param _value) {
		m_dry_7 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_rampdepth(t_param _value) {
		m_rampdepth_8 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_depth(t_param _value) {
		m_depth_9 = (_value < 0.1 ? 0.1 : (_value > 5 ? 5 : _value));
	};
	inline void set_shape(t_param _value) {
		m_shape_10 = (_value < 0.01 ? 0.01 : (_value > 0.99 ? 0.99 : _value));
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
int num_params() { return 7; }

/// Assistive lables for the signal inputs and outputs

const char *gen_kernel_innames[] = { "in1" };
const char *gen_kernel_outnames[] = { "out1" };

/// Invoke the signal process of a State object

int perform(CommonState *cself, t_sample **ins, long numins, t_sample **outs, long numouts, long n) {
	State* self = (State *)cself;
	return self->perform(ins, outs, n);
}

/// Reset all parameters and stateful operators of a State object

void reset(CommonState *cself) {
	State* self = (State *)cself;
	self->reset(cself->sr, cself->vs);
}

/// Set a parameter of a State object

void setparameter(CommonState *cself, long index, t_param value, void *ref) {
	State *self = (State *)cself;
	switch (index) {
		case 0: self->set_depth(value); break;
		case 1: self->set_dry(value); break;
		case 2: self->set_rampdepth(value); break;
		case 3: self->set_ramprate(value); break;
		case 4: self->set_rate(value); break;
		case 5: self->set_shape(value); break;
		case 6: self->set_tone(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_depth_9; break;
		case 1: *value = self->m_dry_7; break;
		case 2: *value = self->m_rampdepth_8; break;
		case 3: *value = self->m_ramprate_4; break;
		case 4: *value = self->m_rate_5; break;
		case 5: *value = self->m_shape_10; break;
		case 6: *value = self->m_tone_6; break;
		
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

void *create(t_param sr, long vs) {
	State *self = new State;
	self->reset(sr, vs);
	ParamInfo *pi;
	self->__commonstate.inputnames = gen_kernel_innames;
	self->__commonstate.outputnames = gen_kernel_outnames;
	self->__commonstate.numins = gen_kernel_numins;
	self->__commonstate.numouts = gen_kernel_numouts;
	self->__commonstate.sr = sr;
	self->__commonstate.vs = vs;
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(7 * sizeof(ParamInfo));
	self->__commonstate.numparams = 7;
	// initialize parameter 0 ("m_depth_9")
	pi = self->__commonstate.params + 0;
	pi->name = "depth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_depth_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 5;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_dry_7")
	pi = self->__commonstate.params + 1;
	pi->name = "dry";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_dry_7;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_rampdepth_8")
	pi = self->__commonstate.params + 2;
	pi->name = "rampdepth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_rampdepth_8;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_ramprate_4")
	pi = self->__commonstate.params + 3;
	pi->name = "ramprate";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ramprate_4;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 1;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_rate_5")
	pi = self->__commonstate.params + 4;
	pi->name = "rate";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_rate_5;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 10;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_shape_10")
	pi = self->__commonstate.params + 5;
	pi->name = "shape";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_shape_10;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.01;
	pi->outputmax = 0.99;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_tone_6")
	pi = self->__commonstate.params + 6;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0;
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 500;
	pi->outputmax = 12000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	
	return self;
}

/// Release all resources and memory used by a State object:

void destroy(CommonState *cself) {
	State *self = (State *)cself;
	genlib_sysmem_freeptr(cself->params);
		
	delete self;
}


} // gen_exported::
