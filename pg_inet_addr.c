#include "postgres.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>

#include "funcapi.h"
#include "fmgr.h"
#include "miscadmin.h"
#include "utils/builtins.h"
//#include "utils/inet.h"
#include "utils/tuplestore.h"

PG_MODULE_MAGIC;

Datum pg_inet_addr(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(pg_inet_addr);

Datum
pg_inet_addr(PG_FUNCTION_ARGS)
{
#define PG_INET_ADDR_COLS	4
	struct	ifaddrs	*ifaddr, *ifa;

	ReturnSetInfo *rsinfo = (ReturnSetInfo *) fcinfo->resultinfo;
	TupleDesc	tupdesc;
	Tuplestorestate *tupstore;
	MemoryContext per_query_ctx;
	MemoryContext oldcontext;

	/* check to see if caller supports us returning a tuplestore */
	if (rsinfo == NULL || !IsA(rsinfo, ReturnSetInfo))
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("set-valued function called in context that cannot accept a set")));
	if (!(rsinfo->allowedModes & SFRM_Materialize))
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("materialize mode required, but it is not " \
						"allowed in this context")));

	/* Build a tuple descriptor for our result type */
	if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
		elog(ERROR, "return type must be a row type");

	per_query_ctx = rsinfo->econtext->ecxt_per_query_memory;
	oldcontext = MemoryContextSwitchTo(per_query_ctx);

	tupstore = tuplestore_begin_heap(true, false, work_mem);
	rsinfo->returnMode = SFRM_Materialize;
	rsinfo->setResult = tupstore;
	rsinfo->setDesc = tupdesc;

	MemoryContextSwitchTo(oldcontext);

	if (getifaddrs(&ifaddr) == -1)
		elog(ERROR, "getifaddrs failed: %s", strerror(errno));

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		/* for each row */
		Datum		values[PG_INET_ADDR_COLS];
		bool		nulls[PG_INET_ADDR_COLS];
		int			h, n;
		int			family;
		char		address[NI_MAXHOST];
		char		netmask[INET6_ADDRSTRLEN];

		if (ifa->ifa_addr == NULL)
			continue;

		address[0] = '\0';
		netmask[0] = '\0';

		MemSet(values, 0, sizeof(values));
		MemSet(nulls, 0, sizeof(nulls));

		family = ifa->ifa_addr->sa_family;

		if (family == AF_INET || family == AF_INET6)
		{
			/* host address */
			h = getnameinfo(ifa->ifa_addr, (family == AF_INET) ?
							sizeof(struct sockaddr_in) :
							sizeof(struct sockaddr_in6),
							address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (h != 0)
				elog(ERROR, "address: getnameinfo failed: %s", gai_strerror(errno));

			if (ifa->ifa_netmask == NULL)
				elog(DEBUG1, "%s: netmask is null", address);

			/* host netmask */
			n = getnameinfo(ifa->ifa_netmask, (family == AF_INET) ?
							sizeof(struct sockaddr_in) :
							sizeof(struct sockaddr_in6),
							netmask, INET6_ADDRSTRLEN, NULL, 0, NI_NUMERICHOST);
			if (n != 0)
				elog(ERROR, "netmask: getnameinfo failed: %s", gai_strerror(errno));

			elog(DEBUG1, "interface: %s ; address: %s ; netmask: %s ; family; %s", ifa->ifa_name, address, netmask, (family == AF_INET) ? "IPv4" : "IPv6");
			
			values[0] = CStringGetTextDatum(ifa->ifa_name);
			if (h == 0)
//				values[1] = DirectFunctionCall1(inet_in, CStringGetDatum(address));
				values[1] = CStringGetTextDatum(address);
			else
				nulls[1] = true;
			if (n == 0)
//				values[2] = DirectFunctionCall1(inet_in, CStringGetDatum(netmask));
				values[2] = CStringGetTextDatum(netmask);
			else
				nulls[2] = true;
			if (family == AF_INET)
				values[3] = CStringGetTextDatum("IPv4");
			else if (family == AF_INET6)
				values[3] = CStringGetTextDatum("IPv6");

			tuplestore_putvalues(tupstore, tupdesc, values, nulls);
		}
	}

	freeifaddrs(ifaddr);

	/* clean up and return the tuplestore */
	tuplestore_donestoring(tupstore);

	return (Datum) 0;
}
