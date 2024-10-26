all: clean

main: main.c
	gcc main.c -o main.out

did:
	python3 did.py

logs:
	tail -f /var/log/syslog | grep "my_daemon"

clean:
	rm *.out
