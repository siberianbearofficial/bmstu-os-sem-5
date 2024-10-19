help:
	echo "Используйте цели task1, task2, task3, task4, task5 или clean."

all: help

app1: app1.c
	gcc app1.c -o app1.out

app2: app2.c
	gcc app2.c -lm -o app2.out

task1: task1.c
	gcc task1.c -o task1.out

task2: task2.c
	gcc task2.c -o task2.out

task3: app1 app2 task3.c
	gcc task3.c -o task3.out

task4: task4.c
	gcc task4.c -o task4.out

task5: task5.c
	gcc task5.c -o task5.out

clean:
	rm -rf *.out
