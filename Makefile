all: NTP_LocalTime

NTP_LocalTime: NTP_LocalTime.c
	gcc -o NTP_LocalTime NTP_LocalTime.c -Wall

clean:
	rm -f NTP_LocalTime
