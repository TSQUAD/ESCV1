//i is the phase
int i = 0;
//RPM start tool
int k = 0;
//debugging tool for the MUX
int p = 0;

//this is the motor RPM and is claculated by the prescaler and poles w/ timing
int RPM = 1200;
//tick is not useful and will be overwritten later by the code 
//tick represents the number of ticks for timer 1 to hit before going to the next phase();
int tick  = 255;
int max_RPM = 10000;

//start pwm width for the high
int PWM_WIDTH = 255;
//end pwm width for the high
int end = 250;
int end_final = 245;
bool prev_state = 0;
