all: 517A.out 517Adk.out 517B.out 517B2.out 517Bdk.out 517C.out
%.out: %.R
	R --no-save < $< > $@
clean:
	-rm *~ *.png *.out *swp
view:
	-open *.png
