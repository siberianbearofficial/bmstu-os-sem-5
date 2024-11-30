all: calculator-server

calculator-build: ./calculator/.lastbuildstate
	cd calculator && make -f Makefile.calculator

calculator-server: calculator-build
	cd calculator && ./calculator_server

calculator-client: calculator-build
	cd calculator && ./calculator_client 127.0.0.1

calculator-clean:
	cd calculator && make -f Makefile.calculator clean

clean: calculator-clean
