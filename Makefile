all: xkb-layout-notifier

xkb-layout-notifier: xkbln.c
	gcc xkbln.c -o xkb-layout-notifier -lXext

clean:
	rm xkb-layout-notifier
