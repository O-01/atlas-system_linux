all: 0-hreadelf 1-hreadelf 2-hreadelf

0-hreadelf:
	@echo -e '#!/bin/bash\n' > $@
	@echo 'readelf -W -h $$1' >> $@
	chmod u+x -v $@

1-hreadelf:
	@echo -e '#!/bin/bash\n' > $@
	@echo 'readelf -W -S $$1' >> $@
	chmod u+x -v $@

2-hreadelf:
	@echo -e '#!/bin/bash\n' > $@
	@echo 'readelf -W -l $$1' >> $@
	chmod u+x -v $@
