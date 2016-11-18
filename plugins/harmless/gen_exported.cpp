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
	Phasor m_phasor_10;
	t_sample m_shape_6;
	t_sample m_rate_5;
	t_sample m_tone_7;
	t_sample m_depth_9;
	t_sample m_phase_8;
	t_sample samples_to_seconds;
	t_sample m_smth_4;
	t_sample m_y_2;
	t_sample m_smth_3;
	t_sample samplerate;
	t_sample m_y_1;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(t_sample __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_y_1 = 0;
		m_y_2 = 0;
		m_smth_3 = 0;
		m_smth_4 = 0;
		m_rate_5 = 4;
		m_shape_6 = 0.5;
		m_tone_7 = 6000;
		m_phase_8 = 0;
		m_depth_9 = 100;
		samples_to_seconds = (1 / samplerate);
		m_phasor_10.reset(0);
		genlib_reset_complete(this);
		
	};
	// the signal processing routine;
	inline int perform(t_sample ** __ins, t_sample ** __outs, int __n) { 
		vectorsize = __n;
		const t_sample * __in1 = __ins[0];
		const t_sample * __in2 = __ins[1];
		t_sample * __out1 = __outs[0];
		t_sample * __out2 = __outs[1];
		if (__exception) { 
			return __exception;
			
		} else if (( (__in1 == 0) || (__in2 == 0) || (__out1 == 0) || (__out2 == 0) )) { 
			__exception = GENLIB_ERR_NULL_BUFFER;
			return __exception;
			
		};
		t_sample mul_2241 = (m_depth_9 * 0.01);
		t_sample expr_2230 = (0.0027777777777778 * m_phase_8);
		t_sample wrap_2229 = wrap(expr_2230, 0, 1);
		t_sample expr_2249 = safediv(((m_tone_7 * 2) * 3.1415926535898), 48000);
		t_sample sin_2250 = sin(expr_2249);
		t_sample clamp_2251 = ((sin_2250 <= 1e-05) ? 1e-05 : ((sin_2250 >= 0.99999) ? 0.99999 : sin_2250));
		samples_to_seconds = (1 / samplerate);
		// the main sample loop;
		while ((__n--)) { 
			const t_sample in1 = (*(__in1++));
			const t_sample in2 = (*(__in2++));
			t_sample mix_2268 = (m_shape_6 + (0.999 * (m_smth_4 - m_shape_6)));
			t_sample mix_2227 = mix_2268;
			t_sample mix_2269 = (wrap_2229 + (0.999 * (m_smth_3 - wrap_2229)));
			t_sample mix_2228 = mix_2269;
			t_sample mix_2270 = (m_y_2 + (clamp_2251 * (in1 - m_y_2)));
			t_sample mix_2248 = mix_2270;
			t_sample sub_2247 = (in1 - mix_2248);
			t_sample mix_2271 = (m_y_1 + (clamp_2251 * (in2 - m_y_1)));
			t_sample mix_2240 = mix_2271;
			t_sample sub_2239 = (in2 - mix_2240);
			t_sample phasor_2232 = m_phasor_10(m_rate_5, samples_to_seconds);
			t_sample triangle_2231 = triangle((mix_2228 + phasor_2232), mix_2227);
			t_sample mul_2238 = (mix_2240 * triangle_2231);
			t_sample rsub_2235 = (1 - triangle_2231);
			t_sample mul_2237 = (sub_2239 * rsub_2235);
			t_sample add_2236 = (mul_2238 + mul_2237);
			t_sample mix_2272 = (in2 + (mul_2241 * (add_2236 - in2)));
			t_sample out2 = mix_2272;
			t_sample triangle_2233 = triangle(phasor_2232, mix_2227);
			t_sample mul_2246 = (mix_2248 * triangle_2233);
			t_sample rsub_2243 = (1 - triangle_2233);
			t_sample mul_2245 = (sub_2247 * rsub_2243);
			t_sample add_2244 = (mul_2246 + mul_2245);
			t_sample mix_2273 = (in1 + (mul_2241 * (add_2244 - in1)));
			t_sample out1 = mix_2273;
			t_sample smth1_next_2252 = mix_2227;
			t_sample smth2_next_2253 = mix_2228;
			t_sample y0_next_2254 = mix_2248;
			t_sample y1_next_2255 = mix_2240;
			m_smth_4 = smth1_next_2252;
			m_smth_3 = smth2_next_2253;
			m_y_2 = y0_next_2254;
			m_y_1 = y1_next_2255;
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_rate(t_sample _value) {
		m_rate_5 = (_value < 0.1 ? 0.1 : (_value > 20 ? 20 : _value));
	};
	inline void set_shape(t_sample _value) {
		m_shape_6 = (_value < 0.01 ? 0.01 : (_value > 0.99 ? 0.99 : _value));
	};
	inline void set_tone(t_sample _value) {
		m_tone_7 = (_value < 500 ? 500 : (_value > 6000 ? 6000 : _value));
	};
	inline void set_phase(t_sample _value) {
		m_phase_8 = (_value < -180 ? -180 : (_value > 180 ? 180 : _value));
	};
	inline void set_depth(t_sample _value) {
		m_depth_9 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	
} State;


/// 
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs 

const int gen_kernel_numins = 2;
const int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 5; }

/// Assistive lables for the signal inputs and outputs 

const char * gen_kernel_innames[] = { "in1", "in2" };
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
		case 0: self->set_rate(value); break;
		case 1: self->set_shape(value); break;
		case 2: self->set_tone(value); break;
		case 3: self->set_phase(value); break;
		case 4: self->set_depth(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_rate_5; break;
		case 1: *value = self->m_shape_6; break;
		case 2: *value = self->m_tone_7; break;
		case 3: *value = self->m_phase_8; break;
		case 4: *value = self->m_depth_9; break;
		
		default: break;
	}
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
	// initialize parameter 0 ("m_rate_5")
	pi = self->__commonstate.params + 0;
	pi->name = "rate";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_rate_5;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.1;
	pi->outputmax = 20;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_shape_6")
	pi = self->__commonstate.params + 1;
	pi->name = "shape";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_shape_6;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.01;
	pi->outputmax = 0.99;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_tone_7")
	pi = self->__commonstate.params + 2;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_7;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 500;
	pi->outputmax = 6000;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_phase_8")
	pi = self->__commonstate.params + 3;
	pi->name = "phase";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_phase_8;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = -180;
	pi->outputmax = 180;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_depth_9")
	pi = self->__commonstate.params + 4;
	pi->name = "depth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_depth_9;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
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
