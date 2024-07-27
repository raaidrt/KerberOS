#ifndef _CMOS_H
#define _CMOS_H

#include <stdint.h>
#include <stdbool.h>

#define NMI_DISABLE 0x1
#define CALLBACK_QUEUE_SIZE 32

enum weekday {
    Sunday = 1,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

enum month {
    January = 1,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

struct time {
    enum weekday weekday: 4;
    enum month month: 4;
    uint8_t day;
    uint8_t year; // 7 bits since 2^7 = 128 > 100
    uint8_t century; // 1 bit
    uint8_t seconds; // 6 bits since 2^6 = 64 > 60
    uint8_t minutes; // 6 bits since 2^6 = 64 > 60
    uint8_t hours; // 5 bits since 2^5 = 32 > 24
};

/**
 * get_time will block the execution of the program until
 * the time is made available from the Real Time Clock on the CPU
 *
 * Returns a struct with information about the current time.
 */
struct time get_time();

/**
 * get_time_irq8 will return the time from the Real Time Clock, and
 * assumes that this method is called upon receipt of the Update Interrupt
 * on IRQ 8.
 *
 * Returns a struct with infromation about the current time.
 */
struct time get_time_irq8();

typedef void time_callback_t(struct time, void *);

/**
 * register_time_callback will register a callback provided by
 * the user to be called when the time has been made available from the 
 * Real Time Clock.
 *
 * @param user_callback the callback to register
 * @param data is an optional closure allocated on the heap
 *
 * Returns whether the registration of the callback was successful
 * or not, since the queue for registering the callbacks 
 * is bounded in size
 */
bool register_time_callback(time_callback_t *user_callback, void* data);

/**
 * handle_rtc_ready will handle the time being made available
 * from the Real Time Clock. This function is meant to be used by the
 * interrupt handler upon receipt of the Update Interrupt from the RTC
 *
 * @param time the time information received from the RTC
 */
void handle_rtc_ready(struct time);

/**
 * iso_8601_format will write down the ISO 8601 representation of
 * the time that is supplied
 *
 * @param buffer is the buffer to write to, and must contain exactly
 *  20 elements in the buffer
 * @param time is the time that will be written
 */
void iso_8601_format(char buffer[20], struct time time);

#endif
