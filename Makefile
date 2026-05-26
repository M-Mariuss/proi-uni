build:
	gcc main.c -o lanParty
run:
	./lanParty ./date/t1/c.in ./date/t1/d.in ./out/out1.out
	./lanParty ./date/t2/c.in ./date/t2/d.in ./out/out2.out
	./lanParty ./date/t3/c.in ./date/t3/d.in ./out/out3.out

clean:
	rm lanParty