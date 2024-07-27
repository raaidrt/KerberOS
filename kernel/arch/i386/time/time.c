#include <kernel/time/time.h>
#include <io.h>
#include <stdio.h>
#include <panic.h>
#include <kernel/irq/irq.h>
#include <kernel/interrupt/flag.h>

#define CMOS_CMD_PORT	0x70
#define CMOS_DATA_PORT	0x71

#define CMOS_SECONDS	0x00
#define CMOS_MINUTES	0x02
#define CMOS_HOURS	0x04
#define CMOS_WEEKDAY	0x06
#define CMOS_DAY	0x07
#define CMOS_MONTH	0x08
#define CMOS_YEAR	0x09
#define CMOS_CENTURY	0x32

#define CMOS_REG_A	0x0A
#define CMOS_REG_B	0x0B

enum hour_format {
	hour_unset,
	twenty_four_hour_format,
	twelve_hour_format
};

static enum hour_format hour_format = hour_unset;

enum encoding_format {
	encoding_unset,
	BCD,
	binary
};

static enum encoding_format encoding_format = encoding_unset;

static void set_time_format_if_unset() {
	if (hour_format == hour_unset || encoding_format == encoding_unset) {
		outb(CMOS_CMD_PORT, (NMI_DISABLE << 7) | CMOS_REG_B);
		unsigned char status = inb(CMOS_DATA_PORT);
		if (hour_format == hour_unset) {
			
			if (!!(status & 0b10)) { // hour format
				hour_format = twenty_four_hour_format;
			} else {
				hour_format = twelve_hour_format;
			}
		}
		if (encoding_format == encoding_unset) {
			if (!!(status & 0b100)) {
				encoding_format = binary;
			} else {
				encoding_format = BCD;
			}
		}
	}
}

unsigned char get_rtc_register(unsigned char reg) {
	outb(CMOS_CMD_PORT, (NMI_DISABLE << 7) | reg);
	return inb(CMOS_DATA_PORT);
}

bool update_in_progress() {
	outb(CMOS_CMD_PORT, (NMI_DISABLE << 7) | CMOS_REG_A);
	return !!(inb(CMOS_DATA_PORT) & 0x80);
}

unsigned char convert_from_bcd(unsigned char bcd_num) {
	return (bcd_num & 0xf) + (10 * (bcd_num >> 4));
}

unsigned char convert_12_hour_format(unsigned char hours, bool is_pm) {
	switch (hour_format) {
	case twenty_four_hour_format:
		return hours;
	case twelve_hour_format:
		if (!is_pm && hours == 12)
			return 0;
		else if (is_pm)
			return hours + 12;
		else
			return hours;
	default:
		panic("Hour format has not been set");
	}
}

struct time get_time_irq8() {
	enum weekday weekday = get_rtc_register(CMOS_WEEKDAY);
	enum month month = get_rtc_register(CMOS_MONTH);
	unsigned char seconds = get_rtc_register(CMOS_SECONDS);
	unsigned char hours = get_rtc_register(CMOS_HOURS);
	unsigned char minutes = get_rtc_register(CMOS_MINUTES);
	unsigned char century = get_rtc_register(CMOS_CENTURY);
	unsigned char day = get_rtc_register(CMOS_DAY);
	unsigned char year = get_rtc_register(CMOS_YEAR);

	struct time time = {
		weekday,
		month,
		day,
		year,
		century,
		seconds,
		minutes,
		hours
	};
	return time;
}

struct time get_time() {
	set_time_format_if_unset();
	while (update_in_progress()); // loop while an update is in progress
	
	enum weekday weekday = get_rtc_register(CMOS_WEEKDAY);
	enum month month = get_rtc_register(CMOS_MONTH);
	unsigned char seconds = get_rtc_register(CMOS_SECONDS);
	unsigned char hours = get_rtc_register(CMOS_HOURS);
	unsigned char minutes = get_rtc_register(CMOS_MINUTES);
	unsigned char century = get_rtc_register(CMOS_CENTURY);
	unsigned char day = get_rtc_register(CMOS_DAY);
	unsigned char year = get_rtc_register(CMOS_YEAR);
	enum weekday last_weekday;
	enum month last_month;
	unsigned char last_year, last_second, last_hour, last_minute, last_century, last_day;
	do {
		last_second = seconds;
		last_day = day;
		last_hour = hours;
		last_minute = minutes;
		last_century = century;
		last_weekday = weekday;
		last_month = month;
		last_year = year;

		while (update_in_progress()); 
		// loop while an update is in progress
		weekday = get_rtc_register(CMOS_WEEKDAY);
		day = get_rtc_register(CMOS_DAY);
		month = get_rtc_register(CMOS_MONTH);
		seconds = get_rtc_register(CMOS_SECONDS);
		hours = get_rtc_register(CMOS_HOURS);
		minutes = get_rtc_register(CMOS_MINUTES);
		century = get_rtc_register(CMOS_CENTURY);
		year = get_rtc_register(CMOS_YEAR);
	} while (
	        (last_second != seconds) 
	     || (last_minute != minutes) 
	     || (last_hour != hours) 
	     || (last_day != day) 
	     || (last_weekday != weekday)
	     || (last_month != month) 
	     || (last_year != year) 
	     || (last_century != century));
	if (encoding_format == BCD) {
		unsigned char high_bit = hours & 0x80;
		hours = 0x7f & hours;
		hours = convert_12_hour_format(convert_from_bcd(hours), !!high_bit);
		seconds = convert_from_bcd(seconds);
		minutes = convert_from_bcd(minutes);
		day = convert_from_bcd(day);
		year = convert_from_bcd(year);
		century = convert_from_bcd(century);
	}
	struct time time = {
		weekday,
		month,
		day,
		year,
		century,
		seconds,
		minutes,
		hours
	};
	return time;
}

time_callback_t *callback[CALLBACK_QUEUE_SIZE];
void *callback_data[CALLBACK_QUEUE_SIZE];
size_t queue_size = 0;

enum irq_8_state {
	configured, not_configured
};

enum irq_8_state irq_8_state = not_configured;

void configure_irq_8() {
	if (irq_8_state == not_configured) {
		outb(CMOS_CMD_PORT, 0x8B);
		unsigned char prev = inb(CMOS_DATA_PORT);
		outb(0x70, 0x8B);
		outb(0x71, prev | 0x40);
	}
}

void disable_irq_8() {
	irq_set_mask(8);
}

void enable_irq_8() {
	irq_clear_mask(8);
}

bool register_time_callback(time_callback_t *user_callback, void *data) {
	clear_interrupt_flag();
	configure_irq_8();
	enable_irq_8();
	if (queue_size >= CALLBACK_QUEUE_SIZE) {
		set_interrupt_flag();
		return false;
	}
	callback[queue_size] = user_callback;
	callback_data[queue_size] = data;
	queue_size++;
	set_interrupt_flag();
	return true;
}

void handle_rtc_ready(struct time time) {
	if (queue_size == 0) {
		disable_irq_8();
	}
	queue_size--;
	(*callback[queue_size])(time, callback_data[queue_size]);
}

/**
 * write_integer_repr will write out the representation of
 * some number within a fixed digit width
 *
 * @param p the location where to write the number
 * @param d the number of digits to write
 * @param x the number to write
 *
 * So, for example, if we call write_integer_repr(p, 5, 190),
 * then we will have
 * Contents:      | 0 | 0 | 1 | 9 | 0 |
 * Address:       | p |p+1|p+2|p+3|p+4|
 *
 * If the number to write has more digits than d, then the
 * result will simply be truncated
 */
void write_integer_repr(char *p, int d, int x) {
	for (int i = d - 1; i >= 0; i--) {
		p[i] = "0123456789"[x % 10];
		x /= 10;
	}
}

/*
 * ISO 8601 Time format
 * YYYY:MM:DDThh:mm:ss
 */
void iso_8601_format(char buffer[20], struct time time) {
	write_integer_repr((char *)buffer, 4, time.century * 100 + time.year);
	buffer[4] = ':';
	write_integer_repr((char *)(&buffer[5]), 2, time.month);
	buffer[7] = ':';
	write_integer_repr((char *)(&buffer[8]), 2, time.day);
	buffer[10] = 'T';
	write_integer_repr((char *)(&buffer[11]), 2, time.hours);
	buffer[13] = ':';
	write_integer_repr((char *)(&buffer[14]), 2, time.minutes);
	buffer[16] = ':';
	write_integer_repr((char *)(&buffer[17]), 2, time.seconds);
	buffer[19] = '\0';
}

