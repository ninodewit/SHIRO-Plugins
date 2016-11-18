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
	Delay m_delay_21;
	Delay m_delay_13;
	Delay m_delay_14;
	Delay m_delay_12;
	Delay m_delay_11;
	Delay m_delay_23;
	Delay m_delay_15;
	Delay m_delay_17;
	Delay m_delay_16;
	Delay m_delay_19;
	Delay m_delay_18;
	Delay m_delay_20;
	Delay m_delay_22;
	Delta m_delta_39;
	Delta m_delta_36;
	Delta m_delta_42;
	Delta m_delta_33;
	Phasor m_phasor_32;
	Sah m_sah_37;
	Sah m_sah_35;
	Sah m_sah_38;
	Sah m_sah_41;
	Sah m_sah_40;
	Sah m_sah_34;
	Sah m_sah_44;
	Sah m_sah_43;
	t_sample m_history_5;
	t_sample m_history_6;
	t_sample m_history_7;
	t_sample m_history_10;
	t_sample m_history_4;
	t_sample m_y_2;
	t_sample m_history_3;
	t_sample samplerate;
	t_sample m_history_1;
	t_sample m_history_9;
	t_sample m_history_8;
	t_sample m_tone_31;
	t_sample samples_to_seconds;
	t_sample m_decay_25;
	t_sample m_shimmer_24;
	t_sample m_mix_27;
	t_sample m_damping_26;
	t_sample m_roomsize_29;
	t_sample m_bandwidth_30;
	t_sample m_ratio_28;
	int vectorsize;
	int __exception;
	// re-initialize all member variables;
	inline void reset(t_sample __sr, int __vs) { 
		__exception = 0;
		vectorsize = __vs;
		samplerate = __sr;
		m_history_1 = 0;
		m_y_2 = 0;
		m_history_3 = 0;
		m_history_4 = 0;
		m_history_5 = 0;
		m_history_6 = 0;
		m_history_7 = 0;
		m_history_8 = 0;
		m_history_9 = 0;
		m_history_10 = 0;
		m_delay_11.reset("m_delay_11", 5000);
		m_delay_12.reset("m_delay_12", 7000);
		m_delay_13.reset("m_delay_13", 6000);
		m_delay_14.reset("m_delay_14", 48000);
		m_delay_15.reset("m_delay_15", 15000);
		m_delay_16.reset("m_delay_16", 16000);
		m_delay_17.reset("m_delay_17", 96000);
		m_delay_18.reset("m_delay_18", 10000);
		m_delay_19.reset("m_delay_19", 12000);
		m_delay_20.reset("m_delay_20", 48000);
		m_delay_21.reset("m_delay_21", 48000);
		m_delay_22.reset("m_delay_22", 48000);
		m_delay_23.reset("m_delay_23", 48000);
		m_shimmer_24 = 50;
		m_decay_25 = 50;
		m_damping_26 = 50;
		m_mix_27 = 50;
		m_ratio_28 = 2;
		m_roomsize_29 = 150;
		m_bandwidth_30 = 75;
		m_tone_31 = 1500;
		samples_to_seconds = (1 / samplerate);
		m_phasor_32.reset(0);
		m_delta_33.reset(0);
		m_sah_34.reset(0);
		m_sah_35.reset(0);
		m_delta_36.reset(0);
		m_sah_37.reset(0);
		m_sah_38.reset(0);
		m_delta_39.reset(0);
		m_sah_40.reset(0);
		m_sah_41.reset(0);
		m_delta_42.reset(0);
		m_sah_43.reset(0);
		m_sah_44.reset(0);
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
		t_sample mul_10 = (m_damping_26 * 0.01);
		t_sample mul_11 = (m_mix_27 * 0.01);
		t_sample mul_9 = (m_bandwidth_30 * 0.01);
		t_sample rsub_8 = (1 - mul_9);
		t_sample expr_228 = safediv(((m_tone_31 * 2) * 3.1415926535898), 48000);
		t_sample sin_16 = sin(expr_228);
		t_sample clamp_17 = ((sin_16 <= 1e-05) ? 1e-05 : ((sin_16 >= 0.99999) ? 0.99999 : sin_16));
		t_sample mul_12 = (m_shimmer_24 * 0.01);
		t_sample expr_238 = safediv((m_roomsize_29 * 48000), 340);
		t_sample mul_215 = (expr_238 * 0.63245);
		t_sample mul_216 = (expr_238 * 0.7071);
		t_sample mul_218 = (expr_238 * 1);
		t_sample mul_217 = (expr_238 * 0.81649);
		t_sample expr_227 = safepow(1.0418, m_decay_25);
		t_sample expr_237 = safepow(0.001, safediv(1, (expr_227 * 48000)));
		t_sample expr_229 = (-safepow(expr_237, mul_215));
		t_sample expr_230 = (-safepow(expr_237, mul_216));
		t_sample expr_236 = (-safepow(expr_237, mul_218));
		t_sample expr_231 = (-safepow(expr_237, mul_217));
		t_sample mul_212 = (expr_238 * 0.000527);
		int int_211 = int(mul_212);
		t_sample mul_168 = (int_211 * 419.5);
		t_sample mul_189 = (int_211 * 400.600006);
		t_sample rsub_67 = (1 - m_ratio_28);
		t_sample mul_66 = (rsub_67 * 10);
		samples_to_seconds = (1 / samplerate);
		t_sample add_155 = (expr_238 + 5);
		t_sample expr_232 = safepow(expr_237, add_155);
		t_sample mul_175 = (int_211 * 566.700012);
		t_sample mul_196 = (int_211 * 568.299988);
		t_sample mul_161 = (expr_238 * 0.41);
		t_sample add_158 = (mul_161 + 5);
		t_sample expr_235 = safepow(expr_237, add_158);
		t_sample mul_160 = (expr_238 * 0.3);
		t_sample add_157 = (mul_160 + 5);
		t_sample expr_234 = safepow(expr_237, add_157);
		t_sample mul_159 = (expr_238 * 0.155);
		t_sample add_156 = (mul_159 + 5);
		t_sample expr_233 = safepow(expr_237, add_156);
		t_sample mul_210 = (expr_238 * 0.110732);
		t_sample mul_182 = (int_211 * 144.800003);
		t_sample mul_203 = (int_211 * 162.100006);
		t_sample mul_6 = (m_decay_25 * 2);
		t_sample clamp_1 = ((mul_6 <= 0) ? 0 : ((mul_6 >= 100) ? 100 : mul_6));
		t_sample mul_5 = (clamp_1 * 0.01);
		t_sample atodb_4 = atodb(mul_5);
		t_sample sub_3 = (atodb_4 - 9);
		t_sample dbtoa_2 = dbtoa(sub_3);
		// the main sample loop;
		while ((__n--)) { 
			const t_sample in1 = (*(__in1++));
			t_sample noise_20 = noise();
			t_sample abs_37 = fabs(noise_20);
			t_sample mul_45 = (abs_37 * 0.25);
			t_sample noise_19 = noise();
			t_sample abs_36 = fabs(noise_19);
			t_sample mul_42 = (abs_36 * 0.25);
			t_sample noise_21 = noise();
			t_sample abs_38 = fabs(noise_21);
			t_sample mul_48 = (abs_38 * 0.25);
			t_sample noise_18 = noise();
			t_sample abs_35 = fabs(noise_18);
			t_sample mul_39 = (abs_35 * 0.25);
			t_sample tap_129 = m_delay_23.read_linear(mul_215);
			t_sample tap_135 = m_delay_22.read_linear(mul_216);
			t_sample tap_223 = m_delay_21.read_linear(mul_218);
			t_sample tap_141 = m_delay_20.read_linear(mul_217);
			t_sample mul_125 = (tap_129 * expr_229);
			t_sample mix_249 = (mul_125 + (mul_10 * (m_history_10 - mul_125)));
			t_sample mix_127 = mix_249;
			t_sample mul_131 = (tap_135 * expr_230);
			t_sample mix_250 = (mul_131 + (mul_10 * (m_history_9 - mul_131)));
			t_sample mix_133 = mix_250;
			t_sample mul_214 = (tap_223 * expr_236);
			t_sample mix_251 = (mul_214 + (mul_10 * (m_history_8 - mul_214)));
			t_sample mix_221 = mix_251;
			t_sample mul_137 = (tap_141 * expr_231);
			t_sample mix_252 = (mul_137 + (mul_10 * (m_history_7 - mul_137)));
			t_sample mix_139 = mix_252;
			t_sample tap_167 = m_delay_19.read_linear(mul_168);
			t_sample mul_165 = (tap_167 * 0.625);
			t_sample tap_188 = m_delay_18.read_linear(mul_189);
			t_sample mul_186 = (tap_188 * 0.625);
			t_sample phasor_85 = m_phasor_32(mul_66, samples_to_seconds);
			t_sample add_79 = ((m_history_6 + phasor_85) + 0.25);
			t_sample mod_78 = safemod(add_79, 1);
			t_sample delta_47 = m_delta_33(mod_78);
			t_sample sah_26 = m_sah_34(mul_45, delta_47, 0);
			int sah_46 = m_sah_35(4800, delta_47, 0);
			t_sample mul_33 = (sah_46 * mod_78);
			t_sample sub_77 = (mod_78 - 0.5);
			t_sample mul_76 = (sub_77 * 3.1415926535898);
			t_sample cos_75 = cos(mul_76);
			t_sample mul_52 = (cos_75 * cos_75);
			t_sample add_65 = ((m_history_5 + phasor_85) + 0.5);
			t_sample mod_64 = safemod(add_65, 1);
			t_sample delta_44 = m_delta_36(mod_64);
			t_sample sah_24 = m_sah_37(mul_42, delta_44, 0);
			int sah_43 = m_sah_38(4800, delta_44, 0);
			t_sample mul_32 = (sah_43 * mod_64);
			t_sample sub_63 = (mod_64 - 0.5);
			t_sample mul_62 = (sub_63 * 3.1415926535898);
			t_sample cos_61 = cos(mul_62);
			t_sample mul_51 = (cos_61 * cos_61);
			t_sample add_84 = ((m_history_4 + phasor_85) + 0);
			t_sample mod_83 = safemod(add_84, 1);
			t_sample delta_29 = m_delta_39(mod_83);
			t_sample sah_28 = m_sah_40(mul_48, delta_29, 0);
			int sah_49 = m_sah_41(4800, delta_29, 0);
			t_sample mul_34 = (sah_49 * mod_83);
			t_sample sub_82 = (mod_83 - 0.5);
			t_sample mul_81 = (sub_82 * 3.1415926535898);
			t_sample cos_80 = cos(mul_81);
			t_sample mul_53 = (cos_80 * cos_80);
			t_sample add_59 = ((m_history_3 + phasor_85) + 0.75);
			t_sample mod_58 = safemod(add_59, 1);
			t_sample delta_41 = m_delta_42(mod_58);
			t_sample sah_22 = m_sah_43(mul_39, delta_41, 0);
			int sah_40 = m_sah_44(4800, delta_41, 0);
			t_sample mul_31 = (sah_40 * mod_58);
			t_sample tap_71 = m_delay_17.read_linear(mul_34);
			t_sample tap_72 = m_delay_17.read_linear(mul_33);
			t_sample tap_73 = m_delay_17.read_linear(mul_32);
			t_sample tap_74 = m_delay_17.read_linear(mul_31);
			t_sample mul_60 = (tap_73 * mul_51);
			t_sample mul_68 = (tap_72 * mul_52);
			t_sample mul_69 = (tap_71 * mul_53);
			t_sample sub_57 = (mod_58 - 0.5);
			t_sample mul_56 = (sub_57 * 3.1415926535898);
			t_sample cos_55 = cos(mul_56);
			t_sample mul_50 = (cos_55 * cos_55);
			t_sample mul_54 = (tap_74 * mul_50);
			t_sample add_254 = (((mul_69 + mul_68) + mul_60) + mul_54);
			t_sample mix_253 = (m_y_2 + (clamp_17 * (add_254 - m_y_2)));
			t_sample mix_14 = mix_253;
			t_sample mix_255 = (in1 + (mul_12 * (mix_14 - in1)));
			t_sample mix_86 = mix_255;
			t_sample mul_89 = (mix_86 * 0.707);
			t_sample mix_256 = (mul_89 + (rsub_8 * (m_history_1 - mul_89)));
			t_sample mix_225 = mix_256;
			t_sample add_119 = (mix_221 + mix_139);
			t_sample add_117 = (mix_133 + mix_127);
			t_sample add_112 = (add_119 + add_117);
			t_sample mul_95 = (add_112 * 0.5);
			t_sample sub_116 = (add_119 - add_117);
			t_sample mul_98 = (sub_116 * 0.5);
			t_sample tap_174 = m_delay_16.read_linear(mul_175);
			t_sample tap_195 = m_delay_15.read_linear(mul_196);
			t_sample sub_118 = (mix_221 - mix_139);
			t_sample sub_115 = (mix_133 - mix_127);
			t_sample sub_114 = (sub_118 - sub_115);
			t_sample mul_97 = (sub_114 * 0.5);
			t_sample add_113 = (sub_118 + sub_115);
			t_sample rsub_111 = (0 - add_113);
			t_sample mul_96 = (rsub_111 * 0.5);
			t_sample tap_143 = m_delay_14.read_linear(add_158);
			t_sample tap_144 = m_delay_14.read_linear(add_157);
			t_sample tap_145 = m_delay_14.read_linear(add_156);
			t_sample tap_146 = m_delay_14.read_linear(add_155);
			t_sample mul_147 = (tap_146 * expr_232);
			t_sample add_120 = (mul_95 + mul_147);
			t_sample mul_153 = (tap_143 * expr_235);
			t_sample add_123 = (mul_98 + mul_153);
			t_sample mul_151 = (tap_144 * expr_234);
			t_sample add_122 = (mul_97 + mul_151);
			t_sample mul_149 = (tap_145 * expr_233);
			t_sample add_121 = (mul_96 + mul_149);
			t_sample mul_172 = (tap_174 * 0.625);
			t_sample mul_193 = (tap_195 * 0.625);
			t_sample tap_209 = m_delay_13.read_cubic(mul_210);
			t_sample mul_207 = (tap_209 * 0.75);
			t_sample sub_206 = (mix_225 - mul_207);
			t_sample mul_205 = (sub_206 * 0.75);
			t_sample add_204 = (mul_205 + tap_209);
			t_sample tap_181 = m_delay_12.read_linear(mul_182);
			t_sample tap_202 = m_delay_11.read_linear(mul_203);
			t_sample mul_179 = (tap_181 * 0.75);
			t_sample mul_200 = (tap_202 * 0.75);
			t_sample mul_106 = (mul_153 * mul_5);
			t_sample mul_104 = (mul_149 * mul_5);
			t_sample add_92 = (mul_106 + mul_104);
			t_sample mul_105 = (mul_151 * mul_5);
			t_sample mul_103 = (mul_147 * mul_5);
			t_sample add_91 = (mul_105 + mul_103);
			t_sample sub_101 = (add_92 - add_91);
			t_sample mul_110 = (mul_98 * dbtoa_2);
			t_sample mul_108 = (mul_96 * dbtoa_2);
			t_sample add_94 = (mul_110 + mul_108);
			t_sample mul_109 = (mul_97 * dbtoa_2);
			t_sample mul_107 = (mul_95 * dbtoa_2);
			t_sample add_93 = (mul_109 + mul_107);
			t_sample sub_102 = (add_94 - add_93);
			t_sample add_88 = (sub_102 + sub_101);
			t_sample add_90 = (add_88 + mix_86);
			t_sample sub_178 = (add_90 - mul_179);
			t_sample mul_177 = (sub_178 * 0.75);
			t_sample add_176 = (mul_177 + tap_181);
			t_sample sub_171 = (add_176 - mul_172);
			t_sample mul_170 = (sub_171 * 0.625);
			t_sample add_169 = (mul_170 + tap_174);
			t_sample sub_164 = (add_169 - mul_165);
			t_sample mul_163 = (sub_164 * 0.625);
			t_sample add_162 = (mul_163 + tap_167);
			t_sample mul_87 = (add_162 * mul_11);
			t_sample out2 = (in1 + mul_87);
			t_sample add_100 = (add_88 + mix_86);
			t_sample sub_199 = (add_100 - mul_200);
			t_sample mul_198 = (sub_199 * 0.75);
			t_sample add_197 = (mul_198 + tap_202);
			t_sample sub_192 = (add_197 - mul_193);
			t_sample mul_191 = (sub_192 * 0.625);
			t_sample add_190 = (mul_191 + tap_195);
			t_sample sub_185 = (add_190 - mul_186);
			t_sample mul_184 = (sub_185 * 0.625);
			t_sample add_183 = (mul_184 + tap_188);
			t_sample mul_99 = (add_183 * mul_11);
			t_sample out1 = (in1 + mul_99);
			t_sample history_126_next_239 = mix_127;
			t_sample history_132_next_240 = mix_133;
			t_sample history_220_next_241 = mix_221;
			t_sample history_138_next_242 = mix_139;
			t_sample history_27_next_243 = sah_26;
			t_sample history_25_next_244 = sah_24;
			t_sample history_30_next_245 = sah_28;
			t_sample history_23_next_246 = sah_22;
			t_sample y0_next_247 = mix_14;
			t_sample history_224_next_248 = mix_225;
			m_delay_23.write(add_120);
			m_delay_22.write(add_121);
			m_delay_21.write(add_123);
			m_delay_20.write(add_122);
			m_delay_19.write(sub_164);
			m_delay_18.write(sub_185);
			m_delay_17.write(in1);
			m_delay_16.write(sub_171);
			m_delay_15.write(sub_192);
			m_delay_14.write(add_204);
			m_delay_13.write(sub_206);
			m_delay_12.write(sub_178);
			m_delay_11.write(sub_199);
			m_history_10 = history_126_next_239;
			m_history_9 = history_132_next_240;
			m_history_8 = history_220_next_241;
			m_history_7 = history_138_next_242;
			m_history_6 = history_27_next_243;
			m_history_5 = history_25_next_244;
			m_history_4 = history_30_next_245;
			m_history_3 = history_23_next_246;
			m_y_2 = y0_next_247;
			m_history_1 = history_224_next_248;
			m_delay_11.step();
			m_delay_12.step();
			m_delay_13.step();
			m_delay_14.step();
			m_delay_15.step();
			m_delay_16.step();
			m_delay_17.step();
			m_delay_18.step();
			m_delay_19.step();
			m_delay_20.step();
			m_delay_21.step();
			m_delay_22.step();
			m_delay_23.step();
			// assign results to output buffer;
			(*(__out1++)) = out1;
			(*(__out2++)) = out2;
			
		};
		return __exception;
		
	};
	inline void set_shimmer(t_sample _value) {
		m_shimmer_24 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_decay(t_sample _value) {
		m_decay_25 = (_value < 1 ? 1 : (_value > 100 ? 100 : _value));
	};
	inline void set_damping(t_sample _value) {
		m_damping_26 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_mix(t_sample _value) {
		m_mix_27 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_ratio(t_sample _value) {
		m_ratio_28 = (_value < 0.5 ? 0.5 : (_value > 2 ? 2 : _value));
	};
	inline void set_roomsize(t_sample _value) {
		m_roomsize_29 = (_value < 1 ? 1 : (_value > 300 ? 300 : _value));
	};
	inline void set_bandwidth(t_sample _value) {
		m_bandwidth_30 = (_value < 0 ? 0 : (_value > 100 ? 100 : _value));
	};
	inline void set_tone(t_sample _value) {
		m_tone_31 = (_value < 500 ? 500 : (_value > 6000 ? 6000 : _value));
	};
	
} State;


/// 
///	Configuration for the genlib API
///

/// Number of signal inputs and outputs 

const int gen_kernel_numins = 1;
const int gen_kernel_numouts = 2;

int num_inputs() { return gen_kernel_numins; }
int num_outputs() { return gen_kernel_numouts; }
int num_params() { return 8; }

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
		case 0: self->set_shimmer(value); break;
		case 1: self->set_decay(value); break;
		case 2: self->set_damping(value); break;
		case 3: self->set_mix(value); break;
		case 4: self->set_ratio(value); break;
		case 5: self->set_roomsize(value); break;
		case 6: self->set_bandwidth(value); break;
		case 7: self->set_tone(value); break;
		
		default: break;
	}
}

/// Get the value of a parameter of a State object 

void getparameter(CommonState *cself, long index, t_param *value) {
	State *self = (State *)cself;
	switch (index) {
		case 0: *value = self->m_shimmer_24; break;
		case 1: *value = self->m_decay_25; break;
		case 2: *value = self->m_damping_26; break;
		case 3: *value = self->m_mix_27; break;
		case 4: *value = self->m_ratio_28; break;
		case 5: *value = self->m_roomsize_29; break;
		case 6: *value = self->m_bandwidth_30; break;
		case 7: *value = self->m_tone_31; break;
		
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
	self->__commonstate.params = (ParamInfo *)genlib_sysmem_newptr(8 * sizeof(ParamInfo));
	self->__commonstate.numparams = 8;
	// initialize parameter 0 ("m_shimmer_24")
	pi = self->__commonstate.params + 0;
	pi->name = "shimmer";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_shimmer_24;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 1 ("m_decay_25")
	pi = self->__commonstate.params + 1;
	pi->name = "decay";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_decay_25;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 1;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 2 ("m_damping_26")
	pi = self->__commonstate.params + 2;
	pi->name = "damping";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_damping_26;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 3 ("m_mix_27")
	pi = self->__commonstate.params + 3;
	pi->name = "mix";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_mix_27;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 4 ("m_ratio_28")
	pi = self->__commonstate.params + 4;
	pi->name = "ratio";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_ratio_28;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0.5;
	pi->outputmax = 2;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 5 ("m_roomsize_29")
	pi = self->__commonstate.params + 5;
	pi->name = "roomsize";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_roomsize_29;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 1;
	pi->outputmax = 300;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 6 ("m_bandwidth_30")
	pi = self->__commonstate.params + 6;
	pi->name = "bandwidth";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_bandwidth_30;
	pi->defaultref = 0;
	pi->hasinputminmax = false;
	pi->inputmin = 0; 
	pi->inputmax = 1;
	pi->hasminmax = true;
	pi->outputmin = 0;
	pi->outputmax = 100;
	pi->exp = 0;
	pi->units = "";		// no units defined
	// initialize parameter 7 ("m_tone_31")
	pi = self->__commonstate.params + 7;
	pi->name = "tone";
	pi->paramtype = GENLIB_PARAMTYPE_FLOAT;
	pi->defaultvalue = self->m_tone_31;
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
