
SUBDIRS=

CXXFLAGS += -Wall 
CPPFLAGS += -D__STDC_FORMAT_MACROS #-DNEED_DEBUG

INCLUDE = $(top_srcdir)/common
bin_PROGRAMS=worker_server
worker_server_SOURCES=http_server.hpp http_server.cpp \
              task_base.hpp task_base.cpp \
              worker_store.cpp \
              worker.hpp linked_list.hpp \
              socket_handle.hpp wait_list.hpp \
              config_map.hpp configuration.hpp configuration.cpp \
              worker_loader.cpp \
              request_type.cpp \
              http.cpp \
              base64.cpp\
			  util_common.cpp\
			  output_package.cpp\
              main.cpp

worker_server_LDADD=-lpthread -lACE -lglog -lgflags $(top_srcdir)/common/json/libjson.la



