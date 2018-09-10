collectlib_DIR=$(top_builddir)/Library/Linux
collectbin_DIR=$(top_builddir)/Binary

#collectlib_DIR_CONTENTS=
#collectbin_DIR_CONTENTS=

collectLIBRARIES=$(lib_LIBRARIES) $(pkglib_LIBRARIES)
collectLTLIBRARIES=$(lib_LTLIBRARIES) $(pkglib_LTLIBRARIES)
collectPROGRAMS=$(bin_PROGRAMS) $(sbin_PROGRAM) $(pkglib_PROGRAMS) $(libexec_PROGRAMS)

AM_LDFLAGS+=-L$(collectlib_DIR)

all-local:	collectlib-local collectbin-local

collectlib-local: $(addprefix $(collectlib_DIR)/, $(collectLIBRARIES) $(collectLTLIBRARIES))
collectbin-local: $(addprefix $(collectbin_DIR)/, $(collectPROGRAMS))

$(collectlib_DIR)/%.a: %.a
	mkdir -p $(collectlib_DIR)
	ln -sf --target-directory=$(collectlib_DIR)/ $(addprefix $(CURDIR)/, $<)

$(collectlib_DIR)/%.la: %.la
	mkdir -p $(collectlib_DIR)/.libs
	ln -sf $(CURDIR)/$< $(collectlib_DIR)/
	ln -sf $(CURDIR)/.libs/$(<:%.la=%.so*) $(collectlib_DIR)/
	ln -sf $(CURDIR)/.libs/$(<:%.la=%.a) $(collectlib_DIR)/
	@ln -sf $(CURDIR)/.libs/$(<:%.la=%.so*) $(collectlib_DIR)/.libs/
	@ln -sf $(CURDIR)/.libs/$(<:%.la=%.a) $(collectlib_DIR)/.libs/

$(collectbin_DIR)/%: %
	mkdir -p $(collectbin_DIR)
	ln -sf --target-directory=$(collectbin_DIR)/ $(addprefix $(CURDIR)/, $<)

MOSTLYCLEANFILES+=$(foreach t, $(collectLTLIBRARIES), $(collectlib_DIR)/$(t))
MOSTLYCLEANFILES+=$(foreach t, $(collectLTLIBRARIES), $(collectlib_DIR)/$(t:%.la=%.so*))
MOSTLYCLEANFILES+=$(foreach t, $(collectLTLIBRARIES), $(collectlib_DIR)/$(t:%.la=%.a))
MOSTLYCLEANFILES+=$(foreach t, $(collectLIBRARIES), $(collectlib_DIR)/$(t))
MOSTLYCLEANFILES+=$(foreach t, $(collectPROGRAMS), $(collectlib_DIR)/$(t))

#UILT_SOURCES+=$(collectlib_DIR) $(collectbin_DIR)

.PHONY: mkdir-collect

all: mkdir-collect

mkdir-collect: $(collectlib_DIR) $(collectbin_DIR)

$(collectlib_DIR) $(collectbin_DIR):
	mkdir -p $@

