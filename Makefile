build:
	gcc Minilinux.c -g -o minilinux

clean:
	rm minilinux

checker:
	python3 checker.py
