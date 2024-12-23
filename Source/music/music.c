#include "music.h"
#include "timer.h"

void playNote(NOTE note)
{
	if(note.freq != pausee)
	{
		reset_timer(0);
		init_timer(0, 0, 0, 3, note.freq);
		enable_timer(0);
	}
	reset_timer(2);
	init_timer(2, 0, 0, 7, note.duration);
	enable_timer(2);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM0->TCR != 0) || (LPC_TIM2->TCR != 0));
}
