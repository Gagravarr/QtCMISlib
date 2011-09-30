SUBDIRS = qtcmislib demo

.PHONY: subdirs $(SUBDIRS)
     
subdirs: $(SUBDIRS)

$(SUBDIRS):
	 $(MAKE) -C $@

demo: qtcmislib
