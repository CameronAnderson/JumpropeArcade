// #include <p24FJ128GB206.h>
// #include "common.h"
// #include "segment.h"

// _SD;

// void segment_showNumber(_SD *self, float value){
  
//   int number = abs(value); 
  
//   for (uint8_t x = 0 ; x < 2 ; x++){
//     int leftover = number % 10;

//     postNumber(leftover, false);

//     number /= 10;
//   }

//   //Latch the current segment data
//   digitalWrite(segmentLatch, LOW);
//   digitalWrite(segmentLatch, HIGH); //Register moves storage register on the rising edge of RCK
// }


// void segment_postNumber(_SD *self, uint8_t number, bool decimal){
// 	#define a  1<<0
// 	#define b  1<<6
// 	#define c  1<<5
// 	#define d  1<<4
// 	#define e  1<<3
// 	#define f  1<<1
// 	#define g  1<<2
// 	#define dp 1<<7

// 	uint8_t segments;

// 	switch (number){
//     case 1: segments = b | c; break;
//     case 2: segments = a | b | d | e | g; break;
//     case 3: segments = a | b | c | d | g; break;
//     case 4: segments = f | g | b | c; break;
//     case 5: segments = a | f | g | c | d; break;
//     case 6: segments = a | f | g | e | c | d; break;
//     case 7: segments = a | b | c; break;
//     case 8: segments = a | b | c | d | e | f | g; break;
//     case 9: segments = a | b | c | d | f | g; break;
//     case 0: segments = a | b | c | d | e | f; break;
//     case ' ': segments = 0; break;
//     case 'c': segments = g | e | d; break;
//     case '-': segments = g; break;
//   }

//   if (decimal) segments |= dp;

//    for (uint8_t x = 0 ; x < 8 ; x++)
//   {
//     pin_write(segmentClock, 0);
//     pin_write(segmentData, segments & 1 << (7 - x));
//     pin_write(segmentClock, 1); 
// }



// }