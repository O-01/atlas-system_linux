all: hnm hobjdump

hnm:
	@echo '#!/bin/bash' > $@
	# @echo '( exec -a "./hnm" nm -p $$1 )' >> $@
	chmod u+x -v $@

hobjdump:
	@echo '#!/bin/bash' > $@
	# @echo '( exec -a "./hobjdump" objdump -sf $$1 )' >> $@
	chmod u+x -v $@

clean:
	rm -f hnm hobjdump
