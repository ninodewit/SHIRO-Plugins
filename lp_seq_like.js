inlets = 1			//led, bang
outlets = 2			//led, note
var pattern = 1;	//pattern number
var steps = 8;		//pattern length
var step = 0;		//scroll position
var prev = 7;		//previous scroll position
var value = [];		//array of led values
var dimensional = [];
//for (z=0; z<8; z++) {
//		dimensional[z] = [];
//}

function init() {			//init led values
	outlet(0, 176, pattern + 103, 0)
	pattern = 1;
	outlet(0, 176, pattern + 103, 127)
	for (z=0; z<8; z++) {
		dimensional[z] = [];
	}
	for (z=0; z<8; z++) {
		for (y=0; y<8; y++) {
			for (x=0; x<8; x++) {
				dimensional[z][y * 16 + x] = 0;
			}
		}
	}	
	for (y=0; y<8; y++) {
		for (x=0; x<8; x++) {
			value[y * 16 + x] = dimensional[pattern - 1][y * 16 + x];
		}
	}
}

function msg_int(led) {		//int inlet 1
	toggle(led)
}

function bang() {			//bang inlet 1
	scroll()
}

function toggle(led) {		//toggle leds
	if (value[led]<127) {
		value[led] = 127;
		light(led)
	}
	else if (led % 16==step) {
		value[led] = 15;
		light(led)
	}
	else if (led % 16>=steps) {
		value[led] = 60;
		light(led)
	}
	else {
		value[led] = 0;
		light(led)
	}
}

function light(led) {		//(de)light led
	outlet(0, 144, led, value[led])
}

function scroll() {			//scroll through pattern
	prev = step;
	step = (step + 1) % steps;
	for (y=0; y<8; y++) {
		var last = y * 16 + prev;
		if (prev<steps) {
			if (value[last] == 15) {
				value[last] = 0;
				light(last)
			}
			else if (value[last] == 60) {
 				light(last)
			}
		}
		var led = y * 16 + step;
		if (value[led]==127) {
			outlet(1, Math.ceil(8 - (led / 16)))	//noteout
		}
		else {
			value[led] = 15;
			light(led)
		}
	}
}

function clear() {			//clear leds + pattern
	outlet(0, 176, pattern + 103, 0)
	for (y=0; y<8; y++) {
		for (x=0; x<8; x++) {
			outlet(0, 144, y * 16 + x, 0)
		}
	}
}

function show() {			//show leds + pattern
	outlet(0, 176, pattern + 103, 127)
	for (y=0; y<8; y++) {
		for (x=0; x<8; x++) {
			light(y * 16 + x)
		}
	}
}

function length(stps) {		//change pattern length
	steps = stps;
	if (steps<8) {
		for (x=0; x<steps; x++) {
			for (y=0; y<8; y++) {
				var left = y * 16 + x;
				if (value[left]==60) {
					value[left] = 0;
					light(left)
				}
			}
		}
		for (x=steps; x<8; x++) {
			for (y=0; y<8; y++) {
				var right = y * 16 + x;
				if (value[right]<127) {
					value[right] = 60;
					light(right)
				}
			}
		}
	}
	else {
		for (x=0; x<8; x++) {
			for (y=0; y<8; y++) {
				var all = y * 16 + x;
				if (value[all]==60) {
					value[all] = 0;
					light(all)
				}
			}
		}
	}
}

function page(pttrn) {		//change pattern
	outlet(0, 176, pattern + 103, 0)
	for (y=0; y<8; y++) {
		for (x=0; x<8; x++) {
			dimensional[pattern - 1][y * 16 + x] = value[y * 16 + x];
		}
	}
	outlet(0, 176, pttrn, 127)
	pattern = pttrn - 103;
	for (y=0; y<8; y++) {
		for (x=0; x<8; x++) {
			value[y * 16 + x] = dimensional[pattern - 1][y * 16 + x];
		}
	}
	length(steps)
	outlet(0, 176, pattern + 103, 127)
	for (y=0; y<8; y++) {
		for (x=0; x<8; x++) {
			light(y * 16 + x)
		}
	}
}
