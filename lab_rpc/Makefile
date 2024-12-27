all: calculator-server

calculator-build: ./calculator/.lastbuildstate
	cd calculator && make -f Makefile.calculator

calculator-server: calculator-build
	cd calculator && ./calculator_server

calculator-client: calculator-build
	cd calculator && ./calculator_client 192.168.181.130

calculator-clean:
	cd calculator && make -f Makefile.calculator clean

bakery-build: ./bakery/.lastbuildstate
	cd bakery && make -f Makefile.bakery

bakery-server: bakery-build
	cd bakery && ./bakery_server

bakery-client: bakery-build
	cd bakery && ./bakery_client 127.0.0.1

bakery-clean:
	cd bakery && make -f Makefile.bakery clean

clean: calculator-clean bakery_clean
