VERSIONINC = version.h

lib_LTLIBRARIES  = libmodule.la 

bin_PROGRAMS = contdaem contdaemtest
BUILT_SOURCES = version.h

# Main application
contdaem_SOURCES = contdaem.c module_tick_master.c
contdaem_LDADD = -lmodule
contdaem_DEPENDENCIES = libmodule.la

contdaemtest_SOURCES = contdaemtest.c 
contdaemtest_LDADD = -lmodule
contdaemtest_DEPENDENCIES = libmodule.la


# General libs
libmodule_la_SOURCES = uni_data.c module_event.c module_util.c type_loader.c module_type.c module_base.c module_tick.c module_value.c
#libmodule_la_LIBADD = -lxmlpar 

include_HEADERS = contdaem.h module_base.h module_event.h module_type.h module_util.h uni_data.h module_tick.h module_value.h

# clean backup files
CLEANFILES = *~ .ver $(VERSIONINC)

AM_CFLAGS = -Wall -Werror

version.h:
	echo '/* Auto-generated at build time. */' > $(VERSIONINC)
	echo "#define COMPILE_TIME \"`date '+%B %d, %Y at %H:%M'`\"" >> $(VERSIONINC)
	echo '#define COMPILE_BY "$(USERNAME)"'  >> $(VERSIONINC)
	if (test -n "$(SVNVERSION)" || test -f .svn); then \
		echo -n '#define BUILD "' >> $(VERSIONINC); \
		svnversion -n >> $(VERSIONINC); \
		echo '"' >> $(VERSIONINC); \
	else \
		echo '#define BUILD " "' >> $(VERSIONINC); \
	fi
	if ( test -z "$(host_alias)" ); then \
		echo "#define ARCH \"`uname -m`\"" >> $(VERSIONINC); \
	else \
	  echo "#define ARCH \"$(host_alias)\"" >> $(VERSIONINC); \
	fi

