# pg_inet_addr extension

EXTENSION = pg_inet_addr
MODULES = pg_inet_addr
DATA = pg_inet_addr--1.0.sql
#DOCS = README.md

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
