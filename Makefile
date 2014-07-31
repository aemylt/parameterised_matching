CC=gcc -Wall -O3

all:
	$(CC) p_match.c -o p_match

clean:
	rm p_match

mmatch:
	$(CC) m_match.c -o m_match

mmatch-clean:
	rm m_match

kmp:
	$(CC) kmp.c -o kmp

kmp-clean:
	rm kmp