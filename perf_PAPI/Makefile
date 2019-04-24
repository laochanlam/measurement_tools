all: prime prime_with_papi

prime: prime_sieve-64.c profile.c
	gcc -I/usr/local/include -O0 prime_sieve-64.c profile.c /usr/local/lib/libpapi.a -o prime

prime_with_papi: prime_sieve-64.c profile.c
	gcc -I/usr/local/include -O0 prime_sieve-64.c profile.c /usr/local/lib/libpapi.a -o prime_with_papi -DWITH_PAPI

clean:
	rm prime prime_with_papi