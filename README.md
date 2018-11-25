pg_inet_addr
============

**pg_inet_addr** is an extension that lists interface addresses. The addresses
will be IPv4 and IPv6 addresses assigned to interfaces.

PostgreSQL local connections does not list IP addresses via `inet_server_addr`.
Function `pg_inet_addr` provides all interface addresses even if you are not
using a remote connection.

Installing
----------

Build and install is done using PGXS. PostgreSQL 9.1+ installed (including the
header files). As long as `pg_config` is available in the path, build and
install using:

```
$ make
$ make install
```

Usage
-----

```
mydb=# CREATE EXTENSION pg_inet_addr;
CREATE EXTENSION
mydb=# SELECT * FROM pg_inet_addr();
 interface |                address                 |                 netmask                 | family 
-----------+----------------------------------------+-----------------------------------------+--------
 lo        | 127.0.0.1                              | 255.0.0.0                               | IPv4
 wlp5s0    | 10.5.8.10                              | 255.255.255.0                           | IPv4
 lo        | ::1                                    | ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff | IPv6
 wlp5s0    | 2804:14d:3c82:41cb::4                  | ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff | IPv6
 wlp5s0    | 2804:14d:3c82:41cb:2944:ed92:9c61:e97f | ffff:ffff:ffff:ffff::                   | IPv6
 wlp5s0    | 2804:14d:3c82:41cb:5ec9:d3ff:fe98:24f7 | ffff:ffff:ffff:ffff::                   | IPv6
 wlp5s0    | fe80::5ec9:d3ff:fe98:24f7%wlp5s0       | ffff:ffff:ffff:ffff::                   | IPv6
(7 rows)

mydb=# SELECT interface, address FROM pg_inet_addr() WHERE interface <> 'lo';
 interface |                address                 
-----------+----------------------------------------
 wlp5s0    | 10.5.8.10
 wlp5s0    | 2804:14d:3c82:41cb::4
 wlp5s0    | 2804:14d:3c82:41cb:2944:ed92:9c61:e97f
 wlp5s0    | 2804:14d:3c82:41cb:5ec9:d3ff:fe98:24f7
 wlp5s0    | fe80::5ec9:d3ff:fe98:24f7%wlp5s0
(5 rows)

mydb=# SELECT interface, address, text(address::inet)  FROM pg_inet_addr() WHERE family = 'IPv4';
 interface |  address  |     text     
-----------+-----------+--------------
 lo        | 127.0.0.1 | 127.0.0.1/32
 wlp5s0    | 10.3.2.10 | 10.3.2.10/32
(2 rows)
```

License
-------

> Copyright © 2018 Euler Taveira de Oliveira
> All rights reserved.

> Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

> Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer;
> Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution;
> Neither the name of the Euler Taveira de Oliveira nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

