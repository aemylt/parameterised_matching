CC=gcc -Wall -O3

mmatch:
	$(CC) m_match.c -o m_match

mmatch-clean:
	rm m_match

kmp:
	$(CC) kmp.c -o kmp

kmp-clean:
	rm kmp