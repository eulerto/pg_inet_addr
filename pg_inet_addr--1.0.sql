-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_inet_addr" to load this file. \quit

CREATE FUNCTION pg_catalog.pg_inet_addr(OUT interface text,OUT address text, OUT netmask text, OUT family text)
RETURNS setof record
AS 'MODULE_PATHNAME', 'pg_inet_addr'
LANGUAGE C VOLATILE STRICT;

REVOKE EXECUTE ON FUNCTION pg_catalog.pg_inet_addr() FROM PUBLIC;
