all: hnm hobjdump

hnm:
	@echo '#!/bin/bash' > $@
	@echo 'nm -p $$1' >> $@
	chmod u+x -v $@

hobjdump:
	@echo '#!/bin/bash' > $@
	@echo 'objdump -sf $$1' >> $@
	chmod u+x -v $@
