#include "b1_layer.h"
#include "includes.h"



void b1_layer::create2(CCObject*) {
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20A23C), "\xe9\x79\x06\x00\x00", 5, NULL);
}

void b1_layer::create3(CCObject*) {
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x20A23C), "\x6a\x14\x8b\xcb\xff", 5, NULL);
}



// I copied some code from GDFireMario211 Shoutout to him :D
// and thanks to Matcool for this amazing template :D