all: hnm hobjdump

hnm:
	@echo -e '#!/bin/bash\n' > $@
	@echo 'nm -p $$1' >> $@
	chmod u+x -v $@

hobjdump:
	@echo -e '#!/bin/bash\n' > $@
	@echo 'objdump -sf $$1' >> $@
	chmod u+x -v $@
