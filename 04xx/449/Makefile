all: 449.out 449b.out 449c.out 449d.out
%.out:%.R
	R --no-save < $< > $@
clean:
	-rm *.out *.png *~ *.pdf
view:
	-open *.pdf
	-open *.png

