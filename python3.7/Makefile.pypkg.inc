include $(shell pwd)/../$(PYTHON_VER)/Makefile.common

#PYTHON_VER=python3.7
#PKG_NAME=example
#UPSTREAM=https://example.com/upstream.tar.gz
#PKG_EXAMPLEDEP=$(shell pwd)/../python3-exampledep
#PKG_DEPS=$(PKG_EXAMPLEDEP)
PKG_DEPS?=
BUILD_DEPS=$(shell for i in $(PKG_DEPS); do echo $$i/$(INSTALL_DIR); done)
BUILD_HOSTDEPS=$(shell for i in $(PKG_DEPS); do echo $$i/$(INSTALL_HOSTDIR); done)

.PHONY: all
all: $(INSTALL_DIR) $(INSTALL_HOSTDIR)

dl/$(TARBALL):
	mkdir -p dl
	../scripts/fetch.sh $(UPSTREAM) dl/$(TARBALL)

$(PYTHON_DIST):
	make -C $(PYTHON_DIR)

$(PYTHON_HOSTDIST):
	make -C $(PYTHON_DIR) build_host/cython

%/$(INSTALL_DIR):
	make -C $* $(INSTALL_DIR)

%/$(INSTALL_HOSTDIR):
	make -C $* $(INSTALL_HOSTDIR)

%/freeze_python:
	make -C $* freeze_python/frozen.c

%/$(EXAMPLES_DIR)/pythonRun:
	make -C $* $(EXAMPLES_DIR)/pythonRun

build: $(PYTHON_DIST) $(BUILD_HOSTDEPS) $(BUILD_DEPS) | dl/$(TARBALL)
	mkdir -p $@
	(cd build && tar -x --strip-components 1 -f ../dl/$(TARBALL))

build/stamp_patch: | build patches/dist/*
	(cd build && ../../scripts/apply-patches.sh ./ ../patches/dist/*)
	touch $@

SETUP_PYTHONPATH?="$$(for i in $(PKG_DEPS); do echo -n "$$i/$(INSTALL_HOSTDIR)/lib/$(PYTHON_VER)/site-packages/:"; done)"

INSTALL_DEPS?=build
SETUP_EXTRAARGS?=
SETUP_EXTRAENV?=
$(INSTALL_DIR): $(INSTALL_DEPS)
	cd build; \
		CC=$(RUMPRUN_CC) CXX=$(RUMPRUN_CXX) FC=$(RUMPRUN_FC) _PYTHON_PROJECT_BASE=$(PYTHON_SRC) \
		F90=$(RUMPRUN_FC) F77=$(RUMPRUN_FC) \
		LDFLAGS="-L$(RUMPRUN_PKGS_DIR)/lib" CFLAGS="-I$(RUMPRUN_SYSROOT)/include/gcc-6" \
		PYTHONPATH=$(SETUP_PYTHONPATH) $(SETUP_EXTRAENV) \
		$(PYTHON_VER) setup.py install --prefix $(shell pwd)/$(INSTALL_DIR) \
		--single-version-externally-managed --root / $(SETUP_EXTRAARGS) 2>&1 |tee $(shell pwd)/build.log

FREEZE_DEPS=$(shell for i in $(PKG_DEPS); do echo $$i/freeze_python; done)
FREEZE_PYTHONPATH?="$$(for i in $(PKG_DEPS); do echo -n "$$i/$(INSTALL_DIR)/lib/$(PYTHON_VER)/site-packages:"; done; \
                        echo $$(pwd)/$(INSTALL_DIR)/lib/$(PYTHON_VER)/site-packages/)"
FREEZE_MODULES?=
freeze_python/frozen.c: $(INSTALL_DIR) $(FREEZE_DEPS)
	rm -rf freeze_python
	mkdir freeze_python
	echo $(FREEZE_MODULES) > freeze_python/dep_modules.txt
	(cd freeze_python; \
		echo "import encodings.aliases\nimport encodings.ascii" >> freezeInputScript.py; \
		for i in $(PKG_DEPS); do \
			for m in $$(cat $$i/freeze_python/dep_modules.txt); do \
				echo "import $$m" >> freezeInputScript.py; done; done; \
		for i in $(FREEZE_MODULES); do echo "import $$i" >> freezeInputScript.py; done)
	PYTHONHOME=$(PYTHON_DIST) PYTHONPATH=$(FREEZE_PYTHONPATH) CC=$(RUMPRUN_CC) \
	$(PYTHON_VER) -S $(PYTHON_SRC)/Tools/freeze/freeze.py -o freeze_python -p $(PYTHON_SRC) freeze_python/freezeInputScript.py
	(cd freeze_python; numberOfLines=$$(cat frozen.c | wc -l); \
		numberOfLinesToKeep=$$(($$numberOfLines - 9)); head -n $$numberOfLinesToKeep frozen.c > frozen.h)

build_host: $(PYTHON_HOSTDIST) $(BUILD_HOSTDEPS) | dl/$(PYTHON_TARBALL)
	mkdir -p $@
	(cd build_host && tar -x --strip-components 1 -f ../dl/$(TARBALL))

build_host/stamp_patch: | build_host patches/host/*
	(cd build_host && ../../scripts/apply-patches.sh ./ ../patches/host/*)
	touch $@

INSTALL_HOSTDEPS?=build_host
SETUP_HOSTEXTRAARGS?=
SETUP_HOSTEXTRAENV?=
$(INSTALL_HOSTDIR): $(INSTALL_HOSTDEPS)
	(cd build_host; \
		CPPFLAGS='' LDFLAGS='' _PYTHON_PROJECT_BASE=$(PYTHON_DIR)/build_host \
		PYTHONPATH=$(SETUP_PYTHONPATH) $(SETUP_HOSTEXTRAENV) \
		$(PYTHON_VER) setup.py install --prefix $(shell pwd)/$(INSTALL_HOSTDIR) \
		--single-version-externally-managed --root / $(SETUP_HOSTEXTRAARGS) 2>&1 |tee $(shell pwd)/build_host.log;)


$(EXAMPLES_DIR)/$(PKG_NAME)_builtin.h.default: $(INSTALL_DIR)
        $(PKG_NAME)SharedObjects="$(shell find python -name "*.so")"; \
        $$(echo '#include <Python.h>\n\n#ifdef __cplusplus\nextern "''C''" {\n#endif\n' >> $@; \
                for i in $${$(PKG_NAME)SharedObjects}; do nm $$i | grep PyInit | awk '{ print "extern PyObject *"$$3"(void);" }' >> $@; done; \
                echo '\nvoid add_$(PKG_NAME)_builtin()\n{' >> $@; \
                for i in $${$(PKG_NAME)SharedObjects}; do p=`dirname "$$i"`; \
                a=`echo -n "PyImport_AppendInittab(\"$(PKG_NAME)." && echo -n $$p | awk -F'site-packages/$(PKG_NAME)/' '{ gsub("/",".",$$NF); print $$NF }'`; \
                b=`nm $$i | grep PyInit | awk -FPyInit_ '{ if ($$2) print $$2 "\", &PyInit_"$$2");" }'`; test ! -z "$$b" && echo "\t$$a.$$b" >> $@; done; \
                echo '}\n\n#ifdef __cplusplus\n}\n#endif\n' >> $@)

BUILDP_DEPS=$(shell for i in $(PKG_DEPS); do echo $$i/$(EXAMPLES_DIR)/pythonRun; done)
BUILDP_LIBS?=-lpthread -lopenblas -lgfortran -lquadmath -lm -lutil -lsqlite3 -lz -lssl -ltls -lcrypto -lffi
BUILDP_LINKER?=$(RUMPRUN_CC)
$(EXAMPLES_DIR)/pythonRun: BUILDP_OBJECTS=$(shell find $(shell pwd)/build/ -name \*.o)
$(EXAMPLES_DIR)/pythonRun: freeze_python/frozen.c $(BUILDP_DEPS)
	echo $(BUILDP_OBJECTS) > freeze_python/dep_objects.txt
	(cd $(EXAMPLES_DIR);\
	frozenSources="$(shell find $(shell pwd)/freeze_python/ -name M_\*.c)";\
	objects="$$(for i in $(PKG_DEPS); do cat "$$i/freeze_python/dep_objects.txt"; done) $(BUILDP_OBJECTS)";\
	$(BUILDP_LINKER) -g -Xlinker -export-dynamic -o pythonRun pythonRun.c -I$(PYTHON_DIST)/include/$(PYTHON_VER)m \
		-L${RUMPRUN_PKGS_DIR}/lib -I${RUMPRUN_PKGS_DIR}/include -I$(shell pwd)/freeze_python $$frozenSources $$objects \
		$(PYTHON_DIST)/lib/lib$(PYTHON_VER)m.a $(BUILDP_LIBS))

$(EXAMPLES_DIR)/pythonRun.%t: $(EXAMPLES_DIR)/pythonRun
	cd $(EXAMPLES_DIR);\
	rumprun-bake solo5_$*t pythonRun.$*t pythonRun

$(EXAMPLES_DIR)/fs.iso: $(EXAMPLES_DIR)/*.py
	cd $(EXAMPLES_DIR);\
	rm -rf fs; mkdir -p fs/lib;\
	cp *.py fs/;\
	cp -r $(PYTHON_DIST)/lib/$(PYTHON_VER) fs/lib/;\
	cp -r $(shell pwd)/$(INSTALL_DIR)/lib/$(PYTHON_VER)/site-packages/* fs/lib/$(PYTHON_VER)/site-packages/;\
	for i in $(PKG_DEPS); do \
		cp -r $$i/$(INSTALL_DIR)/lib/$(PYTHON_VER)/site-packages/* fs/lib/$(PYTHON_VER)/site-packages/; done;\
	$(RUMPRUN_GENISOIMAGE) -o fs.iso fs


.PHONY: clean_dist
clean_dist:
	rm -rf build $(INSTALL_DIR)
	rm -rf freeze_python
	rm -rf $(EXAMPLES_DIR)/fs* $(EXAMPLES_DIR)/pythonRun $(EXAMPLES_DIR)/pythonRun.spt $(EXAMPLES_DIR)/pythonRun.hvt

.PHONY: clean_hostdist
clean_hostdist:
	rm -rf build_host $(INSTALL_HOSTDIR)
