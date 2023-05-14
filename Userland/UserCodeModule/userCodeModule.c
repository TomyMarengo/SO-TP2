#include <stdint.h>
#include <syscalls.h>
#include <timeUtil.h>
#include <coreUtil.h>
#include <string.h>
#include <stopwatch.h>
#include <timer.h>
#include <hangman.h>
#include <sudoku.h>
#include <color.h>
#include <libasm.h>
#include <fractal.h>
#define READBUF_LENGTH 32
#define MAX_PARAMETERS 3
#define LENGTH_PARAMETERS 20

uint32_t width, height;

static void help(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) {
	if(parameters[0][0]!=0){
		print("Try help without parameters\n", 28, gray);
		return;
	}
	
	static int helpCounter = 0;
	if (helpCounter++ >= 3 && ((sys_millis() / 100) % 4) == 0) {
		print("I need somebody!\n", 17, yellow);
		helpCounter = 0;
		return;
	}

	const char* helpstring = 
	"CLEAR                Clears the console.\n"
	"DIVIDEANDCONQUER     The screen will be divided into four windows with the following functions:\n"
	".                    1) The time will be displayed in hh:mm:ss format permanently updating.\n"
	".                    2) In another window there will be a stopwatch with tenths of a second to be\n"
	".                    operated from the keyboard.\n"
	".                    3) In a third window user can play Sudoku by detecting the game completion and\n"
	".                    possible invalid moves.\n"
	".                    4) In the fourth window user can play Hangman.\n"
	"DIVIDEBYZERO         Command to verify the operation of the exception routine \"Divide by zero\"\n"
	"FRACTAL              Draws a pretty fractal on the screen. Warning: high CPU usage.\n"
	"HELP                 Provides help information for commands.\n"
	"INFOREG              Prints on screen the value of all registers.\n"
	"INVALIDOPCODE        Command to verify the operation of the exception routine \"Invalid Opcode\"\n"
	"PRINTMEM             Receives as argument a pointer and performs a memory dump of 32 bytes from the\n"
	".                    address received as an argument.\n"
	"TIME                 Command to display the system day and time.\n";
	print(helpstring, strlen(helpstring), gray);
}

static void divideByZero(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) {
	if(parameters[0][0]!=0){
		print("Try dividebyzero without parameters\n", 36, gray);
		return;
	}

	runDivideByZero();
}

static void invalidOPCode(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) {
	if(parameters[0][0]!=0){
		print("Try invalidopcode without parameters\n", 37, gray);
		return;
	}

	print("Running invalid opcode...", 25, gray);
	runInvalidOpcode();
}

static void time(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) {
	if(parameters[0][0]!=0){
		print("Try time without parameters\n", 28, gray);
		return;
	}

	char buffer[DATE_AND_TIME_LENGTH+1];
	getDateAndTime(buffer);
	print(buffer, DATE_AND_TIME_LENGTH+1, green);
	print("\n", 1, gray);
}

static const char* registerNames[17] = {
    "RIP", "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15"
};

static void inforeg(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) {
	if(parameters[0][0]!=0){
		print("Try inforeg without parameters\n", 31, gray);
		return;
	}

	uint64_t regdata[17];
	if(sys_inforeg(regdata)) {
		char buf[18];
		buf[0] = '0';
		buf[1] = 'x';

		for (int i = 0; i < 17; i++) {
			print(registerNames[i], 3, gray);
			print(": ", 2, gray);
			uint64ToHex(regdata[i], buf+2);
			print(buf, sizeof(buf), gray);
			if ((i % 4) == 0)
				print("\n", 1, gray);
			else
				print("   ", 3, gray);
		}
	}
	else {
		print("You must first take a screenshot of the registers. Press CTRL to do so and try again.\n", 86, gray);
	}
	
}

static char valueToHexChar(uint8_t value) {
    return value >= 10 ? (value - 10 + 'A') : (value + '0');
}


static void printMem(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) {
	if(parameters[0][0]==0){
		print("Printmem needs a pointer to a memory address\n", 45, gray);
		return;
	}

	uint8_t* p;
	if (!tryReadHexAddress(parameters[0], &p)) {
		print("Parameter is not a valid address. Must be a 0x-prefixed hex number\n", 67, gray);
		return;
	}
	
	for(int i=0; i<32; i++) {
		char buf[5] = "0x00 ";
		buf[2] = valueToHexChar(p[i] >> 4);
		buf[3] = valueToHexChar(p[i] & 0x0F);
		print(buf, 5, gray);
	}
	print("\n",1,gray);
}

void divideAndConquer(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) {
	if(parameters[0][0]!=0){
		print("Try divideandconquer without parameters\n", 40, gray);
		return;
	}

	clearscreen();
	sys_drawline(width/2, 0, width/2, height, magenta); //vertical line
	sys_drawline(0, height/3, width, height/3, magenta); //horizontal line
	sys_writeat("Press ESC to exit.",18,CHAR_WIDTH,0,gray);
	stw_init();
	sdk_init();
	hang_init();

	char readbuf[1];
	uint64_t lastmillis = 0;

	while(1) {
		uint64_t readlen = sys_pollread(KBDIN, readbuf, 1, 1);
		uint64_t millis = sys_millis();

		if(millis != lastmillis) {
			lastmillis = millis;
			timer_update(millis);
			stw_update(millis);
		}

		if(readlen != 0) {	
			char ascii = scancodeToAscii(readbuf[0]);
			if(ascii == ' ') {
				stw_changeStatus(millis);
			}
			else if(ascii == '\t') {
				stw_stop();
			}
			else if(readbuf[0] == UP || readbuf[0] == DOWN || readbuf[0] == LEFT || readbuf[0] == RIGHT) {
				sdk_move(readbuf[0]);
			}
			else if(ascii>='0' && ascii<='9') {
				sdk_update(ascii);
			}
			else if(ascii>='a' && ascii<='z') {
				hang_update(ascii);
			}
			else if(ascii==27) {
				stw_stop();
				clearscreen();
				break;
			}
		}
	}

	clearscreen();
}

static void fractal(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) {
	if(parameters[0][0]!=0){
		print("Try fractal without parameters\n", 31, gray);
		return;
	}
	
	frc_run();
	clearscreen();
}

static void clear(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) {
	if(parameters[0][0]!=0){
		print("Try clear without parameters\n", 29, gray);
		return;
	}

	clearscreen();	
}

static const char* commands[] = {"clear", "divideandconquer", "dividebyzero", "fractal", "help", "inforeg", "invalidopcode", "printmem", "time"};
static void (*commands_functions[])(char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS]) = {clear, divideAndConquer, divideByZero, fractal, help, inforeg, invalidOPCode, printMem, time};
#define COMMANDS_LENGTH (sizeof(commands)/sizeof(commands[0]))

static int indexCommand(char* readbuf) {
	char *p = readbuf;
	for(int i=0; i<COMMANDS_LENGTH; i++) {
		int cmp;
		if((cmp = strcmp(p,commands[i])) < 0){
			return -1;
		}	
		if(cmp == 0){
			return i;
		}
	}
	return -1;
}

void getCommand(char command[READBUF_LENGTH], char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS], char readbuf[READBUF_LENGTH]) {
	int i, j, k;
	
	for(i=0, j=0; i<READBUF_LENGTH && readbuf[i]!=' '; i++){
			command[j++] = readbuf[i];
	}

	command[j] = 0;

	while(i<READBUF_LENGTH && readbuf[i]==' '){
		i++;
	}

	for(j=0, k=0; i<READBUF_LENGTH;) {
		if(k>=MAX_PARAMETERS || j>=LENGTH_PARAMETERS)
			return;
		if(readbuf[i]!=' ') {
			parameters[k][j++] = readbuf[i++];
		}
		else {
			parameters[k][j] = 0;
			k++;
			j=0;
			while(i<READBUF_LENGTH && readbuf[i]==' '){
				i++;
			}
		}
	}
}

int main() {
	getScreenSize(&width, &height);

	clearscreen();	
	print("Welcome to Shell! Type \"HELP\" for a list of commands.\n\n", 55, green);

	while(1) {
		print("$ ", 2, magenta);
		char readbuf[READBUF_LENGTH] = {0};
		scanf(readbuf,READBUF_LENGTH);

		char command[READBUF_LENGTH] = {0};
		char parameters[MAX_PARAMETERS][LENGTH_PARAMETERS] = {{0}};
		getCommand(command, parameters, readbuf);

		int index;
		if((index = indexCommand(command))>=0) {
			commands_functions[index](parameters);
		}
		else {
			print(command, strlen(command), gray);
			print(": command not found\n", 20, gray);
		}
	}

	return 420;
}
