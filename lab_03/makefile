all: clean

main: main.c
	gcc main.c -o main.out

logs:
	tail -f /var/log/syslog

clean:
	rm *.out
